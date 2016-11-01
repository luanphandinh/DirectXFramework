
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


#endif // !__ICOMPONENT_H__