#pragma once
#include <iostream>
#include <string>

class Car
{
protected:
	int yearOfProduction;
	int mileage;
	float price;
	std::string brand;
	std::string model;
public:
	virtual int getYear() const;
	virtual int getMileage() const;
	virtual float getPrice() const;
	virtual std::string getBrand() const;
	virtual std::string getModel() const;
	virtual void setYear(int yearOfProduction);
	virtual void setMileage(int mileage);
	virtual void setPrice(float price);
	virtual void setBrand(const std::string& CarBrand);
	virtual void setModel(const std::string& CarModel);
	virtual ~Car() = 0;
};