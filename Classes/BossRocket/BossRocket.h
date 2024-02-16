#ifndef __BOSS_ROCKET_H__
#define __BOSS_ROCKET_H__

#include "cocos2d.h"
USING_NS_CC;

class Rocket :public Node {
public:
	static Rocket* create(std::string level);

	virtual bool init(std::string level);

protected:
	bool callbackOnContactBegin(PhysicsContact& contact);
private:
	Sprite* _model;
	PhysicsBody* body;
	Sprite* explosion;
	int dame = 10;
	static int loadAnimation;
	

};


#endif // !__BOSS_ROCKET_H__
