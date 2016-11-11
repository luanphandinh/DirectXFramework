#pragma once

#include"define.h"
#include"Sprite.h"
#include <fstream>
#include"Config.h"
_NAMESPACE_FRAMEWORK_BEGIN
/*
	+ SpriteManager là class dùng để quản lý việc load và release sprite và texture
	+ Class sẽ load tất cả các image cần thiết,nếu cần sử dụng sprite ở đâu
	thì gọi hàm getSprite() để lấy

	+ Để sử dụng thì phải gọi SpriteManager::getInstance();
		Có thể gọi trực tiếp : SpriteManager::getInstance()->DOSOMETHING()
		hay sử dụng một đối tượng tham chiếu đến :
		 		SpriteManager* _spritemanager;
				...
				_spritemanager = SpriteManager::getInstance();
				...
				_spritemanager->DOSOMETHING()
	+ Gọi hàm SpriteManager::loadResource trong Game::loadResource() để load image
	+ Gọi hàm SpriteManager::release() trong game::release để clean memory
*/
class SpriteManager
{
public:
	static SpriteManager* getInstance();
	static void release();

	void loadResource(LPD3DXSPRITE spriteHandler);
	Sprite* getSprite(eID id);

	RECT getSourceRect(eID, string name);
	void loadSpriteInfo(eID id, const char* fileInfoPath);
	GVector2 getSourceOrigin(eID, string);
	//release the sprite,ko release texture
	void releaseSprite(eID id);
	void releaseTexture(eID id);
	
	~SpriteManager();
private:
	SpriteManager();
	static SpriteManager* _instance;
	//	Mỗi eID tương ứng với một nhân vật sẽ có 1 spriteSheet
	map<eID, Sprite*> _listSprite;
	/*
		+ Trong mỗi nhân vật,sẽ có các string thể hiện hành động
		+ Các hành động đó sẽ được lưu bằng 1 rect cụ thể trên spriteSheet
		+ Trong nhân vật sẽ lấy spriteSheet tương ứng với ID
			và lấy các rect với string cho các animate cụ thể
	*/
	map<eID, map<string, RECT>> _sourceRectList;
	map<eID, map<string, GVector2>> _sourceOriginList;

	Sprite* loadXMLDoc(LPD3DXSPRITE spriteHandler, LPWSTR path);
};

_NAMESPACE_FRAMEWORK_END

