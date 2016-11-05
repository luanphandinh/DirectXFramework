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

[event_source(native)]
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
	/*
		kiểm tra va chạm với object khác lấy được hướng va chạm, KO  gọi event Begin, End.
			@otherObject: object cần kt va chạm
			@direction: lấy hướng va chạm của otherObject
			@dt: delta time của mỗi frame
			@updatePosition: collision body sẽ cập nhật vị trí object lại nếu object
			chồng lấp lên object khác khi set = true
	*/
	bool checkCollision(BaseObject* otherObject, eDirection& direction, float dt, bool updatePosition = true);

	/*
		Cập nhập lại target position khi có va chạm:
			@otherObject : Đối tượng mà target va chạm với
			@direction : Hướng va chạm của otherObject
			@withVelocity : True khi kt va chạm với vận tốc,false khi kt với RECT
			@move : khoảng chồng lấp giữ 2 object
	*/
	void updateTargetPosition(BaseObject* otherObject, eDirection direction, bool withVelocity, GVector2 move = GVector2Zero);

	~CollisionBody();

	/*
		Trả về giá trị entry time dựa trên dt(delta Time) được truyền vào
			@otherObject : Đối tượng mà target va chạm với
			@direction : Là hướng va chạm của otherObject xảy ra với _target
			@dt : là thời gian của 1 frame
			
	*/
	float sweptAABB(BaseObject* otherObject,eDirection& direction,float dt);

	/*
		Check bằng AABB
		Được dùng để check BroadphaseRect với otherOBject->getBounding để 
		xét va chạm,nếu có thì dùng swept để lấy timecollision
	*/
	bool isAABB(RECT myRect, RECT otherRect);

	/*
		Hàm này dùng để sử dụng cho hàm updatePosition khi withveloc = false và có vector move
		Là hàm lấy khoảng chồng lấp giữa hai object theo chiều x hoặc y:
			@otheroBject	: đối tượng cần xét khoảng chồng lấp
			@moveX,moveY	: khoảng chồng lấp giữa hai đối tượng theo trục X và Y
								chỉ trả ra một trong giá trị nhỏ nhất giữa 2 trục,giá trị còn lại là 0
			@dt				: deltatime
			
	*/
	bool isCollidingIntersected(BaseObject * otherObject, float & moveX, float & moveY, float dt);
	eDirection getSide(BaseObject* otherObject);
	/*
		Lấy vùng broadphase để check cho swept cho AABB
		Được tính bằng vị trí đầu và vị trí cuối của bounding được dự đoán trước 
			bằng vận tốc trong frame đó
	*/
	RECT getSweptBroadphaseRect(BaseObject* object, float dt);

	void update(float deltaTime);

	__event void onCollisionBegin(CollisionEventArg*);
	__event void onCollisionEnd(CollisionEventArg*);

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

