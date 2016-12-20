#include "SpearKnight.h"
#include "GameStatusBoard.h"
float delay = 500.0f;

SpearKnight::SpearKnight(eStatus status, GVector2 pos, int direction) : BaseEnemy(eID::SPEARKNIGHT) {
	_sprite = SpriteManager::getInstance()->getSprite(eID::SPEARKNIGHT);
	_sprite->setFrameRect(0, 0, 32.0f, 16.0f);

	GVector2 v(direction * SPEARKNIGHT_SPEED, 0);
	GVector2 a(0, 0);
	if (direction > 0) {
		_movingDirection = eDirection::RIGHT;
	}
	else _movingDirection = eDirection::LEFT;
	this->_listComponent.insert(pair<string, IComponent*>("Movement", new Movement(a, v, this->_sprite)));
	this->setStatus(status);
	this->setPosition(pos);
	this->setScale(SCALE_FACTOR);
	this->setScaleX(direction * SCALE_FACTOR);
	this->setPhysicBodySide(eDirection::ALL);
}

SpearKnight::SpearKnight(eStatus status, float x, float y, int direction) : 
	BaseEnemy(eID::SPEARKNIGHT) {

	_sprite = SpriteManager::getInstance()->getSprite(eID::SPEARKNIGHT);
	//_sprite->setFrameRect(0, 0, 32.0f, 16.0f);
	_sprite->setFrameRect(0, 0, 17.0f, 32.0f);
	if (direction > 0) {
		_movingDirection = eDirection::RIGHT;
	}
	else _movingDirection = eDirection::LEFT;
	GVector2 pos(x, y);
	GVector2 v(direction * SPEARKNIGHT_SPEED, 0);
	GVector2 a(0, 0);
	this->_listComponent.insert(pair<string, IComponent*>("Movement", 
		new Movement(a, v, this->_sprite)));
	
	this->setStatus(status);
	this->setPosition(pos);
	this->setScale(SCALE_FACTOR);
	this->setScaleX(direction * SCALE_FACTOR);
	this->setPhysicBodySide(eDirection::ALL);
}


SpearKnight::~SpearKnight() {}

void SpearKnight::init() {
	this->setHitpoint(SPEARKNIGHT_HITPOINT);
	this->setScore(SPEARKNIGHT_SCORE);

	this->_listComponent.insert(pair<string, IComponent*>("Gravity", 
		new Gravity(GVector2(0, -ENEMY_GRAVITY), (Movement*)(this->getComponent("Movement")))));

	auto collisionBody = new CollisionBody(this);
	_listComponent["CollisionBody"] = collisionBody;

	_animations[eStatus::NORMAL] = new Animation(_sprite, 0.1f);
	_animations[eStatus::NORMAL]->addFrameRect(eID::SPEARKNIGHT, "normal", NULL);

	_animations[WALKING] = new Animation(_sprite, 0.15f);
	_animations[WALKING]->addFrameRect(eID::SPEARKNIGHT, "walk_01", "walk_02", "walk_03", NULL);

	_animations[DYING] = new Animation(_sprite, 0.15f);
	_animations[DYING]->addFrameRect(eID::SPEARKNIGHT, NULL);


	//*Test
	//this->setPosition(GVector2(100,100));
	this->setStatus(eStatus::WALKING);
	_sprite->drawBounding(false);
	this->setPhysicBodySide(eDirection::ALL);
}

void SpearKnight::draw(LPD3DXSPRITE spritehandle, Viewport* viewport) {
	if (this->isInStatus(eStatus::DESTROY)) return;

	if (_burning != NULL)
		_burning->draw(spritehandle, viewport);
	if (this->getStatus() == eStatus::DESTROY || this->getStatus() == eStatus::BURN)
		return;

	_animations[WALKING]->draw(spritehandle, viewport);

}

void SpearKnight::release() {
	for (auto component : _listComponent) {
		delete component.second;
	}
	_listComponent.clear();

	if (this->_burning != NULL)
		this->_burning->release();
	SAFE_DELETE(this->_burning);

	SAFE_DELETE(this->_loopwatch);
	//SAFE_DELETE(this->_stopwatch);

	SAFE_DELETE(this->_sprite);
}

IComponent* SpearKnight::getComponent(string componentName) {
	return _listComponent.find(componentName)->second;
}

void SpearKnight::update(float deltatime) {
	

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



	for (auto it : _listComponent) {
		it.second->update(deltatime);
	}

	updateDirection();

	_animations[WALKING]->update(deltatime);
}

void SpearKnight::setPosition(GVector2 pos) {
	_sprite->setPosition(pos);
}

void  SpearKnight::updateDirection() {
	if (!_checkedWithLand) return;
	
	//if (_loopwatch == nullptr) {
	//	_loopwatch = new StopWatch();
	//}

	//if (_loopwatch != nullptr && _loopwatch->isStopWatch(2000)) {
	//	if (_movingDirection == eDirection::RIGHT)
	//		changeDirection(eDirection::LEFT);
	//	else changeDirection(eDirection::RIGHT);
	//	SAFE_DELETE(_loopwatch);
	//}

	if (this->getPositionX() > _checkedLandBounding.right - 16) {
		changeDirection(LEFT);
	}
	else if (this->getPositionX() < _checkedLandBounding.left + 16) {
		changeDirection(RIGHT);
	}
}

void SpearKnight::changeDirection(eDirection dir) 
{
	if (!_checkedWithLand) return;
	
	if (_movingDirection == dir)
		return;
	_movingDirection = dir;
	Movement *movement = (Movement*)this->getComponent("Movement");

	if (_movingDirection == eDirection::RIGHT) {
		if (this->getScale().x < 0) this->setScaleX(this->getScale().x * (-1));
		movement->setVelocity(GVector2(SPEARKNIGHT_SPEED, 0));
	}
	else if (_movingDirection == eDirection::LEFT) 		{
		if (this->getScale().x > 0) this->setScaleX(this->getScale().x * (-1));
		movement->setVelocity(GVector2(-SPEARKNIGHT_SPEED, 0));
	}
		
}


float SpearKnight::checkCollision(BaseObject * object, float dt) {
	if (this->getStatus() == eStatus::DESTROY || 
		this->isInStatus(eStatus::DYING ) || this->isInStatus(eStatus::BURN))
		return 0.0f;

	auto collisionBody = (CollisionBody*)_listComponent["CollisionBody"];
	eID objectId = object->getId();
	eDirection direction;
	if (objectId != eID::LAND && objectId != eID::SIMON) return 0.0f;
	if (objectId == eID::LAND) {
		if (collisionBody->checkCollision(object, direction, dt,false)) {
			if (direction == eDirection::TOP && this->getVelocity().y < 0) {
				auto gravity = (Gravity*)this->_listComponent["Gravity"];
				auto movement = (Movement*)this->_listComponent["Movement"];
				movement->setVelocity(GVector2(movement->getVelocity().x, 0));
				gravity->setStatus(eGravityStatus::SHALLOWED);

				this->setStatus(eStatus::WALKING);
				_checkedWithLand = true;
				_checkedLandBounding = object->getBounding();

				prevObject = object;
			}
		}
		
		return 0.0f;
	}
	else
	if (objectId == eID::SIMON)
	{
		if (collisionBody->checkCollision(object, direction, dt,false))
		{
			((Simon*)object)->getHitted();
		}
		return 0.0f;
	}
	return 0.0f;
}

void SpearKnight::updateCurrentAnimateIndex() {

	if (this->isInStatus(eStatus::WALKING)) {
		_currentAnimateIndex = eStatus::WALKING;
	}

	if ((_currentAnimateIndex & eStatus::FALLING) == eStatus::FALLING) {
		_currentAnimateIndex = eStatus::FALLING;
	}

	// chết
	if (this->isInStatus(eStatus::DYING)) {
		_currentAnimateIndex = eStatus::DYING;
	}
}


GVector2 SpearKnight::getVelocity() {
	auto move = (Movement*)this->_listComponent["Movement"];
	return move->getVelocity();
}

void SpearKnight::die() {
	Gravity *gravity = (Gravity*)this->getComponent("Gravity");
	gravity->setStatus(eGravityStatus::SHALLOWED);
	Movement *movement = (Movement*)this->getComponent("Movement");
	//movement->setVelocity(GVector2(0, 200));
	//this->setStatus(eStatus::DESTROY);
}


void SpearKnight::getHitted()
{

}