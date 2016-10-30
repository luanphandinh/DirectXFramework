#pragma once
#include"define.h"
#include"Animation.h"
#include"InputController.h"
#include"BaseObject.h"

_NAMESPACE_FRAMEWORK_BEGIN

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

	// Character action.
	void standing();
	void moveRight();

	float getMovingSpeed();

	RECT getBounding() override;
private:
	map<eStatus, Animation*> _animations;

	float _movingSpeed;

	eStatus _currentAnimationIndex;

	GVector2 getVelocity();
};
_NAMESPACE_FRAMEWORK_END

