#pragma once
///26/10/2016
#include<Windows.h>
#include"define.h"
_NAMESPACE_FRAMEWORK_BEGIN

class Graphics
{
public:
	//Constructor
	Graphics(HINSTANCE hInstance,LPWSTR name,int width,int height,int fps,int isFullScreen);
	~Graphics();
	void InitWindow();

	//return 1 if fullscreen either return 0
	int isFullScreen();
	
	int getWidth();
	int getHeight();
	//chưa rõ cách sử dụng
	int getFrameRate();
	//trả về window 
	HWND getWnd();
	//trả về handle instance giữ window
	HINSTANCE gethInstance();
protected:
	//handle instance giữ _hWnd
	HINSTANCE		_hInstance;
	HWND			_hWnd;

	//Tên window
	LPWSTR			_wcName;
	int				_isFullScreen;
	int				_width;
	int				_height;
	int				_fps;

	static HRESULT CALLBACK winProc(HWND, UINT, WPARAM, LPARAM);
	Graphics();
};


//Pointer to a windows object
typedef Graphics* pGraphics;

_NAMESPACE_FRAMEWORK_END
