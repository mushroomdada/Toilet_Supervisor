#pragma once

#include "cocos2d.h"

USING_NS_CC;

class gameScene :public Layer
{
public:
	static Scene* createScene();
	static gameScene* create();
	bool init();
	void update(float);
	long getCurrentTime();
	void changeCharacterColor();
	void generateRandomNumForArrays();


	void gameBegin(float);

	void runCharacterA(float);
	void runCharacterB(float);

	void onKeyPressed(EventKeyboard::KeyCode keycode, cocos2d::Event *event);

	void changeDestination();

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
	SpriteFrameCache* cache;

	Vec2 leftDestination = Vec2(winSize.width / 3 / 2, winSize.height / 5);
	Vec2 rightDestination = Vec2(winSize.width / 6 * 5, winSize.height / 5);

	Menu* middleMenu;
	Label* labelLose;
	MenuItemLabel* itemLose;

	bool gameWin;
	bool gameLose;
	bool isCharacterExist;

	Sprite* thisCharacter;
	int manOrWomanArray[15];
	int characterTypeArray[15];
	int characterQuantity;
	int thisCharacterNum;

	long thisCharacterBeginTime = 0;
	long playerGameTime = 0;

	Sprite* destination1man;
	Sprite* destination1woman;
	Sprite* destination2man;
	Sprite* destination2woman;
	int destinaionType;
	float timeBetweenDestinations;
};