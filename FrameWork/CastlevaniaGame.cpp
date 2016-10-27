#include "CastlevaniaGame.h"

_USING_FRAMEWORK

CastlevaniaGame::~CastlevaniaGame()
{

}


CastlevaniaGame::CastlevaniaGame(HINSTANCE hInstance, LPWSTR title) :Game(hInstance,title,WINDOW_WIDTH,WINDOW_HEIGHT)
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
{
	RECT* rect = new RECT();
	rect->top = 100;
	rect->right = 200;
	rect->left = 100;
	rect->bottom = 200;
	GVector3* pos = new GVector3(20, 20, 0.0f);

	_test_texture->render(_spriteHandler,NULL,NULL,pos);
}

void CastlevaniaGame::LoadResource()
{
	_test_texture = new Texture();
	//if (_test_texture->loadFromFile(_spriteHandler, L"kitty_right.bmp") != )
	_test_texture->loadFromFile(_spriteHandler, L"kitty_right.bmp");
	/*try{ !; }
	catch(exception e)
	{
		throw new 
	}*/
		
}