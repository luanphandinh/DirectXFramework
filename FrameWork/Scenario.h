#pragma once
#include "StopWatch.h"
#include <queue>
using namespace std;

[event_source(native)]
class Scenario {
public:
	Scenario(string name);
	~Scenario();

	__event void update(float milisecond, bool& isFinish);

private:

	string _name;

};
