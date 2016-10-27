#include "Sprite.h"
_USING_FRAMEWORK

Sprite::Sprite(LPD3DXSPRITE spriteHandle, LPWSTR filePath, int totalFrames, int cols)
{
	//Lấy vào trung tâm của hình
	_origin = GVector2(0.5f, 0.5f);
	//ko scale
	_scale = GVector2(1.0f, 1.0f);
	_zIndex = 1;
	//ko xoay
	_rotate = 0.0f;

	//load texture lên
	auto rs = _texture.loadFromFile(spriteHandle, filePath);
	if (rs != D3D_OK)
		throw;
	//Tổng số frame để chạy
	_totalFrames = totalFrames;
	_columns = cols;
	_textureWidth = _texture.getWidth();
	_textureHeight = _texture.getHeight();
	_frameWidth = _textureWidth / cols;
	_frameHeight = _textureHeight * cols / totalFrames;
	_index = 0;
	_currentFrame = GVector2(0, 0);

	this->setIndex(0);
	this->updateBounding();

	//ko vẽ bounding và ko có surface
	_isDrawBounding = false;
	_surface = nullptr;


	//Create surface
	DeviceManager::getInstance()->getDevice()->CreateOffscreenPlainSurface(
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		D3DFMT_X8R8G8B8,
		D3DPOOL_DEFAULT,
		&_surface,
		NULL
		);
}


Sprite::~Sprite()
{
}


void Sprite::release()
{
	this->_texture.release();
}

/*
vẽ hình
*/
void Sprite::render(LPD3DXSPRITE spriteHandler)
{
	_texture.render(
		spriteHandler,
		&_frameRect,
		GVector2((int)_position.x, (int)_position.y),
		_scale,
		_rotate,
		_origin,
		_zIndex
		);
}

/*
vẽ hình với viewport
*/
void Sprite::render(LPD3DXSPRITE spriteHandler, Viewport * viewport)
{
	_texture.render(
		spriteHandler,
		&_frameRect,
		*viewport,
		_position,
		_scale,
		_rotate,
		_origin,
		_zIndex
		);

	//vẽ bouding để xem
	if (_surface == nullptr || _isDrawBounding == false)
		return;
	RECT r;
	 r.top = WINDOW_HEIGHT - _bound.top;
	 r.left = _bound.left;
	 r.bottom = WINDOW_HEIGHT - _bound.bottom;
	 r.right = _bound.right;

	 DeviceManager::getInstance()->getDevice()->ColorFill(_surface, NULL, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f));

	 DeviceManager::getInstance()->getDevice()->StretchRect(
		 _surface,
		 NULL,
		 DeviceManager::getInstance()->getSurface(),
		 &r,
		 D3DTEXF_NONE
		 ); 

}

//RECT Sprite::getBounding(){}
//void Sprite::setFrameRect(RECT rect){}
//void Sprite::setFrameRect(float top, float left, float bottom, float right){}
//void Sprite::setFrameRect(float x, float y, int width, int height){}
//RECT Sprite::getFrameRect(){}
//RECT Sprite::getFrameRectByIndex(int index){}
///*
//chuyển sang frame kế tiếp
//*/
//void Sprite::nextFrame(){}
//
///*
//Truyển thứ từ cho frame cụ thể
//*/
//void Sprite::setIndex(int index){}
//
///*
//Lấy chiều dọc của frame
//*/
//int getFrameHeight(){}
//
///*
//Lấy chiều ngang của frame
//*/
//int Sprite::getFrameWidth(){}
//
///*
//Lấy chiều ngang của nguyên tấm hình
//*/
//int getTextureWidth(){}
//
///*
//Lấy chiều dọc của nguyên tấm hình
//*/
//int Sprite::getTextureHeight(){}
//
//void Sprite::drawBounding(){}
//void Sprite::setOpacity(){}
//float Sprite::getOpacity(){}
//
//
///*
//Màu vẽ sprite
//*/
//void Sprite::setColor(D3DXCOLOR color){}
//D3DCOLOR	Sprite::getColor(){}