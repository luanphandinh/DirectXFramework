#include "SceneTime.h"

int SceneTime::_sceneTime = 0;

bool SceneTime::_stop = false;

Text* SceneTime::_sceneTimeText = nullptr;

SceneTime::SceneTime()
{
}


SceneTime::~SceneTime()
{
}

void SceneTime::stop(bool stop)
{
	if (_stop != stop)
		_stop = stop;
}

void SceneTime::setTime(int value)
{
	if (_sceneTime != value)
		_sceneTime = value;
}

int SceneTime::getTime()
{
	return _sceneTime;
}

void SceneTime::draw(LPD3DXSPRITE spriteHandler)
{
	if (_sceneTimeText == nullptr)
		_sceneTimeText = new Text(L"Arial", "SCORE-", TIME_POSITION.x, TIME_POSITION.y);
	if (!_stop)
		_sceneTimeText->setText("TIME  " + formatScoreString(4, to_string(_sceneTime - (int)(GameTime::getInstance()->getTotalGameTime() / 1000))));
	else 
		_sceneTimeText->setText("TIME  " + formatScoreString(4, ""));
	_sceneTimeText->draw();

}