#include "Heart.h"


Heart::Heart()
{
}


Heart::~Heart()
{
}

int Heart::_heart = 0;

Text* Heart::_heartText = nullptr;

Sprite* Heart::_heartSprite = nullptr;

void Heart::plusHeart(int value)
{
	_heart += value;
}

int Heart::getHeart()
{
	return _heart;
}

void Heart::draw(LPD3DXSPRITE spriteHandler)
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
	
	_heartText->setText(formatScoreString(2, to_string(Heart::getHeart())));
	_heartSprite->render(spriteHandler);
}
