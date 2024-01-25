#ifndef __I_DAMAGEABLE_H__
#define __I_DAMAGEABLE_H__

// EnemySpawn

class IDamageable
{
public:
	virtual void takeDamage(int dame) = 0;
};

#endif // !__I_DAMAGEABLE_H__