#pragma once
#include"define.h"
/*
	Class đùng để gán phần viewport cho từng đoạn chơi trong stage
	Được đọc từ file trong Resources//Maps//level?viewportinfo.txt
*/
struct AreaBound
{
	int		left;
	int		right;
};

struct ViewportInfo
{
	GVector2 position;
	AreaBound bound;
};



class Director
{
public:
	Director();
	~Director();
	static void loadStageInfo(const char* path,eID _levelId);
	/*
		trả về Gvector2(RECT.right,RECT.bottom)
	*/
	static GVector2 getCurrentViewportBound();

	static GVector2	getCurrentStartViewportPosition();

	static void setCurrentViewport(const char* name);
private:
	static eID	_currentLevelId;
	static string _currentViewport;
	static map<string, ViewportInfo> _listViewportInfo;
};




