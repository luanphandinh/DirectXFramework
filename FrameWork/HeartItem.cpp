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
	Item::initCommonComponent();
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
