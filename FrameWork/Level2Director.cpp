#include "Level2Director.h"


Level2Director::Level2Director()
{
	_reviveViewport = eLevel2Viewport::V1;
	_revivePosition = GVector2(2700, 100);
}


Level2Director::~Level2Director()
{
}

void Level2Director::init()
{
	this->loadStageInfo("Resources//Maps//level2ViewportInfo.txt", eID::LEVEL2);
	_viewport = new Viewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	this->setCurrentViewport(V1);
}

void Level2Director::updateViewport()
{
	GVector2 pos = _objTracker->getPosition();

	Level2Director::switchViewport();
	// Vị trí hiện tại của viewport. 
	GVector2 current_position = _viewport->getPositionWorld();

	//GVector2 worldsize = this->_backGround->getWorldSize();
	GVector2 boundSize = this->getCurrentViewportBound();
	// Bám theo object.
	GVector2 new_position = GVector2(max(_objTracker->getPositionX() - WINDOW_WIDTH / 2, boundSize.x),
		current_position.y);

	// Không cho đi quá vùng viewport hiện tại.
	if (new_position.x + WINDOW_WIDTH > boundSize.y)
	{
		new_position.x = boundSize.y - WINDOW_WIDTH;
	}

	_viewport->setPositionWorld(new_position);
}


void Level2Director::switchViewport()
{
	GVector2 pos = _objTracker->getPosition();
	
	switch (_currentViewport)
	{
	case eLevel2Viewport::V1:
		if (pos.y > 384 && (Simon*)_objTracker->isInStatus(eStatus::STANDINGONSTAIR))
		{
			this->setCurrentViewport(V2);
			_objTracker->setPosition(2848, 450);
		}
		break;
	case eLevel2Viewport::V2:
		if (pos.y < 440 && (Simon*)_objTracker->isInStatus(eStatus::STANDINGONSTAIR))
		{
			this->setCurrentViewport(V1);
			_objTracker->setPosition(2816, 380);
		}
		break;
	default:
		break;
	}
}

