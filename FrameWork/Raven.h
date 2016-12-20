#pragma once

#include "Animation.h"
#include "IComponent.h"
#include "BaseEnemy.h"
#include "StopWatch.h"
#include "HitEffect.h"
#define RAVEN_SPEED 90
#define RAVEN_HITPOINT 20
#define RAVEN_SCORE 100

class Raven : public BaseEnemy {
public:
	/* Constructor: dơi xanh sà xuống simon và bay thẳng
	@status: HANGING,FLYING
	@pos: Vị trí
	@direction: hướng chạy, -1 là bên trái, 1 là bên phải
	@x, y: tọa độ của pos
	*/
	Raven(eStatus status, GVector2 pos, int direction);
	~Raven();

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

	void changeDirection();
	void flyingDown();
	void fly();
	// Init values

	int hack;
	bool _isHitted;
	StopWatch* _stopWatch;
	bool checkIfOutOfScreen();
	void updateHanging();
	BaseObject *_burning;
	int _direction;
};
