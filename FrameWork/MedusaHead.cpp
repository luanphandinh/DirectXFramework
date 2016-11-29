#include "MedusaHead.h"
#include "PlayScene.h"
MedusaHead::MedusaHead(eStatus status, int direction, GVector2 pos, 
	GVector2 hVeloc, GVector2 ampl, float freq) : BaseEnemy(eID::BAT) {
	this->_beginPosition = pos;
	this->_horizontalVeloc = hVeloc;
	this->_amplitude = ampl;
	this->_frequence = freq;

	_sprite = SpriteManager::getInstance()->getSprite(eID::MEDUSAHEAD);
	_sprite->setFrameRect(SpriteManager::getInstance()->getSourceRect(this->_id, "normal"));

	this->_listComponent.insert(pair<string, IComponent*>("Movement",
		new Movement(GVector2Zero, _horizontalVeloc, _sprite)));
	this->_listComponent.insert(pair<string, IComponent*>("Sinmovement", 
		new SinMovement(_amplitude, _frequence, _sprite)));
	
	this->setStatus(status);
	this->setPosition(pos);
	this->setScale(SCALE_FACTOR);
	this->setScaleX(direction * SCALE_FACTOR);
}

MedusaHead::~MedusaHead() {}

void MedusaHead::init() {
	this->setHitpoint(MEDUSAHEAD_HITPOINT);
	this->setScore(MEDUSAHEAD_SCORE);

	auto collisionBody = new CollisionBody(this);
	_listComponent["CollisionBody"] = collisionBody;

	__hook(&CollisionBody::onCollisionBegin, collisionBody, &MedusaHead::onCollisionBegin);
	__hook(&CollisionBody::onCollisionEnd, collisionBody, &MedusaHead::onCollisionEnd);

	_animations[eStatus::HIDING] = new Animation(_sprite, 0.1f);
	_animations[eStatus::HIDING]->addFrameRect(eID::MEDUSAHEAD, "normal", NULL);

	_animations[FLYING] = new Animation(_sprite, 0.15f);
	_animations[FLYING]->addFrameRect(eID::MEDUSAHEAD, "fly_01", "fly_02", NULL);

	_animations[DYING] = new Animation(_sprite, 0.15f);
	_animations[DYING]->addFrameRect(eID::MEDUSAHEAD, NULL);

	//*Test
	//this->setPosition(GVector2(300, 200));
	this->setStatus(eStatus::HIDING);
	_sprite->drawBounding(false);
}

void MedusaHead::update(float deltaTime) {

	if (this->getStatus() == eStatus::DESTROY)
		return;
	if (this->getStatus() == eStatus::HIDING) {
		this->updateHiding();
		return;
	}
	else {

		/*this->checkIfOutOfScreen();*/
		for (auto component : _listComponent) {

			component.second->update(deltaTime);
		}
		_animations[this->getStatus()]->update(deltaTime);

	}

}

void MedusaHead::draw(LPD3DXSPRITE spriteHandler, Viewport* viewport) {
	_animations[this->getStatus()]->draw(spriteHandler, viewport);

}

void MedusaHead::release() {
	for (auto component : _listComponent) {
		delete component.second;
	}
	_listComponent.clear();

	SAFE_DELETE(this->_sprite);
}

void MedusaHead::onCollisionBegin(CollisionEventArg *) {

}

void MedusaHead::onCollisionEnd(CollisionEventArg *) {

}

float MedusaHead::checkCollision(BaseObject *, float) {
	return 0.0f;
}

void MedusaHead::die() {

}

void MedusaHead::setPosition(GVector2 pos) {
	_sprite->setPosition(pos);

}

IComponent * MedusaHead::getComponent(string) {
	return nullptr;
}

void MedusaHead::checkIfOutOfScreen() {
	if (this->getStatus() != eStatus::NORMAL)
		return;
	auto viewport = ((PlayScene*)SceneManager::getInstance()->getCurrentScene())->getViewport();
	RECT screenBound = viewport->getBounding();
	GVector2 position = this->getPosition();

	if (position.x > screenBound.right) {
		this->setStatus(eStatus::FLYING);
	}
}

void MedusaHead::updateHiding() {
	auto viewport = ((PlayScene*)SceneManager::getInstance()->getCurrentScene())->getViewport();
	RECT screenBound = viewport->getBounding();

	if (this->getPositionY() < 0) {
		this->setStatus(eStatus::HIDING);
	}
	else {
		// cạnh trái con medu < cạnh phải view-->bay
		if (this->getBounding().left< screenBound.right) {
			this->setStatus(eStatus::FLYING);
		}
	}

}

