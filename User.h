#pragma once
#include <iostream>
#include <string>

class User
{
	friend class RepositoryForUsers;
private:
	std::string login;
	std::string password;
	bool isAdmin = 0;
	int countOfBoughtCars = 0;
	float percentageOfDiscount = 0;
	float discountAmount = 0;
	float balance = 0;
	std::string name;
public:
	std::string getLogin() const;
	std::string getPassword() const;
	void setLogin(std::string login);
	void setPassword(std::string password);
	bool getIsAdmin() const;
	void setIsAdmin(bool isAdmin);
	int getCountOfBoughtCars() const;
	void setCountOfBoughtCars(int countOfBoughtCars);
	float getPercentageOfDiscount() const;
	void setPercentageOfDiscount(float percentageOfDiscount);
	float getDiscountAmount() const;
	void setDiscountAmount(float discountAmount);
	float getBalance() const;
	void setBalance(float balance);
	std::string getName() const;
	void setName(std::string name);
};