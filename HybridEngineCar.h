#pragma once
#include "Functions.h"
template <typename T>
class RepositoryForCars;

class HybridEngineCar : public ElectricEngineCar, public CombustionEngineCar
{
	friend class RepositoryForCars<HybridEngineCar>;
private:
	int hybridType; //0 - последовательный, 1 - параллельный
public:
	void setHybridType(int hybridType);
	int getHybridType();
};