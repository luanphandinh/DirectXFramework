#include "Door.h"
#include "PlayScene.h"
Door::Door(eStatus status, GVector2 pos, int direction) :BaseObject(eID::DOOR){
	_sprite = SpriteManager::getInstance()->getSprite(eID::DOOR);
	_sprite->setFrameRect(0, 0, 32.0f, 16.0f);
	this->setStatus(status);
	this->setPosition(pos);
	this->setScale(1.0f);
	this->setScaleX(direction * 1.0f);
}


Door::~Door() {}

void Door::init() {
	_animations[eStatus::CLOSING] = new Animation(_sprite, 0.1f);
	_animations[eStatus::CLOSING]->addFrameRect(eID::DOOR, "normal", NULL);

	_animations[OPENING] = new Animation(_sprite, 0.5f);
	_animations[OPENING]->addFrameRect(eID::DOOR, "open_02", "open_03", "open_02","open_01", NULL);

	_animations[CLOSED] = new Animation(_sprite, 0.5f);
	_animations[CLOSED]->addFrameRect(eID::DOOR, "open_01", NULL);
	//*Test
	//this->setPosition(GVector2(300, 200));
	this->setStatus(eStatus::CLOSING);
	_sprite->drawBounding(false);

	_stopwatch = new StopWatch();
	this->aniCount = 0;
	_stopwatch->restart();
}

void Door::update(float deltaTime) {
	//if (!isVirgin) this->setStatus(CLOSED);


	if (this->getStatus() == eStatus::DESTROY|| this->getStatus() == eStatus::CLOSED)
		return;
	if (this->getStatus() == eStatus::CLOSING) {
		this->updateClosing();
		return;
	}
	else {
		aniCount++;
		//HACK :V
		if (aniCount == 30)
			this->setStatus(CLOSED);
		else {
			/*this->checkIfOutOfScreen();*/
			for (auto component : _listComponent) {

				component.second->update(deltaTime);
			}
			_animations[this->getStatus()]->update(deltaTime);
		}
	}
}

void Door::draw(LPD3DXSPRITE spriteHandler, Viewport * viewport) {
	_animations[this->getStatus()]->draw(spriteHandler, viewport);
}

void Door::release() {
	for (auto component : _listComponent) {
		delete component.second;
	}
	_listComponent.clear();

	SAFE_DELETE(this->_sprite);
}

void Door::setPosition(GVector2 pos) {
	_sprite->setPosition(pos);
}


IComponent * Door::getComponent(string) {
	return nullptr;
}

void Door::updateClosing() {
	// track theo simon
	auto objectTracker = ((PlayScene*)SceneManager::getInstance()->getCurrentScene())->getSimon();
	RECT objectBound = objectTracker->getBounding();
	int x = objectTracker->getPositionX();
	int y = objectTracker->getPositionY();
	// vào đúng cái khoảng này mới mở cửa :v
	if (x < 2100 && x>2070 && y < 700 && y>660) this->setStatus(OPENING);
	else this->setStatus(CLOSING);

}
