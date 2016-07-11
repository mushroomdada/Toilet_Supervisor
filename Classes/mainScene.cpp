#include "mainScene.h"
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



	return true;
}
