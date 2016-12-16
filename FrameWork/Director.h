#pragma once
#include"define.h"
#include"Viewport.h"
#include"BaseObject.h"
#include"LifeCounter.h"
#include"ScenarioManager.h"
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

	//Cập nhật lại kịch bản
	virtual void update(float deltaTime) = 0;
	virtual void updateViewport() = 0;
	virtual void updateScenario(float deltaTime) = 0;
	virtual void release() = 0;

	
	void setCurrentViewport(eLevel2Viewport name);
	void setObjectTracker(BaseObject* objTracker);

	void setRevivePosition(GVector2 pos);
	GVector2 getRevivePosition();
	void setReviveViewport(eLevel2Viewport viewport);
	virtual void updateRevive();


	/*
	end level
	*/
	virtual void generateCrystalBall() = 0;
	virtual void endLevel() = 0;

protected:
	eID	_currentLevelId;
	eLevel2Viewport _currentViewport;
	map<eLevel2Viewport, ViewportInfo> _listViewportInfo;
	BaseObject* _objTracker;
	Viewport* _viewport;

	//Sau khi simon chết thì cần có điểm được revive
	//Cập nhật lại vị trí được revive với viewport revive
	GVector2	_revivePosition;
	eLevel2Viewport _reviveViewport;

	//ScenarioManager dùng để cập nhật kịch bản cho game,ví dụ như qua của,gặp boss
	ScenarioManager* _scenarioManager;
	StopWatch* _generateCrysballStopWatch;
};




