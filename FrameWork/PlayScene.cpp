#include "PlayScene.h"
#include "GameOverScene.h"
#include "GameStatusBoard.h"
#include "Level3.h"
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
	/*LEVEL 2 POS*/
	simon->setPosition(2700, 100);//v1
	simon->setPosition(1400, 638);//v2
	//simon->setPosition(700, 640);//v3
	//simon->setPosition(1666, 1000);//v4
	//simon->setPosition(2700, 1000);//v4
	//simon->setPosition(1000, 1100);//v5
	//simon->setPosition(300, 1000);//v5
	//simon->setPosition(2500, 1324);//v6
	//simon->setPosition(1400, 1324);//v6


	this->_simon = simon;
	_listControlObject.push_back(simon);
	_listObject.push_back(_simon);
	_listObject.push_back(((Simon*)_simon)->getWhip());

	_director = new Level2Director();


	_director->init();
	_director->setObjectTracker(_simon);
	_director->setCurrentViewport(V2);
	_viewport = _director->getViewport();


	//=====================TESTING==========================//
	_itemManager = new ItemManager();
	GameStatusBoard::getInstance()->init();
	/*LEVEL 2*/
	/*
		Load QuadTree
	*/
	_quadTree = QNode::loadQuadTree("Resources//Maps//level2QuadTree.xml");
	loadMapObjects();
	_backGround = Map::LoadFromFile("Resources//Maps//level2.xml", eID::LEVEL2);
	SoundManager::getInstance()->PlayLoop(eSoundId::BACKGROUND_LEVEL2);

	/*LEVEL 3*/
	//_quadTree = QNode::loadQuadTree("Resources//Maps//level3QuadTree.xml");
	//map<string, BaseObject*>* maptemp = ObjectFactory::getMapObjectFromFile("Resources//Maps//level3.xml");
	//this->_mapObject.insert(maptemp->begin(), maptemp->end());
	//_backGround = Map::LoadFromFile("Resources//Maps//level3.xml", eID::LEVEL3);
	//SoundManager::getInstance()->PlayLoop(eSoundId::BACKGROUND_LEVEL3);

	//========================TESTING===========================//
	ActiveWeapon::setItemID((eItemID)7);
	HeartCounter::plusHeart(50);
	//ItemManager::generateItem(eItemID::CROWN, GVector2(2900, 100));
	//=====================TESTING==========================//
	return true;
}
void PlayScene::loadMapObjects()
{
	_mapObject.clear();
	/*
	_mapObject sẽ chứa tất cả các object có trong toàn bộ map của game
	*/
	map<string, BaseObject*>* maptemp = ObjectFactory::getMapObjectFromFile("Resources//Maps//level2.xml");
	this->_mapObject.insert(maptemp->begin(), maptemp->end());
}

void PlayScene::updateRevice()
{
	loadMapObjects();
}

void PlayScene::updateInput(float dt) 
{

}

void PlayScene::update(float deltaTime)
{
	if (_switchSence)
	{
		auto play = new Level3();
		SceneManager::getInstance()->replaceScene(play);
		return;
	}
	//=====================TESTING==========================//
	if (_simon->isInStatus(eStatus::DYING) == false)
	{
		this->updateDirector(deltaTime);
	}
	else
	{
		if (LifeCounter::getLife() == 0)
		{
			//có thể xử lý game over tại đây(để sau này xem thử)
			auto gameOver = new GameOverScene();
			SceneManager::getInstance()->replaceScene(gameOver);
			return;
		}
		this->updateRevice();
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
	screen.top = this->_backGround->getWorldSize().y - viewport_position.y + 70;
	screen.bottom = screen.top + /*_viewport->getHeight()*/ 310;

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
	//Lấy item trong object manager ra ngoài để check
	for (auto item : ItemManager::getListItem())
	{
		auto obj = item;
		if (obj == nullptr)
			continue;
		_activeObject.push_back(obj);
	}

	//[Bước 5]
	_activeObject.insert(_activeObject.end(), _listObject.begin(), _listObject.end());
	
	//[Bước 6]
	for (BaseObject* obj : _activeObject)
	{
		//if (_itemManager != nullptr && (obj->getId() == eID::LAND || obj->getId()== eID::SPEARKNIGHT))
		//{
			//_itemManager->checkCollision(obj, deltaTime);
		//}
		//không cần xét va chạm cho các trường hợp này
		if (obj == nullptr || obj->isInStatus(eStatus::DESTROY) || obj->getId() == eID::LAND || obj->getId() == eID::BRICK || 
			obj->getId() == eID::FLYLAND|| obj->getId() == eID::DOOR )
		continue;	
		// check mấy con như knight vs land đồ :v
		for (BaseObject* passiveobj : _activeObject) {
			if (passiveobj == nullptr || passiveobj == obj || passiveobj->isInStatus(eStatus::DESTROY))
				continue;
			obj->checkCollision(passiveobj, deltaTime);
		}
	}

	//[Bước 7]
	for (BaseObject* obj : _activeObject)
	{
		//if (obj != nullptr)
		obj->update(deltaTime);
	}

	GameStatusBoard::getInstance()->update(deltaTime);

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

	if (_itemManager != nullptr)
	{
		_itemManager->update(1);
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

	_itemManager->draw(spriteHandle, _viewport);

	GameStatusBoard::getInstance()->draw(spriteHandle);
	//=====================TESTING==========================//
}

void PlayScene::release()
{
	for (auto object : _listObject)
	{
		object->release();
		SAFE_DELETE(object);
	}
	_backGround->release();
	SAFE_DELETE(_backGround);
	SAFE_DELETE(_director);
	//SAFE_DELETE(_quadTree);

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
	if (_director != nullptr)
		_viewport = _director->getViewport();
}

//=====================TESTING==========================//

void PlayScene::switchScene()
{
	_switchSence = true;
}

