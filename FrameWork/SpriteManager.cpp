#include "SpriteManager.h"
_USING_FRAMEWORK


SpriteManager* SpriteManager::_instance = nullptr;

SpriteManager::SpriteManager()
{
}


SpriteManager::~SpriteManager()
{
}


void  SpriteManager::loadResource(LPD3DXSPRITE spriteHandler)
{
	Sprite* simon = new Sprite(spriteHandler, L"Resources//Images//simon_animation.png");
	this->_listSprite[eID::SIMON] = simon;
	this->loadSpriteInfo(eID::SIMON, "Resources//Images//simon_animation.txt");
}

Sprite*  SpriteManager::getSprite(eID id)
{
	Sprite *it = this->_listSprite.find(id)->second;
	return new Sprite(*it);
}

RECT  SpriteManager::getSourceRect(eID id, string name)
{
	return _sourceRectList[id][name];
}

void  SpriteManager::loadSpriteInfo(eID id, const char* fileInfoPath)
{
	FILE* file;
	file = fopen(fileInfoPath, "r");

	if (file)
	{
		while (!feof(file))
		{
			RECT rect;
			char name[100];
			fgets(name, 100, file);

			fscanf(file, "%s %d %d %d %d", &name, &rect.left, &rect.top, &rect.right, &rect.bottom);

			_sourceRectList[id][string(name)] = rect;
		}
	}

	fclose(file);
}


void  SpriteManager::releaseSprite(eID id)
{
	Sprite* it = this->_listSprite.find(id)->second;
	delete it;
	this->_listSprite.erase(id);
}

void  SpriteManager::releaseTexture(eID id)
{
	Sprite* spr = this->_listSprite.find(id)->second;
	spr->release();
	delete spr;
	this->_listSprite.erase(id);
}

SpriteManager* SpriteManager::getInstance()
{
	if (_instance == nullptr)
		_instance = new SpriteManager;
	return _instance;
}

void SpriteManager::release()
{
	delete _instance;
	_instance = nullptr;
}