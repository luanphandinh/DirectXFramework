#pragma once
/*
	Cách sử dụng:
	lớp basic game.
	kế thừa lớp này vào lớp game castleVania
	onverride virtual methods(init,release,draw,updateinput,update)
	ví dụ:
	class CastleVania : public Game
	{
	public:
		void init()
		{
			Game::init();
			some objects created
		}
		void release()
		{
			game::release();
			some objects created
		}
		void updateInput(float deltaTime){check if input event and implement action}
		void update(float deltaTime){update all object on screen}
		void draw(float deltaTime){draw to screen}
		//5 overrided methods
		//updateinput,update,draw.they care called aytomatic in Game::run()
	}

	trong main.cpp
	int main function:
	call:
	game* game = new Castlevania(..);
	game->init();
	game->run();
	game->release();
	delete game;
*/
#include<windows.h>
#include"Graphics.h"
#include"DeviceManager.h"
#include"GameTime.h"
#include"Texture.h"
#include"Viewport.h"
#include"InputController.h"

_NAMESPACE_FRAMEWORK_BEGIN
class Game
{
public:
	Game();
	virtual ~Game();

	static int isExit;
	Game(HINSTANCE, LPWSTR = L"Window Game", int width = 800, int height = 600,
		int fps = 30, int isFullScreen = 0);
	void virtual init();		//init your objects
	void virtual release();		//release the objets

	void virtual updateInput(float deltaTime);
	void virtual update(float deltaTime);	
	void virtual draw();		//draw your objects
	void virtual loadResource();//

	void run();
	void render();
	static void exit();
	static Graphics* getWindow();
protected:
	pGameTime _gameTime;
	pDeviceManager _deviceManager;
	LPD3DXSPRITE _spriteHandler;
	static Graphics *hWindow;
	pInputController _input;
	//sau này sẽ chuyển sang Scenceplay
	Viewport* _viewport;
private:
	float _frameRate;
	float _oldTime;
	float _deltaTime;
};

typedef Game* pGame;
_NAMESPACE_FRAMEWORK_END
