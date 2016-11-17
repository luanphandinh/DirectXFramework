#include "Score.h"

int Score::_score = 0;

Score::Score()
{
}


Score::~Score()
{
}

void Score::plusScore(int value)
{
	_score += value;
}

int Score::getScore()
{
	return _score;
}

