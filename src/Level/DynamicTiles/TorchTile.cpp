#include "Level/DynamicTiles/TorchTile.h"
#include "GameObjectComponents/LightComponent.h"
#include "Spells/Spell.h"
#include "Registrar.h"

REGISTER_LEVEL_DYNAMIC_TILE(LevelDynamicTileID::Torch, TorchTile)

TorchTile::TorchTile(LevelScreen* levelScreen) : LevelDynamicTile(levelScreen) {
	m_lightComponent = new LightComponent(LightData(sf::Vector2f(), sf::Vector2f(140.f, 200.f)), this);
	addComponent(m_lightComponent);
}

void TorchTile::init() {
	setSpriteOffset(sf::Vector2f(-10.f, -TILE_SIZE_F / 2));
	setBoundingBox(sf::FloatRect(0.f, 0.f, TILE_SIZE_F / 2.f, TILE_SIZE_F));
}

void TorchTile::loadAnimation(int skinNr) {
	int textureHeight = 2 * TILE_SIZE;

	Animation* idleAnimation = new Animation(sf::seconds(10.f));
	idleAnimation->setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_tile_torch));
	idleAnimation->addFrame(sf::IntRect(0, 0, TILE_SIZE, 2 * TILE_SIZE));

	addAnimation(GameObjectState::Idle, idleAnimation);

	Animation* burningAnimation = new Animation(sf::seconds(0.12f));
	burningAnimation->setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_tile_torch));
	for (int i = 1; i < 9; i++) {
		burningAnimation->addFrame(sf::IntRect(TILE_SIZE * i, (skinNr - 1) * textureHeight, TILE_SIZE, textureHeight));
	}

	addAnimation(GameObjectState::Burning, burningAnimation);

	// initial values
	m_state = GameObjectState::Burning;
	setCurrentAnimation(getAnimation(m_state), false);
	playCurrentAnimation(true);
}

void TorchTile::onHit(Spell* spell) {
	switch (spell->getSpellID()) {
	case SpellID::Chop:
		spell->setDisposed();
		setDisposed();
		break;
	case SpellID::IceBall:
		if (m_state == GameObjectState::Burning) {
			setState(GameObjectState::Idle);
			m_lightComponent->setVisible(false);
			spell->setDisposed();
		}
		break;
	case SpellID::WindGust:
		if (m_state == GameObjectState::Burning) {
			setState(GameObjectState::Idle);
			m_lightComponent->setVisible(false);
		}
		break;
	case SpellID::FireBall:
		if (m_state == GameObjectState::Idle) {
			setState(GameObjectState::Burning);
			m_lightComponent->setVisible(true);
			spell->setDisposed();
		}
		break;
	default:
		break;
	}
}
