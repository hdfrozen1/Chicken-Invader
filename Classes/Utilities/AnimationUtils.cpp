#include "AnimationUtils.h"

Animation* AnimationUtils::createAnimation(std::string name, int numFrame, float duration)
{
    Vector<SpriteFrame*> frames;
    for (int i = 0; i < numFrame; ++i)
    {
        char buffer[100];
        sprintf(buffer, "%s (%d)", name.c_str(), i + 1);
        SpriteFrame* frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(buffer);
        if (frame == nullptr)
        {
            break;
        }
        frames.pushBack(frame);
    }
    if (frames.size() == 0) return nullptr;

    Animation* animation = Animation::createWithSpriteFrames(frames, duration);
    AnimationCache::getInstance()->addAnimation(animation, name);
    return animation;
}

std::pair<Animation*, int> AnimationUtils::createAnimation(std::string name, float duration)
{
    Vector<SpriteFrame*> frames;

    int numFrame = 0;
    while (true)
    {
        char buffer[100];
        sprintf(buffer, "%s (%d)", name.c_str(), numFrame + 1);
        SpriteFrame* frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(buffer);
        if (frame == nullptr)
        {
            break;
        }
        frames.pushBack(frame);
        ++numFrame;
    }

    Animation* animation = Animation::createWithSpriteFrames(frames, duration / (float)numFrame);
    AnimationCache::getInstance()->addAnimation(animation, name);
    return std::pair<Animation*, int>(animation, numFrame);
}

bool AnimationUtils::loadSpriteFrameCache(std::string path, std::string fileName)
{
    if (!FileUtils::getInstance()->isFileExist(path + fileName + ".plist") ||
        !FileUtils::getInstance()->isFileExist(path + fileName + ".png"))
    {
        return false;
    }

    SpriteFrameCache::getInstance()
        ->addSpriteFramesWithFile(path + fileName + ".plist", path + fileName + ".png");

    return true;
}