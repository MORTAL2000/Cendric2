#include "Level/LevelEquipment.h"
#include "Level/LevelMainCharacter.h"
#include "GameObjectComponents/LightComponent.h"

LevelEquipment::LevelEquipment(LevelMainCharacter* mainChar) : AnimatedGameObject() { 
	m_mainChar = mainChar; 
	m_screen = mainChar->getScreen();
}

LevelEquipment::~LevelEquipment() {
	g_resourceManager->deleteResource(m_texturePath);
}

void LevelEquipment::calculatePositionAccordingToMainChar(sf::Vector2f& position) const {
	sf::Vector2f mainCharPosition(m_mainChar->getPosition().x + (m_mainChar->getBoundingBox()->width / 2), m_mainChar->getPosition().y);
	sf::Vector2f offset(-60.f, -30.f);
	if (!m_mainChar->isFacingRight())
		offset.x = -offset.x - getBoundingBox()->width;
	if (m_mainChar->isUpsideDown())
		offset.y = m_mainChar->getBoundingBox()->height - offset.y - getBoundingBox()->height;

	position.x = (mainCharPosition + offset).x;
	position.y = (mainCharPosition + offset).y;
}

void LevelEquipment::setPosition(const sf::Vector2f& position) {
	if (m_lightComponent != nullptr) {
		m_lightComponent->flipLightOffsetX(!m_isFacingRight);
	}
	AnimatedGameObject::setPosition(position);
}

void LevelEquipment::update(const sf::Time& frameTime) {
	GameObjectState newState = m_mainChar->getState();
	if (newState == GameObjectState::Dead) {
		setDisposed();
		return;
	}
	bool newFacingRight = m_mainChar->isFacingRight();
	if (m_state != newState || newFacingRight != m_isFacingRight) {
		m_state = newState;
		m_isFacingRight = newFacingRight;
		setCurrentAnimation(getAnimation(m_state), !m_isFacingRight);
	}
	if (m_mainChar->isUpsideDown() != m_animatedSprite.isFlippedY()) {
		m_animatedSprite.setFlippedY(m_mainChar->isUpsideDown());
	}

	sf::Vector2f newPosition;
	calculatePositionAccordingToMainChar(newPosition);
	setPosition(newPosition);
	AnimatedGameObject::update(frameTime);
	if (m_isCopyingMainCharColor)
		setSpriteColor(m_mainChar->getCurrentSpriteColor(), sf::milliseconds(1));
}

void LevelEquipment::loadEquipment() {
	sf::Vector2f position;
	calculatePositionAccordingToMainChar(position);
	setPosition(position);
}

void LevelEquipment::setTexturePath(const std::string& texturePath) {
	m_texturePath = texturePath;
}

void LevelEquipment::setLightComponent(const LightData& data) {
	m_lightComponent = new LightComponent(data, this);
	addComponent(m_lightComponent);
}

GameObjectType LevelEquipment::getConfiguredType() const {
	return GameObjectType::_LevelEquipment;
}

void LevelEquipment::setCopyingMainCharColor(bool value) {
	m_isCopyingMainCharColor = value;
}
