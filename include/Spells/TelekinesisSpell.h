#pragma once

#include "global.h"
#include "Spells/Spell.h"

class TelekinesisSpell : public Spell {
public:
	TelekinesisSpell();
	void load(const SpellData& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) override;
	void update(const sf::Time& frameTime) override;
	void render(sf::RenderTarget& target) override;
	
	void setItemVector(const std::vector<GameObject*>* items);

private:
	std::unique_ptr<particles::TextureParticleSystem> m_ps;
	particles::AngledVelocityGenerator* m_velGenerator;
	particles::BoxPositionGenerator* m_pointGenerator;
	// collisions with levelitems
	void checkCollisionsWithItems();

	void loadParticleSystem();

	void updateParticleSystemPosition();

	const std::vector<GameObject*>* m_items;
};