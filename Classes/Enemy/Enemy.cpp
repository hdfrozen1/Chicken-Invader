#include "Enemy.h"
#include "Utilities/AnimationUtils.h"
#include "DefineBitmask.h"
#include "DesignPattern/Observer.h"
int Enemy::loadAnimation = 0;
Enemy* Enemy::create(EntityInfo* info)
{
	auto newObject = new Enemy();
	if (newObject != nullptr && newObject->init(info))
	{
		newObject->autorelease();
		return newObject;
	}

	CC_SAFE_DELETE(newObject);
	return nullptr;
}

bool Enemy::init(EntityInfo* info)
{
	if (!Entity::init(info))
	{
		log("Init Enemy failed!");
		return false;
	}
	AnimationUtils::loadSpriteFrameCache(_info->_entityName + "/", _info->_entityName + std::to_string(_info->_level));
	_model = Sprite::createWithSpriteFrameName(_info->_entityName +std::to_string(_info->_level) + " (1)");
	this->addChild(_model);
	Animate* animate = Animate::create(AnimationUtils::createAnimation(_info->_entityName + std::to_string(_info->_level), 10, 0.1f));
	_model->runAction(RepeatForever::create(animate));
	
	auto body = PhysicsBody::createBox(_model->getContentSize(), PhysicsMaterial(1, 0, 1));
	body->setCategoryBitmask(DefineBitmask::ENEMY);
	body->setContactTestBitmask(DefineBitmask::BULLET | DefineBitmask::SHIP);
	body->setCollisionBitmask(DefineBitmask::NON);
	this->setPhysicsBody(body);
	body->setGravityEnable(false);

	auto listener = EventListenerPhysicsContact::create();
	listener->onContactBegin = CC_CALLBACK_1(Enemy::callbackOnContactBegin, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	
	return true;
}

void Enemy::takeDamage(int dame)
{
	//log("take dame: %d", dame);
	_healthCtrl->setCurrentHealth(_healthCtrl->getCurrentHealth() - dame);
}

Vec2 Enemy::getEnemyPostition()
{
	if (GamePos == Vec2(0, 0)) {
		return this->getPosition();
	}
	return GamePos;
}

void Enemy::takePosition(Vec2 pos)
{
	GamePos = pos;
}

void Enemy::onDie()
{

	// add effects....
	if (loadAnimation == 0) {
		AnimationUtils::loadSpriteFrameCache("Explosion/", "NormalExplosion");
		AnimationUtils::createAnimation("NormalExplosion", 0.1f);
		loadAnimation += 1;
	}

	auto explosion = Sprite::createWithSpriteFrameName("NormalExplosion (1)");

	explosion->setPosition(this->getPosition());
	explosion->setScale(0.2);

	this->getParent()->addChild(explosion, this->getLocalZOrder());

	// Run the explosion animation
	auto animation = AnimationCache::getInstance()->getAnimation("NormalExplosion");
	auto animate = Animate::create(animation);
	auto removeExplosion = CallFunc::create([explosion]() {
		explosion->removeFromParentAndCleanup(true);
		});

	auto sequence = Sequence::create(animate, removeExplosion, nullptr);
	explosion->runAction(sequence);

	Observer::getInstance()->notify("EnemyDie", this);

	this->removeFromParentAndCleanup(true);
}

void Enemy::onEnter()
{
	Node::onEnter();

	// health
	_healthCtrl = HealthController::create(_entityStat->_hp, "fill.png");
	_healthCtrl->setOnDie(CC_CALLBACK_0(Enemy::onDie, this));
	_healthCtrl->setPosition(Vec2(-_healthCtrl->getContentSize().width / 2
		, _model->getContentSize().height));
	
	this->addChild(_healthCtrl);
	_healthCtrl->setVisible(false);
}

bool Enemy::callbackOnContactBegin(PhysicsContact& contact)
{
	auto nodeA = contact.getShapeA()->getBody()->getNode();
	auto nodeB = contact.getShapeB()->getBody()->getNode();

	if (nodeA != this && nodeB != this) return false;

	auto target = (nodeA == this) ? (nodeB) : (nodeA);
	//neu target la vien dan thi thoi , con neu target la ship thi se tru di 1 mang tuong duong 10 mau
	auto damageable = dynamic_cast<IDamageable*>(target);
	if (damageable != nullptr)
	{
		damageable->takeDamage(10);
	}
	

	return false;
}