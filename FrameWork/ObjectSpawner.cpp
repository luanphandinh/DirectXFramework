#include "ObjectSpawner.h"
#include "PlayScene.h"
ObjectSpawner::ObjectSpawner(GVector2 position, int width, int height, eID type, int direction, float time, int number)
	: EmptyObject(eID::SPAWNER, position, width, height) {
	_createType = type;
	_time = time;
	_number = number;

	_stopWatch = new StopWatch();

	_direction = direction;
	_isOnePerOne = false;

	_maxObject = 3;

}

ObjectSpawner::~ObjectSpawner() {}

void ObjectSpawner::init() {

}

void ObjectSpawner::update(float deltatime) {
	auto vpBounding = SceneManager::getInstance()->getCurrentScene()->getViewport()->getBounding();

	// check coi đi tới chưa, chưa tới mới tạo
	if (this->getPositionX() > vpBounding.left) {
		if (_stopWatch->isStopWatch(_time)) {
			if (_number != -1 && _counter < _number) {
				_counter++;
				_listObjects.push_back(getObject(_createType));

				if (_counter < _number)
					_stopWatch->restart();
			}
			else if (_number == -1) {
				if (_listObjects.size() < _maxObject)
					_listObjects.push_back(getObject(_createType));
					_stopWatch->restart();
			}
		}		
	}
	//else if (this->getPositionX() <= vpBounding.left || this->getPositionY() <= vpBounding.bottom) {
	//	// Trong màn hình rồi thì huỷ luôn
	//	// qua luôn rồi thì hủy
	//	this->setStatus(eStatus::DESTROY);
	//	return;
	//}

	for (auto object : _listObjects) {
		object->update(deltatime);

		/*if (object->getScale().x > 0 && object->getPositionX() < vpBounding.left) {
			object->setStatus(eStatus::DESTROY);
		}
		else if (object->getScale().x < 0 && object->getPositionX() > vpBounding.right) {
			object->setStatus(eStatus::DESTROY);
		}
		else if (object->getPositionY() > vpBounding.top || object->getPositionY() < vpBounding.bottom) {
			object->setStatus(eStatus::DESTROY);
		}*/
	}

	this->deleteObject();
}

void ObjectSpawner::draw(LPD3DXSPRITE spriteHandle, Viewport * viewport) {
	for (auto object : _listObjects) {
		object->draw(spriteHandle, viewport);
	}
}

void ObjectSpawner::release() {
	SAFE_DELETE(_stopWatch);

	for (auto object : _listObjects) {
		SAFE_DELETE(object);
	}

	_listObjects.clear();
}

float ObjectSpawner::checkCollision(BaseObject * object, float dt) {
	if (object->getId() != eID::LAND && object->getId() != eID::SIMON && object->getId() != eID::WHIP) {
		return 0.0f;
	}

	for (auto item : _listObjects) {
		item->checkCollision(object, dt);
	}

	return 0.0f;
}

BaseObject * ObjectSpawner::getObject(eID id) {
	switch (id) {
	case MEDUSAHEAD:
	{
		auto medusaHead = new MedusaHead(eStatus::FLYING, _direction, this->getPosition(),
			MEDUSAHEAD_HORIZONTAL_VELOC,MEDUSAHEAD_AMPLITUDE,MEDUSAHEAD_FREQUENCY);
		medusaHead->init();
		auto pos = medusaHead->getPosition();
		return medusaHead;
		break;
	}
	default:
		break;
	}
}

void ObjectSpawner::deleteObject() {
	for (auto object : _listObjects) {
		if (object->getStatus() == eStatus::DESTROY) {
			object->release();

			remove(_listObjects.begin(), _listObjects.end(), object);
			_listObjects.pop_back();

			delete object;

			break;
		}
	}
}

RECT ObjectSpawner::getBounding() {
	return RECT();
}

vector<BaseObject*> ObjectSpawner::getObjects() {
	return _listObjects;
}

void ObjectSpawner::setOnePerOne(bool enable) {
	_isOnePerOne = enable;
}

bool ObjectSpawner::isOnePerOne() {
	return _isOnePerOne;
}

void ObjectSpawner::setMaxNumber(int number) {
	_maxObject = number;
}

int ObjectSpawner::getMaxNumber() {
	return _maxObject;
}




