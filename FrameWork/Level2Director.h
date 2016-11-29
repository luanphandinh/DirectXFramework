#pragma once
#include "Director.h"
#include"Simon.h"
#include<string>
class Level2Director :
	public Director
{
public:
	Level2Director();
	~Level2Director();

	void init() override;
	virtual void updateScenario(float deltaTime) override;
	virtual void updateViewport() override;
	void switchViewport();

	//Dùng cho thằng simon tự động đi qua cửa
	void passDoorScene(float deltatime, bool& isFinish);
};

