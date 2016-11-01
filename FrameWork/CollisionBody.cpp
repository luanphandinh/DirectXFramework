#include "CollisionBody.h"
using namespace std;

CollisionBody::CollisionBody(BaseObject* baseObject)
{
	this->_target = baseObject;

	_collisionBodyRect.top = 0;
	_collisionBodyRect.left = 0;
	_collisionBodyRect.right = abs(_target->getBounding().right - _target->getBounding().left);
	_collisionBodyRect.bottom = abs(_target->getBounding().top - _target->getBounding().bottom);
}


CollisionBody::~CollisionBody()
{
}

void CollisionBody::checkCollision(BaseObject* otherObject, float dt, bool updatePosition)
{
	eDirection direction;
	float timeCollision = sweptAABB(otherObject,direction, dt);

	if (timeCollision < 1.0f)//có va chạm
	{

	}
}

float CollisionBody::sweptAABB(BaseObject* otherObject, eDirection& direction, float dt)
{
	RECT myRect = _target->getBounding();
	RECT otherRect = otherObject->getBounding();

	// sử dụng Broadphase rect để kt vùng tiếp theo có va chạm ko
	//

	//swept AABB
	//Tính vận tốc theo mỗi frame
	//dt là milisecond/1000 thành second
	GVector2 myVeclocity = GVector2(_target->getVelocity().x * dt / 1000, _target->getVelocity().y * dt / 1000);
	GVector2 otherVelocity = GVector2(otherObject->getVelocity().x * dt / 1000, otherObject->getVelocity().y * dt / 1000);
	GVector2 velocity = myVeclocity;
	
	if (otherVelocity != GVector2(0.0f, 0.0f))
	{
		velocity = otherVelocity - myVeclocity;
	}

	//Tính khoảng cách gần nhất,xa nhất theo 2 cạnh dx,dy
	//dx
	if (velocity.x > 0)
	{
		_dxEntry = otherRect.left - myRect.right;
		_dxExit = otherRect.right - myRect.left;
	}
	else
	{
		_dxEntry = otherRect.right - myRect.left;
		_dxExit = otherRect.left - myRect.right;
	}

	// dy
	if (velocity.y > 0)
	{
		_dyEntry = otherRect.bottom - myRect.top;
		_dyExit = otherRect.top - myRect.bottom;
	}
	else
	{
		_dyEntry = otherRect.top - myRect.bottom;
		_dyExit = otherRect.bottom - myRect.top;
	}

	//Tìm thời gian va chạm
	if (velocity.x == 0.0f)
	{
		//chia 0 ra vô cực
		_txEntry = -std::numeric_limits<float>::infinity();
		_txExit = std::numeric_limits<float>::infinity();
	}
	else
	{
		_txEntry = _dxEntry / velocity.x;
		_txExit = _dxExit / velocity.x;
	}

	if (velocity.y == 0)
	{
		_tyEntry = -std::numeric_limits<float>::infinity();
		_tyExit = std::numeric_limits<float>::infinity();
	}
	else
	{
		_tyEntry = _dyEntry / velocity.y;
		_tyExit = _dyExit / velocity.y;
	}

	//thời gian va chạm
	// va chạm khi x và y CÙNG chạm => thời gian va chạm trễ nhất giữa x và y
	float entryTime = max(_txEntry, _tyEntry);
	//hết va chạm là khi x,y hêt va chạm,lấy thời gian sớm nhất
	float exitTime = min(_txExit, _tyExit);

	/*
		Object không va chạm khi :
			+ Nếu thời gian bắt đầu va chạm trễ hơn thời gian kết thúc va chạm
			+ Thời gian va chạm x , y < 0.0f,2 vật chạy ra xa nhau
			+ Thời gian va chạm > 1.0f ,hai vật ở xa nhau chưa thể chạm
	*/
	if (entryTime > exitTime || _txEntry < 0.0f && _tyEntry < 0.0f || _txEntry > 1.0f || _tyEntry > 1.0f)
	{
		direction = eDirection::NONE;
		return 1.0f;
	}
	
	//Nếu thời gian va chạm x lớn hơn y
	if (_txEntry > _tyEntry)
	{
		//xét x
		//Nếu _dxEntry < 0 có nghĩa là object đang xét ở phía bên phải otherObject
		if (_dxEntry < 0.0f)
		{
			direction = eDirection::RIGHT;
		}
		else
		{
			direction = eDirection::LEFT;
		}
	}
	else
	{
		// xét y
		if (_dyEntry < 0.0f)
			//if(_dyExit < 0.0f)
		{
			direction = eDirection::TOP;
		}
		else
		{
			direction = eDirection::BOTTOM;
		}
	}

	return entryTime;
}

RECT CollisionBody::getSweptBroadphaseRect(BaseObject* object, float dt)
{
	//Vận tốc mỗi frame
	GVector2 velocity = GVector2(object->getVelocity().x * dt / 1000, object->getVelocity().y * dt / 1000);
	RECT myRect = object->getBounding();
	
	RECT rect;
	rect.top = velocity.y > 0 ? myRect.top + velocity.y : myRect.top;
	rect.bottom = velocity.y > 0 ? myRect.bottom : myRect.bottom + velocity.y;
	rect.left = velocity.x > 0 ? myRect.left : myRect.left + velocity.x;
	rect.right = velocity.x > 0 ? myRect.right + velocity.x : myRect.right;

	return rect;
}