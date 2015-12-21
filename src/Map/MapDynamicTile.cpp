#include "Map/MapDynamicTile.h"

MapDynamicTile::MapDynamicTile(Map* map) : AnimatedGameObject() {
	m_map = map;
}

bool MapDynamicTile::getIsCollidable() const {
	return m_isCollidable;
}

void MapDynamicTile::setTileSize(const sf::Vector2i& tileSize) {
	m_tileSize = tileSize;
}

void MapDynamicTile::setPositionOffset(const sf::Vector2f& offset) {
	m_positionOffset = offset;
}

const sf::Vector2f&  MapDynamicTile::getPositionOffset() const {
	return m_positionOffset;
}

GameObjectType MapDynamicTile::getConfiguredType() const {
	return GameObjectType::_DynamicTile;
}

void MapDynamicTile::onHit(MapMainCharacter* mob) {
	// nop
}