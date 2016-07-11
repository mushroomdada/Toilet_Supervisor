#include "tipScene.h"
#include "mainScene.h"
using namespace std;

#define COUNT_DOWN_FONT_SIZE 64
#define HOW_TO_PLAY_STRING "1.men and women need to go to WC,\n\n2.you should use\n  keyboard left and right arrows\n  to choose a correct room\n  according to their gender,\n\n3.but only who turned red needs to go!\n\n4.if you force the green one to go,\n  or you choose a wrong side for them,\n  you will lose the game!\n\n5.finally you will get a total time"

tipScene::tipScene()
{
}

tipScene::~tipScene()
{
}

Scene* tipScene::createScene()
{
	Scene* scene = Scene::create();
	auto layer = tipScene::create();
	scene->addChild(layer);
	return scene;
}

tipScene* tipScene::create()
{
	tipScene* pRet = new tipScene();
	if (pRet && pRet->init())
	{
		return pRet;
	}
	pRet = NULL;
	return NULL;
}

bool tipScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Size winSize = Director::getInstance()->getWinSize();
	dispatcher = Director::getInstance()->getEventDispatcher();
	auto origin = Director::getInstance()->getVisibleOrigin();

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


	auto labelPause = Label::createWithTTF("Return", "fonts/Marker Felt.ttf", 32);
	auto itemPause = MenuItemLabel::create(labelPause, CC_CALLBACK_1(tipScene::tipSceneReturn, this));

	rightTopMenu = Menu::create(itemPause, NULL);
	rightTopMenu->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	rightTopMenu->setPosition(winSize.width - 50, winSize.height - 60);
	addChild(rightTopMenu, 10);


	
	auto label = Label::createWithTTF(HOW_TO_PLAY_STRING, "fonts/Marker Felt.ttf", 32);
	label->setPosition(winSize.width / 2, winSize.height / 2);
	addChild(label);



	return true;
}


void tipScene::tipSceneReturn(Ref* sender)
{
	auto scene = mainScene::createScene();
	Director::getInstance()->replaceScene(scene);
}