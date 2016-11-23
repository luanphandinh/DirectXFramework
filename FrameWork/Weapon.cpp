#include "Weapon.h"


Weapon::Weapon(GVector2 startPosition, eItemType type, eDirection dir) :Item(startPosition,type)
{
	_direction = dir;
}


Weapon::~Weapon()
{
}

void Weapon::init()
{}

void Weapon::update(float deltatime)
{
	Item::update(deltatime);
}

void Weapon::draw(LPD3DXSPRITE spriteHandler, Viewport* viewport)
{
	Item::draw(spriteHandler,viewport);
}

void Weapon::release()
{
	Item::release();
}

GVector2 Weapon::initVeloc(float speed)
{
	return Item::initVeloc(speed);
}

float Weapon::checkCollision(BaseObject* otherObject, float dt)
{
	//Lấy collision body của item ra để checkCollision
	auto collisionBody = (CollisionBody*)_componentList["CollisionBody"];
	eID otherObjectID = otherObject->getId();
	eDirection direction;
	if (otherObjectID != eID::LAND && otherObjectID != eID::SIMON) return 0.0f;
	//if ((otherObjectID == eID::LAND)
	//	&& collisionBody->checkCollision(otherObject, direction, dt, false))
	if (otherObjectID == eID::LAND && collisionBody->checkCollision(otherObject, direction, dt, false))
	{
		this->stop();
	}
	return 0.0f;
}
