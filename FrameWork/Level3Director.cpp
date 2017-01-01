#include "Level3Director.h"
#include "Level3.h"
#include"ItemManager.h"
#include"Level3.h"

Level3Director::Level3Director() : Director() {
	_reviveViewport = eLevel2Viewport::V1;
	_revivePosition = GVector2(4940, 95);
}


Level3Director::~Level3Director() {}

void Level3Director::init() {
	this->loadStageInfo("Resources//Maps//level3ViewportInfo.txt", eID::LEVEL3);
	_viewport = new Viewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	this->setCurrentViewport(V1);

	auto scenarioPassDoor = new Scenario("PassDoor");
	////Mấy cái hàm này chạy theo thứ tự từ dưới lên khi gọi updateScenario(deltaTime);
	__hook(&Scenario::update, scenarioPassDoor, &Level3Director::moveViewportPassDoor2);
	__hook(&Scenario::update, scenarioPassDoor, &Level3Director::passDoorScene);
	__hook(&Scenario::update, scenarioPassDoor, &Level3Director::moveViewportPassDoor);

	//auto scenarioSpecialItem = new Scenario("crownShowUp");
	//__hook(&Scenario::update, scenarioSpecialItem, &Level3Director::crownShowUp);

	_flagMoveSimonPassDoor = false;
	_flagMoveViewportPassDoor = false;
	_flagMoveViewportPassDoor2 = false;
	////_scenarioManager->insertScenario(scenarioSpecialItem);
	_scenarioManager->insertScenario(scenarioPassDoor);


}
void Level3Director::update(float deltaTime) {
	updateScenario(deltaTime);
	updateViewport();
}

void Level3Director::updateScenario(float deltaTime) {
	_scenarioManager->update(deltaTime);
}

void Level3Director::updateViewport() {
	GVector2 pos = _objTracker->getPosition();

	if (_flagMoveViewportPassDoor || _flagMoveSimonPassDoor || _flagMoveViewportPassDoor2) return;

	Level3Director::switchViewport();
	// Vị trí hiện tại của viewport. 
	GVector2 current_position = _viewport->getPositionWorld();

	//GVector2 worldsize = this->_backGround->getWorldSize();
	GVector2 boundSize = this->getCurrentViewportBound();
	// Bám theo object.ko cho nhân vật vượt quá biên trái vủa vùng viewport hiện đang xét
	GVector2 new_position = GVector2(max(_objTracker->getPositionX() - WINDOW_WIDTH / 2, boundSize.x),
		current_position.y);

	// Không cho đi quá biên bên phải của vùng viewport hiện tại hiện tại.
	if (new_position.x + WINDOW_WIDTH > boundSize.y) {
		new_position.x = boundSize.y - WINDOW_WIDTH;
	}

	_viewport->setPositionWorld(new_position);
}


void Level3Director::switchViewport() {
	GVector2 pos = _objTracker->getPosition();

	switch (_currentViewport) {
	case eLevel2Viewport::V1:
		if (pos.y > 384 && (Simon*)_objTracker->isInStatus(eStatus::STANDINGONSTAIR)) {
			this->setCurrentViewport(V2);
			_objTracker->setPosition(2276, 416);
		}
		break;
	case eLevel2Viewport::V2:
		if (pos.y < 414 && (Simon*)_objTracker->isInStatus(eStatus::STANDINGONSTAIR))
		{
			//this->setCurrentViewport(V1);
			this->setCurrentViewport(V1);
			_objTracker->setPosition(3780, 382);
		}

		if (pos.x > this->getCurrentViewportBound().y)
		{
			this->setCurrentViewport(V3);
			this->_reviveViewport = V3;
			this->_revivePosition = GVector2(3560,640);
		}
		break;
	case eLevel2Viewport::V3:
		if (pos.y > 768 && (Simon*)_objTracker->isInStatus(eStatus::STANDINGONSTAIR)) {
			//this->setCurrentViewport(V1);
			this->setCurrentViewport(V4);
			_objTracker->setPosition(720, 812);
		}
		break;
	case eLevel2Viewport::V4:
		if (pos.y < 810 && (Simon*)_objTracker->isInStatus(eStatus::STANDINGONSTAIR)) {
			//this->setCurrentViewport(V1);
			this->setCurrentViewport(V3);
			_objTracker->setPosition(4800, 768);
		}
		if (pos.x > this->getCurrentViewportBound().y)
		{
			this->setCurrentViewport(V5);
			this->_reviveViewport = V5;
			this->_revivePosition = GVector2(2050, 1050);
		}

		break;
	case eLevel2Viewport::V5:
		if (pos.x > 4700)
		{
			this->setCurrentViewport(V6);
			//this->_reviveViewport = V6;
		}

		break;
	default:
		break;
	}
}
#pragma region passdoor
bool Level3Director::checkPosition()
{
	if (_trackedDoor == nullptr)
	{
		auto door = ((Level3*)SceneManager::getInstance()->getCurrentScene())->getObject(eID::DOOR);
		if (door == nullptr)
			return false;
		_trackedDoor = door;
	}
	auto _simon = ((Level3*)SceneManager::getInstance()->getCurrentScene())->getSimon();
	int xSimon = _simon->getPositionX();
	int ySimon = _simon->getPositionY();
	GVector2 doorPos = _trackedDoor->getPosition();
	if ((xSimon < doorPos.x && xSimon > doorPos.x - 20 && ySimon < doorPos.y && ySimon> doorPos.y - 100))
	{
		return true;
	}
	return false;
}

bool Level3Director::isPassedDoor()
{
	if (_trackedDoor == nullptr)
	{
		auto door = ((Level3*)SceneManager::getInstance()->getCurrentScene())->getObject(eID::DOOR);
		if (door == nullptr)
			return false;
		_trackedDoor = door;
	}
	int xSimon = _objTracker->getPositionX();
	int ySimon = _objTracker->getPositionY();
	GVector2 doorPos = _trackedDoor->getPosition();
	if (xSimon > doorPos.x + 120)
	{
		//_trackedDoor = nullptr;
		return true;
	}
	return false;
}


void Level3Director::passDoorScene(float deltatime, bool & isFinish)
{
	if (_flagMoveViewportPassDoor) return;

	if (checkPosition()) {
		((Door*)_trackedDoor)->open();
		_flagMoveSimonPassDoor = true;
	}
	if (_flagMoveSimonPassDoor)
		((Simon*)_objTracker)->forceMoveRight();

	if (isPassedDoor() && _flagMoveSimonPassDoor) {
		((Simon*)_objTracker)->unforceMoveRight();
		_trackedDoor->setStatus(CLOSING);
		_flagMoveSimonPassDoor = false;
	}


}

void Level3Director::moveViewportPassDoor(float deltatime, bool & finish) {
	
	if (!checkPosition()) return;
	if (checkPosition())
	{
		((Simon*)_objTracker)->unforceMoveRight();
		((Simon*)_objTracker)->unforceJump();
		((Simon*)_objTracker)->setFreeze(true);
		_flagMoveViewportPassDoor = true;
	}


	GVector2 boundSize = this->getCurrentViewportBound();

	GVector2 current_position = _viewport->getPositionWorld();
	// dịch screen từ từ sang TRÁI, speed = vs speed simon
	current_position.x += SIMON_MOVING_SPEED * deltatime / 1000;

	if (current_position.x > boundSize.y - WINDOW_WIDTH / 2)
	{
		current_position.x = boundSize.y - WINDOW_WIDTH / 2;
		_flagMoveViewportPassDoor = false;
	}

	_viewport->setPositionWorld(current_position);
}

void Level3Director::moveViewportPassDoor2(float deltatime, bool & finish)
{
	GVector2 current_position = _viewport->getPositionWorld();

	GVector2 boundSize = this->getCurrentViewportBound();

	if ((!_flagMoveSimonPassDoor && !_flagMoveViewportPassDoor && current_position.x >= boundSize.y - WINDOW_WIDTH/2) == false) return;

	_flagMoveViewportPassDoor2 = true;

	// dịch screen từ từ sang TRÁI, speed = vs speed simon
	current_position.x += SIMON_MOVING_SPEED * deltatime / 1000;
	auto _simon = ((Level3*)SceneManager::getInstance()->getCurrentScene())->getSimon();
	if (current_position.x > boundSize.y) {
		current_position.x = boundSize.y;
		_flagMoveViewportPassDoor2 = false;
		_simon->setFreeze(false);
		_trackedDoor->setStatus(eStatus::DESTROY);
		_trackedDoor = nullptr;
	}

	_viewport->setPositionWorld(current_position);
}

#pragma endregion 


void Level3Director::crownShowUp(float deltatime, bool & finish) {
	auto _simon = ((Level3*)SceneManager::getInstance()->getCurrentScene())->getSimon();
	int xSimon = _simon->getPositionX();
	int ySimon = _simon->getPositionY();
	if (xSimon > 3010 && xSimon < 3050 && ySimon > 192 && ySimon < 260) {
		ItemManager::generateItem(eItemID::CROWN, GVector2(2900, 350));
		//finish = true;
	}

}


void Level3Director::generateCrystalBall()
{
	if (_generateCrysballStopWatch == nullptr)
	{
		_generateCrysballStopWatch = new StopWatch();
		return;
	}

	if (_generateCrysballStopWatch->isStopWatch(2000))
	{
		ItemManager::generateItem(eItemID::CRYSTALBALL, GVector2(1800, 1500));
		SAFE_DELETE(_generateCrysballStopWatch);
	}
}

void Level3Director::endLevel()
{

}

void Level3Director::release()
{}