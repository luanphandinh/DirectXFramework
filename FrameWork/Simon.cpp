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
	//gravity->setStatus(eGravityStatus::SHALLOWED);
	_componentList["Gravity"] = gravity;

	CollisionBody* collisionBody = new CollisionBody(this);
	_componentList["CollisionBody"] = collisionBody;
	__hook(&CollisionBody::onCollisionBegin, collisionBody, &Simon::onCollisionBegin);
	__hook(&CollisionBody::onCollisionEnd, collisionBody, &Simon::onCollisionEnd);
	//_sprite->drawBounding(true);
	//_test_sprite->setPosition(50, 50, 1.0f);
	_animations[eStatus::NORMAL] = new Animation(_sprite, 0.1f);
	_animations[eStatus::NORMAL]->addFrameRect(eID::SIMON, "normal", NULL);

	_animations[eStatus::RUNNING] = new Animation(_sprite, 0.1f);
	_animations[eStatus::RUNNING]->addFrameRect(eID::SIMON, "run_01", "run_02", "run_03", NULL);
	
	_animations[eStatus::JUMPING] = new Animation(_sprite, 0.1f);
	_animations[eStatus::JUMPING]->addFrameRect(eID::SIMON, "jump", NULL);

	_animations[eStatus::FALLING] = new Animation(_sprite, 0.1f);
	_animations[eStatus::FALLING]->addFrameRect(eID::SIMON, "run_03", NULL);

	_animations[eStatus::SITTING] = new Animation(_sprite, 0.1f);
	_animations[eStatus::SITTING]->addFrameRect(eID::SIMON, "sit", NULL);
	
	_animations[eStatus::UPSTAIR] = new Animation(_sprite, 0.1f);
	_animations[eStatus::UPSTAIR]->addFrameRect(eID::SIMON, "up_stair_01", "up_stair_02", NULL);

	/*_animations[eStatus::UPSTAIR] = new Animation(_sprite, 0.1f);
	_animations[eStatus::UPSTAIR]->addFrameRect(eID::SIMON, "normal", "up_stair", NULL);*/

	_animations[eStatus::DOWNSTAIR] = new Animation(_sprite, 0.1f);
	_animations[eStatus::DOWNSTAIR]->addFrameRect(eID::SIMON, "down_stair_01", "down_stair_02", NULL);
/*
	_animations[eStatus::STANDINGONSTAIR] = new Animation(_sprite, 0.1f);
	_animations[eStatus::STANDINGONSTAIR]->addFrameRect(eID::SIMON, "up_stair_01", NULL);*/

	_animations[eStatus::STANDINGONSTAIR_UP] = new Animation(_sprite, 0.1f);
	_animations[eStatus::STANDINGONSTAIR_UP]->addFrameRect(eID::SIMON, "up_stair_01", NULL);

	_animations[eStatus::STANDINGONSTAIR_DOWN] = new Animation(_sprite, 0.1f);
	_animations[eStatus::STANDINGONSTAIR_DOWN]->addFrameRect(eID::SIMON, "down_stair_01", NULL);

	_animations[eStatus::DYING] = new Animation(_sprite, 0.1f);
	_animations[eStatus::DYING]->addFrameRect(eID::SIMON,"dead" , NULL);


	_animations[eStatus::HITTING] = new Animation(_sprite, 0.1f);

	_animations[eStatus::HITTING]->addFrameRect(eID::SIMON, "whip_normal_01", "whip_normal_02", "whip_normal_03","normal", NULL);
	_animations[eStatus::HITTING]->setUseDefaultOrigin(false);
	this->resetValues();
	_reviveStopWatch = nullptr;

	this->_movingSpeed = SIMON_MOVING_SPEED;

	this->setPosition(300, 100);
	this->setStatus(eStatus::NORMAL);	
	gravity->setStatus(eGravityStatus::FALLING_DOWN);

	//Tạo stopWatch
	_stopWatch = new StopWatch();
}

void Simon::resetValues() {
	this->setScale(SCALE_FACTOR);

	_preObject = nullptr;

	_movingSpeed = SIMON_MOVING_SPEED;


	for (auto animate : _animations) {
		animate.second->setColorFlash(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}
}

void Simon::update(float deltatime)
{
	this->checkPosition();

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
	if (this->isInStatus(eStatus::DYING))
		return;

	switch (key_event->_key)
	{
	case DIK_X:
		if (!this->isInStatus(eStatus::SITTING) || this->isInStatus(eStatus::MOVING_LEFT)
			|| this->isInStatus(eStatus::MOVING_RIGHT))
		{
			//Nếu đang ngồi hoặc di chuyển thì cho phép nhảy
			//Chỉ add trạng thái nhảy vào thôi
			this->jump();
		}
		break;
	case DIK_RIGHT:
		if (!_canOnStair)
		{
			this->removeStatus(eStatus::MOVING_RIGHT);
			this->removeStatus(eStatus::SITTING);
			this->addStatus(eStatus::MOVING_RIGHT);
		}
		break;
	case DIK_LEFT:
		if (!_canOnStair)
		{
			this->removeStatus(eStatus::MOVING_LEFT);
			this->removeStatus(eStatus::SITTING);
			this->addStatus(eStatus::MOVING_LEFT);
		}
		break;
	case DIK_DOWN:
		if (this->isInStatus(eStatus::STANDINGONSTAIR))
		{
			this->removeStatus(eStatus::UPSTAIR);
			this->addStatus(eStatus::DOWNSTAIR);
		}
		else
		{
			this->removeStatus(eStatus::MOVING_RIGHT);
			this->removeStatus(eStatus::MOVING_LEFT);
			this->addStatus(eStatus::SITTING);
		}
		break;
	case DIK_C:
		this->removeStatus(eStatus::MOVING_RIGHT);
		this->removeStatus(eStatus::MOVING_LEFT);
		this->addStatus(HITTING);
	case DIK_UP:
		this->removeStatus(eStatus::MOVING_LEFT);
		this->removeStatus(eStatus::MOVING_RIGHT);
		this->removeStatus(eStatus::DOWNSTAIR);
		this->addStatus(eStatus::UPSTAIR);
		break;
	default:
		break;
	}
}
/*
Event for event_reciever
*/
void Simon::onKeyReleased(KeyEventArg* key_event)
{
	if (this->isInStatus(eStatus::DYING))
		return;
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
		this->removeStatus(eStatus::DOWNSTAIR);
		break;
	case DIK_C:
		this->removeStatus(eStatus::HITTING);
	case DIK_UP:
		this->removeStatus(eStatus::UPSTAIR);
		break;
	default:
		break;
	}
}

void Simon::setStatus(eStatus status) 
{
	if ((status & eStatus::DYING) == eStatus::DYING)
		return;

	_status = status;
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
	//Nếu nhân vật chết
	if (this->isInStatus(eStatus::DYING))
	{
		if (_reviveStopWatch == nullptr)
			_reviveStopWatch = new StopWatch();

		if (_reviveStopWatch->isStopWatch(REVIVE_TIME))
		{
			this->revive();
			SAFE_DELETE(_reviveStopWatch);
		}
		return; 
	}


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
		this->sit();
	}
	else if ((this->getStatus() & eStatus::FALLING) == eStatus::FALLING)
	{
		this->falling();
	}
	else if ((this->getStatus() & eStatus::UPSTAIR) == eStatus::UPSTAIR)
	{
		this->upstair();
	}
	else if ((this->getStatus() & eStatus::DOWNSTAIR) == eStatus::DOWNSTAIR)
	{
		this->downstair();
	}
	else if ((this->getStatus() & eStatus::JUMPING) != eStatus::JUMPING)
	{
		this->standing();
	}
}

void Simon::sit()
{
	auto move = (Movement*)this->_componentList["Movement"];
	move->setVelocity(GVector2(0, move->getVelocity().y));
}
#pragma region actions
void Simon::checkPosition()
{
	if (this->isInStatus(eStatus::DYING))
		return;

	auto viewport = SceneManager::getInstance()->getCurrentScene()->getViewport();
	auto viewportPosition = viewport->getPositionWorld();

	if (this->getPositionY() < viewportPosition.y - WINDOW_HEIGHT)
	{
		if (_status != eStatus::DYING)
			_status = eStatus::DYING;
		this->die();
	}
}

void Simon::die()
{
	auto move = (Movement*)this->_componentList["Movement"];
	move->setVelocity(GVector2(-SIMON_MOVING_SPEED * (this->getScale().x / SCALE_FACTOR), SIMON_JUMP_VELOCITY));

	auto g = (Gravity*)this->_componentList["Gravity"];
	g->setStatus(eGravityStatus::FALLING_DOWN);
}

void Simon::revive()
{
	//auto viewport = SceneManager::getInstance()->getCurrentScene()->getViewport();
	//auto viewportPosition = viewport->getPositionWorld();

	//if (auto scene = dynamic_cast<Scene*>(SceneManager::getInstance()->getCurrentScene())) {
		//this->setPosition(viewportPos.x, WINDOW_HEIGHT);
	this->setPosition(100, 150);

	//}
	//else {
	//	this->setPosition(viewportPosition.x + WINDOW_WIDTH / 2, viewportPosition.y - WINDOW_HEIGHT / 2);
	//}

	//reset value
	this->setStatus(eStatus::JUMPING);
	this->resetValues();
	
	//gán lại moveing speed
	auto move = (Movement*)this->_componentList["Movement"];
	move->setVelocity(GVector2(SIMON_MOVING_SPEED, 0));
	//cho nhân vật rớt xuống
	auto gravity = (Gravity*)this->_componentList["Gravity"];
	gravity->setStatus(eGravityStatus::FALLING_DOWN);

}

void Simon::standing()
{
	auto move = (Movement*)this->_componentList["Movement"];
	move->setVelocity(GVector2Zero);

	//Nếu là với lane bình thường thì bỏ trạng thái rớt và nhảy
	this->removeStatus(eStatus::FALLING);
	this->removeStatus(eStatus::JUMPING);
	//this->removeStatus(eStatus::UPSTAIR);
	//Nếu đang đứng trên cầu thang thì add status STANDINGONSTAIR vào
	//ko thì bỏ nó ra
	if (_canOnStair)
		this->addStatus(eStatus::STANDINGONSTAIR);
	else
	{
		this->removeStatus(eStatus::STANDINGONSTAIR);
		this->removeStatus(eStatus::STANDINGONSTAIR_UP);
		this->removeStatus(eStatus::STANDINGONSTAIR_DOWN);
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

void Simon::upstair()
{
	//Nếu ko có va chạm với cầu thang,hoặc đang đứng trên lane mà ko phải 
	//là đứng trên cầu thang
	if (!_canOnStair)
	{
		this->removeStatus(eStatus::UPSTAIR);
		return;
	}
		
	//Gán hướng cầu thang để lên và xuống
	if (_stairDirection == eStairDirection::LEFTBOTTOM_TO_RIGHTTOP)
	{
		moveRight();
		//Gán vector leo cầu thang
		auto move = (Movement*)this->_componentList["Movement"];
		move->setVelocity(GVector2(_movingSpeed, SIMON_UPSTAIR_VELOCITY));
	}
	else 
	{
		moveLeft();
		//Gán vector leo cầu thang
		auto move = (Movement*)this->_componentList["Movement"];
		move->setVelocity(GVector2(-_movingSpeed, SIMON_UPSTAIR_VELOCITY));
	}
	this->removeStatus(eStatus::STANDINGONSTAIR_DOWN);
	this->addStatus(eStatus::STANDINGONSTAIR_UP);
}

void Simon::downstair()
{
	//Nếu ko có va chạm với cầu thang,hoặc đang đứng trên lane mà ko phải 
	//là đứng trên cầu thang
	if (!_canOnStair)
	{
		this->removeStatus(eStatus::DOWNSTAIR);
		return;
	}
	//Gán hướng cầu thang để lên và xuống
	if (_stairDirection == eStairDirection::LEFTBOTTOM_TO_RIGHTTOP)
	{
		moveLeft();
		//Gán vector leo cầu thang
		auto move = (Movement*)this->_componentList["Movement"];
		move->setVelocity(GVector2(-_movingSpeed, -SIMON_UPSTAIR_VELOCITY));
	}
	else
	{
		moveRight();
		//Gán vector leo cầu thang
		auto move = (Movement*)this->_componentList["Movement"];
		move->setVelocity(GVector2(_movingSpeed, -SIMON_UPSTAIR_VELOCITY));
	}
	this->removeStatus(eStatus::STANDINGONSTAIR_UP);
	this->addStatus(eStatus::STANDINGONSTAIR_DOWN);
}

void Simon::falling()
{
	auto gravity = (Gravity*)this->_componentList["Gravity"];
	gravity->setStatus(eGravityStatus::FALLING_DOWN);
}

void Simon::moveLeft()
{
	//Lấy viewport hiện tại
	Viewport* viewport = SceneManager::getInstance()->getCurrentScene()->getViewport();
	GVector2 viewportPosition = viewport->getPositionWorld();
	float simonPositionX = this->getPositionX();
	auto halfWidth = this->getSprite()->getFrameWidth() * this->getOrigin().x;
	//Không cho đi vượt quá cạnh trái
	if (halfWidth + simonPositionX - _movingSpeed * 0.33 < viewportPosition.x)
	{
		this->setPositionX(simonPositionX + halfWidth);
		return;
	}
	//Nếu animation đang hướng sang phải thì gán lại sang left
	if (this->getScale().x > 0)
		this->setScaleX(this->getScale().x * (-1));
	//gán vector move với move speed âm vì đi ngược trục x
	auto move = (Movement*)this->_componentList["Movement"];
	move->setVelocity(GVector2(-_movingSpeed,move->getVelocity().y));
}

void Simon::moveRight()
{
	//Nếu animation đang hướng sang left thì gán lại sang right
	if (this->getScale().x < 0)
		this->setScaleX(this->getScale().x * (-1));

	auto move = (Movement*)this->_componentList["Movement"];
	move->setVelocity(GVector2(_movingSpeed, move->getVelocity().y));
}


#pragma endregion

GVector2 Simon::getVelocity()
{
	auto move = (Movement*)this->_componentList["Movement"];
	return move->getVelocity();
}
#pragma region Collision
void Simon::onCollisionBegin(CollisionEventArg* collision_arg)
{
	eID objectID = collision_arg->_otherObject->getId();
	switch (objectID)
	{

	case eID::LAND:
	default:
		break;
	}
}

void Simon::onCollisionEnd(CollisionEventArg* collision_arg)
{
	eID objectID = collision_arg->_otherObject->getId();

	switch (objectID)
	{
	case eID::LAND:
	{
		if (_preObject == collision_arg->_otherObject)
		{
			// hết chạm với land là fall chứ ko có jump
			this->removeStatus(eStatus::JUMPING);
			
			_preObject = nullptr;
		}
		break;
	}
	default:
		break;
	}
}




float Simon::checkCollision(BaseObject* otherObject, float dt)
{
	//Sau này implent kiểm tra nhân vật đã chết hay bị hủy tại đây
	if (this->isInStatus(eStatus::DYING))
		return 0.0f;
	//Nếu object cần kiểm tra bị trùng thì trả về 0
	if (this == otherObject)
		return 0.0f;

	//Lấy collision body của simon ra để checkCollision
	auto collisionBody = (CollisionBody*)_componentList["CollisionBody"];
	eID otherObjectID = otherObject->getId();
	eDirection direction;

	//Kiểm tra va chạm với land
	if (otherObjectID == eID::LAND || otherObjectID == eID::STAIR)
	{
		/* 
			Với LAND : Nếu simon ko nằm trong cả 2 trang thái là nhảy vả rớt,đang nhảy hoặc rớt từ trên xuống
						Thì check
			Với STAIR : Nếu simon ở trong trạng thái lên cầu thang thì check,check xong thì nếu
						có  va chạm thì để trạng thái thành STANDINGONSTAIR(để lần sau còn check lại ở frame tiếp nếu người chơi ko nhấn nút lên) 
						ko còn va chạm nữa thì remove đi
		*/
		if (((!this->isInStatus(eStatus(eStatus::JUMPING | eStatus::FALLING)) && otherObjectID == eID::LAND)
			|| (((isInStatus(eStatus::UPSTAIR) || isInStatus(eStatus::STANDINGONSTAIR))) && otherObjectID == eID::STAIR ))
			&& collisionBody->checkCollision(otherObject, direction, dt, false))
		{
			if (otherObjectID == eID::LAND)
			{
				auto land = (Land*)otherObject;
				_canJumpDown = land->canJump();
			}

			//Nếu va chạm với câu thang thì cho phép simon đứng trên cầu thang,nếu ko thì false
			if (otherObjectID == eID::STAIR)
			{
				auto stair = (Stair*)otherObject;
				_canOnStair = stair->canStandOnStair();
				_stairDirection = stair->getStairDirection();
			}
			else _canOnStair = false;
			//Nếu như va chạm hướng top,và trừ cái trường hợp mà simon đang trong trạng thái nhảy mà rớt xuống 
			//với vận tốc mà > -200
			if (direction == eDirection::TOP && !(this->getVelocity().y > -200 && this->isInStatus(eStatus::JUMPING)))
			{
				//vận tốc 200 hướng xuống => cho trường hợp nhảy từ dưới lên
				//xử lý đặc biệt,collision body update position bt ko được
				//khi vào đây mới update position cho nó
				float moveX, moveY;
				if (collisionBody->isCollidingIntersected(otherObject, moveX, moveY, dt))
				{
					collisionBody->updateTargetPosition(otherObject, direction, false, GVector2(moveX, moveY));
				}
				
				//Gán trạng thái gravity thành shallow vector trọng lực bằng 0,vật ko rớt xuống mà 
				//chỉ di chuyển sang ngang hoặc nhảy lên
				auto gravity = (Gravity*)this->_componentList["Gravity"];
				gravity->setStatus(eGravityStatus::SHALLOWED);
	
				//Nếu va chạm xong thì hàm standing sẽ bỏ hết 2 trạng thái jump và fall của đối tượng
				//Để xét va chạm lần tiếp theo
				this->standing();
				_preObject = otherObject;
			}
		}
		else if (_preObject == otherObject && !this->isInStatus(eStatus::HITTING))
		{
			// kiểm tra coi nhảy hết qua cái land cũ chưa
			// để gọi event end.
			collisionBody->checkCollision(otherObject, dt, false);

			//Nếu vật đi hết land cũ
			//thì gán gravity lại thành falling
			//Vì simon chỉ check collion kkhi nằm trong 1 trong 2 trạng thái,nên ra khỏi lane sẽ falling
			auto gravity = (Gravity*)this->_componentList["Gravity"];
			gravity->setStatus(eGravityStatus::FALLING_DOWN);
			this->_canOnStair = false;
			//Nếu đi ra khỏi bề mặt của lane thì add trạng thái thành falling
			if (!this->isInStatus(eStatus::JUMPING) && !this->isInStatus(eStatus::FALLING))
			{
				this->addStatus(eStatus::FALLING);
			}

		}
	}
	
	return 0.0f;
}

#pragma endregion 
void  Simon::updateCurrentAnimateIndex()
{
	//Lấy trạng thái của nhân vật
	_currentAnimationIndex = this->getStatus();

	if (isInStatus(eStatus::HITTING) //nếu đang vung roi
	//mà di chuyển
	&& (isInStatus(eStatus::MOVING_LEFT) || isInStatus(eStatus::MOVING_RIGHT)
	|| isInStatus(eStatus::UPSTAIR) || isInStatus(eStatus::DOWNSTAIR)))
	{
		//bỏ animation vung roi đi ,chỉ di chuyển thôi
		_currentAnimationIndex = (eStatus)(this->getStatus() & ~eStatus::HITTING);
	}
	else if ((_currentAnimationIndex & eStatus::HITTING) == eStatus::HITTING)
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
		_currentAnimationIndex = eStatus::SITTING;
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
	//Đang lên cầu thang và xuống cầu thang
	if ((_currentAnimationIndex & eStatus::UPSTAIR) == eStatus::UPSTAIR && _canOnStair)
	{
		_currentAnimationIndex = eStatus::UPSTAIR;
	}

	if ((_currentAnimationIndex & eStatus::DOWNSTAIR) == eStatus::DOWNSTAIR)
	{
		_currentAnimationIndex = eStatus::DOWNSTAIR;
	}

	if ((_currentAnimationIndex & eStatus::STANDINGONSTAIR) == eStatus::STANDINGONSTAIR)
	{
		if ((_currentAnimationIndex & eStatus::STANDINGONSTAIR_UP) == eStatus::STANDINGONSTAIR_UP)
			_currentAnimationIndex = eStatus::STANDINGONSTAIR_UP;
		else
			_currentAnimationIndex = eStatus::STANDINGONSTAIR_DOWN;
	}

	if (this->isInStatus(eStatus::DYING)) {
		_currentAnimationIndex = eStatus::DYING;
	}
}