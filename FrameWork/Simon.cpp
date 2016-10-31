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
	_animations[eStatus::RUNNING] = new Animation(_sprite, 0.08f);
	_animations[eStatus::RUNNING]->addFrameRect(eID::SIMON, "run_01", "run_02", "run_03", NULL);
	_animations[eStatus::NORMAL] = new Animation(_sprite, 0.1f);
	_animations[eStatus::NORMAL]->addFrameRect(eID::SIMON, "normal", NULL);

	this->_movingSpeed = 3.0f;

	this->setPosition(30, 75);

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
	case DIK_D:
		this->removeStatus(eStatus::MOVING_RIGHT);
		this->addStatus(eStatus::MOVING_LEFT);
		break;
	case DIK_A:
		this->removeStatus(eStatus::MOVING_LEFT);
		this->addStatus(eStatus::MOVING_RIGHT);
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
	switch (key_event->_key)
	{
	case DIK_D:
		this->removeStatus(eStatus::MOVING_RIGHT);
		this->setStatus(eStatus::NORMAL);
		break;
	case DIK_A:
		this->removeStatus(eStatus::MOVING_LEFT);
		this->setStatus(eStatus::NORMAL);
		break;
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
	if ((this->getStatus() & eStatus::MOVING_LEFT) == eStatus::MOVING_LEFT) {
		this->moveLeft();
	}
	else
	if ((this->getStatus() & eStatus::MOVING_RIGHT) == eStatus::MOVING_RIGHT) {
		this->moveRight();
	}
}

void Simon::moveLeft()
{
	if (this->getScale().x < 0)
		this->setScaleX(this->getScale().x * (-1));
	this->setPositionX(this->getPositionX() + _movingSpeed);
}

void Simon::moveRight()
{
	if (this->getScale().x > 0)
		this->setScaleX(this->getScale().x * (-1));
	this->setPositionX(this->getPositionX() - _movingSpeed);
}

void  Simon::updateCurrentAnimateIndex()
{
	_currentAnimationIndex = this->getStatus();
	if ((_currentAnimationIndex & eStatus::MOVING_LEFT) == eStatus::MOVING_LEFT 
		|| ((_currentAnimationIndex & eStatus::MOVING_RIGHT) == eStatus::MOVING_RIGHT)) {
		// animate move left/right xài cung 1 animate là running nên cũng bỏ nó ra
		_currentAnimationIndex = (eStatus)(_currentAnimationIndex & ~(eStatus::MOVING_LEFT | MOVING_RIGHT));
		_currentAnimationIndex = (eStatus)(_currentAnimationIndex | eStatus::RUNNING);
	}	
}