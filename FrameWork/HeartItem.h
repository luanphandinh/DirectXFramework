#pragma once
#include"Item.h"
#include"HeartCounter.h"
class HeartItem :
	public Item
{
public:
	HeartItem(GVector2 startPosition,eItemID itemId = eItemID::SMALLHEART);
	~HeartItem();

	void init() override;
	void update(float deltatime) override;
	void draw(LPD3DXSPRITE, Viewport*) override;
	void release() override;

	virtual void pickedUp() override;
private:
	eHeartItemType _heartType;
};

