#include "Item.h"


Item::Item(GVector2 startPosition, eItemType type) :BaseObject(eID::ITEM)
{
	_startPosition = startPosition;
	_type = type;
}


Item::~Item()
{
}


void Item::init()
{
}

void Item::initCommonComponent()
{
	this->setPosition(_startPosition);
	this->setScale(SCALE_FACTOR);

	GVector2 veloc = this->initVeloc(NORMAL_ITEM_SPEED);

	auto movement = new Movement(GVector2(0, 0), GVector2(0, 0), _sprite);
	_componentList.insert(pair<string, IComponent*>("Movement", movement));

	Gravity* gravity = new Gravity(GVector2(0, -NORMAL_ITEM_SPEED), movement);
	_componentList["Gravity"] = gravity;


	auto collisionBody = new CollisionBody(this);
	_componentList.insert(pair<string, IComponent*>("CollisionBody", collisionBody));
}

GVector2 Item::initVeloc(float speed)
{
	return GVector2(0, -speed);
}

void Item::update(float deltatime)
{
	for (auto it = _componentList.begin(); it != _componentList.end(); it++)
	{
		it->second->update(deltatime);
	}
}

void Item::draw(LPD3DXSPRITE spriteHandler, Viewport* viewport)
{
	if (this->getStatus() == eStatus::DESTROY)
		return;
	_sprite->render(spriteHandler,viewport);
}

void Item::release()
{
	for each (auto item in _componentList)
	{
		SAFE_DELETE(item.second);
	}
	_componentList.clear();
}

GVector2 Item::getVelocity()
{
	auto move = (Movement*)this->_componentList.find("Movement")->second;
	return move->getVelocity();
}

eItemType Item::getItemType()
{
	return _type;
}

void Item::onCollisionBegin(CollisionEventArg* collision_arg)
{

}

float Item::checkCollision(BaseObject* otherObject, float dt)
{
	//Lấy collision body của item ra để checkCollision
	auto collisionBody = (CollisionBody*)_componentList["CollisionBody"];
	eID otherObjectID = otherObject->getId();
	eDirection direction;

	if (otherObjectID == eID::LAND && collisionBody->checkCollision(otherObject, direction, dt, false))
	{
		float moveX, moveY;
		if (collisionBody->isCollidingIntersected(otherObject, moveX, moveY, dt))
		{
			collisionBody->updateTargetPosition(otherObject, direction, false, GVector2(moveX, moveY));
		}
		auto gravity = (Gravity*)this->_componentList["Gravity"];
		gravity->setStatus(eGravityStatus::SHALLOWED);
		auto movement = (Movement*)this->_componentList["Movement"];
		movement->setVelocity(GVector2(0, 0));
	}
	return 0.0f;
}