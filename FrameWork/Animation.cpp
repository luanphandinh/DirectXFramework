#include "Animation.h"
_USING_FRAMEWORK

Animation::~Animation()
{
}

Animation::Animation(Sprite* spriteSheet, float timeAnimate, bool loop)
{
	_spriteSheet = spriteSheet;
	_timeAnimate = timeAnimate;
	
	_canAnimate = true;
	_totalFrames = 0;
	_index = 0;
	_timer = 0;
	_valueFlash = 0.5f;

	_startFrame = 0;
	_endFrame = _totalFrames - 1;

	this->setIndex(0);
	this->setLoop(loop);
	_canFlash = false;
	_useDefaultOrigin = false;
	_flashColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
}

Animation::Animation(Sprite* spriteSheet, int totalFrames, int cols, float timeAnimate)
{
	_spriteSheet = spriteSheet;
	_timeAnimate = timeAnimate;

	_canAnimate = true;
	_totalFrames = totalFrames;
	_index = 0;
	_timer = 0;
	_valueFlash = 0.5f;

	_startFrame = 0;
	_endFrame = _totalFrames - 1;

	int frameW = spriteSheet->getTextureWidth() / cols;
	int frameH = spriteSheet->getTextureHeight() * cols / totalFrames;

	for (int i = 0; i < totalFrames; i++)
	{
		int x = i % cols;
		int y = i / cols;

		this->addFrameRect(x * frameW, y * frameH, frameW, frameH);
	}

	_currentRect = _frameRectList[_index];
	_useDefaultOrigin = false;
	_flashColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
}

void Animation::nextFrame()
{
	this->setIndex(_index + 1);
}

void Animation::setIndex(int index)
{
	if (this->_index == index || _totalFrames == 0)
		return;

	this->_index = index;

	if (_index > _endFrame)
		_index = _startFrame;

	_currentRect = _frameRectList[_index];

	if (!_useDefaultOrigin)
		_currentOrigin = _frameOriginList[_index];
	else _currentOrigin = DEFAULT_ORIGIN;

	if (!_isLoop && _index == _endFrame)
		this->stop();
}

int Animation::getIndex()
{
	return this->_index;
}

void Animation::update(float dt)
{
	if (!_canAnimate && !_canFlash)
		return;

	_timer += dt / 1000;
	if (_timer >= _timeAnimate)
	{
		if (_canAnimate)
			this->nextFrame();

		_timer -= _timeAnimate;
		
		if (_canFlash)
		{
			if (_spriteSheet->getOpacity() != _valueFlash)
			{
				_spriteSheet->setOpacity(_valueFlash);
				_spriteSheet->setColor(D3DXCOLOR(_flashColor.r, _flashColor.g, _flashColor.b, _valueFlash));
			}
			else
			{
				_spriteSheet->setOpacity(1.0f);
				_spriteSheet->setColor(D3DXCOLOR(_flashColor.r, _flashColor.g, _flashColor.b, 1.0f));
			}
		}
	}
}

void Animation::setUseDefaultOrigin(bool use)
{
	if (_useDefaultOrigin != use)
		_useDefaultOrigin = use;
}

void Animation::draw(LPD3DXSPRITE spriteHandle, Viewport* viewport)
{
	/*
		Khi draw thì setFrameRect cho _spriteSheet
		Từ đó sprite sẽ cập nhập lại bounding để xét va chạm
		Rồi dùng sprite để vẽ
	*/
	_spriteSheet->setFrameRect(_currentRect);
	if (!_useDefaultOrigin)
		_spriteSheet->setOrigin(_currentOrigin);
	_spriteSheet->render(spriteHandle, viewport);
}

void Animation::setTimeAnimate(float time)
{
	_timeAnimate = time;
}

float Animation::getTimeAnimate()
{
	return _timeAnimate;
}

void Animation::start()
{
	if (!_canAnimate)
		_canAnimate = true;
}

void Animation::stop()
{
	if (_canAnimate)
		_canAnimate = false;
}

void Animation::canAnimate(bool can)
{
	if (_canAnimate != can)
		_canAnimate = can;
}
bool Animation::isAnimate()
{
	return _canAnimate;
}

bool Animation::isLastAnimation()
{
	return _canAnimate;
}

void Animation::addFrameRect(RECT rect)
{
	if (_frameRectList.empty())
	{
		_currentRect = rect;
	}

	_frameRectList.push_back(rect);
	_totalFrames = _frameRectList.size();
	_endFrame = _totalFrames - 1;
}

void Animation::addOriginFrame(GVector2 origin)
{
	if (_frameOriginList.empty())
	{
		_currentOrigin = origin;
	}

	_frameOriginList.push_back(origin);
}

void Animation::addFrameRect(float left, float top, int width, int height)
{
	RECT rect;
	rect.top = top;
	rect.left = left;
	rect.right = left + width;
	rect.bottom = top + height;
	this->addFrameRect(rect);
}

void Animation::addFrameRect(float left, float top, float right, float bottom)
{
	RECT rect;
	rect.top = top;
	rect.left = left;
	rect.right = right;
	rect.bottom = bottom;
	this->addFrameRect(rect);
}

void Animation::addFrameRect(eID id, char* firstRectName, ...)
{
	va_list vl;

	char* name;

	va_start(vl, firstRectName);

	name = firstRectName;

	while (name != NULL)
	{
		this->addFrameRect(SpriteManager::getInstance()->getSourceRect(id, name));
		this->addOriginFrame(SpriteManager::getInstance()->getSourceOrigin(id, name));
		name = va_arg(vl, char*);
	}

	va_end(vl);
}

void Animation::setLoop(bool isLoop)
{
	if (isLoop != _isLoop)
		_isLoop = isLoop;
}

bool Animation::isLoop()
{
	return _isLoop;
}

void Animation::restart(int from)
{
	setIndex(from);

	if (!_canAnimate)
		_canAnimate = true;
}

void Animation::enableFlashes(bool enable)
{
	if (_canFlash == enable)
		return;
	_canFlash = enable;
	_spriteSheet->setColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
}

void Animation::setValueFlashes(float value)
{
	if (_valueFlash != value)
		_valueFlash = value;
}

void Animation::animateFromTo(int from, int to, bool loop)
{
	if (from <= to)
	{
		_startFrame = from;
		_endFrame = to;
	}
	else
	{
		_startFrame = to;
		_endFrame = from;
	}

	this->setIndex(from);
	_isLoop = loop;
	_timer = 0.0f;

	if (!_canAnimate)
		_canAnimate = true;
}

void Animation::setColorFlash(D3DXCOLOR color)
{
	if (_flashColor != color)
		_flashColor = color;
}
D3DXCOLOR Animation::getColorFlash()
{
	return _flashColor;
}

