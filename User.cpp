#include "Functions.h"

std::string User::getLogin() const
{
	return login;
}
std::string User::getPassword() const
{
	return password;
}
void User::setLogin(std::string login) 
{
	this->login = login;
}
void User::setPassword(std::string password) 
{
	this->password = password;
}
bool User::getIsAdmin() const
{
	return isAdmin;
}
void User::setIsAdmin(bool isAdmin)
{
	this->isAdmin = isAdmin;
}
int User::getCountOfBoughtCars() const
{
	return countOfBoughtCars;
}
void User::setCountOfBoughtCars(int countOfBoughtCars) 
{
	this->countOfBoughtCars = countOfBoughtCars;
}
float User::getPercentageOfDiscount() const
{
	return percentageOfDiscount;
}
void User::setPercentageOfDiscount(float percentageOfDiscount) 
{
	this->percentageOfDiscount = percentageOfDiscount;
}
float User::getDiscountAmount() const
{
	return discountAmount;
}
void User::setDiscountAmount(float discountAmount) 
{
	this->discountAmount = discountAmount;
}
float User::getBalance() const
{
	return balance;
}
void User::setBalance(float balance) 
{
	this->balance = balance;
}
std::string User::getName() const
{
	return name;
}
void User::setName(std::string name)
{
	this->name = name;
}