#pragma once
#include"Scene.h"
#include"BaseObject.h"
#include"Animation.h"
#include<map>
class IntroScene : public Scene,public IControlable
{
public:
	IntroScene();
	~IntroScene();


	bool init() override;
	void update(float dt) override;
	void draw(LPD3DXSPRITE spriteHandle) override;
	void release() override;
	void updateInput(float deltatime) override;

private:
	Sprite* _sprite;
	map<string, Animation*> _animations;
	void onKeyPressed(KeyEventArg* key_event);
	Viewport* _viewport;
	Sprite* _okSprite;
	bool _ok;
};

