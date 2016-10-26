#ifndef _FRAMEWORK_
#define _FRAMEWORK_

#define _NAMESPACE_FRAMEWORK_BEGIN namespace FrameWork {

#include<d3d9.h>
#include<d3dx9.h>
//Để handle error cho game
#include<exception>

using namespace std;
typedef D3DXVECTOR2 GVector2;
typedef D3DXVECTOR3 GVector3;
#define C_WHITE D3DXCOLOR(1.0f,1.0f,1.0f,1.0f)	//màu trắng
#define COLOR_KEY D3DXCOLOR(1.0f,0.0f,1.0f,1.0f)
#define _NAMESPACE_FRAMEWORK_END	}
#define _USING_FRAMEWORK using namespace FrameWork;


#endif 