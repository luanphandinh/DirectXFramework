#pragma once

#include"define.h"
#include"SpriteManager.h"
#include"InputController.h"
_NAMESPACE_FRAMEWORK_BEGIN
class IControlable
{
public:
	IControlable();
	virtual void updateInput(float deltaTime = 0);
protected:
	InputController* _input;
};

class BaseObject
{
public:
	BaseObject(eID id);
	~BaseObject();

	virtual void init() = 0;
	virtual void update(float deltaTime) = 0;
	virtual void draw(LPD3DXSPRITE,Viewport*) = 0;
	virtual void release() = 0;

	eID getId();
	eStatus getStatus();
	virtual void setStatus(eStatus status);
	virtual void addStatus(eStatus status);
	virtual void removeStatus(eStatus status);
	virtual bool isInStatus(eStatus status);

	virtual GVector2 getPosition();
	virtual float getPositionX();
	virtual float getPositionY();

	virtual void setPosition(GVector3 vector);
	virtual void setPosition(GVector2 position);
	virtual void setPosition(float x, float y, float z);
	virtual void setPosition(float x, float y);
	virtual void setPositionX(float x);
	virtual void setPositionY(float y);

	virtual GVector2 getScale();
	virtual void setScale(GVector2 scale);
	virtual void setScale(float scale);
	virtual void setScaleX(float sx);
	virtual void setScaleY(float sy);

	virtual float getRotate();
	virtual void setRotate(float degree);

	virtual GVector2 getOrigin();
	virtual void setOrigin(GVector2 origin);

	virtual void setZIndex(float z);
	virtual float getZIndex();

	virtual void setOpacity(float opacity);
	virtual float getOpacity();

	virtual void setColor(D3DXCOLOR color);
	virtual D3DXCOLOR getColor();

	virtual GVector2 getAnchorPoint();

	Sprite* getSprite();

	virtual RECT getBounding();

	virtual float checkCollision(BaseObject* object, float dt);

	//Mặc đinh vật đứng yên sẽ trả về vector 0
	//nếu vật di chuyển thì override lên
	virtual GVector2 getVelocity();

	virtual void setPhysicBodySide(eDirection direction);
	virtual eDirection getPhysicBodySide();

private:
	Sprite* _sprite;

	/*
		Nếu muốn dùng animation thì khai báo trong lớp kế thừa
		Tham chiếu đến sprite 
	*/

	eID _id;
	eStatus _status;
	eDirection _physicsSide;
};

_NAMESPACE_FRAMEWORK_END

