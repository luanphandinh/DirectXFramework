#pragma once
#include"define.h"
#include "Scene.h"
#include "Game.h"
#include"Texture.h"
#include"Sprite.h"
#include"Animation.h"
#include<map>
#include"Simon.h"
#include"Map.h"
#include"Land.h"
#include "SpearKnight.h"
#include"Stair.h"
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

private:
	//==========================TEST=========================//
	void updateViewport(BaseObject* objTracker);
	Simon* _simon;
	SpearKnight* _spearKnight;
	Map* _backGround;
	Land* _land;
	Land* _land2;
	Land* _land3;
	Stair** _stairsRight;
	Stair** _stairsLeft;
	
	//==========================TEST=========================//
};

