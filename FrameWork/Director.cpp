#include "Director.h"


Director::Director()
{
	
}


Director::~Director()
{
}

//eID	Director::_currentLevelId = eID::LEVEL2;
//string Director::_currentViewport = "s1";
//
//map<string, ViewportInfo> Director::_listViewportInfo;

GVector2 Director::getCurrentViewportBound()
{
	return GVector2(_listViewportInfo[_currentViewport].bound.left, 
		_listViewportInfo[_currentViewport].bound.right);
}

GVector2 Director::getCurrentViewportStartPosition()
{
	return GVector2(_listViewportInfo[_currentViewport].position.x,
		_listViewportInfo[_currentViewport].position.y + WINDOW_HEIGHT);
}

Viewport* Director::getViewport()
{
	return _viewport;
}

void Director::setCurrentViewport(eLevel2Viewport name)
{
	if (_currentViewport != name)
		_currentViewport = name; 

	GVector2 pos = this->getCurrentViewportStartPosition();
	_viewport->setPositionWorld(GVector2(pos.x, pos.y));
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
			char name[100];
			eLevel2Viewport _viewport;
			fgets(name, 100, file);

			fscanf(file, "%s %d %d %d %d", &name, &x, &y, &info.bound.left, &info.bound.right);
			info.position = GVector2(x, y);
			_viewport = (eLevel2Viewport)atoi(name);
			_listViewportInfo[_viewport] = info;
		}
	}

	fclose(file);
}

GVector2  Director::getRevivePosition()
{
	return _revivePosition;
}

void Director::setReviveViewport(eLevel2Viewport viewport)
{
	if (_reviveViewport != viewport)
		_reviveViewport = viewport;
}

void Director::updateRevive()
{
	if (_reviveViewport != eLevel2Viewport::VNULL)
		_currentViewport = _reviveViewport;
	_objTracker->setPosition(_revivePosition);
	this->setCurrentViewport(_currentViewport);
}

void Director::setObjectTracker(BaseObject* objTracker)
{
	if (_objTracker != objTracker)
		this->_objTracker = objTracker;
}

void Director::setRevivePosition(GVector2 pos)
{
	if (_revivePosition != pos)
		_revivePosition = pos;
}