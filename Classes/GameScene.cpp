#include "GameScene.h"
#include "json/document.h"
#include "json/rapidjson.h"
#include "Bullet/Bullet.h"
#include "DesignPattern/Observer.h"
#include "DefineBitmask.h"
#include "Enemy_Bullet/EBullet.h"
#include <algorithm>
#include "Gift/Gift.h"
#include "BossRocket/BossRocket.h"

void GameScene::callEnemy(float dt)
{
	second += dt;
	
	if (soluong <= 0 ) {
		
		if (enemy_quantity[_element] == 1) {
			Enemy* enemy = Enemy::create(new EntityInfo(_element + 1, _difficulty));
			MoveBy* moveby;
			Vec2 generate_pos;
			if (_dest_x[_element] == -2) {
				generate_pos = Vec2(430, random(500, 830));
			}
			else {
				generate_pos = Vec2(random(30,360), 950);
			}
			enemy->setPosition(generate_pos);
			moveby = MoveBy::create(5, Vec2(generate_pos.x * _dest_x[_element], generate_pos.y  *_dest_y[_element]));
			/*enemy->runAction(moveby);
			enemy->setRotation(180);
			this->addChild(enemy,2);*/

			auto removeEnemy = CallFunc::create([this, enemy]() {
				enemy->removeFromParentAndCleanup(true);
				});

			// Create a sequence of actions
			auto sequence = Sequence::create(moveby, removeEnemy, nullptr);

			// Run the sequence on the enemy
			enemy->runAction(sequence);

			enemy->setRotation(180);
			this->addChild(enemy, 2);
		}
		else {
			if (second < 19) {
				soluong = enemy_quantity[_element] * 2;
				float x_pos = 0;
				float y_pos = _dest_y[_element];
				float y2_pos = _dest_y2[_element];
				for (int i = 0; i < enemy_quantity[_element]; i++) {
					Enemy* enemy1 = Enemy::create(new EntityInfo(_element + 1, _difficulty));
					Enemy* enemy2 = Enemy::create(new EntityInfo(_element + 1, _difficulty));
					Vec2 leftside = Vec2(random(-100, -50), random(600, 900));
					Vec2 rightside = Vec2(random(450, 500), random(600, 900));
					enemy1->setPosition(leftside);
					enemy2->setPosition(rightside);
					enemy1->setRotation(180);
					enemy2->setRotation(180);
					x_pos += _distance_x[_element];
					y_pos += _distance_y[_element];
					y2_pos -= _distance_y[_element];

					MoveTo* moveto1 = MoveTo::create(1, Vec2(x_pos, y_pos));
					MoveTo* moveto2 = MoveTo::create(1, Vec2(x_pos, y2_pos));
					enemy1->runAction(moveto1);
					enemy2->runAction(moveto2);

					enemy1->takePosition(Vec2(x_pos, y_pos));
					enemy2->takePosition(Vec2(x_pos, y2_pos));

					/*_enemyPosition.push_back(Vec2(x_pos, y_pos));
					_enemyPosition.push_back(Vec2(x_pos, y2_pos));*/

					_enemies.push_back(enemy1);
					_enemies.push_back(enemy2);

					this->addChild(enemy1, 2);
					this->addChild(enemy2, 2);
				}
			}
		}
	}
	else {
		return;
	}
	

}

Scene* GameScene::create(std::string level,int BossLevel)
{
	auto newObject = new GameScene();
	if (newObject != nullptr && newObject->init(level, BossLevel))
	{
		newObject->autorelease();
		return newObject;
	}

	CC_SAFE_DELETE(newObject);
	return nullptr;
}

bool GameScene::init(std::string level, int BossLevel)
{
	if (!Scene::initWithPhysics()) {
		return false;
	}
	boss_level = BossLevel;
	//this->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	visibleSize = Director::getInstance()->getVisibleSize();
	Node* frame = Node::create();
	auto frameBody = PhysicsBody::createEdgeBox(Director::getInstance()->getVisibleSize());
	frame->setPhysicsBody(frameBody);
	frameBody->setCategoryBitmask(DefineBitmask::FRAME);
	frameBody->setContactTestBitmask(DefineBitmask::BULLET | DefineBitmask::EBULLET);
	frame->setPosition(Director::getInstance()->getVisibleSize() / 2);
	this->addChild(frame);


	Sprite* background = Sprite::create("BackGround/BG" + level + ".png");
	background->setPosition(Vec2(Director::getInstance()->getVisibleSize() / 2));
	this->addChild(background, -1);

	rapidjson::Document docs;

	std::string dataContent = cocos2d::FileUtils::getInstance()->getStringFromFile("Data/Maps/MapEnemy" + level + ".json");
	docs.Parse(dataContent.c_str());
	for (int i = 0; i < 6; i++) {
		enemy_quantity.push_back(docs["enemyquantity"][i].GetInt());
		_dest_x.push_back(docs["Dest_x"][i].GetFloat());
		_dest_y.push_back(docs["Dest_y"][i].GetFloat());
		_dest_y2.push_back(docs["Dest_y2"][i].GetFloat());
		_distance_x.push_back(docs["Distance_x"][i].GetFloat());
		_distance_y.push_back(docs["Distance_y"][i].GetFloat());
	}
	soluong = 0;

	

	Size visibleSize = Director::getInstance()->getVisibleSize();
	this->_difficulty = "Enemy" + level;

	_ship = Ship::create(new EntityInfo(2, "Ship"));
	_ship->setPosition(Vec2(visibleSize.width / 2, 100));
	
	this->addChild(_ship,2);

	_shiplife = (_ship->getEntityStat()->_hp) / 10;
	_mang = Label::createWithTTF("life:" + std::to_string(_shiplife), "fonts/Marker Felt.ttf", 24);
	_mang->setPosition(Vec2(50, 50));
	_mang->setOpacity(100);
	this->addChild(_mang, 2);

	_thongbao = Label::createWithTTF("Wave :"+std::to_string(_element + 1) + "/6", "fonts/Marker Felt.ttf", 24);
	_thongbao->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(_thongbao, 2);
	_thongbao->setOpacity(100);
	auto fadeOutAction = FadeTo::create(2.0f, 0);  // 2.0f is the duration in seconds, you can adjust it

	// Create a CallFunc to perform an action after the fade-out is complete
	auto callFunc = CallFunc::create([this]() {
		// Action completed, you can add more code here if needed
		// For example, you might want to remove the label from the scene
		this->removeChild(_thongbao);
		});

	// Run the sequence of actions
	_thongbao->runAction(Sequence::create(fadeOutAction, callFunc, nullptr));

	Observer::getInstance()->registerEvent("EnemyDie", CC_CALLBACK_1(GameScene::updatequantity, this));
	Observer::getInstance()->registerEvent("AddBullet", CC_CALLBACK_0(GameScene::addBullet, this));
	Observer::getInstance()->registerEvent("ShipTakeDame", CC_CALLBACK_1(GameScene::changeLife, this));


	this->schedule(CC_SCHEDULE_SELECTOR(GameScene::callEnemy), 1.0f);
	this->schedule(CC_SCHEDULE_SELECTOR(GameScene::EnemyAttack), 3.0f);
	this->schedule(CC_SCHEDULE_SELECTOR(GameScene::updateEnemy), 20.0f);
	//this->schedule(CC_SCHEDULE_SELECTOR(GameScene::callRandomGift), 15.0f);
	
	this->scheduleOnce([this](float dt) {
		// Update the ContactTestBitmask to include DefineBitmask::FRAME
		
		this->unschedule(CC_SCHEDULE_SELECTOR(GameScene::callEnemy));
		this->unschedule(CC_SCHEDULE_SELECTOR(GameScene::EnemyAttack));
		callBoss();
		this->schedule(CC_SCHEDULE_SELECTOR(GameScene::callrandomAttack), 5.0f);
		// Schedule another callback to unschedule updateEnemy after 2 seconds
		this->scheduleOnce([this](float dt) {
			this->unschedule(CC_SCHEDULE_SELECTOR(GameScene::updateEnemy));
			}, 2.0f, "unscheduleUpdateEnemy");
		}, 119.0f, "callBoss");

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
	return true;
}

void GameScene::updateEnemy(float dt)
{
	second = 0;
	Vector<Node*> children = this->getChildren();
	for (auto child : children) {
		if (dynamic_cast<Enemy*>(child) && enemy_quantity[_element] > 1) {
			child->removeFromParent();
		}
	}
	_element += 1;
	soluong = 0;
	_enemies.clear();

	if (_element < 7) {
		_thongbao = Label::createWithTTF("Wave :" + std::to_string(_element + 1) + "/6", "fonts/Marker Felt.ttf", 24);
		_thongbao->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
		this->addChild(_thongbao, 2);
		_thongbao->setOpacity(100);
		auto fadeOutAction = FadeTo::create(2.0f, 0);  // 2.0f is the duration in seconds, you can adjust it

		// Create a CallFunc to perform an action after the fade-out is complete
		auto callFunc = CallFunc::create([this]() {
			// Action completed, you can add more code here if needed
			// For example, you might want to remove the label from the scene
			this->removeChild(_thongbao);
			});

		// Run the sequence of actions
		_thongbao->runAction(Sequence::create(fadeOutAction, callFunc, nullptr));
	}
	

}

bool GameScene::onTouchBegan(Touch* touch, Event* event)
{
	if (!ispressed) {
		ispressed = true;
		attack(20);
		delta = _ship->getPosition() - touch->getLocation();
		schedule(CC_SCHEDULE_SELECTOR(GameScene::attack), 0.2);
		return true;
	}
	return false;
	
}

void GameScene::onTouchMoved(Touch* touch, Event* event)
{
	newPosition = touch->getLocation() + delta;
	

	// Assuming _ship is the sprite representing your ship
	halfShipWidth = _ship->getModel()->getContentSize().width / 2;
	halfShipHeight = _ship->getModel()->getContentSize().height / 2;

	newPosition.x = clampf(newPosition.x, halfShipWidth, visibleSize.width - halfShipWidth);
	newPosition.y = clampf(newPosition.y, halfShipHeight, visibleSize.height - halfShipHeight);

	_ship->setPosition(newPosition);
	return;
}

void GameScene::onTouchEnded(Touch* touch, Event* event)
{
	unschedule(CC_SCHEDULE_SELECTOR(GameScene::attack));
	if (ispressed) {
		ispressed = false;
	}
}

void GameScene::attack(float dt)
{
	for (int i = 0; i < defaultbullet; i++) {
		auto bullet = Bullet::create(_ship->getEntityInfo()->_level);
		bullet->setPosition(_ship->getPosition() + bulletbegin[i]);
		bullet->setScale(0.3);
		this->addChild(bullet, 2);
	}
}

void GameScene::updatequantity(void* data)
{
	int enemy_level = static_cast<Enemy*>(data)->getEntityInfo()->_level;
	int score = static_cast<Enemy*>(data)->getEntityStat()->_diem;
	Vec2 pos = static_cast<Enemy*>(data)->getEnemyPostition();
	if (enemy_quantity[enemy_level - 1] != 1) {
		auto it = std::find_if(_enemies.begin(), _enemies.end(), [pos](Enemy* enemy) {
			return enemy->getEnemyPostition() == pos;
			});
		Enemy* obj = *it;
		_enemies.erase(it);
		soluong -= 1;
	}
	// Erase the elements from the vector

	_totalscore += score;
}

void GameScene::EnemyAttack(float dt)
{
	if (enemy_quantity[_element] > 1) {
		int firedquantity = random(1, 3);
		for (int i = 0; i < firedquantity; i++) {
			int max = int(_enemies.size()) - 1;
			if (max < 0) {
				return;
			}
			int a = random(0, max);
			Vec2 _realPos = _enemies[a]->getPosition();
			Vec2 _convetpos = Vec2((round(_realPos.x * 100)) / 100, (round(_realPos.y * 100)) / 100);
			Vec2 _EPos = _enemies[a]->getEnemyPostition();
			Vec2 _EconvertPos = Vec2((round(_EPos.x * 100)) / 100, (round(_EPos.y * 100)) / 100);

			if (_convetpos == _EconvertPos) {
				Ebulletpos[_EPos] = _EPos;
			}
		}
		for (const auto& pair : Ebulletpos) {
			EBullet* enemybullet = EBullet::create(_difficulty);
			enemybullet->setPosition(pair.second);
			enemybullet->setScale(0.3);
			this->addChild(enemybullet);
		}
	}
	Ebulletpos.clear();
}

void GameScene::addBullet()
{
	if (defaultbullet < 3) {
		defaultbullet += 1;
	}
}

void GameScene::callRandomGift(float dt)
{
	int randomtime = random(0, 5);
	delta_time += randomtime;

	auto delay = cocos2d::DelayTime::create(delta_time);

	// Create a function call action to execute your gift appearance logic
	auto callFunc = cocos2d::CallFunc::create([this]() {
		Gift* gift = Gift::create();// Replace spawnGift with your actual method to create and show the gift
		gift->setScale(0.25);
		this->addChild(gift,2);
		});

	// Create a sequence of actions: delay followed by the function call
	auto sequence = cocos2d::Sequence::create(delay, callFunc, nullptr);

	// Run the sequence on the current node or on the desired target
	this->runAction(sequence);
}

void GameScene::callBoss()
{
	_thongbao = Label::createWithTTF("Boss ", "fonts/Marker Felt.ttf", 24);
	_thongbao->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(_thongbao, 2);
	_thongbao->setOpacity(100);
	auto fadeOutAction = FadeTo::create(2.0f, 0);  // 2.0f is the duration in seconds, you can adjust it

	// Create a CallFunc to perform an action after the fade-out is complete
	auto callFunc = CallFunc::create([this]() {
		// Action completed, you can add more code here if needed
		// For example, you might want to remove the label from the scene
		this->removeChild(_thongbao);
		});

	// Run the sequence of actions
	_thongbao->runAction(Sequence::create(fadeOutAction, callFunc, nullptr));

	_boss = Boss::create(new EntityInfo(boss_level, "Boss"));
	MoveTo* bossto = MoveTo::create(2.0f, Vec2(visibleSize.width / 2, 700));
	_boss->setPosition(Vec2(150, 444));
	_boss->setScale(3.0f);
	this->addChild(_boss, 2);

	MoveTo* moveby1 = MoveTo::create(2.5f, Vec2(300, 700));
	MoveTo* moveby2 = MoveTo::create(2.5f, Vec2(100, 700));

	auto sequence = Sequence::create(moveby1, moveby2, nullptr);
	auto repeatForever = RepeatForever::create(sequence);
	_boss->runAction(repeatForever);
}

void GameScene::changeLife(void* data)
{
	_shiplife -= (static_cast<Ship*>(data)->getdame()) / 10;
	_mang->setString("life:" + std::to_string(_shiplife));

}

void GameScene::rocketAttack()
{
	Vec2 bosspos = _boss->getPosition();
	Rocket* rocket = Rocket::create(std::to_string(boss_level));
	rocket->setPosition(bosspos);
}

void GameScene::bulletAttack()
{
	Vec2 bosspos = _boss->getPosition();
	EBullet* bossbullet1 = EBullet::create(std::to_string(boss_level));
	bossbullet1->setScale(scales[boss_level-1]);
	bossbullet1->setPosition(bosspos + firstbosspos[boss_level - 1]);
	this->addChild(bossbullet1);

	EBullet* bossbullet2 = EBullet::create(std::to_string(boss_level));
	bossbullet2->setScale(scales[boss_level - 1]);
	bossbullet2->setPosition(bosspos + secondbosspos[boss_level - 1]);
	this->addChild(bossbullet2);
}

void GameScene::callrandomAttack(float dt)
{
	int a = random(1, 2);
	if (a == 1) {
		bulletAttack();
	}
	else {
		rocketAttack();
	}
}
