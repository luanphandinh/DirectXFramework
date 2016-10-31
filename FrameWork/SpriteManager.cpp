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
	Sprite* sp = new Sprite(spriteHandler, L"Resources//Images//simon_animation.png");
	this->_listSprite[eID::SIMON] = sp;
	this->loadSpriteInfo(eID::SIMON, "Resources//Images//simon_animation.txt");

	sp = loadXMLDoc(spriteHandler, L"Resources//Maps//test.xml");
	this->_listSprite[eID::MAPSTAGE1] = sp;
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