#include "Screen.h"
#include "GUI/Button.h"

using namespace std;

#define VIEW_MARGIN 50.f;

inline bool isInsideView(const sf::View& targetView, const sf::FloatRect& boundingBox) {
	sf::FloatRect view(targetView.getCenter().x - targetView.getSize().x / 2.f,
		targetView.getCenter().y - targetView.getSize().y / 2.f,
		targetView.getSize().x,
		targetView.getSize().y);

	view.left -= VIEW_MARGIN;
	view.top -= VIEW_MARGIN;
	view.width += 2 * VIEW_MARGIN;
	view.height += 2 * VIEW_MARGIN;
	return view.intersects(boundingBox);
}

Screen::Screen(CharacterCore* core) {
	m_characterCore = core;

	m_objects = vector<vector<GameObject*>>();
	for (GameObjectType t = GameObjectType::_Undefined; t < GameObjectType::_MAX; t = GameObjectType(t + 1)) {
		vector<GameObject*> newVector;
		m_objects.push_back(newVector);
	}
}

Screen::~Screen() {
	for (auto& overlay : m_overlayQueue) {
		delete overlay;
	}
	m_overlayQueue.clear();
}

void Screen::addObject(GameObject* object) {
	m_toAdd.push_back(object);
	object->setScreen(this);
}

void Screen::update(const sf::Time& frameTime) {
	for (auto& obj : m_toAdd) {
		m_objects[obj->getConfiguredType()].push_back(obj);
	}
	m_toAdd.clear();
	execUpdate(frameTime);
	deleteDisposedObjects();
}

vector<GameObject*>* Screen::getObjects(GameObjectType type) {
	return &m_objects[type];
}

void Screen::onEnter(Screen* previousScreen) {
	execOnEnter(previousScreen);
	for (auto& obj : m_toAdd) {
		m_objects[obj->getConfiguredType()].push_back(obj);
	}
	m_toAdd.clear();
}

void Screen::execOnEnter(const Screen* previousScreen) {
	// nop
}

void Screen::onExit(Screen* nextScreen) {
	deleteAllObjects();
	execOnExit(nextScreen);
	if (nextScreen != nullptr) {
		for (auto& overlay : m_overlayQueue) {
			nextScreen->addScreenOverlay(overlay);
		}
	}
	m_overlayQueue.clear();
}

void Screen::execOnExit(const Screen* nextScreen) {
	// nop
}

void Screen::deleteDisposedObjects() {
	for (GameObjectType t = GameObjectType::_Undefined; t < GameObjectType::_MAX; t = GameObjectType(t + 1)) {
		for (std::vector<GameObject*>::iterator it = m_objects[t].begin(); it != m_objects[t].end(); /*don't increment here*/) {
			if ((*it)->isDisposed()) {
				delete (*it);
				it = m_objects[t].erase(it);
			}
			else {
				it++;
			}
		}
	}
}

void Screen::setAllButtonsEnabled(bool value) {
	vector<GameObject*>* buttons = getObjects(GameObjectType::_Button);
	for (auto it : *buttons) {
		Button* button = dynamic_cast<Button*>(it);
		if (button != nullptr) {
			button->setEnabled(value);
		}
	}
}

void Screen::deleteAllObjects() {
	for (GameObjectType t = GameObjectType::_Undefined; t < GameObjectType::_MAX; t = GameObjectType(t + 1)) {
		for (std::vector<GameObject*>::iterator it = m_objects[t].begin(); it != m_objects[t].end(); /*don't increment here*/) {
			delete (*it);
			it = m_objects[t].erase(it);
		}
	}
}

void Screen::deleteObjects(GameObjectType type) {
	for (std::vector<GameObject*>::iterator it = m_objects[type].begin(); it != m_objects[type].end(); /*don't increment here*/) {
		delete (*it);
		it = m_objects[type].erase(it);
	}
}

void Screen::updateObjectsFirst(GameObjectType type, const sf::Time& frameTime) {
	for (auto& it : m_objects[type]) {
		it->updateFirst(frameTime);
	}
}

void Screen::updateObjects(GameObjectType type, const sf::Time& frameTime) {
	for (auto& it : m_objects[type]) {
		if (it->isUpdatable())
			it->update(frameTime);
	}
}

inline bool compareYCoordAsc(const GameObject* go1, const GameObject* go2) { 
	return (go1->getPosition().y + go1->getBoundingBox()->height < 
		go2->getPosition().y + go2->getBoundingBox()->height);
}

inline bool compareYCoordDesc(const GameObject* go1, const GameObject* go2) {
	return (go1->getPosition().y + go1->getBoundingBox()->height >
		go2->getPosition().y + go2->getBoundingBox()->height);
}

void Screen::depthSortObjects(GameObjectType type, bool asc) {
	if (asc)
		std::sort(m_objects[type].begin(), m_objects[type].end(), compareYCoordAsc);
	else
		std::sort(m_objects[type].begin(), m_objects[type].end(), compareYCoordDesc);
}

void Screen::renderObjects(GameObjectType type, sf::RenderTarget& renderTarget) {
	for (auto& it : m_objects[type]) {
		it->setViewable(isInsideView(renderTarget.getView(), *(it->getBoundingBox())));
		if (it->isViewable())
			it->render(renderTarget);
	}
}

void Screen::renderObjectsAfterForeground(GameObjectType type, sf::RenderTarget& renderTarget) {
	for (auto &it : m_objects[type]) {
		if (it->isViewable())
			it->renderAfterForeground(renderTarget);
	}
}

const BitmapText* Screen::getTooltipText() const {
	return &m_tooltipText;
}

void Screen::setTooltipTextRaw(const string& text, const sf::Color& color, bool isOverride) {
	if (m_tooltipTime > sf::Time::Zero && !isOverride) {
		// another text is still displaying
		return;
	}
	
	m_tooltipText = BitmapText(text);
	m_tooltipText.setTextStyle(TextStyle::Shadowed);
	m_tooltipText.setCharacterSize(12);
	m_tooltipText.setPosition(std::max(0.f, (WINDOW_WIDTH - m_tooltipText.getLocalBounds().width) / 2.f), m_isTooltipTop ? 10.f : WINDOW_HEIGHT - m_tooltipText.getLocalBounds().height - 10.f);
	m_tooltipText.setColor(color);
	m_tooltipTime = sf::seconds(1.f + 0.06f * static_cast<float>(text.length()));
}

void Screen::setTooltipText(const string& textKey, const sf::Color& color, bool isOverride) {
	setTooltipTextRaw(g_textProvider->getText(textKey), color, isOverride);
}

void Screen::renderTooltipText(sf::RenderTarget& target) const {
	sf::View oldView = target.getView();
	target.setView(target.getDefaultView());
	target.draw(m_tooltipText);
	target.setView(oldView);
}

void Screen::setTooltipPositionTop(bool top) {
	m_isTooltipTop = top;
}

CharacterCore* Screen::getCharacterCore() const {
	return m_characterCore;
}

void Screen::updateTooltipText(const sf::Time& frameTime) {
	if (m_tooltipTime > sf::Time::Zero) {
		m_tooltipTime -= frameTime;
		if (m_tooltipTime <= sf::Time::Zero) // yes, sf::Time can be negative.
		{
			// reset tooltip text
			m_tooltipText = BitmapText();
			m_tooltipTime = sf::Time::Zero;
		}
	}
}

bool Screen::isQuitRequested() const {
	return m_requestQuit;
}

Screen* Screen::getNextScreen() const {
	return m_nextScreen;
}

void Screen::setNextScreen(Screen* nextScreen) {
	if (nextScreen == this) return;
	delete m_nextScreen;
	m_nextScreen = nextScreen;
}

void Screen::addScreenOverlay(ScreenOverlay* overlay) {
	if (overlay == nullptr) return;
	m_overlayQueue.push_back(overlay);
}