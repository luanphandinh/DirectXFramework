#include "Ghost.h"
#include "PlayScene.h"

Ghost::Ghost(eStatus status, GVector2 pos, int direction) : BaseEnemy(eID::GHOST) {
	_sprite = SpriteManager::getInstance()->getSprite(eID::GHOST);

	if (direction > 0) {
		_flyingDirection = eDirection::RIGHT;
	}
	else _flyingDirection = eDirection::LEFT;

	GVector2 v(direction * GHOST_SPEED, 0);
	GVector2 a(0, 0);
	this->_listComponent.insert(pair<string, IComponent*>("Movement", new Movement(a, v, this->_sprite)));
	this->setStatus(status);
	this->setPosition(pos);

	this->setScale(SCALE_FACTOR);
	this->setScaleX(direction * SCALE_FACTOR);
	this->_direction = direction;
	this->setPhysicBodySide(eDirection::ALL);
}

Ghost::~Ghost() {

}

void Ghost::init() {
	this->setHitpoint(GHOST_HITPOINT);
	this->setScore(GHOST_SCORE);
	this->_listComponent.insert(pair<string, IComponent*>("Gravity",
		new Gravity(GVector2(0, 0), (Movement*)(this->getComponent("Movement")))));

	auto collisionBody = new CollisionBody(this);
	_listComponent["CollisionBody"] = collisionBody;

	_animations[FLYINGUP] = new Animation(_sprite, 0.15f);
	_animations[FLYINGUP]->addFrameRect(eID::GHOST, "fly_02", "fly_01", NULL);

	_animations[FLYING] = new Animation(_sprite, 0.15f);
	_animations[FLYING]->addFrameRect(eID::GHOST, "fly_02", "fly_01", NULL);

	_stopWatch = new StopWatch();

	_isHiding = true;
	this->hack = 0;
	this->setHitpoint(2);
}

void Ghost::update(float deltaTime) {
	if (this->getStatus() == eStatus::DESTROY)
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
			_burning->update(deltaTime);
			if (_burning->getStatus() == eStatus::DESTROY) {
				this->setStatus(eStatus::DESTROY);
			}
		}
		return;
	}


	if (_isHiding) {
		this->updateHiding();
		return;
	}

	updateDirection();

	for (auto component : _listComponent) {
		component.second->update(deltaTime);
	}

	/*if (hack == 30) {
		this->setStatus(FLYING);
		this->fly();
	}*/

	this->flyingUp();

	//this->checkIfOutOfScreen();

	//if (this->getStatus() == eStatus::FLYING || this->getStatus() == eStatus::FLYINGUP)
	_animations[FLYING]->update(deltaTime);
}

void Ghost::draw(LPD3DXSPRITE spritehandle, Viewport *viewport) { 
	//if (_isHiding) return;
	if (_burning != nullptr)
		_burning->draw(spritehandle, viewport);
	if (this->getStatus() == eStatus::DESTROY || this->isInStatus(eStatus::BURN)) return;
	_animations[FLYING]->draw(spritehandle, viewport);

}

void Ghost::release() {
	for (auto component : _listComponent) {
		delete component.second;
	}
	_listComponent.clear();
	if (_burning != nullptr)
		_burning->release();
	SAFE_DELETE(_burning);
	//SAFE_DELETE(this->_loopwatch);
	SAFE_DELETE(this->_sprite);
}



float Ghost::checkCollision(BaseObject *object, float deltaTime) {
	if (this->getStatus() == eStatus::DESTROY ||
		this->isInStatus(eStatus::DYING) ||
		this->isInStatus(eStatus::HIDING))
		return 0.0f;

	auto collisionBody = (CollisionBody*)_listComponent["CollisionBody"];
	eID objectId = object->getId();
	eDirection direction;
	if (objectId != eID::SIMON) return 0.0f;
	if (objectId == eID::SIMON) {
		if (collisionBody->checkCollision(object, direction, deltaTime, false)) {
			auto movement = (Movement*)this->_listComponent["Movement"];
		/*
			Phần check collision thằng simon ko liên quan nữa nha
			Chuyển hết sang cho cái roi nó check rồi,chỉ check lúc đụng simon thì cho simon getHitted()
			với mất máu thôi
		*/
			((Simon*)object)->getHitted();
		}

		return 0.0f;
	}
	return 0.0f;

}

void Ghost::die() {

}

void Ghost::setPosition(GVector2 pos) {
	_sprite->setPosition(pos);

}

GVector2 Ghost::getVelocity() {
	auto move = (Movement*)this->_listComponent["Movement"];
	return move->getVelocity();
}

int Ghost::getDirection() {
	return _direction;
}

IComponent * Ghost::getComponent(string componentName) {
	return _listComponent.find(componentName)->second;
}

void Ghost::updateDirection()
{
	BaseObject* _simon = ((Scene*)SceneManager::getInstance()->getCurrentScene())->getDirector()->getObjectTracker();
	GVector2 position = this->getPosition();

	if (_flyingDirection == eDirection::LEFT && _simon->getPositionX() > position.x) {
		changeDirection(eDirection::RIGHT);
	}else if(_flyingDirection == eDirection::RIGHT && _simon->getPositionX() < position.x) {
		changeDirection(eDirection::LEFT);
	}

}

void Ghost::changeDirection(eDirection dir) 
{
	if (_flyingDirection == dir)
		return;

	_flyingDirection = dir;

	Movement *movement = (Movement*)this->getComponent("Movement");
	if (_flyingDirection == eDirection::RIGHT) {
		if (this->getScale().x < 0) this->setScaleX(this->getScale().x * (-1));
		movement->setVelocity(GVector2(GHOST_SPEED, 0));
	}
	else if (_flyingDirection == eDirection::LEFT) {
		if (this->getScale().x > 0) this->setScaleX(this->getScale().x * (-1));
		movement->setVelocity(GVector2(-GHOST_SPEED, 0));
	}
}
//void Ghost::changeDirection() {
//	_sprite->setScaleX(-this->getScale().x);
//	Movement *movement = (Movement*)this->getComponent("Movement");
//	movement->setVelocity(GVector2(-movement->getVelocity().x, 0));
//}

void Ghost::flyingUp() {
	BaseObject* _simon = ((Scene*)SceneManager::getInstance()->getCurrentScene())->getDirector()->getObjectTracker();
	RECT objectBound = _simon->getBounding();
	auto viewportTracker = ((Scene*)SceneManager::getInstance()->getCurrentScene())->getDirector()->getViewport();
	RECT vpBound = viewportTracker->getBounding();
	Movement *movement = (Movement*)this->getComponent("Movement");
	movement->setVelocity(GVector2(movement->getVelocity().x, +5));
}

void Ghost::fly() {
	Movement *movement = (Movement*)this->getComponent("Movement");
	movement->setVelocity(GVector2(movement->getVelocity().x, 0));
}

void Ghost::checkIfOutOfScreen() {
}

void Ghost::updateHiding() {
	// track theo simon
	if (!_isHiding) return;
	auto objectTracker = ((Scene*)SceneManager::getInstance()->getCurrentScene())->getDirector()->getObjectTracker();
	RECT objectBound = objectTracker->getBounding();
	int x = objectTracker->getPositionX();
	int y = objectTracker->getPositionY();
	int xthis = this->getPositionX();
	int ythis = this->getBounding().bottom;

	if (this->getDirection() == -1 && objectBound.right < this->getBounding().left - 100 && objectBound.top>this->getBounding().bottom) {
		//this->setStatus(FLYINGUP);
		_isHiding = false;
	}
	else if (this->getDirection() == 1 && objectBound.left > this->getBounding().right + 100 && objectBound.top>this->getBounding().bottom) {
		//this->setStatus(FLYINGUP);
		_isHiding = false;
	}
}
