#pragma once
#include "Animation.h"
#include "IComponent.h"
#include "BaseEnemy.h"
#include "Simon.h"
#include "StopWatch.h"
#include "HitEffect.h"

#define MUMMYMAN_SPEED 100
#define MUMMYMAN_HITPOINT 16
#define MUMMYMAN_SCORE 1000
#define MUMMYMAN_VELOC GVector2(100,0)

enum eMovingPath {
	MOVEHOLD,
	MOVEMEDIUMDISTANCE,
	MOVELONGDISTANCE,
};

class MummyMan : public BaseEnemy
{
public:
	MummyMan(GVector2 pos, eDirection direction);

	~MummyMan();

	void init() override;
	void update(float) override;
	void draw(LPD3DXSPRITE, Viewport*) override;
	void release() override;

	float checkCollision(BaseObject*, float) override;
	GVector2 getVelocity() override;
	IComponent* getComponent(string);

private:
	map<string, IComponent*> _listComponent;
	map<int, Animation*> _animations;
	BaseObject* prevObject;
	BaseObject *_explosion;
	BaseObject *_burning;

	bool _checkedWithLand;

	bool isHitted;
	eStatus _currentAnimateIndex;
	void updateCurrentAnimateIndex();

	/*
		Hướng di chuyển
	*/
	eDirection _direction;
	void trackSimon();
	void updateDirection();
	void changeDirection(eDirection dir);
	/*
		Lúc đầu đứng yên,sau đó di chuyển.dùng mấy cái phía dưới
	*/
	bool _isStanding;
	StopWatch* _standingStopWatch;
	void updateStatus();
	/*
		Khi bị hit,dùng 3 thằng này để cho nó dừng lại và mất máu các kiểu
	*/
	bool _isHitted;
	StopWatch* _getHittedStopWatch;
	void getHitted();

	/*
		Khi va chạm vơi simon
	*/
	bool _isMovingBack;
	eDirection _movingBackDirection;
	void movingBack();
	StopWatch* _movingBackStopWatch;

	bool _isHold;
	StopWatch* _holdStopWatch;

	void getMovingPath();
	eMovingPath _movingPath;
	GVector2 _movingBackPos;

	StopWatch*	_burstStopWatch;

	vector<BaseObject*> _listBandages;
	void createBandages();
	void removeBandages();
};

