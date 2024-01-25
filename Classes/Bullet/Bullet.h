#ifndef __BULLET_H__
#define __BULLET_H__

#include "cocos2d.h"

USING_NS_CC;

class Bullet : public Node
{
public:
	static Bullet* create(int character_level);

	virtual bool init(int character_level);

protected:
	bool callbackOnContactBegin(PhysicsContact& contact);
private:
	Sprite* _model;
	int dame;
	
};

#endif // !__BULLET_H__