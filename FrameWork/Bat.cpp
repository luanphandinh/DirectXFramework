#include "Bat.h"
#include "PlayScene.h"
Bat::Bat(eStatus status, GVector2 pos, int direction) : BaseEnemy(eID::BAT) {
	_sprite = SpriteManager::getInstance()->getSprite(eID::BAT);
	_sprite->setFrameRect(0, 0, 32.0f, 16.0f);

	GVector2 v(direction * BAT_SPEED, 0);
	GVector2 a(0, 0);
	this->_listComponent.insert(pair<string, IComponent*>("Movement", new Movement(a, v, this->_sprite)));
	this->setStatus(status);
	this->setPosition(pos);
	this->setScale(SCALE_FACTOR);
	this->setScaleX(direction * SCALE_FACTOR);
}

Bat::Bat(eStatus status, float x, float y, int direction) :
	BaseEnemy(eID::BAT) {

	_sprite = SpriteManager::getInstance()->getSprite(eID::BAT);
	//_sprite->setFrameRect(0, 0, 32.0f, 16.0f);
	_sprite->setFrameRect(0, 0, 32.0f, 32.0f);

	GVector2 pos(x, y);
	GVector2 v(direction * BAT_SPEED, 0);
	GVector2 a(0, 0);
	this->_listComponent.insert(pair<string, IComponent*>("Movement",
		new Movement(a, v, this->_sprite)));

	this->setStatus(status);
	this->setPosition(pos);
	this->setScale(SCALE_FACTOR);
	this->setScaleX(direction * SCALE_FACTOR);
}


Bat::Bat(eStatus status, int direction, GVector2 pos, GVector2 hVeloc, GVector2 ampl, float freq) :
	BaseEnemy(eID::BAT) {

	_sprite = SpriteManager::getInstance()->getSprite(eID::BAT);
	_sprite->setFrameRect(0, 0, 32.0f, 16.0f);

	GVector2 v(direction * BAT_SPEED, 0);
	GVector2 a(0, 0);
	this->_listComponent.insert(pair<string, IComponent*>("Movement", new Movement(a, v, this->_sprite)));
	this->setStatus(status);
	this->setPosition(pos);
	this->setScale(SCALE_FACTOR);
	this->setScaleX(direction * SCALE_FACTOR);
}

Bat::~Bat() {}

void Bat::init() {
	this->setHitpoint(BAT_HITPOINT);
	this->setScore(BAT_SCORE);
	this->_listComponent.insert(pair<string, IComponent*>("Gravity",
		new Gravity(GVector2(0, 0), (Movement*)(this->getComponent("Movement")))));

	auto collisionBody = new CollisionBody(this);
	_listComponent["CollisionBody"] = collisionBody;

	__hook(&CollisionBody::onCollisionBegin, collisionBody, &Bat::onCollisionBegin);
	__hook(&CollisionBody::onCollisionEnd, collisionBody, &Bat::onCollisionEnd);

	_animations[eStatus::HANGING] = new Animation(_sprite, 0.1f);
	_animations[eStatus::HANGING]->addFrameRect(eID::BAT, "normal", NULL);

	_animations[FLYINGDOWN] = new Animation(_sprite, 0.15f);
	_animations[FLYINGDOWN]->addFrameRect(eID::BAT, "fly_02", "fly_03", "fly_04", NULL);

	_animations[FLYING] = new Animation(_sprite, 0.15f);
	_animations[FLYING]->addFrameRect(eID::BAT,  "fly_02", "fly_03", "fly_04", NULL);

	_animations[DYING] = new Animation(_sprite, 0.15f);
	_animations[DYING]->addFrameRect(eID::BAT, NULL);


	//*Test
	//this->setPosition(GVector2(300, 200));
	this->setStatus(eStatus::HANGING);
	_sprite->drawBounding(false);

	this->hack = 0;
}

void Bat::draw(LPD3DXSPRITE spritehandle, Viewport* viewport) {

	_animations[this->getStatus()]->draw(spritehandle, viewport);

}

void Bat::release() {
	for (auto component : _listComponent) {
		delete component.second;
	}
	_listComponent.clear();

	//SAFE_DELETE(this->_loopwatch);
	SAFE_DELETE(this->_sprite);
}

IComponent* Bat::getComponent(string componentName) {
	return _listComponent.find(componentName)->second;
}

void Bat::update(float deltaTime) {

	if (this->getStatus() == eStatus::DESTROY)
		return;
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
		/*this->checkIfOutOfScreen();*/
		for (auto component : _listComponent) {
			component.second->update(deltaTime);
		}
		_animations[this->getStatus()]->update(deltaTime);
	}
}

void Bat::setPosition(GVector2 pos) {
	_sprite->setPosition(pos);
}


void Bat::changeDirection() {
	_sprite->setScaleX(-this->getScale().x);
	Movement *movement = (Movement*)this->getComponent("Movement");
	movement->setVelocity(GVector2(-movement->getVelocity().x, 0));
}

void Bat::flyingDown() {
	Movement *movement = (Movement*)this->getComponent("Movement");
	movement->setVelocity(GVector2(movement->getVelocity().x, -50));
}

void Bat::fly() {
	Movement *movement = (Movement*)this->getComponent("Movement");
	movement->setVelocity(GVector2(movement->getVelocity().x, 0));
}

void Bat::updateHanging() {
	// track theo simon
	auto objectTracker = ((PlayScene*)SceneManager::getInstance()->getCurrentScene())->getSimon();
	RECT objectBound = objectTracker->getBounding();
	int x = objectTracker->getPositionX();
	int y = objectTracker->getPositionY();
	int xthis = this->getPositionX();
	int ythis = this->getBounding().bottom;
	if (x > xthis&&x<xthis + 250 && y<ythis&&y>ythis-100) {
		this->setStatus(FLYINGDOWN);
	}
	else {
		this->setStatus(HANGING);
	}

}

void Bat::onCollisionBegin(CollisionEventArg* collision_event) {
	eID objectID = collision_event->_otherObject->getId();
	switch (objectID) {
	case eID::SIMON:
	{
		if (collision_event->_otherObject->isInStatus(eStatus::DYING) == false) {
			collision_event->_otherObject->setStatus(eStatus::DYING);
			//* need to add die() for simon
			//((Simon*)collision_event->_otherObject)->die();
		}
		break;
	}
	default:
		break;
	}
}

void Bat::onCollisionEnd(CollisionEventArg* collision_event) {
	if (this->getStatus() == eStatus::DESTROY)
		return;
	eID objectID = collision_event->_otherObject->getId();
	switch (objectID) {
	case eID::LAND:
	default:
		break;
	}
}


float Bat::checkCollision(BaseObject * object, float dt) {
	if (this->getStatus() == eStatus::DESTROY ||
		this->isInStatus(eStatus::DYING))
		return 0.0f;

	auto collisionBody = (CollisionBody*)_listComponent["CollisionBody"];
	eID objectId = object->getId();
	eDirection direction;

	/*if (objectId == eID::LAND) {
		if (collisionBody->checkCollision(object, direction, dt)) {
			if (direction == eDirection::TOP && this->getVelocity().y < 0) {
				auto gravity = (Gravity*)this->_listComponent["Gravity"];
				auto movement = (Movement*)this->_listComponent["Movement"];
				movement->setVelocity(GVector2(movement->getVelocity().x, 0));
				gravity->setStatus(eGravityStatus::SHALLOWED);

				this->setStatus(eStatus::WALKING);
				prevObject = object;
			}
		}
		else if (prevObject == object) {
			auto gravity = (Gravity*)this->_listComponent["Gravity"];
			gravity->setStatus(eGravityStatus::FALLING_DOWN);

			prevObject = nullptr;
		}
	}
	else {

		collisionBody->checkCollision(object, dt, false);

	}*/
	collisionBody->checkCollision(object, dt, false);

	return 0.0f;

}

GVector2 Bat::getVelocity() {
	auto move = (Movement*)this->_listComponent["Movement"];
	return move->getVelocity();
}

void Bat::die() {
	Gravity *gravity = (Gravity*)this->getComponent("Gravity");
	gravity->setStatus(eGravityStatus::SHALLOWED);
	Movement *movement = (Movement*)this->getComponent("Movement");
	movement->setVelocity(GVector2(0, 200));
}

