#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "Enemy/Enemy.h"
#include "Boss/Boss.h"
#include "Ship/Ship.h"
#include "vector"
#include "cocos2d.h"
#include "map"
USING_NS_CC;

class GameScene :public Scene {
private:
	MenuItemImage* _pause;
	Sprite* _live;
	std::vector<int> enemy_quantity;
	int _totalscore = 0, _element = 0;
	std::vector<float>  _dest_x;
	std::vector<float>  _dest_y2;
	std::vector<float>  _dest_y;
	std::vector<float>  _distance_x;
	std::vector<float>  _distance_y;
	std::vector<Vec2> _enemyPosition;
	//std::vector<Enemy*> _enemies;
	int soluong;
	std::string _difficulty;
	Ship* _ship;
	Boss* _boss;
	bool ispressed = false;
	Vec2 delta;
	std::map<Vec2, Vec2> Ebulletpos;
public:
	void callEnemy(float dt);
	static Scene* create(std::string level, int BossLevel);
	virtual bool init(std::string level, int BossLevel);
	void updateEnemy(float dt);
private:
	bool onTouchBegan(Touch* touch, Event* event);
	void onTouchMoved(Touch* touch, Event* event);
	void onTouchEnded(Touch* touch, Event* event);
	void attack(float dt);
	void updatequantity(void* data);
	void EnemyAttack(float dt);
};

#endif // !__GAME_SCENE_H__
