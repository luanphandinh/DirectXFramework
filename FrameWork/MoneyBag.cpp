#include "MoneyBag.h"


MoneyBag::MoneyBag(GVector2 startPosition) :Item(startPosition, eItemType::DROP)
{

}


MoneyBag::~MoneyBag()
{
}


void MoneyBag::init()
{
	/*between 1 and 3: */
	int type = rand() % 3 + 1;
	_moneyBagType = (eMoneyBagItemType)type;

	_sprite = SpriteManager::getInstance()->getSprite(eID::ITEM);

	switch (_moneyBagType)
	{
	case eMoneyBagItemType::RED:
		_sprite->setFrameRect(SpriteManager::getInstance()->getSourceRect(eID::ITEM, "red_money_bag"));
		break;
	case eMoneyBagItemType::PURPLE:
		_sprite->setFrameRect(SpriteManager::getInstance()->getSourceRect(eID::ITEM, "purple_money_bag"));
		break;
	case eMoneyBagItemType::WHITE:
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
	switch (_moneyBagType)
	{
	case eMoneyBagItemType::RED:
		Score::plusScore(100);
		break;
	case eMoneyBagItemType::PURPLE:
		Score::plusScore(300);
		break;
	case eMoneyBagItemType::WHITE:
		Score::plusScore(700);
		break;
	default:
		break;
	}
	this->setStatus(eStatus::DESTROY);
}
