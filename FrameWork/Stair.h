#pragma once
#include "Land.h"
class Stair :
	public Land
{
public:
	Stair(int x, int y, int width, int height, eDirection physicalBodyDirection,eStairDirection direction = 
		eStairDirection::LEFTBOTTOM_TO_RIGHTTOP,bool _canUpStair = true);
	~Stair();

	void enableStandOnStair(bool enable);
	bool canStandOnStair();
	bool canUpStair();
	void setStairDirection(eStairDirection direction);
	eStairDirection getStairDirection();
protected:
	bool _canStandOnStair;
	bool _canUpStair;
	eStairDirection _stairDirection;
};

