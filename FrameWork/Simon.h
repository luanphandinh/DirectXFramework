#pragma once
#include"define.h"
#include"Animation.h"
#include"InputController.h"
#include"BaseObject.h"
#include"IComponent.h"
#include"CollisionBody.h"
#include"Land.h"
#include"Stair.h"
#include"StopWatch.h"
#include"SceneManager.h"
#include"LifeUI.h"
#include"ActiveWeapon.h"
//========================TESTING===========================//
#include"Item.h"
#include"ItemManager.h"
#include"HeartItem.h"
#include"WhipUpgrade.h"
#include"MoneyBag.h"
#include"Sword.h"
#include"ThrowingAxe.h"
//========================TESTING===========================//
_NAMESPACE_FRAMEWORK_BEGIN
#define SIMON_MOVING_SPEED 126

#define GRAVITY 800
#define SIMON_JUMP_VELOCITY 350
// Sửa phần lên cầu thang cho hết sida
#define SIMON_UPSTAIR_VELOCITY_X 82
#define SIMON_UPSTAIR_VELOCITY_Y 136
#define REVIVE_TIME 2000

[event_receiver(native)]
class Simon : public BaseObject,public IControlable
{
public:
	Simon();
	~Simon();
	//overiride IControlable method
	void updateInput(float dt);
	//overiride BaseObject methods
	void init();
	void update(float deltatime);
	void draw(LPD3DXSPRITE spriteHandle, Viewport* viewport);
	void release();

	/*
		Event for event_reciever
	*/
	void onKeyPressed(KeyEventArg* key_event);
	/*
		Event for event_reciever
	*/
	void onKeyReleased(KeyEventArg* key_event);

	void setStatus(eStatus status) override;

	/*
		Cập nhật lại trạng thái cho nhân vật
	*/
	void checkPosition();
	void updateStatus(float deltatime);
	/*
		CheckCollision
	*/
	float checkCollision(BaseObject* object, float dt);
	void onCollisionBegin(CollisionEventArg* collision_arg);
	void onCollisionEnd(CollisionEventArg* collision_arg);

	/*
		Cập nhật lại rect để vẽ 
	*/
	void updateCurrentAnimateIndex();

	// Character action.

	/*
		romve hết 2 trạng thái jump và falling của simon
		Nếu đứng trên cầu thang thì cho đứng
		ko thì remove luôn trạng thái đứng ở cầu thang
	*/
	void standing();
	void moveRight();
	void moveLeft();
	/*
		Trạng thái lúc object nhảy lên
	*/
	void jump();
	/*
		Trạng thái lúc objec rơi xuống
	*/
	void falling();
	void sit();
	void upstair();
	void downstair();
	void die();
	void revive();
	void hitting();
	void getWeapon();
	void forceMoveRight();
	void unforceMoveRight();
	void forceMoveLeft();
	void unforceMoveLeft();
	void forceJump();
	void unforceJump();
	void removeGravity();
	void enableGravity(bool);

	float getMovingSpeed();

	RECT getBounding() override;
	
	eDirection getDirection();

	
private:
	map<int, Animation*> _animations;
	map<string, IComponent*> _componentList;

	float _movingSpeed;

	bool _canJumpDown;
	bool _canOnStair;
	int _whipLevel;
	//Sử dụng để animation hitting khi có stopwatch
	bool _isHitting;
	//Sử dụng để animation throwing khi có stopwatch
	bool _isThrowing;
	//Hướng cầu thang đi lên qua phải hoặc đi lên qua trái
	eStairDirection _stairDirection;

	StopWatch* _stopWatch;
	StopWatch* _reviveStopWatch;
	StopWatch* _hittingStopWatch;
	StopWatch* _throwItemStopWatch;

	eStatus _currentAnimationIndex;

	GVector2 getVelocity();

	BaseObject* _preObject;
	// reset các thuộc tính lại giá trị ban đầu.
	void resetValues();

	//========================TESTING===========================//
	Item** _testItem;
	//========================TESTING===========================//
};
_NAMESPACE_FRAMEWORK_END

