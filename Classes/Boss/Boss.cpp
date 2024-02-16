#include "Boss.h"
#include "DefineBitmask.h"
#include "Utilities/AnimationUtils.h"
#include "DesignPattern/Observer.h"
Boss* Boss::create(EntityInfo* info)
{
	auto newObject = new Boss();
	if (newObject != nullptr && newObject->init(info))
	{
		newObject->autorelease();
		return newObject;
	}

	CC_SAFE_DELETE(newObject);
	return nullptr;
}

bool Boss::init(EntityInfo* info)
{
	if (!Entity::init(info))
	{
		log("Init Enemy failed!");
		return false;
	}
	AnimationUtils::loadSpriteFrameCache("Boss/", _info->_entityName + std::to_string(_info->_level));
	_model = Sprite::createWithSpriteFrameName("Boss" + std::to_string(_info->_level) + " (1)");
	this->addChild(_model);
	Animate* animate = Animate::create(AnimationUtils::createAnimation("Boss" +std::to_string(_info->_level), 10, 0.1f));
	_model->runAction(RepeatForever::create(animate));

	auto body = PhysicsBody::createBox(_model->getContentSize(), PhysicsMaterial(1, 0, 1));
	body->setCategoryBitmask(DefineBitmask::ENEMY);
	body->setContactTestBitmask(DefineBitmask::BULLET | DefineBitmask::SHIP);
	body->setCollisionBitmask(DefineBitmask::NON);
	this->setPhysicsBody(body);
	body->setGravityEnable(false);

	auto listener = EventListenerPhysicsContact::create();
	listener->onContactBegin = CC_CALLBACK_1(Boss::callbackOnContactBegin, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void Boss::takeDamage(int dame)
{

	//log("take dame: %d", dame);
	_healthCtrl->setCurrentHealth(_healthCtrl->getCurrentHealth() - dame);

}


void Boss::onDie()
{
	//log("die");
	// add effects....
	Observer::getInstance()->notify("BossDie", this);
	this->removeFromParentAndCleanup(true);
}

void Boss::onEnter()
{
	Node::onEnter();

	// health
	_healthCtrl = HealthController::create(_entityStat->_hp, "fill.png");
	_healthCtrl->setOnDie(CC_CALLBACK_0(Boss::onDie, this));
	_healthCtrl->setPosition(Vec2(-(_healthCtrl->getContentSize().width / 2)
		, _model->getContentSize().height - 20));
	this->addChild(_healthCtrl);
}


bool Boss::callbackOnContactBegin(PhysicsContact& contact)
{
	auto nodeA = contact.getShapeA()->getBody()->getNode();
	auto nodeB = contact.getShapeB()->getBody()->getNode();

	//neu va cham voi dan thi se tru mau
	//neu va cham voi Ship thi ko sao

	return false;
}