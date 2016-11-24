#include "HeartItem.h"


HeartItem::HeartItem(GVector2 startPosition) :Item(startPosition,eItemType::DROP)
{

}


HeartItem::~HeartItem()
{
}

void HeartItem::init() 
{
	/*between 1 and 3: */
	int type = rand() % 2 + 1;
	_heartType = (eHeartItemType)type;

	_sprite = SpriteManager::getInstance()->getSprite(eID::ITEM);

	switch (_heartType)
	{
	case eHeartItemType::SMALL:
		_sprite->setFrameRect(SpriteManager::getInstance()->getSourceRect(eID::ITEM, "heart_small"));
		break;
	case eHeartItemType::LARGE:
		_sprite->setFrameRect(SpriteManager::getInstance()->getSourceRect(eID::ITEM, "heart_large"));
		break;
	default:
		break;
	}
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

void HeartItem::pickedUp()
{
	switch (_heartType)
	{
	case eHeartItemType::SMALL:
		HeartCounter::plusHeart(1);
		break;
	case eHeartItemType::LARGE:
		HeartCounter::plusHeart(3);
		break;
	default:
		break;
	}
	this->setStatus(eStatus::DESTROY);
}