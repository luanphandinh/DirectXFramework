#include "SceneTime.h"

int SceneTime::_sceneTime = 0;

Text* SceneTime::_sceneTimeText = nullptr;

SceneTime::SceneTime()
{
}


SceneTime::~SceneTime()
{
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
		_sceneTimeText = new Text(L"Arial", "SCORE-", TIME_POSITION.x, TIME_POSITION.y, 21);

	_sceneTimeText->setText("TIME  " + formatScoreString(4, to_string(_sceneTime - (int)(GameTime::getInstance()->getTotalGameTime() / 1000))));
	_sceneTimeText->draw();

}