#include "Stair.h"


Stair::Stair(int x, int y, int width, int height, eDirection physicalBodyDirection,eStairDirection direction):
	Land(x, y, width, height, physicalBodyDirection)
{
	this->_canStandOnStair = true;
	this->_id = eID::STAIR;
	this->_stairDirection = direction;
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