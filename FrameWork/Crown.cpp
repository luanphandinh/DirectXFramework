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
	Item::initCommonComponent();
	_endPosition = this->_startPosition + GVector2(0, 40);
}

void Crown::update(float deltatime)
{
	//Item::update(deltatime);
	_animation->update(deltatime);
	if (this->getPositionY() > _endPosition.y) return;
	this->setPositionY(this->getPositionY() + 100 * deltatime / 1000);
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
	Item::pickedUp();
	Score::plusScore(1000);
	this->setStatus(eStatus::DESTROY);
}

//float Crown::checkCollision(BaseObject* object, float dt)
//{
//	return Item::checkCollision(object, dt);
//}