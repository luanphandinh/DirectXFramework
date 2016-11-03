#include "Land.h"


Land::Land(int x, int y, int width, int height, eDirection physicalBodyDirection) :BaseObject(eID::LAND)
{
	this->_bound.left = x;
	this->_bound.top = y;
	this->_bound.bottom = _bound.top - height;
	this->_bound.right = _bound.left + width;
	setPhysicBodySide(physicalBodyDirection);

	_canJump = false;

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


Land::~Land()
{
}


void Land::init()
{
	this->_sprite = nullptr;
}

void Land::update(float deltaTime)
{}

void Land::draw(LPD3DXSPRITE spriteHandle, Viewport* viewport)
{

	RECT r;
	auto pos = viewport->getPositionInViewport(new GVector3(getPositionX(), getPositionY(), 0));
	r.top = max(pos.y, 1);
	r.left = max(pos.x, 1);
	r.bottom = min(pos.y + _bound.top - _bound.bottom, WINDOW_HEIGHT - 1);
	r.right = min(pos.x + _bound.right - _bound.left, WINDOW_WIDTH - 1);

	DeviceManager::getInstance()->getDevice()->ColorFill(_surface, NULL, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f));

	DeviceManager::getInstance()->getDevice()->StretchRect(
		_surface,
		NULL,
		DeviceManager::getInstance()->getSurface(),
		&r,
		D3DTEXF_NONE
		);
}

void Land::release()
{

}

GVector2 Land::getPosition()
{
	return GVector2(_bound.left, _bound.right);
}

void Land::setPosition(GVector2 position)
{
	_bound.left = position.x;
	_bound.top = position.y;
}

GVector2 Land::getOrigin()
{
	return GVector2Zero;
}

RECT Land::getBounding()
{
	return this->_bound;
}

void Land::enableJump(bool enable)
{
	if (this->_canJump != enable)
		this->_canJump = enable;
}

bool Land::canJump()
{
	return _canJump;
}

float Land::getPositionX()
{
	return this->_bound.left;
}

float Land::getPositionY() 
{
	return this->_bound.top;
}

void Land::setPosition(GVector3 vector) 
{
	_bound.left = vector.x;
	_bound.top = vector.y;
}

void Land::setPosition(float x, float y, float z) 
{
	_bound.left = x;
	_bound.top = y;

}

void Land::setPosition(float x, float y) 
{
	_bound.left = x;
	_bound.top = y;
}

void Land::setPositionX(float x) 
{
	_bound.left = x;
}

void Land::setPositionY(float y) 
{
	_bound.top = y;
}

void Land::setOrigin(GVector2 origin) {}

GVector2 Land::getScale() 
{
	return GVector2Zero;
}

void Land::setScale(GVector2 scale) 
{

}

void Land::setScale(float scale) 
{

}

void Land::setScaleX(float sx) 
{

}

void Land::setScaleY(float sx) 
{

}

float Land::getRotate() 
{
	return 0.0f;
}

void Land::setRotate(float degree) 
{

}

GVector2 Land::getAnchorPoint() 
{
	return GVector2(_bound.left, _bound.right);;
}

void Land::setZIndex(float z)
{

}

float Land::getZIndex()
{
	return 1.0f;
}