#pragma once

#include "BaseObject.h"
#include "Animation.h"

#define SPEED_ANIMATION			0.07f
class Burning : public BaseObject {
public:
	Burning(int type);
	~Burning();

	void init();
	void update(float deltatime);
	void draw(LPD3DXSPRITE, Viewport*);
	void release();

	void setPosition(GVector2 position);
	void setTimeAnimated(float);
private:
	Animation* _animation;
	int _type;
};
