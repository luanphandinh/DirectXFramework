#include "HolyWater.h"


HolyWater::HolyWater(GVector2 startPosition, eItemType type, eDirection dir) : Weapon(startPosition, type, dir, eItemID::HOLYWATER)
{
}


HolyWater::~HolyWater()
{
}

void HolyWater::init()
{
	_sprite = SpriteManager::getInstance()->getSprite(eID::ITEM);
	_sprite->setFrameRect(SpriteManager::getInstance()->getSourceRect(eID::ITEM, "holywater_throw"));

	if (_type == eItemType::PICKED_UP)
		HeartCounter::plusHeart(-1);

	Weapon::initCommonComponent();

	initWeaponComponent();
}


void HolyWater::update(float deltatime)
{
	Weapon::update(deltatime);
	//if (abs(this->getPositionX() - this->_startPosition.x) > THROWING_AXE_DISTANCE.x)
	//	this->setStatus(eStatus::DESTROY);
}

void HolyWater::draw(LPD3DXSPRITE spriteHandler, Viewport* viewport)
{
	Weapon::draw(spriteHandler, viewport);
}

void HolyWater::release()
{

}

void HolyWater::initWeaponComponent()
{
	if (_type != eItemType::PICKED_UP) return;

	GVector2 veloc = this->initVeloc(HOLYWATER_SPEED);

	auto move = (Movement*)this->_componentList["Movement"];
	move->setVelocity(veloc);

	auto gravity = (Gravity*)this->_componentList["Gravity"];
	gravity->setStatus(eGravityStatus::FALLING_DOWN);
	gravity->setGravity(GVector2(0, -600));
}

float HolyWater::checkCollision(BaseObject* otherObject, float dt)
{
	if (_type == eItemType::DROP)
	{
		Weapon::checkCollision(otherObject, dt);
	}
	else
	if (_type == eItemType::PICKED_UP)
	{

	}
	return 0.0f;
}
