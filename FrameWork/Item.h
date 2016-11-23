#pragma once
#include"define.h"
#include"BaseObject.h"
#include"CollisionBody.h"
#define NORMAL_ITEM_SPEED 1000
_USING_FRAMEWORK

class Item : public BaseObject
{
public:
	/*
		Khởi tạo Item:
			@startPosition : vị trí bắt đầu
			@type: Loại item,rớt ra hoặc bị lượm lên
	*/
	Item(GVector2 startPosition, eItemType type);
	~Item();


	virtual void init() = 0;
	virtual void update(float deltatime) = 0;
	virtual void draw(LPD3DXSPRITE, Viewport*) = 0;
	virtual void release() = 0;
	
	virtual GVector2 initVeloc(float speed);
	virtual void initCommonComponent();

	GVector2 getVelocity();
	eItemType getItemType();

	virtual void onCollisionBegin(CollisionEventArg* collision_arg);

	virtual float checkCollision(BaseObject* object, float dt);

	void stop();
protected:
	GVector2	_startPosition;
	eItemType	_type;
	map<string, IComponent*> _componentList;
};

