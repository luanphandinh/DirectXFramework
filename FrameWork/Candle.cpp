#include "Candle.h"
#include"ItemManager.h"
#include"Simon.h"

Candle::Candle(GVector2 pos,eItemID _dropItemId) :BaseObject(eID::CANDLE)
{
	_sprite = SpriteManager::getInstance()->getSprite(eID::ITEM);
	this->setPosition(pos);
	this->setStatus(eStatus::NORMAL);
	this->_dropItemId = _dropItemId;
	this->setScale(SCALE_FACTOR);
	this->setPhysicBodySide(eDirection::ALL);
}

Candle::~Candle()
{

}

void Candle::init() 
{
	_animations[NORMAL] = new Animation(_sprite,0.12);
	_animations[NORMAL]->addFrameRect(eID::ITEM, "candle_01", "candle_02", NULL);

	_animations[BURN] = new Animation(_sprite, 0.12);
	_animations[BURN]->addFrameRect(eID::ITEM, "candle_burst", "explosion_03", NULL);

	auto collisionBody = new CollisionBody(this);
	_componentList.insert(pair<string, IComponent*>("CollisionBody", collisionBody));
}



void Candle::update(float deltaTime) 
{
	eStatus status = this->getStatus();
	if (status == eStatus::BURN)
	{
		if (_burstStopWatch == nullptr)
		{
			_burstStopWatch = new StopWatch();
		}

		if (_burstStopWatch->isStopWatch(200))
		{
			this->removeStatus(eStatus::BURN);
			SAFE_DELETE(_burstStopWatch);
			this->setStatus(eStatus::DESTROY);
			ItemManager::generateItem(_dropItemId, this->getPosition());
		}
	}

	_animations[status]->update(deltaTime);
}

void Candle::draw(LPD3DXSPRITE spriteHandler, Viewport* viewport) 
{
	if (this->isInStatus(eStatus::DESTROY)) return;
	_animations[this->getStatus()]->draw(spriteHandler, viewport);
}

void Candle::release() 
{
	SAFE_DELETE(_burstStopWatch);
	SAFE_DELETE(_sprite);
}

float Candle::checkCollision(BaseObject* otherObject, float dt)
{
	////Lấy collision body của item ra để checkCollision
	//auto collisionBody = (CollisionBody*)_componentList["CollisionBody"];
	//eID otherObjectID = otherObject->getId();
	//eDirection direction;
	//if (otherObjectID != eID::SIMON) return 0.0f;
	////if ((otherObjectID == eID::LAND)
	////	&& collisionBody->checkCollision(otherObject, direction, dt, false))
	//if (otherObjectID == eID::SIMON)
	//{
	//	if (otherObjectID == eID::SIMON && otherObject->isInStatus(eStatus::HITTING)
	//		&& collisionBody->checkCollision(otherObject, direction, dt, false))
	//	{
	//		this->setStatus(eStatus::BURST);
	//	}
	//}
	return 0.0f;
}