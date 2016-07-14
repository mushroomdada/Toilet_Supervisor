#pragma once

#include "cocos2d.h"

USING_NS_CC;

class mainScene :public Layer
{
public:
	static Scene* createScene();
	static mainScene* create();
	bool init();
	void GameScene(Ref *sender);
	void InfoScene(Ref *sender);
	void TipScene(Ref* sender);
	void RankScene(Ref* sender);
	void InfiniteGameScene(Ref* sender);

private:
	mainScene();
	~mainScene();

	EventDispatcher* dispatcher;

};