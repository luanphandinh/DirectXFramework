#pragma once

#include"define.h"
#include"DeviceManager.h"
#include"Texture.h"
#include"Viewport.h"
#include"Transformable.h"

_NAMESPACE_FRAMEWORK_BEGIN
/*
	+ Lớp này được kế thừa từ lớp Transformable sỡ hữu các 
		tuộc tính cơ bản như postion,scale,orgin....
	+ Lớp có hàm render chứa texture để render theo viewport
	+ Có frameRect với frameWidth,frameHeight để vẽ lên màn hình
	+ Gồm các frame(rect) trên texture được đánh index và có hàm update 
	để tạo hiệu ứng chuyển động
*/
class Sprite : public Transformable
{
public:
	/*
		@filePath : đường dẫn hình ảnh
		@totalFrames : số frame ảnh trên 1 tiles
		@cols : số frame trên một dòng
		Mặc định số frae và cols là 1 vẽ hết hình.
		Để vẽ 1 vị trí cụ thể trên hình thì dùng setFrameRect().
	*/
	Sprite(LPD3DXSPRITE spriteHandle, LPWSTR filePath, int totalFrames = 1, int cols = 1);
	~Sprite();

	void release();
	//override methods của transformable
	//======================================//

	void setPosition(float x, float y, float z = 1);
	void setPosition(GVector3 vector);
	void setPosition(GVector2 position);
	void setPositionX(float x);
	void setPositionY(float y);

	void setScale(GVector2 scale);
	void setScale(float scale);
	void setScaleX(float sx);
	void setScaleY(float sy);

	void setRotate(float degree);

	void setOrigin(GVector2 origin);

	void setZIndex(float z);

	//======================================//

	/*
		vẽ hình
	*/
	void render(LPD3DXSPRITE spriteHandler);

	/*
		vẽ hình với viewport
	*/
	void render(LPD3DXSPRITE spriteHandler, Viewport * viewport);

	/*
		getBounding để xử lý va chạm theo decac
	*/
	RECT getBounding();
	/*
		setFrameRect để vẽ
	*/
	void setFrameRect(RECT rect);
	void setFrameRect(float top, float left, float bottom, float right);
	void setFrameRect(float x, float y, int width, int height);
	RECT getFrameRect();
	RECT getFrameRectByIndex(int index);
	/*
		chuyển sang frame kế tiếp
	*/
	void nextFrame();

	/*
		Truyển thứ từ cho frame cụ thể
	*/
	void setIndex(int index);

	/*
		Lấy chiều dọc của frame
	*/
	int getFrameHeight();

	/*
		Lấy chiều ngang của frame
	*/
	int getFrameWidth();

	/*
		Lấy chiều ngang của nguyên tấm hình
	*/
	int getTextureWidth();

	/*
		Lấy chiều dọc của nguyên tấm hình
	*/
	int getTextureHeight();

	void drawBounding();
	void setOpacity(float _opacity);
	float getOpacity();

	
	/*
		Màu vẽ sprite
	*/
	void setColor(D3DXCOLOR color);
	D3DCOLOR	getColor();

private:
	Texture			_texture;
	float			_opacity;//0.0f-1.0f
	D3DXCOLOR		_color;

	RECT			_bound;

	int				_totalFrames;
	int				_columns;
	int				_index;
	GVector2		_currentFrame;//frame hiện tại theo hàng x và cột y

	RECT			_frameRect;
	int				_frameWidth;
	int				_frameHeight;
	int				_textureWidth;
	int				_textureHeight;

	void setFrameRect();
	void setCurrentFrame();
	void updateBounding();

	GVector2	rotatePointAroundOrigin(GVector2 point,float angle,GVector2 origin);

	//surface để vẽ lên bounding
	LPDIRECT3DSURFACE9	_surface;
	bool				_isDrawBounding;
};
_NAMESPACE_FRAMEWORK_END

