#pragma once

#include "Animation.h"
#include "IComponent.h"
#include "BaseEnemy.h"
#include "StopWatch.h"
#include "Snake.h"

#define MEDUSA_VELOCITY GVector2(80,0)
#define MEDUSA_HITPOINT 16
#define MEDUSA_SCORE 100
#define MEDUSA_HORIZONTAL_VELOC		GVector2(100.0f, 0.0f)		// vận tốc ngang
#define MEDUSA_FREQUENCY		1.0f						// tần số
#define MEDUSA_AMPLITUDE		GVector2(0.0,20.0f)			// biên độ

enum eFlyPath {
	HOLD,
	MEDIUMDISTANCE,
	LONGDISTANCE,
};

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
	void getHitted();
	IComponent* getComponent(string);

private:
	map<string, IComponent*> _listComponent;
	map<int, Animation*> _animations;
	BaseObject* prevObject;

	void changeDirection(eDirection dir);
	void updateDirection();
	void checkPosition();
	void flyingDown();
	void fly();
	void getFlyPath();
	void trackSimon();
	bool checkFlyDown();

	void createSnake();
	void removeSnake();
	vector<BaseObject*> _listObjects;
	Snake* _snake;

	bool _isHitted;
	// Init values
	GVector2	_beginPosition;
	GVector2	_horizontalVeloc;
	GVector2	_amplitude;
	float		_frequence;
	bool		_verticalflag;

	int hack;
	StopWatch* _stopWatch;
	StopWatch* _hidingStopWatch;
	StopWatch* _getHittedStopWatch;
	StopWatch* _flyingBackStopWatch;
	StopWatch* _flyDownStopWatch;
	StopWatch* _holdStopWatch;
	bool _isHiding;
	bool _isFlyingBack;
	bool _isHold;
	bool _flyDown;
	bool _flyUp;
	eFlyPath _flyPath;
	GVector2 _flyingBackPos;
	//void checkIfOutOfScreen();
	
	void flyingBack();
	void updateHiding();
	BaseObject *_burning;
	eDirection _flyingDirection;
	eDirection _flyingBackDirection;

	eID _createType;
	float _time;
	int _number;
	int _counter;
	int _maxObject;

	int _direction;
	bool _isOnePerOne;
};
