#pragma once
#include"BaseEnemy.h"
#include"define.h"
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
private:
	map<string, IComponent*> _listComponent;
	bool _isStand;
	//Dùng để animation hủy  enemy 
	BaseObject *_burning;
};

