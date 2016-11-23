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
	Weapon(GVector2 startPosition, eItemType type,eDirection dir,eItemID id);
	~Weapon();

	virtual void init() = 0;
	virtual void update(float deltatime) = 0;
	virtual void draw(LPD3DXSPRITE, Viewport*) = 0;
	virtual void release() = 0;

	/*
		override lại hàm initVeloc cho các lớp kế thừa cho riêng từng loại vũ khí
	*/
	virtual GVector2 initVeloc(float speed) override = 0;

	/*
		kiểm tra va chạm
	*/
	virtual float checkCollision(BaseObject* object, float dt) override;
	/*
		dùng cho các lớp con để khởi tạo các component riêng
	*/
	virtual void initWeaponComponent()  = 0;
protected:
	eDirection	_direction;
	eItemID		_itemId;
};

