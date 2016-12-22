#include "Skeleton.h"
#include"Level3.h"

Skeleton::Skeleton(eStatus status, GVector2 pos, int direction) : BaseEnemy(eID::SKELETON) {
	_sprite = SpriteManager::getInstance()->getSprite(eID::SKELETON);
	if (direction > 0) {
		_jumpingDirection = eDirection::RIGHT;
	}
	else _jumpingDirection = eDirection::LEFT;
	_direction = direction;
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


	this->setStatus(eStatus::NORMAL);
	_sprite->drawBounding(false);
	this->setPhysicBodySide(eDirection::ALL);
}

void Skeleton::update(float deltatime) {

	if (this->getStatus() == DESTROY)
		return;

	if (this->getHitpoint() <= 0) {
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
		return;
	}

	if (this->checkIfOutOfScreen()) return;

	if (!_canJump) {
		this->updateSitting();
		return;
	}

	updateDirection();

	jump();

	for (auto component : _listComponent) {
		component.second->update(deltatime);
	}

	_animations[this->getStatus()]->update(deltatime);

}

void Skeleton::draw(LPD3DXSPRITE spritehandle, Viewport *viewport) {
	if (this->isInStatus(eStatus::DESTROY)) return;

	if (_burning != NULL)
		_burning->draw(spritehandle, viewport);
	if (this->getStatus() == eStatus::DESTROY || this->getStatus() == eStatus::BURN)
		return;
	if (this->isInStatus(eStatus::JUMP) || this->isInStatus(eStatus::NORMAL))
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

	SAFE_DELETE(this->_jumpStopWatch);
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
				movement->setVelocity(GVector2(0, 0));
				gravity->setStatus(eGravityStatus::SHALLOWED);

				//this->setStatus(eStatus::JUMP);
				prevObject = object;
			}
			else if (prevObject == object) {

				auto gravity = (Gravity*)this->_listComponent["Gravity"];
				gravity->setStatus(eGravityStatus::FALLING_DOWN);

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
	movement->setVelocity(GVector2(movement->getVelocity().x, 0));
}

void Skeleton::updateCurrentAnimateIndex() {
	if (this->isInStatus(eStatus::HIGHJUMP)) {
		_currentAnimateIndex = eStatus::HIGHJUMP;
	}
	if (this->isInStatus(eStatus::JUMP)) {
		_currentAnimateIndex = eStatus::JUMP;
	}
	if ((_currentAnimateIndex & eStatus::FALLING) == eStatus::FALLING) {
		_currentAnimateIndex = eStatus::FALLING;
	}

	// chết
	if (this->isInStatus(eStatus::DYING)) {
		_currentAnimateIndex = eStatus::DYING;
	}
}

void Skeleton::getHitted() {

}

void Skeleton::updateDirection() {
	BaseObject* _simon = ((Scene*)SceneManager::getInstance()->getCurrentScene())->getDirector()->getObjectTracker();
	GVector2 position = this->getPosition();

	if (_jumpingDirection == eDirection::LEFT && _simon->getPositionX() > position.x) {
		changeDirection(eDirection::RIGHT);
	}
	else if (_jumpingDirection == eDirection::RIGHT && _simon->getPositionX() < position.x) {
		changeDirection(eDirection::LEFT);
	}
}

void Skeleton::changeDirection(eDirection dir) {
	if (_jumpingDirection == dir)
		return;

	_jumpingDirection = dir;

	Movement *movement = (Movement*)this->getComponent("Movement");
	if (_jumpingDirection == eDirection::RIGHT) {
		if (this->getScale().x < 0) this->setScaleX(this->getScale().x * (-1));
		//movement->setVelocity(GVector2(Skeleton_SPEED, 0));
	}
	else if (_jumpingDirection == eDirection::LEFT) {
		if (this->getScale().x > 0) this->setScaleX(this->getScale().x * (-1));
		//movement->setVelocity(GVector2(-Skeleton_SPEED, 0));
	}
}

bool Skeleton::checkIfOutOfScreen() {
	auto viewport = ((Scene*)SceneManager::getInstance()->getCurrentScene())->getViewport();
	RECT screenBound = viewport->getBounding();
	GVector2 vpBound = ((Scene*)SceneManager::getInstance()->getCurrentScene())->getDirector()->getCurrentViewportBound();
	GVector2 position = this->getPosition();

	if (position.x > screenBound.right && _direction > 0) {
		this->setStatus(eStatus::DESTROY);
		return true;
	}
	else if (position.x < screenBound.left && _direction < 0) {
		this->setStatus(eStatus::DESTROY);
		return true;
	}

	if (position.x < vpBound.x + 85) {
		this->setStatus(eStatus::DESTROY);
		return true;
	}

	if (position.x > vpBound.y - 85) {
		this->setStatus(eStatus::DESTROY);
		return true;
	}

	return false;
}

void Skeleton::updateSitting() {
	if (_canJump) return;
	// track theo simon
	auto objectTracker = ((Level3*)SceneManager::getInstance()->getCurrentScene())->getSimon();
	RECT objectBound = objectTracker->getBounding();
	int xSimon = objectTracker->getPositionX();
	int ySimon = objectTracker->getPositionY();
	int xthis = this->getPositionX();
	int ythis = this->getBounding().bottom;
	if (xSimon > xthis&&xSimon < xthis + 250 && ySimon<ythis&&ySimon>ythis - 100) {
		this->setStatus(JUMP);
		_canJump = true;
	}
	else {
		this->setStatus(NORMAL);
	}
}

void Skeleton::jump() {
	if (!_canJump) return;
	if (_jumpStopWatch == nullptr) {
		_jumpStopWatch = new StopWatch();
	}

	if (_jumpStopWatch != nullptr && _jumpStopWatch->isStopWatch(2000)) {
		SAFE_DELETE(_jumpStopWatch);
		auto gravity = (Gravity*)this->_listComponent["Gravity"];
		gravity->setStatus(eGravityStatus::FALLING_DOWN);
		int chance = rand() % 100;
		if (chance % 2 == 0) {
			this->setStatus(eStatus::JUMP);
			auto move = (Movement*)this->_listComponent["Movement"];
			if (_jumpingDirection == eDirection::RIGHT)
				move->setVelocity(GVector2(85, 150));
			else move->setVelocity(GVector2(-85, 150));
		}
		else {
			this->setStatus(eStatus::JUMP);
			auto move = (Movement*)this->_listComponent["Movement"];
			if (_jumpingDirection == eDirection::RIGHT)
				move->setVelocity(GVector2(85, 250));
			else move->setVelocity(GVector2(-85, 250));
		}
	}


}