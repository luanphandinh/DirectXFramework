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
}