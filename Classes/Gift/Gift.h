#ifndef __GIFT_H__
#define __GIFT_H__

#include "cocos2d.h"
USING_NS_CC;

class Gift :public Node {
public:
	static Gift* create();
	bool init();
protected:
	bool callbackOnContactBegin(PhysicsContact& contact);
private:
	int random_number;
	int delta_time = 0;
	Sprite* _gift;
	void callRandomGift(float dt);
};
#endif // !__GIFT_H__
