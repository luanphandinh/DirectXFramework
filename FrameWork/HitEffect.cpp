

#include "HitEffect.h"
/*
 * HitEffect dùng để tạo vụ nổ, đám cháy khi nhân vật hoặc đối tượng chết.
 * hiện tại có 2 kiểu nổ. (xem hình resource)
 * lớp nào chứa HitEffect nhớ quản lý huỷ đối tương này
*/
HitEffect::HitEffect(int type,GVector2 pos) : BaseObject(eID::HITEFFECT) {
	this->_type = type;
	this->_sprite = SpriteManager::getInstance()->getSprite(eID::HITEFFECT);
	this->setPosition(pos);
}

HitEffect::~HitEffect() {}


void HitEffect::init() {
	this->setScale(SCALE_FACTOR);

	this->setStatus(eStatus::NORMAL);

	this->_animation = new Animation(this->_sprite, SPEED_ANIMATION);

	if (this->_type == 1) {
		this->_animation->addFrameRect(eID::HITEFFECT, "explode", NULL);
	}
	else {
		this->_animation->addFrameRect(eID::HITEFFECT, "explode", "burn_01", "burn_02", "burn_03", NULL);
	}
}

void HitEffect::update(float deltatime) {
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

void HitEffect::setPosition(GVector2 position) {
	this->_sprite->setPosition(position);
}

void HitEffect::setTimeAnimated(float time) {
	this->_animation->setTimeAnimate(time);
}

void HitEffect::draw(LPD3DXSPRITE spriteHandle, Viewport* viewport) {
	if (this->getStatus() != eStatus::NORMAL)
		return;
	this->_animation->draw(spriteHandle, viewport);
}
void HitEffect::release() {
	SAFE_DELETE(this->_sprite);
	SAFE_DELETE(_animation);
}
