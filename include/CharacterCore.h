#pragma once

#include <map>

#include "global.h"
#include "ResourceManager.h"
#include "FileIO/CharacterCoreReader.h"
#include "FileIO/CharacterCoreWriter.h"
#include "FileIO/QuestLoader.h"
#include "Item.h"
#include "Weapon.h"

#include "Structs/AttributeData.h"
#include "Structs/CharacterCoreData.h"
#include "ScreenOverlay.h"

class CharacterCore {
public:
	CharacterCore();
	CharacterCore(const CharacterCoreData& data);
	~CharacterCore();

	// returns the currently equipped item of type 'type'
	const Item* getEquippedItem(ItemType type);
	// getter for currently equipped weapon. convenience method for getItem of type weapon with a dynamic cast.
	const Weapon* getWeapon();
	// returns the item of id "id"
	const Item* getItem(const std::string& id);
	QuestState getQuestState(const std::string& id) const;
	void setQuestState(const std::string& id, QuestState state);
	void setQuickslot(const std::string& item, int nr);
	// loads a new game with a new core and default attributes
	void loadNew();
	// tries to create a new file with this name. if successful, returns true. if it already exists, returns false
	bool createFile(const std::string& fileName) const;
	// uses the character core reader to load a .sav file
	bool load(const std::string& fileName);
	bool quickload();
	// uses the character core writer to save a .sav file. The name is the name chosen by the user.
	bool save(const std::string& fileName, const std::string& name);
	bool quicksave();
	// loads equipped itemvector (reload if not empty)
	void loadEquipmentItems();
	// loads itemvector (reload if not empty)
	void loadItems();
	// loads all quests that are started, completed or failed.
	void loadQuests();
	// reloads only the weapon slots.
	void reloadWeaponSlots();
	// base attributes plus the attributes gotten by equipment
	const AttributeData& getTotalAttributes() const;
	// getter for items
	std::map<std::string, int>* getItems();
	// an item was added or removed. if itemID equals "gold", gold is added or removed
	void notifyItemChange(const std::string& itemID, int amount);
	// save the position and map id. also sets "isInLevel" = false
	void setMap(const sf::Vector2f& position, const std::string& map);
	// save the position and level id  also sets "isInLevel" = true
	void setLevel(const sf::Vector2f& position, const std::string& level);
	// equips an item at the given position and adds the former equipment part into the inventory. To deequip an item, use this method with an empty string as item id.
	void equipItem(const std::string& item, ItemType type);
	// tries to remove the spell from slot nr "slotNr"
	void removeSpell(int slotNr);
	// tries to add the spell with id "id" to slot nr "slotNr"
	void addSpell(SpellID id, int slotNr);
	// tries to remove a modifier with modifierNr "modifierNr" from weapon slot nr "slotNr"
	void removeModifier(int slotNr, int modifierNr);
	// tries to add a modifier of type "type" to weapon slot nr "slotNr" into the modifier slot with nr "modifierNr"
	// it uses "force" for that 
	void addModifier(const SpellModifier& modifier, int slotNr, int modifierNr);
	// adds permanent (!) attributes to the core.
	void addPermanentAttributes(const AttributeData& attributes);
	// getter for core part
	const CharacterCoreData& getData() const;
	// initializes three empty boolean maps (enemies killed, enemies looted and items looted) for a given level
	void initializeLevelMaps(const std::string& level);
	// initializes an empty boolean maps (waypoint unlocked) for a given map
	void initializeMapMaps(const std::string& map);
	// signal enemy kill
	void setEnemyKilled(const std::string& level, int pos);
	// signal enemy looted
	void setEnemyLooted(const std::string& level, int pos);
	// signal item looted
	void setItemLooted(const std::string& level, int pos);
	// signal chest looted
	void setChestLooted(const std::string& level, int pos);
	// signal waypoint unlocked
	void setWaypointUnlocked(const std::string& map, int pos);
	// signal trigger triggered
	void setTriggerTriggered(const std::string& world, int pos);
	// getter for quest data. If there is none for that id, returns nullptr
	const QuestData* getQuestData(const std::string& questID) const;
	// getter for number of targets with name "name" killed for quest with id "questID"
	int getNumberOfTargetsKilled(const std::string& questID, const std::string& name) const;
	// getter for number of targets in total with name "name" killed for quest with id "questID"
	int getNumberOfTotalTargets(const std::string& questID, const std::string& name) const;
	// is the quest with id "id" complete?
	bool isQuestComplete(const std::string& questID) const;
	// a target for a quest has been killed
	void setQuestTargetKilled(const std::string& questID, const std::string& name);
	// a condition for a quest has been fulfilled
	void setQuestConditionFulfilled(const std::string& questID, const std::string& condition);
	// is a quest condition for a certain quest fulfilled?
	bool isQuestConditionFulfilled(const std::string& questID, const std::string& condition) const;
	// set a condition fulfilled
	void setConditionFulfilled(const std::string& conditionType, const std::string& condition);
	// unlock a certain quest description
	void unlockQuestDescription(const std::string& questID, int descriptionID);
	// is a condition fulfilled?
	bool isConditionFulfilled(const std::string& conditionType, const std::string& condition) const;
	// is a enemy in a certain level dead?
	bool isEnemyKilled(const std::string& levelID, int objectID);
	// was a trigger already triggered?
	bool isTriggerTriggered(const std::string& world, int triggerID);
	// returns a copy of the merchant data for that id. If it is not already present, tries to load it.
	MerchantData getMerchantData(const std::string& merchantID);
	// sets the merchant data of this merchant id
	void setMerchantData(const std::string& merchantID, const MerchantData& data);
	// learns the spell with id "spellID"
	void learnSpell(SpellID spellID);
	// learns this modifier
	void learnModifier(const SpellModifier& modifier);

	static const char* QUICKSAVE_LOCATION;
	static const char* DEBUGSAVE_LOCATION;

private:
	// clears equipped itemvector
	void clearEquippedItems();
	// clears itemvector
	void clearItems();
	// reload attributes based on equipped items (called by load). all attributes coming from food are removed.
	void reloadAttributes();
	// add gold to the data
	void addGold(int gold);
	// remove gold from the data
	void removeGold(int gold);
	// adds item(s) to the data
	void addItem(const std::string& item, int quantity);
	// removes item(s) to the data
	void removeItem(const std::string& item, int quantity);

	// base attributes plus the attributes of all currently equipped items
	AttributeData m_totalAttributes;

	std::map<std::string, Item> m_items;
	std::map<ItemType, Item*> m_equippedItems;
	std::map<std::string, QuestData> m_quests;

	CharacterCoreData m_data;

	sf::Clock m_stopwatch;
};