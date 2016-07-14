#pragma once

#include "cocos2d.h"

USING_NS_CC;

class gameScene :public Layer
{
public:
	static Scene* createScene(bool TipsOpen);
	static gameScene* create(bool TipsOpen);
	bool init();
	void update(float);
	long getCurrentTime();
	void changeCharacterColor();
	void generateRandomNumForArrays();

	void gameScenePause(Ref*);
	void gameSceneResume(Ref*);
	void returnToMainScene(Ref*);

	void countDown2(float);
	void countDown1(float);
	void countDown0(float);

	void gameBegin(float);
	void showInfo(float);
	void resumeFromInfo(Ref*);

	void runCharacterA(float);
	void runCharacterB(float);

	void onKeyPressed(EventKeyboard::KeyCode keycode, cocos2d::Event *event);
	void removeThisCharacterAfterKeyPressed(float);

	void changeDestination();

	void preloadMusic();
	void playBgm();
	void playEffectSucceed();
	void playEffectFail();
	void stopBgm();
	bool IsOpenTips = true;
	void MusicUP();
	void MusicDOWN();

	void setranking(std::string);

private:
	gameScene();
	~gameScene();
	Size winSize = Director::getInstance()->getWinSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	EventDispatcher* dispatcher = Director::getInstance()->getEventDispatcher();
	EventListenerKeyboard* listenerkeyPad;
	SpriteFrameCache* cache;

	Vec2 leftDestination = Vec2(winSize.width / 3 / 2, winSize.height / 5);
	Vec2 rightDestination = Vec2(winSize.width / 6 * 5, winSize.height / 5);

	Menu* middleMenu;
	Menu* TopMenu;
	Menu* rightTopMenu;
	Menu* infoMenu;
	Menu* menuGetIt;
	

	int countToBegin;
	Label* label0;
	Label* labelBegin;
	MenuItemLabel* itemBegin;
	Label* labelLose;
	MenuItemLabel* itemLose;
	Label* labelBingo;
	MenuItemLabel* itemBingo;

	bool gameWin;
	bool gameLose;
	bool isCharacterExist;
	bool canBeChose;
	bool isMenuGetItExist;
	bool canBePaused;

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
	int score;
};