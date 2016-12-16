#include "Chicken.h"


Chicken::Chicken(GVector2 startPosition, eItemID itemId) :Item(startPosition, eItemType::DROP)
{
	_itemId = itemId;
}


Chicken::~Chicken()
{
}


void Chicken::init()
{
	_sprite = SpriteManager::getInstance()->getSprite(eID::ITEM);
	_sprite->setFrameRect(SpriteManager::getInstance()->getSourceRect(eID::ITEM, "chicken"));
	Item::initCommonComponent();
	//_endPosition = this->_startPosition + GVector2(0, 40);
	
}

void Chicken::update(float deltatime)
{
	Item::update(deltatime);
	//this->setPositionY(this->getPositionY() + 100 * deltatime / 1000);
}

void Chicken::draw(LPD3DXSPRITE spriteHandler, Viewport* viewport)
{
	if (this->isInStatus(eStatus::DESTROY))
		return;
	_sprite->render(spriteHandler, viewport);
}

void Chicken::release()
{
	Item::release();
	SAFE_DELETE(_sprite);
}

void Chicken::pickedUp()
{
	Item::pickedUp();
	Score::plusScore(1000);
	this->setStatus(eStatus::DESTROY);
}

float Chicken::checkCollision(BaseObject* otherObject, float dt)
{
	//Lấy collision body của item ra để checkCollision
	auto collisionBody = (CollisionBody*)_componentList["CollisionBody"];
	eID otherObjectID = otherObject->getId();
	eDirection direction;
	if (otherObjectID != eID::LAND && otherObjectID != eID::SIMON) return 0.0f;
	//if ((otherObjectID == eID::LAND)
	//	&& collisionBody->checkCollision(otherObject, direction, dt, false))
	if (collisionBody->checkCollision(otherObject, direction, dt, false))
	{
		if (otherObjectID == eID::LAND && direction == eDirection::TOP)
		{
			this->stop();
		}
		else if (otherObjectID == eID::SIMON)
		{
			this->pickedUp();
		}
	}
	return 0.0f;
}