#include "LifeUI.h"


LifeUI::LifeUI(GVector2 position, int number,int HP) :EmptyObject(eID::LIFE_ICON,position,0,0)
{
	_hp = HP;
	_life = number;
	for (int i = 0; i < _hp; i++)
	{
		auto sprite = SpriteManager::getInstance()->getSprite(eID::LIFE_ICON);
		sprite->setFrameRect(SpriteManager::getInstance()->getSourceRect(eID::LIFE_ICON, "red_life_icon"));
		sprite->setScale(SCALE_FACTOR);
		_listIcons.push_back(sprite);
		_listIcons.back()->setPosition(position.x + (_listIcons.back()->getFrameWidth() + GAP) * i, position.y);
	}
}


LifeUI::~LifeUI()
{
}


void LifeUI::init()
{

}

void LifeUI::update(float deltatime)
{
	
}

void LifeUI::draw(LPD3DXSPRITE spriteHandler, Viewport* viewport)
{
	for (auto icon : _listIcons)
		icon->render(spriteHandler);
}

void LifeUI::release()
{
	while (!_listIcons.empty())
	{
		SAFE_DELETE(_listIcons.back());
		_listIcons.pop_back();
	}
}

void LifeUI::setHPNumber(int number)
{
	if (_listIcons.size() == number || number < 0)
		return;

	//if (_listIcons.size() < HP)
	//{

	//}
}

int LifeUI::getHPNumber()
{
	return _hp;
}