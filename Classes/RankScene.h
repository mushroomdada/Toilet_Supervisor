#pragma once

#include "cocos2d.h"

USING_NS_CC;

class RankScene :public Layer
{
public:
	static Scene* createScene();
	static RankScene* create();
	bool init();

	void RankSceneReturn(Ref*);
	void testKeyboardEvent();
	void changeRankingList();

private:
	RankScene();
	~RankScene();
	bool normal_list = true;
	//map<string, int> data;
	Menu* rightTopMenu;

	EventDispatcher* dispatcher;

};