#pragma once
#include"Config.h"
#include<string>
/*
	Tương tự trong MapEditor
	Tile là class bao gồm id và rect trên tileSet
*/
class Tile
{
public:
	Tile(Sprite* refSprite,RECT srcRect,int id);
	~Tile();

	const int &getId() const;

	void draw(LPD3DXSPRITE spriteHandler, GVector2 position, Viewport* viewport = NULL);
private:
	int	_id;
	RECT _srcRect;

	Sprite* _refSprite;// ảnh gồm nhiều tile. khi vẽ cắt bằng srcRect như Sprite
};

