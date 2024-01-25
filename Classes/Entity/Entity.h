#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "cocos2d.h"
#include "EntityInfo.h"
#include "EntityStat.h"
#include "EntityDAL.h"

USING_NS_CC;

class Entity : public Node
{
public:
	static Entity* create(EntityInfo* info);

	virtual bool init(EntityInfo* info);

	void setLevel(int newLevel);

	Sprite* getModel() { return _model; }
	EntityInfo* getEntityInfo() { return _info; }
protected:
	virtual void onLevelUp(int level);
protected:
	EntityInfo* _info;
	EntityDAL* _entityDAL;

	CC_SYNTHESIZE(EntityStat*, _entityStat, EntityStat);

	Sprite* _model;
};

#endif // !__ENTITY_H__