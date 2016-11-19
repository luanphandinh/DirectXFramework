#pragma once
#include"Item.h"
class HeartItem :
	public Item
{
public:
	HeartItem(GVector2 startPosition);
	~HeartItem();

	void init() override;
	void update(float deltatime) override;
	void draw(LPD3DXSPRITE, Viewport*) override;
	void release() override;
private:
	int type;
};

