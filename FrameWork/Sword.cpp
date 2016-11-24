#include "Sword.h"


Sword::Sword(GVector2 startPosition, eItemType type, eDirection dir) :Weapon(startPosition,type,dir,eItemID::SWORD)
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

	if ((_direction & eDirection::RIGHT) == eDirection::RIGHT)
		_sprite->setScaleX(this->getScale().x * (-1));

	initWeaponComponent();
}

GVector2 Sword::initVeloc(float speed)
{
	GVector2 result;
	if (_direction != eDirection::NONE)
	{
		if ((_direction & eDirection::LEFT) == eDirection::LEFT)
		{
			result.x = -speed;
		}
		else if ((_direction & eDirection::RIGHT) == eDirection::RIGHT)
		{
			result.x = speed;
		}
		else
		{
			result.x = 0;
		}
	}

	result.y = 0;
	return result;
}


void Sword::update(float deltatime)
{
	Weapon::update(deltatime);
	if (abs(this->getPositionX() - this->_startPosition.x) > SWORD_DISTANCE.x)
		this->setStatus(eStatus::DESTROY);
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

	GVector2 veloc = this->initVeloc(SWORD_SPEED);

	auto move = (Movement*)this->_componentList["Movement"];
	move->setVelocity(veloc);

	auto gravity = (Gravity*)this->_componentList["Gravity"];
	gravity->setStatus(eGravityStatus::SHALLOWED);
	gravity->setGravity(GVector2(0, 0));
}

float Sword::checkCollision(BaseObject* otherObject, float dt)
{
	if (_type == eItemType::DROP)
	{
		Weapon::checkCollision(otherObject, dt);
	}
	return 0.0f;
}