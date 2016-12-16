#include "Brick.h"
#include "ItemManager.h"
#pragma region BrokenBrick
BrokenBrick::BrokenBrick(GVector2 pos,eDirection dir):BaseObject(eID::BROKENBRICK)
{
	_direction = dir;
	_sprite = SpriteManager::getInstance()->getSprite(eID::BROKENBRICK);
	_sprite->setFrameRect(SpriteManager::getInstance()->getSourceRect(eID::BROKENBRICK, "normal"));
	_sprite->setOrigin(GVector2(0.0f, 1.0f));
	this->setPosition(pos);
	this->setStatus(eStatus::NORMAL);
	this->setScale(2.0f);
	this->init();
}

BrokenBrick::~BrokenBrick()
{
}
//implement inherited methods from baseObject
void BrokenBrick::init() 
{

	GVector2 veloc = this->initVeloc(BROKEN_BRICK_SPEED);

	auto movement = new Movement(GVector2(0, 0), GVector2(0, 0), _sprite);
	movement->setVelocity(veloc);
	_componentList.insert(pair<string, IComponent*>("Movement", movement));
	
	Gravity* gravity = new Gravity(GVector2(0, -800), movement);
	_componentList["Gravity"] = gravity;

	auto collisionBody = new CollisionBody(this);
	_componentList.insert(pair<string, IComponent*>("CollisionBody", collisionBody));

}

GVector2 BrokenBrick::initVeloc(GVector2 speed)
{
	GVector2 result;
	if (_direction != eDirection::NONE)
	{
		if ((_direction & eDirection::LEFT) == eDirection::LEFT)
		{
			result.x = -speed.x;
			result.y = 50;
		}
		else if ((_direction & eDirection::RIGHT) == eDirection::RIGHT)
		{
			result.x = 50;
			result.y = speed.y;
		}
		if ((_direction & eDirection::TOP) == eDirection::TOP)
		{
			result.x = 60;
			result.y = speed.y;
		}
		else if ((_direction & eDirection::BOTTOM) == eDirection::BOTTOM)
		{
			result.x = 20;
			result.y = 100;
		}
	}
	return result;
}

void BrokenBrick::update(float deltaTime) 
{
	for (auto it = _componentList.begin(); it != _componentList.end(); it++)
	{
		it->second->update(deltaTime);
	}
}

void BrokenBrick::draw(LPD3DXSPRITE spriteHandler, Viewport* viewport) 
{
	_sprite->render(spriteHandler, viewport);
}

void BrokenBrick::release() 
{
	for (auto component : _componentList) {
		delete component.second;
	}
	_componentList.clear();

	SAFE_DELETE(this->_sprite);
}
#pragma endregion

#pragma region Brick
Brick::Brick(GVector2 pos, eItemID dropItemId) :BaseObject(eID::BRICK)
{
	_sprite = SpriteManager::getInstance()->getSprite(eID::BRICK);
	_sprite->setFrameRect(SpriteManager::getInstance()->getSourceRect(eID::BRICK,"normal"));
	_sprite->setOrigin(GVector2(0.0f, 1.0f));
	this->setPosition(pos);
	this->setStatus(eStatus::NORMAL);
	this->setScale(1.0f);
	this->setPhysicBodySide(eDirection::ALL);
	this->_dropItemId = dropItemId;
}
Brick::~Brick(){}

//implement inherited methods from baseObject
void Brick::init()
{
	BrokenBrick* brokenBrick = new BrokenBrick(this->getPosition(), eDirection::LEFT);
	_brokens.push_back(brokenBrick);
	brokenBrick = new BrokenBrick(this->getPosition(), eDirection::RIGHT);
	_brokens.push_back(brokenBrick);
	brokenBrick = new BrokenBrick(this->getPosition(), eDirection::TOP);
	_brokens.push_back(brokenBrick);
	brokenBrick = new BrokenBrick(this->getPosition(), eDirection::BOTTOM);
	_brokens.push_back(brokenBrick);

	auto collisionBody = new CollisionBody(this);
	_componentList.insert(pair<string, IComponent*>("CollisionBody", collisionBody));
}


void Brick::update(float deltatime)  
{
	if (this->isInStatus(eStatus::DESTROY))
	{
		for (BrokenBrick* item : _brokens)
		{
			item->release();
			_brokens.remove(item);
			delete item;
			item = nullptr;
			break;
		}
		return;
	}

	if (!this->isInStatus(eStatus::BURN)) return;

	for (BrokenBrick* item : _brokens)
	{
		if (item == nullptr)
		{
			_brokens.remove(item);
			break;
		}
		else
		{
			item->update(deltatime);
		}
	}

	if (_stopWatch == nullptr)
	{
		_stopWatch = new StopWatch();
		ItemManager::generateItem(_dropItemId, this->getPosition() + GVector2(16,-16));
		return;
	}

	if (_stopWatch->isStopWatch(2000))
	{
		SAFE_DELETE(_stopWatch);
		this->setStatus(eStatus::DESTROY);
		
	}
}
void Brick::draw(LPD3DXSPRITE spriteHandler, Viewport* viewport)
{
	if (this->isInStatus(eStatus::DESTROY)) return;

	if (!this->isInStatus(eStatus::BURN))
	{
		_sprite->render(spriteHandler, viewport);
		return;
	}
	

	for (BrokenBrick* item : _brokens)
	{
		if (item == nullptr)
		{
			_brokens.remove(item);
			break;
		}
		else
		{
			//item->getSprite()->drawBounding(true);
			item->draw(spriteHandler, viewport);
		}
	}
}

void Brick::release()  
{
	for (auto object : _brokens) {
			object->release();
	}
	_brokens.clear();
}
#pragma endregion