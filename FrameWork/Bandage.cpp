#include "Bandage.h"
#include "PlayScene.h"

Bandage::Bandage(GVector2 pos, eDirection direction) : BaseEnemy(eID::BANDAGE) 
{
	_sprite = SpriteManager::getInstance()->getSprite(eID::BANDAGE);
	GVector2 veloc;
	if (direction == eDirection::RIGHT)
		veloc = GVector2(BANDAGE_SPEED.x, 0);
	else  veloc = GVector2((-1) * BANDAGE_SPEED.x, 0);

	Movement* movement = new Movement(GVector2Zero, veloc, _sprite);
	_listComponent["Movement"] = movement;

	Gravity* gravity = new Gravity(GVector2(0,85), movement);
	//gravity->setStatus(eGravityStatus::SHALLOWED);
	_listComponent["Gravity"] = gravity;

	auto collisionBody = new CollisionBody(this);
	_listComponent["CollisionBody"] = collisionBody;


	this->setPosition(pos);
	_direction = direction;
	this->setScale(2.0f);
}


Bandage::~Bandage() {}

void Bandage::init() 
{
	_animations[eStatus::FLYING] = new Animation(_sprite, 0.1f);
	_animations[eStatus::FLYING]->addFrameRect(eID::BANDAGE, "fly_01", "fly_02", NULL);
	//_animations[eStatus::FALLING] = new Animation(_sprite, 0.1f);
	//_animations[eStatus::FALLING]->addFrameRect(eID::BANDAGE, "move_01", NULL);
	//_animations[eStatus::DYING] = new Animation(_sprite, 0.1f);
	//_animations[eStatus::DYING]->addFrameRect(eID::BANDAGE, "move_02", NULL);


	//*Test
	//this->setPosition(GVector2(100,100));
	this->setStatus(eStatus::FLYING);
	_sprite->drawBounding(false);
	this->setPhysicBodySide(eDirection::ALL);
}

void Bandage::draw(LPD3DXSPRITE spritehandle, Viewport* viewport) {
	if (this->isInStatus(eStatus::DESTROY)) return;

	if (_burning != NULL)
		_burning->draw(spritehandle, viewport);
	if (this->getStatus() == eStatus::DESTROY || this->getStatus() == eStatus::BURN)
		return;

	_animations[FLYING]->draw(spritehandle, viewport);

}

void Bandage::release() {
	for (auto component : _listComponent) {
		delete component.second;
	}
	_listComponent.clear();

	if (_burning != nullptr)
		_burning->release();
	SAFE_DELETE(_burning);
	SAFE_DELETE(this->_sprite);
}

IComponent* Bandage::getComponent(string componentName) {
	return _listComponent.find(componentName)->second;
}

void Bandage::update(float deltatime)
{

	if (this->getStatus() == DESTROY)
		return;

	checkPosition();

	//if (this->getHitpoint() <= 0) {
	//	this->setStatus(eStatus::BURN);
	//}

	// Bị nướng
	if (this->getStatus() == eStatus::BURN) {
		if (_burning == nullptr) {
			auto pos = this->getPosition();
			_burning = new HitEffect(2, pos);
			_burning->init();
		}
		else {
			_burning->update(deltatime);
			if (_burning->getStatus() == eStatus::DESTROY) {
				this->setStatus(eStatus::DESTROY);
			}
		}
	}

	for (auto it : _listComponent)
	{
		it.second->update(deltatime);
	}

	_animations[FLYING]->update(deltatime);
}


float Bandage::checkCollision(BaseObject * otherObject, float dt) {
	if (this->getStatus() == eStatus::DESTROY || this->isInStatus(eStatus::BURN) || this->isInStatus(eStatus::DYING))
		return 0.0f;

	auto collisionBody = (CollisionBody*)_listComponent["CollisionBody"];
	eID otherObjectId = otherObject->getId();
	eDirection direction;
	if (otherObjectId != eID::WHIP && otherObjectId != eID::SIMON && otherObjectId != eID::ITEM) return 0.0f;
	if (collisionBody->checkCollision(otherObject, direction, dt, false))
	{
		if (otherObjectId == eID::SIMON)
		{
			((Simon*)otherObject)->getHitted(1);
			this->setStatus(eStatus::DESTROY);
		}
		else if (otherObjectId == eID::ITEM || (otherObjectId == eID::WHIP && ((Whip*)otherObject)->isHitting()))
		{
			this->setStatus(BURN);
		}
	}
	return 0.0f;

}

void Bandage::updateCurrentAnimateIndex() {

	if (this->isInStatus(eStatus::RUNNING))
	{
		_currentAnimationIndex = eStatus::RUNNING;
	}
}

GVector2 Bandage::getVelocity() {
	auto move = (Movement*)this->_listComponent["Movement"];
	return move->getVelocity();
}

void Bandage::checkPosition()
{
	auto viewportTracker = ((PlayScene*)SceneManager::getInstance()->getCurrentScene())->getViewport();
	RECT vpBound = viewportTracker->getBounding();

	if (this->getPositionX() < vpBound.left || this->getPositionX() > vpBound.right
		|| this->getPositionY() < vpBound.bottom)
		this->setStatus(DESTROY);
}
