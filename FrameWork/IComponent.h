
#ifndef __ICOMPONENT_H__
#define __ICOMPONENT_H__
#include "define.h"
#include "Sprite.h"
#include <numeric>
#include <map>
using namespace std;
_USING_FRAMEWORK // = using namespace framework
/*
	Base Component
	Các hiệu ứng ảnh hưởng đến object suốt game thì kế thừa lại
*/
class IComponent
{
public:
	virtual void update(float deltatime) = 0;
};

class Movement : public IComponent
{
public:
	Movement(GVector2 accel, GVector2 veloc, Sprite* refSprite);
	void update(float deltatime) override;

	void setAccelerate(GVector2 accel);
	void setVelocity(GVector2 veloc);
	GVector2 getAccelerate();
	GVector2 getVelocity();
private:
	GVector2 _accelerate;
	GVector2	_velocity;
	Sprite* _refSprite;
};

#endif // !__ICOMPONENT_H__