#include "HealthController.h"

HealthController* HealthController::create(float maxHealth, std::string fillPath)
{
	auto newObject = new HealthController();
	if (newObject != nullptr && newObject->init(maxHealth, fillPath))
	{
		newObject->autorelease();
		return newObject;
	}

	CC_SAFE_DELETE(newObject);
	return nullptr;
}

bool HealthController::init(float maxHealth, std::string fillPath)
{
	if (!Sprite::initWithFile(fillPath))
	{
		return false;
	}

	this->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);

	this->_maxHealth = maxHealth;
	this->_currentHealth = maxHealth;

	return true;
}

void HealthController::setCurrentHealth(float newVal)
{
	_currentHealth = newVal;
	this->onChangeCurrent(_currentHealth);
}

float HealthController::getCurrentHealth()
{
	return _currentHealth;
}

void HealthController::setMaxHealth(float newVal)
{
	_maxHealth = newVal;
}

float HealthController::getMaxHealth()
{
	return _maxHealth;
}

void HealthController::onChangeCurrent(float newVal)
{
	if (newVal <= 0)
	{
		_currentHealth = 0;
	}

	this->setScaleX(_currentHealth / _maxHealth);
	if (_onDie != nullptr && _currentHealth == 0)
	{
		_onDie();
		return;
	}
}