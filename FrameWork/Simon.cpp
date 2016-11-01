#include "Simon.h"
_USING_FRAMEWORK

Simon::Simon() :BaseObject(eID::SIMON)
{
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
	//_test_sprite->drawBounding(true);
	//_test_sprite->setPosition(50, 50, 1.0f);
	_animations[eStatus::NORMAL] = new Animation(_sprite, 0.1f);
	_animations[eStatus::NORMAL]->addFrameRect(eID::SIMON, "normal", NULL);

	_animations[eStatus::RUNNING] = new Animation(_sprite, 0.12f);
	_animations[eStatus::RUNNING]->addFrameRect(eID::SIMON, "run_01", "run_02", "run_03", NULL);
	
	
	_animations[eStatus::JUMPING] = new Animation(_sprite, 0.1f);
	_animations[eStatus::JUMPING]->addFrameRect(eID::SIMON, "jump", NULL);

	_animations[eStatus::SITTING] = new Animation(_sprite, 0.1f);
	_animations[eStatus::SITTING]->addFrameRect(eID::SIMON, "sit", NULL);

	_animations[eStatus::HITTING] = new Animation(_sprite, 0.2f);
	_animations[eStatus::HITTING]->addFrameRect(eID::SIMON, "whip_normal_01", "whip_normal_02", "whip_normal_03", NULL);
	this->_movingSpeed = 3.0f;

	this->setPosition(500, 90);
	this->_sprite->drawBounding(true);
	this->setStatus(eStatus::NORMAL);
}

void Simon::update(float deltatime)
{
	//this->checkPosition();

	this->updateStatus(deltatime);

	this->updateCurrentAnimateIndex();

	_animations[_currentAnimationIndex]->update(deltatime);

}

void Simon::draw(LPD3DXSPRITE spriteHandle, Viewport* viewport)
{
	_animations[_currentAnimationIndex]->draw(spriteHandle, viewport);
}

void Simon::release(){}


/*
Event for event_reciever
*/
void Simon::onKeyPressed(KeyEventArg* key_event)
{
	switch (key_event->_key)
	{
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
	case DIK_C:
		this->removeStatus(eStatus::HITTING);
	default:
		break;
	}
}

void Simon::setStatus(eStatus status) 
{
	_status = status;
}

// Character action.
void Simon::standing(){}

float Simon::getMovingSpeed()
{
	return _movingSpeed;
}

RECT Simon::getBounding() 
{
	return this->getBounding();
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
}

void Simon::moveLeft()
{
	if (this->getScale().x > 0)
		this->setScaleX(this->getScale().x * (-1));
	this->setPositionX(this->getPositionX() - _movingSpeed);
}

void Simon::moveRight()
{
	if (this->getScale().x < 0)
		this->setScaleX(this->getScale().x * (-1));
	this->setPositionX(this->getPositionX() + _movingSpeed);
}


void  Simon::updateCurrentAnimateIndex()
{
	_currentAnimationIndex = this->getStatus();
	if (isInStatus(eStatus::HITTING) && //nếu đang vung roi
		//mà di chuyển
		isInStatus(eStatus::MOVING_LEFT) || isInStatus(eStatus::MOVING_RIGHT))
	{
		//bỏ animation vung roi đi ,chỉ di chuyển thôi
		_currentAnimationIndex = (eStatus)(this->getStatus() & ~eStatus::HITTING);
	}
	else 
	if ((_currentAnimationIndex & eStatus::HITTING) == eStatus::HITTING)
	{
		_currentAnimationIndex = this->getStatus();
	}


	if ((_currentAnimationIndex & eStatus::MOVING_LEFT) == eStatus::MOVING_LEFT 
		|| ((_currentAnimationIndex & eStatus::MOVING_RIGHT) == eStatus::MOVING_RIGHT)) {
		// animate move left/right xài cung 1 animate là running nên cũng bỏ nó ra
		_currentAnimationIndex = (eStatus)(_currentAnimationIndex & ~(eStatus::MOVING_LEFT | MOVING_RIGHT));
		_currentAnimationIndex = (eStatus)(_currentAnimationIndex | eStatus::RUNNING);
	}	

	
	if ((_currentAnimationIndex & eStatus::SITTING) == eStatus::SITTING)
	{
		_currentAnimationIndex = (eStatus)(_currentAnimationIndex | eStatus::SITTING);
	}
}