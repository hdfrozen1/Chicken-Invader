#ifndef __OBSERVER_H__
#define __OBSERVER_H__

#include "cocos2d.h"
USING_NS_CC;

class Observer {
public:
	static Observer* getInstance();

	void registerEvent(std::string type, std::function<void(void*)> callback);
	void unRegisterEvent(std::string type, std::function<void(void*)> callback);
	void notify(std::string type, void* data);
private:
	static Observer* _instance;

	// <EventType, vector<>> 
	std::map < std::string, std::vector<std::function<void(void*)>>*> _eventTypes;

	std::vector<std::function<void(void*)>>* getObservers(std::string type);

};
#endif // !__OBSERVER_H__