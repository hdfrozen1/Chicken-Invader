#include "Ship.h"
#include "Utilities/AnimationUtils.h"
#include "DefineBitmask.h"
#include "DesignPattern/Observer.h"

Ship* Ship::create(EntityInfo* info)
{
	auto newObject = new Ship();
	if (newObject != nullptr && newObject->init(info))
	{
		newObject->autorelease();
		return newObject;
	}

	CC_SAFE_DELETE(newObject);
	return nullptr;
}

bool Ship::init(EntityInfo* info)
{
	if (!Entity::init(info))
	{
		log("Init Enemy failed!");
		return false;
	}
	AnimationUtils::loadSpriteFrameCache("Ship/", "Ship_idel");
	_model = Sprite::createWithSpriteFrameName("Ship_idel (1)");
	Animate* animate = Animate::create(AnimationUtils::createAnimation("Ship_idel", 10, 0.1f));
	_model->runAction(RepeatForever::create(animate));
	this->addChild(_model);

	auto body = PhysicsBody::createBox(_model->getContentSize(), PhysicsMaterial(1, 0, 1));
	body->setCategoryBitmask(DefineBitmask::SHIP);
	body->setCollisionBitmask(DefineBitmask::NON);
	body->setContactTestBitmask(DefineBitmask::ENEMY | DefineBitmask::GIFT| DefineBitmask::EBULLET);
	this->setPhysicsBody(body);
	body->setGravityEnable(false);

	auto listener = EventListenerPhysicsContact::create();
	listener->onContactBegin = CC_CALLBACK_1(Ship::callbackOnContactBegin, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void Ship::takeDamage(int dame)
{
	log("take dame: %d", dame);
	
	float maxhealth = _healthCtrl->getMaxHealth();
	float newhealth = _healthCtrl->getCurrentHealth() - dame;
	if (newhealth <= maxhealth) {
		_healthCtrl->setCurrentHealth(_healthCtrl->getCurrentHealth() - dame);
		_dame = dame;
		Observer::getInstance()->notify("ShipTakeDame", this);
	}
}


void Ship::onDie()
{
	//log("die");
	// add effects....
	this->removeFromParentAndCleanup(true);
}

void Ship::onEnter()
{
	Node::onEnter();

	// health
	_healthCtrl = HealthController::create(_entityStat->_hp, "fill.png");
	_healthCtrl->setOnDie(CC_CALLBACK_0(Ship::onDie, this));
	_healthCtrl->setPosition(Vec2(-_healthCtrl->getContentSize().width / 2
		, _model->getContentSize().height));
	this->addChild(_healthCtrl);
	_healthCtrl->setVisible(false);
}

bool Ship::callbackOnContactBegin(PhysicsContact& contact)
{
	auto nodeA = contact.getShapeA()->getBody()->getNode();
	auto nodeB = contact.getShapeB()->getBody()->getNode();

	
	if (nodeA != this && nodeB != this) return false;

	auto target = (nodeA == this) ? (nodeB) : (nodeA);

	// neu target la enemy's bullet thi thoi , con neu target la enemy thi cho enemy chet luon
	auto damageable = dynamic_cast<IDamageable*>(target);
	if (damageable != nullptr)
	{
		damageable->takeDamage(10000);
	}

	return false;
}