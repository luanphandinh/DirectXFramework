#pragma once
#include "define.h"
_NAMESPACE_FRAMEWORK_BEGIN
//Class dùng để mô tả số lượng ticks trong 1s
//https://msdn.microsoft.com/en-us/library/system.timespan.tickspersecond(v=vs.110).aspx?cs-save-lang=1&cs-lang=cpp#code-snippet-2
class TimeSpan
{
public:
	static const UINT64 TicksPerSecond = 10000000;
	static const UINT64 TicksPerMilisecond = 10000;

	TimeSpan();
	TimeSpan(UINT64);

	float getMiliSeconds();
	UINT64 getTicks();

	~TimeSpan();

	TimeSpan operator + (TimeSpan);
	TimeSpan operator + (UINT64);
private:
	UINT64 _ticks;
};
_NAMESPACE_FRAMEWORK_END
