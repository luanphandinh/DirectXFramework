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
	if (_input->isKeyDown(DIK_D))
	{
		vX = 1.0f;
		
	}
	else vX = 0.0f;
}

void CastlevaniaGame::update(float deltaTime)
{
	//=====================TESTING==========================//
	//_test_animation->setTimeAnimate(deltaTime);
	//_test_animation->update(deltaTime);
	_test_sprite->setPositionX(_test_sprite->getPositionX() + vX);
	//_test_animations[eStatus::MOVING_LEFT]->setLoop(true);
	if (vX > 0.0f)
		_test_animations[eStatus::MOVING_LEFT]->update(deltaTime);
	else _test_animations[eStatus::NORMAL]->update(deltaTime);
	//_test_animation->setIndex(2);
	//=====================TESTING==========================//
}

void CastlevaniaGame::draw()
{
	//=====================TESTING==========================//
	//RECT* rect = new RECT();
	//rect->top = 0;
	//rect->right = 92;
	//rect->left = 0;
	//rect->bottom = 60;
	////GVector2 pos = GVector2(46, 90);
	//GVector2 pos = GVector2(46, 30);
	//GVector2 center = GVector2(46, 30);
	//GVector2	_origin = GVector2(0.0f, 0.0f);
	//	//ko scale
	//GVector2	_scale = GVector2(1.0f, 1.0f);
	//float    _zIndex = 1;
	//float    _rotate = 00.0f;
	////_test_texture->render(_spriteHandler, rect,/**_viewport,*/ pos,_scale,_rotate,_origin,1.0f);
	//_test_texture->render(_spriteHandler, rect, new GVector3(center.x, center.y, 0.0f), new GVector3(pos.x, pos.y, 0.0f));
	//_test_sprite->render(_spriteHandler,_viewport);
	if (vX > 0.0f)
		_test_animations[eStatus::MOVING_LEFT]->draw(_spriteHandler, _viewport);
	else _test_animations[eStatus::NORMAL]->draw(_spriteHandler, _viewport);
}

void CastlevaniaGame::loadResource()
{
	//=====================TESTING==========================//
	/*_test_texture = new Texture();
	
	_test_texture->loadFromFile(_spriteHandler, L"kitty_right.bmp");

	_test_sprite = new Sprite(_spriteHandler, L"kitty_right.bmp");
	
	_test_sprite->setPosition(46, 30);

	_test_animation = new Animation(_test_sprite, 6, 3, 0.05f);*/
	SpriteManager::getInstance()->loadResource(_spriteHandler);
	_test_sprite = SpriteManager::getInstance()->getSprite(eID::SIMON);
	_test_sprite->setScale(2.0f);
	//_test_sprite->drawBounding(true);
	_test_sprite->setPosition(50,50,1.0f);
	_test_animations[eStatus::MOVING_LEFT] = new Animation(_test_sprite, 0.08f);
	_test_animations[eStatus::MOVING_LEFT]->addFrameRect(eID::SIMON, "run_01", "run_02", "run_03", NULL);
	_test_animations[eStatus::NORMAL] = new Animation(_test_sprite, 0.1f);
	_test_animations[eStatus::NORMAL]->addFrameRect(eID::SIMON, "normal", NULL);
}