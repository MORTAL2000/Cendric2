#pragma once

#include "global.h"
#include "Spells/SpellCreator.h"

#include "Spells/UnlockSpell.h"

// a class that creates unlock spells
class UnlockSpellCreator : public SpellCreator {
public:
	UnlockSpellCreator(const SpellData& spellData, LevelMovableGameObject* owner);
	std::string getStrengthModifierName() const override;
	int getStrengthModifierValue() const override;

	void executeSpell(const sf::Vector2f& target) override;

private:
	int m_strength = 1;

	void addStrengthModifier(int level) override;
};