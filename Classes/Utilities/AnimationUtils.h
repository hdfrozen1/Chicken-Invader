#ifndef __ANIMATION_UTILS_H__
#define __ANIMATION_UTILS_H__

#include "cocos2d.h"

USING_NS_CC;

class AnimationUtils
{
public:
	static Animation* createAnimation(std::string name, int numFrame, float duration);
	static std::pair<Animation*, int> createAnimation(std::string name, float duration);
	static bool loadSpriteFrameCache(std::string path, std::string fileName);
};

#endif // !__ANIMATION_UTILS_H__