#pragma once
#include"BaseObject.h"
#include"Animation.h"
#include"StopWatch.h"
#include"IComponent.h"
#include"define.h"
#include"CollisionBody.h"
#include<list>
#define BROKEN_BRICK_SPEED GVector2(100,350)

class BrokenBrick : public BaseObject
{
public:
	BrokenBrick(GVector2 pos,eDirection dir);
	~BrokenBrick(); 
	//implement inherited methods from baseObject
	void init() override;
	void update(float deltaTime) override;
	void draw(LPD3DXSPRITE, Viewport*) override;
	void release() override;
	
private:
	eDirection	_direction;
	map<string, IComponent*> _componentList;
	GVector2 initVeloc(GVector2 speed);
};

class Brick : public BaseObject
{
public:
	Brick(GVector2 pos, eItemID _dropItemId);
	~Brick();

	//implement inherited methods from baseObject
	void init() override;
	void update(float deltaTime) override;
	void draw(LPD3DXSPRITE, Viewport*) override;
	void release() override;
private:
	list<BrokenBrick*> _brokens;
	StopWatch* _stopWatch;
	map<string, IComponent*> _componentList;
	eItemID _dropItemId;
};

