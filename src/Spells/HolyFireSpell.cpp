#include "Spells/HolyFireSpell.h"
#include "GameObjectComponents/LightComponent.h"

void HolyFireSpell::load(const SpellData& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) {
	Spell::load(bean, mob, target);

	LightData lightData(sf::Vector2f(bean.range, bean.range), bean.range);
	addComponent(new LightComponent(lightData, this));
	loadParticleSystem();
}

sf::Vector2f HolyFireSpell::getConfiguredPositionOffset() const {
	return sf::Vector2f(-m_data.range, -m_data.range);
}

void HolyFireSpell::execOnHit(LevelMovableGameObject* target) {
	m_hasDamaged = true;
}

void HolyFireSpell::setPosition(const sf::Vector2f& pos) {
	Spell::setPosition(pos);
	updateParticleSystemPosition();
}

void HolyFireSpell::render(sf::RenderTarget& target) {
	Spell::render(target);
	m_ps->render(target);
}

void HolyFireSpell::update(const sf::Time& frameTime) {
	Spell::update(frameTime);
	m_ps->update(frameTime);
	if (m_emitTime > sf::Time::Zero) {
		GameObject::updateTime(m_emitTime, frameTime);
		if (m_emitTime == sf::Time::Zero) {
			m_ps->emitRate = 0.f;
		}
	}
	if (m_hasDamaged) {
		m_data.damageType = DamageType::VOID;
	}
}

void HolyFireSpell::loadParticleSystem() {
	m_ps = std::unique_ptr<particles::TextureParticleSystem>(new particles::TextureParticleSystem(static_cast<int>(m_data.range), g_resourceManager->getTexture(ResourceID::Texture_Particle_flame)));
	m_ps->additiveBlendMode = true;
	m_ps->emitRate = m_data.range;

	// Generators
	auto posGen = m_ps->addGenerator<particles::DiskPositionGenerator>();
	posGen->center = sf::Vector2f(getPosition().x + getBoundingBox()->width / 2.f, getPosition().y + getBoundingBox()->height / 2.f);
	posGen->radius = m_data.range;
	m_posGenerator = posGen;

	auto sizeGen = m_ps->addGenerator<particles::SizeGenerator>();
	sizeGen->minStartSize = 5.f;
	sizeGen->maxStartSize = 10.f;
	sizeGen->minEndSize = 10.f;
	sizeGen->maxEndSize = 30.f;

	auto colGen = m_ps->addGenerator<particles::ColorGenerator>();
	if (m_data.skinNr == 1) {
		// shadow fire
		colGen->minStartCol = sf::Color(78, 60, 122, 50);
		colGen->maxStartCol = sf::Color(215, 181, 221, 100);
		colGen->minEndCol = sf::Color(20, 0, 30, 30);
		colGen->maxEndCol = sf::Color(25, 5, 35, 50);
	}
	else {
		// default holy fire
		colGen->minStartCol = sf::Color(162, 7, 0, 50);
		colGen->maxStartCol = sf::Color(234, 148, 0, 100);
		colGen->minEndCol = sf::Color(255, 244, 127, 30);
		colGen->maxEndCol = sf::Color(255, 255, 255, 50);
	}
	

	auto velGen = m_ps->addGenerator<particles::AngledVelocityGenerator>();
	velGen->minAngle = -20.f;
	velGen->maxAngle = 20.f;
	velGen->minStartVel = 10.f;
	velGen->maxStartVel = 30.f;

	auto timeGen = m_ps->addGenerator<particles::TimeGenerator>();
	timeGen->minTime = 1.f;
	timeGen->maxTime = 1.2f;

	// Updaters
	m_ps->addUpdater<particles::TimeUpdater>();
	m_ps->addUpdater<particles::ColorUpdater>();
	m_ps->addUpdater<particles::EulerUpdater>();
	m_ps->addUpdater<particles::SizeUpdater>();
}

void HolyFireSpell::updateParticleSystemPosition() {
	if (m_posGenerator == nullptr) return;
	m_posGenerator->center.x = getPosition().x + getBoundingBox()->width / 2;
	m_posGenerator->center.y = getPosition().y + getBoundingBox()->height / 2;
}

