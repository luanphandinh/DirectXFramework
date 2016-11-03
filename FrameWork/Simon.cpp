#include "Simon.h"
_USING_FRAMEWORK

Simon::Simon() :BaseObject(eID::SIMON)
{
	_canJumpDown = true;//cho phép nhảy từ trên xuống
}


Simon::~Simon()
{
}

void Simon::updateInput(float dt)
{

}
//overiride BaseObject methods
void Simon::init()
{
	//Hook event for key press handlers.....
	__hook(&InputController::__enventKeyPressed, _input, &Simon::onKeyPressed);
	__hook(&InputController::__enventKeyReleased, _input, &Simon::onKeyReleased);

	_sprite = SpriteManager::getInstance()->getSprite(eID::SIMON);
	_sprite->setScale(2.0f);
	_sprite->setFrameRect(SpriteManager::getInstance()->getSourceRect(eID::SIMON, "normal"));

	Movement* movement = new Movement(GVector2Zero, GVector2Zero, _sprite);
	_componentList["Movement"] = movement;

	Gravity* gravity = new Gravity(GVector2(0,-GRAVITY),movement);
	gravity->setStatus(eGravityStatus::SHALLOWED);
	_componentList["Gravity"] = gravity;

	CollisionBody* collisionBody = new CollisionBody(this);
	_componentList["CollisionBody"] = collisionBody;
	//_test_sprite->drawBounding(true);
	//_test_sprite->setPosition(50, 50, 1.0f);
	_animations[eStatus::NORMAL] = new Animation(_sprite, 0.1f);
	_animations[eStatus::NORMAL]->addFrameRect(eID::SIMON, "normal", NULL);

	_animations[eStatus::RUNNING] = new Animation(_sprite, 0.12f);
	_animations[eStatus::RUNNING]->addFrameRect(eID::SIMON, "run_01", "run_02", "run_03", NULL);
	
	
	_animations[eStatus::JUMPING] = new Animation(_sprite, 0.1f);
	_animations[eStatus::JUMPING]->addFrameRect(eID::SIMON, "jump", NULL);

	_animations[eStatus::FALLING] = new Animation(_sprite, 0.1f);
	_animations[eStatus::FALLING]->addFrameRect(eID::SIMON, "run_03", NULL);

	_animations[eStatus::SITTING] = new Animation(_sprite, 0.1f);
	_animations[eStatus::SITTING]->addFrameRect(eID::SIMON, "sit", NULL);

	_animations[eStatus::HITTING] = new Animation(_sprite, 0.2f);
	_animations[eStatus::HITTING]->addFrameRect(eID::SIMON, "whip_normal_01", "whip_normal_02", "whip_normal_03","normal", NULL);
	_animations[eStatus::HITTING]->setLoop(false);
	this->_movingSpeed = SIMON_MOVING_SPEED;
	this->setPosition(500, 90);
	this->setStatus(eStatus::NORMAL);
}

void Simon::update(float deltatime)
{
	//this->checkPosition();

	this->updateStatus(deltatime);

	this->updateCurrentAnimateIndex();

	_animations[_currentAnimationIndex]->update(deltatime);

	//update component list
	// update component để sau cùng để sửa bên trên sau đó nó cập nhật đúng
	for (auto it = _componentList.begin(); it != _componentList.end(); it++)
	{
		it->second->update(deltatime);
	}

}

void Simon::draw(LPD3DXSPRITE spriteHandle, Viewport* viewport)
{
	_animations[_currentAnimationIndex]->draw(spriteHandle, viewport);
}

void Simon::release()
{

}


/*
Event for event_reciever
*/
void Simon::onKeyPressed(KeyEventArg* key_event)
{
	switch (key_event->_key)
	{
	case DIK_X:
		if (!this->isInStatus(eStatus::SITTING) || this->isInStatus(eStatus::MOVING_LEFT)
			|| this->isInStatus(eStatus::MOVING_RIGHT))
		{
			this->jump();
		}
		else
		{
			if (_canJumpDown && !this->isInStatus(eStatus::JUMPING) && !this->isInStatus(eStatus::FALLING))
			{
				this->removeStatus(eStatus::HITTING);
				this->addStatus(eStatus::JUMPING);
				this->addStatus(eStatus::FALLING);
			}
		}
		break;
	case DIK_RIGHT:
		this->removeStatus(eStatus::MOVING_RIGHT);
		this->removeStatus(eStatus::SITTING);
		this->addStatus(eStatus::MOVING_RIGHT);
		break;
	case DIK_LEFT:
		this->removeStatus(eStatus::MOVING_LEFT);
		this->removeStatus(eStatus::SITTING);
		this->addStatus(eStatus::MOVING_LEFT);
		break;
	case DIK_DOWN:
		this->removeStatus(eStatus::MOVING_RIGHT);
		this->removeStatus(eStatus::MOVING_LEFT);
		this->addStatus(eStatus::SITTING);
		break;
	case DIK_C:
		this->addStatus(eStatus::HITTING);
		_animations[eStatus::HITTING]->canAnimate(true);
		//_animations[eStatus::HITTING]->setIndex(0);
	default:
		break;
	}
}
/*
Event for event_reciever
*/
void Simon::onKeyReleased(KeyEventArg* key_event)
{
	switch (key_event->_key)
	{
	case DIK_RIGHT:
		this->removeStatus(eStatus::MOVING_RIGHT);
		break;
	case DIK_LEFT:
		this->removeStatus(eStatus::MOVING_LEFT);
		break;
	case DIK_DOWN:
		this->removeStatus(eStatus::SITTING);
		break;
	case DIK_C:
		//this->removeStatus(eStatus::HITTING);
	default:
		break;
	}
}

void Simon::setStatus(eStatus status) 
{
	_status = status;
}

// Character action.
void Simon::standing()
{
	auto move = (Movement*)this->_componentList["Movement"];
	move->setVelocity(GVector2Zero);
}

float Simon::getMovingSpeed()
{
	return _movingSpeed;
}

RECT Simon::getBounding() 
{
	return _sprite->getBounding();
}

void Simon::updateStatus(float deltatime)
{
	if ((this->getStatus() & eStatus::MOVING_LEFT) == eStatus::MOVING_LEFT) 
	{
		this->moveLeft();
	}
	else
	if ((this->getStatus() & eStatus::MOVING_RIGHT) == eStatus::MOVING_RIGHT) 
	{
		this->moveRight();
	}
	else 
	if ((this->getStatus() & eStatus::SITTING) == eStatus::SITTING) 
	{
	}
	else if ((this->getStatus() & eStatus::JUMPING) != eStatus::JUMPING)
	{
		this->standing();
	}
}

void Simon::jump()
{
	//Nếu object đã và đang nhảy rồi thì return
	if (this->isInStatus(JUMPING) || this->isInStatus(FALLING))
	{
		return;
	}

	//Chưa nhảy thì add trạng thái nhảy vào
	this->addStatus(eStatus::JUMPING);

	auto move = (Movement*)this->_componentList["Movement"];
	move->setVelocity(GVector2(move->getVelocity().x, SIMON_JUMP_VELOCITY));

	auto gravity = (Gravity*)this->_componentList["Gravity"];
	gravity->setStatus(eGravityStatus::FALLING_DOWN);
}

void Simon::moveLeft()
{
	if (this->getScale().x > 0)
		this->setScaleX(this->getScale().x * (-1));
	
	auto move = (Movement*)this->_componentList["Movement"];
	move->setVelocity(GVector2(-_movingSpeed,move->getVelocity().y));
}

void Simon::moveRight()
{
	if (this->getScale().x < 0)
		this->setScaleX(this->getScale().x * (-1));

	auto move = (Movement*)this->_componentList["Movement"];
	move->setVelocity(GVector2(_movingSpeed, move->getVelocity().y));
}

GVector2 Simon::getVelocity()
{
	auto move = (Movement*)this->_componentList["Movement"];
	return move->getVelocity();
}

float Simon::checkCollision(BaseObject* otherObject, float dt)
{
	//Sau này implent kiểm tra nhân vật đã chết hay bị hủy tại đây

	//Nếu object cần kiểm tra bị trùng thì trả về 0
	if (this == otherObject)
		return 0.0f;

	//Lấy collision body của simon ra để checkCollision
	auto collisionBody = (CollisionBody*)_componentList["CollisionBody"];
	eID otherObjectID = otherObject->getId();
	eDirection direction;

	//Kiểm tra va chạm với land
	if (otherObjectID == eID::LAND)
	{

	}

	return 0.0f;
}

void  Simon::updateCurrentAnimateIndex()
{
	_currentAnimationIndex = this->getStatus();
	if (isInStatus(eStatus::HITTING) && //nếu đang vung roi
		//mà di chuyển
		(isInStatus(eStatus::MOVING_LEFT) || isInStatus(eStatus::MOVING_RIGHT)))
	{
		//bỏ animation vung roi đi ,chỉ di chuyển thôi
		_currentAnimationIndex = (eStatus)(this->getStatus() & ~eStatus::HITTING);
	}
	else 
	if ((_currentAnimationIndex & eStatus::HITTING) == eStatus::HITTING)
	{
		_currentAnimationIndex = this->getStatus();
	}

	//Đang di chuyển
	if ((_currentAnimationIndex & eStatus::MOVING_LEFT) == eStatus::MOVING_LEFT 
		|| ((_currentAnimationIndex & eStatus::MOVING_RIGHT) == eStatus::MOVING_RIGHT)) 
	{
		// animate move left/right xài cung 1 animate là running nên cũng bỏ nó ra
		_currentAnimationIndex = (eStatus)(_currentAnimationIndex & ~(eStatus::MOVING_LEFT | MOVING_RIGHT));
		_currentAnimationIndex = (eStatus)(_currentAnimationIndex | eStatus::RUNNING);
	}	

	//Đang ngồi
	if ((_currentAnimationIndex & eStatus::SITTING) == eStatus::SITTING)
	{
		_currentAnimationIndex = (eStatus)(_currentAnimationIndex | eStatus::SITTING);
	}

	//Nếu đang nhảy hoặc rớt thì animate duy nhất nhảy
	if ((_currentAnimationIndex & eStatus::FALLING) == eStatus::FALLING)
	{
		_currentAnimationIndex = eStatus::FALLING;
	}

	if ((_currentAnimationIndex & eStatus::JUMPING) == eStatus::JUMPING)
	{
		_currentAnimationIndex = eStatus::JUMPING;
	}
}