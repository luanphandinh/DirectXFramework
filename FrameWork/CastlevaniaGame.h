#pragma once
#include "Game.h"
//include để test texture sau này xóa ko cân thiết
#include"Texture.h"
#include"Sprite.h"
#include"Animation.h"
_NAMESPACE_FRAMEWORK_BEGIN
class CastlevaniaGame : public Game
{
public:
	CastlevaniaGame(HINSTANCE hInstance,LPWSTR title);
	~CastlevaniaGame();

	void init();		//init your objects
	void release();		//release the objets

	void updateInput(float deltaTime);
	void update(float deltaTime);
	void draw();		//draw your objects
	void loadResource();//

	/////include để test texture sau này xóa ko cân thiết
	Texture* _test_texture;
	Sprite* _test_sprite; 
	Animation* _test_animation;
};
_NAMESPACE_FRAMEWORK_END

