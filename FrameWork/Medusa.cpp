#include "Medusa.h"
#include "PlayScene.h"

Medusa::Medusa(GVector2 pos) : BaseEnemy(eID::MEDUSA) {
	_sprite = SpriteManager::getInstance()->getSprite(eID::MEDUSA);
	_sprite->setFrameRect(0, 0, 32.0f, 16.0f);

	GVector2 v(0, 0);
	GVector2 a(0, 0);
	this->_listComponent.insert(pair<string, IComponent*>("Movement", new Movement(a, v, this->_sprite)));
	this->setPosition(pos);
	
//	this->setScale(1.75f);
	
}

Medusa::~Medusa() {}

void Medusa::init() {
	this->setHitpoint(MEDUSA_HITPOINT);
	this->setScore(MEDUSA_SCORE);
	//this->_listComponent.insert(pair<string, IComponent*>("Gravity",
	//	new Gravity(GVector2(0, 0), (Movement*)(this->getComponent("Movement")))));

	auto collisionBody = new CollisionBody(this);
	_listComponent["CollisionBody"] = collisionBody;

	_animations[eStatus::HIDING] = new Animation(_sprite, 0.1f);
	_animations[eStatus::HIDING]->addFrameRect(eID::MEDUSA, "stone", NULL);

	_animations[FLYINGDOWN] = new Animation(_sprite, 0.15f);
	_animations[FLYINGDOWN]->addFrameRect(eID::MEDUSA, "fly_01", "fly_02", "fly_03", "fly_04", NULL);

	_animations[FLYING] = new Animation(_sprite, 0.15f);
	_animations[FLYING]->addFrameRect(eID::MEDUSA, "fly_01", "fly_02", "fly_03", "fly_04", NULL);

	_animations[DYING] = new Animation(_sprite, 0.15f);
	_animations[DYING]->addFrameRect(eID::MEDUSA, NULL);
	//*Test
	//this->setPosition(GVector2(300, 200));
	this->setStatus(eStatus::HIDING);
	
	this->setPhysicBodySide(eDirection::ALL);
	this->hack = 0;
	_isHiding = true;
	this->setHitpoint(16);
}

void Medusa::update(float deltaTime) {
	if (this->getStatus() == eStatus::DESTROY)
		return;

	if (this->getHitpoint() <= 0) {
		this->setStatus(eStatus::BURN);
	}

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

	this->updateHiding();

	//else {
	//	if (hack == 30) {
	//		this->setStatus(FLYING);
	//		this->fly();
	//	}
	//	if (this->getStatus() == FLYINGDOWN) {
	//		hack++;
	//		this->flyingDown();
	//	}

	//	//this->checkIfOutOfScreen();
	//	for (auto component : _listComponent) {
	//		component.second->update(deltaTime);
	//	}
	//	
	//}
	if (this->getStatus() == eStatus::FLYING)
		this->_sprite->setScale(2.0f);
	_animations[this->getStatus()]->update(deltaTime);
}

void Medusa::draw(LPD3DXSPRITE spritehandle, Viewport *viewport) {
	if (_burning != nullptr)
		_burning->draw(spritehandle, viewport);
	if (this->isInStatus(eStatus::DESTROY) || this->isInStatus(eStatus::BURN)) return;
	_animations[this->getStatus()]->draw(spritehandle, viewport);
}

void Medusa::release() {
	for (auto component : _listComponent) {
		delete component.second;
	}
	_listComponent.clear();
}

float Medusa::checkCollision(BaseObject *object, float deltaTime) {
	if (this->getStatus() == eStatus::DESTROY ||
		this->isInStatus(eStatus::DYING) || this->isInStatus(eStatus::HIDING))
		return 0.0f;

	auto collisionBody = (CollisionBody*)_listComponent["CollisionBody"];
	eID objectId = object->getId();
	eDirection direction;
	if (collisionBody->checkCollision(object, direction, deltaTime, false))
	{
		if (objectId == eID::SIMON) 
		{
			((Simon*)object)->getHitted();
		}
		else if (objectId == eID::WHIP && ((Whip*)object)->isHitting())
		{
			this->dropHitpoint(1);
		}

		
	}
	return 0.0f;
}

void Medusa::die() {

}

void Medusa::setPosition(GVector2 pos) {
	_sprite->setPosition(pos);

}

IComponent * Medusa::getComponent(string componentName) {
	return _listComponent.find(componentName)->second;

}

void Medusa::changeDirection() {
	_sprite->setScaleX(-this->getScale().x);
	Movement *movement = (Movement*)this->getComponent("Movement");
	movement->setVelocity(GVector2(-movement->getVelocity().x, 0));
}

void Medusa::flyingDown() {
	auto objectTracker = ((PlayScene*)SceneManager::getInstance()->getCurrentScene())->getSimon();
	RECT objectBound = objectTracker->getBounding();
	auto viewportTracker = ((PlayScene*)SceneManager::getInstance()->getCurrentScene())->getViewport();
	RECT vpBound = viewportTracker->getBounding();
	Movement *movement = (Movement*)this->getComponent("Movement");
	movement->setVelocity(GVector2(movement->getVelocity().x, -(vpBound.top - objectBound.top) / 2));
}

void Medusa::fly() {
	Movement *movement = (Movement*)this->getComponent("Movement");
	movement->setVelocity(GVector2(movement->getVelocity().x, 0));
}

void Medusa::updateHiding() {
	// track theo simon
	if (!_isHiding)  return;
	auto objectTracker = ((PlayScene*)SceneManager::getInstance()->getCurrentScene())->getSimon();
	int xTracker = objectTracker->getPositionX();
	int yTracker = objectTracker->getPositionY();
	if (this->getPositionX() > xTracker)
	{
		if (_hidingStopWatch == nullptr)
		{
			_hidingStopWatch = new StopWatch();
		}
	}
	
	if (_hidingStopWatch != nullptr && _hidingStopWatch->isStopWatch(3000))
	{
		this->setStatus(eStatus::FLYING);
		SAFE_DELETE(_hidingStopWatch);
		_isHiding = false;
	}

}
