#pragma once
#include"define.h"
///
///
///
class Viewport
{
public:
	Viewport();
	Viewport(float x, float y);
	Viewport(float x, float y, float width, float height);
	~Viewport();

	void setPositionWorld(GVector2 position);
	GVector2 getPositionWorld();

	float getWidth();
	float getHeight();

	//lấy position của object vào trong viewport
	GVector3 getPositionInViewport(GVector3* position);

	//kiểm tra hcn có nằm trong màn hình hay không
	bool isContains(const RECT &rect);

	//lấy kích thước vủa viewport bằng HCN,tính tỏng world lớn
	RECT getBounding();

private:
	GVector2 _positionWorld;
	float _width;
	float _height;
};

