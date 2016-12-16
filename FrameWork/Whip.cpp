#include "Whip.h"
#include"Simon.h"
#include"PlayScene.h"
#include"BaseEnemy.h"

Whip::Whip(int level) :BaseObject(eID::WHIP)
{
	this->_level = level;
	_damage = 10;
}


Whip::~Whip()
{
}

void Whip::init()
{
	this->_sprite = SpriteManager::getInstance()->getSprite(eID::WHIP);
	_sprite->setScale(2.0f);
	_animations[1] = new Animation(_sprite, 0.12f);
	_animations[1]->addFrameRect(eID::WHIP, "whip_01", "whip_02", "whip_03", "whip_03", NULL);
	auto collisionBody = new CollisionBody(this);
	_componentList.insert(pair<string, IComponent*>("CollisionBody", collisionBody));
	this->setPhysicBodySide(eDirection::ALL);
	//_hitEffect = new HitEffect(1, this->getPosition());
}

void Whip::update(float deltatime) 
{
	_sprite->setScale(_simon->getSprite()->getScale());
	if (_simon->isInStatus(eStatus::SITTING))
		this->setPosition(_simon->getPosition() - GVector2(0, 7));
	else
		this->setPosition(_simon->getPosition());
	if (_simon->isInStatus(eStatus::HITTING))
		_animations[_level]->update(deltatime);
	else this->restart();

	if (_listColliding.size() == 0)
	{
		SAFE_DELETE(_hitEffect);
		_hitEffect = nullptr;
	}
	else
	for (auto it = _listColliding.begin(); it != _listColliding.end(); ++it)
	{
		if (((BaseObject*)(it->first)) != nullptr && !((BaseObject*)(it->first))->isInStatus(eStatus::BURN))
		{
			if (_hitEffect == nullptr) {
				auto pos = ((BaseObject*)(it->first))->getPosition();
				_hitEffect = new HitEffect(1, pos);
				_hitEffect->init();
			}
			else {
				_hitEffect->setPosition(((BaseObject*)(it->first))->getPosition());
				_hitEffect->update(deltatime);
				if (_hitEffect->getStatus() == eStatus::DESTROY) {
					_hitEffect->release();
					SAFE_DELETE(_hitEffect);
					_hitEffect = nullptr;
				}
			}
		}
	}

}

void Whip::draw(LPD3DXSPRITE spriteHandler, Viewport* viewport)
{
	if (_simon->isInStatus(eStatus::HITTING))
	{
		//_sprite->setScale(_simon->getSprite()->getScale());
		//if (_simon->isInStatus(eStatus::SITTING))
		//	this->setPosition(_simon->getPosition() - GVector2(0,7));
		//else
		//	this->setPosition(_simon->getPosition());
		_animations[_level]->draw(spriteHandler, viewport);
	}

	if (_hitEffect != nullptr)
		_hitEffect->draw(spriteHandler, viewport);

}

void Whip::release() 
{
	SAFE_DELETE(_sprite);
}

void Whip::restart()
{
	_animations[_level]->restart();
	_listColliding.clear();
}

void Whip::setTracker(BaseObject* simon)
{
	this->_simon = simon;
}

bool Whip::isHitting()
{
	return (_animations[_level]->getIndex() == 2);
}

float Whip::checkCollision(BaseObject* otherObject, float dt)
{
	//Lấy collision body của item ra để checkCollision
	auto collisionBody = (CollisionBody*)_componentList["CollisionBody"];
	eID otherObjectID = otherObject->getId();
	eDirection direction;
	if (otherObjectID == eID::SIMON || otherObjectID == eID::LAND
		||otherObjectID == eID::STAIR) return 0.0f;
	//if ((otherObjectID == eID::LAND)
	//	&& collisionBody->checkCollision(otherObject, direction, dt, false))
	if (collisionBody->checkCollision(otherObject, direction, dt, false) && this->isHitting() 
		/*&& !otherObject->isInStatus(eStatus::BURN)
		&& !otherObject->isInStatus(eStatus::DESTROY)*/)
	{
		auto object = _listColliding.find(otherObject);
		switch (otherObjectID)
		{
		case BRICK:
		case CANDLE:
			otherObject->setStatus(eStatus::BURN);
			SoundManager::getInstance()->Play(eSoundId::HIT);
			break;
		case GHOST:
		case BAT:
		case SPEARKNIGHT:
		case MEDUSAHEAD:
			if (object == _listColliding.end() || object._Ptr == nullptr)
			{
				((BaseEnemy*)otherObject)->dropHitpoint(this->_damage);
				_listColliding[otherObject] = true;
			}
			SoundManager::getInstance()->Play(eSoundId::HIT);
			break;
		case DRAGON:
			if (object == _listColliding.end() || object._Ptr == nullptr) {
				((BaseEnemy*)otherObject)->dropHitpoint(this->_damage);
				_listColliding[otherObject] = true;
			}
			SoundManager::getInstance()->Play(eSoundId::HITCANNON);
			break;
		case MEDUSA:
			if (object == _listColliding.end() || object._Ptr == nullptr)
			{
				((Medusa*)otherObject)->dropHitpoint(this->_damage);
				((Medusa*)otherObject)->getHitted();
				_listColliding[otherObject] = true;
			}
			SoundManager::getInstance()->Play(eSoundId::HIT);
			break;
		default:
			break;
		}
	}
	return 0.0f;
}
