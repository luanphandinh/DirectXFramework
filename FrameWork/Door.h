#pragma once

#include "Animation.h"
#include "IComponent.h"
#include "StopWatch.h"
#include "BaseObject.h"
#include "CollisionBody.h"

#define TOTAL_ANI 4

class Door :public BaseObject{
public:
	/*
	@status: CLOSING,OPENING
	@pos: Vị trí
	@direction: hướng mở, -1 là bên trái, 1 là bên phải
	@x, y: tọa độ của pos
	*/
	Door(eStatus status, GVector2 pos, int direction);
	~Door();

	void init();
	void update(float);
	void draw(LPD3DXSPRITE, Viewport*);
	void release();
	// chắc là ko nên va chạm :v
	//void onCollisionBegin(CollisionEventArg*);
	//void onCollisionEnd(CollisionEventArg*);
	//float checkCollision(BaseObject*, float);
	void setPosition(GVector2);
	IComponent* getComponent(string);

private:
	map<string, IComponent*> _listComponent;
	map<int, Animation*> _animations;
	BaseObject* prevObject;

	StopWatch* _stopwatch;
	void changeDirection();
	bool isVirgin;
	void updateClosing();
	int aniCount;

};
