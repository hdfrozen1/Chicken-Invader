#include "EntityDAL.h"
#include "json/rapidjson.h"
#include "json/document.h"

EntityDAL::EntityDAL(std::string entityName)
{
    rapidjson::Document docs;

    std::string dataContent = cocos2d::FileUtils::getInstance()->getStringFromFile("Data/Entity/" + entityName + ".json");
    docs.Parse(dataContent.c_str());

    auto dataArray = docs["data"].GetArray();

    for (int i = 0; i < dataArray.Size(); ++i)
    {
        int hp = dataArray[i]["_hp"].GetInt();
        int enemyscore = dataArray[i]["_diem"].GetInt();

        EntityStat* stat = new EntityStat();
        stat->_hp = hp;
        stat->_diem = enemyscore;

        _stats.push_back(stat);
    }
}

EntityStat* EntityDAL::getEntityStat(int level)
{
    if (level < 1 || level > _stats.size())
    {
        return nullptr;
    }
    return _stats[level - 1];
}