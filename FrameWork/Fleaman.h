#pragma once

#include "Animation.h"
#include "IComponent.h"
#include "BaseEnemy.h"
#include "Simon.h"
#include "StopWatch.h"
#include "HitEffect.h"
using namespace std;

#define FLEAMAN_SPEED 50
#define FLEAMAN_JUMP_VEL GVector2(85,100)
#define FLEAMAN_HIGH_JUMP_VEL GVector2(85,200)
#define FLEAMAN_HITPOINT 3
#define FLEAMAN_SCORE 100

class Fleaman : public BaseEnemy {
public:
	/*
	@status: WALKING
	@pos: Vị trí
	@direction: hướng chạy, -1 là bên trái, 1 là bên phải
	@x, y: tọa độ của pos
	*/
	Fleaman(eStatus status, GVector2 pos, int direction);

	~Fleaman();

	void init();
	void update(float);
	void draw(LPD3DXSPRITE, Viewport*);
	void release();
	float checkCollision(BaseObject*, float);
	void die();
	void setPosition(GVector2);
	GVector2 getVelocity();
	IComponent* getComponent(string);

private:
	map<string, IComponent*> _listComponent;
	map<int, Animation*> _animations;
	BaseObject* prevObject;
	BaseObject *_explosion;
	BaseObject *_burning;

	// Vòng lặp thời gian, giúp knight di chuyển qua lại liên tục
	StopWatch *_stopWatch;
	void changeDirection();
	bool isHitted;
	eStatus _currentAnimateIndex;
	void updateCurrentAnimateIndex();
	StopWatch*	_burstStopWatch;
	void getHitted();

	bool checkIfOutOfScreen();
	void updateSitting();
	void jump();
	void highJump();
};
