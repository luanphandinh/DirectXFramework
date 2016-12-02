#pragma once
#include"BaseObject.h"
#include"Animation.h"
#include"CollisionBody.h"
#include"Weapon.h"
class Whip : public BaseObject
{
public:
	/*
		Tạm thời để cái level thành 1cho dễ test
	*/
	Whip(int level = 1);
	~Whip();
	void setTracker(BaseObject* simon);
	//Các hàm bắt buộc override từ lớp baseobject
	void init() override;
	void update(float deltatime) override;
	void draw(LPD3DXSPRITE, Viewport*) override;
	void release() override;

	void restart();
	bool isHitting();

	virtual float checkCollision(BaseObject* object, float dt);
private:
	//level của whip
	int _level;
	int _damage = 1;
	map<int, Animation*> _animations;
	BaseObject* _simon;
	map<string, IComponent*> _componentList;
	map<BaseObject*, bool> _listColliding;
};

