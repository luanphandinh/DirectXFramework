#include "ObjectFactory.h"


ObjectFactory::ObjectFactory()
{
}


ObjectFactory::~ObjectFactory()
{
}

vector<BaseObject*>* ObjectFactory::getListObjectFromFile(const string path)
{
	pugi::xml_document doc;
	vector<BaseObject*>* listobject = new vector<BaseObject*>();

	//Mở file
	xml_parse_result result = doc.load_file(path.data(), pugi::parse_default | pugi::parse_pi);
	if (result == false)
	{
		return listobject;
	}

	//Đọc Tilesmap nếu ko có thì return
	xml_node tilemap = doc.child("Tilesmap");
	if (tilemap == NULL)
		return listobject;

	/*
	Đọc tới node con Objects trong tilemap
	Lấy các phần tử trong Objects vào list
	*/
	xml_node objects = tilemap.child("Objects");
	auto list = objects.children();

	for (auto item : list)
	{
		int id = item.attribute("Id").as_int();
		eID enumID;
		try
		{
			enumID = (eID)id;
		}
		catch (exception e)
		{
			continue;
		}
		BaseObject* obj = getObjectById(item, enumID);
//		obj->setZIndex(0.5f);
		if (obj != NULL)
			listobject->push_back(obj);
	}

	return listobject;
}

map<string, BaseObject*>* ObjectFactory::getMapObjectFromFile(const string path)
{
	pugi::xml_document doc;
	map<string,BaseObject*>* listobject = new map<string,BaseObject*>();

	//Mở file
	xml_parse_result result = doc.load_file(path.data(), pugi::parse_default | pugi::parse_pi);
	if (result == false)
	{
		return listobject;
	}

	//Đọc Tilesmap nếu ko có thì return
	xml_node tilemap = doc.child("Tilesmap");
	if (tilemap == NULL)
		return listobject;

	/*
	Đọc tới node con Objects trong tilemap
	Lấy các phần tử trong Objects vào list
	*/
	xml_node objects = tilemap.child("Objects");
	auto list = objects.children();

	for (auto item : list)
	{
		int id = item.attribute("Id").as_int();
		string name = item.attribute("Name").as_string();
		eID enumID;
		try
		{
			enumID = (eID)id;
		}
		catch (exception e)
		{
			continue;
		}
		BaseObject* obj = getObjectById(item, enumID);
		obj->setZIndex(0.5f);
		if (obj != NULL)
			(*listobject)[name] = obj;
	}

	return listobject;
}



BaseObject* ObjectFactory::getObjectById(xml_node node, eID id)
{
	switch (id)
	{
	case LAND:
		return getLand(node);
		break;
		case DOOR:
			return getDoor(node);
			break;
		case FLYLAND:
			return getFlyLand(node);
	case STAIR:
		return getStair(node);
		break;
	case STAIR_LEFTBOTTOM_RIGHTTOP:
		break;
	case STAIR_RIGHTBOTTOM_LEFTTOP:
		break;
	case ITEM:
		break;
	case WEAPON:
		break;
	case SPECIALITEM:
		break;
	case SPEARKNIGHT:
		return getSpearKnight(node);
		break;
	case BAT:
		return getBat(node);
		break;
	case MEDUSAHEAD:
		return getMedusaHead(node);
		break;
	case MAPSTAGE1:
		break;
	case LIFE_ICON:
		break;
	default:
		break;
	}
}
BaseObject* ObjectFactory::getLand(xml_node node)
{
	auto properties = ObjectFactory::getObjectProperties(node);
	if (properties.size() == 0)
		return nullptr;

	int x, y, width, height;
	eDirection direction;
	bool canJumpDown;

	x = stoi(properties["X"]);
	y = stoi(properties["Y"]);
	width = stoi(properties["Width"]);
	height = stoi(properties["Height"]);

	
	if (properties.find("physicBodyDirection") != properties.end())
	{
		direction = (eDirection)(stoi(properties.find("physicBodyDirection")->second));
	}
	else
	{
		direction = eDirection::TOP;
	}

	if (properties.find("canJumpDown") != properties.end())
	{
		canJumpDown = (bool)(stoi(properties.find("canJumpDown")->second));
	}
	else
	{
		canJumpDown = true;
	}

	auto land = new Land(x, y, width, height, direction);
	land->init();
	land->enableJump(canJumpDown);

	return land;
}

BaseObject* ObjectFactory::getStair(xml_node node)
{
	auto properties = ObjectFactory::getObjectProperties(node);
	if (properties.size() == 0)
		return nullptr;

	int x, y, width, height;
	eDirection direction;
	eStairDirection stairDirection;

	x = stoi(properties["X"]);
	y = stoi(properties["Y"]);
	width = stoi(properties["Width"]);
	height = stoi(properties["Height"]);


	if (properties.find("physicBodyDirection") != properties.end())
	{
		direction = (eDirection)(stoi(properties.find("physicBodyDirection")->second));
	}
	else
	{
		direction = eDirection::TOP;
	}

	if (properties.find("stairDirection") != properties.end())
	{
		stairDirection = (eStairDirection)(stoi(properties.find("stairDirection")->second));
	}
	else
	{
		stairDirection = eStairDirection::LEFTBOTTOM_TO_RIGHTTOP;
	}

	auto stair = new Stair(x, y, width, height, direction,stairDirection);
	stair->init();

	return stair;
}

BaseObject * ObjectFactory::getBat(xml_node node) {
	auto properties = getObjectProperties(node);
	if (properties.size() == 0)
		return nullptr;

	int x, y, direction;
	eStatus status;

	x = stoi(properties["X"]);
	y = stoi(properties["Y"]);

	if (properties.find("status") != properties.end()) {
		status = (eStatus)(stoi(properties.find("status")->second));
	}
	else {
		status = eStatus::HANGING;
	}

	if (properties.find("direction") != properties.end()) {
		direction = stoi(properties.find("direction")->second);
	}
	else {
		direction = 1;
	}

	auto bat = new Bat(status, GVector2(x, y), direction);
	bat->init();

	return bat;
}

BaseObject * ObjectFactory::getSpearKnight(xml_node node) {
	auto properties = getObjectProperties(node);
	if (properties.size() == 0)
		return nullptr;

	int x, y, direction;
	eStatus status;

	x = stoi(properties["X"]);
	y = stoi(properties["Y"]);

	if (properties.find("status") != properties.end()) {
		status = (eStatus)(stoi(properties.find("status")->second));
	}
	else {
		status = eStatus::WALKING;
	}

	if (properties.find("direction") != properties.end()) {
		direction = stoi(properties.find("direction")->second);
	}
	else {
		direction = 1;
	}

	auto spearKnight = new SpearKnight(status, GVector2(x, y), direction);
	spearKnight->init();

	return spearKnight;
}

BaseObject * ObjectFactory::getMedusaHead(xml_node node) {
	return nullptr;
}

BaseObject * ObjectFactory::getDoor(xml_node node) {
	return nullptr;
}

BaseObject * ObjectFactory::getFlyLand(xml_node node) {
	return nullptr;
}

map<string, string> ObjectFactory::getObjectProperties(xml_node node)
{
	map<string, string> properties;
	//Lấy phần active rectangel của object
	properties["X"] = node.attribute("X").as_string();
	properties["Y"] = node.attribute("Y").as_string();
	properties["Width"] = node.attribute("Width").as_string();
	properties["Height"] = node.attribute("Height").as_string();

	//parameters
	xml_node params = node.child("Params");
	for (auto item : params)
	{
		auto key = item.attribute("Key").as_string();
		auto value = item.attribute("Value").as_string();
		properties[key] = value;
	}

	return properties;
}
