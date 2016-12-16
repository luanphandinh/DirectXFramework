#include "CrystalBall.h"
#include "GameStatusBoard.h"

CrystalBall::CrystalBall(GVector2 startPosition, eItemID itemId) :Item(startPosition, eItemType::DROP)
{
	_itemId = itemId;
}


CrystalBall::~CrystalBall()
{
}


void CrystalBall::init()
{
	_sprite = SpriteManager::getInstance()->getSprite(eID::ITEM);
	_animation = new Animation(_sprite, 0.08f);
	_animation->addFrameRect(eID::ITEM, "crystal_ball_01", "crystal_ball_02", NULL);
	Item::initCommonComponent();
	//_endPosition = this->_startPosition + GVector2(0, 40);

}

void CrystalBall::update(float deltatime)
{
	Item::update(deltatime);
	_animation->update(deltatime);
	//this->setPositionY(this->getPositionY() + 100 * deltatime / 1000);
}

void CrystalBall::draw(LPD3DXSPRITE spriteHandler, Viewport* viewport)
{
	if (this->isInStatus(eStatus::DESTROY))
		return;
	_animation->draw(spriteHandler, viewport);
}

void CrystalBall::release()
{
	Item::release();
	SAFE_DELETE(_sprite);
}

void CrystalBall::pickedUp()
{
	Item::pickedUp();
	this->setStatus(eStatus::DESTROY);
}

float CrystalBall::checkCollision(BaseObject* otherObject, float dt)
{
	//Lấy collision body của item ra để checkCollision
	auto collisionBody = (CollisionBody*)_componentList["CollisionBody"];
	eID otherObjectID = otherObject->getId();
	eDirection direction;
	if (otherObjectID != eID::LAND && otherObjectID != eID::SIMON) return 0.0f;
	//if ((otherObjectID == eID::LAND)
	//	&& collisionBody->checkCollision(otherObject, direction, dt, false))
	if (collisionBody->checkCollision(otherObject, direction, dt, false))
	{
		if (otherObjectID == eID::LAND && direction == eDirection::TOP)
		{
			this->stop();
		}
		else if (otherObjectID == eID::SIMON)
		{
			this->pickedUp();
		}
	}
	return 0.0f;
}