#include "GameScene.h"
#include "json/document.h"
#include "json/rapidjson.h"
#include "Bullet/Bullet.h"
#include "DesignPattern/Observer.h"
#include "DefineBitmask.h"
#include "Enemy_Bullet/EBullet.h"


void GameScene::callEnemy(float dt)
{
	log("child:%d", this->getChildren().size());
	//log("soluong:%d", soluong);
	//log("element:%d", _element);
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
				x_pos +=  _distance_x[_element];
				y_pos +=  _distance_y[_element];
				y2_pos -=  _distance_y[_element];

				MoveTo* moveto1 = MoveTo::create(1, Vec2(x_pos, y_pos));
				MoveTo* moveto2 = MoveTo::create(1, Vec2(x_pos, y2_pos));
				enemy1->runAction(moveto1);
				enemy2->runAction(moveto2);

				enemy1->takePosition(Vec2(x_pos, y_pos));
				enemy2->takePosition(Vec2(x_pos, y2_pos));

				_enemyPosition.push_back(Vec2(x_pos, y_pos));
				_enemyPosition.push_back(Vec2(x_pos, y2_pos));
				
				
				this->addChild(enemy1,2);
				this->addChild(enemy2,2);
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
	this->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

	Node* frame = Node::create();
	auto frameBody = PhysicsBody::createEdgeBox(Director::getInstance()->getVisibleSize());
	frame->setPhysicsBody(frameBody);
	frameBody->setCategoryBitmask(DefineBitmask::FRAME);
	frameBody->setContactTestBitmask(DefineBitmask::BULLET| DefineBitmask::EBULLET);
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

	_ship = Ship::create(new EntityInfo(1, "Ship"));
	_ship->setPosition(Vec2(visibleSize.width / 2, 100));

	this->addChild(_ship,2);
	
	Observer::getInstance()->registerEvent("EnemyDie", CC_CALLBACK_1(GameScene::updatequantity, this));

	this->schedule(CC_SCHEDULE_SELECTOR(GameScene::callEnemy), 1.0f);
	this->schedule(CC_SCHEDULE_SELECTOR(GameScene::EnemyAttack), 3.0f);
	this->schedule(CC_SCHEDULE_SELECTOR(GameScene::updateEnemy), 20.0f);
	
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
	return true;
}

void GameScene::updateEnemy(float dt)
{
	
	Vector<Node*> children = this->getChildren();
	for (auto child : children) {
		if (dynamic_cast<Enemy*>(child) && enemy_quantity[_element] > 1) {
			child->removeFromParent();
		}
	}
	_element += 1;
	soluong = 0;

	_enemyPosition.clear();
}

bool GameScene::onTouchBegan(Touch* touch, Event* event)
{
	if (!ispressed) {
		ispressed = true;
		delta = _ship->getPosition() - touch->getLocation();
		schedule(CC_SCHEDULE_SELECTOR(GameScene::attack), 0.2);
		return true;
	}
	return false;
	
}

void GameScene::onTouchMoved(Touch* touch, Event* event)
{
	_ship->setPosition(touch->getLocation() + delta);
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
	auto bullet = Bullet::create(_ship->getEntityInfo()->_level);
	bullet->setPosition(_ship->getPosition() + Vec2(0, _ship->getContentSize().height));
	bullet->setScale(0.3);
	this->addChild(bullet, 2);
}

void GameScene::updatequantity(void* data)
{
	int enemy_level = static_cast<Enemy*>(data)->getEntityInfo()->_level;
	int score = static_cast<Enemy*>(data)->getEntityStat()->_diem;
	Vec2 pos = static_cast<Enemy*>(data)->getEnemyPostition();

	log("size :%d", int(_enemyPosition.size()));

	auto it = std::remove(_enemyPosition.begin(), _enemyPosition.end(), pos);
	// Erase the elements from the vector
	_enemyPosition.erase(it, _enemyPosition.end());
	log("size:%d", int(_enemyPosition.size()));

	_totalscore += score;
	if (enemy_quantity[enemy_level - 1] != 1) {
		soluong -= 1;
	}
}

void GameScene::EnemyAttack(float dt)
{
	if (enemy_quantity[_element] > 1) {
		int firedquantity = random(1, 3);
		for (int i = 0; i < firedquantity; i++) {
			int a = random(0, int(_enemyPosition.size()) -1);
			Ebulletpos[_enemyPosition[a]] = _enemyPosition[a];
		}
		for (const auto& pair : Ebulletpos) {
			EBullet* enemybullet = EBullet::create(_difficulty);
			enemybullet->setPosition(pair.second);
			enemybullet->setScale(0.3);
			this->addChild(enemybullet);
		}
		Ebulletpos.clear();
	}
	else {
		return;
	}

}
