#pragma once

#include "global.h"
#include "Spells/SpellCreator.h"

#include "Spells/ShadowTrapSpell.h"

// a class that creates shadow trap spells
class ShadowTrapSpellCreator : public SpellCreator {
public:
	ShadowTrapSpellCreator(const SpellData& spellData, LevelMovableGameObject* owner);
	std::string getStrengthModifierName() const override;

	void executeSpell(const sf::Vector2f &target) override;

private:
	void addDurationModifier(int level) override;
};