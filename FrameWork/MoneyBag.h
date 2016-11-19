#pragma once
#include"Item.h"
class MoneyBag : public Item
{
public:
	MoneyBag(GVector2 startPosition);
	~MoneyBag();

	void init() override;
	void update(float deltatime) override;
	void draw(LPD3DXSPRITE, Viewport*) override;
	void release() override;
private:
	eMoneyBagItemType _moneyBagType;
};

