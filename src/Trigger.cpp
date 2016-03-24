#include "Trigger.h"
#include "Screens/WorldScreen.h"

Trigger::Trigger(WorldScreen* screen, const TriggerData& data) {
	m_worldScreen = screen;
	m_mainChar = screen->getMainCharacter();
	m_data = data;

	setBoundingBox(data.triggerRect);
	setDebugBoundingBox(sf::Color::Magenta);
}

void Trigger::update(const sf::Time& frameTime) {
	if (m_isOnTrigger && !m_mainChar->getBoundingBox()->intersects(m_data.triggerRect)) {
		m_isOnTrigger = false;
	}
	if (m_isOnTrigger) return;
	if (!m_data.isTriggerable) return;
	if (m_data.isKeyGuarded && !g_inputController->isKeyJustPressed(Key::Up)) {
		return;
	}
	if (m_mainChar->getBoundingBox()->intersects(m_data.triggerRect)) {
		for (auto& content : m_data.content) {
			TriggerContent::executeTrigger(content, m_worldScreen);
		}
		if (!m_data.isPersistent) {
			m_worldScreen->getCharacterCore()->setTriggerTriggered(m_data.worldID, m_data.objectID);
		}
		setDisposed();
	}
}

GameObjectType Trigger::getConfiguredType() const {
	return GameObjectType::_Overlay;
}

TriggerData& Trigger::getData() {
	return m_data;
}