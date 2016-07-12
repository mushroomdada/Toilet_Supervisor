#pragma once

#include "cocos2d.h"

USING_NS_CC;

class gameScene :public Layer
{
public:
	static Scene* createScene();
	static gameScene* create();
	bool init();
	//void update(float);
	void generateRandomNumForArrays();

	void preloadMusic();
	void playBgm();
	void playEffectSucceed();
	void playEffectFail();
	void stopBgm();

private:
	gameScene();
	~gameScene();
	Size winSize = Director::getInstance()->getWinSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	EventDispatcher* dispatcher = Director::getInstance()->getEventDispatcher();
	//EventListenerKeyboard* listenerkeyPad;
	SpriteFrameCache* cache;

	Vec2 leftDestination = Vec2(winSize.width / 3 / 2, winSize.height / 5);
	Vec2 rightDestination = Vec2(winSize.width / 6 * 5, winSize.height / 5);

	Menu* middleMenu;
	Menu* rightTopMenu;
	int manOrWomanArray[15];
	int characterTypeArray[15];
	int characterQuantity;
	Sprite* destination1man;
	Sprite* destination1woman;
	Sprite* destination2man;
	Sprite* destination2woman;
	int destinaionType;
};