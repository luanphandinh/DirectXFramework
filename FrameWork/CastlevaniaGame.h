#pragma once
#include "Game.h"
//include để test texture sau này xóa ko cân thiết
#include"Texture.h"
#include"Sprite.h"
#include"Animation.h"
#include<map>
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

	//==========================TEST=========================//
	Texture* _test_texture;
	Sprite* _test_sprite; 
	map<eStatus,Animation*> _test_animations;
	float vX = 0;
};
_NAMESPACE_FRAMEWORK_END

