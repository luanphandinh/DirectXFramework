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
	rect->top = 0;
	rect->right = 100;
	rect->left = 0;
	rect->bottom = 100;
	GVector2 pos = GVector2(200, 200);
	GVector2 center = GVector2(100, 100);
	GVector2	_origin = GVector2(0.5f, 0.5f);
		//ko scale
	GVector2	_scale = GVector2(2.0f, 2.0f);
	float    _zIndex = 1;
	float    _rotate = 90.0f;
	_test_texture->render(_spriteHandler, rect, pos,_scale,_rotate,_origin,0.0f);
}

void CastlevaniaGame::loadResource()
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