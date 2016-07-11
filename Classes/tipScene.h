#pragma once

#include "cocos2d.h"

USING_NS_CC;

class tipScene :public Layer
{
public:
	static Scene* createScene();
	static tipScene* create();
	bool init();

	void tipSceneReturn(Ref*);

private:
	tipScene();
	~tipScene();

	Menu* rightTopMenu;

	EventDispatcher* dispatcher;

};