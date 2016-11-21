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

void ActiveWeapon::drawIcon(LPD3DXSPRITE spriteHandler)
{
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
	default:
		break;
	}

	_spriteWeapon->render(spriteHandler);
}