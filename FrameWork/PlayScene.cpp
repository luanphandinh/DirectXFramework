#include "PlayScene.h"


PlayScene::PlayScene()
{
	Director::loadStageInfo("Resources//Maps//level2ViewportInfo.txt",eID::LEVEL2);
	Director::setCurrentViewport("s2");
	GVector2 pos = Director::getCurrentStartViewportPosition();
	_viewport = new Viewport(pos.x, pos.y, WINDOW_WIDTH, WINDOW_HEIGHT);
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
	this->_simon->setPosition(2580, 700);
	_itemManager = new ItemManager();
	_gameStatusBoard = GameStatusBoard::getInstance();
	_gameStatusBoard->init();
	//ActiveWeapon::setItemID(eItemID::SWORD);

	//_spearKnight = new SpearKnight(NORMAL, NULL, NULL, 1);
	//_spearKnight->init();

	/*_bat = new Bat(HANGING, 340, 100, -1);
	_bat->init();

	_medusaHead = new MedusaHead(HIDING, -1, START_POSITION,
		MEDUSAHEAD_HORIZONTAL_VELOC, MEDUSAHEAD_AMPLITUDE, MEDUSAHEAD_FREQUENCY);
	_medusaHead->init();*/

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
		_simon->checkCollision(obj, deltaTime);
		_itemManager->checkCollision(obj, deltaTime);
	}
	//_spearKnight->checkCollision(_land, deltaTime);
	//_spearKnight->checkCollision(_simon, deltaTime);
	_itemManager->checkCollision(_simon, deltaTime);
	_simon->update(deltaTime);
	_itemManager->update(deltaTime);
	//_spearKnight->update(deltaTime);
	//_bat->update(deltaTime);
	//_medusaHead->update(deltaTime);


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
	// Vị trí hiện tại của viewport. 
	GVector2 current_position = _viewport->getPositionWorld();

	//GVector2 worldsize = this->_backGround->getWorldSize();
	GVector2 worldsize = Director::getCurrentViewportSize();
	// Bám theo object.
	GVector2 new_position = GVector2(max(objTracker->getPositionX() - WINDOW_WIDTH / 2, worldsize.x),
		current_position.y);

	// Không cho đi quá map.
	if (new_position.x + WINDOW_WIDTH > worldsize.y)
	{
		new_position.x = worldsize.y - WINDOW_WIDTH;
	}

	_viewport->setPositionWorld(new_position);
}
//=====================TESTING==========================//