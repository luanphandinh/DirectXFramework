#pragma once
#include "define.h"
#include "TimeSpan.h"
_NAMESPACE_FRAMEWORK_BEGIN
/*
	+ Dùng để quản lý thời gian 2 frame của Game
	+ Tương tự graphics GameTime sử dụng 1 static member getInstance()
	+ được init tại hàm init trong game
	+ Được gọi tại bất cứ đâu thông lời gọi  getInstance()
*/
class GameTime
{
public:
	~GameTime();
	static void release();
	static GameTime* getInstance();
	void init();
	void resetLastTick();

	void updateGameTime();

	float getElapsedGameTime();	//Tổng thời gian trôi qua kể từ lần cuối gọi update
	float getTotalGameTime();	//Tổng thời gian trôi qua kể từ lần cuối gọi init
private:
	static GameTime* _instance;

	TimeSpan	_elapsedGameTime;
	TimeSpan	_totalGameTime;
	LARGE_INTEGER	_Query;//Dùng làm tham số cho hàm QueryPerformanceCounter()
	float			_freQuery;

	LONGLONG	startTicks;
	LONGLONG	lastTicks;
	LONGLONG	curTicks;

	GameTime();
	GameTime(TimeSpan& elapsedGameTime);

	void setELapsedGameTime(TimeSpan& elapsedGameTime);
	void setTotalGameTime(TimeSpan& TotalGameTime);
};

typedef GameTime* pGameTime;
_NAMESPACE_FRAMEWORK_END

