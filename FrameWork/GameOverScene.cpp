#include "GameOverScene.h"
#include "SoundManager.h"
//#include "GameStatusBoard.h"
#include "PlayScene.h"

GameOverScene::GameOverScene()
{
	_viewport = new Viewport(0, WINDOW_HEIGHT, WINDOW_WIDTH, WINDOW_HEIGHT);
	_gameOverText = new Text(L"Arial", "GAME OVER", 190, 200,30);
	_playAgainText = new Text(L"Arial", "CONTINUE", 210, 280,30);
	_endText = new Text(L"Arial", "END", 210, 350,30);

}
void GameOverScene::setContinueLevel(eID level)
{
	_continueLevel = level;
}

GameOverScene::~GameOverScene()
{

}

void GameOverScene::updateInput(float deltatime)
{
	if (_input->isKeyDown(DIK_UP))
	{
		if (_continue) return;
		else
		{
			_continue = true;
			_end = false;
			_sprite->setPosition(GVector2(170, 280));
		}
	}
	else 
	if (_input->isKeyDown(DIK_DOWN))
	{
		if (_end) return;
		else
		{
			_continue = false;
			_end = true;
			_sprite->setPosition(GVector2(170, 355));
		}
	}
	else
	if (_input->isKeyDown(DIK_X))
	{
		if (_end) return;
		if (_continue)
		{
			GameStatusBoard::getInstance()->init();
			auto play = new PlayScene();
			SceneManager::getInstance()->replaceScene(play);
		}
	}
}

bool GameOverScene::init()
{
	
	_sprite = SpriteManager::getInstance()->getSprite(eID::ITEM);
	_sprite->setScale(3.0f);
	_sprite->setOrigin(GVector2(0.0f, 1.0f));
	_sprite->setFrameRect(SpriteManager::getInstance()->getSourceRect(eID::ITEM, "heart_small"));

	SoundManager::getInstance()->PlayLoop(eSoundId::BACKGROUND_MENU);
	_continue = true;
	_end = false;
	_sprite->setPosition(GVector2(170, 280));
	return true;
}

void GameOverScene::onKeyPressed(KeyEventArg* key_event)
{

}
void GameOverScene::update(float dt)
{
	
}
void GameOverScene::draw(LPD3DXSPRITE spriteHandle)
{
	GameStatusBoard::getInstance()->draw(spriteHandle);
	_sprite->render(spriteHandle);
	_gameOverText->draw();
	_playAgainText->draw();
	_endText->draw();

}
void GameOverScene::release()
{
	SAFE_DELETE(_sprite);
	SoundManager::getInstance()->Stop(eSoundId::BACKGROUND_MENU);
}

