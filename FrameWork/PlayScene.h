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
private:
	//==========================TEST=========================//
	void updateViewport(BaseObject* objTracker);
	BaseObject* _simon;
	BaseObject* _itemManager;
	BaseObject* _spearKnight;
	BaseObject* _bat;
	BaseObject* _medusaHead;
	Map* _backGround;
	//BaseObject* _land;
	//BaseObject* _land2;
	//BaseObject* _land3;
	//BaseObject* _land4;
	//BaseObject** _stairsRight;
	//BaseObject** _stairsLeft;
	vector<BaseObject*>* _mapObject;
	BaseObject** _testItem;
	GameStatusBoard* _gameStatusBoard;
	//Level2Director* _director;
	bool _isSwitchSence;
	//==========================TEST=========================//
};

