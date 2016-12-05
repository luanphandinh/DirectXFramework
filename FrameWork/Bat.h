#pragma once

#include "Animation.h"
#include "IComponent.h"
#include "BaseEnemy.h"
#include "StopWatch.h"

#define BAT_SPEED 90
#define BAT_HITPOINT 20
#define BAT_SCORE 100

class Bat : public BaseEnemy {
public:
	/* Constructor: dơi xanh sà xuống simon và bay thẳng
	@status: HANGING,FLYING
	@pos: Vị trí
	@direction: hướng chạy, -1 là bên trái, 1 là bên phải
	@x, y: tọa độ của pos
	*/
	Bat(eStatus status, GVector2 pos, int direction);
	Bat(eStatus status, float x, float y, int direction);
	/* Contructor: con dơi màu đỏ sẽ bay hình sin
	* @pos: vị trí bắt đầu.
	* @hVeloc: vận tốc tuyến tính, tạo một phần chuyển động thăng.
	* @ampl: biên độ của chuyển động tuần hoàn.
	* @freq: tần số góc của chuyển động tuần hoàn.
	*/
	Bat(eStatus status,  int direction, GVector2 pos, GVector2 hVeloc, GVector2 ampl, float freq);
	~Bat();

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
	bool _isHitted;
	StopWatch* _stopWatch;
	void checkIfOutOfScreen();
	void updateHanging();
	BaseObject *_burning;
};
