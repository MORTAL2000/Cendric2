#include "CharacterCore.h"
#include "Item.h"

using namespace std;

const char* CharacterCore::QUICKSAVE_LOCATION = "saves/quicksave.sav";
const char* CharacterCore::DEBUGSAVE_LOCATION = "saves/debug.sav";

CharacterCore::CharacterCore()
{
	m_data = DEFAULT_CORE;
}

CharacterCore::CharacterCore(const CharacterCoreData& data)
{
	m_data = data;
	m_stopwatch.restart();
	reloadAttributes();
}

CharacterCore::~CharacterCore()
{
	clearEquippedItems();
	clearItems();
}

bool CharacterCore::quickload()
{
	return load(QUICKSAVE_LOCATION);
}

bool CharacterCore::load(const std::string& fileName)
{
	CharacterCoreReader reader;
	
	if (!reader.readCharacterCore(fileName, m_data))
	{
		return false;
	} 
	
	// measuring the time played with this save.
	m_stopwatch.restart();
	reloadAttributes();
	return true;
}

void CharacterCore::loadNew()
{
	// start map & position when a new game is loaded
	m_data.isInLevel = false;
	m_data.currentMap = MapID::Firstmap; 
	m_data.currentMapPosition = sf::Vector2f(4400.0f, 650.0f); 
	m_data.attributes.currentHealthPoints = 100;
	m_data.attributes.maxHealthPoints = 100;
	m_data.attributes.critical = 5;
	m_stopwatch.restart();
	reloadAttributes();
}

const Item* CharacterCore::getEquippedItem(ItemType type)
{
	if (m_equippedItems.empty())
	{
		loadEquipmentItems();
	}
	return m_equippedItems.at(type);
}

const Item& CharacterCore::getItem(const std::string& id)
{
	if (m_items.empty())
	{
		loadItems();
	}
	return m_items.at(id);
}

const Weapon* CharacterCore::getWeapon()
{
	const Weapon* weapon;
	if (!(weapon = dynamic_cast<const Weapon*>(getEquippedItem(ItemType::Equipment_weapon))))
	{
		// unexpected
		return nullptr;
	}
	return weapon;
}

NPCState CharacterCore::getNPCState(NPCID id)
{
	if (m_data.npcStates.find(id) != m_data.npcStates.end())
	{
		return m_data.npcStates[id];
	}
	return NPCState::Never_talked;
}

QuestState CharacterCore::getQuestState(QuestID id)
{
	if (m_data.questStates.find(id) != m_data.questStates.end())
	{
		return m_data.questStates[id];
	}
	return QuestState::Void;
}

void CharacterCore::setNPCState(NPCID id, NPCState state)
{
	if (m_data.npcStates.find(id) != m_data.npcStates.end())
	{
		m_data.npcStates[id] = state;
		return;
	}
	m_data.npcStates.insert({ id, state });
}

void CharacterCore::setQuestState(QuestID id, QuestState state)
{
	if (m_data.questStates.find(id) != m_data.questStates.end())
	{
		m_data.questStates[id] = state;
		return;
	}
	m_data.questStates.insert({ id, state });
}

void CharacterCore::setQuickslot(const std::string& item, int nr)
{
	if (nr == 1)
	{
		m_data.quickSlot1 = item;
	}
	else if (nr == 2)
	{
		m_data.quickSlot2 = item;
	}
}

bool CharacterCore::save(const std::string& fileName, const string& name) 
{
	m_data.timePlayed += m_stopwatch.restart();
	m_data.dateSaved = time(nullptr);
	m_data.saveGameName = name;
	
	// write to savefile.
	CharacterCoreWriter writer;

	writer.createFile(fileName);
	return writer.saveToFile(fileName, m_data);
}

bool CharacterCore::quicksave()
{
	m_data.timePlayed += m_stopwatch.restart();
	m_data.dateSaved = time(nullptr);
	m_data.saveGameName = "Quicksave";

	// write to savefile.
	CharacterCoreWriter writer;
	writer.createFile(QUICKSAVE_LOCATION);
	return writer.saveToFile(QUICKSAVE_LOCATION, m_data);
}

bool CharacterCore::createFile(const std::string& fileName) const
{
	CharacterCoreWriter writer;
	return writer.createFile(fileName);
}

void CharacterCore::reloadAttributes()
{
	m_totalAttributes = m_data.attributes;
	loadEquipmentItems();
	for (auto &it : m_equippedItems)
	{
		if (it.second == nullptr) continue;
		m_totalAttributes.addBean(it.second->getAttributes());
	}
	m_totalAttributes.currentHealthPoints = m_totalAttributes.maxHealthPoints;
}

void CharacterCore::loadEquipmentItems()
{
	clearEquippedItems();

	Weapon* eqWeapon = nullptr;
	Item* eqBack = nullptr;
	Item* eqBody = nullptr;
	Item* eqHead = nullptr;
	Item* eqNeck = nullptr;
	Item* eqRing1 = nullptr;
	Item* eqRing2 = nullptr;
	if (!m_data.equippedWeapon.empty() && g_resourceManager->getItemBean(m_data.equippedWeapon) != nullptr)
	{
		eqWeapon = new Weapon(*g_resourceManager->getItemBean(m_data.equippedWeapon));
		// add equipped spells and their modifiers
		for (int slot = 0; slot < m_data.equippedWeaponSlots.size(); slot++)
		{
			eqWeapon->addSpell(slot, m_data.equippedWeaponSlots[slot].first, false);
			for (auto& it : m_data.equippedWeaponSlots[slot].second)
			{
				eqWeapon->addModifier(slot, it, false);
			}
		}
	}
	if (!m_data.equippedBack.empty() && g_resourceManager->getItemBean(m_data.equippedBack) != nullptr)
	{
		eqBack = new Item(*g_resourceManager->getItemBean(m_data.equippedBack));
	}
	if (!m_data.equippedBody.empty() && g_resourceManager->getItemBean(m_data.equippedBody) != nullptr)
	{
		eqBody = new Item(*g_resourceManager->getItemBean(m_data.equippedBody));
	}
	if (!m_data.equippedHead.empty() && g_resourceManager->getItemBean(m_data.equippedHead) != nullptr)
	{
		eqHead = new Item(*g_resourceManager->getItemBean(m_data.equippedHead));
	}
	if (!m_data.equippedNeck.empty() && g_resourceManager->getItemBean(m_data.equippedNeck) != nullptr)
	{
		eqNeck = new Item(*g_resourceManager->getItemBean(m_data.equippedNeck));
	}
	if (!m_data.equippedRing1.empty() && g_resourceManager->getItemBean(m_data.equippedRing1) != nullptr)
	{
		eqRing1 = new Item(*g_resourceManager->getItemBean(m_data.equippedRing1));
	}
	if (!m_data.equippedRing2.empty() && g_resourceManager->getItemBean(m_data.equippedRing2) != nullptr)
	{
		eqRing2 = new Item(*g_resourceManager->getItemBean(m_data.equippedRing2));
	}
	
	m_equippedItems.insert({ ItemType::Equipment_weapon, eqWeapon });
	m_equippedItems.insert({ ItemType::Equipment_back, eqBack });
	m_equippedItems.insert({ ItemType::Equipment_body, eqBody });
	m_equippedItems.insert({ ItemType::Equipment_head, eqHead });
	m_equippedItems.insert({ ItemType::Equipment_neck, eqNeck });
	m_equippedItems.insert({ ItemType::Equipment_ring_1, eqRing1 });
	m_equippedItems.insert({ ItemType::Equipment_ring_2, eqRing2 });
}

void CharacterCore::loadItems()
{
	clearItems();
	for (auto& it : m_data.items)
	{
		const ItemBean* bean = g_resourceManager->getItemBean(it.first);
		if (bean == nullptr) continue;
		m_items.insert({ it.first, Item(*bean) });
	}
}

void CharacterCore::clearEquippedItems()
{
	for (auto& it : m_equippedItems)
	{
		delete it.second;
	}
	m_equippedItems.clear();
}

void CharacterCore::clearItems()
{
	m_items.clear();
}

void CharacterCore::initializeMaps(LevelID level)
{
	// if these entries for the given level already exist, an insert will do nothing.
	m_data.enemiesKilled.insert({ level, std::set<int>() });
	m_data.enemiesLooted.insert({ level, std::set<int>() });
	m_data.itemsLooted.insert({ level, std::set<int>() });
	m_data.chestsLooted.insert({ level, std::set<int>() });
}

void CharacterCore::setEnemyKilled(LevelID level, int pos)
{
	m_data.enemiesKilled.at(level).insert(pos);
}

void CharacterCore::setEnemyLooted(LevelID level, int pos)
{
	m_data.enemiesLooted.at(level).insert(pos);
}

void CharacterCore::setItemLooted(LevelID level, int pos)
{
	m_data.itemsLooted.at(level).insert(pos);
}

void CharacterCore::setChestLooted(LevelID level, int pos)
{
	m_data.chestsLooted.at(level).insert(pos);
}

const CharacterCoreData& CharacterCore::getData() const
{
	return m_data;
}
const AttributeBean& CharacterCore::getTotalAttributes() const
{
	return m_totalAttributes;
}

std::map<std::string, int>* CharacterCore::getItems()
{
	return &(m_data.items);
}

void CharacterCore::addGold(int gold)
{
	m_data.gold += std::max(gold, 0);
}

void CharacterCore::removeGold(int gold)
{
	m_data.gold -= std::min(m_data.gold, gold);
}

void CharacterCore::addItem(const std::string& item, int quantity)
{
	auto it = m_data.items.find(item);

	if (it != m_data.items.end())
	{
		m_data.items.at(item) = m_data.items.at(item) + quantity;
	}
	else
	{
		m_data.items.insert({ item, quantity });
	}
}

void CharacterCore::removeItem(const std::string& item, int quantity)
{
	auto it = m_data.items.find(item);

	if (it != m_data.items.end())
	{
		m_data.items.at(item) = m_data.items.at(item) - quantity;
		if (m_data.items.at(item) <= 0)
		{
			m_data.items.erase(item);
		}
	}
}

void CharacterCore::setMap(const sf::Vector2f& position, MapID map)
{
	m_data.currentMap = map;
	m_data.currentMapPosition = position;
	m_data.isInLevel = false;
}

void CharacterCore::setLevel(const sf::Vector2f& position, LevelID level)
{
	m_data.currentLevel = level;
	m_data.currentLevelPosition = position;
	m_data.isInLevel = true;
}

void CharacterCore::equipItem(const std::string& item, ItemType type)
{
	std::string oldItem = "";
	switch (type)
	{
	case ItemType::Equipment_back:
		oldItem = m_data.equippedBack;
		m_data.equippedBack = item;
		break;
	case ItemType::Equipment_body:
		oldItem = m_data.equippedBody;
		m_data.equippedBody = item;
		break;
	case ItemType::Equipment_head:
		oldItem = m_data.equippedHead;
		m_data.equippedHead = item;
		break;
	case ItemType::Equipment_neck:
		oldItem = m_data.equippedNeck;
		m_data.equippedNeck = item;
		break;
	case ItemType::Equipment_ring_1:
		oldItem = m_data.equippedRing1;
		m_data.equippedRing1 = item;
		break;
	case ItemType::Equipment_ring_2:
		oldItem = m_data.equippedRing2;
		m_data.equippedRing2 = item;
		break;
	case ItemType::Equipment_weapon:
		oldItem = m_data.equippedWeapon;
		m_data.equippedWeapon = item;
		m_data.equippedWeaponSlots.clear();
		break;
	default: 
		return;
	}
	removeItem(item, 1);
	if (!oldItem.empty())
	{
		addItem(oldItem, 1);
	}
	loadEquipmentItems();
	loadItems();
}