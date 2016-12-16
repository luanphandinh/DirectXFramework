#pragma once
#include"Text.h"
#include"GameTime.h"
#define TIME_POSITION GVector2(180, 0)
class SceneTime
{
public:
	SceneTime();
	~SceneTime();
	static void setTime(int value);
	static int getTime();
	static void stop(bool stop);
	static void draw(LPD3DXSPRITE spriteHandler);
private:
	static int _sceneTime;
	static Text* _sceneTimeText;
	static bool _stop;
};

