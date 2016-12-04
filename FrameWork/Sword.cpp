#include "Sword.h"
#include"PlayScene.h"

Sword::Sword(GVector2 startPosition, eItemType type, eDirection dir) :Weapon(startPosition,type,dir,eItemID::SWORD)
{
	_damage = 1;
}


Sword::~Sword()
{
}

void Sword::init()
{
	_sprite = SpriteManager::getInstance()->getSprite(eID::ITEM);
	_sprite->setFrameRect(SpriteManager::getInstance()->getSourceRect(eID::ITEM, "sword"));

	if (_type == eItemType::PICKED_UP)
		HeartCounter::plusHeart(-1);

	Weapon::initCommonComponent();

	if ((_direction & eDirection::RIGHT) == eDirection::RIGHT)
		_sprite->setScaleX(this->getScale().x * (-1));

	initWeaponComponent();
}

void Sword::update(float deltatime)
{
	Weapon::update(deltatime);

	Viewport* viewport = SceneManager::getInstance()->getCurrentScene()->getViewport();

	GVector2 viewportPosition = viewport->getPositionWorld();

	if (this->getPositionX() < viewportPosition.x || this->getPositionX() > viewportPosition.x + WINDOW_WIDTH)
		this->setStatus(eStatus::DESTROY);
}

void Sword::draw(LPD3DXSPRITE spriteHandler, Viewport* viewport)
{
	Weapon::draw(spriteHandler, viewport);
}

void Sword::release()
{
	
}

void Sword::initWeaponComponent()
{
	if (_type != eItemType::PICKED_UP) return;

	GVector2 veloc = this->initVeloc(SWORD_SPEED);

	auto move = (Movement*)this->_componentList["Movement"];
	move->setVelocity(veloc);

	auto gravity = (Gravity*)this->_componentList["Gravity"];
	gravity->setStatus(eGravityStatus::SHALLOWED);
	gravity->setGravity(GVector2(0, 0));
}

float Sword::checkCollisionWeapon(BaseObject* otherObject, float dt)
{
	//Lấy collision body của item ra để checkCollision
	auto collisionBody = (CollisionBody*)_componentList["CollisionBody"];
	eID otherObjectID = otherObject->getId();
	eDirection direction;
	if (otherObjectID == eID::SIMON || otherObjectID == eID::LAND
		|| otherObjectID == eID::STAIR || otherObjectID == eID::WHIP) return 0.0f;

	if (collisionBody->checkCollision(otherObject, direction, dt, false))
	{
		switch (otherObjectID)
		{
		case CANDLE:
			otherObject->setStatus(eStatus::BURST);
			break;
		case DRAGON:
		case SPEARKNIGHT:
			((BaseEnemy*)otherObject)->dropHitpoint(this->_damage);
			break;
		case BAT:
			otherObject->setStatus(eStatus::BURST);
			break;
		case MEDUSAHEAD:
			break;
		default:
			break;
		}
		this->setStatus(eStatus::DESTROY);
	}
	return 0.0f;
}