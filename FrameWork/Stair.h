#pragma once
#include "Land.h"
class Stair :
	public Land
{
public:
	Stair(int x, int y, int width, int height, eDirection physicalBodyDirection);
	~Stair();

	void enableStandOnStair(bool enable);
	bool canStandOnStair();
protected:
	bool _canStandOnStair;
};

