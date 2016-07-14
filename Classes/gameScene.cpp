#include "gameScene.h"
#include "mainScene.h"
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include <stdlib.h> 
#include <time.h>
#include <fstream>
#include <algorithm>
#include <sstream>

#define COUNT_DOWN_FONT_SIZE 64

#define random(x) (rand()%x)

using namespace CocosDenshion;

gameScene::gameScene()
{
}

gameScene::~gameScene()
{
}

Scene* gameScene::createScene(bool TipsOpen)
{
	Scene* scene = Scene::create();
	auto layer = gameScene::create(TipsOpen);
	scene->addChild(layer);
	return scene;
}

gameScene* gameScene::create(bool TipsOpen)
{
	gameScene* pRet = new gameScene();
	
	if (pRet && pRet->init())
	{
		pRet->IsOpenTips = TipsOpen;
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

	score = 0;
	std::stringstream ss;
	ss << "Score: " << score;
	label0 = Label::createWithTTF(ss.str(), "fonts/Marker Felt.ttf", 30);
	label0->setPosition(winSize.width / 7, winSize.height - winSize.height / 12);
	addChild(label0, 20);

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

	
	//a menu of pause and quit
	auto labelPause = Label::createWithTTF("Pause", "fonts/Marker Felt.ttf", 32);
	auto itemPause = MenuItemLabel::create(labelPause, CC_CALLBACK_1(gameScene::gameScenePause, this));
	auto labelQuit = Label::createWithTTF("Quit", "fonts/Marker Felt.ttf", 32);
	auto itemQuit = MenuItemLabel::create(labelQuit, CC_CALLBACK_1(gameScene::returnToMainScene, this));

	auto labelUP = Label::createWithTTF("UP", "fonts/Marker Felt.ttf", 32);
	auto itemUP = MenuItemLabel::create(labelUP, CC_CALLBACK_0(gameScene::MusicUP, this));
	auto labelDOWN = Label::createWithTTF("DOWN", "fonts/Marker Felt.ttf", 32);
	auto itemDOWN = MenuItemLabel::create(labelDOWN, CC_CALLBACK_0(gameScene::MusicDOWN, this));
	TopMenu = Menu::create(itemUP, itemDOWN, NULL);
	TopMenu->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	TopMenu->setPosition(winSize.width /2, winSize.height - 60);
	TopMenu->alignItemsVerticallyWithPadding(labelPause->getContentSize().height / 2);
	addChild(TopMenu, 10);

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
	labelBegin = Label::createWithTTF("3", "fonts/Marker Felt.ttf", COUNT_DOWN_FONT_SIZE);
	itemBegin = MenuItemLabel::create(labelBegin);
	middleMenu->addChild(itemBegin);

	//add 15 characters into the vector
	characterQuantity = 15;
	thisCharacterNum = 0;

	isCharacterExist = false;
	canBeChose = false;
	isMenuGetItExist = false;
	gameWin = false;
	gameLose = false;
	canBePaused = true;
	
	generateRandomNumForArrays();

	scheduleOnce(schedule_selector(gameScene::countDown2), 1.0f);
	schedule(schedule_selector(gameScene::update));

	return true;
}

void gameScene::gameScenePause(Ref* sender)
{
	if (!gameLose && !gameWin && canBePaused) {
		auto labelResume = Label::createWithTTF("Resume", "fonts/Marker Felt.ttf", 32);
		auto itemResume = MenuItemLabel::create(labelResume, CC_CALLBACK_1(gameScene::gameSceneResume, this));
		auto labelQuit = Label::createWithTTF("Quit", "fonts/Marker Felt.ttf", 32);
		auto itemQuit = MenuItemLabel::create(labelQuit, CC_CALLBACK_1(gameScene::returnToMainScene, this));

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

void gameScene::gameSceneResume(Ref* sender)
{
	rightTopMenu->removeFromParent();
	auto labelPause = Label::createWithTTF("Pause", "fonts/Marker Felt.ttf", 32);
	auto itemPause = MenuItemLabel::create(labelPause, CC_CALLBACK_1(gameScene::gameScenePause, this));
	auto labelQuit = Label::createWithTTF("Quit", "fonts/Marker Felt.ttf", 32);
	auto itemQuit = MenuItemLabel::create(labelQuit, CC_CALLBACK_1(gameScene::returnToMainScene, this));
	rightTopMenu = Menu::create(itemPause, itemQuit, NULL);
	rightTopMenu->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	rightTopMenu->setPosition(winSize.width - 50, winSize.height - 60);
	rightTopMenu->alignItemsVerticallyWithPadding(labelPause->getContentSize().height / 2);
	addChild(rightTopMenu, 10);
	Director::getInstance()->resume();
}

void gameScene::returnToMainScene(Ref* sender)
{
	stopBgm();
	Scene* scene = Scene::create();
	auto layer = mainScene::create();
	layer->IsOpenTips = IsOpenTips;
	scene->addChild(layer);
	Director::getInstance()->replaceScene(scene);
	if (Director::getInstance()->isPaused()) {
		Director::getInstance()->resume();
	}
}

void gameScene::countDown2(float dt)
{
	middleMenu->removeAllChildren();
	labelBegin = Label::createWithTTF("2", "fonts/Marker Felt.ttf", COUNT_DOWN_FONT_SIZE);
	itemBegin = MenuItemLabel::create(labelBegin);
	middleMenu->addChild(itemBegin);
	scheduleOnce(schedule_selector(gameScene::countDown1), 1.0f);
}

void gameScene::countDown1(float dt)
{
	middleMenu->removeAllChildren();
	labelBegin = Label::createWithTTF("1", "fonts/Marker Felt.ttf", COUNT_DOWN_FONT_SIZE);
	itemBegin = MenuItemLabel::create(labelBegin);
	middleMenu->addChild(itemBegin);
	scheduleOnce(schedule_selector(gameScene::countDown0), 1.0f);
}

void gameScene::countDown0(float dt)
{
	middleMenu->removeAllChildren();
	labelBegin = Label::createWithTTF("Begin!", "fonts/Marker Felt.ttf", COUNT_DOWN_FONT_SIZE);
	itemBegin = MenuItemLabel::create(labelBegin);
	middleMenu->addChild(itemBegin);
	scheduleOnce(schedule_selector(gameScene::gameBegin), 1.0f);
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
			canBeChose = false;

			bool changemale=random(2);
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
			canBeChose = true;
		}
	}
	if (isMenuGetItExist) {
		menuGetIt->removeAllChildren();
		menuGetIt->removeFromParent();
		isMenuGetItExist = false;
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
	canBeChose = false;
	gameLose = false;
	gameWin = false;
	isMenuGetItExist = false;
	canBePaused = true;

	//add a keyboard listener
	listenerkeyPad = EventListenerKeyboard::create();
	listenerkeyPad->onKeyPressed = CC_CALLBACK_2(gameScene::onKeyPressed, this);
	dispatcher->addEventListenerWithFixedPriority(listenerkeyPad, 1);//addEventListenerWithSceneGraphPriority(listenerkeyPad, this);

	//game begin
	destinaionType = 1;
	thisCharacterNum = -1;
	
	//call createCharacter function to begin the characters action
	timeBetweenDestinations = 3.0f;


	//schedule(schedule_selector(gameScene::changeDestination), timeBetweenDestinations);


	scheduleOnce(schedule_selector(gameScene::runCharacterA), 0.0);
	if (IsOpenTips)
		CCLOG("%s \n", "TRUE");
	else
		CCLOG("%s \n", "FALSE");
	if (IsOpenTips)
	scheduleOnce(schedule_selector(gameScene::showInfo), 4.0);
}

void gameScene::showInfo(float dt)
{
	canBePaused = false;
	middleMenu->removeAllChildren();

	infoMenu = Menu::create();
	infoMenu->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	infoMenu->setPosition(winSize.width / 2, winSize.height / 2);
	addChild(infoMenu, 11);
	auto labelInfo = Label::createWithTTF("I wanna remind you that,\n\nthe people coming is\nSPEEDING UP\n\nGOOD LUCK!", "fonts/Marker Felt.ttf", 40);
	auto itemInfo = MenuItemLabel::create(labelInfo);
	infoMenu->addChild(itemInfo);

	menuGetIt = Menu::create();
	menuGetIt->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
	menuGetIt->setPosition(winSize.width, origin.y);
	addChild(menuGetIt, 11);
	auto labelGetIt = Label::createWithTTF("get it", "fonts/Marker Felt.ttf", 32);
	auto itemGetIt = MenuItemLabel::create(labelGetIt, CC_CALLBACK_1(gameScene::resumeFromInfo, this));
	itemGetIt->setPosition(-itemGetIt->getContentSize().width, itemGetIt->getContentSize().height);
	menuGetIt->addChild(itemGetIt);
	
	Director::getInstance()->pause();
}

void gameScene::resumeFromInfo(Ref* sender)
{
	if (Director::getInstance()->isPaused()) {
		Director::getInstance()->resume();
		infoMenu->removeAllChildren();
		infoMenu->removeFromParent();
		isMenuGetItExist = true;
		canBePaused = true;
	}
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
		std::string tempTimeString= Value(tempTimeInt).asString();
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
		std::string tempTimeStringAfterConverted = "";
		for (int i = 0; i < tempTimeStringLength; i++) {
			if (i == tempTimeStringLength - 3) {
				tempTimeStringAfterConverted += '.';
			}
			tempTimeStringAfterConverted += tempTimeString[i];
		}
		setranking(tempTimeStringAfterConverted);                       //ÅÅÐÐ°ñ
		tempTimeStringAfterConverted = "\nYour Time:\n" + tempTimeStringAfterConverted;
		//log(tempTimeString.c_str());
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




//keyboard event
void gameScene::onKeyPressed(EventKeyboard::KeyCode keycode, cocos2d::Event *event)
{
	if (isCharacterExist && !Director::getInstance()->isPaused()) {
		if (keycode == EventKeyboard::KeyCode::KEY_LEFT_ARROW)
		{
			if (manOrWomanArray[thisCharacterNum] == destinaionType && characterTypeArray[thisCharacterNum] == 1 && canBeChose) {
				middleMenu->removeAllChildren();
				labelBingo = Label::createWithTTF("bingo!", "fonts/Marker Felt.ttf", COUNT_DOWN_FONT_SIZE);
				itemBingo = MenuItemLabel::create(labelBingo);
				middleMenu->addChild(itemBingo);

				thisCharacter->stopAllActions();
				thisCharacter->setPosition(leftDestination.x, leftDestination.y - thisCharacter->getContentSize().height / 2);
				playEffectSucceed();
				score += 100;
				std::stringstream ss;
				ss << "Score: " << score;
				label0->setString(ss.str());

				playerGameTime += getCurrentTime() - thisCharacterBeginTime;

				bool changemale = random(2);
				if (changemale) {
					changeDestination();
				}
				scheduleOnce(schedule_selector(gameScene::removeThisCharacterAfterKeyPressed), 0.05);
			}
			else {
				middleMenu->removeAllChildren();
				labelLose = Label::createWithTTF("You Lose!", "fonts/Marker Felt.ttf", COUNT_DOWN_FONT_SIZE);
				itemLose = MenuItemLabel::create(labelLose);
				middleMenu->addChild(itemLose);
				isCharacterExist = false;
				gameLose = true;
				gameWin = false;
				playEffectFail();

				thisCharacter->stopAllActions();
				thisCharacter->setPosition(leftDestination.x, leftDestination.y - thisCharacter->getContentSize().height / 2);
				Director::getInstance()->pause();
			}

		}
		else if (keycode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
		{
			if (manOrWomanArray[thisCharacterNum] != destinaionType && characterTypeArray[thisCharacterNum] == 1 && canBeChose) {
				middleMenu->removeAllChildren();
				labelBingo = Label::createWithTTF("bingo!", "fonts/Marker Felt.ttf", COUNT_DOWN_FONT_SIZE);
				itemBingo = MenuItemLabel::create(labelBingo);
				middleMenu->addChild(itemBingo);

				thisCharacter->stopAllActions();
				thisCharacter->setPosition(rightDestination.x, rightDestination.y - thisCharacter->getContentSize().height / 2);
				playEffectSucceed();
				score += 100;
				std::stringstream ss;
				ss << "Score: " << score;
				label0->setString(ss.str());

				playerGameTime += getCurrentTime() - thisCharacterBeginTime;

				bool changemale = random(2);
				if (changemale) {
					changeDestination();
				}
				scheduleOnce(schedule_selector(gameScene::removeThisCharacterAfterKeyPressed), 0.05);
			}
			else {
				middleMenu->removeAllChildren();
				labelLose = Label::createWithTTF("You Lose!", "fonts/Marker Felt.ttf", COUNT_DOWN_FONT_SIZE);
				itemLose = MenuItemLabel::create(labelLose);
				middleMenu->addChild(itemLose);
				isCharacterExist = false;
				gameLose = true;
				gameWin = false;
				playEffectFail();

				thisCharacter->stopAllActions();
				thisCharacter->setPosition(rightDestination.x, rightDestination.y - thisCharacter->getContentSize().height / 2);
				Director::getInstance()->pause();
			}
		}
	}
	
}

void gameScene::removeThisCharacterAfterKeyPressed(float dt)
{
	thisCharacter->removeFromParentAndCleanup(true);
	canBeChose = false;
	isCharacterExist = false;
	gameWin = false;
	gameLose = false;
	
}

void gameScene::MusicUP()
{
	CCLOG("%f \n", SimpleAudioEngine::sharedEngine()->getBackgroundMusicVolume());
	
	SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(SimpleAudioEngine::sharedEngine()->getBackgroundMusicVolume()+0.1f);

}

void gameScene::MusicDOWN()
{
	CCLOG("%f \n", SimpleAudioEngine::sharedEngine()->getBackgroundMusicVolume());

	SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(SimpleAudioEngine::sharedEngine()->getBackgroundMusicVolume() - 0.1f);

}
bool complareB(int a, int b)

{
	return a<b;
}
std::string doubleConverToString(double d){
	std::ostringstream os;
	if (os << d) return os.str();
	return "invalid conversion";
}
void gameScene::setranking(std::string temp) {
	std::string data = "";
	std::fstream file;
	double this_time = atof(temp.c_str());
	double rankList[11];
	char str[100];
	file.open("RankList.a", std::ios::in);
	if (!file) {
		file.open("RankList.a", std::ios::out);
		data = "11\n";
		for (int i = 0; i < 10; i++) {
			file << data;
		}
		file.close();
		file.open("RankList.a", std::ios::in);
	}

	for (int i = 0; i < 10; i++) {
		file.getline(str, 100);
		log(str);
		rankList[i] = atof(str);
	}
	file.close();
	rankList[10] = this_time;
	std::sort(rankList, rankList + 11, complareB);

	//Ð´ÎÄµµ
	char t[10];
	file.open("RankList.a", std::ios::out);
	for (int i = 0; i < 10; i++) {
		data = doubleConverToString(rankList[i]);
		file << data << '\n';
	}
	file.close();
}
