#pragma once
#include "Functions.h"
template <typename T>
class RepositoryForCars;

class CombustionEngineCar : virtual public Car
{
	friend class RepositoryForCars<CombustionEngineCar>;
protected:
	float fuelTankCapacity;
public:
	void setFuelTankCapacity(float fuelTankCapacity);
	float getFuelTankCapacity();
};