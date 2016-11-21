#include "Score.h"

int Score::_score = 0;

Text* Score::_scoreText = nullptr;

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

void Score::draw(LPD3DXSPRITE spriteHandler)
{
	if (_scoreText == nullptr)
		_scoreText  = new Text(L"Arial", "SCORE-", SCORE_POSITION.x, SCORE_POSITION.y, 21);
	_scoreText->setText("SCORE-" + formatScoreString(6, to_string(Score::getScore())));
	_scoreText->draw();
}

