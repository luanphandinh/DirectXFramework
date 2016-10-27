#pragma once

#include"define.h"
#include"DeviceManager.h"
#include"Viewport.h"
_NAMESPACE_FRAMEWORK_BEGIN
class Texture
{
public:
	Texture();
	~Texture();

	//giải phóng image từ bộ nhớ
	void release();
	/*load image từ file rồi lưu trong LPDIRECT3DTEXTURE9
		@spriteHandle : sprite handle được quản lý vởi directx,dùng để load content
		@filePath : đường dẫn của file
		@colora : transparent color
	*/
	HRESULT loadFromFile(LPD3DXSPRITE spriteHandle, LPWSTR filePath, D3DXCOLOR color = C_WHITE, D3DXCOLOR colorkey = COLOR_KEY);

	/*
		vẽ LPDIRECT3DTEXTURE9 lên screen,directX quản lý việc vẽ vào buffer
		@spriteHandle : spriteHandle được quản lý bởi directX,sử dụng để load content
		@rect : phần hình chữ nhật trên cái image mà mình cần vẽ
		@center :  tâm của frame cần được vẽ:
			-nếu là NULL thì topleft của image sẽ được vẽ ở @postion
			-nếu chọng x,y của frame ,topleft của image sẽ được dịch @postion - (x,y) sau đó vẽ lên screen
		@position : vị trí được vẽ
	*/
	void render(LPD3DXSPRITE spriteHandle, const RECT* rect, const GVector3* center, const GVector3* position);

	/*
	render texture với các tham số tranform, gốc tọa độ top-left
	@position: vị trí
	@scale:
	@rotate: góc xoay theo số độ (0-360)
	@origin: điểm neo để transform
	@zIndex: độ sâu
	*/
	void render(LPD3DXSPRITE spriteHandle, RECT* srcRect,
		GVector2 position, GVector2 scale, float rotate, GVector2 origin, float zIndex = 0);

	/*
		render texture với các tham số transform trong viewport
		@viewport:
		@position: vị trí
		@scale:
		@rotate: góc xoay theo số độ (0-360)
		@origin: điểm neo để transform
		@zIndex: độ sâu
	*/
	void render(LPD3DXSPRITE spriteHandle, RECT* srcRect,
		Viewport viewport, GVector2 position, GVector2 scale, float rotate, GVector2 origin, float zIndex = 0);

	void setColor(D3DXCOLOR);

	D3DXCOLOR getColor();

	//get width of image
	int getWidth();
	//get height of image
	int getHeight();
private:
	LPDIRECT3DTEXTURE9	_texture;	//giữ image loaded từ file
	D3DXIMAGE_INFO		_imageInfo;	//giữ info như width,height của image loaded từ file

	D3DCOLOR			_color;		//hinhfd dược load có backcolor,backcolor sẽ bị bỏ qua khi draw
};

typedef Texture* pTexture;

_NAMESPACE_FRAMEWORK_END

