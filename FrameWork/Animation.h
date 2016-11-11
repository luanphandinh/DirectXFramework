#pragma once
/*
	Là class dùng để tạo animation(chuyển động) cho object
	+ Lớp bao gồm một Sprite Sheet(SS)
	+ Danh sách các rect để vẽ từ SS lên màn hình
	+ Một animation sẽ gồm các frames được đánh theo index hoặc _currentRect
		từ _startFrame tới _endFrame
	+ Thời gian animate cho 1 frame
*/
#include"define.h"
#include"Sprite.h"
#include"Viewport.h"
#include <stdarg.h>     /* va_list, va_start, va_arg, va_end */
#include"SpriteManager.h"
_NAMESPACE_FRAMEWORK_BEGIN
class Animation
{
public:
	~Animation();
	/*
	Tạo animation với sprite sheet
		@spriteSheet: spritesheet của chuyển động
		@timeAnimate: thời gian chuyển từng frame
	Sau khi tạo bằng contructor này phải addFrameRect nó mới có frame để chuyển động.
	*/
	Animation(Sprite* spriteSheet, float timeAnimate = 0.0f, bool loop = true);
	/*
	Tạo aniation với spritesheet có frame đều nhau
		@spriteSheet: spritesheet của chuyển động
		@totalFrames: tổng số frame
		@cols: số cột
		@timeAnimate: thời gian chuyển từng frame
	*/
	Animation(Sprite* spriteSheet, int totalFrames, int cols, float timeAnimate = 0.0f);
	/*Chuyển qua frame kế*/
	void nextFrame();
	/*Truyền thứ tự cho 1 fram cụ thể*/
	void setIndex(int index);
	/*Lấy giá trị index hiện tại*/
	int getIndex();
	/*Update animation*/
	void update(float dt);
	/*Vẽ chuyển động*/
	void draw(LPD3DXSPRITE spriteHandle, Viewport* viewport);
	/*Đặt thời gian chuyển fram,@time tính theo s*/
	void setTimeAnimate(float time);
	/*Lấy giá trị thời gian chuyển frame*/
	float getTimeAnimate();
	/*Bắt đầu chuyển frame*/
	void start();
	/*Dừng chuyển frame*/
	void stop();

	void canAnimate(bool can);
	bool isAnimate();
	void addFrameRect(RECT rect);
	void addFrameRect(float top, float left, int width, int height);
	void addFrameRect(float top, float left, float right, float bottom);
	void setLoop(bool isLoop);
	bool isLoop();

	/*
	Tạo nhiều framerect liên tiếp nhau, không giới hạn tham số
		@id: ID của sprite
		@firstRectName,...: các tên của frame rect
		Sau khi truyền xong các tên, kết thúc PHẢI bằng NULL để xác định được kết thúc.
	*/
	void addFrameRect(eID id, char* firstRectName, ...);

	/*
		Chạy lại animate
		@from :index frame muốn bắt đầu
	*/
	void restart(int from = 0);
	/*
		Tạo hiệu ứng chớp tắt cho animation
		sau khi set enable = true, phải đặt giá trị thay đổi ở hàm setValueFlashes
	*/
	void enableFlashes(bool enable);
	/*
		Gán giá trị chớp tắt
		@value: giá trị thay đổi để chớp, từ 0 đến < 1 (giá trị bình thường là 1)
	*/
	void setValueFlashes(float value);
	/*
	Animate từ frame này đến frame khác cụ thể
		@from: frame bắt đầu
		@to: frame kết thúc
		@loop: có lặp hay ko
		Giá trị from/to là thứ tự của frame add vô, tính từ 0 (from nên to).
	*/
	void animateFromTo(int from, int to, bool loop = true);

	void setColorFlash(D3DXCOLOR color);
	D3DXCOLOR getColorFlash();

	bool isLastAnimation();
private:
	bool			_isAll;
	int				_startFrame;
	int				_endFrame;
	int				_index;
	int				_totalFrames;

	float			_timeAnimate;
	float			_timer;

	bool			_canAnimate;
	bool			_isLoop;

	Sprite*			_spriteSheet;
	vector<RECT>	_frameRectList;
	vector<GVector2> _frameOriginList;
	string			_nameAnimation;
	RECT			_currentRect;

	bool			_canFlash;
	float			_valueFlash;
	D3DXCOLOR		_flashColor;
};

_NAMESPACE_FRAMEWORK_END
