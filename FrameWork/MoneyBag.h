#pragma once
#include"Item.h"
#include"Score.h"
class MoneyBag : public Item
{
public:
	MoneyBag(GVector2 startPosition,eItemID id = eItemID::MONEYBAGRED);
	~MoneyBag();

	void init() override;
	void update(float deltatime) override;
	void draw(LPD3DXSPRITE, Viewport*) override;
	void release() override;

	virtual void pickedUp() override;
private:
	eMoneyBagItemType _moneyBagType;
};

