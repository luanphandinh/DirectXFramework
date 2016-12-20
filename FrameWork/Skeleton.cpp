#include "Skeleton.h"

Skeleton::Skeleton(eStatus status, GVector2 pos, int direction) : BaseEnemy(eID::SKELETON) {
	_sprite = SpriteManager::getInstance()->getSprite(eID::SKELETON);
	_sprite->setFrameRect(0, 0, 32.0f, 16.0f);

	GVector2 v(0, 0);
	GVector2 a(0, 0);
	this->_listComponent.insert(pair<string, IComponent*>("Movement", new Movement(a, v, this->_sprite)));
	this->setStatus(status);
	this->setPosition(pos);
	this->setScale(SCALE_FACTOR);
	this->setScaleX(direction * SCALE_FACTOR);
	this->setPhysicBodySide(eDirection::ALL);
}

Skeleton::~Skeleton() {
}

void Skeleton::init() {
	this->setHitpoint(SKELETON_HITPOINT);
	this->setScore(SKELETON_SCORE);

	this->_listComponent.insert(pair<string, IComponent*>("Gravity",
		new Gravity(GVector2(0, -ENEMY_GRAVITY), (Movement*)(this->getComponent("Movement")))));

	auto collisionBody = new CollisionBody(this);
	_listComponent["CollisionBody"] = collisionBody;

	_animations[eStatus::NORMAL] = new Animation(_sprite, 0.1f);
	_animations[eStatus::NORMAL]->addFrameRect(eID::SKELETON, "normal", NULL);

	_animations[JUMP] = new Animation(_sprite, 0.15f);
	_animations[JUMP]->addFrameRect(eID::SKELETON, "move_01", "move_02", NULL);

	_animations[HIGHJUMP] = new Animation(_sprite, 0.15f);
	_animations[HIGHJUMP]->addFrameRect(eID::SKELETON, "move_01", "move_02", NULL);

	_animations[DYING] = new Animation(_sprite, 0.15f);
	_animations[DYING]->addFrameRect(eID::SKELETON, NULL);

	_stopWatch = new StopWatch();

	//*Test
	//this->setPosition(GVector2(100,100));
	this->setStatus(eStatus::NORMAL);
	_sprite->drawBounding(false);
	this->setPhysicBodySide(eDirection::ALL);
}

void Skeleton::update(float deltatime) {
	if (this->getStatus() == DESTROY)
		return;

	if (this->getHitpoint() <= 0 && this->getStatus() != DYING) {
		this->setStatus(eStatus::DYING);
		this->die();
	}

	if (this->getStatus() == eStatus::DYING) {
		this->setStatus(eStatus::BURN);
	}

	// Bị nướng
	if (this->getStatus() == eStatus::BURN) {
		if (_burning == nullptr) {
			auto pos = this->getPosition();
			_burning = new HitEffect(2, pos);
			_burning->init();
		}
		else {
			_burning->update(deltatime);
			if (_burning->getStatus() == eStatus::DESTROY) {
				this->setStatus(eStatus::DESTROY);
			}
		}
	}
	//if (this->checkIfOutOfScreen()) return;
	//if (this->getStatus() == eStatus::SITTING) {
	//	this->updateSitting();
	//	return;
	//}
	//else {

		for (auto component : _listComponent) {
			component.second->update(deltatime);
		}
		_animations[this->getStatus()]->update(deltatime);
	/*}*/
}

void Skeleton::draw(LPD3DXSPRITE spritehandle, Viewport *viewport) {
	if (this->isInStatus(eStatus::DESTROY)) return;

	if (_burning != NULL)
		_burning->draw(spritehandle, viewport);
	if (this->getStatus() == eStatus::DESTROY || this->getStatus() == eStatus::BURN)
		return;

	_animations[this->getStatus()]->draw(spritehandle, viewport);

}

void Skeleton::release() {
	for (auto component : _listComponent) {
		delete component.second;
	}
	_listComponent.clear();

	if (this->_burning != NULL)
		this->_burning->release();
	SAFE_DELETE(this->_burning);

	SAFE_DELETE(this->_stopWatch);
	//SAFE_DELETE(this->_stopwatch);

	SAFE_DELETE(this->_sprite);
}

float Skeleton::checkCollision(BaseObject *object, float dt) {
	if (this->getStatus() == eStatus::DESTROY ||
		this->isInStatus(eStatus::DYING) || this->isInStatus(eStatus::BURN))
		return 0.0f;

	auto collisionBody = (CollisionBody*)_listComponent["CollisionBody"];
	eID objectId = object->getId();
	eDirection direction;
	if (objectId != eID::LAND && objectId != eID::SIMON) return 0.0f;
	if (objectId == eID::LAND) {
		if (collisionBody->checkCollision(object, direction, dt)) {
			if (direction == eDirection::TOP && this->getVelocity().y < 0) {
				auto gravity = (Gravity*)this->_listComponent["Gravity"];
				auto movement = (Movement*)this->_listComponent["Movement"];
				movement->setVelocity(GVector2(movement->getVelocity().x, 0));
				gravity->setStatus(eGravityStatus::SHALLOWED);

				//this->setStatus(eStatus::JUMP);
				prevObject = object;
			}
			else if (prevObject == object) {

				/*auto gravity = (Gravity*)this->_listComponent["Gravity"];

				int chance = rand() % 100;
				if (chance % 2 == 0) {
					jump();
				}
				else {
					highJump();
				}
				gravity->setStatus(eGravityStatus::FALLING_DOWN);*/

				prevObject = nullptr;
			}
		}

		return 0.0f;
	}
	else
		if (objectId == eID::SIMON) {
			if (collisionBody->checkCollision(object, direction, dt, false)) {

				((Simon*)object)->getHitted();
			}


			return 0.0f;
		}
	return 0.0f;
}

void Skeleton::die() {
	Gravity *gravity = (Gravity*)this->getComponent("Gravity");
	gravity->setStatus(eGravityStatus::SHALLOWED);
}

void Skeleton::setPosition(GVector2 pos) {
	_sprite->setPosition(pos);

}

GVector2 Skeleton::getVelocity() {
	auto move = (Movement*)this->_listComponent["Movement"];
	return move->getVelocity();
}

IComponent * Skeleton::getComponent(string componentName) {
	return _listComponent.find(componentName)->second;

}

void Skeleton::changeDirection() {
	_sprite->setScaleX(-this->getScale().x);
	Movement *movement = (Movement*)this->getComponent("Movement");
	movement->setVelocity(GVector2(-movement->getVelocity().x, 0));
}

void Skeleton::backward() {
	_sprite->setScaleX(-this->getScale().x);
	Movement *movement = (Movement*)this->getComponent("Movement");
	movement->setVelocity(GVector2(-movement->getVelocity().x, 0));
}

void Skeleton::forward() {
	_sprite->setScaleX(this->getScale().x);
	Movement *movement = (Movement*)this->getComponent("Movement");
	movement->setVelocity(GVector2(movement->getVelocity().x, 0));
}

void Skeleton::jump() {
	this->setStatus(eStatus::JUMP);

	auto move = (Movement*)this->_listComponent["Movement"];
	move->setVelocity(GVector2( 85, 250));
}

