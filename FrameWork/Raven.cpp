#include "Raven.h"
#include "PlayScene.h"
Raven::Raven(eStatus status, GVector2 pos, int direction) : BaseEnemy(eID::RAVEN) {
	_sprite = SpriteManager::getInstance()->getSprite(eID::RAVEN);
	_sprite->setFrameRect(0, 0, 32.0f, 16.0f);

	if (direction > 0) {
		_flyingDirection = eDirection::RIGHT;
	}
	else _flyingDirection = eDirection::LEFT;
	GVector2 v(0, 0);
	GVector2 a(0, 0);
	this->_listComponent.insert(pair<string, IComponent*>("Movement", new Movement(a, v, this->_sprite)));
	this->setStatus(status);
	this->setPosition(pos);
	this->setScale(SCALE_FACTOR);
	this->_direction = direction;

	this->setScaleX(direction * SCALE_FACTOR);
}

Raven::~Raven() {
}

void Raven::init() {
	this->setHitpoint(RAVEN_HITPOINT);
	this->setScore(RAVEN_SCORE);
	this->_listComponent.insert(pair<string, IComponent*>("Gravity",
		new Gravity(GVector2(0, 0), (Movement*)(this->getComponent("Movement")))));

	auto collisionBody = new CollisionBody(this);
	_listComponent["CollisionBody"] = collisionBody;


	_animations[eStatus::LANDING] = new Animation(_sprite, 0.1f);
	_animations[eStatus::LANDING]->addFrameRect(eID::RAVEN, "normal", NULL);

	_animations[FLYING] = new Animation(_sprite, 0.15f);
	_animations[FLYING]->addFrameRect(eID::RAVEN, "fly_01", "fly_02", "fly_03",NULL);

	_animations[DYING] = new Animation(_sprite, 0.15f);
	_animations[DYING]->addFrameRect(eID::RAVEN, NULL);

	_isLanding = true;
	_flyDown = false;
	fuck = false;

	this->setStatus(eStatus::LANDING);
	_sprite->drawBounding(false);

	this->hack = 0;
	this->setPhysicBodySide(eDirection::ALL);
	this->setHitpoint(1);
}

void Raven::update(float deltaTime) {
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


	if (_isLanding) {
		this->updateLanding();
		//updateDirection();

		return;
	}

	fly();

	for (auto component : _listComponent) {
		component.second->update(deltaTime);
	}
	_animations[this->getStatus()]->update(deltaTime);

}

void Raven::draw(LPD3DXSPRITE spritehandle, Viewport *viewport) {
	if (_burning != nullptr)
		_burning->draw(spritehandle, viewport);
	if (this->isInStatus(eStatus::DESTROY) || this->isInStatus(eStatus::BURN)) return;
	_animations[this->getStatus()]->draw(spritehandle, viewport);

}

void Raven::release() {
	for (auto component : _listComponent) {
		delete component.second;
	}
	_listComponent.clear();
	if (_burning != nullptr)
		_burning->release();
	//SAFE_DELETE(this->_loopwatch);
	SAFE_DELETE(this->_sprite);
	SAFE_DELETE(this->_stopWatch);
}

float Raven::checkCollision(BaseObject *object, float deltaTime) {
	if (this->getStatus() == eStatus::DESTROY)
		return 0.0f;

	if (_isLanding) return 0.0f;

	auto collisionBody = (CollisionBody*)_listComponent["CollisionBody"];
	eID objectId = object->getId();
	eDirection direction;
	if (objectId != eID::SIMON && objectId != eID::WHIP && objectId != eID::ITEM) return 0.0f;
	if (collisionBody->checkCollision(object, direction, deltaTime, false)) {
		if (objectId == eID::SIMON) {
			((Simon*)object)->getHitted();
			this->dropHitpoint(1);
		}
		else if (objectId == eID::WHIP && ((Whip*)object)->isHitting()) {
			this->dropHitpoint(1);
		}
		else if (objectId == eID::ITEM && ((Item*)object)->getItemType() == eItemType::PICKED_UP) {
			this->setStatus(eStatus::BURN);
		}

		return 0.0f;
	}
	return 0.0f;
}

void Raven::die() {

}

void Raven::setPosition(GVector2 pos) {
	_sprite->setPosition(pos);

}

GVector2 Raven::getVelocity() {
	auto move = (Movement*)this->_listComponent["Movement"];
	return move->getVelocity();
}

IComponent * Raven::getComponent(string componentName) {
	return _listComponent.find(componentName)->second;
}

int Raven::getDirection() {
	return _direction;
}

void Raven::updateDirection() {
	BaseObject* _simon = ((Scene*)SceneManager::getInstance()->getCurrentScene())->getDirector()->getObjectTracker();
	GVector2 position = this->getPosition();

	if (_flyingDirection == eDirection::LEFT && _simon->getPositionX() > position.x) {
		changeDirection(eDirection::RIGHT);
	}
	else if (_flyingDirection == eDirection::RIGHT && _simon->getPositionX() < position.x) {
		changeDirection(eDirection::LEFT);
	}
}

void Raven::changeDirection(eDirection dir) {
	if (_flyingDirection == dir)
		return;

	_flyingDirection = dir;

	Movement *movement = (Movement*)this->getComponent("Movement");
	if (_flyingDirection == eDirection::RIGHT) {
		if (this->getScale().x < 0) this->setScaleX(this->getScale().x * (-1));
		//movement->setVelocity(GVector2(RAVEN_SPEED, 0));
	}
	else if (_flyingDirection == eDirection::LEFT) {
		if (this->getScale().x > 0) this->setScaleX(this->getScale().x * (-1));
		//movement->setVelocity(GVector2(-RAVEN_SPEED, 0));
	}
}

void Raven::fly() {
	if (_isLanding) return;
	BaseObject* _simon = ((Scene*)SceneManager::getInstance()->getCurrentScene())->getDirector()->getObjectTracker();
	RECT objectBound = _simon->getBounding();
	auto viewportTracker = ((Scene*)SceneManager::getInstance()->getCurrentScene())->getDirector()->getViewport();
	RECT vpBound = viewportTracker->getBounding();

	auto movement = (Movement*)this->getComponent("Movement");

	if (_stopWatch == nullptr) {
		_stopWatch = new StopWatch();
	}



	if (_stopWatch != nullptr&&_stopWatch->isStopWatch(1000)) {
		SAFE_DELETE(_stopWatch);
		if (this->getStatus() == FLYING) {
			movement->setVelocity(GVector2(-80,-100));
		}
	}


	int xthis = this->getPositionX();
	int ythis = this->getBounding().bottom;
	if (ythis < objectBound.top - 30) {
		movement->setVelocity(GVector2(80, 0));
		if (xthis > objectBound.right + 190)
			fuck = true;
	}

	if (fuck) {
		movement->setVelocity(GVector2(-120, 0));
	}
}

bool Raven::checkIfOutOfScreen() {
	return false;
}

void Raven::updateLanding() {
	// track theo simon
	if (!_isLanding) return;
	auto objectTracker = ((Scene*)SceneManager::getInstance()->getCurrentScene())->getDirector()->getObjectTracker();
	RECT objectBound = objectTracker->getBounding();
	int x = objectTracker->getPositionX();
	int y = objectTracker->getPositionY();
	int xthis = this->getPositionX();
	int ythis = this->getBounding().bottom;

	if (this->getDirection() == -1 && objectBound.right > this->getBounding().left - 185&& objectBound.top < this->getBounding().bottom) {
		this->setStatus(FLYING);
		_isLanding = false;
	}
	else if (this->getDirection() == -1 && objectBound.right > this->getBounding().left - 70) {
		this->setStatus(FLYING);
		_isLanding = false;
	}
	else {
		this->setStatus(LANDING);
	}
}
