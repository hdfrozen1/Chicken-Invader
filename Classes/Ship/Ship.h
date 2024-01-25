#ifndef __SHIP_H__
#define __SHIP_H__

#include "Entity/Entity.h"
#include "IDamageable.h"
#include "Health/HealthController.h"

class Ship : public Entity, public IDamageable
{
public:
	static Ship* create(EntityInfo* info);
	virtual bool init(EntityInfo* info) override;

	void takeDamage(int dame) override;
protected:

	HealthController* _healthCtrl;

	void onDie();

	void onEnter() override;
	bool callbackOnContactBegin(PhysicsContact& contact);
};

#endif // !__SHIP_H__