#include "CastlevaniaGame.h"

_USING_FRAMEWORK

CastlevaniaGame::~CastlevaniaGame()
{

}


CastlevaniaGame::CastlevaniaGame(HINSTANCE hInstance, LPWSTR title) :Game(hInstance,title,WINDOW_WIDTH,WINDOW_HEIGHT)
{}

CastlevaniaGame::~CastlevaniaGame()
{}

void CastlevaniaGame::init()
{
	Game::init();
}

void CastlevaniaGame::release()
{}

void CastlevaniaGame::updateInput(float deltaTime)
{}

void CastlevaniaGame::update(float deltaTime)
{}

void CastlevaniaGame::draw()
{}

void CastlevaniaGame::LoadResource()
{

}