#include "WhipUpgrade.h"



WhipUpgrade::WhipUpgrade(GVector2 startPosition) :Item(startPosition, eItemType::DROP)
{

}


WhipUpgrade::~WhipUpgrade()
{
}


void WhipUpgrade::init()
{
	_sprite = SpriteManager::getInstance()->getSprite(eID::ITEM);
	_sprite->setFrameRect(SpriteManager::getInstance()->getSourceRect(eID::ITEM, "whip_upgrade"));
	Item::initCommonComponent();
}

void WhipUpgrade::update(float deltatime)
{
	Item::update(deltatime);
}

void WhipUpgrade::draw(LPD3DXSPRITE spriteHandler, Viewport* viewport)
{
	Item::draw(spriteHandler, viewport);
}

void WhipUpgrade::release()
{
	Item::release();
}

void WhipUpgrade::pickedUp()
{
}