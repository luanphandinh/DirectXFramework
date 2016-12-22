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
	case BRICK:
		return getBrick(node);
		break;
		case SPIKE:
			return getSpike(node);
			break;
	case DRAGON:
		return getDragon(node);
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
	case GHOST:
		return getGhost(node);
		break;
	case MEDUSA:
		return getMedusa(node);
		break;
	case FLEAMAN:
		return getFleaman(node);
		break;
	case RAVEN:
		return getRaven(node);
		break;
	case SKELETON:
		return getSkeleton(node);
		break;
	case MUMMYMAN:
		return getMummyMan(node);
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
	bool canJumpDown,checkWithStair;

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
	if (properties.find("checkWithStair") != properties.end()) {
		checkWithStair = (bool)(stoi(properties.find("checkWithStair")->second));
	}
	else {
		checkWithStair = false;
	}
	auto land = new Land(x, y, width, height, checkWithStair, direction);
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

	x = stoi(properties["X"])+8;
	y = stoi(properties["Y"])-16;

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

	x = stoi(properties["X"])+32;
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

	pos.x = stoi(properties["X"])+32;
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
	pos.y = stoi(properties["Y"]) ;
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
		dir = 0;
	}

	// time
	if (properties.find("time") != properties.end()) {
		time = stof(properties.find("time")->second);
	}
	else {
		time = 3000.0f;
	}

	// num
	if (properties.find("number") != properties.end()) {
		num = stoi(properties.find("number")->second);
	}
	else {
		num = 5;
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

	//auto spawner = new ObjectSpawner(pos, width, height, type, dir, time, num);
	auto spawner = new ObjectSpawner(pos, width, height, type, dir, time, -1);
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
	/*x += 8;
	width -= 64;
	y -= 48;*/
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

BaseObject * ObjectFactory::getSpike(xml_node node) {
	auto properties = getObjectProperties(node);
	if (properties.size() == 0)
		return nullptr;

	int x, y;
	//eStatus status;
	eSpikeState state;

	xml_node activebound = node.child("ActiveBound");
	x = stoi(properties["X"])+32;
	y = stoi(properties["Y"]);
	//hack :v
	/*x += 8;
	width -= 64;
	y -= 48;*/
	if (properties.find("state") != properties.end()) {
		state = (eSpikeState)(stoi(properties.find("state")->second));
	}
	else {
		state = eSpikeState::SPIKE_FALLING_01;
	}

	auto spike = new Spike(state, GVector2(x, y));
	spike->init();

	return spike;
}

BaseObject * ObjectFactory::getGhost(xml_node node) {
	auto properties = getObjectProperties(node);
	if (properties.size() == 0)
		return nullptr;

	int x, y, direction;
	eStatus status;

	x = stoi(properties["X"]) + 8;
	y = stoi(properties["Y"]);

	if (properties.find("status") != properties.end()) {
		status = (eStatus)(stoi(properties.find("status")->second));
	}
	else {
		status = eStatus::HIDING;
	}

	if (properties.find("direction") != properties.end()) {
		direction = stoi(properties.find("direction")->second);
	}
	else {
		direction = 1;
	}

	auto ghost = new Ghost(status, GVector2(x, y), direction);
	ghost->init();

	return ghost;
}

BaseObject * ObjectFactory::getMedusa(xml_node node) {
	auto properties = getObjectProperties(node);
	if (properties.size() == 0)
		return nullptr;
	int x, y, width, height;

	eStatus status;

	x = stoi(properties["X"]);
	y = stoi(properties["Y"]);
	width = stoi(properties["Width"]);
	height = stoi(properties["Height"]);

	auto pos = GVector2(x + width/2, y - height/2);

	auto medusa = new Medusa(pos);
	medusa->init();

	return medusa;
}

BaseObject * ObjectFactory::getMummyMan(xml_node node) {
	auto properties = getObjectProperties(node);
	if (properties.size() == 0)
		return nullptr;
	int x, y, width, height;

	eStatus status;

	x = stoi(properties["X"]);
	y = stoi(properties["Y"]);
	width = stoi(properties["Width"]);
	height = stoi(properties["Height"]);

	auto pos = GVector2(x + width / 2, y - height / 2);

	auto mummyMan = new MummyMan(pos,eDirection::LEFT);
	mummyMan->init();

	return mummyMan;
}


BaseObject * ObjectFactory::getFleaman(xml_node node) {
	auto properties = getObjectProperties(node);
	if (properties.size() == 0)
		return nullptr;

	int x, y, direction;
	eStatus status;

	x = stoi(properties["X"]) + 32;
	y = stoi(properties["Y"])-16;

	if (properties.find("status") != properties.end()) {
		status = (eStatus)(stoi(properties.find("status")->second));
	}
	else {
		status = eStatus::SITTING;
	}

	if (properties.find("direction") != properties.end()) {
		direction = stoi(properties.find("direction")->second);
	}
	else {
		direction = 1;
	}

	auto fleaman = new Fleaman(status, GVector2(x, y), direction);
	fleaman->init();

	return fleaman;
}

BaseObject * ObjectFactory::getSkeleton(xml_node node) {
	auto properties = getObjectProperties(node);
	if (properties.size() == 0)
		return nullptr;

	int x, y, direction;
	eStatus status;

	x = stoi(properties["X"]) + 32;
	y = stoi(properties["Y"]) - 16;

	if (properties.find("status") != properties.end()) {
		status = (eStatus)(stoi(properties.find("status")->second));
	}
	else {
		status = eStatus::NORMAL;
	}

	if (properties.find("direction") != properties.end()) {
		direction = stoi(properties.find("direction")->second);
	}
	else {
		direction = 1;
	}

	auto skeleton = new Skeleton(status, GVector2(x, y), direction);
	skeleton->init();

	return skeleton;
}

BaseObject * ObjectFactory::getRaven(xml_node node) {
	auto properties = getObjectProperties(node);
	if (properties.size() == 0)
		return nullptr;

	int x, y, direction;
	eStatus status;

	x = stoi(properties["X"]) + 16;
	y = stoi(properties["Y"]) - 16;

	if (properties.find("status") != properties.end()) {
		status = (eStatus)(stoi(properties.find("status")->second));
	}
	else {
		status = eStatus::LANDING;
	}

	if (properties.find("direction") != properties.end()) {
		direction = stoi(properties.find("direction")->second);
	}
	else {
		direction = 1;
	}

	auto raven = new Raven(status, GVector2(x, y), direction);
	raven->init();

	return raven;
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

BaseObject* ObjectFactory::getBrick(xml_node node)
{
	auto properties = ObjectFactory::getObjectProperties(node);
	if (properties.size() == 0)
		return nullptr;

	int x, y;

	x = stoi(properties["X"]);
	y = stoi(properties["Y"]);
	eItemID _dropItemId;
	GVector2 pos = GVector2(x , y );
	if (properties.find("DropItemId") != properties.end()) {
		_dropItemId = (eItemID)stoi(properties["DropItemId"]);
	}
	else {
		_dropItemId = eItemID::NOITEM;
	}
	
	auto brick = new Brick(pos,_dropItemId);
	brick->init();
	return brick;
}

BaseObject* ObjectFactory::getDragon(xml_node node)
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

	GVector2 pos = GVector2(x + width / 2, y - height / 2);
	

	auto dragon = new Dragon(pos);
	dragon->init();
	return dragon;
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


