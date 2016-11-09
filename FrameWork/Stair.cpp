#include "Stair.h"


Stair::Stair(int x, int y, int width, int height, eDirection physicalBodyDirection):
		Land(x,y,width,height,physicalBodyDirection)
{

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
