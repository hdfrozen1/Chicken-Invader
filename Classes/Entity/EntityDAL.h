#ifndef __ENTITY_DAL_H_
#define __ENTITY_DAL_H_

#include "Entity/EntityStat.h"
#include "cocos2d.h"

class EntityDAL
{
public:
	EntityDAL(std::string entityName);
	EntityStat* getEntityStat(int level);
protected:
	std::vector<EntityStat*> _stats;
};

#endif // !__ENTITY_DAL_H_