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
	auto simon = new Simon();
	simon->init();
	simon->setPosition(2700, 100);//v1
	//simon->setPosition(2300, 638);//v2
	simon->setPosition(700, 640);//v3
	//this->_simon = simon;

	_listControlObject.push_back(simon);
	_listObject.push_back(_simon);

	_director = new Level2Director();
	_director->init();
	_director->setObjectTracker(_simon);
	_director->setCurrentViewport(V1);
	_viewport = _director->getViewport();
	//=====================TESTING==========================//
	_itemManager = new ItemManager();
	_gameStatusBoard = GameStatusBoard::getInstance();
	_gameStatusBoard->init();

	/*
		Load QuadTree
	*/
	_quadTree = QNode::loadQuadTree("Resources//Maps//level2QuadTree.xml");
	/*
		_mapObject sẽ chứa tất cả các object có trong toàn bộ map của game
	*/
	map<string, BaseObject*>* maptemp = ObjectFactory::getMapObjectFromFile("Resources//Maps//level2.xml");
	this->_mapObject.insert(maptemp->begin(), maptemp->end());

	_backGround = Map::LoadFromFile("Resources//Maps//level2.xml", eID::LEVEL2);

	//========================TESTING===========================//
	_testItem = new BaseObject*[15];
	_testItem[0] = new HeartItem(GVector2(2700, 200),eItemID::LARGEHEART);
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
	//auto scenarioDoorMoveViewport = new Scenario("DoorViewport");
	//__hook(&Scenario::update, scenarioDoorMoveViewport, &PlayScene::doorScene);
	//_directorDoor = new ScenarioManager();
	//_directorDoor->insertScenario(scenarioDoorMoveViewport);


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
		this->updateDirector(deltaTime);
	}
	/*
		Khi vào game thì bản thân các Object sẽ được load toàn bộ và được init() sau đó add vào _mapObject
		Quá trình game chạy thì những object nào được load lên vùng viewport mới update và checkCollision

		Ta có 2 danh sách Object.
		Một là _listobject chứa các đối tượng hoạt động rộng,không thể đưa vào quadtree
		Hai là _mapObject chứa tất cả các đối tượng của map
		Ta có một listObject phụ là activeObject chứa các object sẽ được update,draww ở mỗi frame,được clear ở đầu hàm update.
		_activeObject chứa các object được lấy ra từ quadtree

		Quá trình update gồm các bước:
			Bước 1	:	Kiểm tra các đối tương hết hiệu lực (Status = Destroy) từ frame trước
			Bước 2	:	Clear danh sách activeObject của frame trước,chuẩn bị cho vòng lặp mới
			Bước 3	:	Tìm các tên của đối tượng có trong vùng screen mà quadtree lấy ra được
			Bước 4	:	Từ danh sách B3,add các đối tượng có tên tương ứng với _mapObject vào activeObject
			Bước 5	:	Add danh sách các đối tương trong _listOBject vào _activeObject
			Bước 6	:	Kiểm tra va chạm giữa các activeObject,nếu có n đối tượng,thì có n*n lần kiểm tra
			Bước 7	:	Update các đối tượng trong _activeObject
	*/

	GVector2 viewport_position = _viewport->getPositionWorld();
	RECT viewport_in_transform = _viewport->getBounding();

	//Vì Quad tree lưu các Node Bound với tọa độ top-left
	// Hàm getlistobject của quadtree yêu cầu truyền vào một hình chữ nhật theo hệ top left, nên cần tính lại khung màn hình
	RECT screen;
	// left right không đổi dù hệ top-left hay hệ bot-left
	screen.left = viewport_in_transform.left;
	screen.right = viewport_in_transform.right;
	screen.top = this->_backGround->getWorldSize().y - viewport_position.y;
	screen.bottom = screen.top + _viewport->getHeight();

	//[Bước 1]
	this->destroyObject();

	//[Bước 2]
	_activeObject.clear();

	//[Bước 3]
	auto listObjectName = _quadTree->GetActiveObject(screen);

	//[Bước 4]
	for (auto name : listObjectName)
	{
		auto obj = _mapObject.find(name);
		if (obj == _mapObject.end() || obj._Ptr == nullptr)
			continue;
		_activeObject.push_back(obj->second);
	}

	//[Bước 5]
	_activeObject.insert(_activeObject.end(), _listObject.begin(), _listObject.end());

	//[Bước 6]
	for (BaseObject* obj : _activeObject)
	{
		//if (_itemManager != nullptr && (obj->getId() == eID::LAND || obj->getId()== eID::SPEARKNIGHT))
		//{
			_itemManager->checkCollision(obj, deltaTime);
		//}
		//không cần xét va chạm cho các trường hợp này
		if (obj == nullptr || obj->isInStatus(eStatus::DESTROY) || obj->getId() == eID::LAND ||
			 obj->getId() == eID::FLYLAND|| obj->getId() == eID::DOOR)
			continue;	
		// check mấy con như knight vs land đồ :v
		for (BaseObject* passiveobj : _activeObject) {
			if (passiveobj == nullptr || passiveobj == obj || passiveobj->isInStatus(eStatus::DESTROY))
				continue;
			obj->checkCollision(passiveobj, deltaTime);
		}
	}

	if (_itemManager != nullptr)
	{
		//_itemManager->checkCollision(_simon, deltaTime);
		_itemManager->update(deltaTime);
	}
	//[Bước 7]
	for (BaseObject* obj : _activeObject)
	{
		obj->update(deltaTime);
	}


	// update scenario here
	////*** fix later :v
	//this->ScenarioMoveViewport(deltaTime);

	//for (BaseObject* obj : (*_mapTestObject))
	//{
	//	obj->update(deltaTime);
	//	_simon->checkCollision(obj, deltaTime);
	//	_itemManager->checkCollision(obj, deltaTime);
	//	// nếu là mấy cục shit này ko cần check va chạm 
	//	if (obj == nullptr || obj->isInStatus(eStatus::DESTROY) || obj->getId() == eID::LAND ||
	//		 obj->getId() == eID::FLYLAND|| obj->getId() == eID::DOOR)
	//		continue;
	//	// check mấy con như knight vs land đồ :v
	//	for (BaseObject* passiveobj : (*_mapTestObject)) {
	//		if (passiveobj == nullptr || passiveobj == obj || passiveobj->isInStatus(eStatus::DESTROY))
	//			continue;
	//		obj->checkCollision(passiveobj, deltaTime);
	//	}
	//}

	//_itemManager->checkCollision(_simon, deltaTime);
	//_simon->update(deltaTime);
	//_itemManager->update(deltaTime);

	//=====================TESTING==========================//
}

void PlayScene::destroyObject()
{
	for (auto object : _listObject)
	{
		//Nếu object trong trạn thái destroy thì loại khỏi _listObject
		if (object->getStatus() == eStatus::DESTROY)
		{
			object->release();
			auto rs1 = remove(_listObject.begin(), _listObject.end(), object);
			_listObject.pop_back();

			//loại khỏi list_control
			vector<IControlable*>::iterator icontrol = find(_listControlObject.begin(), _listControlObject.end(), dynamic_cast<IControlable*>(object));
			if (icontrol != _listControlObject.end())
			{
				auto rs2 = std::remove(_listControlObject.begin(), _listControlObject.end(), (*icontrol));
				_listControlObject.pop_back();
			}

			delete object;
			break;		// sau pop_back phần tử đi thì list bị thay đồi, nên vòng for-each không còn nguyên trạng nữa. -> break (mỗi frame chỉ remove được 1 đối tượng)
		}
	}

	for (auto name : QNode::ActiveObject)
	{
		//Lấy object từ toàn bộ map
		auto object = _mapObject.find(name);
		//Nếu ko tìm thấy object này thì chuyển sang tìm object khác
		if (object == _mapObject.end() || object._Ptr == nullptr)
			continue;
		//Nếu tìm thấy mà obj này đang trong trạng thái hủy
		if (object->second->getStatus() == eStatus::DESTROY)
		{
			//release nó
			object->second->release();
			delete object->second;
			object->second = NULL;
			//xóa khỏi _mapObject
			_mapObject.erase(name);
		}
	}
}

void PlayScene::draw(LPD3DXSPRITE spriteHandle) 
{
	//=====================TESTING==========================//
	_backGround->draw(spriteHandle, _viewport);

	for (BaseObject* obj : _activeObject)
	{
		obj->draw(spriteHandle, _viewport);
	}
	//for (BaseObject* obj : (*_mapTestObject))
	//{
	//	obj->draw(spriteHandle, _viewport);
	//}

	
	
	//_simon->draw(spriteHandle, _viewport);

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

BaseObject * PlayScene::getObject(eID id) {
	if (id == eID::SIMON)
		return getSimon();
	eID objectID;
	if (_activeObject.size() == 0) {
		return nullptr;
	}
	for (BaseObject* object : _activeObject) {
		objectID = object->getId();
		if (objectID == id)
			return object;
	}
	return nullptr;
}

//=====================TESTING==========================//
void PlayScene::updateDirector(float deltaTime)
{
	_director->update(deltaTime);
	_viewport = _director->getViewport();
}

//=====================TESTING==========================//

#pragma region Pass the door scenario
//************ Hàng họ để mở cửa :v ************//
//void PlayScene::doorScene(float dt, bool & finish) {
//	GVector2 current_position = _viewport->getPositionWorld();
//	GVector2 worldsize = this->_backGround->getWorldSize();
//	// dịch screen từ từ sang TRÁI, speed = vs speed simon
//	current_position.x -= SIMON_MOVING_SPEED * dt / 1000;
//
//	_viewport->setPositionWorld(current_position);
//	if (_simon->getBounding().left > current_position.x) {
//		GVector2 curPos = _simon->getPosition();
//		curPos.x = current_position.x + (_simon->getSprite()->getFrameWidth() >> 1);
//		_simon->setPosition(curPos);
//	}
//	finish = false;
//}
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

void PlayScene::ScenarioPassDoor(float deltatime) {
	
}

#pragma endregion