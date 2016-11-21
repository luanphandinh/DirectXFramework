#pragma once
#include"pugixml-1.7\src\pugixml.hpp"
#include"BaseObject.h"
#include"define.h"
#include"Land.h"
#include"Stair.h"
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

	static map<string, string> getObjectProperties(xml_node node);
};

