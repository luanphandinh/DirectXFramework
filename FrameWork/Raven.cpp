#include "Raven.h"

Raven::Raven(eStatus status, GVector2 pos, int direction) : BaseEnemy(eID::RAVEN) {
	_sprite = SpriteManager::getInstance()->getSprite(eID::RAVEN);
	_sprite->setFrameRect(0, 0, 32.0f, 16.0f);

	GVector2 v(direction * RAVEN_SPEED, 0);
	GVector2 a(0, 0);
	this->_listComponent.insert(pair<string, IComponent*>("Movement", new Movement(a, v, this->_sprite)));
	this->setStatus(status);
	this->setPosition(pos);
	this->setScale(SCALE_FACTOR);
	this->setScaleX(direction * SCALE_FACTOR);
}

Raven::~Raven() {
}

void Raven::init() {
	this->setHitpoint(RAVEN_HITPOINT);
	this->setScore(RAVEN_SCORE);
	this->_listComponent.insert(pair<string, IComponent*>("Gravity",
		new Gravity(GVector2(0, 0), (Movement*)(this->getComponent("Movement")))));

	auto collisionBody = new CollisionBody(this);
	_listComponent["CollisionBody"] = collisionBody;


	_animations[eStatus::HANGING] = new Animation(_sprite, 0.1f);
	_animations[eStatus::HANGING]->addFrameRect(eID::RAVEN, "normal", NULL);

	_animations[FLYINGDOWN] = new Animation(_sprite, 0.15f);
	_animations[FLYINGDOWN]->addFrameRect(eID::RAVEN, "fly_01", "fly_02", "fly_03", NULL);

	_animations[FLYING] = new Animation(_sprite, 0.15f);
	_animations[FLYING]->addFrameRect(eID::RAVEN, "fly_01", "fly_02", "fly_03",NULL);

	_animations[DYING] = new Animation(_sprite, 0.15f);
	_animations[DYING]->addFrameRect(eID::RAVEN, NULL);

	_stopWatch = new StopWatch();
	//*Test
	//this->setPosition(GVector2(300, 200));
	this->setStatus(eStatus::HANGING);
	_sprite->drawBounding(false);

	this->hack = 0;
	this->setPhysicBodySide(eDirection::ALL);
	this->setHitpoint(1);
}

void Raven::update(float deltaTime) {
	if (this->getStatus() == eStatus::DESTROY)
		return;

	if (this->getHitpoint() <= 0) {
		this->setStatus(eStatus::BURN);
	}

	// Bị nướng
	if (this->getStatus() == eStatus::BURN) {
		if (_burning == nullptr) {
			auto pos = this->getPosition();
			_burning = new HitEffect(2, pos);
			_burning->init();
		}
		else {
			_burning->update(deltaTime);
			if (_burning->getStatus() == eStatus::DESTROY) {
				this->setStatus(eStatus::DESTROY);
			}
		}
		return;
	}

	if (this->checkIfOutOfScreen()) return;
	if (this->getStatus() == eStatus::HANGING) {
		this->updateHanging();
		return;
	}
	else {
		if (hack == 30) {
			this->setStatus(FLYING);
			this->fly();
		}
		if (this->getStatus() == FLYINGDOWN) {
			hack++;
			this->flyingDown();
		}

		for (auto component : _listComponent) {
			component.second->update(deltaTime);
		}
		_animations[this->getStatus()]->update(deltaTime);
	}
}

void Raven::draw(LPD3DXSPRITE spritehandle, Viewport *viewport) {
	if (_burning != nullptr)
		_burning->draw(spritehandle, viewport);
	if (this->isInStatus(eStatus::DESTROY) || this->isInStatus(eStatus::BURN)) return;
	_animations[this->getStatus()]->draw(spritehandle, viewport);

}

void Raven::release() {
	for (auto component : _listComponent) {
		delete component.second;
	}
	_listComponent.clear();
	if (_burning != nullptr)
		_burning->release();
	//SAFE_DELETE(this->_loopwatch);
	SAFE_DELETE(this->_sprite);
}

float Raven::checkCollision(BaseObject *, float) {
	return 0.0f;
}

void Raven::die() {

}

void Raven::setPosition(GVector2 pos) {
	_sprite->setPosition(pos);

}

GVector2 Raven::getVelocity() {
	auto move = (Movement*)this->_listComponent["Movement"];
	return move->getVelocity();
}

IComponent * Raven::getComponent(string componentName) {
	return _listComponent.find(componentName)->second;
}

void Raven::changeDirection() {

}

void Raven::flyingDown() {

}

void Raven::fly() {

}

bool Raven::checkIfOutOfScreen() {
	return false;
}

void Raven::updateHanging() {

}
