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
{}

void CastlevaniaGame::update(float deltaTime)
{}

void CastlevaniaGame::draw()
{
	//RECT* rect = new RECT();
	//rect->top = 0;
	//rect->right = 92;
	//rect->left = 0;
	//rect->bottom = 60;
	//GVector2 pos = GVector2(46, 90);
	////GVector2 center = GVector2(46, 46);
	//GVector2	_origin = GVector2(0.5f, 0.5f);
	//	//ko scale
	//GVector2	_scale = GVector2(1.0f, 1.0f);
	//float    _zIndex = 1;
	//float    _rotate = 00.0f;
	//_test_texture->render(_spriteHandler, rect,*_viewport, pos,_scale,_rotate,_origin,0.0f);
	_test_sprite->setPosition(46, 90);
	_test_sprite->render(_spriteHandler,_viewport);
}

void CastlevaniaGame::loadResource()
{
	//_test_texture = new Texture();
	//
	//_test_texture->loadFromFile(_spriteHandler, L"kitty_right.bmp");

	_test_sprite = new Sprite(_spriteHandler, L"kitty_right.bmp",6,3);
	
}