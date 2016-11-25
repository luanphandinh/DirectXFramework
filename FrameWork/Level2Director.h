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
	virtual void updateViewport() override;
	void switchViewport();
};

