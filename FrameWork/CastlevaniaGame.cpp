#include "CastlevaniaGame.h"

_USING_FRAMEWORK

CastlevaniaGame::~CastlevaniaGame()
{
	
}


CastlevaniaGame::CastlevaniaGame(HINSTANCE hInstance, LPWSTR title) :Game(hInstance,title,WINDOW_WIDTH,WINDOW_HEIGHT)
{

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
	//=====================TESTING==========================//
}

void CastlevaniaGame::draw()
{
	//=====================TESTING==========================//
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
}