#include "Sprite.h"
_USING_FRAMEWORK

Sprite::Sprite(LPD3DXSPRITE spriteHandle, LPWSTR filePath, int totalFrames, int cols)
{
	_origin = GVector2(0.5f, 0.5f);
	_scale = GVector2(1.0f, 1.0f);
	_zIndex = 1;
	_rotate = 0.0f;

	auto rs = _texture.loadFromFile(spriteHandle, filePath);
	if (rs != D3D_OK)
		throw;

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

	_isDrawBounding = false;
	_surface = nullptr;

	//create surface
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

	//vẽ bounding để xem
	if (_surface == nullptr || _isDrawBounding == false)
		return;
	RECT r;
	 r.top = WINDOW_HEIGHT - _bound.top;
	 r.left = _bound.left;
	 r.bottom = WINDOW_HEIGHT - _bound.bottom;
	 r.right = _bound.right;

	 DeviceManager::getInstance()->getDevice()->ColorFill(_surface, NULL, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

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

void Sprite::drawBounding(bool isDrawBouding)
{
	if (isDrawBouding != _isDrawBounding)
		_isDrawBounding = isDrawBouding;
}
/*
	Độ trong suốt của sprite
*/
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

//Tính bounding cho sprite để  xét va chạm,theo tọa độ decac
void Sprite::updateBounding()
{
	float scaleW = _frameWidth * abs(_scale.x);
	float scaleH = _frameHeight * abs(_scale.y);

	//left và right vẫn giữ nguyên theo trục x
	this->_bound.left = _position.x - scaleW * _origin.x;
	//vì sẽ lấy position là tâm nếu có origin
	//nên top và bottom sẽ dịch lên trên
	//left và right dịch sang bên phải
	//tính từ tọa độ position gốc theo origin
	this->_bound.bottom = _position.y - scaleH * _origin.y;
	this->_bound.right = _bound.left + scaleW;
	this->_bound.top = _bound.bottom + scaleH;

	// 4 điểm của hcn
	GVector2 p1 = GVector2(_bound.left, _bound.top);
	GVector2 p2 = GVector2(_bound.right, _bound.top);
	GVector2 p3 = GVector2(_bound.right, _bound.bottom);
	GVector2 p4 = GVector2(_bound.left, _bound.bottom);
	_anchorPoint = GVector2(_bound.left + scaleW * _origin.x, _bound.bottom + scaleH * _origin.y);

	//rotate 4 điểm
	p1 = rotatePointAroundOrigin(p1, _rotate, _anchorPoint);
	p2 = rotatePointAroundOrigin(p2, _rotate, _anchorPoint);
	p3 = rotatePointAroundOrigin(p3, _rotate, _anchorPoint);
	p4 = rotatePointAroundOrigin(p4, _rotate, _anchorPoint);

	///so sánh các tọa độ để lấy dữ liệu cho bound
	_bound.left = min(min(p1.x, p2.x), min(p3.x, p4.x));
	_bound.top = max(max(p1.y, p2.y), max(p3.y, p4.y));
	_bound.right = max(max(p1.x, p2.x), max(p3.x, p4.x));
	_bound.bottom = min(min(p1.y, p2.y), min(p3.y, p4.y));
}

GVector2	Sprite::rotatePointAroundOrigin(GVector2 point, float angle, GVector2 origin)
{
	// nhân ma trận xoay
	/*
	x' = x.cos(t) - y.sin(t)
	y' = x.sin(t) + y.cos(t)

	t là góc quay theo radian
	vậy quanh quanh 1 điểm mình dời về góc rồi quay xong dời lại
	*/

	GVector2 newPoint;
	//trừ vì sprite xoay với cái này lệch 90*
	float rad = -angle * (3.14 / 180);

	float _sin = sin(rad);
	float _cos = cos(rad);

	//dời điểm về góc
	point -= origin;

	//xoay
	newPoint.x = point.x * _cos - point.y * _sin;
	newPoint.y = point.x * _sin + point.y * _cos;

	//dời về chổ cũ
	newPoint += origin;

	return newPoint;
}
