#include "CastlevaniaGame.h"

_USING_FRAMEWORK

CastlevaniaGame::~CastlevaniaGame()
{
	
}


CastlevaniaGame::CastlevaniaGame(HINSTANCE hInstance, LPWSTR title) :Game(hInstance,title,WINDOW_WIDTH,WINDOW_HEIGHT)
{
	//=====================TESTING==========================//
	_viewport = new Viewport(0, WINDOW_HEIGHT , WINDOW_WIDTH, WINDOW_HEIGHT);
	//=====================TESTING==========================//
}

void CastlevaniaGame::init()
{
	Game::init();
}

void CastlevaniaGame::release()
{}

void CastlevaniaGame::updateInput(float deltaTime)
{
}

void CastlevaniaGame::update(float deltaTime)
{
	//=====================TESTING==========================//
	_simon->update(deltaTime);
	updateViewport(_simon);
	//=====================TESTING==========================//
}

void CastlevaniaGame::draw()
{
	//=====================TESTING==========================//
	_backGround->draw(_spriteHandler, _viewport);
	_simon->draw(_spriteHandler, _viewport);
	_land->draw(_spriteHandler, _viewport);
	//=====================TESTING==========================//
}

void CastlevaniaGame::loadResource()
{
	//=====================TESTING==========================//
	SpriteManager::getInstance()->loadResource(_spriteHandler);
	_simon = new Simon();
	_simon->init();
	
	_backGround = Map::LoadFromFile("Resources//Maps//test.xml", eID::MAPSTAGE1);

	_land = new Land(0, 64, 200, 20, eDirection::TOP);
	//=====================TESTING==========================//
}
//=====================TESTING==========================//
void CastlevaniaGame::updateViewport(BaseObject* objTracker)
{
	// Vị trí hiện tại của viewport. 
	GVector2 current_position = _viewport->getPositionWorld();

	GVector2 worldsize = this->_backGround->getWorldSize();
	// Bám theo object.
	GVector2 new_position = GVector2(max(objTracker->getPositionX() - WINDOW_WIDTH / 2, 0), WINDOW_HEIGHT);

	// Không cho đi quá map.
	if (new_position.x + WINDOW_WIDTH > worldsize.x)
	{
		new_position.x = worldsize.x - WINDOW_WIDTH;
	}

	_viewport->setPositionWorld(new_position);
}
//=====================TESTING==========================//