#pragma once

#include <map>

#include "global.h"
#include "Spells/Spell.h"

#include "Structs/SpellModifier.h"

class LevelScreen;

// a class that spawns spells of a certain type - directly to the screen
class SpellCreator {
public:
	SpellCreator(const SpellData& spellData, LevelMovableGameObject* owner);
	virtual ~SpellCreator();

	void addModifiers(const std::vector<SpellModifier>& modifiers);

	// calculates spells using the owners attributes & the target, and executes their behaviour, adding objects to the screen.
	virtual void executeSpell(const sf::Vector2f& target) = 0;
	const SpellData& getSpellData() const;
	void setSpellAllied(bool allied);

	// used by the descriptions to show what this creator does if it has a strength modifier
	// default returns empty string
	virtual std::string getStrengthModifierName() const;
	// used by the descriptions to return strength modifier value
	// default returns 0
	virtual int getStrengthModifierValue() const;

	// updates the spells damage and heal, using the attribute data. It adds damage/heal and uses some rng and calculates critical hits if told so
	static void updateDamageAndHeal(SpellData& bean, const AttributeData* attributes, bool includeRngAndCrit);

protected:
	// filled by the subclasses
	std::vector<SpellModifierType> m_allowedModifiers;
	// updates the spells damage, using the mobs attribute bean. It adds damage, some rng and calculates critical hits
	void updateDamageAndHeal(SpellData& bean) const;

	virtual void addRangeModifier(int level);
	virtual void addSpeedModifier(int level);
	virtual void addCountModifier(int level);
	virtual void addReflectModifier(int level);
	virtual void addStrengthModifier(int level);
	virtual void addDurationModifier(int level);

	const AttributeData* m_attributeData;
	SpellData m_spellData;
	Level* m_level = nullptr;
	LevelScreen* m_screen = nullptr;
	LevelMovableGameObject* m_owner = nullptr;
};