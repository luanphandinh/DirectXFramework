#pragma once
#include"Text.h"
#define LIFE_POSITION GVector2(320,46)
class LifeCounter
{
public:
	LifeCounter();
	~LifeCounter();

	static void plusLife(int value);
	static void setLife(int value);
	static int getLife();
	static void draw(LPD3DXSPRITE spriteHandler);
private:
	static int _life;
	static Text* _lifeText;
};

