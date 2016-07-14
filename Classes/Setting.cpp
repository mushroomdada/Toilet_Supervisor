#include "mainScene.h"
#include "Setting.h"
#include "SimpleAudioEngine.h"  
#include "gameScene.h"
 
using namespace CocosDenshion;
using namespace std;
USING_NS_CC;

Setting::Setting()
{
}

Setting::~Setting()
{
}

Scene* Setting::createScene()
{
	Scene* scene = Scene::create();
	auto layer = Setting::create();
	scene->addChild(layer);
	return scene;
}

Setting* Setting::create()
{
	Setting* pRet = new Setting();
	if (pRet && pRet->init())
	{
		return pRet;
	}
	pRet = NULL;
	return NULL;
}

bool Setting::init()
{
	if (!Layer::init())
	{
		return false;
	}
	Size winSize = Director::getInstance()->getWinSize();
	auto background = Sprite::create("picture/mainPageBackground.png");
	background->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	background->setPosition(winSize.width / 2, winSize.height / 2);
	addChild(background);

	auto label1 = Label::createWithTTF("Open Tips", "fonts/Marker Felt.ttf", 32);
	auto label2 = Label::createWithTTF("Close Tips", "fonts/Marker Felt.ttf", 32);
	auto label6 = Label::createWithTTF("Maximum volume", "fonts/Marker Felt.ttf", 32);
	auto label4 = Label::createWithTTF("Normal volume", "fonts/Marker Felt.ttf", 32);
	auto label5 = Label::createWithTTF("Minimum volume", "fonts/Marker Felt.ttf", 32);
	auto label3 = Label::createWithTTF("Back", "fonts/Marker Felt.ttf", 32);
	auto item1 = MenuItemLabel::create(label1, CC_CALLBACK_0(Setting::OpenTips, this));
	auto item2 = MenuItemLabel::create(label2, CC_CALLBACK_0(Setting::CloseTips, this));
	auto item3 = MenuItemLabel::create(label3, CC_CALLBACK_0(Setting::Back, this));
	auto item4 = MenuItemLabel::create(label4, CC_CALLBACK_0(Setting::VolumeNorm, this));
	auto item5 = MenuItemLabel::create(label5, CC_CALLBACK_0(Setting::VolumeMin, this));
	auto item6 = MenuItemLabel::create(label6, CC_CALLBACK_0(Setting::VolumeMax, this));
	auto menu = Menu::create(item1, item2,item6,item4,item5, item3, NULL);
	menu->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	menu->setPosition(winSize.width / 2, winSize.height / 3);
	menu->alignItemsVerticallyWithPadding(label1->getContentSize().height / 2);
	addChild(menu);

	
	return true;
}

void Setting::OpenTips()
{
	S_IsOpenTips = true;
}

void Setting::CloseTips()
{
	S_IsOpenTips = false;
}

void Setting::Back()
{
	Scene* scene = Scene::create();
	auto layer = mainScene::create();
	layer->IsOpenTips = S_IsOpenTips;
	scene->addChild(layer);
	Director::getInstance()->replaceScene(scene);
	if (Director::getInstance()->isPaused()) {
		Director::getInstance()->resume();
	}
}



void Setting::VolumeMax()
{
	SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(1);
}
void Setting::VolumeMin()
{
	SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0.2);
}
void Setting::VolumeNorm()
{
	SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0.6);
}