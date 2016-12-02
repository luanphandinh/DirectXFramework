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
		obj->setZIndex(0.5f);
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
		//obj->setZIndex(0.5f);
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
		break;
	case STAIR:
		return getStair(node);
		break;
	case CANDLE:
		return getCandle(node);
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
	case SPAWNER:
		return getSpawner(node);
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
	bool canUpStair;

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

	if (properties.find("canUpStair") != properties.end())
	{
		canUpStair = (bool)(stoi(properties.find("canUpStair")->second));
	}
	else
	{
		canUpStair = true;
	}


	auto stair = new Stair(x, y, width, height, direction, stairDirection, canUpStair);
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
	auto properties = getObjectProperties(node);
	if (properties.size() == 0)
		return nullptr;

	int x, y,direction;
	eStatus status;
	GVector2 pos, hVeloc, ampl;
	float freq;

	pos.x = stoi(properties["X"]);
	pos.y = stoi(properties["Y"]);
	// status
	if (properties.find("status") != properties.end()) {
		status = (eStatus)(stoi(properties.find("status")->second));
	}
	else {
		status = eStatus::FLYING;
	}

	if (properties.find("direction") != properties.end()) {
		direction = stoi(properties.find("direction")->second);
	}
	else {
		direction = 1;
	}

	// ampl
	if (properties.find("Amplitude") != properties.end()) {
		auto velocStr = properties.find("Amplitude")->second;
		auto value = splitString(velocStr, ',');

		ampl.x = stoi(value[0]);
		ampl.y = stoi(value[1]);
	}
	else {
		ampl = MEDUSAHEAD_AMPLITUDE;
	}

	// hVeloc
	if (properties.find("HVelocity") != properties.end()) {
		auto velocStr = properties.find("HVelocity")->second;
		auto value = splitString(velocStr, ',');

		hVeloc.x = stoi(value[0]);
		hVeloc.y = stoi(value[1]);
	}
	else {
		hVeloc = MEDUSAHEAD_HORIZONTAL_VELOC;
	}

	// freq
	if (properties.find("Frequency") != properties.end()) {
		freq = stof(properties.find("Frequency")->second);
	}
	else {
		freq = MEDUSAHEAD_FREQUENCY;
	}

	auto medusaHead = new MedusaHead(status,direction,pos, hVeloc, ampl, freq);

	medusaHead->init();

	return medusaHead;
}

BaseObject * ObjectFactory::getSpawner(xml_node node) {
	auto properties = getObjectProperties(node);
	if (properties.size() == 0)
		return nullptr;

	GVector2 pos;
	int dir, num, width, height;
	eID type;
	float time;
	bool oneperone;

	pos.x = stoi(properties["X"]) + 32;
	pos.y = stoi(properties["Y"]) - 32;
	width = stoi(properties["Width"]);
	height = stoi(properties["Height"]);

	// type
	if (properties.find("type") != properties.end()) {
		type = (eID)(stoi(properties.find("type")->second));
	}
	else {
		type = eID::MEDUSAHEAD;
	}

	// dir
	if (properties.find("direction") != properties.end()) {
		dir = stoi(properties.find("direction")->second);
	}
	else {
		dir = -1;
	}

	// time
	if (properties.find("time") != properties.end()) {
		time = stof(properties.find("time")->second);
	}
	else {
		time = 1000.0f;
	}

	// num
	if (properties.find("number") != properties.end()) {
		num = stoi(properties.find("number")->second);
	}
	else {
		num = -1;
	}

	// one per one
	if (properties.find("oneperone") != properties.end()) {
		auto v = properties.find("oneperone")->second;
		if (v == "true") {
			oneperone = true;
		}
		else {
			oneperone = false;
		}
	}
	else {
		oneperone = false;
	}

	// max num
	int maxNum = 2;
	if (properties.find("maxNumber") != properties.end()) {
		maxNum = stoi(properties.find("maxNumber")->second);
	}

	auto spawner = new ObjectSpawner(pos, width, height, type, dir, time, num);
	spawner->setOnePerOne(oneperone);
	spawner->setMaxNumber(maxNum);
	spawner->init();

	return spawner;
}

BaseObject * ObjectFactory::getDoor(xml_node node) {
	auto properties = getObjectProperties(node);
	if (properties.size() == 0)
		return nullptr;

	int x, y, direction;
	eStatus status;

	xml_node activebound = node.child("ActiveBound");
	x = stoi(properties["X"]);
	y = stoi(properties["Y"]);
	int width = activebound.attribute("Width").as_int();
	//hack :v
	x += 8;
	width -= 64;
	y -= 48;
	if (properties.find("status") != properties.end()) {
		status = (eStatus)(stoi(properties.find("status")->second));
	}
	else {
		status = eStatus::CLOSING;
	}

	if (properties.find("direction") != properties.end()) {
		direction = stoi(properties.find("direction")->second);
	}
	else {
		direction = 1;
	}

	auto door = new Door(status, GVector2(x, y), direction);
	door->init();

	return door;
}

BaseObject * ObjectFactory::getFlyLand(xml_node node) {
	xml_node activebound = node.child("ActiveBound");

	// 
	int x = activebound.attribute("X").as_int();
	int y = activebound.attribute("Y").as_int();
	int width = activebound.attribute("Width").as_int();

	x += 32;
	width -= 64;
	//y -= 32;

	auto flyLand = new FlyLand(GVector2(x, y), GVector2(x + width, y));
	flyLand->init();
	return flyLand;
}



BaseObject* ObjectFactory::getCandle(xml_node node)
{
	auto properties = ObjectFactory::getObjectProperties(node);
	if (properties.size() == 0)
		return nullptr;

	int x, y, width, height;
	eItemID	_dropItemId;

	x = stoi(properties["X"]);
	y = stoi(properties["Y"]);
	width = stoi(properties["Width"]);
	height = stoi(properties["Height"]);

	GVector2 pos = GVector2(x + width/2,y - height/2);
	_dropItemId = (eItemID)stoi(properties["DropItemId"]);

	auto candle = new Candle(pos,_dropItemId);
	candle->init();
	return candle;
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
