#include "GUI/QuestDescriptionWindow.h"

using namespace std;

const float QuestDescriptionWindow::WIDTH = 340.f;

QuestDescriptionWindow::QuestDescriptionWindow(const CharacterCore* core) : Window(
	sf::FloatRect(0.f, 0.f, WIDTH, WIDTH),
	WindowOrnamentStyle::FANCY,
	GUIConstants::MAIN_COLOR,
	GUIConstants::BACK_COLOR,
	GUIConstants::ORNAMENT_COLOR) {
	m_core = core;

	m_titleText.setCharacterSize(GUIConstants::CHARACTER_SIZE_M);
	m_titleText.setColor(COLOR_WHITE);

	m_descriptionText.setCharacterSize(GUIConstants::CHARACTER_SIZE_S);
	m_descriptionText.setColor(COLOR_LIGHT_GREY);

	m_stateText.setCharacterSize(GUIConstants::CHARACTER_SIZE_S);
	m_stateText.setColor(COLOR_LIGHT_PURPLE);

	setPosition(sf::Vector2f(GUIConstants::LEFT + GUIConstants::TEXT_OFFSET + QuestLog::WIDTH, GUIConstants::TOP));
}

void QuestDescriptionWindow::reload(const std::string& questID) {
	m_collectiblesTexts.clear();
	m_conditionTexts.clear();
	m_targetsTexts.clear();
	const QuestData* data = m_core->getQuestData(questID);
	if (data == nullptr) {
		m_titleText.setString(g_textProvider->getText("Unknown"));
		m_descriptionText.setString(g_textProvider->getText("Unknown"));
		setPosition(getPosition());
		return;
	}

	m_titleText.setString(g_textProvider->getText(data->title, "quest"));
	std::string description = g_textProvider->getCroppedText(
		data->title, "quest_desc",
		GUIConstants::CHARACTER_SIZE_S,
		static_cast<int>(WIDTH - 2 * GUIConstants::TEXT_OFFSET));
	

	if (m_core->getData().questDescriptionProgress.find(questID) != m_core->getData().questDescriptionProgress.end()) {
		const std::set<int>& unlockedDescriptions = m_core->getData().questDescriptionProgress.at(questID);
		for (auto& it : unlockedDescriptions) {
			description.append("\n\n");
			description.append(g_textProvider->getCroppedText(
				data->title, "quest_desc_" + std::to_string(it),
				GUIConstants::CHARACTER_SIZE_S,
				static_cast<int>(WIDTH - 2 * GUIConstants::TEXT_OFFSET)));
		}
	}

	m_descriptionText.setString(description);

	QuestState currentState = m_core->getQuestState(questID);
	if (currentState == QuestState::Completed) {
		m_stateText.setString(g_textProvider->getText(EnumNames::getQuestStateName(currentState)));
		m_stateText.setColor(COLOR_GOOD);
	}
	else if (currentState == QuestState::Failed) {
		m_stateText.setString(g_textProvider->getText(EnumNames::getQuestStateName(currentState)));
		m_stateText.setColor(COLOR_BAD);
	}
	else {
		m_stateText.setString("");
	}

	for (auto& it : data->targets) {
		std::string target = "";
		target.append(g_textProvider->getText(it.first, "enemy"));
		target.append(": ");

		int goal = it.second;
		int progress = (m_core->getQuestState(questID) == QuestState::Completed) ?
		goal :
			 m_core->getNumberOfTargetsKilled(questID, it.first);

		target.append(to_string(progress) + "/" + to_string(goal));

		BitmapText targetText;
		targetText.setCharacterSize(GUIConstants::CHARACTER_SIZE_S);
		targetText.setString(target);
		targetText.setColor(progress == 0 ? COLOR_BAD : progress >= goal ? COLOR_GOOD : COLOR_NEUTRAL);
		m_targetsTexts.push_back(targetText);
	}

	for (auto& it : data->collectibles) {
		std::string collectible = "";
		collectible.append(g_textProvider->getText(it.first, "item"));
		collectible.append(": ");
		int progress = 0;
		int goal = it.second;
		if (m_core->getQuestState(questID) == QuestState::Completed) {
			progress = goal;
		}
		else if (m_core->getData().items.find(it.first) != m_core->getData().items.end()) {
			progress = m_core->getData().items.at(it.first);
		}
		collectible.append(to_string(progress) + "/" + to_string(goal));

		BitmapText collectibleText;
		collectibleText.setCharacterSize(GUIConstants::CHARACTER_SIZE_S);
		collectibleText.setString(collectible);
		collectibleText.setColor(progress == 0 ? COLOR_BAD : progress >= goal ? COLOR_GOOD : COLOR_NEUTRAL);
		m_collectiblesTexts.push_back(collectibleText);
	}

	for (auto& it : data->conditions) {
		BitmapText conditionText;

		std::string condition = "";
		condition.append(g_textProvider->getText(it, questID));
		condition.append(": ");

		if ((m_core->getQuestState(questID) == QuestState::Completed)
			||
			(m_core->getData().questConditionProgress.find(questID) != m_core->getData().questConditionProgress.end() &&
			m_core->getData().questConditionProgress.at(questID).find(it) != m_core->getData().questConditionProgress.at(questID).end())) {
			condition.append(g_textProvider->getText("Done"));
			conditionText.setColor(COLOR_GOOD);
		}
		else {
			condition.append(g_textProvider->getText("Pending"));
			conditionText.setColor(COLOR_BAD);
		}

		conditionText.setCharacterSize(GUIConstants::CHARACTER_SIZE_S);
		conditionText.setString(condition);
		m_conditionTexts.push_back(conditionText);
	}

	setPosition(getPosition());
}

void QuestDescriptionWindow::show() {
	m_isVisible = true;
}

void QuestDescriptionWindow::hide() {
	m_isVisible = false;
}

void QuestDescriptionWindow::setPosition(const sf::Vector2f& position) {
	Window::setPosition(position);
	sf::Vector2f pos(position);
	pos.y += GUIConstants::TEXT_OFFSET;
	pos.x += GUIConstants::TEXT_OFFSET;

	m_titleText.setPosition(position.x + ((WIDTH - m_titleText.getLocalBounds().width) / 2.f), pos.y);
	pos.y += 2 * m_titleText.getLocalBounds().height;

	m_descriptionText.setPosition(pos);

	pos.y += GUIConstants::TEXT_OFFSET + m_descriptionText.getLocalBounds().height;

	if (!m_stateText.getString().empty()) {
		m_stateText.setPosition(pos.x, pos.y);
		pos.y += GUIConstants::TEXT_OFFSET + m_stateText.getLocalBounds().height;
	}

	for (auto& it : m_targetsTexts) {
		it.setPosition(pos.x, pos.y);
		pos.y += 2 * it.getBounds().height;
	}
	for (auto& it : m_collectiblesTexts) {
		it.setPosition(pos.x, pos.y);
		pos.y += 2 * it.getBounds().height;
	}
	for (auto& it : m_conditionTexts) {
		it.setPosition(pos.x, pos.y);
		pos.y += 2 * it.getBounds().height;
	}
	pos.y += GUIConstants::TEXT_OFFSET;

	setHeight(pos.y - position.y);
}

void QuestDescriptionWindow::render(sf::RenderTarget& renderTarget) {
	if (!m_isVisible) return;
	Window::render(renderTarget);
	renderTarget.draw(m_titleText);
	renderTarget.draw(m_descriptionText);
	renderTarget.draw(m_stateText);
	for (auto& it : m_targetsTexts) {
		renderTarget.draw(it);
	}
	for (auto& it : m_collectiblesTexts) {
		renderTarget.draw(it);
	}
	for (auto& it : m_conditionTexts) {
		renderTarget.draw(it);
	}
}