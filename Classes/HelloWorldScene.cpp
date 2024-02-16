/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "HelloWorldScene.h"
#include "Utilities/AnimationUtils.h"
#include "Boss/Boss.h"
#include "Enemy/Enemy.h"
#include "Ship/Ship.h"
#include "BossRocket/BossRocket.h"
#include "Enemy_Bullet/EBullet.h"
USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::initWithPhysics() )
    {
        return false;
    }
    //this->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width/2;
        float y = origin.y + closeItem->getContentSize().height/2;
        closeItem->setPosition(Vec2(x,y));
    }

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label

    auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
    if (label == nullptr)
    {
        problemLoading("'fonts/Marker Felt.ttf'");
    }
    else
    {
        // position the label on the center of the screen
        label->setPosition(Vec2(origin.x + visibleSize.width/2,
                                origin.y + visibleSize.height - label->getContentSize().height));

        // add the label as a child to this layer
        this->addChild(label, 1);
    }

    // add "HelloWorld" splash screen"
    /*AnimationUtils::loadSpriteFrameCache("Boss/", "Boss3");
    auto sprite = Sprite::createWithSpriteFrameName("Boss3 (1)");
    sprite->setScale(1.5);
    Animate* animate = Animate::create(AnimationUtils::createAnimation("Boss3",10 , 0.1f));
    sprite->runAction(RepeatForever::create(animate));
    sprite->setVisible(false);

    auto enemyBullet = Sprite::create("Enemy_Bullet/EnemyEasy_Bullet.png");
    enemyBullet->setPosition(Vec2(150, 500));
    enemyBullet->setScale(0.3);
    this->addChild(enemyBullet);*/

    Boss* boss = Boss::create(new EntityInfo(1, "Boss"));
    boss->setPosition(Vec2(393 / 2, 700 ));
    boss->setScale(3);
    this->addChild(boss);

    auto bosspos = boss->getPosition();

    auto dan1 = Sprite::create("Enemy_Bullet/1_Bullet.png");
    auto dan2 = Sprite::create("Enemy_Bullet/1_Bullet.png");
    
    dan1->setScale(0.45);
    dan2->setScale(0.45);

    dan1->setPosition(bosspos + Vec2(28, -48));
    dan2->setPosition(bosspos + Vec2(-28, -48));
    this->addChild(dan1);
    this->addChild(dan2);

    this->schedule(CC_SCHEDULE_SELECTOR(HelloWorld::deltatime), 5.0f);

    this->scheduleOnce([this](float dt) {
        // Update the ContactTestBitmask to include DefineBitmask::FRAME
        this->unschedule(CC_SCHEDULE_SELECTOR(HelloWorld::deltatime));
        }, 5.0f, "updateContactTestBitmask");
    
    int a = random(0, 0);
    log("%d", a);
    //AnimationUtils::loadSpriteFrameCache("Explosion/", "ExplosionBoss");
    //AnimationUtils::createAnimation("ExplosionBoss", 3.0f);

    //auto explosion = Sprite::createWithSpriteFrameName("ExplosionBoss (1)");

    //explosion->setPosition(this->getPosition());
    //explosion->setScale(0.2);
    //// Run the explosion animation
    //auto animation = AnimationCache::getInstance()->getAnimation("ExplosionBoss");
    //auto animate = Animate::create(animation);
    //auto zoomInAction = ScaleTo::create(5.0f, 5.0f); // Zoom in to 1.5 times the original size
    //auto removeExplosion = CallFunc::create([explosion]() {
    //    explosion->removeFromParentAndCleanup(true);
    //    });
    //auto sequence = Sequence::create(animate,zoomInAction, removeExplosion, nullptr);
    //explosion->runAction(sequence);

    //this->addChild(explosion, 2);
    //explosion->setPosition(Vec2(150,300));

    /*auto rocket = Rocket::create("1");
    rocket->setPosition(Vec2(150, 600));
    this->addChild(rocket);*/

    /*Enemy* ship = Enemy::create(new EntityInfo(6, "EnemyHard"));
    ship->setScale(1.3);
    ship->setPosition(Vec2(240, 160));
    this->addChild(ship);
    auto shipsize = ship->getModel()->getContentSize() *1.2;
    log("chieu ngang : %f", shipsize.width);
    log("chieu doc : %f", shipsize.height);*/

    /*auto sprite2 = Sprite::create("Bullet/5.png");
    sprite2->setPosition(Vec2(300, 200));
    sprite2->setScale(0.3);
    this->addChild(sprite2);*/
    //float spacing = visibleSize.width  / 7.0f; // 7 sprites including spaces
    //log("spacing:%f", spacing);
    //float y_Postion = 400;
    //float y_Postion2 = 820;
    //for (int i = 0; i < 6; ++i)
    //{
    //    // Create sprite
    //    auto enemy = Enemy::create(new EntityInfo(4, "EnemyEasy"));
    //    enemy->setRotation(180);
    //    auto enemy2 = Enemy::create(new EntityInfo(4, "EnemyEasy"));
    //    enemy2->setRotation(180);
    //    //enemy->setScale(1.2);
    //    // Set position
    //    float xPosition = (i + 1) * spacing; // Add 1 to avoid starting from the left edge
    //    y_Postion += 60; // Center vertically
    //    y_Postion2 -= 60;
    //    enemy->setPosition(Vec2(xPosition, y_Postion));
    //    enemy2->setPosition(Vec2(xPosition, y_Postion2));

    //    //y_Postion += 50; // Center vertically
    //    //y_Postion2 -= 50;

    //    log("x%d:%f", i, xPosition);
    //    log("y%d:%f", i, y_Postion);
    //    // Add sprite to the scene
    //    this->addChild(enemy);
    //    this->addChild(enemy2);
    //}
    //if (sprite == nullptr)
    //{
    //    problemLoading("'HelloWorld.png'");
    //}
    //else
    //{
    //    // position the sprite on the center of the screen
    //    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    //    // add the sprite as a child to this layer
    //    this->addChild(sprite, 0);
    //}
    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}

void HelloWorld::callHello()
{
    
}

void HelloWorld::deltatime(float dt)
{
    numbers.clear();
    log("size : %d", int(numbers.size()));
}

