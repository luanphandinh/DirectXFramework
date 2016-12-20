#include "Fleaman.h"
#include "Level3.h"

Fleaman::Fleaman(eStatus status, GVector2 pos, int direction) : BaseEnemy(eID::FLEAMAN) {
	_sprite = SpriteManager::getInstance()->getSprite(eID::FLEAMAN);
	_sprite->setFrameRect(0, 0, 32.0f, 16.0f);
	if (direction > 0) {
		_movingDirection = eDirection::RIGHT;
	}
	else _movingDirection = eDirection::LEFT;
	GVector2 v(0, 0);
	GVector2 a(0, 0);
	this->_listComponent.insert(pair<string, IComponent*>("Movement", new Movement(a, v, this->_sprite)));
	this->setStatus(status);
	this->setPosition(pos);
	this->setScale(SCALE_FACTOR);
	this->setScaleX(direction * SCALE_FACTOR);
	this->setPhysicBodySide(eDirection::ALL);
}

Fleaman::~Fleaman() {
}

void Fleaman::init() {
	this->setHitpoint(FLEAMAN_HITPOINT);
	this->setScore(FLEAMAN_SCORE);

	this->_listComponent.insert(pair<string, IComponent*>("Gravity",
		new Gravity(GVector2(0, -ENEMY_GRAVITY), (Movement*)(this->getComponent("Movement")))));

	auto collisionBody = new CollisionBody(this);
	_listComponent["CollisionBody"] = collisionBody;

	_animations[eStatus::SITTING] = new Animation(_sprite, 0.1f);
	_animations[eStatus::SITTING]->addFrameRect(eID::FLEAMAN, "normal_01", "normal_02", NULL);

	_animations[JUMP] = new Animation(_sprite, 0.15f);
	_animations[JUMP]->addFrameRect(eID::FLEAMAN, "run_01", "run_02", NULL);

	_animations[HIGHJUMP] = new Animation(_sprite, 0.15f);
	_animations[HIGHJUMP]->addFrameRect(eID::FLEAMAN, "run_01", "run_02", NULL);

	_animations[DYING] = new Animation(_sprite, 0.15f);
	_animations[DYING]->addFrameRect(eID::FLEAMAN, NULL);

	_stopWatch = new StopWatch();

	//*Test
	//this->setPosition(GVector2(100,100));
	this->setStatus(eStatus::SITTING);
	_sprite->drawBounding(false);
	this->setPhysicBodySide(eDirection::ALL);
}

void Fleaman::update(float deltatime) {

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
	if (this->checkIfOutOfScreen()) return;
	if (this->getStatus() == eStatus::SITTING) {
		this->updateSitting();
		return;
	}
	else {

		for (auto component : _listComponent) {
			component.second->update(deltatime);
		}
		_animations[this->getStatus()]->update(deltatime);
	}
}

void Fleaman::draw(LPD3DXSPRITE spritehandle, Viewport *viewport) {
	if (this->isInStatus(eStatus::DESTROY)) return;

	if (_burning != NULL)
		_burning->draw(spritehandle, viewport);
	if (this->getStatus() == eStatus::DESTROY || this->getStatus() == eStatus::BURN)
		return;

	//_animations[_currentAnimateIndex]->draw(spritehandle, viewport);
	_animations[this->getStatus()]->draw(spritehandle, viewport);

}

void Fleaman::release() {
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

float Fleaman::checkCollision(BaseObject *object, float dt) {
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

				auto gravity = (Gravity*)this->_listComponent["Gravity"];

				int chance = rand()%100;
				if (chance % 2 == 0) {
					jump();
				}
				else {
					highJump();
				}
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

void Fleaman::die() {
	Gravity *gravity = (Gravity*)this->getComponent("Gravity");
	gravity->setStatus(eGravityStatus::SHALLOWED);
}

void Fleaman::setPosition(GVector2 pos) {
	_sprite->setPosition(pos);

}

GVector2 Fleaman::getVelocity() {
	auto move = (Movement*)this->_listComponent["Movement"];
	return move->getVelocity();
}

IComponent * Fleaman::getComponent(string componentName) {
	return _listComponent.find(componentName)->second;

}

void Fleaman::changeDirection() {
	_sprite->setScaleX(-this->getScale().x);
	Movement *movement = (Movement*)this->getComponent("Movement");
	movement->setVelocity(GVector2(movement->getVelocity().x, 0));
}

void Fleaman::updateCurrentAnimateIndex() {
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

void Fleaman::getHitted() {
}

void Fleaman::updateDirection() {
	

	
}

void Fleaman::changeDirection(eDirection dir) {
	
}

bool Fleaman::checkIfOutOfScreen() {
	auto viewport = ((Level3*)SceneManager::getInstance()->getCurrentScene())->getViewport();
	RECT screenBound = viewport->getBounding();
	GVector2 position = this->getPosition();

	if (position.x > screenBound.right) {
		this->setStatus(eStatus::DESTROY);
		return true;
	}
	return false;
}

void Fleaman::updateSitting() {
	// track theo simon
	auto objectTracker = ((Level3*)SceneManager::getInstance()->getCurrentScene())->getSimon();
	RECT objectBound = objectTracker->getBounding();
	int xSimon = objectTracker->getPositionX();
	int ySimon = objectTracker->getPositionY();
	int xthis = this->getPositionX();
	int ythis = this->getBounding().bottom;
	if (xSimon > xthis&&xSimon < xthis + 250 && ySimon<ythis&&ySimon>ythis - 100) {
		this->setStatus(JUMP);
	}
	else {
		this->setStatus(SITTING);
	}
}

void Fleaman::jump() {
	this->setStatus(eStatus::JUMP);
	auto move = (Movement*)this->_listComponent["Movement"];
	move->setVelocity(GVector2(85,150));
}


void Fleaman::highJump() {
	this->setStatus(eStatus::HIGHJUMP);

	auto move = (Movement*)this->_listComponent["Movement"];
	move->setVelocity(GVector2(85, 250));
}


