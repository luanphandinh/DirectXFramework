#pragma once

#include "Animation.h"
#include "IComponent.h"
#include "BaseEnemy.h"
#include "StopWatch.h"

#define GHOST_SPEED 50
#define GHOST_HITPOINT 20
#define GHOST_SCORE 100

class Ghost : public BaseEnemy {
public:
	/* Constructor: con ma bay từ dưới đất lên attack simon
	@status: HIDING,FLYING
	@pos: Vị trí
	@direction: hướng bay, -1 là bên trái, 1 là bên phải
	@x, y: tọa độ của pos
	*/
	Ghost(eStatus status, GVector2 pos, int direction);
	~Ghost();

	void init();
	void update(float);
	void draw(LPD3DXSPRITE, Viewport*);
	void release();

	float checkCollision(BaseObject*, float);
	void die();
	//void setPosition(GVector2);
	GVector2 getVelocity() override;
	int getDirection();
	IComponent* getComponent(string);

private:
	map<string, IComponent*> _listComponent;
	map<int, Animation*> _animations;
	BaseObject* prevObject;
	eDirection _flyingDirection;
	void updateDirection();
	void changeDirection(eDirection dir);
	void flyingUp();
	void fly();
	// Init values
	GVector2 trackedPosition;
	bool _isHiding;
	bool _flyUp;

	int hack;
	bool _isHitted;
	StopWatch* _stopWatch;
	void checkIfOutOfScreen();
	void updateHiding();
	int _direction;

	BaseObject *_burning;
};
