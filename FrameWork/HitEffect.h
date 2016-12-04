#pragma once

#include "BaseObject.h"
#include "Animation.h"
#include "StopWatch.h"

#define SPEED_ANIMATION			0.08f
class HitEffect : public BaseObject {
public:
	HitEffect(int type = 1, GVector2 pos = GVector2Zero);
	~HitEffect();

	void init();
	void update(float deltatime);
	void draw(LPD3DXSPRITE, Viewport*);
	void release();

	void setPosition(GVector2 position);
	void setTimeAnimated(float);
private:
	Animation* _animation;
	int _type;
	StopWatch _animationStopWatch;
};
