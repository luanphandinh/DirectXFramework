#include "Candle.h"

Candle::Candle(GVector2 pos,eItemID _dropItemId) :BaseObject(eID::CANDLE)
{
	_sprite = SpriteManager::getInstance()->getSprite(eID::ITEM);
	this->setPosition(pos);
	this->setStatus(eStatus::NORMAL);
	this->_dropItemId = _dropItemId;
	this->setScale(SCALE_FACTOR);
}

Candle::~Candle()
{

}

void Candle::init() 
{
	_animations[NORMAL] = new Animation(_sprite,0.12);
	_animations[NORMAL]->addFrameRect(eID::ITEM, "candle_01", "candle_02", NULL);

	_animations[BURST] = new Animation(_sprite, 0.12);
	_animations[BURST]->addFrameRect(eID::ITEM, "candle_burst", "explosion_03", NULL);
}

void Candle::update(float deltaTime) 
{
	_animations[this->getStatus()]->update(deltaTime);
}

void Candle::draw(LPD3DXSPRITE spriteHandler, Viewport* viewport) 
{
	_animations[NORMAL]->draw(spriteHandler, viewport);
}

void Candle::release() 
{

}