#pragma once
#include"define.h"
#include"Viewport.h"
#include"Simon.h"
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

	virtual void init() = 0;

	void loadStageInfo(const char* path,eID _levelId);

	GVector2 getCurrentViewportBound();

	GVector2 getCurrentViewportStartPosition();

	Viewport* getViewport();

	void setCurrentViewport(eLevel2Viewport name);
	virtual void updateViewport(BaseObject* objTracker) = 0;
protected:
	eID	_currentLevelId;
	eLevel2Viewport _currentViewport;
	map<eLevel2Viewport, ViewportInfo> _listViewportInfo;
	Simon* _simon;
	Viewport* _viewport;
};




