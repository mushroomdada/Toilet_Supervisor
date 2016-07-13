#pragma once

#include "cocos2d.h"

USING_NS_CC;

class mainScene :public Layer
{
public:
	static Scene* createScene();
	static mainScene* create();
	bool init();

private:
	mainScene();
	~mainScene();

	void GameScene(Ref *sender);
	void InfoScene(Ref *sender);
	void TipScene(Ref* sender);
	void RankScene(Ref* sender);
	
	EventDispatcher* dispatcher;

};
