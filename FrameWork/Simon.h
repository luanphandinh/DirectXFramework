#pragma once
#include"define.h"
#include"Animation.h"
#include"InputController.h"
#include"BaseObject.h"
#include"IComponent.h"
#include"CollisionBody.h"
_NAMESPACE_FRAMEWORK_BEGIN
#define SIMON_MOVING_SPEED 150
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

	/*
		Cập nhật lại rect để vẽ 
	*/
	void updateCurrentAnimateIndex();

	// Character action.
	void standing();
	void moveRight();
	void moveLeft();
	void jump();

	float getMovingSpeed();

	RECT getBounding() override;
private:
	map<eStatus, Animation*> _animations;
	map<string, IComponent*> _componentList;

	float _movingSpeed;

	bool _canJumpDown;

	eStatus _currentAnimationIndex;

	GVector2 getVelocity();
};
_NAMESPACE_FRAMEWORK_END

