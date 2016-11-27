#include "MoneyBag.h"


MoneyBag::MoneyBag(GVector2 startPosition,eItemID itemId) :Item(startPosition, eItemType::DROP)
{
	_itemId = itemId;
}


MoneyBag::~MoneyBag()
{
}


void MoneyBag::init()
{
	/*between 1 and 3: */
	//int type = rand() % 3 + 1;
	//_moneyBagType = (eMoneyBagItemType)type;

	_sprite = SpriteManager::getInstance()->getSprite(eID::ITEM);

	switch (_itemId)
	{
	case eItemID::MONEYBAGRED:
		_sprite->setFrameRect(SpriteManager::getInstance()->getSourceRect(eID::ITEM, "red_money_bag"));
		break;
	case eItemID::MONEYBAGPURPLE:
		_sprite->setFrameRect(SpriteManager::getInstance()->getSourceRect(eID::ITEM, "purple_money_bag"));
		break;
	case eItemID::MOENYBAGWHITE:
		_sprite->setFrameRect(SpriteManager::getInstance()->getSourceRect(eID::ITEM, "white_money_bag"));
		break;
	default:
		break;
	}
	Item::initCommonComponent();
}

void MoneyBag::update(float deltatime)
{
	Item::update(deltatime);
}

void MoneyBag::draw(LPD3DXSPRITE spriteHandler, Viewport* viewport)
{
	Item::draw(spriteHandler, viewport);
}

void MoneyBag::release()
{
	Item::release();
}

void MoneyBag::pickedUp()
{
	switch (_itemId)
	{
	case eItemID::MONEYBAGRED:
		Score::plusScore(100);
		break;
	case eItemID::MONEYBAGPURPLE:
		Score::plusScore(300);
		break;
	case eItemID::MOENYBAGWHITE:
		Score::plusScore(700);
		break;
	default:
		break;
	}
	this->setStatus(eStatus::DESTROY);
}
