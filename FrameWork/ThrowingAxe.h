#pragma once
#include"Weapon.h"
#define THROWING_AXE_DISTANCE GVector2(150,350)
class ThrowingAxe : public Weapon
{
public:
	ThrowingAxe(GVector2 startPosition, eItemType type, eDirection dir = eDirection::NONE);
	~ThrowingAxe();

	void init() override;
	void update(float deltatime) override;
	void draw(LPD3DXSPRITE, Viewport*) override;
	void release() override;
	//virtual float checkCollision(BaseObject* object, float dt) override;
	virtual void initWeaponComponent() override;

protected:
	GVector2 _distance;
	
};

