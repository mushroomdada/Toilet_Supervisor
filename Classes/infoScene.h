#pragma once

#include "cocos2d.h"

USING_NS_CC;

class infoScene :public Layer
{
public:
	static Scene* createScene();
	static infoScene* create();
	bool init();

	void infoSceneReturn(Ref*);

private:
	infoScene();
	~infoScene();

	Menu* rightTopMenu;

	EventDispatcher* dispatcher;

};
