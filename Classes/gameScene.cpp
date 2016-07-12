#include "gameScene.h"
#include "mainScene.h"
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include <stdlib.h> 
#include <time.h>

#define COUNT_DOWN_FONT_SIZE 64

#define random(x) (rand()%x)

using namespace CocosDenshion;

gameScene::gameScene()
{
}

gameScene::~gameScene()
{
}

Scene* gameScene::createScene()
{
	Scene* scene = Scene::create();
	auto layer = gameScene::create();
	scene->addChild(layer);
	return scene;
}

gameScene* gameScene::create()
{
	gameScene* pRet = new gameScene();
	if (pRet && pRet->init())
	{
		return pRet;
	}
	pRet = NULL;
	return NULL;
}

bool gameScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

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

	//add 15 characters into the vector
	characterQuantity = 15;
	thisCharacterNum = 0;

	isCharacterExist = false;
	gameWin = false;
	gameLose = false;

	generateRandomNumForArrays();
	schedule(schedule_selector(gameScene::update));

	return true;
}


void gameScene::update(float dt)
{
	if (isCharacterExist) {
		if (thisCharacter->getPosition() == Vec2(winSize.width / 2, origin.y)) {
			if (characterTypeArray[thisCharacterNum] == 1) {
				middleMenu->removeAllChildren();
				labelLose = Label::createWithTTF("You Lose!", "fonts/Marker Felt.ttf", COUNT_DOWN_FONT_SIZE);
				itemLose = MenuItemLabel::create(labelLose);
				middleMenu->addChild(itemLose);
				isCharacterExist = false;
				gameLose = true;
				playEffectFail();
				Director::getInstance()->pause();
			}
		}
		if (thisCharacter->getPosition() == Vec2(winSize.width / 2, origin.y - thisCharacter->getContentSize().height)) {
			thisCharacter->removeFromParentAndCleanup(true);
			isCharacterExist = false;

			bool changemale = random(2);
			if (changemale) {
				changeDestination();
			}
			//dispatcher->removeEventListener(listenerkeyPad);
		}
	}
	if (isCharacterExist) {
		if (thisCharacter->getPosition().y <= winSize.height / 2 + 5 && thisCharacter->getPosition().y >= winSize.height / 2 - 5) {
			//set the begin time
			thisCharacterBeginTime = getCurrentTime();

			//change character color
			changeCharacterColor();
		}
	}
}

long gameScene::getCurrentTime()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

void gameScene::changeDestination()
{
	if (destinaionType == 1) {
		destinaionType = 2;
		destination1man->setVisible(false);
		destination1woman->setVisible(true);
		destination2man->setVisible(true);
		destination2woman->setVisible(false);
	}
	else if (destinaionType == 2) {
		destinaionType = 1;
		destination1man->setVisible(true);
		destination1woman->setVisible(false);
		destination2man->setVisible(false);
		destination2woman->setVisible(true);
	}
}

void gameScene::changeCharacterColor()
{
	if (manOrWomanArray[thisCharacterNum] == 1) {
		if (characterTypeArray[thisCharacterNum] == 1) {
			SpriteFrame* a = SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("manRed1.png");
			thisCharacter->setDisplayFrame(a);
		}
		else {
			SpriteFrame* a = SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("manGreen1.png");
			thisCharacter->setDisplayFrame(a);
		}
	}
	else {
		if (characterTypeArray[thisCharacterNum] == 1) {
			SpriteFrame* a = SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("womanRed1.png");
			thisCharacter->setDisplayFrame(a);
		}
		else {
			SpriteFrame* a = SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("womanGreen1.png");
			thisCharacter->setDisplayFrame(a);
		}
	}
}

void gameScene::generateRandomNumForArrays()
{
	srand((unsigned)time(NULL));
	int womanLeft = 5, type2Left = 5;
	for (int i = 0; i < characterQuantity; i++) {
		int random1 = rand() % 2 + 1;
		int random2 = rand() % 2 + 1;
		if (random1 == 2 && womanLeft > 0) {
			manOrWomanArray[i] = 2;
			womanLeft--;
		}
		else {
			manOrWomanArray[i] = 1;
		}
		if (random2 == 2 && type2Left > 0) {
			characterTypeArray[i] = 2;
			type2Left--;
		}
		else {
			characterTypeArray[i] = 1;
		}
	}
}

void gameScene::preloadMusic()
{
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic("music/bgm.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("music/succeed.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("music/fail.wav");
}

void gameScene::playBgm()
{
	SimpleAudioEngine::getInstance()->playBackgroundMusic("music/bgm.mp3", true);
}

void gameScene::playEffectSucceed()
{
	SimpleAudioEngine::getInstance()->playEffect("music/succeed.wav");
}

void gameScene::playEffectFail()
{
	SimpleAudioEngine::getInstance()->playEffect("music/fail.wav");
}

void gameScene::stopBgm()
{
	SimpleAudioEngine::getInstance()->stopBackgroundMusic();
}





void gameScene::gameBegin(float dt)
{
	//initialization
	middleMenu->removeAllChildren();
	isCharacterExist = false;
	gameLose = false;
	gameWin = false;
	//game begin
	destinaionType = 1;
	thisCharacterNum = -1;

	//call createCharacter function to begin the characters action
	timeBetweenDestinations = 3.0f;


	//schedule(schedule_selector(gameScene::changeDestination), timeBetweenDestinations);


	scheduleOnce(schedule_selector(gameScene::runCharacterA), 0.0);
}




void gameScene::runCharacterA(float dt)
{
	thisCharacterNum++;
	//change destination exchanging frequency
	//unschedule(schedule_selector(gameScene::changeDestination));
	if (timeBetweenDestinations > 1.5f) {
		timeBetweenDestinations -= 0.15f;
	}
	//schedule(schedule_selector(gameScene::changeDestination), timeBetweenDestinations);
	if (!gameWin && !gameLose && thisCharacterNum < characterQuantity)
	{
		middleMenu->removeAllChildren();

		std::string characterType = (manOrWomanArray[thisCharacterNum] == 1) ? "manBlack1.png" : "womanBlack1.png";
		thisCharacter = Sprite::createWithSpriteFrame(cache->spriteFrameByName(characterType));

		thisCharacter->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
		thisCharacter->setPosition(winSize.width / 2, winSize.height);
		addChild(thisCharacter);
		isCharacterExist = true;
		//action move
		if (characterTypeArray[thisCharacterNum] == 1) {
			auto actionMove = MoveTo::create(timeBetweenDestinations, Vec2(winSize.width / 2, origin.y));
			thisCharacter->runAction(actionMove);
		}
		else {
			auto actionMove = MoveTo::create(timeBetweenDestinations, Vec2(winSize.width / 2, origin.y - thisCharacter->getContentSize().height));
			thisCharacter->runAction(actionMove);
		}

		scheduleOnce(schedule_selector(gameScene::runCharacterB), 4.0);
	}
	else if (thisCharacterNum == characterQuantity) {
		isCharacterExist = false;
		middleMenu->removeAllChildren();
		//label of win
		auto labelWin = Label::createWithTTF("You win!", "fonts/Marker Felt.ttf", COUNT_DOWN_FONT_SIZE);
		auto itemWin = MenuItemLabel::create(labelWin);
		itemWin->setPosition(0, 30);
		//label of time
		int tempTimeInt = playerGameTime;
		std::string tempTimeString = Value(tempTimeInt).asString();
		int tempTimeStringLength = tempTimeString.length();
		std::string tempTimeStringAfterConverted = "";
		for (int i = 0; i < tempTimeStringLength; i++) {
			if (i == tempTimeStringLength - 3) {
				tempTimeStringAfterConverted += '.';
			}
			tempTimeStringAfterConverted += tempTimeString[i];
		}
		auto labelTime = Label::createWithTTF(tempTimeStringAfterConverted, "fonts/Marker Felt.ttf", COUNT_DOWN_FONT_SIZE);
		auto itemTime = MenuItemLabel::create(labelTime);
		itemTime->setPosition(0, -30);
		middleMenu->addChild(itemWin);
		middleMenu->addChild(itemTime);
		gameLose = false;
		gameWin = true;
		this->unscheduleAllSelectors();
	}
}

void gameScene::runCharacterB(float dt)
{
	thisCharacterNum++;
	//change destination exchanging frequency
	//unschedule(schedule_selector(gameScene::changeDestination));
	if (timeBetweenDestinations > 1.5f) {
		timeBetweenDestinations -= 0.15f;
	}
	//schedule(schedule_selector(gameScene::changeDestination), timeBetweenDestinations);
	if (!gameWin && !gameLose && thisCharacterNum < characterQuantity)
	{
		middleMenu->removeAllChildren();

		std::string characterType = (manOrWomanArray[thisCharacterNum] == 1) ? "manBlack1.png" : "womanBlack1.png";
		thisCharacter = Sprite::createWithSpriteFrame(cache->spriteFrameByName(characterType));

		thisCharacter->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
		thisCharacter->setPosition(winSize.width / 2, winSize.height);
		addChild(thisCharacter);
		isCharacterExist = true;

		if (characterTypeArray[thisCharacterNum] == 1) {
			auto actionMove = MoveTo::create(timeBetweenDestinations, Vec2(winSize.width / 2, origin.y));
			thisCharacter->runAction(actionMove);
		}
		else {
			auto actionMove = MoveTo::create(timeBetweenDestinations, Vec2(winSize.width / 2, origin.y - thisCharacter->getContentSize().height));
			thisCharacter->runAction(actionMove);
		}
		scheduleOnce(schedule_selector(gameScene::runCharacterA), 4.0);

	}
	else if (thisCharacterNum == characterQuantity) {
		isCharacterExist = false;
		middleMenu->removeAllChildren();
		auto labelWin = Label::createWithTTF("You win!", "fonts/Marker Felt.ttf", COUNT_DOWN_FONT_SIZE);
		auto itemWin = MenuItemLabel::create(labelWin);
		itemWin->setPosition(0, 30);
		int tempTime = playerGameTime;
		//label of time
		int tempTimeInt = playerGameTime;
		std::string tempTimeString = Value(tempTimeInt).asString();
		int tempTimeStringLength = tempTimeString.length();
		std::string tempTimeStringAfterConverted = "\nYour Time:\n";
		for (int i = 0; i < tempTimeStringLength; i++) {
			if (i == tempTimeStringLength - 3) {
				tempTimeStringAfterConverted += '.';
			}
			tempTimeStringAfterConverted += tempTimeString[i];
		}
		tempTimeStringAfterConverted += "s";
		auto labelTime = Label::createWithTTF(tempTimeStringAfterConverted, "fonts/Marker Felt.ttf", 40);
		auto itemTime = MenuItemLabel::create(labelTime);
		itemTime->setPosition(0, -30);
		middleMenu->addChild(itemWin);
		middleMenu->addChild(itemTime);
		gameLose = false;
		gameWin = true;
		this->unscheduleAllSelectors();
	}
}


