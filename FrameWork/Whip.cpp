#include "Whip.h"
#include"Simon.h"
#include"PlayScene.h"
#include"BaseEnemy.h"

Whip::Whip(int level) :BaseObject(eID::WHIP)
{
	this->_level = level;
	_damage = 1;
}


Whip::~Whip()
{
}

void Whip::init()
{
	this->_sprite = SpriteManager::getInstance()->getSprite(eID::WHIP);
	_sprite->setScale(2.0f);
	_animations[1] = new Animation(_sprite, 0.12f);
	_animations[1]->addFrameRect(eID::WHIP, "whip_01", "whip_02", "whip_03", "whip_03", NULL);
	auto collisionBody = new CollisionBody(this);
	_componentList.insert(pair<string, IComponent*>("CollisionBody", collisionBody));
}

void Whip::update(float deltatime) 
{
	if (_simon->isInStatus(eStatus::HITTING))
		_animations[_level]->update(deltatime);
}

void Whip::draw(LPD3DXSPRITE spriteHandler, Viewport* viewport)
{
	if (_simon->isInStatus(eStatus::HITTING))
	{
		_sprite->setScale(_simon->getSprite()->getScale());
		if (_simon->isInStatus(eStatus::SITTING))
			this->setPosition(_simon->getPosition() - GVector2(0,7));
		else
			this->setPosition(_simon->getPosition());
		_animations[_level]->draw(spriteHandler, viewport);
	}
}

void Whip::release() 
{
}

void Whip::restart()
{
	_animations[_level]->restart();
	_listColliding.clear();
}

void Whip::setTracker(BaseObject* simon)
{
	this->_simon = simon;
}

bool Whip::isHitting()
{
	return (_animations[_level]->getIndex() == 2);
}

float Whip::checkCollision(BaseObject* otherObject, float dt)
{
	//Lấy collision body của item ra để checkCollision
	auto collisionBody = (CollisionBody*)_componentList["CollisionBody"];
	eID otherObjectID = otherObject->getId();
	eDirection direction;
	if (otherObjectID == eID::SIMON || otherObjectID == eID::LAND
		||otherObjectID == eID::STAIR) return 0.0f;
	//if ((otherObjectID == eID::LAND)
	//	&& collisionBody->checkCollision(otherObject, direction, dt, false))
	if (collisionBody->checkCollision(otherObject, direction, dt, false) && this->isHitting())
	{
		auto object = _listColliding.find(otherObject);
		switch (otherObjectID)
		{
		case BRICK:
		case CANDLE:
			otherObject->setStatus(eStatus::BURST);
			break;
		case SPEARKNIGHT:
			if (object == _listColliding.end() || object._Ptr == nullptr)
			{
				((BaseEnemy*)otherObject)->dropHitpoint(this->_damage);
				_listColliding[otherObject] = true;
			}
			break;
		case BAT:
			otherObject->setStatus(eStatus::BURST);
			break;
		case MEDUSAHEAD:
			break;
		default:
			break;
		}
	}
	return 0.0f;
}
