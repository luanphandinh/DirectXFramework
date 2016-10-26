#pragma once
#include "define.h"
#include "TimeSpan.h"
_NAMESPACE_FRAMEWORK_BEGIN
///Tương tự graphics
///GameTime sử dụng 1 static member getInstance()
///để lấy một instance thông qua tên class mà có thể sử dụng ở bất kì đâu sau 
///lời gọi getInstance()->Init() trong mainGame
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
_NAMESPACE_FRAMEWORK_END

