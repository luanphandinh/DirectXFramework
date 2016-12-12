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


	_animations[eStatus::HANGING] = new Animation(_sprite, 0.1f);
	_animations[eStatus::HANGING]->addFrameRect(eID::BAT, "normal", NULL);

	_animations[FLYINGDOWN] = new Animation(_sprite, 0.15f);
	_animations[FLYINGDOWN]->addFrameRect(eID::BAT, "fly_02", "fly_03", "fly_04", NULL);

	_animations[FLYING] = new Animation(_sprite, 0.15f);
	_animations[FLYING]->addFrameRect(eID::BAT,  "fly_02", "fly_03", "fly_04", NULL);

	_animations[DYING] = new Animation(_sprite, 0.15f);
	_animations[DYING]->addFrameRect(eID::BAT, NULL);

	_stopWatch = new StopWatch();
	//*Test
	//this->setPosition(GVector2(300, 200));
	this->setStatus(eStatus::HANGING);
	_sprite->drawBounding(false);

	this->hack = 0;
	this->setPhysicBodySide(eDirection::ALL);
	this->setHitpoint(1);
}

void Bat::draw(LPD3DXSPRITE spritehandle, Viewport* viewport) {
	if (_burning != nullptr)
		_burning->draw(spritehandle, viewport);
	if (this->isInStatus(eStatus::DESTROY) || this->isInStatus(eStatus::BURN)) return;
	_animations[this->getStatus()]->draw(spritehandle, viewport);

}

void Bat::release() {
	for (auto component : _listComponent) {
		delete component.second;
	}
	_listComponent.clear();
	if (_burning != nullptr)
		_burning->release();
	//SAFE_DELETE(this->_loopwatch);
	SAFE_DELETE(this->_sprite);
}

IComponent* Bat::getComponent(string componentName) {
	return _listComponent.find(componentName)->second;
}

void Bat::update(float deltaTime) {

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
		this->checkIfOutOfScreen();
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
	auto objectTracker = ((PlayScene*)SceneManager::getInstance()->getCurrentScene())->getSimon();
	RECT objectBound = objectTracker->getBounding();
	auto viewportTracker = ((PlayScene*)SceneManager::getInstance()->getCurrentScene())->getViewport();
	RECT vpBound = viewportTracker->getBounding();
	Movement *movement = (Movement*)this->getComponent("Movement");
	movement->setVelocity(GVector2(movement->getVelocity().x, -(vpBound.top-objectBound.top)/2));
}

void Bat::fly() {
	Movement *movement = (Movement*)this->getComponent("Movement");
	movement->setVelocity(GVector2(movement->getVelocity().x, 0));
}

// Ko giết nó, để nó bay ra khỏi view thì hủy luôn
void Bat::checkIfOutOfScreen() {
	if (this->getStatus() != eStatus::NORMAL)
		return;
	auto viewport = ((PlayScene*)SceneManager::getInstance()->getCurrentScene())->getViewport();
	RECT screenBound = viewport->getBounding();
	GVector2 position = this->getPosition();

	if (position.x > screenBound.right) {
		this->setStatus(eStatus::DESTROY);
	}
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


float Bat::checkCollision(BaseObject * object, float dt) {
	if (this->getStatus() == eStatus::DESTROY ||
		this->isInStatus(eStatus::DYING))
		return 0.0f;

	auto collisionBody = (CollisionBody*)_listComponent["CollisionBody"];
	eID objectId = object->getId();
	eDirection direction;
	if (objectId != eID::SIMON) return 0.0f;
	if (objectId == eID::SIMON) {
		if (collisionBody->checkCollision(object, direction, dt, false)) {
			auto movement = (Movement*)this->_listComponent["Movement"];
			/*
				Phần check collision thằng simon ko liên quan nữa nha
				Chuyển hết sang cho cái roi nó check rồi,chỉ check lúc đụng simon thì cho simon getHitted()
				với mất máu thôi
			*/
			//if (object->isInStatus(eStatus::HITTING)) {
			//	if (_stopWatch->isStopWatch(200)) {
			//		//this->_animations[this->getStatus()]->enableFlashes(true);
			//		//_animations[this->getStatus()]->setColorFlash(D3DXCOLOR(1.0f, 0.5f, 0.5f, 1));
			//	}
			//	this->dropHitpoint(1);
			//	//_isHitted = true;
			//}
			//else {
				((Simon*)object)->getHitted();
				//this->_animations[this->getStatus()]->enableFlashes(false);
				//_isHitted = false;
			//}
		}

		return 0.0f;
	}

	//collisionBody->checkCollision(object, dt, false);

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
	movement->setVelocity(GVector2(0, -200));
}

