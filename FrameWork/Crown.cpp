#include "Crown.h"


Crown::Crown(GVector2 startPosition, eItemID itemId) :Item(startPosition, eItemType::DROP)
{
	_itemId = itemId;
}


Crown::~Crown()
{
}


void Crown::init()
{
	_sprite = SpriteManager::getInstance()->getSprite(eID::ITEM);
	_animation = new Animation(_sprite, 0.08f);
	_animation->addFrameRect(eID::ITEM, "crown_01", "crown_02", NULL);
	//Item::initCommonComponent();
}

void Crown::update(float deltatime)
{
	//Item::update(deltatime);
	_animation->update(deltatime);
}

void Crown::draw(LPD3DXSPRITE spriteHandler, Viewport* viewport)
{
	if (this->isInStatus(eStatus::DESTROY))
		return;
	_animation->draw(spriteHandler, viewport);
}

void Crown::release()
{
	Item::release();
	SAFE_DELETE(_animation);
	_animation = nullptr;
}

void Crown::pickedUp()
{
	//Score::plusScore(1000);
	//this->setStatus(eStatus::DESTROY);
}

float Crown::checkCollision(BaseObject* object, float dt)
{
	return 0.0f;
}