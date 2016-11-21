#pragma once
#include"Text.h"
#define SCORE_POSITION GVector2(20, 0)
class Score
{
public:
	Score();
	~Score();

	static void plusScore(int value);
	static int getScore();
	static void draw(LPD3DXSPRITE spriteHandler);
private:
	static int _score;
	static Text* _scoreText;
};

