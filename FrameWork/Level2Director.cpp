#include "Level2Director.h"
#include "PlayScene.h"

Level2Director::Level2Director() : Director()
{
	_reviveViewport = eLevel2Viewport::V1;
	_revivePosition = GVector2(2700, 100);
	// -_-  
	//_revivePosition = GVector2(2300, 638);

}


Level2Director::~Level2Director()
{
}

void Level2Director::init()
{
	this->loadStageInfo("Resources//Maps//level2ViewportInfo.txt", eID::LEVEL2);
	_viewport = new Viewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	this->setCurrentViewport(V1);

	auto scenarioPassDoor = new Scenario("PassDoor");
	//Mấy cái hàm này chạy theo thứ tự từ dưới lên khi gọi updateScenario(deltaTime);
	__hook(&Scenario::update, scenarioPassDoor, &Level2Director::moveViewportPassDoor2);
	__hook(&Scenario::update, scenarioPassDoor, &Level2Director::passDoorScene);
	__hook(&Scenario::update, scenarioPassDoor, &Level2Director::moveViewportPassDoor);
	
	_flagMoveSimonPassDoor = false;
	_flagMoveViewportPassDoor = false;
	_flagMoveViewportPassDoor2 = false;
	_scenarioManager->insertScenario(scenarioPassDoor);


}
void Level2Director::update(float deltaTime) 
{
	updateScenario(deltaTime);
	updateViewport();
}

void Level2Director::updateScenario(float deltaTime)
{
	_scenarioManager->update(deltaTime);
}

void Level2Director::updateViewport()
{
	GVector2 pos = _objTracker->getPosition();
	
	if (_flagMoveViewportPassDoor || _flagMoveSimonPassDoor || _flagMoveViewportPassDoor2) return;

	Level2Director::switchViewport();
	// Vị trí hiện tại của viewport. 
	GVector2 current_position = _viewport->getPositionWorld();

	//GVector2 worldsize = this->_backGround->getWorldSize();
	GVector2 boundSize = this->getCurrentViewportBound();
	// Bám theo object.ko cho nhân vật vượt quá biên trái vủa vùng viewport hiện đang xét
	GVector2 new_position = GVector2(max(_objTracker->getPositionX() - WINDOW_WIDTH / 2, boundSize.x),
		current_position.y);

	// Không cho đi quá biên bên phải của vùng viewport hiện tại hiện tại.
	if (new_position.x + WINDOW_WIDTH > boundSize.y)
	{
		new_position.x = boundSize.y - WINDOW_WIDTH;
	}

	_viewport->setPositionWorld(new_position);
}


void Level2Director::switchViewport()
{
	GVector2 pos = _objTracker->getPosition();
	
	switch (_currentViewport)
	{
	case eLevel2Viewport::V1:
		if (pos.y > 384 && (Simon*)_objTracker->isInStatus(eStatus::STANDINGONSTAIR))
		{
			this->setCurrentViewport(V2);
			_objTracker->setPosition(2848, 450);
		}
		break;
	case eLevel2Viewport::V2:
		if (pos.y < 440 && (Simon*)_objTracker->isInStatus(eStatus::STANDINGONSTAIR))
		{
			//this->setCurrentViewport(V1);
			this->setCurrentViewport(V1);
			_objTracker->setPosition(2816, 380);
		}

		if (pos.x < this->getCurrentViewportBound().x) 
		{
			this->setCurrentViewport(V3);
		}
		break;
	case eLevel2Viewport::V3:
		if (pos.y > 768 && (Simon*)_objTracker->isInStatus(eStatus::STANDINGONSTAIR)) {
			//this->setCurrentViewport(V1);
			this->setCurrentViewport(V4);
			_objTracker->setPosition(1650, 830);
		}
		break;
	case eLevel2Viewport::V4:
		if (pos.y < 810 && (Simon*)_objTracker->isInStatus(eStatus::STANDINGONSTAIR)) {
			//this->setCurrentViewport(V1);
			this->setCurrentViewport(V3);
			_objTracker->setPosition(646, 750);
		}
		break;
	default:
		break;
	}
}


void Level2Director::passDoorScene(float deltatime, bool & isFinish) 
{
	if (_flagMoveViewportPassDoor) return;
	auto door =  ((PlayScene*)SceneManager::getInstance()->getCurrentScene())->getObject(eID::DOOR);
	if (door == nullptr)
		return;
	auto _simon = ((PlayScene*)SceneManager::getInstance()->getCurrentScene())->getSimon();
	// track theo simon
	int xSimon = _simon->getPositionX();
	int ySimon = _simon->getPositionY();
	int xthis = door->getPositionX();
	int ythis = door->getPositionY();
	////test :v
	//if (x > xthis&&x < xthis + 50 && y<ythis+50&&y>ythis - 50) {
	//	this->setStatus(OPENING);
	//}
	//else {
	//	this->setStatus(CLOSING);
	//}
	if (xSimon < 2100 && xSimon>2070 && ySimon < 700 && ySimon>660) {
		door->setStatus(OPENING);
		((Simon*)_simon)->forceMoveLeft();
		_flagMoveSimonPassDoor = true;
	}
	else if (xSimon < 1950 && ySimon < 700 && ySimon>660 && _flagMoveSimonPassDoor) {
		((Simon*)_simon)->unforceMoveLeft();
		door->setStatus(CLOSING);
		_flagMoveSimonPassDoor = false;
	}
	
}

void Level2Director::moveViewportPassDoor(float deltatime, bool & finish) {
	auto _simon = ((PlayScene*)SceneManager::getInstance()->getCurrentScene())->getSimon();
	int xSimon = _simon->getPositionX();
	int ySimon = _simon->getPositionY();

	
	if (_currentViewport != eLevel2Viewport::V2 || !((xSimon < 2100 && xSimon>2070 && ySimon < 700 && ySimon>660))) return;
	if(xSimon < 2100 && xSimon>2070 && ySimon < 700 && ySimon>660)
		_flagMoveViewportPassDoor = true;

	GVector2 boundSize = this->getCurrentViewportBound();

	GVector2 current_position = _viewport->getPositionWorld();
	// dịch screen từ từ sang TRÁI, speed = vs speed simon
	current_position.x -= SIMON_MOVING_SPEED * deltatime / 1000;
	
	if (current_position.x < boundSize.x - WINDOW_WIDTH / 2)
	{
		current_position.x = boundSize.x - WINDOW_WIDTH / 2;
		_flagMoveViewportPassDoor = false;
	}
		
	_viewport->setPositionWorld(current_position);
}

void Level2Director::moveViewportPassDoor2(float deltatime, bool & finish) 
{
	GVector2 current_position = _viewport->getPositionWorld();

	GVector2 boundSize = this->getCurrentViewportBound();

	if((!_flagMoveSimonPassDoor && !_flagMoveViewportPassDoor && current_position.x < boundSize.x) == false) return;

	_flagMoveViewportPassDoor2 = true;

	// dịch screen từ từ sang TRÁI, speed = vs speed simon
	current_position.x -= SIMON_MOVING_SPEED * deltatime / 1000;

	if (current_position.x < boundSize.x - WINDOW_WIDTH) {
		current_position.x = boundSize.x - WINDOW_WIDTH;
		_flagMoveViewportPassDoor2 = false;
	}

	_viewport->setPositionWorld(current_position);
}