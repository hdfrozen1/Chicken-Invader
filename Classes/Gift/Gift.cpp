#include "Gift.h"
#include "DefineBitmask.h"
#include "Ship/Ship.h"
#include "DesignPattern/Observer.h"
Gift* Gift::create()
{
	auto newObject = new Gift();
	if (newObject != nullptr && newObject->init())
	{
		newObject->autorelease();
		return newObject;
	}

	CC_SAFE_DELETE(newObject);
	return nullptr;
}

bool Gift::init()
{
	if (!Node::init())
	{
		log("init Bullet failed!");
		return false;
	}
	random_number = random(1,3);
	_gift = Sprite::create("Gift/" + std::to_string(random_number) + ".png");
	this->addChild(_gift);

	auto body = PhysicsBody::createEdgeBox(_gift->getContentSize(), PhysicsMaterial(1, 0, 1), 1.0f);
	body->setCategoryBitmask(DefineBitmask::GIFT);
	body->setContactTestBitmask(DefineBitmask::SHIP);
	body->setCollisionBitmask(DefineBitmask::NON);
	body->setVelocity(Vec2(0, -100));
	this->setPhysicsBody(body);
	this->setPosition(Vec2(random(100, 350), 900));
	


	auto listener = EventListenerPhysicsContact::create();
	listener->onContactBegin = CC_CALLBACK_1(Gift::callbackOnContactBegin, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	this->scheduleOnce([this](float dt) {
		// Update the ContactTestBitmask to include DefineBitmask::FRAME
		this->removeFromParentAndCleanup(true);
		}, 9.0f, "updateContactTestBitmask");
	return true;
}

bool Gift::callbackOnContactBegin(PhysicsContact& contact)
{
	auto nodeA = contact.getShapeA()->getBody()->getNode();
	auto nodeB = contact.getShapeB()->getBody()->getNode();
	if (nodeA != this && nodeB != this) return false;

	auto target = (nodeA == this) ? (nodeB) : (nodeA);

	auto theship = static_cast<Ship*>(target);
	if (theship != nullptr) {
		if (random_number == 1 && theship->getEntityInfo()->_level < 5) {
			theship->setLevel(theship->getEntityInfo()->_level + 1);
		}
		else if (random_number == 3) {
			theship->takeDamage(-10);
		}
		else if(random_number == 2) {
			Observer::getInstance()->notify("AddBullet", this);
		}
	}
	
	this->removeFromParentAndCleanup(true);
	return false;
}


