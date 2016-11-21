#include "GameStatusBoard.h"


GameStatusBoard* GameStatusBoard::_instance = nullptr;

GameStatusBoard::GameStatusBoard()
{
}


GameStatusBoard::~GameStatusBoard()
{
}

GameStatusBoard* GameStatusBoard::getInstance()
{
	if (_instance == nullptr)
		_instance = new GameStatusBoard();
	return _instance;
}

void GameStatusBoard::init()
{
	_simonLifeUI = new LifeUI(SIMONLIFEUI_POSITION,"PLAYER","red_life_icon", 3);
	_simonLifeUI->setHPNumber(14);
	_enemyLifeUI = new LifeUI(ENEMYLIFEUI_POSITION, "ENEMY", "yellow_life_icon", 1);
	_enemyLifeUI->setHPNumber(3);
	_scoreText = new Text(L"Arial", "SCORE-", SCORE_POSITION.x, SCORE_POSITION.y, 21);
	_timeText = new Text(L"Arial", "TIME", TIME_POSITION.x, TIME_POSITION.y, 21);
	_timeScene = 300;
}

void GameStatusBoard::setSimonLifeUI(LifeUI* _lifeUI)
{
	if (_simonLifeUI != _lifeUI)
		_simonLifeUI = _lifeUI;
}

LifeUI* GameStatusBoard::getSimonLifeUI()
{
	return _simonLifeUI;
}

void GameStatusBoard::setEnemyLifeUI(LifeUI* _lifeUI)
{
	if (_enemyLifeUI != _lifeUI)
		_enemyLifeUI = _lifeUI;
}

LifeUI* GameStatusBoard::getEnemyLifeUI(LifeUI* _lifeUI)
{
	return _enemyLifeUI;
}

void GameStatusBoard::draw(LPD3DXSPRITE spriteHandle)
{
	_simonLifeUI->draw(spriteHandle, nullptr);
	_enemyLifeUI->draw(spriteHandle, nullptr);
	_scoreText->setText("SCORE-" + formatScoreString(6,to_string(Score::getScore())));
	_scoreText->draw();
	_timeText->setText("TIME  " + formatScoreString(4,to_string(_timeScene - (int)(GameTime::getInstance()->getTotalGameTime() / 1000))));
	_timeText->draw();
}


void GameStatusBoard::setTimeScene(int time)
{
	if (this->_timeScene != time)
		_timeScene = time;
}

int GameStatusBoard::getTimeScene()
{
	return _timeScene;
}