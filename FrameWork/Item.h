#pragma once
#include"define.h"
#include"BaseObject.h"
#include"CollisionBody.h"
#define NORMAL_ITEM_SPEED 400
_USING_FRAMEWORK

class Item : public BaseObject
{
public:
	/*
		Khởi tạo Item:
			@startPosition : vị trí bắt đầu
			@type: Loại item,rớt ra hoặc bị lượm lên
			@dir : Hướng của item khi bị ném ra
	*/
	Item(GVector2 startPosition, eItemType type, eItemID itemID, eDirection dir);
	~Item();


	virtual void init();
	virtual void update(float deltatime);
	virtual void draw(LPD3DXSPRITE, Viewport*);
	virtual void release();

	virtual GVector2 initVeloc(float speed);

	GVector2 getVelocity();
	eItemType getItemType();

	void onCollisionBegin(CollisionEventArg* collision_arg);

	virtual float checkCollision(BaseObject* object, float dt);
protected:
	GVector2	_startPosition;
	float	_degree;

	eDirection _direction;
	eItemType _type;
	eItemID	_itemID;

	map<string, IComponent*> _componentList;
};

