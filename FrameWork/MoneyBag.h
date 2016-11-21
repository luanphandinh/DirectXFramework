#pragma once
#include"Item.h"
#include"Score.h"
class MoneyBag : public Item
{
public:
	MoneyBag(GVector2 startPosition);
	~MoneyBag();

	void init() override;
	void update(float deltatime) override;
	void draw(LPD3DXSPRITE, Viewport*) override;
	void release() override;

	virtual float checkCollision(BaseObject* object, float dt) override;
private:
	eMoneyBagItemType _moneyBagType;
};

