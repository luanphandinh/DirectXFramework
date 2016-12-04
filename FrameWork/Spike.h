#pragma once
#pragma once

#include "Animation.h"
#include "IComponent.h"
#include "StopWatch.h"
#include "BaseObject.h"
#include "CollisionBody.h"
#define SPEED_RETURN 100
#define SPEED_FALLING 200
class Spike :public BaseObject {
public:
	/*
	@status: WAITING,FALLING
	@pos: Vị trí
	@x, y: tọa độ của pos
	*/
	Spike(eSpikeState state, GVector2 pos);
	~Spike();

	void init();
	void update(float);
	void draw(LPD3DXSPRITE, Viewport*);
	void release();

	void onCollisionBegin(CollisionEventArg*);
	void onCollisionEnd(CollisionEventArg*);
	float checkCollision(BaseObject*, float);

	void setPosition(GVector2);
	IComponent* getComponent(string);

private:
	map<string, IComponent*> _listComponent;
	map<int, Animation*> _animations;
	BaseObject* prevObject;
	StopWatch* _stopWatch;
	eStatus _currentAnimateIndex;
	void updateCurrentAnimateIndex();
	void updateWaiting();
	int _hack;

	eStatus spikeState;
};

