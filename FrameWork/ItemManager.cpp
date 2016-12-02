#include "ItemManager.h"
#include"Weapon.h"
#include"ThrowingAxe.h"
#include"Boomerang.h"
#include"MoneyBag.h"
#include"HeartItem.h"
#include"WhipUpgrade.h"
#include"HolyWater.h"
#include"Sword.h"

list<Item*> ItemManager::_listItem;

ItemManager::ItemManager() :BaseObject(eID::ITEM)
{}

ItemManager::~ItemManager()
{
}


void ItemManager::init()
{}

void ItemManager::insertItem(Item* item)
{
	if (item == nullptr)
		throw new exception("item cannot null");
	item->init();
	_listItem.push_back(item);
}

void ItemManager::update(float deltatime)
{
	for (Item* item : _listItem)
	{
		if (item == nullptr)
		{
			_listItem.remove(item);
			break;
		}
		else
		{
			item->update(deltatime);
			if (item->getStatus() == eStatus::DESTROY)
			{
				item->release();
				_listItem.remove(item);
				delete item;
				item = nullptr;
				break;
			}
		}
	}
}

void ItemManager::draw(LPD3DXSPRITE spriteHandle, Viewport* viewport)
{
	for (Item* item : _listItem)
	{
		if (item == nullptr)
		{
			_listItem.remove(item);
			break;
		}
		else
		{
			//item->getSprite()->drawBounding(true);
			item->draw(spriteHandle,viewport);
		}
	}
}

void ItemManager::release()
{
	for (Item* item : _listItem)
	{
		if (item == nullptr)
		{
			_listItem.remove(item);
			break;
		}
		else
		{
			_listItem.remove(item);
			break;
			item->release();
			delete item;
			item = nullptr;
		}
	}
}

RECT ItemManager::getBounding()
{
	return RECT();
}

float ItemManager::checkCollision(BaseObject * otherObject, float dt)
{
	eID otherObjectID = otherObject->getId();
	//Chỉ kiểm tra va chạm cho simon và land
	if (otherObjectID != eID::SIMON && otherObjectID != eID::LAND)
		return 0.0f;

	for (Item* item : _listItem)
	{
		if (item == nullptr)
		{
			_listItem.remove(item);
			break;
		}
		else
		{
			item->checkCollision(otherObject, dt);
		}
	}

	return 0.0f;
}

void ItemManager::generateItem(eItemID id, GVector2 pos, eItemType type)
{
	Item* item = nullptr;
	switch (id)
	{
	case SMALLHEART:
	case LARGEHEART:
		item = new HeartItem(pos, id);
		break;
	case MONEYBAGPURPLE:
	case MONEYBAGRED:
	case MOENYBAGWHITE:
		item = new MoneyBag(pos, id);
		break;
	case WHIPUPGRADE:
		item = new WhipUpgrade(pos);
		break;
	case SWORD:
		item = new Sword(pos, type);
		break;
	case AXE:
		item = new ThrowingAxe(pos, type);
		break;
	case BOORMERANG:
		item = new Boomerang(pos, type);
		break;
	case HOLYWATER:
		item = new HolyWater(pos, type);
		break;
	case STOPWATCH:
		break;
	default:
		break;
	}
	if (item != nullptr)
		insertItem(item);
}

void ItemManager::generateWeapon(eItemID id, GVector2 startPos, eDirection dir, eItemType type)
{
	Item* item = nullptr;
	switch (id)
	{
	case NOITEM:
		break;
	case eItemID::SWORD:
		item = new Sword(startPos, eItemType::PICKED_UP, dir);
		break;
	case eItemID::AXE:
		item = new ThrowingAxe(startPos, eItemType::PICKED_UP, dir);
		break;
	case eItemID::BOORMERANG:
		item = new Boomerang(startPos, eItemType::PICKED_UP, dir);
		break;
	case eItemID::HOLYWATER:
		item = new HolyWater(startPos, eItemType::PICKED_UP, dir);
		break;
	default:
		break;
	}
	if (item != nullptr)
		insertItem(item);
}