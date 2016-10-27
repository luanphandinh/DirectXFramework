#include "Texture.h"

_USING_FRAMEWORK

Texture::Texture()
{
	_color = C_WHITE;
	_texture = nullptr;
}

Texture::~Texture()
{}


void Texture::release()
{

}

HRESULT Texture::loadFromFile(LPD3DXSPRITE spriteHandle, LPWSTR filePath, D3DXCOLOR color, D3DXCOLOR colorkey) 
{
	HRESULT	result;

	result = D3DXGetImageInfoFromFile(filePath, &this->_imageInfo);
	if (result != D3D_OK)
	{
		return result;
	}

	LPDIRECT3DDEVICE9 _device = DeviceManager::getInstance()->getDevice();
	spriteHandle->GetDevice(&_device);

	result = D3DXCreateTextureFromFileEx(
		_device,
		filePath,
		this->_imageInfo.Width,
		this->_imageInfo.Height,
		1,
		D3DUSAGE_DYNAMIC,
		D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		colorkey,
		&this->_imageInfo,
		nullptr,
		&this->_texture);

	_color = color;

	return result;
}

void Texture::render(LPD3DXSPRITE spriteHandle, const RECT* rect, const GVector3* center, const GVector3* position)
{
	spriteHandle->Begin(D3DXSPRITE_ALPHABLEND);
	spriteHandle->Draw(
		this->_texture,
		rect,
		center,
		position,
		_color
	);
	spriteHandle->End();
}

void Texture::setColor(D3DXCOLOR color)
{
	this->_color = color;
}

D3DXCOLOR Texture::getColor()
{
	return _color;
}


int Texture::getWidth()
{
	return this->_imageInfo.Width;
}

int Texture::getHeight()
{
	return this->_imageInfo.Height;
}