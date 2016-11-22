#pragma once
#include"Text.h"
#include"SpriteManager.h"
#define HEARTTEXT_POSITION GVector2(320,23)
class Heart
{
public:
	Heart();
	~Heart();

	static void plusHeart(int value);
	static int getHeart();
	static void draw(LPD3DXSPRITE spriteHandler);
private:
	static int _heart;
	static Text* _heartText;
	static Sprite* _heartSprite;
};


