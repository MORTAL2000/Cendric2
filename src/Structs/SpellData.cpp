#include "Structs/SpellData.h"
#include "Spells/SpellCreator.h"
#include "SpellCreators/FireBallSpellCreator.h"
#include "SpellCreators/IceBallSpellCreator.h"
#include "SpellCreators/DivineShieldSpellCreator.h"
#include "SpellCreators/AureolaSpellCreator.h"
#include "SpellCreators/ChopSpellCreator.h"
#include "SpellCreators/FearSpellCreator.h"
#include "SpellCreators/AntiGravitySpellCreator.h"
#include "SpellCreators/WindGustSpellCreator.h"
#include "SpellCreators/LeechSpellCreator.h"
#include "SpellCreators/IcyAmbushSpellCreator.h"
#include "SpellCreators/UnlockSpellCreator.h"
#include "SpellCreators/LightSpellCreator.h"
#include "SpellCreators/TelekinesisSpellCreator.h"
#include "SpellCreators/InvisibilitySpellCreator.h"
#include "SpellCreators/FlashSpellCreator.h"
#include "SpellCreators/LeapOfFaithSpellCreator.h"
#include "SpellCreators/GhostFormSpellCreator.h"
#include "SpellCreators/ShadowTrapSpellCreator.h"
#include "SpellCreators/RaiseTheDeadSpellCreator.h"
#include "SpellCreators/HolyFireSpellCreator.h"
#include "SpellCreators/SummonGargoyleSpellCreator.h"
#include "SpellCreators/ProjectileSpellCreator.h"

std::vector<SpellModifierType> SpellData::getAllowedModifiers(SpellID id) {
	std::vector<SpellModifierType> types;
	switch (id) {
	case SpellID::Chop:
		break;
	case SpellID::FireBall:
		types.push_back(SpellModifierType::Count);
		types.push_back(SpellModifierType::Reflect);
		types.push_back(SpellModifierType::Speed);
		break;
	case SpellID::IceBall:
		types.push_back(SpellModifierType::Count);
		types.push_back(SpellModifierType::Reflect);
		types.push_back(SpellModifierType::Speed);
		break;
	case SpellID::Aureola:
		types.push_back(SpellModifierType::Count);
		types.push_back(SpellModifierType::Speed);
		types.push_back(SpellModifierType::Range);
		types.push_back(SpellModifierType::Duration);
		types.push_back(SpellModifierType::Strength);
		break;
	case SpellID::DivineShield:
		types.push_back(SpellModifierType::Duration);
		types.push_back(SpellModifierType::Strength);
		break;
	case SpellID::Fear:
		types.push_back(SpellModifierType::Count);
		types.push_back(SpellModifierType::Reflect);
		types.push_back(SpellModifierType::Speed);
		types.push_back(SpellModifierType::Duration);
		types.push_back(SpellModifierType::Strength);
		break;
	case SpellID::AntiGravity:
		types.push_back(SpellModifierType::Duration);
		break;
	case SpellID::WindGust:
		types.push_back(SpellModifierType::Duration);
		types.push_back(SpellModifierType::Range);
		types.push_back(SpellModifierType::Strength);
		break;
	case SpellID::Leech:
		types.push_back(SpellModifierType::Count);
		types.push_back(SpellModifierType::Reflect);
		types.push_back(SpellModifierType::Speed);
		break;
	case SpellID::IcyAmbush:
		types.push_back(SpellModifierType::Range);
		types.push_back(SpellModifierType::Reflect);
		types.push_back(SpellModifierType::Strength);
		types.push_back(SpellModifierType::Duration);
		break;
	case SpellID::Unlock:
		types.push_back(SpellModifierType::Strength);
		break;
	case SpellID::Light:
		types.push_back(SpellModifierType::Range);
		types.push_back(SpellModifierType::Duration);
		break;
	case SpellID::Telekinesis:
		types.push_back(SpellModifierType::Range);
		types.push_back(SpellModifierType::Reflect);
		break;
	case SpellID::Invisibility:
		types.push_back(SpellModifierType::Strength);
		types.push_back(SpellModifierType::Duration);
		break;
	case SpellID::Flash:
		types.push_back(SpellModifierType::Range);
		break;
	case SpellID::LeapOfFaith:
		types.push_back(SpellModifierType::Duration);
		types.push_back(SpellModifierType::Strength);
		break;
	case SpellID::GhostForm:
		types.push_back(SpellModifierType::Duration);
		types.push_back(SpellModifierType::Strength);
		types.push_back(SpellModifierType::Speed);
		break;
	case SpellID::ShadowTrap:
		types.push_back(SpellModifierType::Duration);
		types.push_back(SpellModifierType::Strength);
		break;
	case SpellID::RaiseTheDead:
		types.push_back(SpellModifierType::Duration);
		types.push_back(SpellModifierType::Strength);
		types.push_back(SpellModifierType::Range);
		break;
	case SpellID::HolyFire:
		types.push_back(SpellModifierType::Duration);
		types.push_back(SpellModifierType::Range);
		break;
	case SpellID::SummonGargoyle:
		types.push_back(SpellModifierType::Duration);
		types.push_back(SpellModifierType::Count);
		types.push_back(SpellModifierType::Strength);
		break;
	default:
		break;
	}
	return types;
}

SpellCreator* SpellData::getSpellCreator(const SpellData& data, const std::vector<SpellModifier>& modifiers, LevelMovableGameObject* owner) {
	SpellCreator* creator;
	switch (data.id) {
	case SpellID::Chop:
		creator = new ChopSpellCreator(data, owner);
		break;
	case SpellID::FireBall:
		creator = new FireBallSpellCreator(data, owner);
		break;
	case SpellID::IceBall:
		creator = new IceBallSpellCreator(data, owner);
		break;
	case SpellID::DivineShield:
		creator = new DivineShieldSpellCreator(data, owner);
		break;
	case SpellID::Aureola:
		creator = new AureolaSpellCreator(data, owner);
		break;
	case SpellID::Fear:
		creator = new FearSpellCreator(data, owner);
		break;
	case SpellID::AntiGravity:
		creator = new AntiGravitySpellCreator(data, owner);
		break;
	case SpellID::WindGust:
		creator = new WindGustSpellCreator(data, owner);
		break;
	case SpellID::Leech:
		creator = new LeechSpellCreator(data, owner);
		break;
	case SpellID::IcyAmbush:
		creator = new IcyAmbushSpellCreator(data, owner);
		break;
	case SpellID::Unlock:
		creator = new UnlockSpellCreator(data, owner);
		break;
	case SpellID::Light:
		creator = new LightSpellCreator(data, owner);
		break;
	case SpellID::Telekinesis:
		creator = new TelekinesisSpellCreator(data, owner);
		break;
	case SpellID::Invisibility:
		creator = new InvisibilitySpellCreator(data, owner);
		break;
	case SpellID::Flash:
		creator = new FlashSpellCreator(data, owner);
		break;
	case SpellID::LeapOfFaith:
		creator = new LeapOfFaithSpellCreator(data, owner);
		break;
	case SpellID::GhostForm:
		creator = new GhostFormSpellCreator(data, owner);
		break;
	case SpellID::ShadowTrap:
		creator = new ShadowTrapSpellCreator(data, owner);
		break;
	case SpellID::RaiseTheDead:
		creator = new RaiseTheDeadSpellCreator(data, owner);
		break;
	case SpellID::HolyFire:
		creator = new HolyFireSpellCreator(data, owner);
		break;
	case SpellID::SummonGargoyle:
		creator = new SummonGargoyleSpellCreator(data, owner);
		break;
	case SpellID::Projectile:
		creator = new ProjectileSpellCreator(data, owner);
		break;
	default:
		return nullptr;
	}

	creator->addModifiers(modifiers);

	return creator;
}

SpellData SpellData::getSpellData(SpellID id) {
	switch (id) {
	case SpellID::Chop:
		return getChopSpellData();
	case SpellID::FireBall:
		return getFireBallSpellData();
	case SpellID::IceBall:
		return getIceBallSpellData();
	case SpellID::Aureola:
		return getAureolaSpellData();
	case SpellID::DivineShield:
		return getDivineShieldSpellData();
	case SpellID::Fear:
		return getFearSpellData();
	case SpellID::AntiGravity:
		return getAntiGravitySpellData();
	case SpellID::WindGust:
		return getWindGustSpellData();
	case SpellID::Leech:
		return getLeechSpellData();
	case SpellID::IcyAmbush:
		return getIcyAmbushSpellData();
	case SpellID::Unlock:
		return getUnlockSpellData();
	case SpellID::Light:
		return getLightSpellData();
	case SpellID::Telekinesis:
		return getTelekinesisSpellData();
	case SpellID::Invisibility:
		return getInvisibilitySpellData();
	case SpellID::Flash:
		return getFlashSpellData();
	case SpellID::LeapOfFaith:
		return getLeapOfFaithSpellData();
	case SpellID::GhostForm:
		return getGhostFormSpellData();
	case SpellID::ShadowTrap:
		return getShadowTrapSpellData();
	case SpellID::RaiseTheDead:
		return getRaiseTheDeadSpellData();
	case SpellID::HolyFire:
		return getHolyFireSpellData();
	case SpellID::SummonGargoyle:
		return getSummonGargoyleSpellData();
	case SpellID::Projectile:
		return getProjectileSpellData();
	default:
		return EMPTY_SPELL;
	}
}

SpellData SpellData::getChopSpellData() {
	SpellData chop = EMPTY_SPELL;
	chop.id = SpellID::Chop;

	chop.cooldown = sf::milliseconds(400);
	chop.boundingBox = sf::FloatRect(0, 0, 40, 80);
	chop.damageType = DamageType::Physical;
	chop.damage = 3;
	chop.activeDuration = sf::milliseconds(320);
	chop.attachedToMob = true;
	chop.isDynamicTileEffect = true;

	chop.inputKey = Key::Chop;

	return chop;
}

SpellData SpellData::getProjectileSpellData() {
	SpellData projectile = EMPTY_SPELL;
	projectile.id = SpellID::Projectile;

	projectile.cooldown = sf::seconds(1);
	projectile.boundingBox = sf::FloatRect(0, 0, 10, 10);
	projectile.divergenceAngle = 0.2f;
	projectile.damageType = DamageType::Physical;
	projectile.activeDuration = sf::seconds(5);
	projectile.damagePerSecond = 2;
	projectile.duration = sf::seconds(3);
	projectile.needsTarget = true;
	projectile.isDynamicTileEffect = true;
	projectile.damage = 10;
	projectile.speed = 600.f;

	projectile.inputKey = Key::Chop;

	return projectile;
}

SpellData SpellData::getFireBallSpellData() {
	SpellData fireBall = EMPTY_SPELL;
	fireBall.id = SpellID::FireBall;
	fireBall.spellType = SpellType::Elemental;
	fireBall.iconTextureRect = sf::IntRect(0, 0, 50, 50);

	fireBall.cooldown = sf::seconds(1);
	fireBall.boundingBox = sf::FloatRect(0, 0, 10, 10);
	fireBall.divergenceAngle = 0.2f;
	fireBall.damageType = DamageType::Fire;
	fireBall.activeDuration = sf::seconds(5);
	fireBall.damagePerSecond = 2;
	fireBall.duration = sf::seconds(3);
	fireBall.needsTarget = true;
	fireBall.isDynamicTileEffect = true;
	fireBall.damage = 10;
	fireBall.speed = 300.f;

	fireBall.countModifierAddition = 1;
	fireBall.damageModifierAddition = 20;
	fireBall.reflectModifierAddition = 1;
	fireBall.speedModifierAddition = 100.f;

	return fireBall;
}

SpellData SpellData::getIceBallSpellData() {
	SpellData iceBall = EMPTY_SPELL;
	iceBall.id = SpellID::IceBall;
	iceBall.spellType = SpellType::Elemental;
	iceBall.iconTextureRect = sf::IntRect(50, 0, 50, 50);

	iceBall.cooldown = sf::seconds(1);
	iceBall.boundingBox = sf::FloatRect(0, 0, 10, 10);
	iceBall.divergenceAngle = 0.2f;
	iceBall.damageType = DamageType::Ice;
	iceBall.activeDuration = sf::seconds(5);
	iceBall.isDynamicTileEffect = true;
	iceBall.needsTarget = true;
	iceBall.damage = 6;
	iceBall.speed = 200.f;

	iceBall.countModifierAddition = 1;
	iceBall.damageModifierAddition = 20;
	iceBall.reflectModifierAddition = 1;
	iceBall.speedModifierAddition = 100.f;

	return iceBall;
}

SpellData SpellData::getDivineShieldSpellData() {
	SpellData divineShield = EMPTY_SPELL;
	divineShield.id = SpellID::DivineShield;
	divineShield.spellType = SpellType::Divine;
	divineShield.iconTextureRect = sf::IntRect(100, 150, 50, 50);

	divineShield.cooldown = sf::milliseconds(10000);
	divineShield.boundingBox = sf::FloatRect(0, 0, 98, 98);
	divineShield.duration = sf::seconds(3);
	divineShield.activeDuration = divineShield.duration;
	divineShield.heal = 20;
	divineShield.attachedToMob = true;

	divineShield.durationModifierAddition = sf::seconds(2);

	return divineShield;
}


SpellData SpellData::getAureolaSpellData() {
	SpellData aureola = EMPTY_SPELL;
	aureola.id = SpellID::Aureola;
	aureola.spellType = SpellType::Divine;
	aureola.iconTextureRect = sf::IntRect(200, 150, 50, 50);

	aureola.cooldown = sf::milliseconds(10000);
	aureola.boundingBox = sf::FloatRect(0, 0, 20, 20);
	aureola.count = 4;
	aureola.divergenceAngle = 2 * M_PI / aureola.count;
	aureola.damageType = DamageType::Light;
	aureola.activeDuration = sf::seconds(10);
	aureola.needsTarget = true;
	aureola.duration = sf::seconds(1.f);
	aureola.isStunning = true;
	aureola.strength = 1;
	aureola.damage = 10;
	aureola.heal = 10;
	aureola.speed = 300.f;
	aureola.range = 150.f;

	aureola.countModifierAddition = 2;
	aureola.damageModifierAddition = 10;
	aureola.speedModifierAddition = 100.f;
	aureola.rangeModifierAddition = 100.f;
	aureola.durationModifierAddition = sf::seconds(0.5f);

	return aureola;
}

SpellData SpellData::getFearSpellData() {
	SpellData fear = EMPTY_SPELL;
	fear.id = SpellID::Fear;
	fear.spellType = SpellType::Necromancy;
	fear.iconTextureRect = sf::IntRect(0, 100, 50, 50);

	fear.cooldown = sf::seconds(3);
	fear.boundingBox = sf::FloatRect(0, 0, 10, 10);
	fear.divergenceAngle = 0.2f;
	fear.activeDuration = sf::seconds(5);
	fear.duration = sf::seconds(2);
	fear.needsTarget = true;
	fear.speed = 300.f;
	fear.isFearing = true;
	fear.strength = 1;

	fear.countModifierAddition = 1;
	fear.reflectModifierAddition = 1;
	fear.speedModifierAddition = 100.f;
	fear.durationModifierAddition = sf::seconds(2);

	return fear;
}

SpellData SpellData::getAntiGravitySpellData() {
	SpellData antiGravity = EMPTY_SPELL;
	antiGravity.id = SpellID::AntiGravity;
	antiGravity.spellType = SpellType::Elemental;
	antiGravity.iconTextureRect = sf::IntRect(200, 0, 50, 50);

	antiGravity.cooldown = sf::seconds(15);
	antiGravity.boundingBox = sf::FloatRect(0, 0, 98, 98);
	antiGravity.duration = sf::seconds(5);
	antiGravity.activeDuration = antiGravity.duration;
	antiGravity.attachedToMob = true;

	antiGravity.durationModifierAddition = sf::seconds(3);

	return antiGravity;
}

SpellData SpellData::getTelekinesisSpellData() {
	SpellData telekinesis = EMPTY_SPELL;
	telekinesis.id = SpellID::Telekinesis;
	telekinesis.spellType = SpellType::Elemental;

	telekinesis.iconTextureRect = sf::IntRect(100, 0, 50, 50);
	telekinesis.cooldown = sf::seconds(3);
	telekinesis.boundingBox = sf::FloatRect(0, 0, 20, 20);
	telekinesis.speed = 200.f;
	telekinesis.isDynamicTileEffect = true;
	telekinesis.needsTarget = true;
	telekinesis.range = 100;
	telekinesis.activeDuration = sf::seconds(telekinesis.range / telekinesis.speed);

	telekinesis.rangeModifierAddition = 150.f;
	telekinesis.reflectModifierAddition = 1;

	return telekinesis;
}

SpellData SpellData::getWindGustSpellData() {
	SpellData windGust = EMPTY_SPELL;
	windGust.id = SpellID::WindGust;
	windGust.spellType = SpellType::Elemental;

	windGust.iconTextureRect = sf::IntRect(150, 0, 50, 50);
	windGust.cooldown = sf::seconds(3);
	windGust.range = 50.f;
	windGust.boundingBox = sf::FloatRect(0.f, 0.f, windGust.range, 60.f);
	windGust.duration = sf::seconds(1);
	windGust.activeDuration = windGust.duration;
	windGust.attachedToMob = true;
	windGust.isDynamicTileEffect = true;
	windGust.damageType = DamageType::Ice;
	windGust.damagePerSecond = 5;

	windGust.damageModifierAddition = 5;
	windGust.rangeModifierAddition = 50.f;
	windGust.durationModifierAddition = sf::seconds(1);

	return windGust;
}

SpellData SpellData::getLeechSpellData() {
	SpellData leech = EMPTY_SPELL;
	leech.id = SpellID::Leech;
	leech.spellType = SpellType::Necromancy;
	leech.iconTextureRect = sf::IntRect(50, 100, 50, 50);

	leech.cooldown = sf::seconds(3);
	leech.boundingBox = sf::FloatRect(0, 0, 10, 10);
	leech.divergenceAngle = 0.2f;
	leech.damageType = DamageType::Shadow;
	leech.activeDuration = sf::seconds(5);
	leech.needsTarget = true;
	leech.damage = 10;
	leech.speed = 150.f;

	leech.countModifierAddition = 1;
	leech.damageModifierAddition = 10;
	leech.reflectModifierAddition = 1;
	leech.speedModifierAddition = 50.f;

	return leech;
}

SpellData SpellData::getIcyAmbushSpellData() {
	SpellData icyAmbush = EMPTY_SPELL;
	icyAmbush.id = SpellID::IcyAmbush;
	icyAmbush.spellType = SpellType::Twilight;
	icyAmbush.iconTextureRect = sf::IntRect(200, 50, 50, 50);

	icyAmbush.cooldown = sf::seconds(5);
	icyAmbush.boundingBox = sf::FloatRect(0, 0, 20, 20);
	icyAmbush.damageType = DamageType::Ice;
	icyAmbush.range = 300.f;
	icyAmbush.speed = 200.f;
	icyAmbush.activeDuration = sf::seconds(icyAmbush.range / icyAmbush.speed);
	icyAmbush.duration = sf::seconds(1.5f); // stun duration;
	icyAmbush.needsTarget = true;
	icyAmbush.damage = 100;
	icyAmbush.isStunning = true;
	icyAmbush.strength = 1;
	
	icyAmbush.damageModifierAddition = 50;
	icyAmbush.reflectModifierAddition = 1;
	icyAmbush.rangeModifierAddition = 200.f;
	icyAmbush.durationModifierAddition = sf::seconds(0.5f);

	return icyAmbush;
}

SpellData SpellData::getFlashSpellData() {
	SpellData flash = EMPTY_SPELL;
	flash.id = SpellID::Flash;
	flash.spellType = SpellType::Twilight;
	flash.iconTextureRect = sf::IntRect(150, 50, 50, 50);

	flash.cooldown = sf::seconds(5);
	flash.damageType = DamageType::Light;
	flash.activeDuration = sf::seconds(2.0);
	flash.attachedToMob = true;
	flash.damage = 20;
	flash.range = 150.f;
	flash.boundingBox = sf::FloatRect(0, 0, 100, 120);

	flash.damageModifierAddition = 20;
	flash.rangeModifierAddition = 100.f;

	return flash;
}

SpellData SpellData::getLightSpellData() {
	SpellData light = EMPTY_SPELL;
	light.id = SpellID::Light;
	light.spellType = SpellType::Divine;
	light.iconTextureRect = sf::IntRect(0, 150, 50, 50);

	light.cooldown = sf::seconds(60);
	light.boundingBox = sf::FloatRect(0, 0, 1, 1);
	light.duration = sf::seconds(60);
	light.activeDuration = light.duration;
	light.range = 300.f;
	light.attachedToMob = true;

	light.durationModifierAddition = sf::seconds(60);
	light.rangeModifierAddition = 200.f;

	return light;
}

SpellData SpellData::getHolyFireSpellData() {
	SpellData holyFire = EMPTY_SPELL;
	holyFire.id = SpellID::HolyFire;
	holyFire.spellType = SpellType::Divine;
	holyFire.iconTextureRect = sf::IntRect(50, 150, 50, 50);

	holyFire.cooldown = sf::seconds(10);
	holyFire.range = 100.f;
	holyFire.boundingBox = sf::FloatRect(0, 0, 2 * holyFire.range, 2 * holyFire.range);
	holyFire.damagePerSecond = 5;
	holyFire.damageType = DamageType::Light;
	holyFire.duration = sf::seconds(2);
	holyFire.activeDuration = sf::seconds(3);
	holyFire.attachedToMob = true;

	holyFire.damageModifierAddition = 10;
	holyFire.durationModifierAddition = sf::seconds(1);
	holyFire.rangeModifierAddition = 50.f;

	return holyFire;
}

SpellData SpellData::getLeapOfFaithSpellData() {
	SpellData leapOfFaith = EMPTY_SPELL;
	leapOfFaith.id = SpellID::LeapOfFaith;
	leapOfFaith.spellType = SpellType::Divine;
	leapOfFaith.iconTextureRect = sf::IntRect(150, 150, 50, 50);

	leapOfFaith.cooldown = sf::seconds(30);
	leapOfFaith.boundingBox = sf::FloatRect(0, 0, 80, 120);
	leapOfFaith.duration = sf::seconds(10);
	leapOfFaith.activeDuration = leapOfFaith.duration;
	leapOfFaith.attachedToMob = true;

	leapOfFaith.durationModifierAddition = sf::seconds(10);

	return leapOfFaith;
}

SpellData SpellData::getUnlockSpellData() {
	SpellData unlock = EMPTY_SPELL;
	unlock.id = SpellID::Unlock;
	unlock.spellType = SpellType::Twilight;
	unlock.iconTextureRect = sf::IntRect(0, 50, 50, 50);

	unlock.cooldown = sf::seconds(2);
	unlock.boundingBox = sf::FloatRect(0, 0, 10, 10);
	unlock.activeDuration = sf::seconds(1);
	unlock.needsTarget = true;
	unlock.isDynamicTileEffect = true;
	unlock.speed = 200.f;
	unlock.strength = 1;

	return unlock;
}

SpellData SpellData::getInvisibilitySpellData() {
	SpellData invisibility = EMPTY_SPELL;
	invisibility.id = SpellID::Invisibility;
	invisibility.spellType = SpellType::Twilight;
	invisibility.iconTextureRect = sf::IntRect(100, 50, 50, 50);

	invisibility.cooldown = sf::seconds(30);
	invisibility.boundingBox = sf::FloatRect(0, 0, 1, 1);
	invisibility.activeDuration = sf::seconds(5);
	invisibility.duration = invisibility.activeDuration;
	invisibility.attachedToMob = true;

	invisibility.durationModifierAddition = sf::seconds(5);

	return invisibility;
}

SpellData SpellData::getGhostFormSpellData() {
	SpellData ghostForm = EMPTY_SPELL;
	ghostForm.id = SpellID::GhostForm;

	ghostForm.spellType = SpellType::Necromancy;
	ghostForm.iconTextureRect = sf::IntRect(150, 100, 50, 50);
	ghostForm.cooldown = sf::seconds(30);
	ghostForm.boundingBox = sf::FloatRect(0, 0, 30, 80);
	ghostForm.activeDuration = sf::seconds(5);
	ghostForm.duration = ghostForm.activeDuration;
	ghostForm.speed = 50.f;
	ghostForm.attachedToMob = true;

	ghostForm.durationModifierAddition = sf::seconds(5);
	ghostForm.speedModifierAddition = 50.f;

	return ghostForm;
}

SpellData SpellData::getShadowTrapSpellData() {
	SpellData shadowTrap = EMPTY_SPELL;
	shadowTrap.id = SpellID::ShadowTrap;
	shadowTrap.spellType = SpellType::Twilight;
	shadowTrap.iconTextureRect = sf::IntRect(50, 50, 50, 50);

	shadowTrap.cooldown = sf::seconds(10);
	shadowTrap.boundingBox = sf::FloatRect(0, 0, 25, 10);
	shadowTrap.divergenceAngle = 0.2f;
	shadowTrap.damageType = DamageType::Shadow;
	shadowTrap.activeDuration = sf::seconds(15);
	shadowTrap.damagePerSecond = 5;
	shadowTrap.duration = sf::seconds(3);
	shadowTrap.isStunning = true;
	shadowTrap.strength = 1;

	shadowTrap.damageModifierAddition = 5;
	shadowTrap.durationModifierAddition = sf::seconds(1);

	return shadowTrap;
}

SpellData SpellData::getSummonGargoyleSpellData() {
	SpellData summonGargoyle = EMPTY_SPELL;
	summonGargoyle.id = SpellID::SummonGargoyle;
	summonGargoyle.spellType = SpellType::Necromancy;
	summonGargoyle.iconTextureRect = sf::IntRect(200, 100, 50, 50);

	summonGargoyle.cooldown = sf::seconds(30);
	summonGargoyle.damageType = DamageType::Physical;
	summonGargoyle.damage = 0;
	summonGargoyle.duration = sf::seconds(20);
	summonGargoyle.strength = 1;

	summonGargoyle.damageModifierAddition = 10;
	summonGargoyle.durationModifierAddition = sf::seconds(20);
	summonGargoyle.countModifierAddition = 1;

	return summonGargoyle;
}

SpellData SpellData::getRaiseTheDeadSpellData() {
	SpellData raiseTheDead = EMPTY_SPELL;
	raiseTheDead.id = SpellID::RaiseTheDead;
	raiseTheDead.spellType = SpellType::Necromancy;

	raiseTheDead.iconTextureRect = sf::IntRect(100, 100, 50, 50);
	raiseTheDead.cooldown = sf::seconds(10);
	raiseTheDead.boundingBox = sf::FloatRect(0, 0, 10, 10);
	raiseTheDead.speed = 200.f;
	raiseTheDead.damage = 5;
	raiseTheDead.duration = sf::seconds(30.f);
	raiseTheDead.needsTarget = true;
	raiseTheDead.range = 100;
	raiseTheDead.activeDuration = sf::seconds(raiseTheDead.range / raiseTheDead.speed);
	raiseTheDead.strength = 1;

	raiseTheDead.rangeModifierAddition = 150.f;
	raiseTheDead.damageModifierAddition = 5;
	raiseTheDead.durationModifierAddition = sf::seconds(20.f);

	return raiseTheDead;
}




