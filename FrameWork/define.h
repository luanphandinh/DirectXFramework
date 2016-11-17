#ifndef _FRAMEWORK_
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

enum eID
{
	SIMON = 0, // Main character.
	//Land and stairs with direction
	LAND = 11,
	STAIR = 12,
	STAIR_LEFTBOTTOM_RIGHTTOP = 13,
	STAIR_RIGHTBOTTOM_LEFTTOP = 14,
	//Throwable weapon
	CROSS = 21,
	SWORD = 22,
	AXE = 23,
	//Enemies
	SPEARKNIGHT = 35,
	BAT = 36,
	MEDUSAHEAD = 37,

	//stage
	MAPSTAGE1 = 40,

	//icon in gamestatusboard
	LIFE_ICON = 51,
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

	DYING= (1 << 7),
	WALKING=(1<<8),
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
	LEFTBOTTOM_TO_RIGHTTOP = 0,
	RIGHTBOTTOM_TO_LEFTTOP = 1,
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