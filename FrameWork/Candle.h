#pragma once
#include"BaseObject.h"
#include"Animation.h"
#include"SpriteManager.h"
class Candle : public BaseObject
{
public:
	Candle(GVector2 pos,eItemID _dropItemId);
	~Candle();

	void init() override;
	void update(float deltaTime) override;
	void draw(LPD3DXSPRITE, Viewport*) override;
	void release() override;

private:
	map<eStatus, Animation*> _animations;
	eItemID		_dropItemId;
};

