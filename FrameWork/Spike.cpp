#include "Spike.h"
#include "PlayScene.h"
Spike::Spike(eSpikeState state, GVector2 pos) :BaseObject(eID::SPIKE) {
	_sprite = SpriteManager::getInstance()->getSprite(eID::SPIKE);
	_sprite->setFrameRect(0, 0, 64.0f, 64.0f);

	this->setPosition(pos);
	this->setScale(SCALE_FACTOR);

	_stopWatch = new StopWatch();
	switch (state) {
	case SPIKE_FALLING_01:
		spikeState=FALLING_01;
		break;
	case SPIKE_FALLING_02:
		spikeState = FALLING_02;
		break;
	case SPIKE_FALLING_03:
		spikeState = FALLING_03;
		break;
	default:
		break;
	}

	/*this->spikeState = status;*/

}

Spike::~Spike() {}

void Spike::init() {
	_animations[eStatus::WAITING] = new Animation(_sprite, 0.15f);
	_animations[eStatus::WAITING]->addFrameRect(eID::SPIKE, "normal", NULL);

	// spike dài nhất :đang test
	_animations[eStatus::FALLING_01] = new Animation(_sprite, 0.15f);
	_animations[eStatus::FALLING_01]->addFrameRect(eID::SPIKE, "fall_02", "fall_03", "fall_04", "fall_05", "fall_06", "fall_07", "fall_08", "fall_07",
		"fall_06", "fall_05", "fall_04", "fall_03", "fall_02", "fall_01", NULL);

	// vừa vừa :v
	_animations[eStatus::FALLING_02] = new Animation(_sprite, 0.15f);
	_animations[eStatus::FALLING_02]->addFrameRect(eID::SPIKE, "fall_02", "fall_03", "fall_04", "fall_05", "fall_06", "fall_07",
		"fall_06", "fall_05", "fall_04", "fall_03", "fall_02", "fall_01", NULL);

	// spike ngắn nhất
	_animations[eStatus::FALLING_03] = new Animation(_sprite, 0.15f);
	_animations[eStatus::FALLING_03]->addFrameRect(eID::SPIKE, "fall_02", "fall_03", "fall_04", "fall_05", "fall_06",  "fall_05", "fall_04",
		"fall_03", "fall_02", "fall_01", NULL);
	

	//*Test
	//this->setPosition(GVector2(300, 200));
	this->setStatus(eStatus::WAITING);
	_sprite->drawBounding(false);

	_stopWatch = new StopWatch();
	_hack = 0;
}

void Spike::update(float deltaTime) {
	if (this->getStatus() == eStatus::DESTROY)
		return;
	if (this->getStatus() == eStatus::WAITING) {
		this->updateWaiting();
		return;
	}


	for (auto it : _listComponent) {
		it.second->update(deltaTime);
	}

	if (this->getStatus() != DESTROY) {
		_animations[this->getStatus()]->update(deltaTime);


		switch (this->getStatus()) {
		case FALLING_01:
			if (_animations[this->getStatus()]->getIndex() >= 6) {
				_animations[this->getStatus()]->setTimeAnimate(0.45f);
			}
			else {
				_animations[this->getStatus()]->setTimeAnimate(0.15f);
			}
			break;
		case FALLING_02:
			if (_animations[this->getStatus()]->getIndex() >= 5) {
				_animations[this->getStatus()]->setTimeAnimate(0.45f);
			}
			else {
				_animations[this->getStatus()]->setTimeAnimate(0.15f);
			}
			break;
		case FALLING_03:
			if (_animations[this->getStatus()]->getIndex() >= 4) {
				_animations[this->getStatus()]->setTimeAnimate(0.45f);
			}
			else {
				_animations[this->getStatus()]->setTimeAnimate(0.15f);
			}
			break;
		default:
			break;
		}
		// Loại dài nhất, lúc nó fall thì nhanh, return thì chậm
		/*if ((this->getStatus() == eStatus::SPIKE_FALLING_01) && _animations[this->getStatus()]->getIndex() >= 6) {
			_animations[this->getStatus()]->setTimeAnimate(0.5f);

		}
		else {
			_animations[this->getStatus()]->setTimeAnimate(0.1f);

		}*/
	}
}

void Spike::draw(LPD3DXSPRITE spriteHandler, Viewport *viewport) {
	_animations[this->getStatus()]->draw(spriteHandler, viewport);

}

void Spike::release() {
	for (auto component : _listComponent) {
		delete component.second;
	}
	_listComponent.clear();

	//SAFE_DELETE(this->_loopwatch);
	SAFE_DELETE(this->_sprite);
	SAFE_DELETE(this->_stopWatch);

}

void Spike::onCollisionBegin(CollisionEventArg *) {

}

void Spike::onCollisionEnd(CollisionEventArg *) {

}

float Spike::checkCollision(BaseObject *, float) {
	return 0.0f;
}

void Spike::setPosition(GVector2 pos) {
	_sprite->setPosition(pos);

}

IComponent * Spike::getComponent(string componentName) {
	return _listComponent.find(componentName)->second;
}


void Spike::updateWaiting() {
	auto viewport = ((PlayScene*)SceneManager::getInstance()->getCurrentScene())->getViewport();
	RECT screenBound = viewport->getBounding();

	if (this->getPositionY() < 0) {
		this->setStatus(eStatus::WAITING);
	}
	else {
		// cạnh phải spike >cạnh trái view-->falling
		if (this->getBounding().left < screenBound.right) {
			this->setStatus(spikeState);
		}
	}
}
