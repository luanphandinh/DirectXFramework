#include "TileSet.h"


TileSet::TileSet(eID spriteId)
{
	Sprite* sp = SpriteManager::getInstance()->getSprite(spriteId);
	this->_tileImage = sp;
	this->_heightTile = sp->getFrameHeight();
	this->_widthTile = sp->getFrameWidth();
}


TileSet::~TileSet()
{
}

//void TileSet::loadListTiles(pugi::xml_node &node)
//{
//	/*
//		tilelistnode chứa tất cả các element <Tile> trong file xml.
//		Cấu trúc dạng:
//		<Tiles>
//			<Tile>
//		<Tiles>
//	*/
//	auto tileListNode = node.child("Tiles").children();
//	RECT srcRect = { 0, 0, 0, 0 };
//	int id = 0;
//	for each (pugi::xml_node_iterator it in tileListNode)
//	{
//		id = it->attribute("id").as_int();
//		srcRect.top = it->child("Rect").attribute("Y").as_int();
//		srcRect.left = it->child("Rect").attribute("X").as_int();
//		srcRect.right = srcRect.left + it->child("Rect").attribute("Width").as_int() + 1;
//		srcRect.bottom = srcRect.top + it->child("Rect").attribute("Height").as_int() + 1;
//		this->_listTiles.push_back(new Tile(this->_tileImage, srcRect, id));
//	}
//}

Sprite* TileSet::getSprite()
{
	return _tileImage;
}

void TileSet::draw(LPD3DXSPRITE spriteHandler, int id, GVector2 position, Viewport* viewport)
{
	for (auto tile : _listTiles)
	{
		if (tile->getId() == id)
		{
			tile->draw(spriteHandler, position, viewport);
			return;
		}
		else
		{
			continue;
		}
	}
}

int TileSet::getWidthtile() const
{
	return this->_widthTile;
}

void TileSet::setWidthtile(const int &value)
{
	if (this->_widthTile != value)
		this->_widthTile = value;
}

int TileSet::getHeighttile() const
{
	return this->_heightTile;
}

void TileSet::setHeighttile(const int &value)
{
	if (this->_heightTile != value)
		this->_heightTile = value;
}
