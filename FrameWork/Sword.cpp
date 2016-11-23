#include "Sword.h"


Sword::Sword(GVector2 startPosition, eItemType type, eDirection dir) :Weapon(startPosition,type,dir)
{
	_damage = 3;
}


Sword::~Sword()
{
}

void Sword::init()
{
	_sprite = SpriteManager::getInstance()->getSprite(eID::ITEM);
	_sprite->setFrameRect(SpriteManager::getInstance()->getSourceRect(eID::ITEM, "sword"));
	Weapon::initCommonComponent();
	initWeaponComponent();
}

void Sword::update(float deltatime)
{
	Weapon::update(deltatime);
}

void Sword::draw(LPD3DXSPRITE spriteHandler, Viewport* viewport)
{
	Weapon::draw(spriteHandler, viewport);
}

void Sword::release()
{
	
}

void Sword::initWeaponComponent()
{
	if (_type != eItemType::PICKED_UP) return;

	auto move = (Movement*)this->_componentList["Movement"];
	move->setVelocity(GVector2(100, 0));
	auto gravity = (Gravity*)this->_componentList["Gravity"];
	gravity->setStatus(eGravityStatus::SHALLOWED);
	gravity->setGravity(GVector2(0, 0));
}