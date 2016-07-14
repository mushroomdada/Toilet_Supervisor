#pragma once

#include "cocos2d.h"

USING_NS_CC;

class infiniteGameScene :public Layer
{
public:
	static Scene* createScene();
	static infiniteGameScene* create();
	bool init();

	//void tipSceneReturn(Ref*);

private:
	infiniteGameScene();
	~infiniteGameScene();

	//Menu* rightTopMenu;

	EventDispatcher* dispatcher;

};