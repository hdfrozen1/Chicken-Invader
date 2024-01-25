
#ifndef __HEALTH_CONTROLLER_H__
#define __HEALTH_CONTROLLER_H__

#include "cocos2d.h"
USING_NS_CC;

class HealthController : public Sprite
{
public:
	static HealthController* create(float current, std::string fillPath);
	bool init(float current, std::string fillPath);

	void setOnDie(std::function<void()> onDie) { this->_onDie = onDie; }

	void setCurrentHealth(float newVal);
	float getCurrentHealth();

	void setMaxHealth(float newVal);
	float getMaxHealth();
private:
	float _currentHealth, _maxHealth;
	std::function<void()> _onDie;

	void onChangeCurrent(float newVal);
};

#endif // !__HEALTH_CONTROLLER_H__