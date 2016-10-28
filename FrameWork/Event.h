/*
	use operator += to register an event
	use operator -= to remove an event
	chose where active event by using call fireEvent

	you have to explicit cast to (EventFunction) when use operator += and -=
	example:
		Event myevent;
		myevent += (EventFunction) &FUNCTION;	// must have reference (&)

	FUNCTION have to have prototype:
	void <FunctionName> (EventArg* e);			or
	void <FunctionName> (XXXEventArg* e);		// must use pointer in argument
										// because using pointer, you should check if e == NULL in define of FucntionName 
	with: XXXEventArg is inheritanced from EventArg and have some argument if you wish
		you can define argument class when you use.
		class XXXEventArg : public EventArg
		{
			public:
				XXXEventArg([type] arg1, [type])
				{
					this->_arg1 = arg1;
					this->_arg2 = arg2;
				}
				[type]	_arg1;
				[type]	_arg2;
				...
		}
	
	when call fireEvent you can call
		myevent.fireEvent(NULL);
		myevent.fireEvent(new EventArg());
		myevent.fireEvent(new XXXEventArg(x1,x2));
*/
#pragma once
#include"define.h"
_NAMESPACE_FRAMEWORK_BEGIN

class EventArg;
//Con trỏ hàm EventFunction
//Đối số truyền vào là EventArg*
//kiểu trả về void
//link con trỏ hàm
//http://diendan.congdongcviet.com/threads/t25204::con-tro-ham-trong-c-cpp-function-pointer.cpp
typedef void(*EventFunction)(EventArg*);

class Event
{
public:
	Event();
	virtual ~Event();
	virtual void fireEvent(EventArg* e);//active the event-kích hoạt sự kiện
	void operator += (EventFunction pFunction);//register the event-đăng kí sự kiện
	void operator -= (EventFunction pFunction);//remove an action- hủy sự kiện
private:
	vector<EventFunction>	_listAction;
};

typedef Event* pEvent;

class EventArg
{
public:
	EventArg(){}
	virtual ~EventArg(){}
};

_NAMESPACE_FRAMEWORK_END

