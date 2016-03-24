#include "Level/MOBBehavior/MovingBehavior.h"
#include "Screens/LevelScreen.h"
#include "Level/DynamicTiles/MovingTile.h"

MovingBehavior::MovingBehavior(LevelMovableGameObject* mob) {
	m_mob = mob;
	m_mainChar = dynamic_cast<LevelScreen*>(mob->getScreen())->getMainCharacter();
}

void MovingBehavior::update(const sf::Time& frameTime) {
	
	if (!m_isCollisionTilt) {
		handleMovementInput();

		sf::Vector2f nextPosition;
		m_mob->calculateNextPosition(frameTime, nextPosition);
		sf::Vector2f oldPosition = m_mob->getPosition();
		checkCollisions(nextPosition);
		checkForCollisionTilt(oldPosition);
	}

	// update animation
	GameObject::updateTime(m_fightAnimationTime, frameTime);
	updateAnimation();
}

void MovingBehavior::checkForCollisionTilt(const sf::Vector2f& oldPosition) {
	if (m_isCollisionTiltSuppressed || m_mob->isImmortal()) {
		m_isCollisionTiltSuppressed = false;
		return;
	}

	if (dist(oldPosition, m_mob->getPosition()) > TILE_SIZE_F / 2.f + norm(m_mob->getVelocity())) {
		m_mob->setPosition(oldPosition);
		m_mob->setMovingParent(nullptr);
		m_mob->setVelocity(sf::Vector2f(0.f, 0.f));
		m_mob->setAcceleration(sf::Vector2f(0.f, 0.f));
		m_isCollisionTilt = true;
		m_mob->setDead();
	}
}

void MovingBehavior::calculateUnboundedVelocity(const sf::Time& frameTime, sf::Vector2f& nextVel) const {
	// distinguish damping in the air and at the ground
	float dampingPerSec = (m_mob->getVelocity().y == 0.0f) ? m_dampingGroundPerS : m_dampingAirPerS;
	// don't damp when there is active acceleration 
	if (m_mob->getAcceleration().x != 0.0f) dampingPerSec = 0;
	nextVel.x = (m_mob->getVelocity().x + m_mob->getAcceleration().x * frameTime.asSeconds()) * pow(1 - dampingPerSec, frameTime.asSeconds());
	nextVel.y = m_mob->getVelocity().y + m_mob->getAcceleration().y * frameTime.asSeconds();
}

void MovingBehavior::setFightAnimation() {
	m_fightAnimationTime = m_configuredFightAnimationTime;
}

void MovingBehavior::flipGravity() {
	m_isFlippedGravity = !m_isFlippedGravity;
}

void MovingBehavior::setGravityScale(float scale) {
	m_gravity = scale * m_configuredGravity;
	m_maxVelocityYDown = scale * m_configuredMaxVelocityYDown;
}

void MovingBehavior::setMaxXVelocityScale(float scale) {
	m_maxVelocityX = scale * m_configuredMaxVelocityX;
}

void MovingBehavior::setIgnoreDynamicTiles(bool value) {
	m_ignoreDynamicTiles = value;
}

void MovingBehavior::setWalkAcceleration(float acceleration) {
	m_walkAcceleration = acceleration;
}

void MovingBehavior::setGravityAcceleration(float acceleration) {
	m_configuredGravity = acceleration;
	m_gravity = acceleration;
}

float MovingBehavior::getGravity() const {
	return m_gravity;
}

bool MovingBehavior::isFacingRight() const {
	return m_isFacingRight;
}

bool MovingBehavior::isGrounded() const {
	return m_isGrounded;
}

bool MovingBehavior::isUpsideDown() const {
	return m_isFlippedGravity;
}

bool MovingBehavior::isIgnoreDynamicTiles() const {
	return m_ignoreDynamicTiles;
}

void MovingBehavior::setFightAnimationTime(const sf::Time& fightAnimationTIme) {
	m_configuredFightAnimationTime = fightAnimationTIme;
}

void MovingBehavior::setDampingGroundPerS(float damping) {
	m_dampingGroundPerS = damping;
}

void MovingBehavior::setDampingAirPerS(float damping) {
	m_dampingAirPerS = damping;
}

void MovingBehavior::setMaxVelocityX(float vel) {
	m_configuredMaxVelocityX = vel;
	m_maxVelocityX = vel;
}

void MovingBehavior::setMaxVelocityYUp(float vel) {
	m_configuredMaxVelocityYUp = vel;
	m_maxVelocityYUp = vel;
}

void MovingBehavior::setMaxVelocityYDown(float vel) {
	m_configuredMaxVelocityYDown = vel;
	m_maxVelocityYDown = vel;
}

float MovingBehavior::getMaxVelocityX() const {
	return m_maxVelocityX;
}

float MovingBehavior::getMaxVelocityYDown() const {
	return m_maxVelocityYDown;
}

float MovingBehavior::getMaxVelocityYUp() const {
	return m_maxVelocityYUp;
}

void MovingBehavior::checkXYDirection(const sf::Vector2f& nextPosition, bool& collidesX, bool& collidesY) {
	collidesX = false;
	collidesY = false;

	const sf::FloatRect& bb = *m_mob->getBoundingBox();
	const Level& level = *m_mob->getLevel();
	sf::FloatRect nextBoundingBoxX(nextPosition.x, bb.top, bb.width, bb.height);
	sf::FloatRect nextBoundingBoxY(bb.left, nextPosition.y, bb.width, bb.height);

	WorldCollisionQueryRecord rec;
	rec.ignoreDynamicTiles = m_ignoreDynamicTiles;

	bool isMovingDown = nextPosition.y > bb.top;
	bool isMovingRight = nextPosition.x > bb.left;

	// should we use strategy 2: try y direction first, then x direction?
	bool tryYfirst = false;
	rec.boundingBox = nextBoundingBoxX;
	rec.collisionDirection = isMovingRight ? CollisionDirection::Right : CollisionDirection::Left;
	if (level.collides(rec)) {
		if (std::abs(nextPosition.x - rec.safeLeft) > std::abs(m_mob->getVelocity().x) + 10.f) {
			tryYfirst = true;
		}
	}

	if (!tryYfirst) {
		// check for collision on x axis
		rec.boundingBox = nextBoundingBoxX;
		rec.collisionDirection = isMovingRight ? CollisionDirection::Right : CollisionDirection::Left;
		collidesX = level.collides(rec);
		if (collidesX) {
			m_mob->setAccelerationX(0.f);
			m_mob->setVelocityX(0.f);
			m_mob->setPositionX(rec.safeLeft);
			nextBoundingBoxY.left = rec.safeLeft;
		}
		else {
			nextBoundingBoxY.left = nextPosition.x;
		}

		// check for collision on y axis
		rec.boundingBox = nextBoundingBoxY;
		rec.collisionDirection = isMovingDown ? CollisionDirection::Down : CollisionDirection::Up;
		bool isFalling = isUpsideDown() != isMovingDown;
		// reset moving parent
		rec.movingParent = nullptr;
		collidesY = level.collides(rec);
		m_mob->setMovingParent(rec.movingParent);
		if (collidesY) {
			if (isFalling) {
				m_isGrounded = true;
			}
			m_mob->setAccelerationY(0.f);
			m_mob->setVelocityY(0.f);
			m_mob->setPositionY(rec.safeTop);
		}
	}
	else {
		// check for collision on y axis
		rec.boundingBox = nextBoundingBoxY;
		rec.collisionDirection = isMovingDown ? CollisionDirection::Down : CollisionDirection::Up;
		bool isFalling = isUpsideDown() != isMovingDown;
		collidesY = level.collides(rec);
		if (collidesY) {
			if (isFalling) {
				m_isGrounded = true;
				m_mob->setMovingParent(rec.movingParent);
			}
			m_mob->setAccelerationY(0.f);
			m_mob->setVelocityY(0.f);
			m_mob->setPositionY(rec.safeTop);
			nextBoundingBoxX.top = rec.safeTop;
		}
		else {
			nextBoundingBoxX.top = nextPosition.y;
		}

		// check for collision on x axis
		rec.boundingBox = nextBoundingBoxX;
		rec.collisionDirection = isMovingRight ? CollisionDirection::Right : CollisionDirection::Left;

		collidesX = level.collides(rec);
		if (collidesX) {
			m_mob->setAccelerationX(0.f);
			m_mob->setVelocityX(0.f);
			m_mob->setPositionX(rec.safeLeft);
		}
	}

	if (std::abs(m_mob->getVelocity().y) > 0.f)
		m_isGrounded = false;

	if ((!isMovingDown && nextBoundingBoxY.top < -bb.height) ||
		(isMovingDown && nextBoundingBoxY.top > level.getWorldRect().top + level.getWorldRect().height)) {
		m_mob->setDead();
	}

	// check for wrong parent
	if (MovingTile* mt = m_mob->getMovingParent()) {
		if (!isUpsideDown()) {
			if (mt->getBoundingBox()->top + Epsilon < m_mob->getBoundingBox()->top + m_mob->getBoundingBox()->height) {
				m_mob->setMovingParent(nullptr);
			}
		}
		else {
			if (mt->getBoundingBox()->top + mt->getBoundingBox()->height > Epsilon + m_mob->getBoundingBox()->top) {
				m_mob->setMovingParent(nullptr);
			}
		}
	}
}
