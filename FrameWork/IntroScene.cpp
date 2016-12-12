#include "IntroScene.h"
#include "PlayScene.h"

IntroScene::IntroScene()
{
	_viewport = new Viewport(0, WINDOW_HEIGHT, WINDOW_WIDTH, WINDOW_HEIGHT);
}

void IntroScene::updateInput(float deltatime)
{
	if (_input->isKeyDown(DIK_X))
	{
		_ok = true;
	}
}

bool IntroScene::init()
{
	_sprite = SpriteManager::getInstance()->getSprite(eID::INTROSCENE);
	_sprite->setScale(SCALE_FACTOR);
	_sprite->setOrigin(GVector2(0.0f, 0.0f));

	_animations["fly_bat"] = new Animation(_sprite, 0.12f);
	_animations["fly_bat"]->addFrameRect(eID::INTROSCENE,"fly_bat_01", "fly_bat_02", "fly_bat_03", "fly_bat_04", "fly_bat_05",
		"fly_bat_06", "fly_bat_07", "fly_bat_08", "fly_bat_09", "fly_bat_10", "fly_bat_11", "fly_bat_12", "fly_bat_13", "fly_bat_14", "fly_bat_15", NULL);

	_animations["back_ground"] = new Animation(_sprite, 0.1f);
	_animations["back_ground"]->addFrameRect(eID::INTROSCENE, "background", NULL);

	_animations["start"] = new Animation(_sprite, 0.1f);
	_animations["start"]->addFrameRect(eID::INTROSCENE, "start", NULL);
//	_okSprite = new Sprite(, L"Resources//Images//introScene.png");

	_ok = false;
	_flash = false;
	delay = 0;
	return true;
}

void IntroScene::onKeyPressed(KeyEventArg* key_event)
{

}
void IntroScene::update(float dt)
{
	_animations["fly_bat"]->update(dt);
	if (_ok)
	{
		if (!_flash && delay > 200)
		{
			_flash = true;
			delay = 0;
		}
		else
		{
			delay += dt;
			_flash = false;
		}
		if (pendingStopWatch == nullptr)
		{
			pendingStopWatch = new StopWatch();
		}

		if (pendingStopWatch->isStopWatch(1500))
		{
			auto play = new PlayScene();
			SceneManager::getInstance()->replaceScene(play);
		}
	}
}
void IntroScene::draw(LPD3DXSPRITE spriteHandle)
{

	_sprite->setPosition(GVector2(0, WINDOW_HEIGHT));
	_animations["back_ground"]->draw(spriteHandle,_viewport);
	_sprite->setPosition(GVector2(367,254));
	_animations["fly_bat"]->draw(spriteHandle, _viewport);
	if (!_flash)
	{
		_sprite->setPosition(GVector2(146, 204));
		_animations["start"]->draw(spriteHandle, _viewport);
	}

}
void IntroScene::release()
{

}

