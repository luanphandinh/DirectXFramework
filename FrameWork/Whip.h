#pragma once
#include"BaseObject.h"
#include"Animation.h"
#include"CollisionBody.h"
class Whip : public BaseObject
{
public:
	Whip(int level = 0);
	~Whip();
	void setTracker(BaseObject* simon);
	//Các hàm bắt buộc override từ lớp baseobject
	void init() override;
	void update(float deltatime) override;
	void draw(LPD3DXSPRITE, Viewport*) override;
	void release() override;

	void restart();
	void isHitting();

	virtual float checkCollision(BaseObject* object, float dt);
private:
	//level của whip
	int _level;
	Animation* _animation;
	BaseObject* _simon;
	map<string, IComponent*> _componentList;
};

