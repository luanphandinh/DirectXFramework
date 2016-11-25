#pragma once
#include "Scenario.h"
#include <queue>
class ScenarioManager {
public:
	ScenarioManager();
	~ScenarioManager();
	void update(float milisecond);
	void insertScenario(Scenario* scenario);
	bool isFinish();
private:

	queue<Scenario*> _drama;
};