#pragma once
#include"BaseObject.h"
#include"IComponent.h"
#include<map>
/*
	Sử dụng cho 1 đối tượng dùng để xét va chạm

	- Thêm vào object dưới dạng 1 component

	- Có 2 event chính là :
		@onCollisionBegin: sẽ được gọi khi bắt đầu va chạm
		@onCollisionEnd: sẽ được gọi khi kết thúc va chạm

	- Đối tượng cần sử dụng 2 hàm trên thì
		- Thêm [event_receiver(native)] vào trước class định nghĩa object

		- Viết hàm có THAM SỐ như sau:
			+ void onCollisionBegin(CollisionEventArg* collision_event);
			+ void onCollisionEnd(CollisionEventArg* collision_event);

		- Gán hàm cho Collision Body
			+ __hook(&CollisionBody::onCollisionBegin, <collision body của object> , & <tên lớp>::<tên hàm>);

		- Việc kiểm tra và sử lý va chạm cho đối tượng đó sẽ làm bên trong 2 hàm BEGIN và END.

		- CollisionEventArg bao gồm 2 thành phần:
			+ Đối tượng va chạm với đối tượng hiện tại.
			+ _sideCollision: phía va chạm của đối tượng kia.

*/
class CollisionEventArg : public EventArg
{
public:
	CollisionEventArg(BaseObject* _object)
	{
		_otherObject = _object;
		_sideCollision = eDirection::NONE;
	}

	BaseObject* _otherObject;
	eDirection _sideCollision;
};

class CollisionBody : public IComponent
{
public:
	CollisionBody(BaseObject* baseObject);
	/*
		Kiểm tra va chạm với object khác,gọi event begin,end
			@otherObject : object cần kiểm tra va chạm với
			@dt : delta time của mỗi frame
			@updatePosition : collision body sẽ cập nhật lại vị trí của object
	*/
	void checkCollision(BaseObject* otherObject,float dt,bool updatePosition = true);

	~CollisionBody();

	/*
		Trả về giá trị entry time dựa trên dt(delta Time) được truyền vào
			@dt : là thời gian của 1 frame
	*/
	float sweptAABB(BaseObject* otherObject,eDirection& direction,float dt);

	/*
		Lấy vùng broadphase để check cho swept cho AABB
		Được tính bằng vị trí đầu và vị trí cuối của BoudingObject
	*/
	RECT getSweptBroadphaseRect(BaseObject* object, float dt);
private:
	BaseObject* _target;
	RECT _collisionBodyRect;

	float	_dxEntry,//Khoảng cách gần nhất theo trục x giữa 2 rect
		_dxExit,//Khoảng cách xa nhất theo trục x giữa 2 rect
		_dyEntry,//Khoảng cách gần nhất theo trục y giữa 2 rect
		_dyExit;//Khoảng cách xa nhất theo trục y giữa 2 rect
	float  _txEntry, _tyEntry, _txExit, _tyExit;//thời gian

	map<BaseObject*, bool> _listColliding;//Danh sách các objet mà base object collide với
};

