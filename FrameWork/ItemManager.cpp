#include "ItemManager.h"

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