#pragma once
#include "BaseObject.h"
class EmptyObject :
	public BaseObject
{
public:
	EmptyObject(eID id, GVector2 pos, int width, int height);
	~EmptyObject();

protected:
	RECT _bound;
};

