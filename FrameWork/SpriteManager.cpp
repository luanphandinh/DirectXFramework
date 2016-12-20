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
	//SIMON
	Sprite* sp = new Sprite(spriteHandler, L"Resources//Images//simon_animation.png");
	this->_listSprite[eID::SIMON] = sp;
	this->loadSpriteInfo(eID::SIMON, "Resources//Images//simon_animation.txt");
	//SPEARKNGIHT
	sp = new Sprite(spriteHandler, L"Resources//Images//spearKnight.png");
	this->_listSprite.insert(pair<eID, Sprite*>(eID::SPEARKNIGHT, sp));
	this->loadSpriteInfo(eID::SPEARKNIGHT, "Resources//Images//spearKnight_animation.txt");
	//BAT
	sp = new Sprite(spriteHandler, L"Resources//Images//bat.png");
	this->_listSprite.insert(pair<eID, Sprite*>(eID::BAT, sp));
	this->loadSpriteInfo(eID::BAT, "Resources//Images//bat_animation.txt");
	//MEDUSA
	sp = new Sprite(spriteHandler, L"Resources//Images//medusaHead.png");
	this->_listSprite.insert(pair<eID, Sprite*>(eID::MEDUSAHEAD, sp));
	this->loadSpriteInfo(eID::MEDUSAHEAD, "Resources//Images//medusaHead_animation.txt");
	//ITEM
	sp = new Sprite(spriteHandler, L"Resources//Images//item.png");
	this->_listSprite.insert(pair<eID, Sprite*>(eID::ITEM, sp));
	this->loadSpriteInfo(eID::ITEM, "Resources//Images//item_info.txt");

	// DOOR
	sp = new Sprite(spriteHandler, L"Resources//Images//door.png");
	this->_listSprite.insert(pair<eID, Sprite*>(eID::DOOR, sp));
	this->loadSpriteInfo(eID::DOOR, "Resources//Images//door_animation.txt");

	// FLYLAND
	sp = new Sprite(spriteHandler, L"Resources//Images//flyLand.png");
	this->_listSprite.insert(pair<eID, Sprite*>(eID::FLYLAND, sp));
	this->loadSpriteInfo(eID::FLYLAND, "Resources//Images//flyLand_animation.txt");

	// BURNING
	sp = new Sprite(spriteHandler, L"Resources//Images//burning.png");
	this->_listSprite[eID::HITEFFECT] = sp;
	this->loadSpriteInfo(eID::HITEFFECT, "Resources//Images//burning_animation.txt");

	//BRICK
	sp = new Sprite(spriteHandler, L"Resources//Images//brick.png");
	this->_listSprite[eID::BRICK] = sp;
	this->loadSpriteInfo(eID::BRICK, "Resources//Images//brick_animation.txt");

	sp = new Sprite(spriteHandler, L"Resources//Images//brokenbrick.png");
	this->_listSprite[eID::BROKENBRICK] = sp;
	this->loadSpriteInfo(eID::BROKENBRICK, "Resources//Images//brokenbrick_animation.txt");

	//DRAGON
	sp = new Sprite(spriteHandler, L"Resources//Images//dragon.png");
	this->_listSprite[eID::DRAGON] = sp;
	this->loadSpriteInfo(eID::DRAGON, "Resources//Images//dragon_animation.txt");

	//WHIP
	sp = new Sprite(spriteHandler, L"Resources//Images//whip.png");
	this->_listSprite[eID::WHIP] = sp;
	this->loadSpriteInfo(eID::WHIP, "Resources//Images//whip_info.txt");

	// SPIKE
	sp = new Sprite(spriteHandler, L"Resources//Images//spike.png");
	this->_listSprite.insert(pair<eID, Sprite*>(eID::SPIKE, sp));
	this->loadSpriteInfo(eID::SPIKE, "Resources//Images//spike_animation.txt");
	// GHOST
	sp = new Sprite(spriteHandler, L"Resources//Images//ghost.png");
	this->_listSprite.insert(pair<eID, Sprite*>(eID::GHOST, sp));
	this->loadSpriteInfo(eID::GHOST, "Resources//Images//ghost_animation.txt");

	// MEDUSA
	sp = new Sprite(spriteHandler, L"Resources//Images//medusa.png");
	this->_listSprite.insert(pair<eID, Sprite*>(eID::MEDUSA, sp));
	this->loadSpriteInfo(eID::MEDUSA, "Resources//Images//medusa_animation.txt");

	// SNAKE
	sp = new Sprite(spriteHandler, L"Resources//Images//snake.png");
	this->_listSprite.insert(pair<eID, Sprite*>(eID::SNAKE, sp));
	this->loadSpriteInfo(eID::SNAKE, "Resources//Images//snake_animation.txt");

	// FLEAMAN
	sp = new Sprite(spriteHandler, L"Resources//Images//fleaman.png");
	this->_listSprite.insert(pair<eID, Sprite*>(eID::FLEAMAN, sp));
	this->loadSpriteInfo(eID::FLEAMAN, "Resources//Images//fleaman_animation.txt");

	// RAVEN
	sp = new Sprite(spriteHandler, L"Resources//Images//raven.png");
	this->_listSprite.insert(pair<eID, Sprite*>(eID::RAVEN, sp));
	this->loadSpriteInfo(eID::RAVEN, "Resources//Images//raven_animation.txt");

	// SKELETON
	sp = new Sprite(spriteHandler, L"Resources//Images//skeleton.png");
	this->_listSprite.insert(pair<eID, Sprite*>(eID::SKELETON, sp));
	this->loadSpriteInfo(eID::SKELETON, "Resources//Images//skeleton_animation.txt");

	//MUMMYMAN
	sp = new Sprite(spriteHandler, L"Resources//Images//MummyMan.png");
	this->_listSprite.insert(pair<eID, Sprite*>(eID::MUMMYMAN, sp));
	this->loadSpriteInfo(eID::MUMMYMAN, "Resources//Images//mummy_man_animation.txt");
	//BANDAGES
	sp = new Sprite(spriteHandler, L"Resources//Images//bandages.png");
	this->_listSprite.insert(pair<eID, Sprite*>(eID::BANDAGE, sp));
	this->loadSpriteInfo(eID::BANDAGE, "Resources//Images//badages_animation.txt");

	//sp = loadXMLDoc(spriteHandler, L"Resources//Maps//test.xml");
	//sp->setOrigin(GVector2(0.0f, 0.0f));
	//this->_listSprite[eID::MAPSTAGE1] = sp;
	//IntroScene
	sp = new Sprite(spriteHandler, L"Resources//Images//introScene.png");
	this->_listSprite.insert(pair<eID, Sprite*>(eID::INTROSCENE, sp));
	this->loadSpriteInfo(eID::INTROSCENE, "Resources//Images//introSceneInfo.txt");

	sp = loadXMLDoc(spriteHandler, L"Resources//Maps//level2.xml");
	sp->setOrigin(GVector2(0.0f, 0.0f));
	this->_listSprite[eID::LEVEL2] = sp;

	sp = loadXMLDoc(spriteHandler, L"Resources//Maps//level3.xml");
	sp->setOrigin(GVector2(0.0f, 0.0f));
	this->_listSprite[eID::LEVEL3] = sp;
}

//Dùng để load sprite từ file map
//Vì ta sử dụng map .xml và map.png cùng tên
Sprite* SpriteManager::loadXMLDoc(LPD3DXSPRITE spriteHandler, LPWSTR path)
{
	xml_document doc;
	xml_parse_result result = doc.load_file(path, parse_default | parse_pi);
	if (result == false)
	{
		OutputDebugString(L"Khong tìm thấy file xml");
		return nullptr;
	}
	xml_node root = doc.first_child();//Tilesmap
	xml_node tileSet_node = root.child("TileSet");
	//Tìm tên file
	//Cắt từ chuỗi path ra để tìm thư mục
	//Sau đó ghép với tên file ảnh được lấy từ file xml để laod ảnh
	string fileName = tileSet_node.attribute("FileName").as_string();//Lấy tên hình từ file xml
	wstring L_fileName = wstring(fileName.begin(), fileName.end());//convert to wstring

	wstring strPath = wstring(path);
	int index = strPath.find_last_of(L'//');
	strPath = strPath.substr(0, index);
	strPath += L"/" + L_fileName;
	//tìm số dòng và số cột để add cho sprite
	int rows = tileSet_node.attribute("Rows").as_int();
	int columns = tileSet_node.attribute("Columns").as_int();
	return new Sprite(spriteHandler, (LPWSTR)strPath.c_str(), rows * columns, columns);
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

GVector2 SpriteManager::getSourceOrigin(eID id, string name)
{
	return _sourceOriginList[id][name];
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
			GVector2 origin;
			char name[100];
			fgets(name, 100, file);

			fscanf(file, "%s %d %d %d %d %f %f", &name, &rect.left, &rect.top, &rect.right, &rect.bottom,&origin.x,&origin.y);

			_sourceRectList[id][string(name)] = rect;
			_sourceOriginList[id][string(name)] = origin;
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