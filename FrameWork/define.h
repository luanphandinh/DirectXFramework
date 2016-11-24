﻿#ifndef _FRAMEWORK_
#define _FRAMEWORK_

#define _NAMESPACE_FRAMEWORK_BEGIN namespace FrameWork {
#define _USE_MATH_DEFINES
#include<d3d9.h>
#include<d3dx9.h>
#include<dinput.h>
#include<dsound.h>

//Để handle error cho game
#include<exception>

#include<math.h>
#include <algorithm>    // std::find
#include<vector>
#include<map>
#include <stdio.h>
#include"utils.h"
#include <time.h>       /* time */
#include<stdlib.h>/* for random srand, rand */
using namespace std;

#define WINDOW_WIDTH 512
#define WINDOW_HEIGHT 448

typedef D3DXVECTOR2 GVector2;
typedef D3DXVECTOR3 GVector3;
#define C_WHITE D3DXCOLOR(1.0f,1.0f,1.0f,1.0f)	//màu trắng
#define COLOR_KEY D3DXCOLOR(1.0f,0.0f,1.0f,1.0f)
#define GVector2Zero D3DXVECTOR2(0,0)
#define DEFAULT_ORIGIN   D3DXVECTOR2(0.5f,0.5f)
#define SCALE_FACTOR 2.0f

/*
	17/11 L : t sửa lại cái id nha,land gì đồ vật thể thì để có số 1 ở trước,
	chừng m thêm cái chông và thì để 15 cửa 16 gì đó
	từ vũ khí thì có số 2,tương tự tăng số đầu lên để chừng sữa cho dễ
	*/

enum eID
{
	SIMON = 0, // Main character.
	//Land and stairs with direction
	LAND = 2,
	STAIR = 3,
	DOOR = 4,
	STAIR_LEFTBOTTOM_RIGHTTOP = 13,
	STAIR_RIGHTBOTTOM_LEFTTOP = 14,
	//Throwable weapon
	ITEM = 20,
	WEAPON = 21,
	SPECIALITEM = 22,
	//Enemies
	SPEARKNIGHT = 35,
	BAT = 36,
	MEDUSAHEAD = 37,

	//stage
	MAPSTAGE1 = 40,
	LEVEL2 = 41,

	//icon in gamestatusboard
	LIFE_ICON = 51,
	BLANK = 52,
};

enum eItemID
{
	NOITEM = 0,
	SWORD = 1,
	AXE = 2,
	BOORMERANG = 3,
	HOLYWATER = 4,
	STOPWATCH = 5,
};

//status cho nhân vật
enum eStatus
{
	NORMAL = 0,					// 00000 = 0	
	MOVING_LEFT = (1 << 0),		// 00001 = 2^0
	MOVING_RIGHT = (1 << 1),
	RUNNING = (1 << 2),
	JUMPING = (1 << 3),
	HITTING = (1 << 4),
	SITTING = (1 << 5),
	FALLING = (1 << 6),

	DYING = (1 << 7),
	WALKING = (1 << 8),
	// Trạng thái huỷ, lúc này, đối tượng không update, không draw, 
	// ở scene kiểm tra nếu phát hiện trạng thái này thì huỷ đối tượng.
	DESTROY = (1 << 9),
	WAITING = (1 << 10),

	UPSTAIR = (1 << 11),
	DOWNSTAIR = (1 << 12),
	STANDINGONSTAIR = (1 << 13),
	STANDINGONSTAIR_UP = (1 << 14),
	STANDINGONSTAIR_DOWN = (1 << 15),
	THROWING_ITEM = (1 << 16),
	LEVEL1 = (1 << 18),

	FLYING = (1 << 19),
	HANGING = (1 << 20),
	HIDING = (1 << 21)
};

enum eStairDirection
{
	LEFTBOTTOM_TO_RIGHTTOP = 1,
	RIGHTBOTTOM_TO_LEFTTOP = 2,
};

enum eItemType
{
	PICKED_UP = 1,
	DROP = 2,
};

enum eHeartItemType
{
	SMALL = 1,
	LARGE = 2,
};

enum eMoneyBagItemType
{
	RED = 1,
	PURPLE = 2,
	WHITE = 3,
};

enum eDirection
{
	NONE	=		0,
	TOP		=		1,
	BOTTOM	=		2,
	LEFT	=		4,
	RIGHT	=		8,
	ALL		= (TOP | BOTTOM | LEFT | RIGHT),
};

enum eLevel2Viewport
{
	V1 = 1,
	V2 = 2,
};

#define _NAMESPACE_FRAMEWORK_END	}


#define _USING_FRAMEWORK using namespace FrameWork;


#ifndef SAFE_DELETE

#define SAFE_DELETE(p) \
if(p) \
{\
	delete (p); \
	p = nullptr; \
} \

#endif // !SAFE_DELETE

#endif 