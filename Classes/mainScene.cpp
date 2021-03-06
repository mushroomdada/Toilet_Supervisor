﻿#include "mainScene.h"
#include "gameScene.h"
#include "infiniteGameScene.h"
#include "infoScene.h"
#include "tipScene.h"
#include "RankScene.h"
using namespace std;


mainScene::mainScene()
{
}

mainScene::~mainScene()
{
}

Scene* mainScene::createScene()
{
	Scene* scene = Scene::create();
	auto layer = mainScene::create();
	scene->addChild(layer);
	return scene;
}

mainScene* mainScene::create()
{
	mainScene* pRet = new mainScene();
	if (pRet && pRet->init())
	{
		return pRet;
	}
	pRet = NULL;
	return NULL;
}

bool mainScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Size winSize = Director::getInstance()->getWinSize();
	dispatcher = Director::getInstance()->getEventDispatcher();

	//add background
	auto background = Sprite::create("picture/mainPageBackground.png");
	background->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	background->setPosition(winSize.width / 2, winSize.height / 2);
	addChild(background);

	//name of game
	auto label0 = Label::createWithTTF("Toilet Supervisor", "fonts/Marker Felt.ttf", 40);
	label0->setPosition(winSize.width / 2, winSize.height / 3 * 2);
	addChild(label0);

	//add a menu
	auto label1 = Label::createWithTTF("Play(Normal)", "fonts/Marker Felt.ttf", 32);
	auto label2 = Label::createWithTTF("How to play", "fonts/Marker Felt.ttf", 32);
	auto label3 = Label::createWithTTF("Information about us", "fonts/Marker Felt.ttf", 32);
	auto label4 = Label::createWithTTF("Ranking", "fonts/Marker Felt.ttf", 32); //排行榜
	auto label5 = Label::createWithTTF("Play(Infinite)", "fonts/Marker Felt.ttf", 32); //无限模式

	auto item1 = MenuItemLabel::create(label1, CC_CALLBACK_1(mainScene::GameScene, this));
	auto item5 = MenuItemLabel::create(label5, CC_CALLBACK_1(mainScene::InfiniteGameScene, this));
	auto item2 = MenuItemLabel::create(label2, CC_CALLBACK_1(mainScene::TipScene, this));
	auto item3 = MenuItemLabel::create(label3, CC_CALLBACK_1(mainScene::InfoScene, this));
	auto item4 = MenuItemLabel::create(label4, CC_CALLBACK_1(mainScene::RankScene, this));
	
	auto menu = Menu::create(item1, item5, item2, item3, item4, NULL);
	menu->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	menu->setPosition(winSize.width / 2, winSize.height / 3);
	menu->alignItemsVerticallyWithPadding(label1->getContentSize().height / 2);
	addChild(menu);


	return true;
}


//jump to game page
void mainScene::GameScene(Ref *ref)
{
	
	auto scene = gameScene::createScene();
	Director::getInstance()->replaceScene(scene);
	if (Director::getInstance()->isPaused()) {
		Director::getInstance()->resume();
	}
}


//jump to Information Page
void mainScene::InfoScene(Ref *ref)
{
	this->stopAllActions();
	auto scene = infoScene::createScene();
	Director::getInstance()->replaceScene(scene);
}

//jump to tips page
void mainScene::TipScene(Ref *ref)
{
	this->stopAllActions();
	auto scene = tipScene::createScene();
	Director::getInstance()->replaceScene(scene);
}

//ranking list page
void mainScene::RankScene(Ref *ref)
{
	this->stopAllActions();
	auto scene = RankScene::createScene();
	Director::getInstance()->replaceScene(scene);
}

void mainScene::InfiniteGameScene(Ref *ref)
{
	this->stopAllActions();
	auto scene = infiniteGameScene::createScene();
	Director::getInstance()->replaceScene(scene);
}