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

	if (_type == eItemType::PICKED_UP)
		HeartCounter::plusHeart(-1);

	Weapon::initCommonComponent();

	if ((_direction & eDirection::RIGHT) == eDirection::RIGHT)
		_sprite->setScaleX(this->getScale().x * (-1));

	initWeaponComponent();
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
	else if (_type == eItemType::PICKED_UP)
	{
		/*auto collisionBody = (CollisionBody*)_componentList["CollisionBody"];
		eID otherObjectID = otherObject->getId();
		eDirection direction;
		if (otherObjectID == eID::SPEARKNIGHT)
		{
			if (collisionBody->checkCollision(otherObject, direction, dt, false))
			{
				this->setStatus(eStatus::DESTROY);
			}
		}
		else
		if (otherObjectID == eID::SIMON)
		{
			if (collisionBody->checkCollision(otherObject, direction, dt, false))
			{
				HeartCounter::plusHeart(10);
			}
		}*/
	}
	return 0.0f;
}