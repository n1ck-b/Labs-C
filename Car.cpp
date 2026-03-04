#include "Functions.h"

Car::~Car() = default;

int Car::getYear() const
{
	return yearOfProduction;
}
int Car::getMileage() const
{
	return mileage;
}
float Car::getPrice() const
{
	return price;
}
string Car::getBrand() const
{
	return brand;
}
string Car::getModel() const
{
	return model;
}
void Car::setYear(int CarYearOfProduction)
{
	this->yearOfProduction = CarYearOfProduction;
}
void Car::setMileage(int CarMileage)
{
	this->mileage = CarMileage;
}
void Car::setPrice(float CarPrice)
{
	this->price = CarPrice;
}
void Car::setBrand(const std::string& CarBrand)
{
	this->brand = CarBrand;
}
void Car::setModel(const std::string& CarModel)
{
	this->model = CarModel;
}