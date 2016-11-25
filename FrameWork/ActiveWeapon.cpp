#include "ActiveWeapon.h"


ActiveWeapon::ActiveWeapon()
{
}


ActiveWeapon::~ActiveWeapon()
{
}


Sprite* ActiveWeapon::_spriteWeapon = nullptr;

eItemID ActiveWeapon::_itemID = eItemID::NOITEM;


void ActiveWeapon::setItemID(eItemID id)
{
	if (_itemID != id)
		_itemID = id;
}

eItemID ActiveWeapon::getItemID()
{
	return _itemID;
}

bool ActiveWeapon::isAvailable()
{
	bool _available = false;

	switch (_itemID)
	{
	case eItemID::SWORD:
	case eItemID::BOORMERANG:
	case eItemID::HOLYWATER:
	case eItemID::AXE:
		_available = (HeartCounter::getHeart() > 0) ? true : false;
		break;
	case eItemID::STOPWATCH:
		_available = (HeartCounter::getHeart() >= 5) ? true : false;
		break;
	default:
		break;
	}

	return _available;
}

void ActiveWeapon::drawIcon(LPD3DXSPRITE spriteHandler)
{
	if (_itemID == eItemID::NOITEM) return;
	if (_spriteWeapon == nullptr)
	{
		_spriteWeapon = SpriteManager::getInstance()->getSprite(eID::ITEM);
		_spriteWeapon->setOrigin(GVector2(0.0f, 1.0f));
		_spriteWeapon->setPosition(WEAPON_ICON_DRAW_POS);
		_spriteWeapon->setScale(2.0f);
	}

	switch (_itemID)
	{
	case eItemID::SWORD:
		_spriteWeapon->setFrameRect(SpriteManager::getInstance()->getSourceRect(eID::ITEM, "sword"));
		break;
	case eItemID::AXE:
		_spriteWeapon->setFrameRect(SpriteManager::getInstance()->getSourceRect(eID::ITEM, "axe"));
		break;
	case eItemID::BOORMERANG:
		_spriteWeapon->setFrameRect(SpriteManager::getInstance()->getSourceRect(eID::ITEM, "boomerang_flying"));
		break;
	case eItemID::HOLYWATER:
		_spriteWeapon->setFrameRect(SpriteManager::getInstance()->getSourceRect(eID::ITEM, "holywater_icon"));
		break;
	default:
		break;
	}

	_spriteWeapon->render(spriteHandler);
}