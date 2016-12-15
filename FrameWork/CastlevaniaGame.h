#pragma once
#include "Game.h"
#include "SceneManager.h"
#include "PlayScene.h"
#include "IntroScene.h"
#include "GameOverScene.h"
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

};
_NAMESPACE_FRAMEWORK_END

