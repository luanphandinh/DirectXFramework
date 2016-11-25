#pragma once
#include"define.h"
#include "Scene.h"
#include "Game.h"
#include"Animation.h"
#include<map>
#include"Simon.h"
#include"Map.h"
#include "ItemManager.h"
#include "ObjectFactory.h"
#include"GameStatusBoard.h"
#include"ActiveWeapon.h"
<<<<<<< HEAD
#include "ScenarioManager.h"
#include "Door.h"
#define DOOR_VIEWPORT_ANCHOR		2111


=======
#include"Level2Director.h"

#include"Sword.h"
#include"ThrowingAxe.h"
>>>>>>> 6f5e0eb664cfa72f86d01e56c5cafce7be0495aa
using namespace std;
_USING_FRAMEWORK

class PlayScene :
	public Scene
{
public:
	PlayScene();
	~PlayScene();

	bool init() override;
	void updateInput(float dt) override;
	void update(float dt) override;
	void draw(LPD3DXSPRITE spriteHandle) override;
	void release() override;

	void setViewport(Viewport* viewport);
	Simon* getSimon();
private:
	//==========================TEST=========================//
	void updateViewport(BaseObject* objTracker);
	BaseObject* _simon;
	BaseObject* _itemManager;
	BaseObject* _spearKnight;
	BaseObject* _bat;
	BaseObject* _medusaHead;
	Map* _background;
	vector<BaseObject*>* _mapObject;
	BaseObject** _testItem;
	GameStatusBoard* _gameStatusBoard;
<<<<<<< HEAD

	BaseObject* _door;
	ScenarioManager* _director;
	ScenarioManager* _directorPassDoor;

	bool flagDoorScenario;
	void doorSceneViewport(float deltaTime, bool& finish);
	void passDoorScene(float deltatime, bool& isFinish);

	void ScenarioMoveViewport(float deltaTime);
	void ScenarioPassDoor(float deltaTime);
=======
	//Level2Director* _director;
	bool _isSwitchSence;
>>>>>>> 6f5e0eb664cfa72f86d01e56c5cafce7be0495aa
	//==========================TEST=========================//
};

