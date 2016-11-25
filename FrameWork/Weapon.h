#pragma once
#include"Item.h"
#include"ActiveWeapon.h"
class Weapon : public Item
{
public:
	/*
	Khởi tạo Item:
		@startPosition : vị trí bắt đầu
		@type: Loại item,rớt ra hoặc bị lượm lên
		@dir : Hướng của item khi bị ném ra
		+ Để cho item rớt ra thì sử dụng @type = DROP	
			Sau đó sử dụng ItemManager.insertItem((Item*)) để render
			Khi va chạm với simon thì trả về trạng thái DESTROY đồng
			thời gọi ActiveWeapon::setItemID(idweapon)
		+ Nếu simon sử dụng weapon thì dùng trạng thái PickedUp
			các hàm sử lý va chạm sẽ được override để xử lý riêng cho từng đối tượng
			sau đó add vào ItemManager.insertItem((Item*))

			Xử lý va chạm sẽ xử lý trên weapon
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
	virtual GVector2 initVeloc(GVector2 speed) override = 0;

	/*
		kiểm tra va chạm
	*/
	virtual float checkCollision(BaseObject* object, float dt) override = 0;
	/*
		dùng cho các lớp con để khởi tạo các component riêng
	*/
	virtual void initWeaponComponent()  = 0;

	virtual void pickedUp() override;
protected:
	eDirection	_direction;
	eItemID		_itemId;
};

