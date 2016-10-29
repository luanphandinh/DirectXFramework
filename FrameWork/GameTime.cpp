#include "GameTime.h"
_USING_FRAMEWORK

GameTime* GameTime::_instance = nullptr;

GameTime::~GameTime()
{
}

GameTime::GameTime()
{

}

GameTime::GameTime(TimeSpan& elapsedGameTime)
{
	this->_elapsedGameTime = elapsedGameTime;
}

void GameTime::setELapsedGameTime(TimeSpan& elapsedGameTime)
{
	this->_elapsedGameTime = elapsedGameTime;
}

void GameTime::setTotalGameTime(TimeSpan& TotalGameTime)
{
	this->_totalGameTime = TotalGameTime;
}

void GameTime::release()
{
	delete _instance;
	_instance = NULL;
}

GameTime* GameTime::getInstance()
{
	if (_instance == NULL)
		_instance = new GameTime();
	return _instance;
}

void GameTime::init()
{
	//lấy số xung nhịp cpu/giây
	QueryPerformanceFrequency(&this->_Query);
	//lưu vào _freQuery
	this->_freQuery = (float)_Query.QuadPart / 10000000;
	// hàm QueryPerformanceCounter(LARGE_INTEGER *) để thực hiện việc đo thời gian
	// Hàm này có kiểu trả về là BOOL và trả về giá trị là thời gian tính bằng giây từ lúc chương trình bắt đầu chạy,
	//tham số đầu vào là con trỏ tới một biến kiểu LARGE_INTEGER.
	//ở đây giá trị LARGE_INTEGER là _Query 
	QueryPerformanceCounter(&_Query);
	//gán startTIcks và lastTicks bằng thời gian khi init();
	startTicks = lastTicks = _Query.QuadPart;
	_totalGameTime = (TimeSpan)0;
}

void GameTime::resetLastTick()
{
	lastTicks = 0;
	curTicks = 0;
	_totalGameTime = (TimeSpan)0;
}
//tính time 
//http://diendan.congdongcviet.com/threads/t210307::huong-dan-viet-mot-game-don-gian-trong-directx-tu-a-z.cpp
void GameTime::updateGameTime()
{
	QueryPerformanceCounter(&_Query);
	curTicks = _Query.QuadPart;
	//16 = 1000/60 = frame_rate
	if ((UINT64)((float)(curTicks - lastTicks) / _freQuery) < TimeSpan::TicksPerMilisecond * 16)
	{
		return;
	}
	auto gt = ((float)(curTicks - lastTicks)) / _freQuery;
	this->setTotalGameTime(_totalGameTime + gt);
	this->setELapsedGameTime(TimeSpan((UINT64)gt));
	lastTicks = curTicks;
}
//Tổng thời gian trôi qua kể từ lần cuối gọi update
float GameTime::getElapsedGameTime()
{
	return this->_elapsedGameTime.getMiliSeconds();
}	

//Tổng thời gian trôi qua kể từ lần cuối gọi init
float GameTime::getTotalGameTime()
{
	return this->_totalGameTime.getMiliSeconds();
}