#include "Simon.h"
#include "GameStatusBoard.h"
_USING_FRAMEWORK

Simon::Simon() :BaseObject(eID::SIMON)
{
	_canJumpDown = true;//cho phép nhảy từ trên xuống
}

//overiride BaseObject methods

Simon::~Simon()
{
}

void Simon::updateInput(float dt)
{

}
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
	this->setPhysicBodySide(eDirection::ALL);
	//_sprite->drawBounding(true);
	//_test_sprite->setPosition(50, 50, 1.0f);
	_animations[eStatus::NORMAL] = new Animation(_sprite, 0.1f);
	_animations[eStatus::NORMAL]->addFrameRect(eID::SIMON, "normal", NULL);

	// sửa lại thứ tự chút 
	_animations[eStatus::RUNNING] = new Animation(_sprite, 0.12f);
	_animations[eStatus::RUNNING]->addFrameRect(eID::SIMON, "run_02", "run_01", "run_03", NULL);
	
	_animations[eStatus::JUMPING] = new Animation(_sprite, 0.1f);
	_animations[eStatus::JUMPING]->addFrameRect(eID::SIMON, "jump", NULL);

	//Hầu hết mỗi frame thì thằng falling nó đều có vì được check liên tục
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


	_animations[eStatus::HITTING] = new Animation(_sprite, 0.12f);
	_animations[eStatus::HITTING]->addFrameRect(eID::SIMON, "throw_item_01", "throw_item_02", "throw_item_03","normal", NULL);

	_animations[eStatus::HITTING | eStatus::STANDINGONSTAIR_UP] = new Animation(_sprite, 0.12f);
	_animations[eStatus::HITTING | eStatus::STANDINGONSTAIR_UP]->addFrameRect(eID::SIMON, "throw_item_stair_up_01", "throw_item_stair_up_02", "throw_item_stair_up_03", "up_stair_01", NULL);

	_animations[eStatus::HITTING | eStatus::STANDINGONSTAIR_DOWN] = new Animation(_sprite, 0.12f);
	_animations[eStatus::HITTING | eStatus::STANDINGONSTAIR_DOWN]->addFrameRect(eID::SIMON, "throw_item_stair_down_01", "throw_item_stair_down_02", "throw_item_stair_down_03", "down_stair_01", NULL);

	_animations[eStatus::HITTING | eStatus::SITTING] = new Animation(_sprite, 0.12f);
	_animations[eStatus::HITTING | eStatus::SITTING]->addFrameRect(eID::SIMON, "whip_sit_001", "whip_sit_002", "whip_sit_003", "sit", NULL);

	//_whipLevel = 1;

	//_animations[eStatus::HITTING | eStatus::LEVEL1] = new Animation(_sprite, 0.1f);
	//_animations[eStatus::HITTING | eStatus::LEVEL1]->addFrameRect(eID::SIMON, "whip_normal_lv1_01", "whip_normal_lv1_02", "whip_normal_lv1_03", "normal", NULL);

	//_animations[eStatus::HITTING | eStatus::STANDINGONSTAIR_UP | eStatus::LEVEL1] = new Animation(_sprite, 0.1f);
	//_animations[eStatus::HITTING | eStatus::STANDINGONSTAIR_UP | eStatus::LEVEL1]->addFrameRect(eID::SIMON, "whip_stair_up_lv1_01", "whip_stair_up_lv1_02", "whip_stair_up_lv1_03", "up_stair_01", NULL);

	//_animations[eStatus::HITTING | eStatus::STANDINGONSTAIR_DOWN | eStatus::LEVEL1] = new Animation(_sprite, 0.1f);
	//_animations[eStatus::HITTING | eStatus::STANDINGONSTAIR_DOWN | eStatus::LEVEL1]->addFrameRect(eID::SIMON, "whip_stair_down_lv1_01", "whip_stair_down_lv1_02", "whip_stair_down_lv1_03", "down_stair_01", NULL);

	//_animations[eStatus::HITTING | eStatus::SITTING | eStatus::LEVEL1] = new Animation(_sprite, 0.1f);
	//_animations[eStatus::HITTING | eStatus::SITTING | eStatus::LEVEL1]->addFrameRect(eID::SIMON, "whip_sit_lv1_01", "whip_sit_lv1_02", "whip_sit_lv1_03", "sit", NULL);

	_animations[eStatus::THROWING_ITEM] = new Animation(_sprite, 0.12f);
	_animations[eStatus::THROWING_ITEM]->addFrameRect(eID::SIMON, "throw_item_01", "throw_item_02", "throw_item_03", "normal", NULL);

	_animations[eStatus::THROWING_ITEM | eStatus::STANDINGONSTAIR_UP] = new Animation(_sprite, 0.12f);
	_animations[eStatus::THROWING_ITEM | eStatus::STANDINGONSTAIR_UP]->addFrameRect(eID::SIMON, "throw_item_stair_up_01", "throw_item_stair_up_02", "throw_item_stair_up_03", "up_stair_01", NULL);

	_animations[eStatus::THROWING_ITEM | eStatus::STANDINGONSTAIR_DOWN] = new Animation(_sprite, 0.12f);
	_animations[eStatus::THROWING_ITEM | eStatus::STANDINGONSTAIR_DOWN]->addFrameRect(eID::SIMON, "throw_item_stair_down_01", "throw_item_stair_down_02", "throw_item_stair_down_03", "down_stair_01", NULL);
	
	this->resetValues();
	_reviveStopWatch = nullptr;


	this->_movingSpeed = SIMON_MOVING_SPEED;

	//this->setPosition(50, 100);
	this->setStatus(eStatus::NORMAL);	
	gravity->setStatus(eGravityStatus::FALLING_DOWN);

	//Tạo stopWatch
	_stopWatch = new StopWatch();
	//Có đang đánh hay không
	_isHitting = false;
	_isThrowing = false;
	//Tạo lifeUI
	
	_whip = new Whip();
	_whip->init();
	_whip->setTracker(this);
	//ItemManager::insertItem((Item*)_whip);
	//_whipAnimation

}

void Simon::resetValues() {
	this->setScale(SCALE_FACTOR);
	this->_isFreezed = false;
	_preObject = nullptr;
	_isHitted = false;
	_isProtected = false;
	SAFE_DELETE(_reviveStopWatch);
	SAFE_DELETE(_hittingStopWatch);
	SAFE_DELETE(_throwItemStopWatch);
	SAFE_DELETE(_isHittedStopWatch);
	SAFE_DELETE(_protectStopWatch);
	_movingSpeed = SIMON_MOVING_SPEED;


	for (auto animate : _animations) {
		animate.second->enableFlashes(false);
		animate.second->setColorFlash(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}
}

void Simon::update(float deltatime)
{
	this->checkPosition();

	if (GameStatusBoard::getInstance()->getSimonLifeUI()->getHPNumber() <= 0)
	{
		_status = eStatus::DYING;
		this->die();
	}

	this->updateStatus(deltatime);

	this->updateCurrentAnimateIndex();

	_animations[_currentAnimationIndex]->update(deltatime);

	//_whip->update(deltatime);

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

	//_whip->draw(spriteHandle, viewport);
		
}

void Simon::release()
{
	for (auto it = _animations.begin(); it != _animations.end(); it++)
	{
		SAFE_DELETE(it->second);
	}
	_animations.clear();

	for (auto it = _componentList.begin(); it != _componentList.end(); it++)
	{
		SAFE_DELETE(it->second);
	}
	_componentList.clear();

	SAFE_DELETE(_sprite);
	SAFE_DELETE(_stopWatch);
	SAFE_DELETE(_reviveStopWatch);
	SAFE_DELETE(_protectStopWatch);
	SAFE_DELETE(_hittingStopWatch);
	SAFE_DELETE(_throwItemStopWatch);
	SAFE_DELETE(_isHittedStopWatch);
	this->unhookInputEvent();
}

void Simon::unhookInputEvent()
{
	if (_input != nullptr)
		__unhook(_input);
}

/*
Event for event_reciever
*/
void Simon::onKeyPressed(KeyEventArg* key_event)
{
	if (this->isInStatus(eStatus::DYING))
		return;
	if (_isFreezed) return;
	//if (_hittingStopWatch != nullptr || _reviveStopWatch != nullptr)
	//	return;
	if (_isHitted)
		return;

	switch (key_event->_key)
	{
	case DIK_W:
	case DIK_S:
		if (!this->isInStatus(eStatus::STANDINGONSTAIR))
		{
			//Nếu đang ngồi hoặc di chuyển thì cho phép nhảy
			//Chỉ add trạng thái nhảy vào thôi
			this->jump();
		}
		break;
	case DIK_RIGHT:
		if (!this->isInStatus(eStatus::UPSTAIR))
		{
			this->removeStatus(eStatus::MOVING_LEFT);
			this->removeStatus(eStatus::SITTING);
			this->addStatus(eStatus::MOVING_RIGHT);
		}
		break;
	case DIK_LEFT:
		if (!this->isInStatus(eStatus::UPSTAIR))
		{
			this->removeStatus(eStatus::MOVING_RIGHT);
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
			this->addStatus(eStatus::SITTING);
			this->removeStatus(eStatus::MOVING_RIGHT);
			this->removeStatus(eStatus::MOVING_LEFT);
		}
		break;
	case DIK_A:
		if (_hittingStopWatch != nullptr) break;
		this->removeStatus(eStatus::UPSTAIR);
		this->removeStatus(eStatus::DOWNSTAIR);
		if (this->isInStatus(eStatus::STANDINGONSTAIR))
		{
			this->enableGravity(false);
			this->hitting();
		}
		this->addStatus(eStatus::HITTING);
		_whip->restart();
		_isHitting = false;
		break;
	case DIK_Z:
		if (_throwItemStopWatch != nullptr) break;
		if (ActiveWeapon::isAvailable())
		{
			this->addStatus(eStatus::THROWING_ITEM);
			_isThrowing = false;
		}
		break;
	case DIK_UP:
		if (this->isInStatus(eStatus::FALLING) || this->isInStatus(eStatus::MOVING_LEFT) || this->isInStatus(eStatus::MOVING_RIGHT)) break;
		this->removeStatus(eStatus::DOWNSTAIR);
		this->removeStatus(eStatus::FALLING);
		this->addStatus(eStatus::UPSTAIR);
		break;
		//========================TESTING===========================//
	case DIK_E:
		swapWeapon();
		break;
		//========================TESTING===========================//
	default:
		break;
	}
}
//========================TESTING===========================//
void Simon::swapWeapon()
{
	int id = (int)ActiveWeapon::getItemID();
	if (id == 11) id = 7;
	else id += 1;
	ActiveWeapon::setItemID((eItemID)id);
}
//========================TESTING===========================//
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
		break;
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
	//Nếu nhân vật đang bị đánh
	if (_isHitted)
	{
		if (_isHittedStopWatch == nullptr)
		{
			_isHittedStopWatch = new StopWatch();
			//Nhân với trừ 1 để nhảy ngược lại
			if (!this->isInStatus(eStatus::STANDINGONSTAIR) && !this->isInStatus(UPSTAIR) && !this->isInStatus(DOWNSTAIR))
			{
				auto move = (Movement*)this->_componentList["Movement"];
				move->setVelocity(GVector2((-1) * _movingSpeed * (this->getScale().x / SCALE_FACTOR), this->getVelocity().y + 300));
				auto gravity = (Gravity*)this->_componentList["Gravity"];
				gravity->setStatus(eGravityStatus::FALLING_DOWN);
			}
		}
	
		if (_isHittedStopWatch->isStopWatch(200))
		{
			_isHitted = false;
			SAFE_DELETE(_isHittedStopWatch);
		}
		return;
	}

	if (_isProtected)
	{
		if (_protectStopWatch == nullptr)
		{
			_protectStopWatch = new StopWatch();
		}
		if (_protectStopWatch->isStopWatch(1000))
		{
			_isProtected = false;
			SAFE_DELETE(_protectStopWatch);
		}
	}
	//Nếu nhân vật đang đánh
	if (this->isInStatus(eStatus::HITTING))
	{
		if (_hittingStopWatch == nullptr)
		{
			_hittingStopWatch = new StopWatch();
		}

		if (_hittingStopWatch->isStopWatch(350))
		{
			//_whip->restart();
			this->removeStatus(eStatus::HITTING);
			SAFE_DELETE(_hittingStopWatch);
			this->removeStatus(eStatus::UPSTAIR);
		}
		return;
	}else
	if (this->isInStatus(eStatus::THROWING_ITEM))
	{
		if (_throwItemStopWatch == nullptr)
		{
			_throwItemStopWatch = new StopWatch();
		}

		if (_throwItemStopWatch->isStopWatch(400))
		{
			this->removeStatus(eStatus::THROWING_ITEM);
			SAFE_DELETE(_throwItemStopWatch);
			this->removeStatus(eStatus::UPSTAIR);
			this->getWeapon();
		}
		return;
	}

	trackFlyLandPosition(deltatime);
	//_isHitted = false;

	if ((this->getStatus() & eStatus::MOVING_LEFT) == eStatus::MOVING_LEFT 
		&& (this->getStatus() & eStatus::STANDINGONSTAIR) != eStatus::STANDINGONSTAIR
		&& (this->getStatus() & eStatus::JUMPING) != eStatus::JUMPING
		&& (this->getStatus() & eStatus::HITTING) != eStatus::HITTING)
	{
		this->moveLeft();
	}
	else
	if ((this->getStatus() & eStatus::MOVING_RIGHT) == eStatus::MOVING_RIGHT 
		&& (this->getStatus() & eStatus::STANDINGONSTAIR) != eStatus::STANDINGONSTAIR
		&& (this->getStatus() & eStatus::JUMPING) != eStatus::JUMPING
		&& (this->getStatus() & eStatus::HITTING) != eStatus::HITTING)
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
	else if ((this->getStatus() & eStatus::JUMPING) == eStatus::JUMPING)
	{
		this->jump();
	}
}

void Simon::sit()
{
	auto move = (Movement*)this->_componentList["Movement"];
	move->setVelocity(GVector2(0, move->getVelocity().y));
}
#pragma region actions
void Simon::enableGravity(bool enable)
{
	if (!enable)
	{
		auto gravity = (Gravity*)this->_componentList["Gravity"];
		gravity->setStatus(eGravityStatus::SHALLOWED);
	}
	else
	{
		auto gravity = (Gravity*)this->_componentList["Gravity"];
		gravity->setStatus(eGravityStatus::FALLING_DOWN);
	}
}

void Simon::checkPosition()
{
	if (this->isInStatus(eStatus::DYING))
		return;

	auto viewport = SceneManager::getInstance()->getCurrentScene()->getViewport();
	auto viewportPosition = viewport->getPositionWorld();

	float simonPositionX = this->getPositionX();
	auto halfWidth = this->getSprite()->getFrameWidth() * this->getOrigin().x;
	//Không cho đi vượt quá cạnh trái
	if (halfWidth + simonPositionX - _movingSpeed * 0.33 < viewportPosition.x)
	{
		this->setPositionX(simonPositionX + halfWidth);
		return;
	}

	halfWidth = this->getSprite()->getFrameWidth() * this->getOrigin().x;
	//Không cho đi vượt quá cạnh phải
	if (halfWidth + simonPositionX > viewportPosition.x + WINDOW_WIDTH) {
		this->setPositionX(simonPositionX - halfWidth);
		return;
	}

	if (this->getPositionY() < viewportPosition.y - WINDOW_HEIGHT)
	{
		if (_status != eStatus::DYING)
			_status = eStatus::DYING;
		this->die();
	}
}

void Simon::die()
{
	//auto move = (Movement*)this->_componentList["Movement"];
	//move->setVelocity(GVector2(-SIMON_JUMP_VELOCITY.x * (this->getScale().x / SCALE_FACTOR), SIMON_JUMP_VELOCITY.y));

	//auto g = (Gravity*)this->_componentList["Gravity"];
	//g->setStatus(eGravityStatus::FALLING_DOWN);
	_isHitted = false;
	_isProtected = false;
}

void Simon::revive()
{
	SceneManager::getInstance()->getCurrentScene()->getDirector()->updateRevive();
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
	{
		this->addStatus(eStatus::STANDINGONSTAIR);
	}
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
	move->setVelocity(GVector2(move->getVelocity().x == 0 ? 0 : SIMON_JUMP_VELOCITY.x * (this->getScale().x / SCALE_FACTOR)
		, SIMON_JUMP_VELOCITY.y));

	auto gravity = (Gravity*)this->_componentList["Gravity"];
	gravity->setStatus(eGravityStatus::FALLING_DOWN);
}

void Simon::upstair()
{
	//Nếu ko có va chạm với cầu thang,hoặc đang đứng trên lane mà ko phải 
	//là đứng trên cầu thang
	if (!_canOnStair/* || !_canUpStair*/)
	{
		this->removeStatus(eStatus::UPSTAIR);
		//this->addStatus(eStatus::FALLING);
		return;
	}
		
	//Gán hướng cầu thang để lên và xuống
	if (_stairDirection == eStairDirection::LEFTBOTTOM_TO_RIGHTTOP)
	{
		moveRight();
		//Gán vector leo cầu thang
		auto move = (Movement*)this->_componentList["Movement"];
		move->setVelocity(GVector2(SIMON_UPSTAIR_VELOCITY_X, SIMON_UPSTAIR_VELOCITY_Y));
	}
	else 
	{
		moveLeft();
		//Gán vector leo cầu thang
		auto move = (Movement*)this->_componentList["Movement"];
		move->setVelocity(GVector2(-SIMON_UPSTAIR_VELOCITY_X, SIMON_UPSTAIR_VELOCITY_Y));
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
		move->setVelocity(GVector2(-SIMON_UPSTAIR_VELOCITY_X, -SIMON_UPSTAIR_VELOCITY_Y));
	}
	else
	{
		moveRight();
		//Gán vector leo cầu thang
		auto move = (Movement*)this->_componentList["Movement"];
		move->setVelocity(GVector2(SIMON_UPSTAIR_VELOCITY_X, -SIMON_UPSTAIR_VELOCITY_Y));
	}
	this->removeStatus(eStatus::STANDINGONSTAIR_UP);
	this->addStatus(eStatus::STANDINGONSTAIR_DOWN);
}

void Simon::falling()
{
	//Ko đứng trên cầu thang thì falling thành rơi thẳng xuống dưới
	if (!isInStatus(eStatus::STANDINGONSTAIR))
	{
		auto move = (Movement*)this->_componentList["Movement"];
		move->setVelocity(GVector2(0, -300));
	}

	//không thì nhảy bình thường sẽ có thêm hướng x
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
	//Lấy viewport hiện tại
	Viewport* viewport = SceneManager::getInstance()->getCurrentScene()->getViewport();
	GVector2 viewportPosition = viewport->getPositionWorld();
	float simonPositionX = this->getPositionX();
	auto halfWidth = this->getSprite()->getFrameWidth() * this->getOrigin().x;
	//Không cho đi vượt quá cạnh phải
	if (halfWidth + simonPositionX > viewportPosition.x + WINDOW_WIDTH) {
		this->setPositionX(simonPositionX - halfWidth);
		return;
	}
	//Nếu animation đang hướng sang left thì gán lại sang right
	if (this->getScale().x < 0)
		this->setScaleX(this->getScale().x * (-1));

	auto move = (Movement*)this->_componentList["Movement"];
	move->setVelocity(GVector2(_movingSpeed, move->getVelocity().y));
}

void Simon::hitting()
{
	auto move = (Movement*)this->_componentList["Movement"];
	move->setVelocity(GVector2Zero);
}

void Simon::getHitted(int _dropHp)
{
	//Nếu simon chưa bị hit và ko được protected
	if (_isHitted == false && _isHittedStopWatch == nullptr && !_isProtected)
	{
		_isHitted = true;
		_isProtected = true;
		GameStatusBoard::getInstance()->getSimonLifeUI()->setHPNumber(
			GameStatusBoard::getInstance()->getSimonLifeUI()->getHPNumber() - _dropHp);
	}
}

void Simon::trackFlyLandPosition(float deltatime)
{
	if (_trackedFlyLand == nullptr) return;
	auto flyLandVeloc = _trackedFlyLand->getVelocity();
	auto newPos = GVector2(flyLandVeloc.x * deltatime / 1000,0);
	this->setPosition(this->getPosition() + newPos);;
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
	//if (this->isInStatus(eStatus::DYING))
	//	return 0.0f;
	//Nếu object cần kiểm tra bị trùng thì trả về 0
	if (this == otherObject)
		return 0.0f;

	//this->_whip->checkCollision(otherObject, dt);

	//Lấy collision body của simon ra để checkCollision
	auto collisionBody = (CollisionBody*)_componentList["CollisionBody"];
	eID otherObjectID = otherObject->getId();
	eDirection direction;

	//Kiểm tra va chạm với land
	if (otherObjectID == eID::LAND || otherObjectID == eID::STAIR || otherObjectID == eID::FLYLAND || otherObjectID == eID::BRICK)
	{
		/* 
			Với LAND : Nếu simon ko nằm trong cả 2 trang thái là nhảy vả rớt,đang nhảy hoặc rớt từ trên xuống
						Thì check
			Với STAIR : + Nếu simon ở trong trạng thái lên cầu thang thì check,check xong thì nếu
						có  va chạm thì để trạng thái thành STANDINGONSTAIR(để lần sau còn check lại ở frame tiếp nếu người chơi ko nhấn nút lên) 
						ko còn va chạm nữa thì remove đi
						+ Nếu simon đang đứng trên đầu cầu thang,mà nhấn xuống,thì kiểm tra xem có va chạm ko
						va chạm thì bỏ status sitting đi ,add downstair vào
		*/
		if (
			(
			(!this->isInStatus(eStatus(eStatus::JUMPING | eStatus::FALLING)) && (otherObjectID == eID::LAND || otherObjectID == eID::BRICK))
			|| (!this->isInStatus(eStatus(eStatus::JUMPING | eStatus::FALLING)) && otherObjectID == eID::FLYLAND)
			 //||	(this->isInStatus(eStatus::SITTING) && otherObjectID == eID::STAIR)
			 || (((isInStatus(eStatus::UPSTAIR) && ((Stair*)otherObject)->canUpStair()) 
					|| (isInStatus(eStatus::SITTING) && !((Stair*)otherObject)->canUpStair()) || this->isInStatus(eStatus::DOWNSTAIR)) && otherObjectID == eID::STAIR)
			)
			&& collisionBody->checkCollision(otherObject, direction, dt, false)
			)
		{
			if (otherObjectID == eID::LAND)
			{
				auto land = (Land*)otherObject;
				_canJumpDown = land->canJump();
				if ((this->isInStatus(STANDINGONSTAIR) || this->isInStatus(UPSTAIR)) && !this->isInStatus(eStatus::DOWNSTAIR)) 
				{
					if (land->checkable()) return 0.0f;
				}
				//if (_preObject != nullptr && _preObject->getId() == eID::LAND) return 0.0f;
			}

			if (otherObjectID == eID::FLYLAND)
			{
				_trackedFlyLand = otherObject;
			}

			if (otherObjectID == eID::BRICK)
			{
				if (otherObject->isInStatus(eStatus::BURN))
					return 0.0f;
			}

			//Nếu va chạm với câu thang thì cho phép simon đứng trên cầu thang,nếu ko thì false
			if (otherObjectID == eID::STAIR && _preObject != nullptr)
			{
				//if (_preObject != nullptr)
				if (_preObject != nullptr && _preObject->getId() == eID::STAIR) return 0.0f;
				auto stair = (Stair*)otherObject;
				_canOnStair = stair->canStandOnStair();
				_stairDirection = stair->getStairDirection();

				if (this->isInStatus(eStatus::UPSTAIR) && stair->canUpStair())
				{
					setPositionInStair(stair);
				}

				if (this->isInStatus(eStatus::SITTING)) 
				{
					this->removeStatus(eStatus::SITTING);
					this->addStatus(eStatus::DOWNSTAIR);
					setPositionInStair(stair);
					this->addStatus(eStatus::STANDINGONSTAIR);
				}
				//}
			}
			else _canOnStair = false;
			//Nếu như va chạm hướng top,và trừ cái trường hợp mà simon đang trong trạng thái nhảy mà rớt xuống 
			//với vận tốc mà > -200
			if (direction == eDirection::TOP && !(this->getVelocity().y > 0 && this->isInStatus(eStatus::JUMPING)))
			{
				//vận tốc 200 hướng xuống => cho trường hợp nhảy từ dưới lên
				//xử lý đặc biệt,collision body update position bt ko được
				//khi vào đây mới update position cho nó
				float moveX, moveY;
				if (collisionBody->isCollidingIntersected(otherObject, moveX, moveY, dt))
				{
					collisionBody->updateTargetPosition(otherObject, direction, false, GVector2(moveX, moveY));
				}
			/*	if (_preObject != nullptr && _preObject->getId()== eID::STAIR)
				{
					setPositionInLand((Land*)otherObject);
				}*/
				//Gán trạng thái gravity thành shallow vector trọng lực bằng 0,vật ko rớt xuống mà 
				//chỉ di chuyển sang ngang hoặc nhảy lên
				enableGravity(false);
				//Nếu va chạm xong thì hàm standing sẽ bỏ hết 2 trạng thái jump và fall của đối tượng
				//Để xét va chạm lần tiếp theo
				//if (otherObjectID == eID::STAIR && _preObject->getId() == eID::STAIR) return 0.0f;
				this->standing();

				_preObject = otherObject;
			}
			//Va chạm với wall
			else if (((direction == eDirection::LEFT) 
				|| (direction == eDirection::RIGHT))
				&& (!this->isInStatus(eStatus::JUMPING) || (this->isInStatus(eStatus::JUMPING) && !_canJumpDown)))
			{
				//this->setStatus(eStatus::NORMAL);
				//vì khi có va chạm thì vật vẫn còn di chuyển
				//nên cần dùng hàm dưới để cập nhật lại vị trí khi simon giao với wall(land với hướng va chạm trái phải)
				float moveX, moveY;
				if (collisionBody->isCollidingIntersected(otherObject, moveX, moveY, dt))
				{
					collisionBody->updateTargetPosition(otherObject, direction, false, GVector2(moveX, moveY));
				}
				if (!this->isInStatus(eStatus::JUMPING)) 
				{
					auto move = (Movement*)this->_componentList["Movement"];
					move->setVelocity(GVector2(0, this->getVelocity().y));
				}
				else
				{
					auto move = (Movement*)this->_componentList["Movement"];
					move->setVelocity(GVector2(0,-400));
				}
				//enableGravity(false);
				//_preObject = otherObject;
			}
		}
		else if (_preObject == otherObject)
		{
			// kiểm tra coi nhảy hết qua cái land cũ chưa
			// để gọi event end.
			//collisionBody->checkCollision(otherObject, dt, false);
			if (_preObject->getId() == eID::STAIR) return 0.0f;
			_preObject = nullptr;
			_trackedFlyLand = nullptr;

			//Nếu vật đi hết land cũ
			//thì gán gravity lại thành falling
			//Vì simon chỉ check collion kkhi nằm trong 1 trong 2 trạng thái,nên ra khỏi lane sẽ falling
			enableGravity(true);
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

	if (_currentAnimationIndex & eStatus::PICKUPITEM == eStatus::PICKUPITEM)
		_currentAnimationIndex = (eStatus)(_currentAnimationIndex & ~eStatus::PICKUPITEM);
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


	//Đang lên cầu thang và xuống cầu thang
	if ((_currentAnimationIndex & eStatus::UPSTAIR) == eStatus::UPSTAIR/* && _canOnStair*/)
	{
		_currentAnimationIndex = eStatus::UPSTAIR;
		return;
	}
	if ((_currentAnimationIndex & eStatus::DOWNSTAIR) == eStatus::DOWNSTAIR)
	{
		_currentAnimationIndex = eStatus::DOWNSTAIR;
		return;
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
	//Phần animation cho hitting
	if (this->isInStatus(eStatus::HITTING))
	{
		if (this->isInStatus(eStatus::STANDINGONSTAIR_UP))
		{
			_currentAnimationIndex = eStatus(eStatus::HITTING | eStatus::STANDINGONSTAIR_UP);
		}
		else if (this->isInStatus(eStatus::STANDINGONSTAIR_DOWN))
		{
			_currentAnimationIndex = eStatus(eStatus::HITTING | eStatus::STANDINGONSTAIR_DOWN);
		}
		else if (this->isInStatus(eStatus::SITTING))
		{
			_currentAnimationIndex = eStatus(eStatus::HITTING | eStatus::SITTING);
		}
		else 
		{
			_currentAnimationIndex = eStatus::HITTING;
		}

		/*if (_whipLevel == 1)
		{
			_currentAnimationIndex = eStatus(_currentAnimationIndex | eStatus::LEVEL1);
		}*/

		if (!_isHitting)
		{
			_animations[_currentAnimationIndex]->restart();
			//_whip->restart();
			_isHitting = true;
		}
	}
	//////////////////////////////////////////////////
	//Phần animation cho throwing
	if (this->isInStatus(eStatus::THROWING_ITEM))
	{
		if (this->isInStatus(eStatus::STANDINGONSTAIR_UP))
		{
			_currentAnimationIndex = eStatus(eStatus::THROWING_ITEM | eStatus::STANDINGONSTAIR_UP);
		}
		else if (this->isInStatus(eStatus::STANDINGONSTAIR_DOWN))
		{
			_currentAnimationIndex = eStatus(eStatus::THROWING_ITEM | eStatus::STANDINGONSTAIR_DOWN);
		}
		else
		{
			_currentAnimationIndex = eStatus::THROWING_ITEM;
		}

		if (!_isThrowing)
		{
			_animations[_currentAnimationIndex]->restart();
			//_whip->restart();
			_isThrowing = true;
		}
	}

	//=====================================================================================//
	//Chỉ animation cho trạng thái đứng yên trên cầu thang
	if ((_currentAnimationIndex & eStatus::STANDINGONSTAIR) == eStatus::STANDINGONSTAIR 
		&& (_currentAnimationIndex & eStatus::THROWING_ITEM) != eStatus::THROWING_ITEM
		&& (_currentAnimationIndex & eStatus::HITTING) != eStatus::HITTING)
	{
		if ((_currentAnimationIndex & eStatus::STANDINGONSTAIR_UP) == eStatus::STANDINGONSTAIR_UP)
		{
			_currentAnimationIndex = eStatus::STANDINGONSTAIR_UP;
		}
		else
			_currentAnimationIndex = eStatus::STANDINGONSTAIR_DOWN;
	}

	if (_isHitted)
	{
		_currentAnimationIndex = eStatus::STANDINGONSTAIR_DOWN;
	}

	if (this->isInStatus(eStatus::DYING))
	{
		_currentAnimationIndex = eStatus::DYING;
	}

	if (_isProtected)
	{

		_animations[_currentAnimationIndex]->enableFlashes(true);
		_animations[_currentAnimationIndex]->setColorFlash(D3DXCOLOR(1.0f, 0.5f, 0.5f, 1));
	}
	else
	{
		_animations[_currentAnimationIndex]->enableFlashes(false);
		_animations[_currentAnimationIndex]->setColorFlash(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1));
	}
		
}

eDirection Simon::getDirection()
{
	if (this->getScale().x > 0)
		return eDirection::RIGHT;
	else return eDirection::LEFT;
}

void Simon::getWeapon()
{
	eItemID _itemID = ActiveWeapon::getItemID();

	if (ActiveWeapon::getItemID() == eItemID::NOITEM) return;

	Item* item = nullptr;

	//Simon* simon = ((PlayScene*)SceneManager::getInstance()->getCurrentScene())->getSimon();

	GVector2 startPos = this->getPosition();

	eDirection dir = this->getDirection();

	ItemManager::generateWeapon(_itemID, startPos,dir, eItemType::PICKED_UP);
}

//int Simon::getDamage()
//{
//	return 6;
//}

void Simon::setPositionInStair(Stair* stair)
{
	auto stairPos = stair->getPosition(); 
	if(this->isInStatus(eStatus::UPSTAIR))
	{
		auto newPos = GVector2(stairPos.x , stairPos.y + 20);
		this->setPosition(newPos);
	}
	if (this->isInStatus(eStatus::DOWNSTAIR))
	{
		auto newPos = GVector2(stairPos.x - 10, stairPos.y);
		this->setPosition(newPos);
	}
}

void Simon::setPositionInLand(Land* land)
{
	auto landPos = land->getPosition();
	auto newPos = GVector2(this->getPositionX(),this->getPositionY() + 30);
	this->setPosition(newPos);
}

BaseObject* Simon::getWhip()
{
	//if (!this->isInStatus(eStatus::HITTING))
		_whip->restart();
	return this->_whip;
	
}

void Simon::forceMoveRight() {
	//onKeyPressed(new KeyEventArg(DIK_RIGHT));
	if (!this->isInStatus(eStatus::STANDINGONSTAIR) || !this->isInStatus(eStatus::HITTING))
	{
		this->removeStatus(eStatus::MOVING_LEFT);
		this->removeStatus(eStatus::SITTING);
		this->addStatus(eStatus::MOVING_RIGHT);
	}
}
void Simon::unforceMoveRight() {
	//onKeyReleased(new KeyEventArg(DIK_RIGHT));
	this->removeStatus(eStatus::MOVING_RIGHT);
}
void Simon::forceMoveLeft() {
	/*onKeyPressed(new KeyEventArg(DIK_LEFT));*/
	if (!this->isInStatus(eStatus::STANDINGONSTAIR) || !this->isInStatus(eStatus::HITTING))
	{
		this->removeStatus(eStatus::MOVING_RIGHT);
		this->removeStatus(eStatus::SITTING);
		this->addStatus(eStatus::MOVING_LEFT);
	}
}
void Simon::unforceMoveLeft() {
	//onKeyReleased(new KeyEventArg(DIK_LEFT));
	this->removeStatus(eStatus::MOVING_LEFT);
}
void Simon::forceJump() {
	onKeyPressed(new KeyEventArg(DIK_X));
}
void Simon::unforceJump() {
	//gán vector move với move speed âm vì đi ngược trục x
	auto move = (Movement*)this->_componentList["Movement"];
	move->setVelocity(GVector2(0,-300));
}
void Simon::removeGravity() {
	auto graivity = (Gravity*)(this->_componentList.find("Gravity")->second);
	graivity->setGravity(GVector2Zero);
}

void Simon::setFreeze(bool freeze)
{
	if (_isFreezed != freeze)
		_isFreezed = freeze;
}