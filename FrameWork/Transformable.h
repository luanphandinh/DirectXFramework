#pragma once
#include"define.h"
_NAMESPACE_FRAMEWORK_BEGIN
	/*
	Được dùng cho Sprite để biểu thị toa độ
	zoom xoay các kiểu
	*/
class Transformable
{
public:
	Transformable();
	~Transformable();	
	
	virtual GVector2 getPosition();
	virtual float getPositionX();
	virtual float getPositionY();

	virtual void setPosition(GVector3 vector);
	virtual void setPosition(float x, float y, float z);
	virtual void setPosition(GVector2 position);
	virtual void setPosition(float x, float y);
	virtual void setPositionX(float x);
	virtual void setPositionY(float y);

	virtual GVector2 getScale();
	virtual void setScale(GVector2 scale);
	virtual void setScale(float scale);
	virtual void setScaleX(float sx);
	virtual void setScaleY(float sy);

	virtual float getRotate();
	virtual void setRotate(float degree);

	virtual GVector2 getOrigin();
	virtual void setOrigin(GVector2 origin);

	virtual void setZIndex(float z);
	virtual float getZIndex();

	GVector2 getAnchorPoint();

protected:
	GVector2	_position;
	GVector2	_scale;
	float		_rotate;//0-360
	GVector2	_origin;//gốc của transform đùng để xoay,scale
	GVector2	_anchorPoint;
	float		_zIndex;

};
_NAMESPACE_FRAMEWORK_END



