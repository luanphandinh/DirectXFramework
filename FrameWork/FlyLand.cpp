#include "FlyLand.h"
#include "PlayScene.h"
FlyLand::FlyLand(GVector2 leftPos, GVector2 rightPos) : BaseObject(eID::FLYLAND) {
	this->_leftPosition = leftPos;
	this->_rightPosition = rightPos;
}

FlyLand::~FlyLand() {}

void FlyLand::init() {
	this->_sprite = SpriteManager::getInstance()->getSprite(eID::FLYLAND);
	this->_sprite->setScale(SCALE_FACTOR);
	this->_sprite->setPosition(_rightPosition);
	this->_sprite->setFrameRect(SpriteManager::getInstance()->getSourceRect(eID::FLYLAND, "normal"));
	_sprite->setOrigin(GVector2(0.5f, 1.0f));

	Movement* movement = new Movement(GVector2Zero, FLYLAND_HORIZONTAL_VELOC, _sprite);
	CollisionBody* collisionBody = new CollisionBody(this);
	this->_listComponent["Movement"] = movement;
	this->_listComponent["CollisionBody"] = collisionBody;
	this->setStatus(NORMAL);
	this->setPhysicBodySide(eDirection::TOP);
}

void FlyLand::update(float deltaTime) {
	if (this->getStatus() == eStatus::DESTROY)
		return;

	this->checkPosition();
	if (this->getStatus() == eStatus::WAITING) {
		if (true) {
			auto simon = ((PlayScene*)SceneManager::getInstance()->getCurrentScene())->getSimon();

			simon->setPositionX(this->getPositionX());
			if (simon->isInStatus(MOVING_LEFT) || simon->isInStatus(MOVING_RIGHT))
				this->setStatus(RUNNING);
		}
	}

	GVector2 position = this->getPosition();
	if (position.x < _leftPosition.x) {
		auto move = (Movement*) this->_listComponent["Movement"];
		move->setVelocity(FLYLAND_HORIZONTAL_VELOC);
	}

	if (position.x > _rightPosition.x) {
		auto move = (Movement*) this->_listComponent["Movement"];
		move->setVelocity(FLYLAND_HORIZONTAL_VELOC_PRE);
	}

	for (auto component : _listComponent) {
		component.second->update(deltaTime);
	}


}

void FlyLand::draw(LPD3DXSPRITE spriteHandler, Viewport *viewport) {
	if (this->getStatus() == eStatus::DESTROY)
		return;
	_sprite->render(spriteHandler, viewport);
}

void FlyLand::release() {
	for (auto component : _listComponent) {
		delete component.second;
	}
	_listComponent.clear();

	SAFE_DELETE(this->_sprite);
}

GVector2 FlyLand::getVelocity() {
	auto move = (Movement*)this->_listComponent["Movement"];
	return move->getVelocity();
}

IComponent * FlyLand::getComponent(string componentName) {
	auto it = _listComponent.find(componentName);
	if (it == _listComponent.end())
		throw new exception("key not match");
	return it->second;
}

//RECT FlyLand::getBounding() {
//	return RECT();
//}

void FlyLand::checkPosition() {
	//auto viewport = ((PlayScene*)SceneManager::getInstance()->getCurrentScene())->getViewport();
	//auto simon = ((PlayScene*)SceneManager::getInstance()->getCurrentScene())->getSimon();
	//RECT screenBound = viewport->getBounding();
	//GVector2 position = this->getPosition();
	//GVector2 viewportposition = viewport->getPositionWorld();
	////if (position.x > screenBound.right || BaseObject::getBounding().top < viewportposition.y - WINDOW_HEIGHT) {
	////	this->setStatus(eStatus::DESTROY);

	////}
	//if (isRectangleIntersectedInDescartes(simon->getBounding(), this->getBounding())) {
	//	// Nếu giao nhau
	//	if (position.y < simon->getPositionY()) {
	//		//Nếu y của land nhỏ hơn y của simon ->setwait
	//		this->setStatus(eStatus::WAITING);

	//	}
	//	if (simon->isInStatus(MOVING_LEFT) || simon->isInStatus(MOVING_RIGHT) ||
	//		simon->isInStatus(eStatus::JUMPING) || simon->isInStatus(eStatus::FALLING)) {
	//		this->setStatus(NORMAL);

	//	}

	//}
	//else {
	//	if (this->getStatus() != eStatus::DESTROY) {
	//		this->setStatus(NORMAL);

	//	}

	//}
}
