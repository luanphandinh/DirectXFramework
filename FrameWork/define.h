#ifndef _FRAMEWORK_
#define _FRAMEWORK_

#define _NAMESPACE_FRAMEWORK_BEGIN namespace FrameWork {

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

using namespace std;

#define WINDOW_WIDTH 512
#define WINDOW_HEIGHT 448

typedef D3DXVECTOR2 GVector2;
typedef D3DXVECTOR3 GVector3;
#define C_WHITE D3DXCOLOR(1.0f,1.0f,1.0f,1.0f)	//màu trắng
#define COLOR_KEY D3DXCOLOR(1.0f,0.0f,1.0f,1.0f)

#define _NAMESPACE_FRAMEWORK_END	}


#define _USING_FRAMEWORK using namespace FrameWork;


#endif 