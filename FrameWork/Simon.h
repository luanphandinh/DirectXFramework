#pragma once
#include"define.h"
#include"Animation.h"
#include"InputController.h"
#include"BaseObject.h"
#include"IComponent.h"
#include"CollisionBody.h"
#include"Land.h"
_NAMESPACE_FRAMEWORK_BEGIN
#define SIMON_MOVING_SPEED 125
#define GRAVITY 800
#define SIMON_JUMP_VELOCITY 450

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

	float getMovingSpeed();

	RECT getBounding() override;

	
private:
	map<int, Animation*> _animations;
	map<string, IComponent*> _componentList;

	float _movingSpeed;

	bool _canJumpDown;

	eStatus _currentAnimationIndex;

	GVector2 getVelocity();

	BaseObject* _preObject;
	// reset các thuộc tính lại giá trị ban đầu.
	void resetValues();
};
_NAMESPACE_FRAMEWORK_END

