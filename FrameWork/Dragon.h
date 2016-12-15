#pragma once
#include"BaseEnemy.h"
#include"define.h"
#include"StopWatch.h"
#include"ItemManager.h"
#include"DragonFire.h"
#define DRAGON_HP 5
class Dragon : public BaseEnemy
{
public:
	Dragon(GVector2 pos);
	~Dragon();

	void init() override;
	void update(float) override;
	void draw(LPD3DXSPRITE, Viewport*) override;
	void release() override;
	virtual float checkCollision(BaseObject* object, float dt);
	void updateDirection();
	void fire();
private:
	map<string, IComponent*> _listComponent;
	bool _isStand;
	bool _isOverHeated;
	int  _countFire;
	eDirection _direction;
	//Dùng để animation hủy  enemy 
	BaseObject *_burning;
	StopWatch* _overHeatedStopWatch;
	StopWatch* _fireStopWatch;
	vector<DragonFire*> _listItem;
	void removeFire();
};

