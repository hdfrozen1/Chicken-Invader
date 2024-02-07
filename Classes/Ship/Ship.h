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
	int getdame() { return _dame; }
	void takeDamage(int dame) override;
protected:

	HealthController* _healthCtrl;
	int _dame;
	void onDie();

	void onEnter() override;
	bool callbackOnContactBegin(PhysicsContact& contact);
};

#endif // !__SHIP_H__