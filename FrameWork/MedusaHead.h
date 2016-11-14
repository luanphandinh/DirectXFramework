#pragma once

#include "Animation.h"
#include "IComponent.h"
#include "BaseEnemy.h"
#include "StopWatch.h"

#define MEDUSAHEAD_SPEED 50
#define MEDUSAHEAD_HITPOINT 1
#define MEDUSAHEAD_SCORE 100

// TEST VALUE
#define START_POSITION			GVector2(600.0f, 300.0f)	// vị trí khởi tạo
#define MEDUSAHEAD_HORIZONTAL_VELOC		GVector2(100.0f, 0.0f)		// vận tốc ngang
#define MEDUSAHEAD_FREQUENCY		0.9f						// tần số
#define MEDUSAHEAD_AMPLITUDE		GVector2(0, 32.0f)			// biên độ
class MedusaHead : public BaseEnemy {
public:
	/* Contructor
	* @status: HIDING,FLYING
	* @direction: hướng bay, -1 là bên trái, 1 là bên phải
	* @pos: vị trí bắt đầu.
	* @hVeloc: vận tốc tuyến tính, tạo một phần chuyển động thẳng.
	* @ampl: biên độ của chuyển động tuần hoàn.
	* @freq: tần số góc của chuyển động tuần hoàn.
	*/
	MedusaHead(eStatus status, int direction, GVector2 pos, 
		GVector2 hVeloc, GVector2 ampl, float freq);
	~MedusaHead();

	void init();
	void update(float);
	void draw(LPD3DXSPRITE, Viewport*);
	void release();
	void onCollisionBegin(CollisionEventArg*);
	void onCollisionEnd(CollisionEventArg*);
	float checkCollision(BaseObject*, float);
	void die();
	void setPosition(GVector2);
	IComponent* getComponent(string);

private:
	map<string, IComponent*> _listComponent;
	map<int, Animation*> _animations;
	BaseObject* prevObject;

	// Init values
	GVector2	_beginPosition;
	GVector2	_horizontalVeloc;
	GVector2	_amplitude;
	float		_frequence;

	// bay ra khỏi view sẽ chết? nếu muốn thì set trong update
	void checkIfOutOfScreen();

	// nếu view chưa tới chỗ nó thì nó ko bay
	void updateHiding();

};
