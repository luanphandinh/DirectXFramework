#pragma once


#include "Animation.h"
#include "IComponent.h"
#include "BaseEnemy.h"
#include "Simon.h"
#include "StopWatch.h"
#include "HitEffect.h"
using namespace std;

#define SNAKE_SPEED 150
#define SNAKE_HITPOINT 1
#define SNAKE_SCORE 100

class Snake : public BaseEnemy {
public:
	Snake(GVector2 pos, eDirection direction);

	~Snake();

	void init();
	void update(float);
	void draw(LPD3DXSPRITE, Viewport*);
	void release();

	float checkCollision(BaseObject*, float) override;
	IComponent* getComponent(string);
	GVector2 getVelocity() override;
private:
	map<string, IComponent*> _listComponent;
	map<int, Animation*> _animations;
	BaseObject* prevObject;
	BaseObject *_explosion;
	BaseObject *_burning;
	eDirection _direction;
	BaseObject* _preObject;
	void die();
	void checkPosition();
	eStatus _currentAnimationIndex;
	void updateCurrentAnimateIndex();
	// Vòng lặp thời gian, giúp knight di chuyển qua lại liên tục
	StopWatch *_loopwatch;
	StopWatch*	_burstStopWatch;
};