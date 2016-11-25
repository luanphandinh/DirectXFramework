#include "Level2Director.h"


Level2Director::Level2Director()
{
}


Level2Director::~Level2Director()
{
}

void Level2Director::init()
{
	this->loadStageInfo("Resources//Maps//level2ViewportInfo.txt", eID::LEVEL2);
	this->setCurrentViewport(V1);
	GVector2 pos = this->getCurrentViewportStartPosition();
	_viewport = new Viewport(pos.x, pos.y, WINDOW_WIDTH, WINDOW_HEIGHT);
}

void Level2Director::updateViewport(BaseObject* objTracker)
{
	GVector2 pos = objTracker->getPosition();

	Level2Director::switchViewport(objTracker);
	// Vị trí hiện tại của viewport. 
	GVector2 current_position = _viewport->getPositionWorld();

	//GVector2 worldsize = this->_backGround->getWorldSize();
	GVector2 boundSize = this->getCurrentViewportBound();
	// Bám theo object.
	GVector2 new_position = GVector2(max(objTracker->getPositionX() - WINDOW_WIDTH / 2, boundSize.x),
		current_position.y);

	//test
	/*GVector2 new_position = GVector2(max(objTracker->getPositionX() - WINDOW_WIDTH / 2,WINDOW_HEIGHT*2),
		current_position.y);*/

	// Không cho đi quá map.
	if (new_position.x + WINDOW_WIDTH > boundSize.y)
	{
		new_position.x = boundSize.y - WINDOW_WIDTH;
	}

	_viewport->setPositionWorld(new_position);
}


void Level2Director::switchViewport(BaseObject* objTracker)
{
	GVector2 pos = objTracker->getPosition();
	
	switch (_currentViewport)
	{
	case eLevel2Viewport::V1:
		if (pos.y > 384)
		{
			this->setCurrentViewport(V2);
			GVector2 pos = this->getCurrentViewportStartPosition();
			_viewport->setPositionWorld(GVector2(pos.x, pos.y));
			objTracker->setPosition(2848, 450);
		}
		break;
	case eLevel2Viewport::V2:
		if (pos.y < 440)
		{
			this->setCurrentViewport(V1);
			GVector2 pos = this->getCurrentViewportStartPosition();
			_viewport->setPositionWorld(GVector2(pos.x, pos.y));
			objTracker->setPosition(2816, 380);
		}
		break;
	default:
		break;
	}

	
}