#pragma once
#include"Weapon.h"
#define BOORMERANG_DISTANCE GVector2(150,0)
#define BOORMERANG_SPEED GVector2(250,0)
class Boomerang :
	public Weapon
{
public:
	Boomerang(GVector2 startPosition, eItemType type, eDirection dir);
	~Boomerang();

	void init() override;
	void update(float deltatime) override;
	void draw(LPD3DXSPRITE, Viewport*) override;
	void release() override;

	virtual float checkCollision(BaseObject* object, float dt) override;

	//virtual float checkCollision(BaseObject* object, float dt) override;
	virtual void initWeaponComponent() override;

private:
	int		_damage;
	GVector2 _distance;
	bool	_isFliedBack;
};

