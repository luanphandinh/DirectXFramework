// HOW TO USE
/*
	call InputController::getInstance()->init() at Game::init
	call InputController::getInstance()->update() at game loop.
	if you want to check if key down use:
			InputController::getInstance()->isKeydown(int keycode) at game loop or update function of an object
			With key down you can hold key to keep action.
	if you want to check if key is pressed:
			InputController::getInstance()->_keyPress += (EventFunction)&[Insert name of function here];

	if you want tỏ check if key is released:
			InputController::getInstance()->_keyReleased += (EventFunction)&[Insert name of function here];

	if you dont want to refference this function any more, call:
			InputController::getInstance()->_keyPress -= (EventFunction)&[Insert name of function here]
			InputController::getInstance()->_keyReleased -= (EventFunction)&[Insert name of function here];

	The referenced function have prototype:
		void <FunctionName>(KeyEventArg* e);
	Should call InputController::release() at Game::release()
	Example:
	void Jump(KeyEventArg* e)
	{
		if (e == NULL)
			return;
		switch (e->_keycode)
		{
			case [Space]:
			// I'm jumping....
		}
	}

	InputController::getInstance()->_keyPressed += (EventFunction) &Jump;
			And you can press space to make me jump
*/

#pragma once
#include"define.h"
#include"Event.h"
_NAMESPACE_FRAMEWORK_BEGIN

/*
	Sử dụng event_source(native) gán event cần fire
	Sẽ sử dụng  event_receiver để handle event

	+Khai báo event:
		Sử dụng __event để khai báo event trong lớp event nguồn(source event class,InputController)

	+Định nghĩa event handlers:
		Trong lớp nhận event(reciever class),ví dụ lớp belmon ta định nghĩa 2 hàm onKeyPressed,onKeyReleased

	+Hooking EventHandlers to Event:
		Cũng trong lớp nhận event sử dụng __hook để kết nối events(onKeyPressed,onKeyReleased của belmon) 
		với event handlers(các hàm của InputController)
	
	+FireEvent:
		Gọi hàm của Inputcontroller
	https://msdn.microsoft.com/vi-vn/library/ee2k0a7d.aspx
*/
#define KEYBOARD_BUFFER_SIZE 1024

typedef	LPDIRECTINPUT	pGinput;
typedef LPDIRECTINPUTDEVICE8 pGKeyBoard;

typedef void(*KeyEvent)(int);

class KeyEventArg : public EventArg
{
public:
	KeyEventArg(int keyCode){ _key = keyCode; }
	int _key;
};


[event_source(native)]
class InputController
{
public:
	~InputController();

	static InputController* getInstance();
	static void release();

	bool	init(HWND, HINSTANCE);//được gọi trong game::init()
	void	update();				//dduocj gọi trong main loop
	int		isKeyDown(int keyCode);
	int		isKeyPressed(int keyCode);
	int		isKeyRelease(int keyCode);

	Event	_keyPressed;
	Event	_keyReleased;

	// dùng marco __event thì có thể trỏ được đến các hàm thành viên của các lớp. nhưng cách sử dụng phức tạp hơn
	__event void __enventKeyPressed(KeyEventArg* e);
	__event void __enventKeyReleased(KeyEventArg* e);

private:
	static InputController* _instance;

	pGinput			_input;
	pGKeyBoard		_keyBoard;
	BYTE			_keyBuffer[256];//BYTE = unsiged char
	HWND			_hWnd;
	DIDEVICEOBJECTDATA _keyEvents[KEYBOARD_BUFFER_SIZE];
	InputController();

	bool _previousKeyBuffer[256];
};
typedef InputController *pInputController;
_NAMESPACE_FRAMEWORK_END

