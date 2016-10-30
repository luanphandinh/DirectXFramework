#pragma once
#include<vector>
#include<string>
#include"Config.h"
#include"Tile.h"

class TileSet
{
private:
	Sprite* _tileImage;
	int _widthTile;
	int _heightTile;
	string	_fileName;
	int _columns;
	int _rows;
	vector<Tile*> _listTiles;

public:
	TileSet(eID spriteId);

	~TileSet();

	int getWidthtile() const;
	void setWidthtile(const int &value);
	int getHeighttile() const;
	void setHeighttile(const int &value);

	void loadListTiles(pugi::xml_node &node);
	Sprite* getSprite();
	void draw(LPD3DXSPRITE spriteHandler, int id, GVector2 position, Viewport* viewport);
};

