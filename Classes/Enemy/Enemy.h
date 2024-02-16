#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "Entity/Entity.h"
#include "IDamageable.h"
#include "Health/HealthController.h"

class Enemy : public Entity, public IDamageable
{
public:
	
	static Enemy* create(EntityInfo* info);
	virtual bool init(EntityInfo* info) override;
	void takeDamage(int dame) override;
	Vec2 getEnemyPostition();
	void takePosition(Vec2 pos = Vec2(0,0));
protected:

	HealthController* _healthCtrl;

	void onDie();

	void onEnter() override;
	bool callbackOnContactBegin(PhysicsContact& contact);
private:
	Vec2 GamePos = Vec2(0, 0);
	static int loadAnimation;
};

#endif // !__ENEMY_H__