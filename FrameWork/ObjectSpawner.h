#pragma once
/*
 * Dùng để đẻ ra Medusa Head liên tục
 */

#include "define.h"
#include "StopWatch.h"
#include "BaseObject.h"
#include "EmptyObject.h"
#include "MedusaHead.h"

[event_receiver(native)]
class ObjectSpawner : public EmptyObject {
public:

	/*
	 * Đẻ ra số lượng object ở vị trí: pos, trong khỏang thời gian: time
	 * @position: vị trí
	 * @type: lọai object
	 * @direction: -1 là trái, 1 là phải
	 * @time: khoảnng thời gian giữa mỗi lần tạo
	 * @number: số lượng object (-1 là vô hạn)
	*/
	ObjectSpawner(GVector2 position, int width, int height, eID type, int direction = -1, float time = 1000, int number = -1);

	~ObjectSpawner();

	virtual void init();
	virtual void update(float deltatime);
	virtual void draw(LPD3DXSPRITE spriteHandler, Viewport* viewport);
	virtual void release();

	virtual float checkCollision(BaseObject* object, float deltaTime);
	RECT getBounding() override;

	vector<BaseObject*> getObjects();

	void setOnePerOne(bool enable);
	bool isOnePerOne();

	void setMaxNumber(int number);
	int getMaxNumber();

private:

	BaseObject* getObject(eID id);
	void deleteObject();

	StopWatch* _stopWatch;
	vector<BaseObject*> _listObjects;

	eID _createType;
	float _time;
	int _number;
	int _counter;
	int _maxObject;

	int _direction;
	bool _isOnePerOne;
	RECT _bound;
	LPDIRECT3DSURFACE9 _surface;
	void updateDirection();
};

