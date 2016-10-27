#include<Windows.h>
#include<iostream>
#include"CastlevaniaGame.h"
_USING_FRAMEWORK
#define APP_TITLE L"Castlevania"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpcmdLine, int nCmdShow)
{
	Game* myGame = new CastlevaniaGame(hInstance, APP_TITLE);
	myGame->init();
	myGame->run();
	myGame->release();
	delete myGame;
}