#include "Ghost.h"
#include "PlayScene.h"

Ghost::Ghost(eStatus status, GVector2 pos, int direction) : BaseEnemy(eID::GHOST) {
	_sprite = SpriteManager::getInstance()->getSprite(eID::GHOST);
	_sprite->setFrameRect(0, 0, 32.0f, 16.0f);

	GVector2 v(direction * GHOST_SPEED, 0);
	GVector2 a(0, 0);
	this->_listComponent.insert(pair<string, IComponent*>("Movement", new Movement(a, v, this->_sprite)));
	this->setStatus(status);
	this->setPosition(pos);
	this->setScale(SCALE_FACTOR);
	this->setScaleX(direction * SCALE_FACTOR);
	this->_direction = direction;
}

Ghost::~Ghost() {

}

void Ghost::init() {
	this->setHitpoint(GHOST_HITPOINT);
	this->setScore(GHOST_SCORE);
	this->_listComponent.insert(pair<string, IComponent*>("Gravity",
		new Gravity(GVector2(0, 0), (Movement*)(this->getComponent("Movement")))));

	auto collisionBody = new CollisionBody(this);
	_listComponent["CollisionBody"] = collisionBody;

	__hook(&CollisionBody::onCollisionBegin, collisionBody, &Ghost::onCollisionBegin);
	__hook(&CollisionBody::onCollisionEnd, collisionBody, &Ghost::onCollisionEnd);

	_animations[eStatus::HIDING] = new Animation(_sprite, 0.1f);
	_animations[eStatus::HIDING]->addFrameRect(eID::GHOST, NULL);

	_animations[FLYINGUP] = new Animation(_sprite, 0.15f);
	_animations[FLYINGUP]->addFrameRect(eID::GHOST, "fly_02", "fly_01", NULL);

	_animations[FLYING] = new Animation(_sprite, 0.15f);
	_animations[FLYING]->addFrameRect(eID::GHOST, "fly_02", "fly_01", NULL);

	_animations[DYING] = new Animation(_sprite, 0.15f);
	_animations[DYING]->addFrameRect(eID::GHOST, NULL);

	_stopWatch = new StopWatch();
	//*Test
	//this->setPosition(GVector2(300, 200));
	this->setStatus(eStatus::HIDING);
	_sprite->drawBounding(false);

	this->hack = 0;
}

void Ghost::update(float deltaTime) {
	if (this->getStatus() == eStatus::DESTROY)
		return;
	if (this->getStatus() == eStatus::HIDING) {
		this->updateHiding();
		return;
	}
	else {
		if (hack == 30) {
			this->setStatus(FLYING);
			this->fly();
		}
		if (this->getStatus() == FLYINGUP) {
			hack++;
			this->flyingUp();
		}
		this->checkIfOutOfScreen();
		for (auto component : _listComponent) {
			component.second->update(deltaTime);
		}
		if (this->getStatus() == eStatus::FLYING || this->getStatus() == eStatus::FLYINGUP)
			_animations[this->getStatus()]->update(deltaTime);
	}
}

void Ghost::draw(LPD3DXSPRITE spritehandle, Viewport *viewport) {
	if (this->getStatus() == eStatus::DESTROY) return;
	if (this->getStatus() == eStatus::FLYING || this->getStatus() == eStatus::FLYINGUP)
		_animations[this->getStatus()]->draw(spritehandle, viewport);

}

void Ghost::release() {
	for (auto component : _listComponent) {
		delete component.second;
	}
	_listComponent.clear();

	//SAFE_DELETE(this->_loopwatch);
	//SAFE_DELETE(this->_sprite);
}

void Ghost::onCollisionBegin(CollisionEventArg *collision_event) {
	eID objectID = collision_event->_otherObject->getId();
	switch (objectID) {
	case eID::SIMON:
	{
		if (collision_event->_otherObject->isInStatus(eStatus::DYING) == false) {
			collision_event->_otherObject->setStatus(eStatus::DYING);
		}
		break;
	}
	default:
		break;
	}
}

void Ghost::onCollisionEnd(CollisionEventArg *collision_event) {
	if (this->getStatus() == eStatus::DESTROY)
		return;
	eID objectID = collision_event->_otherObject->getId();

}

float Ghost::checkCollision(BaseObject *object, float deltaTime) {
	if (this->getStatus() == eStatus::DESTROY ||
		this->isInStatus(eStatus::DYING))
		return 0.0f;

	auto collisionBody = (CollisionBody*)_listComponent["CollisionBody"];
	eID objectId = object->getId();
	eDirection direction;

	if (objectId == eID::SIMON) {
		if (collisionBody->checkCollision(object, direction, deltaTime, false)) {
			auto movement = (Movement*)this->_listComponent["Movement"];

			if (object->isInStatus(eStatus::HITTING)) {
				if (_stopWatch->isStopWatch(200)) {
					this->_animations[this->getStatus()]->enableFlashes(true);
					_animations[this->getStatus()]->setColorFlash(D3DXCOLOR(1.0f, 0.5f, 0.5f, 1));
				}
				this->dropHitpoint(1);
				_isHitted = true;
			}
			else {
				((Simon*)object)->getHitted();
				//this->_animations[this->getStatus()]->enableFlashes(false);
				_isHitted = false;
			}
		}

		return 0.0f;
	}

	collisionBody->checkCollision(object, deltaTime, false);

	return 0.0f;

}

void Ghost::die() {

}

void Ghost::setPosition(GVector2 pos) {
	_sprite->setPosition(pos);

}

GVector2 Ghost::getVelocity() {
	auto move = (Movement*)this->_listComponent["Movement"];
	return move->getVelocity();
}

int Ghost::getDirection() {
	return _direction;
}

IComponent * Ghost::getComponent(string componentName) {
	return _listComponent.find(componentName)->second;
}

void Ghost::changeDirection() {
	_sprite->setScaleX(-this->getScale().x);
	Movement *movement = (Movement*)this->getComponent("Movement");
	movement->setVelocity(GVector2(-movement->getVelocity().x, 0));
}

void Ghost::flyingUp() {
	auto objectTracker = ((PlayScene*)SceneManager::getInstance()->getCurrentScene())->getSimon();
	RECT objectBound = objectTracker->getBounding();
	auto viewportTracker = ((PlayScene*)SceneManager::getInstance()->getCurrentScene())->getViewport();
	RECT vpBound = viewportTracker->getBounding();
	Movement *movement = (Movement*)this->getComponent("Movement");
	movement->setVelocity(GVector2(movement->getVelocity().x, +5));
}

void Ghost::fly() {
	Movement *movement = (Movement*)this->getComponent("Movement");
	movement->setVelocity(GVector2(movement->getVelocity().x, 0));
}

void Ghost::checkIfOutOfScreen() {
	if (this->getStatus() != eStatus::NORMAL)
		return;
	auto viewport = ((PlayScene*)SceneManager::getInstance()->getCurrentScene())->getViewport();
	RECT screenBound = viewport->getBounding();
	GVector2 position = this->getPosition();

	if (position.x > screenBound.right) {
		this->setStatus(eStatus::DESTROY);
	}
}

void Ghost::updateHiding() {
	// track theo simon
	auto objectTracker = ((PlayScene*)SceneManager::getInstance()->getCurrentScene())->getSimon();
	RECT objectBound = objectTracker->getBounding();
	int x = objectTracker->getPositionX();
	int y = objectTracker->getPositionY();
	int xthis = this->getPositionX();
	int ythis = this->getBounding().bottom;
	/*if (x > xthis&&x < xthis + 250 && y<ythis&&y>ythis + 100) {
		this->setStatus(FLYINGUP);
	}*/
	/*if (this->getDirection() == 1&& objectBound.right < this->getBounding().left) {
		this->setStatus(FLYINGUP);
	}
	else if (this->getDirection() == -1 && objectBound.right < this->getBounding().left
		&&ythis<y+10&&ythis>y) {
		this->setStatus(FLYINGUP);
	}*/
	if (this->getDirection() == -1&&objectBound.right < this->getBounding().left - 100 && objectBound.top>this->getBounding().bottom) {
		this->setStatus(FLYINGUP);
	}
	else if (this->getDirection() == 1 && objectBound.left > this->getBounding().right + 100 && objectBound.top>this->getBounding().bottom) {
		this->setStatus(FLYINGUP);
	}
	else {
		this->setStatus(HIDING);
	}
}
