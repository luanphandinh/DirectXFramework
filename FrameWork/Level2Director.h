#pragma once
#include "Director.h"
#include<string>
class Level2Director :
	public Director
{
public:
	Level2Director();
	~Level2Director();

	void init() override;
	virtual void updateViewport(BaseObject* objTracker) override;
	void switchViewport(BaseObject* objTracker);
};

