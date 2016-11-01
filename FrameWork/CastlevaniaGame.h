#pragma once
#include "Game.h"
//include để test texture sau này xóa ko cân thiết
#include"Texture.h"
#include"Sprite.h"
#include"Animation.h"
#include<map>
#include"Simon.h"
#include"Map.h"
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
	void updateViewport(BaseObject* objTracker);
	Simon* _simon;
	Map* _backGround;
	Viewport* _viewport;
};
_NAMESPACE_FRAMEWORK_END

