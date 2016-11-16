#include "LifeUI.h"


LifeUI::LifeUI(GVector2 position,string text, int number,int HP) :EmptyObject(eID::LIFE_ICON,position,0,0)
{
	_hp = HP;
	_life = number;
	for (int i = 0; i < MAX_HP_NUMBER; i++)
	{
		auto sprite = SpriteManager::getInstance()->getSprite(eID::LIFE_ICON);
		sprite->setFrameRect(SpriteManager::getInstance()->getSourceRect(eID::LIFE_ICON, "red_life_icon"));
		sprite->setScale(SCALE_FACTOR);
		_listIcons.push_back(sprite);
		_listIcons.back()->setPosition(position.x + (_listIcons.back()->getFrameWidth() + GAP) * i + 80, position.y);
	}

	_text = new Text(L"Arial",text, 20, 20,21);
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
	_text->draw();
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
	if (_hp == number || number < 0 || number > MAX_HP_NUMBER)
		return;

	_hp = number;
	
	for (int i = 0; i < _hp; i++)
	{
		_listIcons[i]->setFrameRect(SpriteManager::getInstance()->getSourceRect(eID::LIFE_ICON, "red_life_icon"));
	}

	for (int i = _hp; i < MAX_HP_NUMBER;i++)
	{
		_listIcons[i]->setFrameRect(SpriteManager::getInstance()->getSourceRect(eID::LIFE_ICON, "white_life_icon"));
	}
}

int LifeUI::getHPNumber()
{
	return _hp;
}

void LifeUI::setLifeNumber(int number)
{
	_life = number;
}

int LifeUI::getLifeNumber()
{
	return _life;
}