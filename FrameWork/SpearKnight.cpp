#include "SpearKnight.h"
float delay = 250.0f;

SpearKnight::SpearKnight(eStatus status, GVector2 pos, int direction) : BaseEnemy(eID::SPEARKNIGHT) {
	_sprite = SpriteManager::getInstance()->getSprite(eID::SPEARKNIGHT);
	_sprite->setFrameRect(0, 0, 32.0f, 16.0f);

	GVector2 v(direction * SPEARKNIGHT_SPEED, 0);
	GVector2 a(0, 0);
	this->_listComponent.insert(pair<string, IComponent*>("Movement", new Movement(a, v, this->_sprite)));
	this->setStatus(status);
	this->setPosition(pos);
	this->setScale(SCALE_FACTOR);
	this->setScaleX(direction * SCALE_FACTOR);
}

SpearKnight::SpearKnight(eStatus status, float x, float y, int direction) : 
	BaseEnemy(eID::SPEARKNIGHT) {

	_sprite = SpriteManager::getInstance()->getSprite(eID::SPEARKNIGHT);
	//_sprite->setFrameRect(0, 0, 32.0f, 16.0f);
	_sprite->setFrameRect(0, 0, 17.0f, 32.0f);

	GVector2 pos(x, y);
	GVector2 v(direction * SPEARKNIGHT_SPEED, 0);
	GVector2 a(0, 0);
	this->_listComponent.insert(pair<string, IComponent*>("Movement", 
		new Movement(a, v, this->_sprite)));
	
	this->setStatus(status);
	this->setPosition(pos);
	this->setScale(SCALE_FACTOR);
	this->setScaleX(direction * SCALE_FACTOR);
}


SpearKnight::~SpearKnight() {}

void SpearKnight::init() {
	this->setHitpoint(SPEARKNIGHT_HITPOINT);
	this->setScore(SPEARKNIGHT_SCORE);

	this->_listComponent.insert(pair<string, IComponent*>("Gravity", 
		new Gravity(GVector2(0, -ENEMY_GRAVITY), (Movement*)(this->getComponent("Movement")))));

	auto collisionBody = new CollisionBody(this);
	_listComponent["CollisionBody"] = collisionBody;

	__hook(&CollisionBody::onCollisionBegin, collisionBody, &SpearKnight::onCollisionBegin);
	__hook(&CollisionBody::onCollisionEnd, collisionBody, &SpearKnight::onCollisionEnd);

	_animations[eStatus::NORMAL] = new Animation(_sprite, 0.1f);
	_animations[eStatus::NORMAL]->addFrameRect(eID::SPEARKNIGHT, "normal", NULL);

	_animations[WALKING] = new Animation(_sprite, 0.15f);
	_animations[WALKING]->addFrameRect(eID::SPEARKNIGHT, "walk_01", "walk_02", "walk_03", NULL);

	_animations[DYING] = new Animation(_sprite, 0.15f);
	_animations[DYING]->addFrameRect(eID::SPEARKNIGHT, NULL);

	_loopwatch = new StopWatch();

	//*Test
	//this->setPosition(GVector2(100,100));
	this->setStatus(eStatus::WALKING);
	_sprite->drawBounding(false);
}

void SpearKnight::draw(LPD3DXSPRITE spritehandle, Viewport* viewport) {

	_animations[this->getStatus()]->draw(spritehandle, viewport);

}

void SpearKnight::release() {
	for (auto component : _listComponent) {
		delete component.second;
	}
	_listComponent.clear();

	SAFE_DELETE(this->_loopwatch);
	SAFE_DELETE(this->_sprite);
}

IComponent* SpearKnight::getComponent(string componentName) {
	return _listComponent.find(componentName)->second;
}

void SpearKnight::update(float deltatime) {

	if (this->getStatus() == DESTROY )
		return;

	Gravity *gravity = (Gravity*)this->getComponent("Gravity");
	Movement *movement = (Movement*)this->getComponent("Movement");

	if (this->getHitpoint() <= 0 && this->getStatus() != DYING) {
		this->setStatus(eStatus::DYING);
		this->die();
	}

	if (this->getStatus() == eStatus::DYING) {
		// do nothing
	}

	for (auto it : _listComponent) {
		it.second->update(deltatime);
	}

	if (this->getStatus() != DESTROY) {
		_animations[this->getStatus()]->update(deltatime);


		if (_loopwatch->isTimeLoop(delay)) {
			
			this->changeDirection();
			delay = 1000.0f;
		}
	}
}

void SpearKnight::setPosition(GVector2 pos) {
	_sprite->setPosition(pos);
}
void SpearKnight::changeDirection() {
	_sprite->setScaleX(-this->getScale().x);
	Movement *movement = (Movement*)this->getComponent("Movement");
	movement->setVelocity(GVector2(-movement->getVelocity().x, 0));
}

void SpearKnight::onCollisionBegin(CollisionEventArg* collision_event) {
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

void SpearKnight::onCollisionEnd(CollisionEventArg* collision_event) {
	if (this->getStatus() == eStatus::DESTROY)
		return;
	eID objectID = collision_event->_otherObject->getId();
	switch (objectID) {
	case eID::LAND:
	default:
		break;
	}
}


float SpearKnight::checkCollision(BaseObject * object, float dt) {
	if (this->getStatus() == eStatus::DESTROY || 
		this->isInStatus(eStatus::DYING))
		return 0.0f;

	auto collisionBody = (CollisionBody*)_listComponent["CollisionBody"];
	eID objectId = object->getId();
	eDirection direction;

	if (objectId == eID::LAND) {
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

	}
	return 0.0f;

}

GVector2 SpearKnight::getVelocity() {
	auto move = (Movement*)this->_listComponent["Movement"];
	return move->getVelocity();
}

void SpearKnight::die() {
	Gravity *gravity = (Gravity*)this->getComponent("Gravity");
	gravity->setStatus(eGravityStatus::SHALLOWED);
	Movement *movement = (Movement*)this->getComponent("Movement");
	movement->setVelocity(GVector2(0, 200));
}

