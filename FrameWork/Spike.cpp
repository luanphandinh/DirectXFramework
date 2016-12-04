#include "Spike.h"
#include "PlayScene.h"
Spike::Spike(eStatus status, GVector2 pos) :BaseObject(eID::SPIKE) {
	_sprite = SpriteManager::getInstance()->getSprite(eID::SPIKE);
	_sprite->setFrameRect(0, 0, 32.0f, 32.0f);
	// rớt xuống
	GVector2 v(0, 0);
	GVector2 a(0, 0);
	this->_listComponent.insert(pair<string, IComponent*>("Movement", new Movement(a, v, this->_sprite)));
	this->setStatus(status);
	this->setPosition(pos);
	this->setScale(SCALE_FACTOR);
	this->setScaleX(SCALE_FACTOR);

	_stopWatch = new StopWatch();
}

Spike::~Spike() {}

void Spike::init() {
	_animations[eStatus::WAITING] = new Animation(_sprite, 0.1f);
	_animations[eStatus::WAITING]->addFrameRect(eID::SPIKE, "normal", NULL);
	// spike dài nhất :đang test
	_animations[eStatus::SPIKE_FALLING_01] = new Animation(_sprite, 0.5f);
	_animations[eStatus::SPIKE_FALLING_01]->addFrameRect(eID::SPIKE, "fall_02", "fall_03", "fall_04", "fall_05", "fall_06", "fall_07", "fall_08", "fall_07",
		"fall_06", "fall_05", "fall_04", "fall_03", "fall_02", "fall_01", NULL);

	// spike ngắn nhất
	/*_animations[FALLING_02] = new Animation(_sprite, 0.5f);
	_animations[FALLING_02]->addFrameRect(eID::SPIKE, "fall_02", "fall_03", "fall_04", "fall_03", "fall_02", "fall_01", NULL);*/

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
	//else {

	//if ((this->getStatus() == eStatus::FALLING_01) && _animations[_currentAnimateIndex]->getIndex() == 6) {
	//	auto movement = (Movement*)this->_listComponent["Movement"];
	//	movement->setVelocity(GVector2(0, SPEED_RETURN));

	//}

	for (auto it : _listComponent) {
		it.second->update(deltaTime);
	}

	if (this->getStatus() != DESTROY) {
		/*this->updateCurrentAnimateIndex();

		_animations[_currentAnimateIndex]->update(deltaTime);*/
		_animations[this->getStatus()]->update(deltaTime);

	}

	/*}*/
}

void Spike::draw(LPD3DXSPRITE spriteHandler, Viewport *viewport) {
	//_animations[_currentAnimateIndex]->draw(spriteHandler, viewport);
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

void Spike::updateCurrentAnimateIndex() {


	if (this->isInStatus(eStatus::WAITING)) {
		_currentAnimateIndex = eStatus::WAITING;
	}
	if (_currentAnimateIndex & eStatus::SPIKE_FALLING_01) {
		_currentAnimateIndex = eStatus::SPIKE_FALLING_01;
	}
}

void Spike::updateWaiting() {
	auto viewport = ((PlayScene*)SceneManager::getInstance()->getCurrentScene())->getViewport();
	RECT screenBound = viewport->getBounding();

	if (this->getPositionY() < 0) {
		this->setStatus(eStatus::WAITING);
	}
	else {
		// cạnh phải spike >cạnh trái view-->falling
		if (this->getBounding().right > screenBound.left) {
			this->setStatus(eStatus::SPIKE_FALLING_01);
		}
	}
}
