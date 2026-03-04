#pragma once
#include "Functions.h"

class SessionStatus
{
private:
	static bool isAuthorized;
	static std::string login;
	static std::string name;
	static bool isAdmin;
	static float balance;
	static float discountAmount;
	static float percentageOfDiscount;
	static SessionStatus* instance;
	SessionStatus(bool isAuthorized, std::string login, std::string name);
	~SessionStatus();
public:
	SessionStatus(const SessionStatus& other) = delete;
	SessionStatus& operator=(const SessionStatus& other) = delete;
	static SessionStatus* getInstance(bool isAuthorized = false, std::string login = "", std::string name = "");
	std::string getLogin();
	std::string getName();
	bool getIsAuthorized();
	bool getIsAdmin();
	float getBalance();
	float getDiscountAmount();
	float getPercentageOfDiscount();
	void setLogin(std::string login);
	void setName(std::string name);
	void setIsAutorized(bool isAuthorized);
	void setIsAdmin(bool isAdmin);
	void setBalance(float balance);
	void setDefault();
	void setDiscountAmount(float discountAmount);
	void setPercentageOfDiscount(float percentageOfDiscount);
};

extern SessionStatus* sessionStatus;