#include "LifeCounter.h"

int LifeCounter::_life = 3;
Text* LifeCounter::_lifeText = nullptr;

LifeCounter::LifeCounter()
{
}


LifeCounter::~LifeCounter()
{
}

void LifeCounter::plusLife(int value)
{
	_life += value;
}

int LifeCounter::getLife()
{
	return _life;
}

void LifeCounter::setLife(int value)
{
	if (_life != value)
		_life = value;
}

void LifeCounter::draw(LPD3DXSPRITE spriteHandler)
{
	if (_lifeText == nullptr)
		_lifeText = new Text(L"Arial", "P-", LIFE_POSITION.x, LIFE_POSITION.y);
	_lifeText->setText("P -" + formatScoreString(2, to_string(LifeCounter::getLife())));
	_lifeText->draw();
}

