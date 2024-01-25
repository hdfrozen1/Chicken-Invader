#include "Entity.h"

Entity* Entity::create(EntityInfo* info)
{
    auto newObject = new Entity();
    if (newObject != nullptr && newObject->init(info))
    {
        newObject->autorelease();
        return newObject;
    }

    CC_SAFE_DELETE(newObject);
    return nullptr;
}

bool Entity::init(EntityInfo* info)
{
    if (!Node::init())
    {
        log("Init Entity failed!");
        return false;
    }
    this->_info = info;
    _entityDAL = new EntityDAL(_info->_entityName);
    onLevelUp(_info->_level);
    
    return true;
}

void Entity::setLevel(int newLevel)
{
    _info->_level = newLevel;
    onLevelUp(newLevel);
}


void Entity::onLevelUp(int level)
{
    auto newStat = _entityDAL->getEntityStat(level);
    if (newStat == nullptr)
    {
        return;
    }

    _entityStat = newStat;
}