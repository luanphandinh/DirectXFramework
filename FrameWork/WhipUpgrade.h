#pragma once
#include"Item.h"
class WhipUpgrade : public Item
{
public:
	WhipUpgrade(GVector2 startPosition);
	~WhipUpgrade();

	void init() override;
	void update(float deltatime) override;
	void draw(LPD3DXSPRITE, Viewport*) override;
	void release() override;
private:
	int _levelUpgrade;
};

