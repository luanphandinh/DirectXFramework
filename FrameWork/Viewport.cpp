﻿#include "Viewport.h"


Viewport::Viewport()
{
}

//gán tọa đọ camera trên thế giới world
Viewport::Viewport(float x, float y)
{
	_positionWorld.x = x;
	_positionWorld.y = y;
}

Viewport::Viewport(float x, float y, float width, float height)
{
	_positionWorld.x = x;
	_positionWorld.y = y;
	this->_width = width;
	this->_height = height;
}

Viewport::~Viewport()
{

}

void Viewport::setPositionWorld(GVector2 position)
{
	if (_positionWorld != position)
		_positionWorld = position;
}
GVector2 Viewport::getPositionWorld()
{
	return _positionWorld;
}

float Viewport::getWidth()
{
	return _width;
}

float Viewport::getHeight()
{
	return _height;
}

//lấy position của object vào trong viewport
GVector3 Viewport::getPositionInViewport(GVector3* position)
{
	D3DXMATRIX mt;
	D3DXVECTOR4 posViewport;

	D3DXMatrixIdentity(&mt);
	mt._22 = -1.0f;
	mt._41 = (-1) * _positionWorld.x;
	mt._42 = _positionWorld.y;

	D3DXVec3Transform(&posViewport, position, &mt);

	return GVector3(posViewport.x, posViewport.y, posViewport.z);
}

//kiểm tra hcn có nằm trong màn hình hay không
bool Viewport::isContains(const RECT &rect)
{
	if (rect.right < _positionWorld.x)
		return false;
	if (rect.bottom > _positionWorld.y)
		return false;
	if (rect.left > _positionWorld.x + _width)
		return false;
	if (rect.top < _positionWorld.y - _height)
		return false;
	return true;
}

//lấy kích thước vủa viewport bằng HCN,tính tỏng world lớn
RECT Viewport::getBounding()
{
	RECT rect;

	rect.left = _positionWorld.x;
	rect.top = _positionWorld.y;
	rect.bottom = rect.top - _height;
	rect.right = rect.left + _width;

	return rect;
}

