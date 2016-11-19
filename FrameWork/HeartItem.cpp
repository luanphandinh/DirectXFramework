#include "HeartItem.h"


HeartItem::HeartItem(GVector2 startPosition) :Item(startPosition,eItemType::DROP)
{

}


HeartItem::~HeartItem()
{
}

void HeartItem::init() 
{
	_sprite = SpriteManager::getInstance()->getSprite(eID::ITEM);
	_sprite->setFrameRect(SpriteManager::getInstance()->getSourceRect(eID::ITEM, "heart_small"));
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

void HeartItem::update(float deltatime) 
{
	Item::update(deltatime);
}

void HeartItem::draw(LPD3DXSPRITE spriteHandler, Viewport* viewport)
{
	Item::draw(spriteHandler, viewport);
}

void HeartItem::release() 
{
	Item::release();
}
