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
#include"Level2Director.h"
#include"ScenarioManager.h"

#include"Sword.h"
#include"ThrowingAxe.h"
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
	BaseObject* getObject(eID id);
private:
	//==========================TEST=========================//
	void updateViewport(BaseObject* objTracker);
	BaseObject* _simon;
	BaseObject* _itemManager;
	BaseObject* _spearKnight;
	BaseObject* _bat;
	BaseObject* _medusaHead;
	Map* _backGround;
	vector<BaseObject*>* _mapObject;
	BaseObject** _testItem;
	GameStatusBoard* _gameStatusBoard;
	//Level2Director* _director;
	bool _isSwitchSence;

	// Hàng họ để mở cửa :v
	bool flagDoorScenario;
	ScenarioManager* _directorDoor;
	ScenarioManager* _directorPassDoor;

	void passDoorScene(float deltatime, bool& isFinish);
	// Tự đi qua cửa
	void ScenarioPassDoor(float deltatime);

	void doorScene(float dt, bool& finish);
	// Dịch screen đi gặp cái cửa
	void ScenarioMoveViewport(float deltatime);

	//==========================TEST=========================//
};

