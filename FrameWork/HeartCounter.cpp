#include "HeartCounter.h"


HeartCounter::HeartCounter()
{
}


HeartCounter::~HeartCounter()
{
}

int HeartCounter::_heart = 0;

Text* HeartCounter::_heartText = nullptr;

Sprite* HeartCounter::_heartSprite = nullptr;

void HeartCounter::plusHeart(int value)
{
	_heart += value;
}

int HeartCounter::getHeart()
{
	return _heart;
}

void HeartCounter::draw(LPD3DXSPRITE spriteHandler)
{
	if (_heartSprite == nullptr)
	{
		_heartSprite = SpriteManager::getInstance()->getSprite(eID::ITEM);
		_heartSprite->setFrameRect(SpriteManager::getInstance()->getSourceRect(eID::ITEM, "heart_icon"));
		_heartSprite->setOrigin(GVector2(0.0f,1.0f));
		_heartSprite->setPosition(HEARTTEXT_POSITION);
		_heartSprite->setScale(2.0f);
	}

	if (_heartText == nullptr)
		_heartText = new Text(L"Arial", "", HEARTTEXT_POSITION.x + _heartSprite->getFrameWidth()
		, HEARTTEXT_POSITION.y, 21);
	
	_heartText->draw();
	
	_heartText->setText("-" + formatScoreString(2, to_string(HeartCounter::getHeart())));
	_heartSprite->render(spriteHandler);
}
