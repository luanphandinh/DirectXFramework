#pragma once
#include"pugixml-1.7\src\pugixml.hpp"
#include"BaseObject.h"
#include"define.h"
#include"Land.h"
#include"Stair.h"
#include"Bat.h"
#include"MedusaHead.h"
#include"ObjectSpawner.h"
#include"SpearKnight.h"
#include"Door.h"
#include"FlyLand.h"
#include"Candle.h"
#include"Brick.h"
class ObjectFactory
{
public:
	ObjectFactory();
	~ObjectFactory();
	static map<string, BaseObject*>* getMapObjectFromFile(const string path);
	static vector<BaseObject*>* getListObjectFromFile(const string path);
private:
	static BaseObject* getObjectById(xml_node node, eID id);
	static BaseObject* getLand(xml_node node);
	static BaseObject* getStair(xml_node node);

	static BaseObject* getBat(xml_node node);
	static BaseObject* getSpearKnight(xml_node node);
	static BaseObject* getMedusaHead(xml_node node);
	static BaseObject* getSpawner(xml_node node);

	static BaseObject* getDoor(xml_node node);
	static BaseObject* getFlyLand(xml_node node);
	static BaseObject* getCandle(xml_node node);
	static BaseObject* getBrick(xml_node node);

	static map<string, string> getObjectProperties(xml_node node);
};

