#include "PlayScene.h"


PlayScene::PlayScene()
{
	_viewport = new Viewport(0, WINDOW_HEIGHT, WINDOW_WIDTH, WINDOW_HEIGHT);
}


PlayScene::~PlayScene()
{
	delete _viewport;
	_viewport = nullptr;
}


bool PlayScene::init() 
{
	//=====================TESTING==========================//
	_simon = new Simon();
	_simon->init();
	// set pos ở đây, đừng đặt trong class
	//stage 1
	//this->_simon->setPosition(2580, 63);
	//stage 2
	this->_simon->setPosition(2800,638);
	_itemManager = new ItemManager();
	_gameStatusBoard = GameStatusBoard::getInstance();
	_gameStatusBoard->init();
	ActiveWeapon::setItemID(eItemID::SWORD);

	_spearKnight = new SpearKnight(NORMAL, 2700, 320, 1);
	_spearKnight->init();

	_bat = new Bat(HANGING, 2650, 320, 1);
	_bat->init();

	_medusaHead = new MedusaHead(HIDING, 1, GVector2(2560,263),
		MEDUSAHEAD_HORIZONTAL_VELOC, MEDUSAHEAD_AMPLITUDE, MEDUSAHEAD_FREQUENCY);
	_medusaHead->init();

	_door = new Door(CLOSING, GVector2(2070, 687), -1);
	_door->init();
	/*_backGround = Map::LoadFromFile("Resources//Maps//test.xml", eID::MAPSTAGE1);
	_mapObject = ObjectFactory::getListObjectFromFile("Resources//Maps//test.xml");*/

	_background = Map::LoadFromFile("Resources//Maps//level2.xml", eID::LEVEL2);
	_mapObject = ObjectFactory::getListObjectFromFile("Resources//Maps//level2.xml");

	//========================TESTING===========================//
	_testItem = new BaseObject*[15];
	_testItem[0] = new HeartItem(GVector2(50, 200));
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
		_testItem[i] = new HeartItem(GVector2(230 + i * 20, 300));
	}
	_testItem[10] = new Sword(GVector2(2650, 200), eItemType::DROP, eDirection::LEFT);
	_testItem[11] = new Sword(GVector2(2675, 200), eItemType::PICKED_UP, eDirection::LEFT);
	_testItem[12] = new Sword(GVector2(2675, 200), eItemType::PICKED_UP, eDirection::RIGHT);
	for (int i = 0; i < 13; i++)
	{
		ItemManager::insertItem((Item*)_testItem[i]);
	}
	// có gì đó sai sai :v
	_director = new ScenarioManager();
	auto scenarioDoor_Viewport = new Scenario("DoorViewport");
	__hook(&Scenario::update, scenarioDoor_Viewport, &PlayScene::doorSceneViewport);
	_director->insertScenario(scenarioDoor_Viewport);
	flagDoorScenario = false;

	_directorPassDoor = new ScenarioManager();
	auto scenarioPassDoor = new Scenario("PassDoor");
	__hook(&Scenario::update, scenarioPassDoor, &PlayScene::passDoorScene);
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
		_simon->checkCollision(obj, deltaTime);
		_itemManager->checkCollision(obj, deltaTime);
		_spearKnight->checkCollision(obj, deltaTime);
	}
	//_spearKnight->checkCollision(_land, deltaTime);
	//_spearKnight->checkCollision(_simon, deltaTime);
	_itemManager->checkCollision(_simon, deltaTime);
	_simon->update(deltaTime);
	_itemManager->update(deltaTime);

	_spearKnight->update(deltaTime);
	_bat->update(deltaTime);
	_medusaHead->update(deltaTime);

	_door->update(deltaTime);

	// có gì đó sai sai :v 
	//this->ScenarioMoveViewport(deltaTime);
	this->ScenarioPassDoor(deltaTime);
	//=====================TESTING==========================//
}

void PlayScene::draw(LPD3DXSPRITE spriteHandle) 
{
	//=====================TESTING==========================//
	_background->draw(spriteHandle, _viewport);

	_spearKnight->draw(spriteHandle, _viewport);

	_bat->draw(spriteHandle, _viewport);

	_medusaHead->draw(spriteHandle, _viewport);

	_door->draw(spriteHandle, _viewport);

	for (BaseObject* obj : (*_mapObject))
	{
		obj->draw(spriteHandle, _viewport);
	}
	
	_simon->draw(spriteHandle, _viewport);

	_itemManager->draw(spriteHandle, _viewport);

	_gameStatusBoard->draw(spriteHandle);
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

//=====================TESTING==========================//
void PlayScene::updateViewport(BaseObject* objTracker)
{
	// Vị trí hiện tại của viewport. 
	GVector2 current_position = _viewport->getPositionWorld();

	GVector2 worldsize = this->_background->getWorldSize();
	// Bám theo object.
	//GVector2 new_position = GVector2(max(objTracker->getPositionX() - WINDOW_WIDTH/2, 0), WINDOW_HEIGHT*2);
	float test = objTracker->getPositionY();

	//// fải jump 1 cái mới normal :'(
	//GVector2 new_position = GVector2(max(objTracker->getPositionX() - WINDOW_WIDTH / 2, 0), 
	//								max(objTracker->getPositionY() + 50, WINDOW_HEIGHT));
	// hack để test :v
	GVector2 new_position = GVector2(max(objTracker->getPositionX() - WINDOW_WIDTH / 2, 0),
		max(objTracker->getPositionY() + 50, WINDOW_HEIGHT*2));

	if (new_position.y < current_position.y) {
		new_position.y = current_position.y;
	}

	// Không cho đi quá map.
	if (new_position.x + WINDOW_WIDTH > worldsize.x)
	{
		new_position.x = worldsize.x - WINDOW_WIDTH;
	}

	_viewport->setPositionWorld(new_position);
}

void PlayScene::doorSceneViewport(float deltaTime, bool & finish) {
	GVector2 current_position = _viewport->getPositionWorld();
	GVector2 worldsize = this->_background->getWorldSize();

	current_position.x -= SIMON_MOVING_SPEED * deltaTime / 1000;
	if (current_position.x < _door->getPositionX() - 400) {
		finish = true;
		_viewport->setPositionWorld(current_position);
		return;
	}

	finish = false;
}

void PlayScene::passDoorScene(float deltatime, bool& isFinish) {
	auto simon = (Simon*)_simon;
	int x = _simon->getPositionX();
	int y = _simon->getPositionY();
	if (x < _door->getPositionX() - 400) {
		isFinish = true;
		simon->unforceMoveLeft();
		return;
	}
	if (x < 2100 && x>2070 && y < 700 && y>660) {
			simon->forceMoveLeft();
			
	}
}

void PlayScene::ScenarioMoveViewport(float deltaTime) {
	if (_director == nullptr)
		return;
	int x = _simon->getPositionX();
	int y = _simon->getPositionY();
	// Mở cửa đầu tiên ở stage 2
	if (x < 2100 && x>2070 && y < 700 && y>660) {
		flagDoorScenario = true;
	}

	if (flagDoorScenario == true) {
		this->_director->update(deltaTime);
		if (this->_director->isFinish() == true) {
			SAFE_DELETE(_director);
		}
	}
}
void PlayScene::ScenarioPassDoor(float deltaTime) {
	if (_directorPassDoor == nullptr)
		return;
	if (_door != nullptr && _door->isInStatus(eStatus::OPENING) == true) {
		this->_directorPassDoor->update(deltaTime);
		if (this->_directorPassDoor->isFinish() == true) {
			SAFE_DELETE(_directorPassDoor);		
		}
	}
}
//=====================TESTING==========================//