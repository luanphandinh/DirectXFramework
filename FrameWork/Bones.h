#pragma once
#include"Weapon.h"
#include"Animation.h"
#include"HitEffect.h"
#define BONES_VELOC GVector2(150,350)
class Bones : public Weapon
{
public:
	Bones(GVector2 startPos, eDirection dir);
	~Bones();

	void init() override;
	void update(float deltatime) override;
	void draw(LPD3DXSPRITE, Viewport*) override;
	void release() override;

	void checkPosition();

	virtual float checkCollisionWeapon(BaseObject* object, float dt) override;

	//virtual float checkCollision(BaseObject* object, float dt) override;
	virtual void initWeaponComponent() override;
private:
	Animation* _animation;
	HitEffect*	_hitEffect;
	StopWatch*	_burstStopWatch;
};

