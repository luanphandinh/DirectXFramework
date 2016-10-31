#pragma once
#include"Config.h"
#include"TileSet.h"
/*
	Map là class dùng để quản lý việc vẽ map bằng tiles

	Một tileset được lưu như một resource hình ảnh bao gồm nhiều ô nhỏ chứ một phần nhỏ của map.
	Vị trí các tile được lưu dưới dạng 2dmatrix trên file xml
	Với giá trị các phân từ trong ma trận là (index của tile trong tiletSet.lisTile.id + 1)
	Vì giá trị 0 tại map.xml là ko có tile,và các tile được đánh dấu từ 1.

	File xml được load bằng phương thứ init,truyền vào đường dẫn tương đối của file
	hình ảnh được load nhứ sprite thông qua sprite manager

*/
class Map
{
public:
	void draw(LPD3DXSPRITE, Viewport*);
	void release();
	/*
		Load dữ liệu từ file xml.
			@path: file tilemap xml được định nghĩa cấu trúc sẵn
			@eID : sprite id được đăng kí ở sprite manager 
	*/
	static Map* LoadFromFile(const string path, eID spriteId);

	GVector2 getWorldSize();
	Map();
	~Map();

private:
	int _frameWidth;
	int _frameHeight;
	/*
		Lấy kích thước map/
	*/
	GVector2 _mapSize;

	/*
		Ma trận Tile id.
		Id trong ma trận này được so sánh với Id trong Tileset.listTile
	*/
	int** _mapIndex;

	//Chứa list tile
	TileSet* _tileSet;
	/*
		Lấy thông tin một attribute từ một node.Sử dụng pugi framework
			node : một element chứa attribute cần tìm.
			attributename : tên của attribute muốn tìm.
	*/
	static xml_attribute getAttributeValue(const pugi::xml_node& node, string attributeName);

	/*
		Đọc dữ liệu  một mảng tile id
			node : một element chứ mảng id cần tìm
			matrix : tham chiếu đến ma trận index
	*/
	static void getElementMatrixIndex(pugi::xml_node& node, int** matrix);

};

