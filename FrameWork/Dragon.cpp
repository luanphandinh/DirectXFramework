#include "Dragon.h"
#include"PlayScene.h"
#include"Simon.h"
#include"DragonFire.h"
Dragon::Dragon(GVector2 pos) : BaseEnemy(eID::DRAGON)
{
	_sprite = SpriteManager::getInstance()->getSprite(eID::DRAGON);
	_sprite->setFrameRect(SpriteManager::getInstance()->getSourceRect(eID::DRAGON, "normal"));
	_sprite->setOrigin(GVector2(0.5f, 0.5f));
	this->setPosition(pos);
	this->setStatus(eStatus::NORMAL);
	this->setScale(SCALE_FACTOR);
	this->setPhysicBodySide(eDirection::ALL);
	this->setHitpoint(DRAGON_HP);
	_isStand = false;
	_direction = eDirection::LEFT;
}


Dragon::~Dragon()
{
}

void Dragon::init()
{
	Movement* movement = new Movement(GVector2Zero, GVector2Zero, _sprite);
	_listComponent["Movement"] = movement;

	Gravity* gravity = new Gravity(GVector2(0, -800), movement);
	//gravity->setStatus(eGravityStatus::SHALLOWED);
	_listComponent["Gravity"] = gravity;

	CollisionBody* collisionBody = new CollisionBody(this);
	_listComponent["CollisionBody"] = collisionBody;
}

void Dragon::update(float deltatime)
{
	if (this->isInStatus(eStatus::DESTROY))
		return;

	updateDirection();
	fire();

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
	}

	if (_isStand) return;
	for (auto it = _listComponent.begin(); it != _listComponent.end(); it++)
	{
		it->second->update(deltatime);
	}
}

void Dragon::updateDirection()
{
	auto _simon = ((PlayScene*)SceneManager::getInstance()->getCurrentScene())->getSimon();
	auto _simomPos = _simon->getPosition();
	if (_simomPos.x < this->getPosition().x)
	{
		_direction = eDirection::LEFT;
		if (this->getScale().x > 0)
			return;
		this->setScaleX(this->getScale().x * (-1));
	}
	else
	if (_simomPos.x > this->getPosition().x)
	{
		_direction = eDirection::RIGHT;
		if (this->getScale().x < 0)
			return;
		this->setScaleX(this->getScale().x * (-1));
	}
}

void Dragon::fire()
{
	if (_fireStopWatch == nullptr)
	{
		_fireStopWatch = new StopWatch();
	}

	if (_fireStopWatch->isStopWatch(1500))
	{
		ItemManager::generateWeapon(eItemID::DRAGON_FIRE,this->getPosition() + GVector2(0,10),_direction);
		SAFE_DELETE(_fireStopWatch);
		_fireStopWatch = nullptr;
	}
}

void Dragon::draw(LPD3DXSPRITE spriteHandler, Viewport* viewport)
{
	if (_burning != NULL)
		_burning->draw(spriteHandler, viewport);
	if (this->isInStatus(eStatus::DESTROY) || this->isInStatus(eStatus::BURN))
		return;
	_sprite->render(spriteHandler, viewport);
}

void Dragon::release()
{
	if (_burning != nullptr)
	{
		_burning->release();
		SAFE_DELETE(_burning);
	}
}


float Dragon::checkCollision(BaseObject* otherObject, float dt)
{
	//Lấy collision body của item ra để checkCollision
	auto collisionBody = (CollisionBody*)_listComponent["CollisionBody"];
	eID otherObjectID = otherObject->getId();
	eDirection direction;
	if (otherObjectID != eID::LAND && otherObjectID != eID::SIMON) return 0.0f;
	//if ((otherObjectID == eID::LAND)
	//	&& collisionBody->checkCollision(otherObject, direction, dt, false))
	if (collisionBody->checkCollision(otherObject, direction, dt, false))
	{
		if (otherObjectID == eID::LAND && !_isStand)
		{
			float moveX, moveY;
			if (collisionBody->isCollidingIntersected(otherObject, moveX, moveY, dt))
			{
				collisionBody->updateTargetPosition(otherObject, direction, false, GVector2(moveX, moveY));
			}
			auto gravity = (Gravity*)this->_listComponent["Gravity"];
			gravity->setStatus(eGravityStatus::SHALLOWED);
			_isStand = true;
		}
		else if (otherObjectID == eID::SIMON)
		{
			auto _simon = ((PlayScene*)SceneManager::getInstance()->getCurrentScene())->getSimon();
			_simon->getHitted();
		}
	}

	return 0.0f;
}