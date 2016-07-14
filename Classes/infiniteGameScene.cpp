#include "infiniteGameScene.h"
#include "mainScene.h"
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include <stdlib.h> 
#include <time.h>

#define COUNT_DOWN_FONT_SIZE 64
#define POINT_LABEL 10
#define BURST 11
#define ANI_LABEL 12
#define BOUNS_LABEL 13
#define BURST_TIME 5.0

#define random(x) (rand()%x)

using namespace CocosDenshion;

bool changemale, changecolor;
infinitegameScene::infinitegameScene()
{
}

infinitegameScene::~infinitegameScene()
{
}

Scene* infinitegameScene::createScene()
{
	Scene* scene = Scene::create();
	auto layer = infinitegameScene::create();
	scene->addChild(layer);
	return scene;
}

infinitegameScene* infinitegameScene::create()
{
	infinitegameScene* pRet = new infinitegameScene();
	if (pRet && pRet->init())
	{
		return pRet;
	}
	pRet = NULL;
	return NULL;
}

bool infinitegameScene::init()
{
	if (!Layer::init())
	{
		return false;
	}
	srand((int)time(0));
	Director::getInstance()->setAnimationInterval(1.0 / 60);

	//musics
	preloadMusic();
	playBgm();

	cache = SpriteFrameCache::sharedSpriteFrameCache();
	cache->addSpriteFramesWithFile("picture/manAndWoman.plist", "picture/manAndWoman.png");

	//add background layer
	Size backgroundLayerSize(winSize.width / 3, winSize.height);
	Layer* backgroundLayer1 = LayerColor::create(ccc4(30, 144, 255, 255), backgroundLayerSize.width, backgroundLayerSize.height);
	Layer* backgroundLayer2 = LayerColor::create(ccc4(139, 0, 139, 255), backgroundLayerSize.width, backgroundLayerSize.height);
	Layer* backgroundLayer3 = LayerColor::create(ccc4(178, 34, 34, 255), backgroundLayerSize.width, backgroundLayerSize.height);
	backgroundLayer1->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	backgroundLayer2->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	backgroundLayer3->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	backgroundLayer1->setPosition(origin);
	backgroundLayer2->setPosition(Vec2(origin.x + winSize.width / 3, origin.y));
	backgroundLayer3->setPosition(Vec2(origin.x + winSize.width / 3 * 2, origin.y));
	addChild(backgroundLayer1);
	addChild(backgroundLayer2);
	addChild(backgroundLayer3);

	//add destinations
	destination1man = Sprite::createWithSpriteFrame(cache->spriteFrameByName("manBlack1.png"));//create("manBlack1.png");
	destination1woman = Sprite::createWithSpriteFrame(cache->spriteFrameByName("womanBlack1.png"));//Sprite::create("womanBlack1.png");
	destination2man = Sprite::createWithSpriteFrame(cache->spriteFrameByName("manBlack1.png"));//Sprite::create("manBlack1.png");
	destination2woman = Sprite::createWithSpriteFrame(cache->spriteFrameByName("womanBlack1.png"));//Sprite::create("womanBlack1.png");
	destination1man->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	destination1man->setPosition(leftDestination);
	destination1woman->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	destination1woman->setPosition(leftDestination);
	destination2man->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	destination2man->setPosition(rightDestination);
	destination2woman->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	destination2woman->setPosition(rightDestination);
	destination1man->setVisible(true);
	destination1woman->setVisible(false);
	destination2man->setVisible(false);
	destination2woman->setVisible(true);
	addChild(destination1man);
	addChild(destination1woman);
	addChild(destination2woman);
	addChild(destination2man);


	//a menu to count to begin the game
	middleMenu = Menu::create();
	middleMenu->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	middleMenu->setPosition(winSize.width / 2, winSize.height / 2);
	this->addChild(middleMenu, 10);

	//a menu of pause and quit
	auto labelPause = Label::createWithTTF("Pause", "fonts/Marker Felt.ttf", 32);
	auto itemPause = MenuItemLabel::create(labelPause, CC_CALLBACK_1(infinitegameScene::gameScenePause, this));
	auto labelQuit = Label::createWithTTF("Quit", "fonts/Marker Felt.ttf", 32);
	auto itemQuit = MenuItemLabel::create(labelQuit, CC_CALLBACK_1(infinitegameScene::returnToMainScene, this));

	rightTopMenu = Menu::create(itemPause, itemQuit, NULL);
	rightTopMenu->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	rightTopMenu->setPosition(winSize.width - 50, winSize.height - 60);
	rightTopMenu->alignItemsVerticallyWithPadding(labelPause->getContentSize().height / 2);
	addChild(rightTopMenu, 10);

	//a menu to count to begin the game
	middleMenu = Menu::create();
	middleMenu->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	middleMenu->setPosition(winSize.width / 2, winSize.height / 2);
	this->addChild(middleMenu, 10);

	schedule(schedule_selector(infinitegameScene::countDown), 1.0f);
	return true;
}

void infinitegameScene::countDown(float dt)
{
	if (canReady) {
		middleMenu->removeAllChildren();
		if (readytime > 0) {
			char t[4];
			itoa(readytime, t, 10);
			labelBegin = Label::createWithTTF(t, "fonts/Marker Felt.ttf", COUNT_DOWN_FONT_SIZE);
			readytime--;
			itemBegin = MenuItemLabel::create(labelBegin);
			middleMenu->addChild(itemBegin);
		}
		else if (readytime==0) {
			labelBegin = Label::createWithTTF("Begin!", "fonts/Marker Felt.ttf", COUNT_DOWN_FONT_SIZE);
			readytime--;
			itemBegin = MenuItemLabel::create(labelBegin);
			middleMenu->addChild(itemBegin);
		}
		else {
			canReady = false;
			gameBegin();
		}
		
	}
}


void infinitegameScene::preloadMusic()
{
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic("music/bgm.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("music/succeed.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("music/fail.wav");
}

void infinitegameScene::playBgm()
{
	SimpleAudioEngine::getInstance()->playBackgroundMusic("music/bgm.mp3", true);
}

void infinitegameScene::playEffectSucceed()
{
	SimpleAudioEngine::getInstance()->playEffect("music/succeed.wav");
}

void infinitegameScene::playEffectFail()
{
	SimpleAudioEngine::getInstance()->playEffect("music/fail.wav");
}

void infinitegameScene::stopBgm()
{
	SimpleAudioEngine::getInstance()->stopBackgroundMusic();
}


void infinitegameScene::gameScenePause(Ref* sender)
{
	if (!gameLose && canBePaused) {
		auto labelResume = Label::createWithTTF("Resume", "fonts/Marker Felt.ttf", 32);
		auto itemResume = MenuItemLabel::create(labelResume, CC_CALLBACK_1(infinitegameScene::gameSceneResume, this));
		auto labelQuit = Label::createWithTTF("Quit", "fonts/Marker Felt.ttf", 32);
		auto itemQuit = MenuItemLabel::create(labelQuit, CC_CALLBACK_1(infinitegameScene::returnToMainScene, this));

		rightTopMenu->removeFromParent();
		rightTopMenu = Menu::create(itemResume, itemQuit, NULL);
		rightTopMenu->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		rightTopMenu->setPosition(winSize.width - 50, winSize.height - 60);
		rightTopMenu->alignItemsVerticallyWithPadding(labelResume->getContentSize().height / 2);
		addChild(rightTopMenu, 10);
		if (!Director::getInstance()->isPaused()) {
			Director::getInstance()->pause();
		}
	}
}

void infinitegameScene::gameSceneResume(Ref* sender)
{
	rightTopMenu->removeFromParent();
	auto labelPause = Label::createWithTTF("Pause", "fonts/Marker Felt.ttf", 32);
	auto itemPause = MenuItemLabel::create(labelPause, CC_CALLBACK_1(infinitegameScene::gameScenePause, this));
	auto labelQuit = Label::createWithTTF("Quit", "fonts/Marker Felt.ttf", 32);
	auto itemQuit = MenuItemLabel::create(labelQuit, CC_CALLBACK_1(infinitegameScene::returnToMainScene, this));
	rightTopMenu = Menu::create(itemPause, itemQuit, NULL);
	rightTopMenu->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	rightTopMenu->setPosition(winSize.width - 50, winSize.height - 60);
	rightTopMenu->alignItemsVerticallyWithPadding(labelPause->getContentSize().height / 2);
	addChild(rightTopMenu, 10);
	Director::getInstance()->resume();
}

void infinitegameScene::returnToMainScene(Ref* sender)
{
	stopBgm();
	auto scene = mainScene::createScene();
	Director::getInstance()->replaceScene(scene);
	if (Director::getInstance()->isPaused()) {
		Director::getInstance()->resume();
	}
}

void infinitegameScene::gameBegin() {
	//isCharacterExist = false;
	canBeChose = false;
	gameLose = false;

	//add a keyboard listener
	listenerkeyPad = EventListenerKeyboard::create();
	listenerkeyPad->onKeyPressed = CC_CALLBACK_2(infinitegameScene::onKeyPressed, this);
	dispatcher->addEventListenerWithFixedPriority(listenerkeyPad, 1);//addEventListenerWithSceneGraphPriority(listenerkeyPad, this);

	//game begin
	destinaionType = 0;

	//call createCharacter function to begin the characters action
	timeBetweenDestinations = 3.0f;
	//schedule(schedule_selector(gameScene::changeDestination), timeBetweenDestinations);
	runCharacter();
	this->scheduleUpdate();

	//scheduleOnce(schedule_selector(gameScene::runCharacter), 0.0);
}

void infinitegameScene::runCharacter()
{
	//change destination exchanging frequency
	//unschedule(schedule_selector(gameScene::changeDestination));
	
	//schedule(schedule_selector(gameScene::changeDestination), timeBetweenDestinations);
	changemale = random(2);
	changecolor = random(2);
	if (!gameLose)
	{
		middleMenu->removeAllChildren();

		std::string characterType = (changemale == 0) ? "manBlack1.png" : "womanBlack1.png";
		thisCharacter = Sprite::createWithSpriteFrame(cache->spriteFrameByName(characterType));

		thisCharacter->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
		thisCharacter->setPosition(winSize.width / 2, winSize.height);
		addChild(thisCharacter);


		//action move
		auto actionMove = MoveTo::create(timeBetweenDestinations, Vec2(winSize.width / 2, origin.y - thisCharacter->getContentSize().height - 5));
		thisCharacter->runAction(actionMove);
		isCharacterExist = true;
	}
}

void infinitegameScene::update(float dt) {
	//brust_bar->setPercentage(100);
	if (isCharacterExist) {
		if (thisCharacter->getPositionX() == winSize.width / 2 && thisCharacter->getPositionY()< origin.y - thisCharacter->getContentSize().height) {
			if (changecolor == 0) {
				isCharacterExist = false;
				middleMenu->removeAllChildren();
				//label of win
				auto labelWin = Label::createWithTTF("Finish!", "fonts/Marker Felt.ttf", COUNT_DOWN_FONT_SIZE);
				auto itemWin = MenuItemLabel::create(labelWin);
				itemWin->setPosition(0, 30);
				//label of time

				gameLose = true;
				this->unscheduleAllSelectors();
				return;
			}
			else {
				removeThisCharacterAfterKeyPressed();
				bool changemale = random(2);
				if (changemale) {
					changeDestination();
				}
				
				
				return;
			}
		}

		if (!canBeChose) {
			if (thisCharacter->getPosition().y <= winSize.height / 2 + 5 && thisCharacter->getPosition().y >= winSize.height / 2 - 5) {
				changeCharacterColor();
				
				canBeChose = true;
			}
		}
		/*
		if (thisCharacter->getPositionX() == winSize.width / 2 && thisCharacter->getPositionY() < origin.y - thisCharacter->getContentSize().height) {
			
			//dispatcher->removeEventListener(listenerkeyPad);
		}*/
	}
	
}

void infinitegameScene::onKeyPressed(EventKeyboard::KeyCode keycode, cocos2d::Event *event)
{
	if (isCharacterExist && !Director::getInstance()->isPaused()) {
		if (keycode == EventKeyboard::KeyCode::KEY_LEFT_ARROW )
		{
			if ((changemale == destinaionType && changecolor == 0) && canBeChose) {
				thisCharacter->stopAllActions();
				thisCharacter->setPosition(leftDestination.x, leftDestination.y - thisCharacter->getContentSize().height / 2);
				playEffectSucceed();
				playerGameTime += getCurrentTime() - thisCharacterBeginTime;
				
				changemale = random(2);
				if (changemale) {
					changeDestination();
				}
				removeThisCharacterAfterKeyPressed();

			}
			else if (canBeChose){
				canBeChose = false;
				auto labelWin = Label::createWithTTF("Finish!", "fonts/Marker Felt.ttf", COUNT_DOWN_FONT_SIZE);
				auto itemWin = MenuItemLabel::create(labelWin);
				itemWin->setPosition(0, 30);
				//label of time
				middleMenu->addChild(itemWin);
				gameLose = true;
				this->unscheduleAllSelectors();
			}

		}
		else if (keycode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW) {
			if ((changemale != destinaionType && changecolor == 0) && canBeChose) {
				thisCharacter->stopAllActions();
				thisCharacter->setPosition(rightDestination.x, rightDestination.y - thisCharacter->getContentSize().height / 2);
				playEffectSucceed();
				playerGameTime += getCurrentTime() - thisCharacterBeginTime;
				
				bool changemale = random(2);
					if (changemale) {
						changeDestination();
				}
				removeThisCharacterAfterKeyPressed();
			}
			else if (canBeChose){
				canBeChose = false;
				auto labelWin = Label::createWithTTF("Finish!", "fonts/Marker Felt.ttf", COUNT_DOWN_FONT_SIZE);
				auto itemWin = MenuItemLabel::create(labelWin);
				itemWin->setPosition(0, 30);
				//label of time
				middleMenu->addChild(itemWin);
				gameLose = true;
				this->unscheduleAllSelectors();
			}
		}
	}

}

void infinitegameScene::removeThisCharacterAfterKeyPressed()
{
	thisCharacter->removeFromParentAndCleanup(true);
	canBeChose = false;
	isCharacterExist = false;
	gameLose = false;
	runCharacter();
}

void infinitegameScene::changeDestination()
{
	if (destinaionType == 0) {
		destinaionType = 1;
		destination1man->setVisible(false);
		destination1woman->setVisible(true);
		destination2man->setVisible(true);
		destination2woman->setVisible(false);
	}
	else if (destinaionType == 1) {
		destinaionType = 0;
		destination1man->setVisible(true);
		destination1woman->setVisible(false);
		destination2man->setVisible(false);
		destination2woman->setVisible(true);
	}
}

void infinitegameScene::changeCharacterColor()
{
	if (changemale == 0) {
		if (changecolor == 0) {
			SpriteFrame* a = SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("manRed1.png");
			thisCharacter->setDisplayFrame(a);
		}
		else {
			SpriteFrame* a = SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("manGreen1.png");
			thisCharacter->setDisplayFrame(a);
		}
	}
	else {
		if (changecolor == 0) {
			SpriteFrame* a = SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("womanRed1.png");
			thisCharacter->setDisplayFrame(a);
		}
		else {
			SpriteFrame* a = SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("womanGreen1.png");
			thisCharacter->setDisplayFrame(a);
		}
	}
}

long infinitegameScene::getCurrentTime()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

