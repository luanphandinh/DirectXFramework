#pragma once
#include"Item.h"
class Weapon : public Item
{
public:
	/*
	Khởi tạo Item:
		@startPosition : vị trí bắt đầu
		@type: Loại item,rớt ra hoặc bị lượm lên
		@dir : Hướng của item khi bị ném ra
	*/
	Weapon(GVector2 startPosition, eItemType type,eDirection dir);
	~Weapon();

	virtual void init() = 0;
	virtual void update(float deltatime) = 0;
	virtual void draw(LPD3DXSPRITE, Viewport*) = 0;
	virtual void release() = 0;

	virtual GVector2 initVeloc(float speed) override;
	virtual float checkCollision(BaseObject* object, float dt) override;
	virtual void initWeaponComponent()  = 0;
protected:
	eDirection	_direction;
};

