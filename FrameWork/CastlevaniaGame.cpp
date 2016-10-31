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
	_viewport->setPositionWorld(GVector2(_simon->getPositionX() - WINDOW_WIDTH / 2, WINDOW_HEIGHT));
	//=====================TESTING==========================//
}

void CastlevaniaGame::draw()
{
	//=====================TESTING==========================//
	_backGround->draw(_spriteHandler, _viewport);
	_simon->draw(_spriteHandler, _viewport);

	//=====================TESTING==========================//
}

void CastlevaniaGame::loadResource()
{
	//=====================TESTING==========================//
	SpriteManager::getInstance()->loadResource(_spriteHandler);
	_simon = new Simon();
	_simon->init();
	//=====================TESTING==========================//
	_backGround = Map::LoadFromFile("Resources//Maps//test.xml", eID::MAPSTAGE1);
}