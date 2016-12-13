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
	Animation* animation = new Animation(_sprite, 0.12f);
	switch (_itemId)
	{
	case eItemID::MONEYBAGRED:
		animation->addFrameRect(eID::ITEM, "red_money_bag", NULL);
		break;
	case eItemID::MONEYBAGPURPLE:
		animation->addFrameRect(eID::ITEM, "purple_money_bag", NULL);
		break;
	case eItemID::MOENYBAGWHITE:
		animation->addFrameRect(eID::ITEM, "white_money_bag", NULL);
		break;
	case eItemID::MONEYBAG_SPECIAL:
		animation->addFrameRect(eID::ITEM, "red_money_bag", "purple_money_bag", "white_money_bag",NULL);
	default:
		break;
	}
	_animations[_itemId] = animation;
	Item::initCommonComponent();
}

void MoneyBag::update(float deltatime)
{
	Item::update(deltatime);
}

void MoneyBag::draw(LPD3DXSPRITE spriteHandler, Viewport* viewport)
{
	if (this->isInStatus(eStatus::DESTROY))
		return;
	_animations[_itemId]->draw(spriteHandler, viewport);
}

void MoneyBag::release()
{
	Item::release();
	_animations.clear();
}

void MoneyBag::pickedUp()
{
	Item::pickedUp();
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
	case eItemID::MONEYBAG_SPECIAL:
		Score::plusScore(1000);
		break;
	default:
		break;
	}
	this->setStatus(eStatus::DESTROY);
}
