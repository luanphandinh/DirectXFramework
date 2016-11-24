#include "Director.h"


Director::Director()
{
}


Director::~Director()
{
}

eID	Director::_currentLevelId = eID::LEVEL2;
string Director::_currentViewport = "s1";
map<string, RECT> Director::_listViewportInfo;

GVector2 Director::getCurrentViewportSize()
{
	return GVector2(_listViewportInfo[_currentViewport].right, 
		_listViewportInfo[_currentViewport].bottom);
}

GVector2 Director::getCurrentStartViewportPosition()
{
	return GVector2(_listViewportInfo[_currentViewport].left,
		_listViewportInfo[_currentViewport].top + WINDOW_HEIGHT);
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
			RECT rect;
			GVector2 origin;
			char name[100];
			fgets(name, 100, file);

			fscanf(file, "%s %d %d %d %d", &name, &rect.left, &rect.top, &rect.right, &rect.bottom);

			_listViewportInfo[name] = rect;
		}
	}

	fclose(file);
}

