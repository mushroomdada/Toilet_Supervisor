#pragma once

#include "cocos2d.h"

USING_NS_CC;

class infinitegameScene :public Layer
{
public:
	static Scene* createScene();
	static infinitegameScene* create();
	bool init();
	void update(float);

	void gameScenePause(Ref*);
	void gameSceneResume(Ref*);
	void returnToMainScene(Ref*);

	void changeDestination();
	void changeCharacterColor();
	void countDown(float);
	void gameBegin();
	void runCharacter();
	void removeThisCharacterAfterKeyPressed();
	void onKeyPressed(EventKeyboard::KeyCode keycode, cocos2d::Event *event);

	long getCurrentTime();
	void preloadMusic();
	void playBgm();
	void playEffectSucceed();
	void playEffectFail();
	void stopBgm();

private:
	infinitegameScene();
	~infinitegameScene();
	Size winSize = Director::getInstance()->getWinSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	EventDispatcher* dispatcher = Director::getInstance()->getEventDispatcher();
	EventListenerKeyboard* listenerkeyPad;
	SpriteFrameCache* cache;

	Vec2 leftDestination = Vec2(winSize.width / 3 / 2, winSize.height / 5);
	Vec2 rightDestination = Vec2(winSize.width / 6 * 5, winSize.height / 5);

	Menu* middleMenu;
	Menu* rightTopMenu;

	Label* labelBegin;
	MenuItemLabel* itemBegin;
	Sprite* thisCharacter;
	bool gameLose;
	bool canBePaused;
	bool isCharacterExist;
	bool canBeChose;
	Sprite* destination1man;
	Sprite* destination1woman;
	Sprite* destination2man;
	Sprite* destination2woman;
	int destinaionType;
	long playerGameTime = 0;
	long thisCharacterBeginTime = 0;
	float timeBetweenDestinations;

	int readytime = 3;
	bool canReady = true;
};