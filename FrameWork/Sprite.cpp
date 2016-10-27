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
//===============================================================//
void Sprite::setPosition(float x, float y, float z)
{
	GVector3 v(x, y, z);
	this->setPosition(v);
}

void Sprite::setPosition(GVector3 vector)
{
	this->_position = GVector2(vector.x, vector.y);
	this->updateBounding();
}

void Sprite::setPosition(GVector2 position)
{
	this->_position = GVector2(position.x, position.y);
	this->updateBounding();
}

void Sprite::setPositionX(float x)
{
	if (x == _position.x)
		return;

	_position.x = x;
	this->updateBounding();
}

void Sprite::setPositionY(float y)
{
	if (y == _position.y)
		return;

	_position.y = y;
	this->updateBounding();
}

void Sprite::setScale(GVector2 scale)
{
	if (scale == _scale)
		return;

	_scale = scale;
	this->updateBounding();
}

void Sprite::setScale(float scale)
{
	if (scale != _scale.x || scale != _scale.y)
	{
		_scale.x = scale;
		_scale.y = scale;
		this->updateBounding();
	}
}

void Sprite::setScaleX(float sx)
{
	if (sx == _scale.x)
		return;

	_scale.x = sx;
	this->updateBounding();
}

void Sprite::setScaleY(float sy)
{
	if (sy == _scale.y)
		return;

	_scale.y = sy;
	this->updateBounding();
}

void Sprite::setRotate(float degree)
{
	if (degree == _rotate)
		return;

	_rotate = degree;
	this->updateBounding();
}

void Sprite::setOrigin(GVector2 origin)
{
	if (origin == _origin)
		return;

	_origin = origin;
	//điểm neo
	//ở đây ta lấy vào điểm giữa của _bound
	_anchorPoint = GVector2(_bound.left + _frameWidth * _scale.x * _origin.x, _bound.bottom + _frameHeight * _scale.y * _origin.y);

	this->updateBounding();
}

void Sprite::setZIndex(float z)
{
	if (z != _zIndex)
		_zIndex = z;
}
//===============================================================//

RECT Sprite::getBounding()
{
	return _bound;
}


void Sprite::setFrameRect(RECT rect)
{
	_frameRect = rect;

	_frameWidth = abs(_frameRect.left - _frameRect.right);
	_frameHeight = abs(_frameRect.top - _frameRect.bottom);

	this->updateBounding();
}

void Sprite::setFrameRect(float top, float left, float bottom, float right)
{
	_frameRect.top = top;
	_frameRect.left = left;
	_frameRect.right = right;
	_frameRect.bottom = bottom;

	_frameWidth = abs(_frameRect.left - _frameRect.right);
	_frameHeight = abs(_frameRect.top - _frameRect.bottom);

	this->updateBounding();
}

void Sprite::setFrameRect(float x, float y, int width, int height)
{
	_frameRect.top = y;
	_frameRect.left = x;
	_frameRect.right = x + width;
	_frameRect.bottom = y + height;

	_frameWidth = abs(_frameRect.left - _frameRect.right);
	_frameHeight = abs(_frameRect.top - _frameRect.bottom);

	this->updateBounding();
}
RECT Sprite::getFrameRect()
{
	return _frameRect;
}

RECT Sprite::getFrameRectByIndex(int index)
{
	//phép chia lấy phần dư để lấy index
	index = index % _totalFrames;
	RECT rect;
	rect.left = (long)_currentFrame.x * _frameWidth;
	rect.right = _frameRect.left + _frameWidth;
	rect.top = (long)_currentFrame.y * _frameHeight;
	rect.bottom = _frameRect.top + _frameHeight;
	return rect;
}
/*
chuyển sang frame kế tiếp
*/
void Sprite::nextFrame()
{
	if (_totalFrames <= 1)
		return;

	this->setIndex(_index + 1);
}

/*
Truyển thứ từ cho frame cụ thể
*/
void Sprite::setIndex(int index)
{
	if (_index != index)
		_index = index;

	this->setCurrentFrame();
}

/*
Lấy chiều dọc của frame
*/
int Sprite::getFrameHeight()
{
	return _frameHeight * abs(_scale.y);
}

/*
Lấy chiều ngang của frame
*/
int Sprite::getFrameWidth()
{
	return _frameWidth * abs(_scale.x);
}

/*
Lấy chiều ngang của nguyên tấm hình
*/
int Sprite::getTextureWidth()
{
	return _textureWidth;
}

/*
Lấy chiều dọc của nguyên tấm hình
*/
int Sprite::getTextureHeight()
{
	return _textureHeight;
}

void Sprite::drawBounding()
{

}

void Sprite::setOpacity(float opacity)
{
	if (_opacity == opacity)
		return;

	_opacity = opacity;
	_texture.setColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, _opacity));
}

float Sprite::getOpacity()
{
	return _opacity;
}


/*
Màu vẽ sprite
*/
void Sprite::setColor(D3DXCOLOR color)
{
	_color = color;
	_texture.setColor(_color);
}

D3DCOLOR Sprite::getColor()
{
	return _color;
}

void Sprite::setFrameRect()
{
	this->_frameRect.top = (long)_currentFrame.y * _frameHeight;
	this->_frameRect.bottom = _frameRect.top + _frameHeight;
	this->_frameRect.left = (long)_currentFrame.x * _frameWidth;
	this->_frameRect.right = _frameRect.left + _frameWidth;
	
}

void Sprite::setCurrentFrame()
{
	if (_index >= _totalFrames)
		_index = _index % _totalFrames;

	this->_currentFrame.x = _index % _columns;
	this->_currentFrame.y = _index / _columns;

	this->setFrameRect();
}

void Sprite::updateBounding()
{

}
//
//GVector2	Sprite::rotatePointAroundOrigin(GVector2 point, float angle, GVector2 origin)
//{
//
//}
