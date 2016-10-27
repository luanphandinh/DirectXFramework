#pragma once

#include"define.h"
#include"DeviceManager.h"
#include"Texture.h"
#include"Viewport.h"
#include"Transformable.h"

_NAMESPACE_FRAMEWORK_BEGIN

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

	/*
		vẽ hình
	*/
	void render(LPD3DXSPRITE spriteHandler);

	/*
		vẽ hình với viewport
	*/
	void render(LPD3DXSPRITE spriteHandler, Viewport * viewport);

	RECT getBounding();
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
	void setOpacity();
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
	GVector2		_currentFrame;//frame hiện tại

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

