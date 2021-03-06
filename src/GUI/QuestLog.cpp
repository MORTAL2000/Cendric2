#include "GUI/QuestLog.h"
#include "GUI/QuestDescriptionWindow.h"

// <<< QUEST LOG >>>

float QuestLog::WIDTH = (WINDOW_WIDTH - GUIConstants::LEFT - 20.f) / 3.f;
float QuestLog::HEIGHT = WINDOW_HEIGHT - 250.f;

QuestLog::QuestLog(CharacterCore* core) {
	m_core = core;

	init();
}

void QuestLog::init() {
	// init window
	sf::FloatRect box(GUIConstants::LEFT, GUIConstants::TOP, WIDTH, HEIGHT);
	m_window = new Window(box,
		WindowOrnamentStyle::FANCY,
		GUIConstants::MAIN_COLOR,
		GUIConstants::BACK_COLOR,
		GUIConstants::ORNAMENT_COLOR);

	// init text
	m_title.setPosition(sf::Vector2f(GUIConstants::LEFT + GUIConstants::TEXT_OFFSET, GUIConstants::TOP + GUIConstants::TEXT_OFFSET));
	m_title.setColor(COLOR_WHITE);
	m_title.setCharacterSize(GUIConstants::CHARACTER_SIZE_M);
	m_title.setString(g_textProvider->getText("Journal"));
	m_title.setPosition(
		m_window->getPosition().x +
		WIDTH / 2 -
		m_title.getLocalBounds().width / 2, m_title.getPosition().y);

	// fill the helper map
	m_stateMap.insert({
		{ QuestState::Started, &m_startedQuests },
		{ QuestState::Completed, &m_completedQuests },
		{ QuestState::Failed, &m_failedQuests },
	});

	// tabbar
	int nTabs = 3;
	float width = nTabs * BUTTON_SIZE.x;
	float height = BUTTON_SIZE.y;
	float x = GUIConstants::LEFT + 0.5f * (QuestLog::WIDTH - width);
	float y = GUIConstants::TOP + 2 * GUIConstants::TEXT_OFFSET + GUIConstants::CHARACTER_SIZE_M;
	
	m_tabBar = new TabBar(sf::FloatRect(x, y, width, height), nTabs);
	for (int i = 0; i < nTabs; ++i) {
		m_tabBar->getTabButton(i)->setText(EnumNames::getQuestStateName((QuestState)(i+1)));
		m_tabBar->getTabButton(i)->setCharacterSize(GUIConstants::CHARACTER_SIZE_S);
	}

	delete m_descriptionWindow;
	m_descriptionWindow = new QuestDescriptionWindow(m_core);

	selectTab(QuestState::Started);
	reload();
}

QuestLog::~QuestLog() {
	delete m_window;
	delete m_descriptionWindow;
	delete m_tabBar;
	clearAllEntries();
}

void QuestLog::clearAllEntries() {
	m_completedQuests.clear();
	m_failedQuests.clear();
	m_startedQuests.clear();
	m_selectedEntry = nullptr;
}

void QuestLog::update(const sf::Time& frameTime) {
	if (!m_isVisible) return;

	// check whether an entry was selected
	for (auto& it : *(m_stateMap[m_currentTab])) {
		it.update(frameTime);
		if (it.isClicked()) {
			selectEntry(&it);
			return;
		}
	}

	m_tabBar->update(frameTime);
	int activeIndex = m_tabBar->getActiveTabIndex();
	QuestState state = (QuestState)(activeIndex + 1);
	if (m_tabBar->getTabButton(activeIndex)->isClicked() && m_currentTab != state) {
		selectTab(state);
	}
}

void QuestLog::selectEntry(QuestEntry* entry) {
	if (entry == nullptr) return;
	if (entry == m_selectedEntry) return;
	if (m_selectedEntry != nullptr) {
		m_selectedEntry->deselect();
	}
	m_selectedEntry = entry;
	m_selectedEntry->select();
	showDescription(m_selectedEntry->getQuestID());
	m_selectedQuestID = m_selectedEntry->getQuestID();
}

bool QuestLog::isVisible() const {
	return m_isVisible;
}

void QuestLog::render(sf::RenderTarget& target) {
	if (!m_isVisible) return;

	m_window->render(target);
	target.draw(m_title);
	for (auto& it : *(m_stateMap[m_currentTab])) {
		it.render(target);
		// it.renderAfterForeground(target); // uncomment for debug box
	}

	m_tabBar->render(target);

	m_descriptionWindow->render(target);
}

void QuestLog::showDescription(const std::string& questID) {
	m_descriptionWindow->reload(questID);
	m_descriptionWindow->show();
}

void QuestLog::hideDescription() {
	m_descriptionWindow->hide();
	m_selectedQuestID = "";
}

void QuestLog::selectTab(QuestState state) {
	hideDescription();
	if (m_selectedEntry != nullptr) {
		m_selectedEntry->deselect();
		m_selectedEntry = nullptr;
	}
	m_currentTab = state;
}

void QuestLog::reload() {
	clearAllEntries();

	for (auto& it : m_core->getData().questStates) {
		if (m_stateMap[it.second] == nullptr) continue;
		m_stateMap[it.second]->push_back(QuestEntry(it.first));
		if (it.first.compare(m_selectedQuestID) == 0 && m_currentTab != it.second) {
			// assure that an item that is not in the current tab can never be selected
			hideDescription();
		}
	}

	// calculate positions
	for (auto& it : m_stateMap) {
		float xOffset = GUIConstants::LEFT + GUIConstants::TEXT_OFFSET;
		float yOffset = GUIConstants::TOP + 3 * GUIConstants::TEXT_OFFSET + GUIConstants::CHARACTER_SIZE_M + BUTTON_SIZE.y;

		for (auto& it2 : *it.second) {
			it2.deselect();
			it2.setPosition(sf::Vector2f(xOffset, yOffset));
			yOffset += 1.5f * GUIConstants::CHARACTER_SIZE_M;
			if (it2.getQuestID().compare(m_selectedQuestID) == 0) {
				selectEntry(&it2);
			}
		}
	}
}

void QuestLog::show() {
	m_isVisible = true;
}

void QuestLog::hide() {
	m_isVisible = false;
}

// <<< QUEST ENTRY >>>

QuestEntry::QuestEntry(const std::string& questID) {
	m_questID = questID;
	m_name.setCharacterSize(GUIConstants::CHARACTER_SIZE_M);
	m_name.setColor(COLOR_WHITE);
	m_name.setString(">  " + g_textProvider->getText(questID, "quest"));
	setBoundingBox(sf::FloatRect(0.f, 0.f, m_name.getLocalBounds().width, m_name.getLocalBounds().height));
	setInputInDefaultView(true);
}

const std::string& QuestEntry::getQuestID() const {
	return m_questID;
}

void QuestEntry::setPosition(const sf::Vector2f& pos) {
	GameObject::setPosition(pos);
	m_name.setPosition(pos);
}

void QuestEntry::render(sf::RenderTarget& renderTarget) {
	renderTarget.draw(m_name);
}

void QuestEntry::onLeftJustPressed() {
	g_inputController->lockAction();
	m_isClicked = true;
}

bool QuestEntry::isClicked() {
	bool wasClicked = m_isClicked;
	m_isClicked = false;
	return wasClicked;
}

void QuestEntry::select() {
	m_name.setColor(COLOR_WHITE);
	m_isSelected = true;
}

GameObjectType QuestEntry::getConfiguredType() const {
	return GameObjectType::_Interface;
}

void QuestEntry::deselect() {
	m_name.setColor(COLOR_GREY);
	m_isSelected = false;
}

bool QuestEntry::isSelected() const {
	return m_isSelected;
}


