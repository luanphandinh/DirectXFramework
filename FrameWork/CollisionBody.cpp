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
	//check sweptAABB
	float timeCollision = sweptAABB(otherObject,direction, dt);

	if (timeCollision < 1.0f)//có va chạm
	{
		if (otherObject->getPhysicBodySide() != eDirection::NONE && (direction & otherObject->getPhysicBodySide()) == direction)
		{
			updateTargetPosition(otherObject, direction, true);
		}
		CollisionEventArg* e = new CollisionEventArg(otherObject);
		e->_sideCollision = direction;

		__raise onCollisionBegin(e);
		_listColliding[otherObject] = true;
	}
	else if (_listColliding.find(otherObject) == _listColliding.end())	// ko có trong list đã va chạm
	{
		if (isAABB(_target->getBounding(), otherObject->getBounding()))
		{
			CollisionEventArg* e = new CollisionEventArg(otherObject);
			e->_sideCollision = this->getSide(otherObject);

			__raise onCollisionBegin(e);

			_listColliding[otherObject] = true;
		}
	}
	else	// có trong list đã va chạm
	{
		float moveX, moveY;
		if (isCollidingIntersected(otherObject, moveX, moveY, dt))		// kt va trạm lấy khoảng chồng lấp của 2 object
		{
			auto side = this->getSide(otherObject);

			if (otherObject->getPhysicBodySide() == eDirection::NONE || (side & otherObject->getPhysicBodySide()) != side)
				return;

			// cập nhật tọa độ
			if (updatePosition)
				updateTargetPosition(otherObject, side, false, GVector2(moveX, moveY));
		}
		else // nếu ko va chạm nữa là kết thúc va chạm
		{
			CollisionEventArg* e = new CollisionEventArg(otherObject);
			e->_sideCollision = eDirection::NONE;

			__raise onCollisionEnd(e);
			_listColliding.erase(otherObject);
		}
	}
}



bool CollisionBody::checkCollision(BaseObject* otherObject, eDirection& direction, float dt, bool updatePosition)
{
	float timeCollision = sweptAABB(otherObject, direction, dt);

	if (timeCollision < 1.0f)//Nếu có va chạm xảy ra
	{
		//otherObject phải có hướng va chạm của nó và hướng va chạm của nó phải bằng với hướng va chạm được 
		//xét sau khi sweptAABB 
		if (otherObject->getPhysicBodySide() != eDirection::NONE && (direction & otherObject->getPhysicBodySide()) == otherObject->getPhysicBodySide())
		{
			// cập nhật tọa độ
			updateTargetPosition(otherObject, direction, true);
		}

		return true;
	}
	else//Trường hợp sweptAABB ko xảy ra va chạm,kiểm tra xem giữa 2 object có intersectd với nhau ko
		//Từ đó update lại position
	{
		float moveX, moveY;
		if (isCollidingIntersected(otherObject, moveX, moveY, dt))
		{
			auto side = this->getSide(otherObject);
			direction = side;

			if (otherObject->getPhysicBodySide() == eDirection::NONE || (side & otherObject->getPhysicBodySide()) != side)
				return false;

			//Cập nhật tọa độ
			if (updatePosition)
				this->updateTargetPosition(otherObject,direction ,false,GVector2(moveX,moveY));

			return true;
		}
	}

	direction = eDirection::NONE;

	return false;
}



float CollisionBody::sweptAABB(BaseObject* otherObject, eDirection& direction, float dt)
{
	RECT myRect = _target->getBounding();
	RECT otherRect = otherObject->getBounding();

	// sử dụng Broadphase rect để kt vùng tiếp theo có va chạm ko
	//
	RECT broadphaseRect = this->getSweptBroadphaseRect(_target, dt);
	if (!isAABB(broadphaseRect, otherRect))
	{
		direction = eDirection::NONE;
		return 1.0f;
	}

	//swept AABB
	//Tính vận tốc theo mỗi frame
	//dt là milisecond/1000 thành second
	GVector2 myVeclocity = GVector2(_target->getVelocity().x * dt / 1000, _target->getVelocity().y * dt / 1000);
	GVector2 otherVelocity = GVector2(otherObject->getVelocity().x * dt / 1000, otherObject->getVelocity().y * dt / 1000);
	GVector2 velocity = myVeclocity;
	
	if (otherVelocity != GVector2Zero)
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
			//direction là phí va chạm với otherObject
			direction = eDirection::RIGHT;
		}
		else
		{
			/*
				vì _dxEntry là khoảng cách dương otherObject nằm phía bên phải của object đang xét
				nên phía va chạm trả về sẽ là bên trái của otherObject
			*/
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

bool CollisionBody::isAABB(RECT myRect, RECT otherRect)
{
	float left = otherRect.left - myRect.right;
	float right = otherRect.right - otherRect.left;
	float top = otherRect.top - myRect.bottom;
	float bottom = otherRect.bottom - myRect.top;

	//Nếu mà left > 0 có nghĩa là otherRect.left  nằm hẳn về phía bên phải của myRect 
	//thì va chạm không thể xảy ra,tương tự với 3 trường hợp còn lại
	return !(left > 0 || right < 0 || top < 0 || bottom > 0);
}

void CollisionBody::updateTargetPosition(BaseObject* otherObject, eDirection direction, bool withVelocity, GVector2 move)
{
	//Nếu update bằng vận tốc
	if (withVelocity)
	{
		if (otherObject->getPhysicBodySide() != eDirection::NONE && (direction & otherObject->getPhysicBodySide()) == direction)
		{
			GVector2 veloc = _target->getVelocity();
			GVector2 pos = _target->getPosition();
			//va chạm theo trục x
			if (_txEntry > _tyEntry)
			{
				if (0 < _txEntry && _txEntry < 1)
				{
					pos.x += _dxEntry;
				}
			}
			else//va chạm theo trục y
			{
				if (0 < _tyEntry && _tyEntry < 1)
				{
					pos.y += _dyEntry;
				}
			}
			_target->setPosition(pos);
		}
	}
	else//Nếu update bằng khoảng chồng lấp của 2 object
	{
		//Nếu object đang rơi xuống hoặc di chuyển mà va chạm cần xét lại tọa độ
		if (move.y > 0 && (otherObject->getPhysicBodySide() & eDirection::TOP) == eDirection::TOP && _target->getVelocity().y <= 0)
		{
			_target->setPositionY(_target->getPositionY() + move.y);
		}
		else if (move.y < 0 && (otherObject->getPhysicBodySide() & eDirection::BOTTOM) == eDirection::BOTTOM && _target->getVelocity().y >= 0)
		{
			_target->setPositionY(_target->getPositionY() + move.y);
		}

		if (move.x > 0 && (otherObject->getPhysicBodySide() & eDirection::RIGHT) == eDirection::RIGHT && _target->getVelocity().x <= 0)
		{
			_target->setPositionX(_target->getPositionX() + move.x);
		}
		else if (move.x < 0 && (otherObject->getPhysicBodySide() & eDirection::LEFT) == eDirection::LEFT && _target->getVelocity().x >= 0)
		{
			_target->setPositionX(_target->getPositionX() + move.x);
		}
	}
}

bool CollisionBody::isCollidingIntersected(BaseObject * otherObject, float & moveX, float & moveY, float dt)
{
	moveX = moveY = 0.0f;
	auto myRect = _target->getBounding();
	auto otherRect = otherObject->getBounding();

	//Xét giống trong AABB để xem hai đối tượng có xảy ra intersected hay ko
	float left = otherRect.left - myRect.right;
	float top = otherRect.top - myRect.bottom;
	float right = otherRect.right - myRect.left;
	float bottom = otherRect.bottom - myRect.top;

	// kt coi có va chạm không
	//  CÓ va chạm khi 
	//  left < 0 && right > 0 && top > 0 && bottom < 0
	//
	if (left > 0 || right < 0 || top < 0 || bottom > 0)
		return false;

	// tính offset chồng lấp giữa x,y
	// lấy khoảng cách nhỏ nhất
	moveX = abs(left) < right ? left : right;
	moveY = top < abs(bottom) ? top : bottom;

	// chỉ lấy phần lấn vào nhỏ nhất
	if (abs(moveX) < abs(moveY))
		moveY = 0.0f;
	else
		moveX = 0.0f;

	return true;
}

eDirection CollisionBody::getSide(BaseObject* otherObject)
{
	auto myRect = _target->getBounding();
	auto otherRect = otherObject->getBounding();

	float left = otherRect.left - myRect.right;
	float top = otherRect.top - myRect.bottom;
	float right = otherRect.right - myRect.left;
	float bottom = otherRect.bottom - myRect.top;

	// kt va chạm
	if (left > 0 || right < 0 || top < 0 || bottom > 0)
		return eDirection::NONE;

	float minX;
	float minY;
	eDirection sideY;
	eDirection sideX;

	if (top > abs(bottom))
	{
		minY = bottom;
		sideY = eDirection::BOTTOM;
	}
	else
	{
		minY = top;
		sideY = eDirection::TOP;
	}


	if (abs(left) > right)
	{
		minX = right;
		sideX = eDirection::RIGHT;
	}
	else
	{
		minX = left;
		sideX = eDirection::LEFT;
	}


	if (abs(minX) < abs(minY))
	{
		return sideX;
	}
	else
	{
		return sideY;
	}
}

void CollisionBody::update(float deltaTime)
{
}