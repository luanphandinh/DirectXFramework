#include "ThrowingAxe.h"


ThrowingAxe::ThrowingAxe(GVector2 startPosition, eItemType type, eDirection dir) : Weapon(startPosition, type, dir, eItemID::AXE)
{
}


ThrowingAxe::~ThrowingAxe()
{
}

void ThrowingAxe::init()
{
	_sprite = SpriteManager::getInstance()->getSprite(eID::ITEM);
	_sprite->setFrameRect(SpriteManager::getInstance()->getSourceRect(eID::ITEM, "axe"));

	if (_type == eItemType::PICKED_UP)
		HeartCounter::plusHeart(-1);

	Weapon::initCommonComponent();

	if ((_direction & eDirection::RIGHT) == eDirection::RIGHT)
		_sprite->setScaleX(this->getScale().x * (-1));

	initWeaponComponent();
}

GVector2 ThrowingAxe::initVeloc(GVector2 speed)
{
	GVector2 result;
	if (_direction != eDirection::NONE)
	{
		if ((_direction & eDirection::LEFT) == eDirection::LEFT)
		{
			result.x = -speed.x;
		}
		else if ((_direction & eDirection::RIGHT) == eDirection::RIGHT)
		{
			result.x = speed.x;
		}
		else
		{
			result.x = 0;
		}
	}

	result.y = speed.y;
	return result;
}


void ThrowingAxe::update(float deltatime)
{
	Weapon::update(deltatime);
	if (abs(this->getPositionX() - this->_startPosition.x) > THROWING_AXE_DISTANCE.x)
		this->setStatus(eStatus::DESTROY);
}

void ThrowingAxe::draw(LPD3DXSPRITE spriteHandler, Viewport* viewport)
{
	Weapon::draw(spriteHandler, viewport);
}

void ThrowingAxe::release()
{

}

void ThrowingAxe::initWeaponComponent()
{
	if (_type != eItemType::PICKED_UP) return;

	GVector2 veloc = this->initVeloc(THROWING_AXE_DISTANCE);

	auto move = (Movement*)this->_componentList["Movement"];
	move->setVelocity(veloc);

	auto gravity = (Gravity*)this->_componentList["Gravity"];
	gravity->setStatus(eGravityStatus::FALLING_DOWN);
	gravity->setGravity(GVector2(0, -800));

	RotateMovement* rotateMovement = new RotateMovement(_sprite);
	_componentList["RotateMovement"] = rotateMovement;
}

float ThrowingAxe::checkCollision(BaseObject* otherObject, float dt)
{
	if (_type == eItemType::DROP)
	{
		Weapon::checkCollision(otherObject, dt);
	}
	return 0.0f;
}