#include "ItemManager.h"

list<Item*> ItemManager::_listItem;

ItemManager::ItemManager()
{
}


ItemManager::~ItemManager()
{
}

void ItemManager::init()
{}

void ItemManager::update(float deltatime)
{}

void ItemManager::draw(LPD3DXSPRITE spriteHandle, Viewport* viewport)
{}

RECT ItemManager::getBounding()
{}

float ItemManager::checkCollision(BaseObject * object, float dt)
{}