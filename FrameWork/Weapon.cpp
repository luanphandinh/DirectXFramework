#include "Weapon.h"
#include"BaseEnemy.h"

Weapon::Weapon(GVector2 startPosition, eItemType type, eDirection dir,eItemID id) :Item(startPosition,type)
{
	_direction = dir;
	_itemId = id;
	this->setPhysicBodySide(eDirection::ALL);
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

GVector2 Weapon::initVeloc(GVector2 speed)
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

float Weapon::checkCollision(BaseObject* otherObject, float dt)
{
	if (this->_type == eItemType::DROP)
		Item::checkCollision(otherObject, dt);
	else this->checkCollisionWeapon(otherObject, dt);
	return 0.0f;
}

float  Weapon::checkCollisionWeapon(BaseObject* otherObject, float dt)
{
	//Lấy collision body của item ra để checkCollision
	auto collisionBody = (CollisionBody*)_componentList["CollisionBody"];
	eID otherObjectID = otherObject->getId();
	eDirection direction;
	if (otherObjectID == eID::SIMON || otherObjectID == eID::LAND
		|| otherObjectID == eID::STAIR) return 0.0f;
	//if ((otherObjectID == eID::LAND)
	//	&& collisionBody->checkCollision(otherObject, direction, dt, false))
	if (collisionBody->checkCollision(otherObject, direction, dt, false))
	{
		auto object = _listColliding.find(otherObject);
		switch (otherObjectID)
		{
		case CANDLE:
			otherObject->setStatus(eStatus::BURN);
			break;
		case DRAGON:
		case SPEARKNIGHT:
			if (object == _listColliding.end() || object._Ptr == nullptr)
			{
				((BaseEnemy*)otherObject)->dropHitpoint(this->_damage);
				_listColliding[otherObject] = true;
			}
			break;
		case BAT:
		case GHOST:
			if (object == _listColliding.end() || object._Ptr == nullptr)
			{
				((BaseEnemy*)otherObject)->dropHitpoint(this->_damage);
				_listColliding[otherObject] = true;
			}
			break;
			break;
		case MEDUSAHEAD:
			break;
		default:
			break;
		}
	}
	return 0.0f;
}

void Weapon::pickedUp()
{
	ActiveWeapon::setItemID(this->_itemId);
	this->setStatus(eStatus::DESTROY);
}
