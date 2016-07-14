
#include "cocos2d.h"
USING_NS_CC;

class Setting : public cocos2d::Layer
{
public:
	static Scene* createScene();
	static Setting* create();
	bool init();
	void OpenTips();
	void CloseTips();
	void Back();
	bool S_IsOpenTips = true;

	void VolumeMax();
	void VolumeMin();
	void VolumeNorm();
private:
	Setting();
	~Setting();
};