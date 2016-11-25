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
	SceneTime::setTime(300);
	//tạo surface để vẽ background cho bảng
	DeviceManager::getInstance()->getDevice()->CreateOffscreenPlainSurface(
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		D3DFMT_X8R8G8B8,
		D3DPOOL_DEFAULT,
		&_surface,
		NULL
	);
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
	drawBlankBackground(spriteHandle);

	_simonLifeUI->draw(spriteHandle, nullptr);
	_enemyLifeUI->draw(spriteHandle, nullptr);

	Score::draw(spriteHandle);
	HeartCounter::draw(spriteHandle);
	SceneTime::draw(spriteHandle);
	LifeCounter::draw(spriteHandle);
	ActiveWeapon::drawIcon(spriteHandle);
}

void GameStatusBoard::drawBlankBackground(LPD3DXSPRITE spriteHandler) 
{
	RECT r;
	r.top = 0;
	r.left = 0;
	r.bottom = 64;
	r.right = WINDOW_WIDTH;
	DeviceManager::getInstance()->getDevice()->ColorFill(_surface, NULL, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));

	DeviceManager::getInstance()->getDevice()->StretchRect(
		_surface,
		NULL,
		DeviceManager::getInstance()->getSurface(),
		&r,
		D3DTEXF_NONE
	);
	//_background->render(spriteHandler);
}
