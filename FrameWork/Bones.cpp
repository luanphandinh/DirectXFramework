#include "Bones.h"
#include "Scene.h"
#include "Simon.h"
#include "Whip.h"
#include "SceneManager.h"
Bones::Bones(GVector2 startPosition, eDirection dir) : Weapon(startPosition, eItemType::PICKED_UP, dir, eItemID::BONES)
{
	_damage = 2;
}


Bones::~Bones()
{
}

void Bones::init()
{
	_sprite = SpriteManager::getInstance()->getSprite(eID::SKELETON);
	
	_animation = new Animation(_sprite, 0.15f);
	_animation->addFrameRect(eID::SKELETON, "bone_01", "bone_02", NULL);

	Weapon::initCommonComponent();
	this->setPhysicBodySide(eDirection::ALL);
	initWeaponComponent();
}


void Bones::update(float deltatime)
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

	_animation->update(deltatime);

	for (auto it : _componentList)
	{
		it.second->update(deltatime);
	}
}

void Bones::draw(LPD3DXSPRITE spriteHandler, Viewport* viewport)
{
	if (this->isInStatus(eStatus::DESTROY)) return;

	if (_hitEffect != NULL)
	{
		_hitEffect->draw(spriteHandler, viewport);
	}
	else _animation->draw(spriteHandler, viewport);
}

void Bones::release()
{

}

void Bones::checkPosition()
{
	auto viewport = ((Scene*)SceneManager::getInstance()->getCurrentScene())->getViewport();
	RECT screenBound = viewport->getBounding();
	GVector2 position = this->getPosition();

	if (position.y < screenBound.bottom) {
		this->setStatus(eStatus::DESTROY);
	}
}

float Bones::checkCollisionWeapon(BaseObject* otherObject, float dt)
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


void Bones::initWeaponComponent()
{
	if (_type != eItemType::PICKED_UP) return;

	GVector2 veloc = this->initVeloc(GVector2(BONES_VELOC.x - 20, BONES_VELOC.y + 700));

	auto move = (Movement*)this->_componentList["Movement"];
	move->setVelocity(veloc);

	auto gravity = (Gravity*)this->_componentList["Gravity"];
	gravity->setStatus(eGravityStatus::FALLING_DOWN);
	gravity->setGravity(GVector2(0, -3000));

	RotateMovement* rotateMovement = new RotateMovement(_sprite);
	_componentList["RotateMovement"] = rotateMovement;
}
