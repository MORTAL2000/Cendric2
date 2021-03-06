#include "Level/Enemies/FireRatEnemy.h"
#include "Level/LevelMainCharacter.h"
#include "Level/MOBBehavior/MovingBehaviors/AggressiveWalkingBehavior.h"
#include "Level/MOBBehavior/MovingBehaviors/AllyWalkingBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/AggressiveBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/AllyBehavior.h"
#include "Registrar.h"

REGISTER_ENEMY(EnemyID::FireRat, FireRatEnemy)

void FireRatEnemy::insertDefaultLoot(std::map<std::string, int>& loot, int& gold) const {
	if (gold != 0 || !loot.empty()) return;

	loot.insert({ "fo_rawmeat", 1 });
	if (rand() % 2 > 0)
		loot.insert({ "mi_teeth", 1 });
	gold = rand() % 4 + 1;
}

FireRatEnemy::FireRatEnemy(Level* level, Screen* screen) :
	LevelMovableGameObject(level),
    Enemy(level, screen) {
	load(EnemyID::FireRat);
}

void FireRatEnemy::loadAttributes() {
	m_attributes.setHealth(40);
	m_attributes.resistanceIce = -20;
	m_attributes.calculateAttributes();

	m_immuneDamageTypes.push_back(DamageType::Fire);
}

void FireRatEnemy::loadSpells() {
	SpellData chopSpell = SpellData::getSpellData(SpellID::Chop);
	chopSpell.damage = 6;
	chopSpell.activeDuration = sf::milliseconds(500);
	chopSpell.cooldown = sf::milliseconds(1000);
	chopSpell.boundingBox = sf::FloatRect(10, 0, 30, 30);

	SpellData fireBallSpell = SpellData::getSpellData(SpellID::FireBall);
	fireBallSpell.damage = 6;
	fireBallSpell.damagePerSecond = 2;
	fireBallSpell.duration = sf::seconds(2);
	fireBallSpell.cooldown = sf::milliseconds(3000);
	fireBallSpell.speed = 200.f;

	m_spellManager->addSpell(chopSpell);
	m_spellManager->addSpell(fireBallSpell);
	m_spellManager->setCurrentSpell(0); // chop
}

sf::Vector2f FireRatEnemy::getConfiguredSpellOffset() const {
	return sf::Vector2f(-10.f, 0.f);
}

void FireRatEnemy::handleAttackInput() {
	if (m_enemyState != EnemyState::Chasing) return;
	if (getCurrentTarget() == nullptr) return;
	if (m_enemyAttackingBehavior->distToTarget() < m_enemyAttackingBehavior->getAggroRange()) {
		m_spellManager->setCurrentSpell(1); // fire ball
		if (m_enemyAttackingBehavior->distToTarget() < 50.f) {
			m_spellManager->setCurrentSpell(0); // chop
		}

		m_spellManager->executeCurrentSpell(getCurrentTarget()->getCenter());
	}
}

void FireRatEnemy::loadAnimation() {
	setBoundingBox(sf::FloatRect(0.f, 0.f, 40.f, 30.f));
	setSpriteOffset(sf::Vector2f(-5.f, -20.f));

	Animation* walkingAnimation = new Animation();
	walkingAnimation->setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_enemy_firerat));
	walkingAnimation->addFrame(sf::IntRect(0, 0, 50, 50));
	walkingAnimation->addFrame(sf::IntRect(50, 0, 50, 50));
	walkingAnimation->addFrame(sf::IntRect(100, 0, 50, 50));
	walkingAnimation->addFrame(sf::IntRect(50, 0, 50, 50));

	addAnimation(GameObjectState::Walking, walkingAnimation);

	Animation* idleAnimation = new Animation();
	idleAnimation->setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_enemy_firerat));
	idleAnimation->addFrame(sf::IntRect(50, 0, 50, 50));
	idleAnimation->addFrame(sf::IntRect(300, 0, 50, 50));

	addAnimation(GameObjectState::Idle, idleAnimation);

	Animation* jumpingAnimation = new Animation();
	jumpingAnimation->setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_enemy_firerat));
	jumpingAnimation->addFrame(sf::IntRect(150, 0, 50, 50));

	addAnimation(GameObjectState::Jumping, jumpingAnimation);

	Animation* fightingAnimation = new Animation(sf::seconds(0.08f));
	fightingAnimation->setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_enemy_firerat));
	fightingAnimation->addFrame(sf::IntRect(200, 0, 50, 50));
	fightingAnimation->addFrame(sf::IntRect(250, 0, 50, 50));

	addAnimation(GameObjectState::Fighting, fightingAnimation);

	Animation* deadAnimation = new Animation();
	deadAnimation->setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_enemy_firerat));
	deadAnimation->addFrame(sf::IntRect(350, 0, 50, 50));

	addAnimation(GameObjectState::Dead, deadAnimation);

	// initial values
	setState(GameObjectState::Idle);
	playCurrentAnimation(true);
}

MovingBehavior* FireRatEnemy::createMovingBehavior(bool asAlly) {
	WalkingBehavior* behavior;
	if (asAlly) {
		behavior = new AllyWalkingBehavior(this);
	}
	else {
		behavior = new AggressiveWalkingBehavior(this);
	}
	behavior->setDistanceToAbyss(20.f);
	behavior->setApproachingDistance(10.f);
	behavior->setMaxVelocityYDown(400.f);
	behavior->setMaxVelocityYUp(400.f);
	behavior->setMaxVelocityX(50.f);
	behavior->setFightAnimationTime(sf::milliseconds(4 * 80));
	behavior->calculateJumpHeight();
	return behavior;
}

AttackingBehavior* FireRatEnemy::createAttackingBehavior(bool asAlly) {
	EnemyAttackingBehavior* behavior;
	if (asAlly) {
		behavior = new AllyBehavior(this);
	}
	else {
		behavior = new AggressiveBehavior(this);
	}
	behavior->setAggroRange(500.f);
	behavior->setAttackInput(std::bind(&FireRatEnemy::handleAttackInput, this));
	return behavior;
}

int FireRatEnemy::getMentalStrength() const {
	return 1;
}
