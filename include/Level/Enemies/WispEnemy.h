#pragma once

#include "global.h"
#include "Level/Level.h"
#include "Spells/SpellManager.h"
#include "Screen.h"

// A flying mysterious wisp
class WispEnemy : public Enemy {
public:
	WispEnemy(Level* level, Screen* screen);
	~WispEnemy() {}

	void loadAnimation() override;

	MovingBehavior* createMovingBehavior(bool asAlly) override;
	AttackingBehavior* createAttackingBehavior(bool asAlly) override;

	sf::Time getConfiguredWaitingTime() const override;
	sf::Time getConfiguredChasingTime() const override;

	sf::Vector2f getConfiguredSpellOffset() const override;

	void insertDefaultLoot(std::map<std::string, int>& loot, int& gold) const override;

private:

	void handleAttackInput();
	// loads attributes and adds immune spells + enemies. all attributes are set to zero before that call. default does nothing.
	void loadAttributes() override;
	// loads spells and adds them to the spell manager. default does nothing.
	void loadSpells() override;
};