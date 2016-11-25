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
	_sprite->setFrameRect(SpriteManager::getInstance()->getSourceRect(eID::ITEM, "holywater_icon"));

	_throwAnimation = new Animation(_sprite, 0.1);
	_throwAnimation->addFrameRect(eID::ITEM, "holywater_throw",NULL);

	_burstAnimation = new Animation(_sprite, 0.12);
	_burstAnimation->addFrameRect(eID::ITEM, "explosion_01", "explosion_02", "explosion_03", "explosion_04", "explosion_05",NULL);
	if (_type == eItemType::PICKED_UP)
		HeartCounter::plusHeart(-1);

	Weapon::initCommonComponent();

	initWeaponComponent();
}


void HolyWater::update(float deltatime)
{

	if (this->isInStatus(eStatus::BURST))
	{
		if (_burstStopWatch == nullptr)
		{
			_burstStopWatch = new StopWatch();
		}

		if (_burstStopWatch->isStopWatch(1000))
		{
			this->removeStatus(eStatus::BURST);
			SAFE_DELETE(_burstStopWatch);
			this->setStatus(eStatus::DESTROY);
		}
	}
	else
		Weapon::update(deltatime);

	if (this->isInStatus(eStatus::BURST))
	{
		_burstAnimation->update(deltatime);
	}
	
}

void HolyWater::draw(LPD3DXSPRITE spriteHandler, Viewport* viewport)
{
	if (this->isInStatus(eStatus::DESTROY)) return;
	if (_type == eItemType::DROP)
	{
		Weapon::draw(spriteHandler, viewport);
	}
	else if (_type == eItemType::PICKED_UP)
	{
		if (this->isInStatus(eStatus::BURST))
		{
			_burstAnimation->draw(spriteHandler, viewport);
		}
		else
			_throwAnimation->draw(spriteHandler, viewport);
	}
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
		//Lấy collision body của item ra để checkCollision
		auto collisionBody = (CollisionBody*)_componentList["CollisionBody"];
		eID otherObjectID = otherObject->getId();
		eDirection direction;
		if (otherObjectID != eID::LAND) return 0.0f;
		//if ((otherObjectID == eID::LAND)
		//	&& collisionBody->checkCollision(otherObject, direction, dt, false))
		if (otherObjectID == eID::LAND && collisionBody->checkCollision(otherObject, direction, dt, false))
		{
			this->stop();
			this->setStatus(eStatus::BURST);
		}

	}
	return 0.0f;
}
