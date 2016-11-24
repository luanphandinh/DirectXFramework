#pragma once
#include"define.h"

class Director
{
public:
	Director();
	~Director();
	static void loadStageInfo(const char* path,eID _levelId);
	/*
		trả về Gvector2(RECT.right,RECT.bottom)
	*/
	static GVector2 getCurrentViewportSize();

	static GVector2	getCurrentStartViewportPosition();

	static void setCurrentViewport(const char* name);
private:
	static eID	_currentLevelId;
	static string _currentViewport;
	static map<string, RECT> _listViewportInfo;
};



