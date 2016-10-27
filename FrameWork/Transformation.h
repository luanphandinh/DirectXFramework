#pragma once
#include"define.h"
_NAMESPACE_FRAMEWORK_BEGIN
/*
	Được dùng cho Sprite để biểu thị toa độ
	zoom xoay các kiểu
*/
class Transformation
{
public:
	Transformation();
	~Transformation();
protected:
	GVector2	_position;
	GVector2	_scale;
	float		_rotate;//0-360
	GVector2	_origin;//gốc của transform đùng để xoay,scale
	GVector2	_anchorPoint;
	float		_xIndex;

};
_NAMESPACE_FRAMEWORK_END
