#ifndef __E_BULLET_H__
#define __E_BULLET_H__
#include "cocos2d.h"
USING_NS_CC;

class EBullet :public Node {
public:
	static EBullet* create(std::string level);

	virtual bool init(std::string level);

protected:
	bool callbackOnContactBegin(PhysicsContact& contact);
private:
	Sprite* _model;
	int dame = 10;

};
#endif // !__E_BULLET_H__
