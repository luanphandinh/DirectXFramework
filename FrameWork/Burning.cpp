

#include "Burning.h"
/*
 * Burning dùng để tạo vụ nổ, đám cháy khi nhân vật hoặc đối tượng chết.
 * hiện tại có 2 kiểu nổ. (xem hình resource)
 * lớp nào chứa Burning nhớ quản lý huỷ đối tương này
*/
Burning::Burning(int type) : BaseObject(eID::BURNING) {
	this->_type = type;
}

Burning::~Burning() {}


void Burning::init() {
	this->_sprite = SpriteManager::getInstance()->getSprite(eID::BURNING);

	this->_animation = new Animation(this->_sprite, SPEED_ANIMATION);
	if (this->_type == 1) {
		this->_animation->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::BURNING, "explode"));
	}
	else {
		this->_animation->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::BURNING, "burn_01"));
		this->_animation->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::BURNING, "burn_02"));
		this->_animation->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::BURNING, "burn_03"));
		
	}
}

void Burning::update(float deltatime) {
	if (this->getStatus() != eStatus::NORMAL)
		return;
	this->_animation->update(deltatime);
	if (this->_type == 1 && this->_animation->getIndex() == 0) {
		this->setStatus(eStatus::DESTROY);
	}

	if (this->_type == 2 && this->_animation->getIndex() == 2) {
		this->setStatus(eStatus::DESTROY);
	}
}

void Burning::setPosition(GVector2 position) {
	this->_sprite->setPosition(position);
}

void Burning::setTimeAnimated(float time) {
	this->_animation->setTimeAnimate(time);
}

void Burning::draw(LPD3DXSPRITE spriteHandle, Viewport* viewport) {
	if (this->getStatus() != eStatus::NORMAL)
		return;
	this->_animation->draw(spriteHandle, viewport);
}
void Burning::release() {
	SAFE_DELETE(this->_sprite);
	SAFE_DELETE(_animation);
}
