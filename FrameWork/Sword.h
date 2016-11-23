#pragma once
#include"Weapon.h"
class Sword : public Weapon
{
public:
	Sword(GVector2 startPosition, eItemType type, eDirection dir);
	~Sword();

	void init() override;
	void update(float deltatime) override;
	void draw(LPD3DXSPRITE, Viewport*) override;
	void release() override;

	//virtual float checkCollision(BaseObject* object, float dt) override;
	virtual void initWeaponComponent() override;
protected:
	int		_damage;
	long	_distance;
};

