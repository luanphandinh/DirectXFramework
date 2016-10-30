#include "Tile.h"

Tile::Tile(Sprite* refSprite, RECT srcRect, int id)
{
	this->_id = id;
	this->_refSprite = refSprite;
	this->_srcRect = srcRect;
}


Tile::~Tile()
{
}


const int& Tile::getId() const
{
	return this->_id;
}

void Tile::draw(LPD3DXSPRITE spriteHandler, GVector2 position, Viewport* viewport)
{
	if (_refSprite == NULL)
		throw new exception("refSprite is not exist");

	_refSprite->setPosition(position);
	_refSprite->setFrameRect(this->_srcRect);

	if (viewport == NULL)
	{
		_refSprite->render(spriteHandler);
	}
	else
	{
		_refSprite->render(spriteHandler, viewport);
	}
}