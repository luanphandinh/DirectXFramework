#pragma once
#include"define.h"
#include"BaseObject.h"
class Land :public BaseObject
{
public:
	Land(int x,int y,int width,int height,bool checkable,eDirection physicalBodyDirection);
	~Land();

	//implement inherited methods from baseObject
	void init() override;
	void update(float deltaTime) override;
	void draw(LPD3DXSPRITE, Viewport*) override;
	void release() override;

	GVector2 getPosition() override;
	void setPosition(GVector2 position) override;

	GVector2 getOrigin() override;

	RECT getBounding() override;

	void enableJump(bool enable);
	bool canJump();
	bool checkable();
protected:
	bool _canJump;
	LPDIRECT3DSURFACE9 _surface;
#pragma region Hide all
	// Đặt những hàm này ở private để giấu chúng đi. Vì lớp này có sprite bằng NULL nên hạn chế những tác động đến sprite.
	float getPositionX() override;
	float getPositionY() override;
	void setPosition(GVector3 vector) override;
	void setPosition(float x, float y, float z) override;
	void setPosition(float x, float y) override;
	void setPositionX(float x) override;
	void setPositionY(float y) override;
	void setOrigin(GVector2 origin) override;

	GVector2 getScale() override;
	void setScale(GVector2 scale) override;
	void setScale(float scale) override;
	void setScaleX(float sx) override;
	void setScaleY(float sx) override;

	float getRotate() override;
	void setRotate(float degree) override;
	GVector2 getAnchorPoint() override;

	void setZIndex(float z) override;
	float getZIndex() override;
	bool _checkable;
	
#pragma endregion
private:
	// Hình chữ nhật bao lấy land.
	RECT _bound;
};

