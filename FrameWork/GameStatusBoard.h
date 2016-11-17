#pragma 
#include"LifeUI.h"
#include"define.h"
#define SIMONLIFEUI_POSITION GVector2(20, 30)
#define ENEMYLIFEUI_POSITION GVector2(20, 60)
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
};
