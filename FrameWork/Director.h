#pragma once
#include"define.h"
/*
	Class đùng để gán phần viewport cho từng đoạn chơi trong stage
	Được đọc từ file trong Resources//Maps//level?viewportinfo.txt
	map<string,RECT> string là tên đoạn
					RECT.left là pos.x của viewport(x trong file được đọc lên)
					RECT.top là pos.y của viewport
					Khi chuyển scene thì 2 cái này là pos của viewport trong world

					RECT.right biên bên trái của viewport(là size.x của vùng đi được trong viewport
					hiện tại cho đến khi qua cửa hoặc lên cầu thang đến đoạn khác)
					RECT.bottm là biên phải của viewport
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




