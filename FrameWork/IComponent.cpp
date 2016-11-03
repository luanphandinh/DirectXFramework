#include"IComponent.h"
#pragma region Movement
Movement::Movement(GVector2 accel, GVector2 veloc, Sprite* refSprite)
{
	this->_accelerate = accel;
	this->_velocity = veloc;
	this->_refSprite = refSprite;
}

void Movement::update(float deltatime) 
{
	if (_refSprite == NULL)
		return;
	GVector2 position = _refSprite->getPosition();
	this->_velocity += this->_accelerate * deltatime / 1000;
	position += this->_velocity * deltatime / 1000;
	this->_refSprite->setPosition(position);
}

void Movement::setAccelerate(GVector2 accel)
{
	if (this->_accelerate != accel)
		this->_accelerate = accel;
}
void Movement::setVelocity(GVector2 veloc)
{
	if (this->_velocity != veloc)
		this->_velocity = veloc;
}

GVector2 Movement::getAccelerate()
{
	return this->_accelerate;
}
GVector2 Movement::getVelocity()
{
	return this->_velocity;
}
#pragma endregion
#pragma region Gravity
Gravity::Gravity(GVector2 gravity, Movement *movement)
{
	this->_gravity = gravity;
	this->_refMovement = movement;
	this->_status = FALLING_DOWN;
}

void Gravity::update(float deltatime)
{
	switch (_status)
	{
	case FALLING_DOWN:
		this->_additionalVeloc = this->_gravity * deltatime / 1000;
		break;
	case SHALLOWED:
		this->_additionalVeloc = GVector2Zero;
		break;
	default:
		break;
	}

	auto veloc = this->_refMovement->getVelocity();
	this->_refMovement->setVelocity(veloc + _additionalVeloc);
}

void Gravity::setStatus(eGravityStatus status)
{
	if (this->_status != status)
		_status = status;
}

void Gravity::setGravity(GVector2 gravity)
{
	if (this->_gravity != gravity)
		_gravity = gravity;
}
#pragma endregion