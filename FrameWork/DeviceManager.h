#pragma once
#include "define.h"
#include "Graphics.h"
_NAMESPACE_FRAMEWORK_BEGIN
///Hàm quản lý device cho game
///Gồm D3DirectX
///DEVICE
///SURFACE
///_instance sẽ là con trỏ của DeviceManger và là static để truy cập thông quan tên class
///vô game sẽ gọi DeviceManager::getInstance->Init(Graphics);
///
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