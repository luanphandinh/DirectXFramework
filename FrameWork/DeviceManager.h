#pragma once
#include "define.h"
#include "Graphics.h"
_NAMESPACE_FRAMEWORK_BEGIN

/*
	+ Class dùng để quản lý device và surface cho game
	+ _instance sẽ là con trỏ của DeviceManger và là static để truy cập thông quan tên class
	được gọi để sử dụng thông qua DeviceManager::getInstance()
	+ _instance được khởi tạo tại game init() và được sử dụng global thông qua lời gọi
	DeviceManager::getInstance()
	+ vì bản thân DeviceManager::getInstance() là một device manager lên sẽ bao
	gồm getDevice() và getSurface() để sử dụng tại bất cứ đâu
*/
class DeviceManager
{
public:
	~DeviceManager();
	//Trả về đối tượng DeviceManager
	static DeviceManager* getInstance();
	static void release();
	void clearScreen();
	//gọi hàm present để vẽ lên front-buffer
	void present();
	void Init(Graphics);

	LPDIRECT3DDEVICE9 getDevice();
	LPDIRECT3DSURFACE9 getSurface();
private:
	static DeviceManager*	_instance;
	LPDIRECT3D9				_pD3d;
	LPDIRECT3DDEVICE9		_pDevice;
	LPDIRECT3DSURFACE9		_surface;
	DeviceManager();
};
typedef DeviceManager* pDeviceManager;

_NAMESPACE_FRAMEWORK_END