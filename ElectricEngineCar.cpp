#include "Functions.h"

void ElectricEngineCar::setBatteryCapacity(float CarBatteryCapacity)
{
	this->batteryCapacity = CarBatteryCapacity;
}
float ElectricEngineCar::getBatteryCapacity()
{
	return batteryCapacity;
}