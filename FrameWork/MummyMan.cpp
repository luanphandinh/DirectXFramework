#include "MummyMan.h"
#include "GameStatusBoard.h"
#include "Level3.h"


MummyMan::MummyMan(GVector2 pos, eDirection direction) : BaseEnemy(eID::MUMMYMAN) 
{
	_sprite = SpriteManager::getInstance()->getSprite(eID::MUMMYMAN);
	_direction = direction;
	GVector2 veloc;
	if (direction == eDirection::LEFT)
	{
		this->setScaleX(SCALE_FACTOR);
		veloc = MUMMYMAN_VELOC;
	}
	else 
	{
		this->setScaleX((-1) * SCALE_FACTOR);
		veloc = GVector2((-1) * MUMMYMAN_VELOC.x,MUMMYMAN_VELOC.y);
	}

	this->_listComponent.insert(pair<string, IComponent*>("Movement", new Movement(GVector2Zero, veloc, this->_sprite)));
	this->setPosition(pos);
	this->setScale(SCALE_FACTOR);

}

MummyMan::~MummyMan() {}

void MummyMan::init()
{
	this->setHitpoint(MUMMYMAN_HITPOINT);
	this->setScore(MUMMYMAN_SCORE);

	this->_listComponent.insert(pair<string, IComponent*>("Gravity",
		new Gravity(GVector2(0, -ENEMY_GRAVITY), (Movement*)(this->getComponent("Movement")))));

	auto collisionBody = new CollisionBody(this);
	_listComponent["CollisionBody"] = collisionBody;

	_animations[eStatus::HIDING] = new Animation(_sprite, 0.1f);
	_animations[eStatus::HIDING]->addFrameRect(eID::MUMMYMAN, "normal", NULL);

	_animations[WALKING] = new Animation(_sprite, 0.15f);
	_animations[WALKING]->addFrameRect(eID::MUMMYMAN, "walk_01", "walk_02", "walk_03", NULL);

	this->setStatus(eStatus::HIDING);
	_sprite->drawBounding(false);
	_checkedWithLand = false;
	this->setPhysicBodySide(eDirection::ALL);
	_isStanding = true;
	_isHitted = false;
	_isMovingBack = false;
}

void MummyMan::draw(LPD3DXSPRITE spritehandle, Viewport* viewport)
{
	if (this->isInStatus(eStatus::DESTROY)) return;

	if (_burning != NULL)
		_burning->draw(spritehandle, viewport);
	if (this->getStatus() == eStatus::DESTROY || this->getStatus() == eStatus::BURN)
		return;

	_animations[WALKING]->draw(spritehandle, viewport);

}

void MummyMan::release()
{
	for (auto component : _listComponent)
	{
		delete component.second;
	}
	_listComponent.clear();

	if (this->_burning != NULL)
		this->_burning->release();
	SAFE_DELETE(this->_burning);
	//SAFE_DELETE(this->_stopwatch);

	SAFE_DELETE(this->_sprite);
}

IComponent* MummyMan::getComponent(string componentName)
{
	return _listComponent.find(componentName)->second;
}

void MummyMan::update(float deltaTime) 
{

	if (this->getStatus() == eStatus::DESTROY) return;

	if (this->getHitpoint() <= 0) {
		GameStatusBoard::getInstance()->getEnemyLifeUI()->setHPNumber(0);
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
				//ItemManager::generateItem(eItemID::CRYSTALBALL, this->getPosition());
				((Level3*)SceneManager::getInstance()->getCurrentScene())->getDirector()->generateCrystalBall();
			}
		}
		return;
	}

	if (this->isInStatus(eStatus::HIDING))
	{
		this->updateStatus();
		return;
	}

	GameStatusBoard::getInstance()->getEnemyLifeUI()->setHPNumber(this->getHitpoint());

	if (_isHitted)
	{
		if (_getHittedStopWatch == nullptr)
		{
			_getHittedStopWatch = new StopWatch();
		}

		if (_getHittedStopWatch->isStopWatch(150))
		{
			_isHitted = false;
			SAFE_DELETE(_getHittedStopWatch);
		}
		return;
	}

	updateDirection();

	for (auto component : _listComponent)
	{
		component.second->update(deltaTime);
	}

	_animations[this->getStatus()]->update(deltaTime);

	//for (auto object : _listObjects)
	//{
	//	object->update(deltaTime);
	//}
	//removeSnake();
}

void MummyMan::updateDirection()
{
	auto viewportTracker = ((Level3*)SceneManager::getInstance()->getCurrentScene())->getViewport();
	RECT vpBound = viewportTracker->getBounding();

	if (_isMovingBack)
	{
		if (_movingBackStopWatch == nullptr)
		{
			_movingBackStopWatch = new StopWatch();
			changeDirection(_movingBackDirection);
		}

		if ((_movingBackDirection == eDirection::LEFT && this->getPositionX() < _movingBackPos.x)
			|| (_movingBackDirection == eDirection::RIGHT && this->getPositionX() > _movingBackPos.x))
		{
			_isMovingBack = false;
			_isHold = true;
			SAFE_DELETE(_movingBackStopWatch);
		}
	}

	if (_isHold)
	{
		if (_holdStopWatch == nullptr)
		{
			Movement *movement = (Movement*)this->getComponent("Movement");
			movement->setVelocity(GVector2Zero);
			_holdStopWatch = new StopWatch();
		}

		if (_holdStopWatch->isStopWatch(1200))
		{
			trackSimon();
			//if (checkFlyDown()) _flyDown = true;
			_isHold = false;
			//if (_flyPath == eFlyPath::LONGDISTANCE)
			//	createSnake();
			SAFE_DELETE(_holdStopWatch);
		}
		return;
	}

	if (this->getPositionX() > vpBound.right - 16)
		changeDirection(eDirection::LEFT);
	else if (this->getPositionX() <  vpBound.left + 16)
		changeDirection(eDirection::RIGHT);
}

void MummyMan::trackSimon()
{
	auto objectTracker = ((Level3*)SceneManager::getInstance()->getCurrentScene())->getSimon();
	int xTracker = objectTracker->getPositionX();
	int yTracker = objectTracker->getPositionY();

	if (this->getPositionX() > xTracker)
	{
		changeDirection(eDirection::LEFT);
	}
	else changeDirection(eDirection::RIGHT);
}


void MummyMan::movingBack()
{
	if (_isMovingBack) return;
	getMovingPath();
	auto objectTracker = ((Level3*)SceneManager::getInstance()->getCurrentScene())->getSimon();
	auto posTracker = objectTracker->getPosition();
	auto viewportTracker = ((Level3*)SceneManager::getInstance()->getCurrentScene())->getViewport();
	RECT vpBound = viewportTracker->getBounding();
	//Simon nằm bên nữa màn hình bên trái
	if (_direction == eDirection::LEFT && ((vpBound.left + (vpBound.right - vpBound.left) / 2)  > posTracker.x))
	{
		switch (_movingPath) {
		case eMovingPath::MOVEHOLD:
			_movingBackPos = GVector2(posTracker + GVector2(10, 0));
			break;
		case eMovingPath::MOVEMEDIUMDISTANCE:
			_movingBackPos = GVector2(posTracker + GVector2(100, 0));
			break;
		case eMovingPath::MOVELONGDISTANCE:
			_movingBackPos = GVector2(posTracker + GVector2(200, 0));
			break;
		default:
			break;
		}

		_isMovingBack = true;
		_movingBackDirection = eDirection::RIGHT;
	}
	//Simon nằm bên nữa màn hình bên phải
	/*((vpBound.right - vpBound.left) / 2 < posTracker.x)*/
	else if (_direction == eDirection::RIGHT && ((vpBound.left + (vpBound.right - vpBound.left) / 2) < posTracker.x))
	{
		switch (_movingPath) {
		case eMovingPath::MOVEHOLD:
			_movingBackPos = GVector2(posTracker + GVector2(-10, 0));
			break;
		case eMovingPath::MOVEMEDIUMDISTANCE:
			_movingBackPos = GVector2(posTracker + GVector2(-100, 0));
			break;
		case eMovingPath::MOVELONGDISTANCE:
			_movingBackPos = GVector2(posTracker + GVector2(-200, 0));
			break;
		default:
			break;
		}
		_isMovingBack = true;
		_movingBackDirection = eDirection::LEFT;
	}
}

void MummyMan::getMovingPath()
{
	int i = rand() % 3;
	_movingPath = (eMovingPath)i;
}

void MummyMan::updateStatus()
{
	// track theo simon
	if (!_isStanding)  return;
	auto objectTracker = ((Level3*)SceneManager::getInstance()->getCurrentScene())->getSimon();
	int xTracker = objectTracker->getPositionX();
	int yTracker = objectTracker->getPositionY();
	if (this->getPositionX() < xTracker)
	{
		if (_standingStopWatch == nullptr)
		{
			_standingStopWatch = new StopWatch();
		}
	}

	if (_standingStopWatch != nullptr && _standingStopWatch->isStopWatch(2000))
	{
		if (this->getPositionX() > xTracker)
		{
			changeDirection(eDirection::LEFT);
		}
		else changeDirection(eDirection::RIGHT);
		this->setStatus(eStatus::WALKING);
		SAFE_DELETE(_standingStopWatch);
		SoundManager::getInstance()->Stop(eSoundId::BACKGROUND_LEVEL3);
		SoundManager::getInstance()->PlayLoop(eSoundId::BOSSMEDUSA);
		_isStanding = false;
	}
}

void MummyMan::changeDirection(eDirection dir)
{
	if (_direction == dir)
		return;
	_direction = dir;
	Movement *movement = (Movement*)this->getComponent("Movement");

	if (_direction == eDirection::RIGHT)
	{
		if (this->getScale().x > 0)
			this->setScaleX((-1) * this->getScale().x);
		movement->setVelocity(MUMMYMAN_VELOC);
	}
	else if (_direction == eDirection::LEFT)
	{
		if (this->getScale().x < 0)
			this->setScaleX((-1) * this->getScale().x);
		movement->setVelocity(GVector2(-MUMMYMAN_VELOC.x, MUMMYMAN_VELOC.y));
	}
}


//void MummyMan::changeDirection() 
//{
//	_sprite->setScaleX(-this->getScale().x);
//	Movement *movement = (Movement*)this->getComponent("Movement");
//	movement->setVelocity(GVector2(-movement->getVelocity().x, 0));
//}


float MummyMan::checkCollision(BaseObject * otherObject, float dt)
{
	if (this->getStatus() == eStatus::DESTROY ||
		this->isInStatus(eStatus::DYING) || this->isInStatus(eStatus::BURN))
		return 0.0f;

	auto collisionBody = (CollisionBody*)_listComponent["CollisionBody"];
	eID otherObjectID = otherObject->getId();
	eDirection direction;
	if (otherObjectID != eID::LAND && otherObjectID != eID::SIMON) return 0.0f;

	if (collisionBody->checkCollision(otherObject, direction, dt, false))
	{
		if (otherObjectID == eID::LAND /*&& !_checkedWithLand*/ && direction == eDirection::TOP)
		{
			float moveX, moveY;
			if (collisionBody->isCollidingIntersected(otherObject, moveX, moveY, dt))
			{
				collisionBody->updateTargetPosition(otherObject, direction, false, GVector2(moveX, moveY));
			}
			auto gravity = (Gravity*)this->_listComponent["Gravity"];
			gravity->setStatus(eGravityStatus::SHALLOWED);
			//_checkedWithLand = true;
		}
		else if (otherObjectID == eID::SIMON)
		{
			auto _simon = ((Level3*)SceneManager::getInstance()->getCurrentScene())->getSimon();
			_simon->getHitted();
			this->movingBack();
		}
		
	}
	return 0.0f;
}

void MummyMan::updateCurrentAnimateIndex() 
{

}


GVector2 MummyMan::getVelocity() 
{
	auto move = (Movement*)this->_listComponent["Movement"];
	return move->getVelocity();
}



void MummyMan::getHitted()
{
	if (_isHitted == false && _getHittedStopWatch == nullptr)
	{
		_isHitted = true;
	}
}