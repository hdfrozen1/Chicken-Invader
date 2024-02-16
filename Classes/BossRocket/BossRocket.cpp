#include "BossRocket.h"
#include "IDamageable.h"
#include "DefineBitmask.h"
#include "Utilities/AnimationUtils.h"
int Rocket::loadAnimation = 0;
Rocket* Rocket::create(std::string level)
{
	auto newObject = new Rocket();
	if (newObject != nullptr && newObject->init(level))
	{
		newObject->autorelease();
		return newObject;
	}

	CC_SAFE_DELETE(newObject);
	return nullptr;
}

bool Rocket::init(std::string level)
{
	if (!Node::init())
	{
		log("init Bullet failed!");
		return false;
	}
	if (loadAnimation == 0) {
		AnimationUtils::loadSpriteFrameCache("BossRocket/", "Rocket" + level);
		AnimationUtils::createAnimation("Rocket" + level, 1.0f);

		AnimationUtils::loadSpriteFrameCache("Explosion/", "ExplosionBoss");
		AnimationUtils::createAnimation("ExplosionBoss", 1.0f);
		loadAnimation += 1;
	}

	
	_model = Sprite::createWithSpriteFrameName("Rocket" + level + " (1)");
	this->addChild(_model);
	auto animation = AnimationCache::getInstance()->getAnimation("Rocket"+level);
	Animate* animate = Animate::create(animation);
	_model->runAction(RepeatForever::create(animate));
	auto moveby = MoveBy::create(3, Vec2(0, random(-500,-300)));

	// Create a callback to handle the explosion after _model finishes moving
	auto explosionCallback = CallFunc::create([this]() {
		// Load Explosion sprite frames
		_model->removeFromParentAndCleanup(true);
		

		// Create and add explosion sprite
		explosion = Sprite::createWithSpriteFrameName("ExplosionBoss (1)");
		this->addChild(explosion);
		body = PhysicsBody::createCircle(explosion->getContentSize().width/2, PhysicsMaterial(1, 0, 1));
		body->setCategoryBitmask(DefineBitmask::EBULLET);
		body->setContactTestBitmask(DefineBitmask::SHIP);
		body->setCollisionBitmask(DefineBitmask::NON);
		body->setDynamic(false);
		this->setPhysicsBody(body);
		auto animation2 = AnimationCache::getInstance()->getAnimation("ExplosionBoss");
		// Create animation for explosion
		Animate* animate2 = Animate::create(animation2);

		// Run the explosion animation
		explosion->runAction(Sequence::create(
			animate2,
			// Call the scaleAndRemove method after the explosion animation
			CallFunc::create([this]() {
				// Scale "this" to 5 times in 5 seconds
				this->runAction(Sequence::create(
					ScaleTo::create(5.0f, 5.0f),
					// Remove "this" from the parent after scaling
					RemoveSelf::create(),
					nullptr
				));
				}),
			nullptr
		));
		});

	// Run the sequence of actions: Move _model and then perform the explosion callback
	this->runAction(Sequence::create(moveby, explosionCallback, nullptr));

	auto listener = EventListenerPhysicsContact::create();
	listener->onContactBegin = CC_CALLBACK_1(Rocket::callbackOnContactBegin, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

bool Rocket::callbackOnContactBegin(PhysicsContact& contact)
{
	auto nodeA = contact.getShapeA()->getBody()->getNode();
	auto nodeB = contact.getShapeB()->getBody()->getNode();
	if (nodeA != this && nodeB != this) return false;

	auto target = (nodeA == this) ? (nodeB) : (nodeA);

	auto damageable = dynamic_cast<IDamageable*>(target);
	if (damageable != nullptr)
	{
		damageable->takeDamage(dame);
	}

	this->removeFromParentAndCleanup(true);

	return false;
}
