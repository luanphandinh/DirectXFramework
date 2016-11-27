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
		break;
	case LARGEHEART:
		item = new HeartItem(pos, id);
		break;
	case MONEYBAGPURPLE:
		break;
	case MONEYBAGRED:
		break;
	case MOENYBAGWHITE:
		break;
	case WHIPUPGRADE:
		break;
	case SWORD:
		break;
	case AXE:
		break;
	case BOORMERANG:
		item = new Boomerang(pos, type);
		break;
	case HOLYWATER:
		break;
	case STOPWATCH:
		break;
	default:
		break;
	}
	if (item != nullptr)
		insertItem(item);
}