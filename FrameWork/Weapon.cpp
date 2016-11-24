#include "Weapon.h"


Weapon::Weapon(GVector2 startPosition, eItemType type, eDirection dir,eItemID id) :Item(startPosition,type)
{
	_direction = dir;
	_itemId = id;
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
	return Item::checkCollision(otherObject, dt);
}

void Weapon::pickedUp()
{
	ActiveWeapon::setItemID(this->_itemId);
	this->setStatus(eStatus::DESTROY);
}
