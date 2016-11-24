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
	_director = new Level2Director();
	_director->init();
	_viewport = _director->getViewport();
	//=====================TESTING==========================//
	_simon = new Simon();
	_simon->init();
	_isSwitchSence = false;
	// set pos ở đây, đừng đặt trong class
	this->_simon->setPosition(2700, 100);
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
	
	for (int i = 0; i < 11; i++)
	{
		ItemManager::insertItem((Item*)_testItem[i]);
	}
	

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

	//=====================TESTING==========================//
}

void PlayScene::draw(LPD3DXSPRITE spriteHandle) 
{
	//=====================TESTING==========================//
	_backGround->draw(spriteHandle, _viewport);
	/*_spearKnight->draw(spriteHandle, _viewport);

	_bat->draw(spriteHandle, _viewport);

	_medusaHead->draw(spriteHandle, _viewport);*/
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

//=====================TESTING==========================//
void PlayScene::updateViewport(BaseObject* objTracker)
{
	_director->updateViewport(objTracker);
	_viewport = _director->getViewport();
}
//=====================TESTING==========================//