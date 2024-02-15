#include "EBullet.h"
#include "IDamageable.h"
#include "DefineBitmask.h"
EBullet* EBullet::create(std::string level)
{
	auto newObject = new EBullet();
	if (newObject != nullptr && newObject->init(level))
	{
		newObject->autorelease();
		return newObject;
	}

	CC_SAFE_DELETE(newObject);
	return nullptr;
}

bool EBullet::init(std::string level)
{
	if (!Node::init())
	{
		log("init Bullet failed!");
		return false;
	}

	_model = Sprite::create("Enemy_Bullet/" + level + "_Bullet.png");
	this->addChild(_model);

	PhysicsBody* body = PhysicsBody::createCircle(_model->getContentSize().width / 2, PhysicsMaterial(1, 0, 1));
	body->setCategoryBitmask(DefineBitmask::EBULLET);
	body->setContactTestBitmask(DefineBitmask::SHIP|DefineBitmask::FRAME);
	body->setCollisionBitmask(DefineBitmask::NON);

	this->setPhysicsBody(body);
	body->setVelocity(Vec2(0, -500));

	auto listener = EventListenerPhysicsContact::create();
	listener->onContactBegin = CC_CALLBACK_1(EBullet::callbackOnContactBegin, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}


bool EBullet::callbackOnContactBegin(PhysicsContact& contact)
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
