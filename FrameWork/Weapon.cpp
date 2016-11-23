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

void Weapon::initCommonComponent()
{
	switch (_type)
	{
	case eItemType::DROP:
		Item::initCommonComponent();
		break;
	case eItemType::PICKED_UP:
		this->initWeaponComponent();
		break;
	default:
		break;
	}
}
