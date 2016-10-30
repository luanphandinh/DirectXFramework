#include "InputController.h"
_USING_FRAMEWORK

InputController::InputController()
{
	_input = NULL;
	_keyBoard = NULL;
	ZeroMemory(_keyBuffer, 256);
}


InputController::~InputController()
{
	if (_input != NULL)
		this->_input->Release();
	if (_keyBoard != NULL)
		this->_keyBoard->Release();
}

InputController* InputController::_instance = nullptr;

InputController* InputController::getInstance()
{
	if (_instance == nullptr)
		_instance = new InputController();
	return _instance;
}
void InputController::release()
{
	delete _instance;
	_instance = nullptr;
}

bool InputController::init(HWND hWnd, HINSTANCE hInstance)
{
	this->_hWnd = hWnd;
	HRESULT rs;
	//khởi tạo _input
	rs = DirectInput8Create(
		hInstance,//hinstance của chương trình
		DIRECTINPUT_VERSION,//Tham số mặc định
		IID_IDirectInput8,
		(void**)&_input,//con trỏ nhận dữ liệu trả về
		NULL
		);
	if (rs != DI_OK)
		return false;
	//tạo bàn phím bằng input
	rs = _input->CreateDevice(GUID_SysKeyboard, (LPDIRECTINPUTDEVICEW*)&_keyBoard, NULL);
	if (rs != DI_OK)
		return false;
	//setdataformat cho bàn phím,sử dụng hằng số c_dfDIKeyboard,mouse thì c_dfDIMouse
	rs = _keyBoard->SetDataFormat(&c_dfDIKeyboard);
	if (rs != DI_OK)
		return false;
	//coorperative level,loại hoạt động. DISCL_FOREGROUND nghĩa là chỉ hoạt động khi của sổ hWnd đang được handle
	rs = _keyBoard->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
	if (rs != DI_OK)
		return false;
	//Set Property cho keyboard buffer
	DIPROPDWORD dipdw;

	dipdw.diph.dwSize = sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	dipdw.diph.dwHow = DIPH_DEVICE;
	dipdw.diph.dwObj = 0;
	dipdw.dwData = KEYBOARD_BUFFER_SIZE;

	rs = _keyBoard->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);
	if (rs != DI_OK)
		return false;

	//Giành quyền kiểm soát thiết bị
	rs = _keyBoard->Acquire();
	if (rs != DI_OK)
		return false;

	return true;

}

void InputController::update()
{
	for (int i = 0; i < 256; i++)
	{
		_previousKeyBuffer[i] = ((_keyBuffer[i] & 0x80) > 0);
	}

	//collect state of all keys
	_keyBoard->GetDeviceState(sizeof(this->_keyBuffer), _keyBuffer);

	if (isKeyDown(DIK_ESCAPE))
	{
		PostMessage(_hWnd, WM_QUIT, 0, 0);
	}

	DWORD dw = KEYBOARD_BUFFER_SIZE;
	HRESULT rs = _keyBoard->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), _keyEvents, &dw, 0);
	int keyCode, keyState;
	for (DWORD i = 0; i < dw; i++)
	{
		keyCode = _keyEvents[i].dwOfs;
		keyState = _keyEvents[i].dwData;
		if ((keyState & 0x80) > 0)
		{
			KeyEventArg* arg = new KeyEventArg(keyCode);
			__raise __enventKeyPressed(arg);
			delete arg;
		}
		else
		{
			KeyEventArg* arg = new KeyEventArg(keyCode);
			__raise __enventKeyReleased(arg);
			delete arg;
		}
	}
}			
int	InputController::isKeyDown(int keyCode)
{
	return ((_keyBuffer[keyCode] & 0x80) > 0);
}

