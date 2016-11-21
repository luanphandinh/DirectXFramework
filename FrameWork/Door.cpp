//#include "Door.h"
//
//Door::Door(eStatus status, GVector2 pos, int direction) :BaseObject(eID::DOOR){
//	_sprite = SpriteManager::getInstance()->getSprite(eID::DOOR);
//	_sprite->setFrameRect(0, 0, 32.0f, 16.0f);
//
//	/*GVector2 v(direction * BAT_SPEED, 0);
//	GVector2 a(0, 0);*/
//	//this->_listComponent.insert(pair<string, IComponent*>("Movement", new Movement(a, v, this->_sprite)));
//	this->setStatus(status);
//	this->setPosition(pos);
//	this->setScale(SCALE_FACTOR);
//	this->setScaleX(direction * SCALE_FACTOR);
//}
//
//
//Door::~Door() {}
//
//void Door::init() {
//}
//
//void Door::update(float) {
//}
//
//void Door::draw(LPD3DXSPRITE, Viewport *) {
//}
//
//void Door::release() {
//}
//
//void Door::updateClosing() {
//}
