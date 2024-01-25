#ifndef __BOSS_H__
#define __BOSS_H__

#include "Entity/Entity.h"
#include "IDamageable.h"
#include "Health/HealthController.h"
class Boss : public Entity, public IDamageable
{
public:
	static Boss* create(EntityInfo* info);
	virtual bool init(EntityInfo* info) override;

	void takeDamage(int dame) override;
protected:
	HealthController* _healthCtrl;
	void onDie();
	void onEnter() override;

	bool callbackOnContactBegin(PhysicsContact& contact);
};

#endif // !__BOSS_H__