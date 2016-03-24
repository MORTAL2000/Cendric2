#pragma once

#include "global.h"
#include "Spells/SpellManager.h"

#include "GUI/WorldInterface.h"
#include "GUI/HealthBar.h"
#include "GUI/BuffBar.h"
#include "GUI/QuickSlotBar.h"
#include "GUI/SpellSelection.h"

// The interface overlay in a level, displaying hp bar, spell cooldowns, buffs, quickslot bar
// if the keys for character screen and inventory are pressed, these will display as well.
class LevelInterface : public WorldInterface {
public:
	LevelInterface(WorldScreen* screen, LevelMainCharacter* character);
	~LevelInterface();

	// the spell manager of the level main character.
	void setSpellManager(SpellManager* spellManager);
	// returns the buff bar
	BuffBar& getBuffBar();
	// an consumable item has been dropped. forward to quick slot bar
	void notifyConsumableDrop(const SlotClone* item);
	// consumes a consumable item
	void consumeItem(const Item& item);
	// highlight quickslots
	void highlightQuickslots(bool highlight);
	// reloads inventory and quickslot bar
	void reloadInventory(const std::string& changeditemID) override;
	// notifies the character info
	void notifyCharacterInfo();

	void render(sf::RenderTarget& target) override;
	void update(const sf::Time& frameTime) override;

	LevelMainCharacter* getMainCharacter() const;

private:
	LevelMainCharacter* m_character = nullptr;

	// <<< HEALTH BAR >>>
	HealthBar* m_healthBar = nullptr;

	// <<< SPELL SELECTION >>>
	SpellSelection* m_spellSelection = nullptr;

	// <<< QUICK SLOTS >>>
	QuickSlotBar* m_quickSlotBar = nullptr;

	// <<< BUFF BAR >>>
	BuffBar* m_buffBar = nullptr;
};