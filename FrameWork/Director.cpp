#include "Director.h"


Director::Director()
{
}


Director::~Director()
{
}

eID	Director::_currentLevelId = eID::LEVEL2;
string Director::_currentViewport = "s1";

map<string, ViewportInfo> Director::_listViewportInfo;

GVector2 Director::getCurrentViewportBound()
{
	return GVector2(_listViewportInfo[_currentViewport].bound.left, 
		_listViewportInfo[_currentViewport].bound.right);
}

GVector2 Director::getCurrentStartViewportPosition()
{
	return GVector2(_listViewportInfo[_currentViewport].position.x,
		_listViewportInfo[_currentViewport].position.y + WINDOW_HEIGHT);
}

void Director::setCurrentViewport(const char* name)
{
	if (_currentViewport != name)
		_currentViewport = name; 
}

void Director::loadStageInfo(const char* fileInfoPath, eID _levelId)
{
	FILE* file;
	file = fopen(fileInfoPath, "r");

	if (file)
	{
		while (!feof(file))
		{
			ViewportInfo info;
			int x, y;
			GVector2 origin;
			char name[100];
			fgets(name, 100, file);

			fscanf(file, "%s %d %d %d %d", &name, &x, &y, &info.bound.left, &info.bound.right);
			info.position = GVector2(x, y);
			_listViewportInfo[name] = info;
		}
	}

	fclose(file);
}

