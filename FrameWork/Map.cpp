#include "Map.h"
#include<math.h>

Map::Map()
{
	_mapIndex = NULL;
}


Map::~Map()
{

}
void Map::draw(LPD3DXSPRITE spriteHandler, Viewport* viewport)
{
	/*
		Để vẽ được map lên màn hình,ta sẽ chỉ vẽ phần hiển thị của map tương ứng
		với vùng mà viewport bao phủ
	*/
	RECT screenRectEx =
	{
		viewport->getPositionWorld().x,
		viewport->getPositionWorld().y,
		viewport->getPositionWorld().x + viewport->getWidth(),
		viewport->getPositionWorld().y - viewport->getHeight()
	};//Vẽ theo tọa đồ decac
	/*
		Giả sử ma trận _mapIndex sẽ có 7 hàng và 10 cột mỗi frame sẽ là (100,100)
		viewport sẽ view có x = 300,y = 600
	*/
	int iBegin = max(screenRectEx.left / _frameWidth, 0);//3
	int iEnd = min(screenRectEx.right / _frameWidth + 1, _mapSize.x);//7
	int jBegin = _mapSize.y - min(screenRectEx.top / this->_frameHeight + 1, _mapSize.y);//0
	int jEnd = _mapSize.y - max(screenRectEx.bottom / _frameHeight, 0);//5

	GVector2 pos;
	/*
		Vì ma trận _mapIndex lưu các tile theo chiều x sang ngang y xuống dưới(top-left)
		Khi vẽ ta sẽ vẽ từ iBegin và jBegin bằng _mapIndex[j][i] vì j theo hàng và i theo cột
		Tọa độ sẽ được phát triển từ 0-0 theo hệ tọa độ decac.
	*/
	for (int i = iBegin; i < iEnd; i++)
	{
		for (int j = jBegin; j < jEnd; j++)
		{
			pos.x = i * _frameWidth;
			pos.y = (_mapSize.y - j - 1) * _frameHeight;
			this->_tileSet->draw(spriteHandler, this->_mapIndex[j][i], pos, viewport);
		}
	}

}

void Map::release()
{

}

Map* Map::LoadFromFile(const string path, eID spriteId)
{
	pugi::xml_document doc;

	//Mở file và đọc
	xml_parse_result result = doc.load_file(path.data(), pugi::parse_default | pugi::parse_pi);
	if (result == false)
	{
		return nullptr;
	}
	Map* map = new Map();
	/*
	Đọc dữ liệu từ element Tilesmap
		<Tilesmap columns="208" rows="14">
		
		</Tilesmap>
	*/
	xml_node tilesMap = doc.child("Tilesmap");
	if (tilesMap == NULL)
		return nullptr;
	map->_mapSize.x = getAttributeValue(tilesMap, "columns").as_int();
	map->_mapSize.y = getAttributeValue(tilesMap, "rows").as_int();

	//Khởi tạo mảng 2 chiều
	map->_mapIndex = new int*[int(map->_mapSize.y)];
	for (int i = 0; i < map->_mapSize.y; i++)
	{
		map->_mapIndex[i] = new int[(int)map->_mapSize.x];
	}
	/*
	<MatrixIndex>
		<Row id="0"></Row>
	<MatrixIndex>
	*/
	xml_node matrixIndex = tilesMap.child("MatrixIndex");
	if (matrixIndex == NULL)
		return nullptr;
	getElementMatrixIndex(matrixIndex, map->_mapIndex);

	/*
		Đọc tileSet
	*/
	xml_node tileSet = tilesMap.child("TileSet");
	map->_tileSet = new TileSet(spriteId);
	map->_tileSet->loadListTiles(tileSet);

	map->_frameWidth = map->_tileSet->getSprite()->getFrameWidth();
	map->_frameHeight = map->_tileSet->getSprite()->getFrameHeight();

	return map;
}

GVector2 Map::getWorldSize()
{
	GVector2 result;
	result.x = this->_mapSize.x * this->_frameWidth;
	result.y = this->_mapSize.y * this->_frameHeight;
	return result;
}

xml_attribute Map::getAttributeValue(const pugi::xml_node& node, string attributeName)
{
	//Chưa hiểu cái này,cần tìm hiểu
	return node.find_attribute(
		[&](xml_attribute attr) -> bool
		{
		if (string(attr.name()).compare(attributeName) == 0)
			return true;
		return false;
		});
}

void Map::getElementMatrixIndex(pugi::xml_node& node, int** matrix)
{
	/*
		Hàm này thực hiên duyệt tất cả Row của matrixIndex
		Cấu trúc có dang:
		<MatrixIndex>
			<Row id="0">0	0	0	0	0	1	3	4</Row>
			<Row id="1">0	2	5	4	6	9	8	0</Row>
		<MatrixIndex>
	*/
	//lấy từ row thứ nhất
	xml_node child = node.first_child();
	//Chuỗi giá trị từ node
	string indexStr;

	//Cặp giá trị [i][j]
	int i = 0, j = 0;
	while (child != nullptr)
	{
		char *temp = NULL;
		char *pch = NULL;
		//Lấy giá trị chuỗi của một row
		indexStr = child.text().as_string();
		//Lấy id của row cũng là giá trị i của hàng trong mảng hai chiều
		i = child.attribute("id").as_int();

		auto str = splitString(indexStr, '\t');

		int j = 0;
		for (string tmp : str)
		{
			matrix[i][j] = atoi(tmp.c_str());
			j++;
		}

		child = child.next_sibling();
		str.clear();
	}
}
