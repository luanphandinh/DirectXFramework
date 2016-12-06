#include "Stair.h"


Stair::Stair(int x, int y, int width, int height, eDirection physicalBodyDirection, eStairDirection direction, bool _canUpStair) :
Land(x, y, width, height,false, physicalBodyDirection)
{
	this->_canStandOnStair = true;
	this->_id = eID::STAIR;
	this->_stairDirection = direction;
	this->_canUpStair = _canUpStair;
}


Stair::~Stair()
{
	
}

void Stair::enableStandOnStair(bool enable)
{
	if (this->_canStandOnStair != enable)
		_canStandOnStair = enable;
}

bool Stair::canStandOnStair()
{
	return _canStandOnStair;
}

void Stair::setStairDirection(eStairDirection direction)
{
	if (this->_stairDirection != direction)
		_stairDirection = direction;
}

eStairDirection Stair::getStairDirection()
{
	return _stairDirection;
}

bool  Stair::canUpStair()
{
	return _canUpStair;
}