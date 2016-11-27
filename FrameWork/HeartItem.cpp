#include "HeartItem.h"


HeartItem::HeartItem(GVector2 startPosition,eItemID itemId) :Item(startPosition,eItemType::DROP)
{
	_itemId = itemId;
}


HeartItem::~HeartItem()
{
}

void HeartItem::init() 
{
	/*between 1 and 3: */
	//int type = rand() % 2 + 1;
	//_heartType = (eHeartItemType)type;

	_sprite = SpriteManager::getInstance()->getSprite(eID::ITEM);

	switch (_itemId)
	{
	case eItemID::SMALLHEART:
		_sprite->setFrameRect(SpriteManager::getInstance()->getSourceRect(eID::ITEM, "heart_small"));
		break;
	case eItemID::LARGEHEART:
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
	switch (_itemId)
	{
	case eItemID::SMALLHEART:
		HeartCounter::plusHeart(1);
		break;
	case eItemID::LARGEHEART:
		HeartCounter::plusHeart(5);
		break;
	default:
		break;
	}
	this->setStatus(eStatus::DESTROY);
}