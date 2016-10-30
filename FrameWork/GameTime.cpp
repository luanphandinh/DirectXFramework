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
	//http://stackoverflow.com/questions/1739259/how-to-use-queryperformancecounter
	//lấy số xung nhịp cpu/giây
	//vd : QueryPerformanceFrequency sẽ trả về ! 3 000 000
	QueryPerformanceFrequency(&this->_Query);
	//lưu vào _freQuery
	/*
		Dùng dây thì chia 1
		Dùng milisecond thì chia 1000
		Dùng Ticks thì chia 10000000
	*/
	this->_freQuery = (float)_Query.QuadPart / TimeSpan::TicksPerSecond;
	/* 
	Hàm QueryPerformanceCounter(LARGE_INTEGER *) để thực hiện việc đo thời gian
	Ở đây giá trị LARGE_INTEGER là _Query 
	Hàm này sẽ lấy số QueryPerformanceCounter hiện tại sau đó lưu vào startTIcks và lastTicks
	*/
	QueryPerformanceCounter(&_Query);
	//gán startTicks và lastTicks bằng thời gian khi init();
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
	//Tính số ticks đã trổi qua
	auto gt = ((float)(curTicks - lastTicks)) / _freQuery;
	//gán lại totalGameTime theo ticks
	this->setTotalGameTime(_totalGameTime + gt);
	//gán thời gian trôi qua của frame
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