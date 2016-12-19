#pragma once


#include "Animation.h"
#include "IComponent.h"
#include "BaseEnemy.h"
#include "Simon.h"
#include "StopWatch.h"
#include "HitEffect.h"
using namespace std;

#define BANDAGE_SPEED GVector2(150,0)

class Bandage : public BaseEnemy {
public:
	Bandage(GVector2 pos, eDirection direction);

	~Bandage();

	void init();
	void update(float);
	void draw(LPD3DXSPRITE, Viewport*);
	void release();

	float checkCollision(BaseObject*, float) override;
	IComponent* getComponent(string);
	GVector2 getVelocity() override;
private:
	map<string, IComponent*> _listComponent;
	map<int, Animation*> _animations;
	BaseObject* prevObject;
	BaseObject *_explosion;
	BaseObject *_burning;
	eDirection _direction;
	BaseObject* _preObject;
	void checkPosition();
	eStatus _currentAnimationIndex;
	void updateCurrentAnimateIndex();
	StopWatch*	_burstStopWatch;
};