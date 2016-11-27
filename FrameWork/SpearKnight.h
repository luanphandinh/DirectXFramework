#pragma once

#include "Animation.h"
#include "IComponent.h"
#include "BaseEnemy.h"
#include "Simon.h"
#include "StopWatch.h"
using namespace std;

#define SPEARKNIGHT_SPEED 50
#define SPEARKNIGHT_HITPOINT 16
#define SPEARKNIGHT_SCORE 100

class SpearKnight : public BaseEnemy {
public:
	/*
	@status: WALKING
	@pos: Vị trí
	@direction: hướng chạy, -1 là bên trái, 1 là bên phải
	@x, y: tọa độ của pos
	*/
	SpearKnight(eStatus status, GVector2 pos, int direction);
	SpearKnight(eStatus status, float x, float y, int direction);

	~SpearKnight();

	void init();
	void update(float);
	void draw(LPD3DXSPRITE, Viewport*);
	void release();
	void onCollisionBegin(CollisionEventArg*);
	void onCollisionEnd(CollisionEventArg*);
	float checkCollision(BaseObject*, float);
	void die();
	void setPosition(GVector2);
	GVector2 getVelocity();
	IComponent* getComponent(string);

private:
	map<string, IComponent*> _listComponent;
	map<int, Animation*> _animations;
	BaseObject* prevObject;

	// Vòng lặp thời gian, giúp knight di chuyển qua lại liên tục
	StopWatch *_loopwatch;
	void changeDirection();
	bool isHitted;
};
