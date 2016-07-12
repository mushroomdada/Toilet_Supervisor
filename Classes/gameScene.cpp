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
	
	generateRandomNumForArrays();


	return true;
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




