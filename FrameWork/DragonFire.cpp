#include "DragonFire.h"
#include "Simon.h"
#include"Whip.h"

DragonFire::DragonFire(GVector2 startPos, eDirection dir) : Weapon(startPos, eItemType::PICKED_UP, dir, eItemID::DRAGON_FIRE)
{
	_damage = 2;
}


DragonFire::~DragonFire()
{
}

void DragonFire::init() 
{
	_sprite = SpriteManager::getInstance()->getSprite(eID::ITEM);
	_sprite->setFrameRect(SpriteManager::getInstance()->getSourceRect(eID::ITEM,"dragon_fire"));
	_sprite->setOrigin(GVector2(0.5, 0.5));
	_sprite->setScale(SCALE_FACTOR);
	Weapon::initCommonComponent();
	this->setPhysicBodySide(eDirection::ALL);
	initWeaponComponent();
}

void DragonFire::update(float deltatime) 
{
	if (this->getStatus() == DESTROY)
		return;

	checkPosition();

	if (this->getStatus() == eStatus::BURN)
	{
		if (_hitEffect == nullptr)
		{
			auto pos = this->getPosition();
			_hitEffect = new HitEffect(2, pos);
			_hitEffect->init();
		}
		else 
		{
			_hitEffect->update(deltatime);
			if (_hitEffect->getStatus() == eStatus::DESTROY) {
				this->setStatus(eStatus::DESTROY);
			}
		}
	}

	for (auto it : _componentList) 
	{
		it.second->update(deltatime);
	}
}

void DragonFire::checkPosition()
{
	Viewport* viewport = SceneManager::getInstance()->getCurrentScene()->getViewport();

	GVector2 viewportPosition = viewport->getPositionWorld();

	//if (this->getPositionX() < viewportPosition.x || this->getPositionX() > viewportPosition.x + WINDOW_WIDTH)
	if (!isContains(viewport->getBounding(), this->getBounding()))
		this->setStatus(eStatus::BURN);
}

void DragonFire::draw(LPD3DXSPRITE spriteHandler, Viewport* viewport) 
{
	if (this->isInStatus(eStatus::DESTROY)) return;

	if (_hitEffect != NULL)
	{
		_hitEffect->draw(spriteHandler, viewport);
	}
	else _sprite->render(spriteHandler, viewport);
}

void DragonFire::release() 
{
	for (auto component : _componentList) {
		delete component.second;
	}
	_componentList.clear();

	if (this->_hitEffect != NULL)
		this->_hitEffect->release();
	SAFE_DELETE(this->_hitEffect);
	SAFE_DELETE(this->_sprite);
}

float DragonFire::checkCollisionWeapon(BaseObject* otherObject, float dt)
{
	if (this->isInStatus(eStatus::DESTROY) || this->isInStatus(eStatus::BURN)) return 0.0f;
	//Lấy collision body của item ra để checkCollision
	auto collisionBody = (CollisionBody*)_componentList["CollisionBody"];
	eID otherObjectID = otherObject->getId();
	eDirection direction;
	if (otherObjectID != eID::SIMON && otherObjectID != eID::WHIP && otherObjectID != eID::ITEM) return 0.0f;
	//if ((otherObjectID == eID::LAND)
	//	&& collisionBody->checkCollision(otherObject, direction, dt, false))
	if (collisionBody->checkCollision(otherObject, direction, dt, false))
	{
		if (otherObjectID == eID::SIMON)
		{
			this->setStatus(eStatus::BURN);
			((Simon*)otherObject)->getHitted();
		}
		else if (otherObjectID == eID::WHIP && ((Whip*)otherObject)->isHitting())
		{
			this->setStatus(eStatus::BURN);
		}
		else if (otherObjectID == eID::ITEM)
		{
			this->setStatus(eStatus::BURN);
		}
	}
	return 0.0f;
}


void DragonFire::initWeaponComponent() 
{
	if (_type != eItemType::PICKED_UP) return;

	GVector2 veloc = this->initVeloc(DRAGON_FIRE_SPEED);

	if (_direction == eDirection::LEFT)
		this->setScaleX(this->getScale().x * (-1));

	auto move = (Movement*)this->_componentList["Movement"];
	move->setVelocity(veloc);

	auto gravity = (Gravity*)this->_componentList["Gravity"];
	gravity->setStatus(eGravityStatus::SHALLOWED);
	gravity->setGravity(GVector2(0, 0));
}
