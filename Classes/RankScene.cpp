#include "RankScene.h"
#include "mainScene.h"
#include <fstream>
using namespace std;

#define TITLE_LABEL 1
#define RANKING_LABEL 0

RankScene::RankScene()
{
}

RankScene::~RankScene()
{
}

Scene* RankScene::createScene()
{
	Scene* scene = Scene::create();
	auto layer = RankScene::create();
	scene->addChild(layer);
	return scene;
}

RankScene* RankScene::create()
{
	RankScene* pRet = new RankScene();
	if (pRet && pRet->init())
	{
		return pRet;
	}
	pRet = NULL;
	return NULL;
}




bool RankScene::init()
{
	normaldata = "";
	infinitedata = "";
	fstream file;
	char str[100];
	//读取普通模式排名信息文件
	file.open("RankList.a", ios::in);
	if (!file) {
		file.open("RankList.a", ios::out);
		normaldata = "9\n";
		for (int i = 0; i < 10; i++) {
			file << normaldata;
		}
		file.close();
		file.open("RankList.a", ios::in);
	}
	
	for (int i = 0; i < 10; i++) {
		file.getline(str, 100);
		//char *p;
		//p = strtok(str, " ");
		normaldata = normaldata + str + '\n';
		//data.insert(pair<string, int>(temp, 100));
	}
	file.close();
	//读取无限模式排名信息文件
	file.open("RankList.b", ios::in);
	if (!file) {
		file.open("RankList.b", ios::out);
		infinitedata = "11\n";
		for (int i = 0; i < 10; i++) {
			file << infinitedata;
		}
		file.close();
		file.open("RankList.b", ios::in);
	}

	for (int i = 0; i < 10; i++) {
		file.getline(str, 100);
		infinitedata = infinitedata + str + '\n';
	}
	file.close();
	
	if (!Layer::init())
	{
		return false;
	}
	Size winSize = Director::getInstance()->getWinSize();
	dispatcher = Director::getInstance()->getEventDispatcher();
	auto origin = Director::getInstance()->getVisibleOrigin();
	//生成背景
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
	//生成内容
	auto labelPause = Label::createWithTTF("Return", "fonts/Marker Felt.ttf", 32);
	auto itemPause = MenuItemLabel::create(labelPause, CC_CALLBACK_1(RankScene::RankSceneReturn, this));
	rightTopMenu = Menu::create(itemPause, NULL);
	rightTopMenu->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	rightTopMenu->setPosition(winSize.width - 50, winSize.height - 60);
	addChild(rightTopMenu, 10);

	auto label = Label::createWithTTF(normaldata, "fonts/Marker Felt.ttf", 32);
	label->setPosition(winSize.width / 2, winSize.height / 2);
	label->setTag(RANKING_LABEL);
	addChild(label);

	auto label2 = Label::createWithTTF("Ranking(Normal)", "fonts/Marker Felt.ttf", 32);
	label2->setPosition(winSize.width / 2, winSize.height -50);
	label2->setTag(TITLE_LABEL);
	addChild(label2);

	testKeyboardEvent();  //键盘监听

	return true;
}

//键盘响应 输入左箭头或者右箭头切换页面
void RankScene::testKeyboardEvent()
{
	auto listener = EventListenerKeyboard::create();

	listener->onKeyPressed = [&](EventKeyboard::KeyCode code, Event* event){
		if (code == EventKeyboard::KeyCode::KEY_LEFT_ARROW || code == EventKeyboard::KeyCode::KEY_RIGHT_ARROW) changeRankingList();
	};


	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

//返回主菜单
void RankScene::RankSceneReturn(Ref* sender)
{
	auto scene = mainScene::createScene();
	Director::getInstance()->replaceScene(scene);
}
//切换排名列表
void RankScene::changeRankingList()
{
	
	auto sp_rank = (Label*)this->getChildByTag(RANKING_LABEL);
	auto sp_title = (Label*)this->getChildByTag(TITLE_LABEL);
	if (normal_list) {
		sp_rank->setString(infinitedata);
		sp_title->setString("Ranking(Infinite)");
		normal_list = false;
		//log("1");
	}
	else {
		sp_rank->setString(normaldata);
		sp_title->setString("Ranking(Normal)");
		normal_list = true;
		//log("0");
	}
	
}