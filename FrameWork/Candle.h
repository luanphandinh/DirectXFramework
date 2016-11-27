#pragma once
#include"BaseObject.h"
#include"Animation.h"
#include"SpriteManager.h"
#include"CollisionBody.h"
#include"StopWatch.h"
class Candle : public BaseObject
{
public:
	Candle(GVector2 pos,eItemID _dropItemId);
	~Candle();

	void init() override;
	void update(float deltaTime) override;
	void draw(LPD3DXSPRITE, Viewport*) override;
	void release() override;

	float checkCollision(BaseObject* object, float dt);
private:
	map<eStatus, Animation*> _animations;
	eItemID		_dropItemId;
	StopWatch*	_burstStopWatch;
	map<string, IComponent*> _componentList;
};

