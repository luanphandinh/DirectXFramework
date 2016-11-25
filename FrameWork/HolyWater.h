#pragma once
#include"Weapon.h"
#include"Animation.h"
#include"StopWatch.h"
#include"define.h"
#define HOLYWATER_SPEED GVector2(150,150)
class HolyWater : public Weapon
{
public:
	HolyWater(GVector2 startPosition, eItemType type, eDirection dir);
	~HolyWater();

	void init() override;
	void update(float deltatime) override;
	void draw(LPD3DXSPRITE, Viewport*) override;
	void release() override;

	virtual float checkCollision(BaseObject* object, float dt) override;

	//virtual float checkCollision(BaseObject* object, float dt) override;
	virtual void initWeaponComponent() override;
protected:
	int		_damage;
	Animation*	_burstAnimation;
	Animation*	_throwAnimation;
	StopWatch*	_burstStopWatch;
};

