#include "Map/DynamicTiles/CookingTile.h"
#include "Map/Map.h"
#include "Screens/MapScreen.h"
#include "Registrar.h"
#include "GameObjectComponents/LightComponent.h"
#include "GameObjectComponents/TooltipComponent.h"

REGISTER_MAP_DYNAMIC_TILE(MapDynamicTileID::Cooking, CookingTile)

CookingTile::CookingTile(MapScreen* mapScreen) : MapDynamicTile(mapScreen) {
	addComponent(new LightComponent(LightData(
		sf::Vector2f(TILE_SIZE_F * 0.5f, TILE_SIZE_F * 0.5f),
		sf::Vector2f(100.f, 100.f)), this));
	addComponent(new TooltipComponent(g_textProvider->getText("Fireplace"), this));
}

void CookingTile::init() {
	setBoundingBox(sf::FloatRect(0.f, 0.f, 
		TILE_SIZE_F, 
		TILE_SIZE_F));
	setSpriteOffset(sf::Vector2f(0.f, -50.f));
}

void CookingTile::loadAnimation(int skinNr) {
	int textureHeight = 2 * TILE_SIZE;

	Animation* burningAnimation = new Animation(sf::seconds(0.15f));
	burningAnimation->setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_tile_cooking));
	for (int i = 0; i < 4; ++i) {
		burningAnimation->addFrame(sf::IntRect(TILE_SIZE * i, (skinNr - 1) * textureHeight, TILE_SIZE, textureHeight));
	}

	addAnimation(GameObjectState::Burning, burningAnimation);

	// initial values
	setState(GameObjectState::Burning);
	playCurrentAnimation(true);
}

void CookingTile::onRightClick() {
	// check if this tile is in range
	sf::Vector2f dist = m_mainChar->getCenter() - getCenter();
	if (sqrt(dist.x * dist.x + dist.y * dist.y) <= 100.f) {
		dynamic_cast<MapScreen*>(m_screen)->setCooking();
	}
	else {
		m_screen->setTooltipText("OutOfRange", COLOR_BAD, true);
	}
}
