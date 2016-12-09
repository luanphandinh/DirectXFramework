#pragma once
#include "Director.h"
#include"Simon.h"
#include<string>
class Level3Director :
	public Director {
public:
	Level3Director();
	~Level3Director();

	void init() override;
	void Level3Director::update(float deltaTime) override;
	virtual void updateScenario(float deltaTime) override;
	virtual void updateViewport() override;
	void switchViewport();

	//Dùng cho thằng simon tự động đi qua cửa
	void passDoorScene(float deltatime, bool& isFinish);
	//Di chuyển cái viewport cho cái cửa nằm ở giữa
	void moveViewportPassDoor(float deltatime, bool & finish);
	//Di chuyển viewport từ từ cho mất cái cửa
	void moveViewportPassDoor2(float deltatime, bool & finish);

	/*
	Xử lý cho các special item xuất hiện
	*/
	void crownShowUp(float deltatime, bool & finish);
private:
	bool _flagMoveViewportPassDoor;
	bool _flagMoveSimonPassDoor;
	bool _flagMoveViewportPassDoor2;
	BaseObject* _simon;
	BaseObject* _trackedDoor;
	bool checkPosition();
	bool isPassedDoor();
};


