#pragma once

#include "Animation.h"
#include "IComponent.h"
#include "BaseEnemy.h"
#include "StopWatch.h"

#define MEDUSA_SPEED 90
#define MEDUSA_HITPOINT 20
#define MEDUSA_SCORE 100

class Medusa : public BaseEnemy {
public:

	Medusa(GVector2 pos);
	~Medusa();

	void init();
	void update(float);
	void draw(LPD3DXSPRITE, Viewport*);
	void release();

	float checkCollision(BaseObject*, float);
	void die();
	void setPosition(GVector2);

	IComponent* getComponent(string);

private:
	map<string, IComponent*> _listComponent;
	map<int, Animation*> _animations;
	BaseObject* prevObject;

	void changeDirection();
	void flyingDown();
	void fly();
	// Init values
	GVector2	_beginPosition;
	GVector2	_horizontalVeloc;
	GVector2	_amplitude;
	float		_frequence;
	bool		_verticalflag;

	int hack;
	StopWatch* _stopWatch;
	StopWatch* _hidingStopWatch;
	bool _isHiding;
	//void checkIfOutOfScreen();
	void updateHiding();
	BaseObject *_burning;

	//
	vector<BaseObject*> _listObjects;

	eID _createType;
	float _time;
	int _number;
	int _counter;
	int _maxObject;

	int _direction;
	bool _isOnePerOne;
};
