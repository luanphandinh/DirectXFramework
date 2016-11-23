#pragma once
#include"SpriteManager.h"
#include"ItemManager.h"

#define WEAPON_ICON_DRAW_POS GVector2(260,32)
class ActiveWeapon
{
public:
	ActiveWeapon();
	~ActiveWeapon();
	static void setItemID(eItemID id);
	static eItemID getItemID();
	static void drawIcon(LPD3DXSPRITE spriteHandler);
private:
	static Sprite* _spriteWeapon;
	static eItemID _itemID;
};

