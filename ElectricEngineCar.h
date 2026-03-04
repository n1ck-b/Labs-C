#pragma once
#include "Functions.h"
template <typename T>
class RepositoryForCars;

class ElectricEngineCar : virtual public Car
{
	friend class RepositoryForCars<ElectricEngineCar>;
protected:
	float batteryCapacity;
public:
	void setBatteryCapacity(float batteryCapacity);
	float getBatteryCapacity();
};