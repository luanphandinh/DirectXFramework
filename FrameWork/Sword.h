#pragma once
#include"Weapon.h"
#define SWORD_SPEED GVector2(250,0)
#define SWORD_DISTANCE GVector2(150,0)
class Sword : public Weapon
{
public:
	Sword(GVector2 startPosition, eItemType type, eDirection dir);
	~Sword();

	void init() override;
	void update(float deltatime) override;
	void draw(LPD3DXSPRITE, Viewport*) override;
	void release() override;

	virtual float checkCollision(BaseObject* object, float dt) override;

	//virtual float checkCollision(BaseObject* object, float dt) override;
	virtual void initWeaponComponent() override;

protected:
	int		_damage;
	GVector2 _distance;
};

