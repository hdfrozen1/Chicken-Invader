#include "Bullet.h"
#include "IDamageable.h"
#include "DefineBitmask.h"
Bullet* Bullet::create(int character_level)
{
	auto newObject = new Bullet();
	if (newObject != nullptr && newObject->init(character_level))
	{
		newObject->autorelease();
		return newObject;
	}

	CC_SAFE_DELETE(newObject);
	return nullptr;
}

bool Bullet::init(int character_level)
{
	if (!Node::init())
	{
		log("init Bullet failed!");
		return false;
	}

	_model = Sprite::create("Bullet/" + std::to_string(character_level) + ".png");
	this->addChild(_model);
	dame = 10 + (character_level - 1) * 5;

	auto body = PhysicsBody::createEdgeBox(_model->getContentSize(), PhysicsMaterial(1, 0, 1), 1.0f);
	body->setCategoryBitmask(DefineBitmask::BULLET);
	body->setContactTestBitmask(DefineBitmask::ENEMY|DefineBitmask::FRAME);
	body->setCollisionBitmask(DefineBitmask::NON);
	body->setVelocity(Vec2(0, 1000));
	this->setPhysicsBody(body);

	auto listener = EventListenerPhysicsContact::create();
	listener->onContactBegin = CC_CALLBACK_1(Bullet::callbackOnContactBegin, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

bool Bullet::callbackOnContactBegin(PhysicsContact& contact)
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