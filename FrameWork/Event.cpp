#include "Event.h"
_USING_FRAMEWORK

Event::Event()
{
}


Event::~Event()
{
}

void Event::fireEvent(EventArg* e)
{
	if (_listAction.empty())
		return;
	for each(auto action in _listAction)
	{
		(*action)(e);
	}
}

void Event::operator += (EventFunction pFunction)
{
	_listAction.push_back(pFunction);
}

void Event::operator -= (EventFunction pFunction)
{
	auto it = find(_listAction.begin(), _listAction.end(), pFunction);
	if (it == _listAction.end())
		throw new exception("Element to remove no found!!!");
	_listAction.erase(it);
}

