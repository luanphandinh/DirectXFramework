﻿#include "CastlevaniaGame.h"

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

	SceneManager::getInstance()->addScene(new PlayScene());
}

void CastlevaniaGame::release()
{
	SceneManager::getInstance()->release();
}

void CastlevaniaGame::updateInput(float deltaTime)
{
	SceneManager::getInstance()->updateInput(deltaTime);
}

void CastlevaniaGame::update(float deltaTime)
{
	SceneManager::getInstance()->update(deltaTime);
}

void CastlevaniaGame::draw()
{
	this->_spriteHandler->Begin(D3DXSPRITE_ALPHABLEND);

	SceneManager::getInstance()->draw(_spriteHandler);

	this->_spriteHandler->End();
}

void CastlevaniaGame::loadResource()
{
	SpriteManager::getInstance()->loadResource(_spriteHandler);
	SoundManager::getInstance()->loadSound(Game::hWindow->getWnd());

}
