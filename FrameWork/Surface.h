#pragma once
#include"define.h"
#include<d3dx9.h>
_NAMESPACE_FRAMEWORK_BEGIN
class Surface
{
public:
	Surface();
	Surface(const Surface&);
	~Surface();

	void LoadSurfaceFromFile(LPDIRECT3DDEVICE9, D3DCOLOR, LPCWSTR);
	void RenderSurface(LPDIRECT3DDEVICE9, RECT*, RECT*);
	void UnloadSurface();
private:
	
	LPDIRECT3DSURFACE9 m_lpSurface;//để chứa cái hình
	LPDIRECT3DSURFACE9 m_lpBackBuffer; //để vẽ lên frame tạm
};
_NAMESPACE_FRAMEWORK_END

