#include "infiniteGameScene.h"
#include "mainScene.h"
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include <stdlib.h> 
#include <time.h>
#include <fstream>
#include <algorithm>

#define COUNT_DOWN_FONT_SIZE 64
#define POINT_LABEL 10
#define BURST 11
#define ANI_LABEL 12
#define BOUNS_LABEL 13
#define BURST_TIME 5.0

#define random(x) (rand()%x)

using namespace CocosDenshion;

bool changemale, changecolor;
int totalpoint;
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


	auto label = Label::createWithTTF("Total", "fonts/Marker Felt.ttf", 32);
	label->setPosition(origin.x +60, winSize.height - 20);
	label->enableOutline(Color4B::RED, 1);
	//label->setTag(POINT_LABEL);
	addChild(label);

	//a menu to count to begin the game
	middleMenu = Menu::create();
	middleMenu->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	middleMenu->setPosition(winSize.width / 2, winSize.height / 2);
	this->addChild(middleMenu, 10);
	totalpoint = 0;
	char t[10];
	itoa(totalpoint, t, 10);
	auto label1 = Label::createWithTTF(t, "fonts/Marker Felt.ttf", 32);
	label1->setPosition(origin.x + 60, winSize.height - 50);
	label1->setTag(POINT_LABEL);
	label1->enableOutline(Color4B::RED, 1);
	addChild(label1);

	Sprite*bg1 = Sprite::create("bar2.png");
	burst_bar = ProgressTimer::create(bg1);
	burst_bar->setPosition(ccp(winSize.width / 2, winSize.height));
	burst_bar->setAnchorPoint(Vec2(0.5, 1));
	//pro1->setRotation(40.0f);
	this->addChild(burst_bar,2);
	burst_bar->setType(kCCProgressTimerTypeBar);
	burst_bar->setBarChangeRate(ccp(0, 1));
	burst_bar->setMidpoint(ccp(0.5, 1));
	burst_bar->setPercentage(0.0f);

	bonus_label = Label::createWithTTF("x1.0", "fonts/Marker Felt.ttf", 32);
	bonus_label->setPosition(origin.x + 50, winSize.height - 150);
	bonus_label->setTag(BOUNS_LABEL);
	bonus_label->enableOutline(Color4B::BLACK, 1);
	addChild(bonus_label,3);

	schedule(schedule_selector(infinitegameScene::countDown), 1.0f);
	schedule(schedule_selector(infinitegameScene::bar_descend), 1.0f);
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

void infinitegameScene::bar_descend(float dt){
	//char t[10];
	//itoa(burst_bar->getPercentage(), t, 10);
	//log(t);
	if (bursttime) {
		burst_bar->setPercentage(burst_bar->getPercentage() - 15.0);
	}
	else {
		burst_bar->setPercentage(burst_bar->getPercentage() - 1.0);
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
	bonus_rate = 1.0;
	bursttime = false;
	bouns_Fire_set = false;
	//schedule(schedule_selector(gameScene::changeDestination), timeBetweenDestinations);
	runCharacter();
	this->scheduleUpdate();

	//scheduleOnce(schedule_selector(gameScene::runCharacter), 0.0);
}

void infinitegameScene::runCharacter()
{
	//change destination exchanging frequency
	//unschedule(schedule_selector(gameScene::changeDestination));
	std::string data = "x1.0";
	if (totalpoint > 100000) {
		timeBetweenDestinations = 1.5f;
		bonus_rate = 10.0;
		data = "x10.0";
		bonus_label->setBMFontSize(46);
		bonus_label->setColor(ccc3(139, 0, 0));   //Éîºì
		bouns_Fire->setLife(1.95f);
		bouns_Fire->setLifeVar(0.35f);
	}
	else if (totalpoint > 50000) {
		timeBetweenDestinations = 1.6f;
		bonus_rate = 7.0;
		data = "x7.0";
		bonus_label->setBMFontSize(42);
		bonus_label->setColor(ccc3(255, 0, 0));   //´¿ºì
		bouns_Fire->setLife(1.50f);
		bouns_Fire->setLifeVar(0.25f);
	}
	else if (totalpoint > 25000) {
		timeBetweenDestinations = 1.7f;
		bonus_rate = 5.0;
		data = "x5.0";
		bonus_label->setBMFontSize(40);
		bonus_label->setColor(ccc3(205, 92, 92));   //Ó¡¶Èºì
		if (!bouns_Fire_set) {
			bouns_Fire_set = true;
			bouns_Fire = ParticleFire::create();
			bouns_Fire->setPosition(origin.x + 50, winSize.height - 160);
			bouns_Fire->setPosVar(Vec2(30, 10));
			bouns_Fire->setStartSize(55);
			bouns_Fire->setStartSizeVar(35);
			bouns_Fire->setEndSize(30);
			bouns_Fire->setEndSizeVar(15);
			bouns_Fire->setLife(1.25f);
			bouns_Fire->setLifeVar(0.35f);
			addChild(bouns_Fire, 2);
		}
	}
	else if (totalpoint > 10000) {
		timeBetweenDestinations = 1.9f;
		bonus_rate = 2.5;
		data = "x2.5";
		bonus_label->setBMFontSize(38);
		bonus_label->setColor(ccc3(255, 255, 0));   //´¿»Æ
	}
	else if (totalpoint > 4000) {
		timeBetweenDestinations = 2.1f;
		bonus_rate = 1.75;
		data = "x1.75";
		bonus_label->setBMFontSize(36);
	}
	else if (totalpoint > 1500) {
		timeBetweenDestinations = 2.5f;
		bonus_rate = 1.25;
		data = "x1.25";
		bonus_label->setBMFontSize(34);
		bonus_label->setColor(ccc3(255, 255, 224));   //Ç³»Æ
		
	}
	
	bonus_label->setString(data);
	
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
			if (changecolor == 0&&!bursttime) {
				isCharacterExist = false;
				middleMenu->removeAllChildren();
				//label of win
				auto labelWin = Label::createWithTTF("Finish!", "fonts/Marker Felt.ttf", COUNT_DOWN_FONT_SIZE);
				auto itemWin = MenuItemLabel::create(labelWin);
				itemWin->setPosition(0, 30);
				//label of time

				char t[10];
				itoa(totalpoint, t, 10);
				auto labelTime = Label::createWithTTF(t, "fonts/Marker Felt.ttf", COUNT_DOWN_FONT_SIZE);
				auto itemTime = MenuItemLabel::create(labelTime);
				itemTime->setPosition(0, -30);
				middleMenu->addChild(itemWin);
				middleMenu->addChild(itemTime);
				gameLose = true;
				infinitegameScene::setranking(totalpoint);
				this->unscheduleAllSelectors();
				return;
			}
			else {
				totalpoint += (bonus_rate*150);
				char t[10];
				itoa(totalpoint, t, 10);
				auto sp_point = (Label*)this->getChildByTag(POINT_LABEL);
				sp_point->setString(t);
				removeThisCharacterAfterKeyPressed(150);
				bool changemale = random(2);
				if (changemale) {
					changeDestination();
				}
				
				
				return;
			}
		}

		if (!canBeChose) {
			if (thisCharacter->getPosition().y <= winSize.height / 2 + 5 && thisCharacter->getPosition().y >= winSize.height / 2 - 5) {
				if (burst_bar->getPercentage() >= 98) {
					bursttime = true;
					burst_Galaxy = ParticleGalaxy::create();
					burst_Galaxy->setPosition(winSize.width / 2, winSize.height - 50);
					addChild(burst_Galaxy, 4);
				}
				else {
					bursttime = false;
				}
				//set the begin time
				thisCharacterBeginTime = getCurrentTime();
				if (bursttime) {
					//Texture2D *tex;
					thisCharacterBeginTime = getCurrentTime();
					Sprite* a = Sprite::create("ManAndWomanRed.png");
					thisCharacter->removeFromParentAndCleanup(true);
					thisCharacter = a;
					thisCharacter->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
					thisCharacter->setPosition(winSize.width / 2, winSize.height/2);
					addChild(thisCharacter);


					//action move
					auto actionMove = MoveTo::create(BURST_TIME, Vec2(winSize.width / 2, origin.y - thisCharacter->getContentSize().height - 5));
					thisCharacter->runAction(actionMove);
					//a->setTag(BURST);
					//tex = a->getTexture();
					//thisCharacter->setTexture(tex);
					thisCharacter->setTag(BURST);
					//addChild(a);

				}
				else {
					//change character color
					changeCharacterColor();
				}
				
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
			int get_point;
			if (((changemale == destinaionType && changecolor == 0) || bursttime) && canBeChose) {
				if (!bursttime) {
					thisCharacter->stopAllActions();
					thisCharacter->setPosition(leftDestination.x, leftDestination.y - thisCharacter->getContentSize().height / 2);
					playEffectSucceed();
					playerGameTime += getCurrentTime() - thisCharacterBeginTime;
					get_point = 100 + (timeBetweenDestinations / 2 - ((float)getCurrentTime() - thisCharacterBeginTime) / 1000) / timeBetweenDestinations * 300;
					totalpoint = totalpoint + bonus_rate*get_point;
				}
				else {
					get_point = 100 + (BURST_TIME - ((float)getCurrentTime() - thisCharacterBeginTime) / 1000) / BURST_TIME * 300;
					totalpoint = totalpoint + sqrt(bonus_rate)*get_point;
				}
				

				char t[10];
				itoa(totalpoint, t, 10);
				auto sp_point = (Label*)this->getChildByTag(POINT_LABEL);
				sp_point->setString(t);
				
				if (!bursttime) {
					itoa(bonus_rate*get_point, t, 10);
				}
				else {
					itoa(sqrt(bonus_rate)*get_point, t, 10);
				}
				
				auto label = Label::createWithTTF(t, "fonts/Marker Felt.ttf", 24);
				label->setPosition(leftDestination.x + random(60) - 30, leftDestination.y + random(60) - 30);
				
				
				auto moveby = MoveBy::create(3.0, Vec2(0,random(60) + 70));
				auto fadeOut = FadeOut::create(3.0);

				auto mySpawn = Spawn::createWithTwoActions(moveby, fadeOut);
				auto seq = Sequence::create(mySpawn,CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent,label)) ,nullptr);
				label->runAction(seq);
				label->setTag(ANI_LABEL);
				addChild(label);

				
				if (!bursttime) {
					bool changemale = random(2);
					if (changemale) {
						changeDestination();
					}
					removeThisCharacterAfterKeyPressed(get_point);
				}
				
			}
			else if (canBeChose){
				canBeChose = false;
				auto labelWin = Label::createWithTTF("Finish!", "fonts/Marker Felt.ttf", COUNT_DOWN_FONT_SIZE);
				auto itemWin = MenuItemLabel::create(labelWin);
				itemWin->setPosition(0, 30);
				//label of time
				char t[10];
				itoa(totalpoint, t, 10);

				auto labelTime = Label::createWithTTF(t, "fonts/Marker Felt.ttf", COUNT_DOWN_FONT_SIZE);
				auto itemTime = MenuItemLabel::create(labelTime);
				itemTime->setPosition(0, -30);
				middleMenu->addChild(itemWin);
				middleMenu->addChild(itemTime);
				gameLose = true;
				infinitegameScene::setranking(totalpoint);
				this->unscheduleAllSelectors();
			}

		}
		else if (keycode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW) {
			if (((changemale != destinaionType && changecolor == 0) || bursttime) && canBeChose) {
				int get_point;
				if (!bursttime) {
					thisCharacter->stopAllActions();
					thisCharacter->setPosition(rightDestination.x, rightDestination.y - thisCharacter->getContentSize().height / 2);
					playEffectSucceed();
					playerGameTime += getCurrentTime() - thisCharacterBeginTime;
					get_point = 100 + (timeBetweenDestinations / 2 - ((float)getCurrentTime() - thisCharacterBeginTime) / 1000) / timeBetweenDestinations * 300;
					totalpoint = totalpoint + bonus_rate*get_point;
				}
				else {
					get_point = 100 + (BURST_TIME  - ((float)getCurrentTime() - thisCharacterBeginTime) / 1000) / BURST_TIME * 300;
					totalpoint = totalpoint + sqrt(bonus_rate)*get_point;
				}

				char t[10];
				itoa(totalpoint, t, 10);
				auto sp_point = (Label*)this->getChildByTag(POINT_LABEL);
				sp_point->setString(t);

				if (!bursttime) {
					itoa(bonus_rate*get_point, t, 10);
				}
				else {
					itoa(sqrt(bonus_rate)*get_point, t, 10);
				}
				auto label = Label::createWithTTF(t, "fonts/Marker Felt.ttf", 24);
				label->setPosition(rightDestination.x + random(60) - 30, rightDestination.y + random(60) - 30);
				

				auto moveby = MoveBy::create(3.0, Vec2(0, random(60) + 70));
				auto fadeOut = FadeOut::create(3.0);

				auto mySpawn = Spawn::createWithTwoActions(moveby, fadeOut);
				auto seq = Sequence::create(mySpawn, CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, label)), nullptr);
				label->runAction(seq);
				label->setTag(ANI_LABEL);
				addChild(label);

				
				if (!bursttime) {
					bool changemale = random(2);
					if (changemale) {
						changeDestination();
					}
					removeThisCharacterAfterKeyPressed(get_point);
				}
			}
			else if (canBeChose){
				canBeChose = false;
				auto labelWin = Label::createWithTTF("Finish!", "fonts/Marker Felt.ttf", COUNT_DOWN_FONT_SIZE);
				auto itemWin = MenuItemLabel::create(labelWin);
				itemWin->setPosition(0, 30);
				//label of time
				char t[10];
				itoa(totalpoint, t, 10);

				auto labelTime = Label::createWithTTF(t, "fonts/Marker Felt.ttf", COUNT_DOWN_FONT_SIZE);
				auto itemTime = MenuItemLabel::create(labelTime);
				itemTime->setPosition(0, -30);
				middleMenu->addChild(itemWin);
				middleMenu->addChild(itemTime);
				gameLose = true;
				infinitegameScene::setranking(totalpoint);
				this->unscheduleAllSelectors();
			}
		}
	}

}

void infinitegameScene::removeThisCharacterAfterKeyPressed(int point)
{
	if (bursttime) {
		burst_Galaxy->removeFromParentAndCleanup(true);
	}
	burst_bar->setPercentage(burst_bar->getPercentage() + (float)point*0.03);       //0.03
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
bool complare(int a, int b)

{
	return a>b;
}
void infinitegameScene::setranking(int point) {
	std::string data = "";
	std::fstream file;
	int rankList[11];
	char str[100];
	file.open("RankList.b", std::ios::in);
	if (!file) {
		file.open("RankList.b", std::ios::out);
		data = "11\n";
		for (int i = 0; i < 10; i++) {
			file << data;
		}
		file.close();
		file.open("RankList.b", std::ios::in);
	}

	for (int i = 0; i < 10; i++) {
		file.getline(str, 100);
		log(str);
		rankList[i] = atoi(str);
	}
	file.close();
	rankList[10] = point;
	std::sort(rankList, rankList + 11, complare);

	//Ð´ÎÄµµ
	char t[10];
	file.open("RankList.b", std::ios::out);
	for (int i = 0; i < 10; i++) {
		itoa(rankList[i], t, 10);
		data = t;
		file << data<<'\n';
	}
	file.close();
}

