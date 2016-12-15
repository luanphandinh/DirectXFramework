#pragma once
#include"Scene.h"
#include"BaseObject.h"
#include"Animation.h"
#include"Text.h"
#include<map>
class GameOverScene : public Scene, public IControlable
{
public:
	GameOverScene();
	~GameOverScene();


	bool init() override;
	void update(float dt) override;
	void draw(LPD3DXSPRITE spriteHandle) override;
	void release() override;
	void updateInput(float deltatime) override;
	void setContinueLevel(eID level);
private:
	Sprite* _sprite;
	void onKeyPressed(KeyEventArg* key_event);
	bool _continue;
	bool _end;
	eID _continueLevel;

	Text* _gameOverText;
	Text* _playAgainText;
	Text* _endText;
};

