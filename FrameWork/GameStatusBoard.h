#pragma 
#include"LifeUI.h"
#include"define.h"
#include"Score.h"
#include"GameTime.h"
#include"HeartCounter.h"
#include"SceneTime.h"
#include"LifeCounter.h"
#include"ActiveWeapon.h"
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
	LifeUI* getEnemyLifeUI();

private:
	LifeUI*		_simonLifeUI;
	LifeUI*		_enemyLifeUI;
	Text*		_life;

	//Dùng để hiển thị nền màu đen cho bảng
	Sprite* _background;
	LPDIRECT3DSURFACE9 _surface;
	void drawBlankBackground(LPD3DXSPRITE);
};

