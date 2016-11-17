#pragma 
#include"LifeUI.h"
#include"define.h"
#include"Score.h"
#include"GameTime.h"
#define SCORE_POSITION GVector2(20, 0)
#define TIME_POSITION GVector2(180, 0)
#define SIMONLIFEUI_POSITION GVector2(20, 23)
#define ENEMYLIFEUI_POSITION GVector2(20, 46)

class GameStatusBoard
{
public:
	GameStatusBoard();
	~GameStatusBoard();
	static GameStatusBoard* _instance;
	static GameStatusBoard* getInstance();

	void init();
	void draw(LPD3DXSPRITE spriteHandle);

	void setSimonLifeUI(LifeUI* _lifeUI);
	LifeUI* getSimonLifeUI();

	void setEnemyLifeUI(LifeUI* _lifeUI);
	LifeUI* getEnemyLifeUI(LifeUI* _lifeUI);

private:
	LifeUI* _simonLifeUI;
	LifeUI* _enemyLifeUI;
	Text*	_scoreText;
	Text*	_timeText;
};

