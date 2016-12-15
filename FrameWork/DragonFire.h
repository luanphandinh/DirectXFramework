#pragma once
#include"Weapon.h"
#include"Animation.h"
#include"StopWatch.h"
#include"HitEffect.h"
#define DRAGON_FIRE_SPEED GVector2(160,0)
class DragonFire : public Weapon
{
public:
	DragonFire(GVector2 startPos,eDirection dir);
	~DragonFire();

	void init() override;
	void update(float deltatime) override;
	void draw(LPD3DXSPRITE, Viewport*) override;
	void release() override;

	void checkPosition();

	virtual float checkCollisionWeapon(BaseObject* object, float dt) override;

	//virtual float checkCollision(BaseObject* object, float dt) override;
	virtual void initWeaponComponent() override;
private:
	HitEffect*	_hitEffect;
	StopWatch*	_burstStopWatch;
};

