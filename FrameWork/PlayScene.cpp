#include "PlayScene.h"


PlayScene::PlayScene()
{
}


PlayScene::~PlayScene()
{
	delete _viewport;
	_viewport = nullptr;
}


bool PlayScene::init() 
{
	_simon = new Simon();
	_simon->init();
	//this->_simon->setPosition(2700, 100);
	this->_simon->setPosition(2300, 638);

	_director = new Level2Director();
	_director->init();
	_director->setObjectTracker(_simon);
	_viewport = _director->getViewport();
	//=====================TESTING==========================//
	_itemManager = new ItemManager();
	_gameStatusBoard = GameStatusBoard::getInstance();
	_gameStatusBoard->init();
	//ActiveWeapon::setItemID(eItemID::SWORD);

	//_medusaHead = new MedusaHead(HIDING, -1, START_POSITION,
	//	MEDUSAHEAD_HORIZONTAL_VELOC, MEDUSAHEAD_AMPLITUDE, MEDUSAHEAD_FREQUENCY);
	//_medusaHead->init();

	/*_backGround = Map::LoadFromFile("Resources//Maps//test.xml", eID::MAPSTAGE1);
	_mapObject = ObjectFactory::getListObjectFromFile("Resources//Maps//test.xml");*/

	_backGround = Map::LoadFromFile("Resources//Maps//level2.xml", eID::LEVEL2);
	_mapObject = ObjectFactory::getListObjectFromFile("Resources//Maps//level2.xml");

	//========================TESTING===========================//
	_testItem = new BaseObject*[15];
	_testItem[0] = new HeartItem(GVector2(2700, 200));
	_testItem[1] = new WhipUpgrade(GVector2(200, 300));
	for (int i = 2; i < 4; i++)
	{
		_testItem[i] = new MoneyBag(GVector2(2700 + i * 20, 300));
	}
	for (int i = 4; i < 7; i++)
	{
		_testItem[i] = new MoneyBag(GVector2(2750 + i * 20, 200));
	}
	for (int i = 7; i < 10; i++)
	{
		_testItem[i] = new HeartItem(GVector2(2700 + i * 20, 300));
	}
	_testItem[10] = new Sword(GVector2(2650, 200), eItemType::DROP, eDirection::LEFT);
	_testItem[11] = new ThrowingAxe(GVector2(2750, 200), eItemType::DROP, eDirection::LEFT);
	_testItem[12] = new Boomerang(GVector2(2780, 200), eItemType::DROP, eDirection::LEFT);
	_testItem[13] = new HolyWater(GVector2(2600, 200), eItemType::DROP, eDirection::LEFT);
	for (int i = 0; i < 14; i++)
	{
		ItemManager::insertItem((Item*)_testItem[i]);
	}
	
	// Scenario here
	auto scenarioDoorMoveViewport = new Scenario("DoorViewport");
	__hook(&Scenario::update, scenarioDoorMoveViewport, &PlayScene::doorScene);
	_directorDoor = new ScenarioManager();
	_directorDoor->insertScenario(scenarioDoorMoveViewport);

	auto scenarioPassDoor = new Scenario("PassDoor");
	__hook(&Scenario::update, scenarioPassDoor, &PlayScene::passDoorScene);
	flagDoorScenario = false;
	_directorPassDoor = new ScenarioManager();
	_directorPassDoor->insertScenario(scenarioPassDoor);

	//========================TESTING===========================//
	
	//=====================TESTING==========================//
	return true;
}

void PlayScene::updateInput(float dt) 
{

}

void PlayScene::update(float deltaTime)
{
	//=====================TESTING==========================//
	if (_simon->isInStatus(eStatus::DYING) == false)
	{
		this->updateViewport(_simon);
	}
	for (BaseObject* obj : (*_mapObject))
	{
		obj->update(deltaTime);
		_simon->checkCollision(obj, deltaTime);
		_itemManager->checkCollision(obj, deltaTime);
		// nếu là mấy cục shit này ko cần check va chạm 
		if (obj == nullptr || obj->isInStatus(eStatus::DESTROY) || obj->getId() == eID::LAND ||
			 obj->getId() == eID::FLYLAND|| obj->getId() == eID::DOOR)
			continue;
		// check mấy con như knight vs land đồ :v
		for (BaseObject* passiveobj : (*_mapObject)) {
			if (passiveobj == nullptr || passiveobj == obj || passiveobj->isInStatus(eStatus::DESTROY))
				continue;
			obj->checkCollision(passiveobj, deltaTime);
		}
	}

	_itemManager->checkCollision(_simon, deltaTime);
	_simon->update(deltaTime);
	_itemManager->update(deltaTime);

	// update scenario here
	////*** fix later :v
	//this->ScenarioMoveViewport(deltaTime);
	this->ScenarioPassDoor(deltaTime);
	//=====================TESTING==========================//
}

void PlayScene::draw(LPD3DXSPRITE spriteHandle) 
{
	//=====================TESTING==========================//
	_backGround->draw(spriteHandle, _viewport);

	for (BaseObject* obj : (*_mapObject))
	{
		obj->draw(spriteHandle, _viewport);
	}

	_gameStatusBoard->draw(spriteHandle);
	
	_simon->draw(spriteHandle, _viewport);

	_itemManager->draw(spriteHandle, _viewport);


	//=====================TESTING==========================//
}

void PlayScene::release()
{

}

void PlayScene::setViewport(Viewport* viewport)
{
	if (viewport != _viewport)
		_viewport = viewport;
}

Simon * PlayScene::getSimon() {
	return (Simon*)this->_simon;
}

BaseObject * PlayScene::getObject(eID id) {
	if (id == eID::SIMON)
		return getSimon();
	eID objectID;
	if ((*_mapObject).size() == 0) {
		return nullptr;
	}
	for (BaseObject* object : (*_mapObject)) {
		objectID = object->getId();
		if (objectID == id)
			return object;
	}
	return nullptr;
}

//=====================TESTING==========================//
void PlayScene::updateViewport(BaseObject* objTracker)
{
	_director->updateViewport();
	_viewport = _director->getViewport();
}

//=====================TESTING==========================//

#pragma region Pass the door scenario
//************ Hàng họ để mở cửa :v ************//
void PlayScene::doorScene(float dt, bool & finish) {
	GVector2 current_position = _viewport->getPositionWorld();
	GVector2 worldsize = this->_backGround->getWorldSize();
	// dịch screen từ từ sang TRÁI, speed = vs speed simon
	current_position.x -= SIMON_MOVING_SPEED * dt / 1000;

	_viewport->setPositionWorld(current_position);
	if (_simon->getBounding().left > current_position.x) {
		GVector2 curPos = _simon->getPosition();
		curPos.x = current_position.x + (_simon->getSprite()->getFrameWidth() >> 1);
		_simon->setPosition(curPos);
	}
	finish = false;
}
void PlayScene::ScenarioMoveViewport(float deltatime) {
	if (_directorDoor == nullptr)
		return;
	int xsimon = _simon->getPositionX();
	int ysimon = _simon->getPositionY();
	//
	if (xsimon>2074&&xsimon<=2104&&ysimon>730&&ysimon<740) {
		flagDoorScenario = true;
	}
	if (flagDoorScenario == true) {
		this->_directorDoor->update(deltatime);
		if (this->_directorDoor->isFinish() == true) {
			SAFE_DELETE(_directorDoor);
		}
	}
}

void PlayScene::passDoorScene(float deltatime, bool & isFinish) {
	auto simon = this->getSimon();
	int xsimon = simon->getPositionX();
	int ysimon = simon->getPositionY();
	// vì có 2 cái cửa, chắc fải làm 1 cái switch case :3
	// tới cửa tự đi, đúng ra phải dừng lại và chờ viewport dịch 1 đoạn, sẽ fix sau
	if (xsimon < 2100 && xsimon>2070 && ysimon < 700 && ysimon>660) {
			simon->forceMoveLeft();
	}
	else {
		// Đã qua khỏi cửa vài bước thì dừng lại
		if(xsimon < 1950 && ysimon < 700 && ysimon>660)
			simon->unforceMoveLeft();
	}
}
void PlayScene::ScenarioPassDoor(float deltatime) {
	if (_directorPassDoor == nullptr)
		return;
	auto door = getObject(eID::DOOR);
	//
	if (door != nullptr && door->isInStatus(eStatus::OPENING) == true) {
		this->_directorPassDoor->update(deltatime);
		if (this->_directorPassDoor->isFinish() == true) {
			SAFE_DELETE(_directorPassDoor);
		}
	}
}

#pragma endregion