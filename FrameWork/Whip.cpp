#include "Whip.h"
#include"Simon.h"
#include"PlayScene.h"

Whip::Whip(int level) :BaseObject(eID::WHIP)
{
	this->_level = level;
}


Whip::~Whip()
{
}

void Whip::init()
{
	this->_sprite = SpriteManager::getInstance()->getSprite(eID::WHIP);
	_sprite->setScale(2.0f);
	_animation = new Animation(_sprite, 0.12f);
	_animation->addFrameRect(eID::WHIP, "whip_01", "whip_02", "whip_03", NULL);
	auto collisionBody = new CollisionBody(this);
	_componentList.insert(pair<string, IComponent*>("CollisionBody", collisionBody));
}

void Whip::update(float deltatime) 
{
	if (_simon->isInStatus(eStatus::HITTING))
		_animation->update(deltatime);
	else _animation->restart();

}

void Whip::draw(LPD3DXSPRITE spriteHandler, Viewport* viewport)
{
	if (_simon->isInStatus(eStatus::HITTING))
	{
		_sprite->setScale(_simon->getSprite()->getScale());
		this->setPosition(_simon->getPosition());
		_animation->draw(spriteHandler, viewport);
	}
}

void Whip::release() 
{
}

void Whip::restart()
{
	_animation->restart(0);
}

void Whip::setTracker(BaseObject* simon)
{
	this->_simon = simon;
}



float Whip::checkCollision(BaseObject* otherObject, float dt)
{
	//Lấy collision body của item ra để checkCollision
	auto collisionBody = (CollisionBody*)_componentList["CollisionBody"];
	eID otherObjectID = otherObject->getId();
	eDirection direction;
	if (otherObjectID != eID::CANDLE) return 0.0f;
	//if ((otherObjectID == eID::LAND)
	//	&& collisionBody->checkCollision(otherObject, direction, dt, false))
	if (otherObjectID == eID::CANDLE)
	{
		if (collisionBody->checkCollision(otherObject, direction, dt, false))
		{
			otherObject->setStatus(eStatus::BURST);
		}
	}
	return 0.0f;
}