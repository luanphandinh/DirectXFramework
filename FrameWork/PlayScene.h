#pragma once
#include"define.h"
#include "Scene.h"
#include "Game.h"
#include"Animation.h"
#include<map>
#include"Simon.h"
#include"Map.h"
#include"QNode.h"
#include "ItemManager.h"
#include "ObjectFactory.h"
#include"GameStatusBoard.h"
#include"ActiveWeapon.h"
#include"Level2Director.h"
#include"ScenarioManager.h"

#include"Sword.h"
#include"ThrowingAxe.h"
using namespace std;
_USING_FRAMEWORK

class PlayScene :
	public Scene
{
public:
	PlayScene();
	~PlayScene();

	bool init() override;
	void updateInput(float dt) override;
	void update(float dt) override;
	void draw(LPD3DXSPRITE spriteHandle) override;
	void release() override;

	void setViewport(Viewport* viewport);

	Simon* getSimon();
	BaseObject* getObject(eID id);
private:
	//Kiểm tra nếu đối tượng ko cần sử dụng nữa thì huỷ
	void destroyObject();
	
	//Danh sách các đối tượng dùng để tạo quadtree
	map<string, BaseObject*> _mapObject;

	//Danh sách các đối tượng hoạt động rộng không thể đưa vào quadtree
	vector<BaseObject*> _listObject;

	//Danh sách các đối tượng nằm trong vùng active
	//Mỗi vòn lặp sẽ được kiểm tra chỉnh sửa tại update,và sử dụng lại tại draw.
	vector<BaseObject*>	_activeObject;

	//Quadtree
	QNode* _quadTree;

	vector<IControlable*> _listControlObject;
	Map* _backGround;

	BaseObject* _simon;
	BaseObject* _itemManager;

	void updateDirector(float deltaTime);

	// Hàng họ để mở cửa :v
	bool flagDoorScenario;
	ScenarioManager* _directorDoor;
	ScenarioManager* _directorPassDoor;

	// Tự đi qua cửa
	void ScenarioPassDoor(float deltatime);

	void doorScene(float dt, bool& finish);
	// Dịch screen đi gặp cái cửa
	void ScenarioMoveViewport(float deltatime);

	//==========================TEST=========================//
	//vector<BaseObject*>* _mapTestObject;
	BaseObject** _testItem;
	GameStatusBoard* _gameStatusBoard;
	//==========================TEST=========================//
};

