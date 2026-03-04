#include "SessionStatus.h"

SessionStatus* SessionStatus::getInstance(bool isAuthorized, std::string login, std::string name)
{
	if (!instance)
	{
		instance = new SessionStatus(isAuthorized, login, name);
	}
	return instance;
}
SessionStatus::SessionStatus(bool isAuthorized, std::string login, std::string name)
{
	SessionStatus::isAuthorized = isAuthorized;
	SessionStatus::login = login;
	SessionStatus::name = name;
}
SessionStatus::~SessionStatus()
{
	delete instance;
}
std::string SessionStatus::getLogin()
{
	return login;
}
std::string SessionStatus::getName()
{
	return name;
}
bool SessionStatus::getIsAdmin()
{
	return isAdmin;
}
bool SessionStatus::getIsAuthorized()
{
	return isAuthorized;
}
float SessionStatus::getBalance()
{
	return balance;
}
float SessionStatus::getDiscountAmount()
{
	return discountAmount;
}
float SessionStatus::getPercentageOfDiscount()
{
	return percentageOfDiscount;
}
void SessionStatus::setLogin(std::string login)
{
	SessionStatus::login = login;
}
void SessionStatus::setName(std::string name)
{
	SessionStatus::name = name;
}
void SessionStatus::setIsAutorized(bool isAuthorized)
{
	SessionStatus::isAuthorized = isAuthorized;
}
void SessionStatus::setIsAdmin(bool isAdmin)
{
	SessionStatus::isAdmin = isAdmin;
}
void SessionStatus::setBalance(float balance)
{
	SessionStatus::balance = balance;
}
void SessionStatus::setDefault()
{
	SessionStatus::isAdmin = false;
	SessionStatus::login = "";
	SessionStatus::name = "";
	SessionStatus::balance = 0;
}
void SessionStatus::setDiscountAmount(float discountAmount)
{
	SessionStatus::discountAmount = discountAmount;
}
void SessionStatus::setPercentageOfDiscount(float percentageOfDiscount)
{
	SessionStatus::percentageOfDiscount = percentageOfDiscount;
}
SessionStatus* SessionStatus::instance = nullptr;
bool SessionStatus::isAdmin = false;
bool SessionStatus::isAuthorized = false;
std::string SessionStatus::login = "";
std::string SessionStatus::name = "";
float SessionStatus::balance = 0;
float SessionStatus::discountAmount = 0.0;
float SessionStatus::percentageOfDiscount = 0.0;
SessionStatus* sessionStatus = SessionStatus::getInstance();