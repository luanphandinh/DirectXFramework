#include "MedusaHead.h"
#include "PlayScene.h"
MedusaHead::MedusaHead(eStatus status, int direction, GVector2 pos, 
	GVector2 hVeloc, GVector2 ampl, float freq) : BaseEnemy(eID::BAT) {
	this->_beginPosition = pos;
	/*this->_horizontalVeloc = hVeloc;
	this->_amplitude = ampl;
	this->_frequence = freq;*/
	_direction = direction;
	if (direction == 1)
		this->_horizontalVeloc = GVector2(160.0f, 0.0f);
	else if (direction == -1) 
		this->_horizontalVeloc = GVector2(-160.0f, 0.0f);
	this->_amplitude = GVector2(0.0f, 90.0f);
	this->_frequence = 0.7f;
	_sprite = SpriteManager::getInstance()->getSprite(eID::MEDUSAHEAD);
	_sprite->setFrameRect(SpriteManager::getInstance()->getSourceRect(this->_id, "normal"));

	this->_listComponent.insert(pair<string, IComponent*>("Movement",
		new Movement(GVector2Zero, _horizontalVeloc, _sprite)));
	this->_listComponent.insert(pair<string, IComponent*>("Sinmovement", 
		new SinMovement(_amplitude, _frequence, _sprite)));
	
	this->setStatus(status);
	this->setPosition(pos);
	this->setScale(SCALE_FACTOR);
	this->setScaleX(direction * SCALE_FACTOR);
}

MedusaHead::~MedusaHead() {}

void MedusaHead::init() {
	this->setHitpoint(MEDUSAHEAD_HITPOINT);
	this->setScore(MEDUSAHEAD_SCORE);

	auto collisionBody = new CollisionBody(this);
	_listComponent["CollisionBody"] = collisionBody;

	_animations[eStatus::HIDING] = new Animation(_sprite, 0.1f);
	_animations[eStatus::HIDING]->addFrameRect(eID::MEDUSAHEAD, "normal", NULL);

	_animations[FLYING] = new Animation(_sprite, 0.15f);
	_animations[FLYING]->addFrameRect(eID::MEDUSAHEAD, "fly_01", "fly_02", NULL);

	_animations[DYING] = new Animation(_sprite, 0.15f);
	_animations[DYING]->addFrameRect(eID::MEDUSAHEAD, NULL);

	//*Test
	//this->setPosition(GVector2(300, 200));
	this->setStatus(eStatus::HIDING);
	_sprite->drawBounding(false);
}

void MedusaHead::update(float deltaTime) {

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
	if (this->checkIfOutOfScreen())
		return;

	if (this->getStatus() == eStatus::HIDING) {
		this->updateHiding();
		return;
	}
	else {

		
		for (auto component : _listComponent) {

			component.second->update(deltaTime);
		}
		_animations[this->getStatus()]->update(deltaTime);

	}

}

void MedusaHead::draw(LPD3DXSPRITE spriteHandler, Viewport* viewport) {
	if (_burning != nullptr)
		_burning->draw(spriteHandler, viewport);
	if (this->isInStatus(eStatus::DESTROY) || this->isInStatus(eStatus::BURN)) return;
	_animations[this->getStatus()]->draw(spriteHandler, viewport);

}

void MedusaHead::release() {
	for (auto component : _listComponent) {
		delete component.second;
	}
	_listComponent.clear();
	if (_burning != nullptr)
		_burning->release();
	SAFE_DELETE(_burning);
	SAFE_DELETE(this->_sprite);
}

float MedusaHead::checkCollision(BaseObject *object, float deltaTime) {
	if (this->getStatus() == eStatus::DESTROY ||
		this->isInStatus(eStatus::DYING) || this->isInStatus(eStatus::BURN))
		return 0.0f;

	if (this->getStatus() == eStatus::DESTROY ||
		this->isInStatus(eStatus::DYING))
		return 0.0f;

	auto collisionBody = (CollisionBody*)_listComponent["CollisionBody"];
	eID objectId = object->getId();
	if (objectId != eID::SIMON && objectId != eID::WHIP && objectId != eID::ITEM)
		return 0.0f;
	eDirection direction;
	if (collisionBody->checkCollision(object, direction, deltaTime, false)) {
		if (objectId == eID::SIMON) {
		
			//auto movement = (Movement*)this->_listComponent["Movement"];

			//if (object->isInStatus(eStatus::HITTING)) {
				// bị chớp
				//if (_stopWatch == nullptr) _stopWatch = new StopWatch();
				/*if (_stopWatch->isStopWatch(200)) {
					this->_animations[this->getStatus()]->enableFlashes(true);
					_animations[this->getStatus()]->setColorFlash(D3DXCOLOR(1.0f, 0.5f, 0.5f, 1));*/
					//movement->setVelocity(GVector2(0, 0));
				//}

				/*this->dropHitpoint(1);
				_isHitted = true;*/
			//}
			//else {
				((Simon*)object)->getHitted();
				//movement->setVelocity(GVector2(movement->getVelocity().x, 0));
			/*	this->_animations[this->getStatus()]->enableFlashes(false);
				_isHitted = false;
			}*/
		}
		else if (objectId == eID::WHIP && ((Whip*)object)->isHitting()) {
			this->setStatus(eStatus::BURN);
		}
		else if (objectId == eID::ITEM && ((Item*)object)->getItemType() == eItemType::PICKED_UP)
		{
			this->setStatus(eStatus::BURN);
		}

		return 0.0f;
	}
}

void MedusaHead::die() {

}

void MedusaHead::setPosition(GVector2 pos) {
	_sprite->setPosition(pos);

}

IComponent * MedusaHead::getComponent(string) {
	return nullptr;
}

// Ko giết nó, để nó bay ra khỏi view thì hủy luôn
bool MedusaHead::checkIfOutOfScreen() {
	auto viewport = ((PlayScene*)SceneManager::getInstance()->getCurrentScene())->getViewport();
	RECT screenBound = viewport->getBounding();
	GVector2 vpBound = ((Scene*)SceneManager::getInstance()->getCurrentScene())->getDirector()->getCurrentViewportBound();
	GVector2 position = this->getPosition();

	if (position.x > screenBound.right && _direction > 0) {
		this->setStatus(eStatus::DESTROY);
		return true;
	}
	else if (position.x < screenBound.left && _direction < 0) {
		this->setStatus(eStatus::DESTROY);
		return true;
	}

	if (position.x < vpBound.x + 85) {
		this->setStatus(eStatus::DESTROY);
		return true;
	}

	if (position.x > vpBound.y - 85) {
		this->setStatus(eStatus::DESTROY);
		return true;
	}

	return false;
}

void MedusaHead::updateHiding() {
	auto viewport = ((PlayScene*)SceneManager::getInstance()->getCurrentScene())->getViewport();
	RECT screenBound = viewport->getBounding();

	if (this->getPositionY() < 0) {
		this->setStatus(eStatus::HIDING);
	}
	else {
		// cạnh trái con medu < cạnh phải view-->bay
		if (this->getBounding().left< screenBound.right) {
			this->setStatus(eStatus::FLYING);
		}
	}

}

