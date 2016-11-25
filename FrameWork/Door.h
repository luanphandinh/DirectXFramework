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
<<<<<<< HEAD
	// chắc là ko nên va chạm :v
	//void onCollisionBegin(CollisionEventArg*);
	//void onCollisionEnd(CollisionEventArg*);
	//float checkCollision(BaseObject*, float);
=======
	// chắc sẽ ko dùng
	//void onCollisionBegin(CollisionEventArg*);
	//void onCollisionEnd(CollisionEventArg*);
	//float checkCollision(BaseObject*, float);

>>>>>>> 6f5e0eb664cfa72f86d01e56c5cafce7be0495aa
	void setPosition(GVector2);
	IComponent* getComponent(string);

private:
	map<string, IComponent*> _listComponent;
	map<int, Animation*> _animations;
	BaseObject* prevObject;

	StopWatch* _stopwatch;
	void changeDirection();
<<<<<<< HEAD
	bool isVirgin;
=======
>>>>>>> 6f5e0eb664cfa72f86d01e56c5cafce7be0495aa
	void updateClosing();
	int aniCount;

};
