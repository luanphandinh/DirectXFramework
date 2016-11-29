#include "Door.h"
#include "PlayScene.h"
Door::Door(eStatus status, GVector2 pos, int direction) :BaseObject(eID::DOOR){
	_sprite = SpriteManager::getInstance()->getSprite(eID::DOOR);
	_sprite->setFrameRect(0, 0, 32.0f, 16.0f);

	this->setStatus(status);
	this->setPosition(pos);
	this->setScale(SCALE_FACTOR);
	this->setScaleX(direction * SCALE_FACTOR);
}


Door::~Door() {}

void Door::init() {
	_animations[eStatus::CLOSING] = new Animation(_sprite, 0.1f);
	_animations[eStatus::CLOSING]->addFrameRect(eID::DOOR, "normal", NULL);

	_animations[OPENING] = new Animation(_sprite, 0.5f);
	_animations[OPENING]->addFrameRect(eID::DOOR, "open_02", "open_03", "open_02","open_01", NULL);

	//*Test
	//this->setPosition(GVector2(300, 200));
	this->setStatus(eStatus::CLOSING);
	_sprite->drawBounding(false);

	_stopWatch = new StopWatch();
	_hack = 0;
}

void Door::update(float deltaTime) {
	if (this->getStatus() == eStatus::DESTROY)
		return;
	if (this->getStatus() == eStatus::CLOSING) {
		this->updateClosing();
		return;
	}
	else {
		_hack++;
		//HACK :V
		if (_hack >= 60)
			this->setStatus(CLOSING);
		else {
			/*this->checkIfOutOfScreen();*/
			for (auto component : _listComponent) {

				component.second->update(deltaTime);
			}
			_animations[this->getStatus()]->update(deltaTime);
		}
		
	}
}

void Door::draw(LPD3DXSPRITE spriteHandler, Viewport *viewport) {
	_animations[this->getStatus()]->draw(spriteHandler, viewport);
}

void Door::release() {
	for (auto component : _listComponent) {
		delete component.second;
	}
	_listComponent.clear();

	//SAFE_DELETE(this->_loopwatch);
	SAFE_DELETE(this->_sprite);
}

void Door::setPosition(GVector2 pos) {
	_sprite->setPosition(pos);

}

IComponent * Door::getComponent(string componentName) {
	return _listComponent.find(componentName)->second;
}

void Door::changeDirection() {
	_sprite->setScaleX(-this->getScale().x);
}

void Door::updateClosing() {
	
}
