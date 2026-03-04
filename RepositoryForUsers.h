#pragma once
#include "Functions.h"
#include "User.h"
using namespace std;

class RepositoryForUsers
{
public:
	static void addUser(const User& user)
	{
		sqlite3* dataBase;
		int error = sqlite3_open_v2("Users.db", &dataBase, SQLITE_OPEN_READWRITE, nullptr);
		if (error != SQLITE_OK)
		{
			throw Exception(sqlite3_errmsg(dataBase), "UserDataBase");
		}
		const char* SQL = "INSERT INTO users (login, password, is_admin, count_of_bought_cars, percentage_of_discount, discount_amount, balance, name) VALUES (?, ?, ?, ?, ?, ?, ?, ?);";
		sqlite3_stmt* stmt;
		error = sqlite3_prepare_v2(dataBase, SQL, -1, &stmt, nullptr);
		if (error != SQLITE_OK)
		{
			sqlite3_close(dataBase);
			return;
		}
		sqlite3_bind_text(stmt, 1, user.login.c_str(), -1, SQLITE_STATIC);
		sqlite3_bind_text(stmt, 2, user.password.c_str(), -1, SQLITE_STATIC);
		sqlite3_bind_int(stmt, 3, (int)user.isAdmin);
		sqlite3_bind_int(stmt, 4, user.countOfBoughtCars);
		sqlite3_bind_double(stmt, 5, user.percentageOfDiscount);
		sqlite3_bind_double(stmt, 6, user.discountAmount);
		sqlite3_bind_double(stmt, 7, user.balance);
		sqlite3_bind_text(stmt, 8, user.name.c_str(), -1, SQLITE_STATIC);
		sqlite3_step(stmt);
		sqlite3_finalize(stmt);
		sqlite3_close(dataBase);
	}
	static bool isUserExisting(const string& login)
	{
		sqlite3* dataBase;
		int error = sqlite3_open_v2("Users.db", &dataBase, SQLITE_OPEN_READWRITE, nullptr);
		if (error)
		{
			sqlite3_close(dataBase);
			return false;
		}
		const char* SQL = "SELECT COUNT(login) FROM users WHERE login = ?;";
		sqlite3_stmt* stmt;
		error = sqlite3_prepare_v2(dataBase, SQL, -1, &stmt, nullptr);
		if (error != SQLITE_OK)
		{
			sqlite3_finalize(stmt);
			sqlite3_close(dataBase);
			return false;
		}
		sqlite3_bind_text(stmt, 1, login.c_str(), -1, SQLITE_STATIC);
		error = sqlite3_step(stmt);
		if (error == SQLITE_ERROR)
		{
			sqlite3_finalize(stmt);
			sqlite3_close(dataBase);
			return false;
		}
		if (sqlite3_column_int(stmt, 0) == 0)
		{
			sqlite3_finalize(stmt);
			sqlite3_close(dataBase);
			return false;
		}
		else
		{
			sqlite3_finalize(stmt);
			sqlite3_close(dataBase);
			return true;
		}
	}
	static bool isPasswordCorrect(std::string login, std::string password)
	{
		if (!isUserExisting(login))
			return false;
		sqlite3* dataBase;
		int error = sqlite3_open_v2("Users.db", &dataBase, SQLITE_OPEN_READWRITE, nullptr);
		if (error)
		{
			sqlite3_close(dataBase);
			return false;
		}
		const char* SQL = "SELECT COUNT(*) FROM users WHERE login = ? AND password = ?;";
		sqlite3_stmt* stmt;
		error = sqlite3_prepare_v2(dataBase, SQL, -1, &stmt, nullptr);
		if (error != SQLITE_OK)
		{
			sqlite3_finalize(stmt);
			sqlite3_close(dataBase);
			return false;
		}
		sqlite3_bind_text(stmt, 1, login.c_str(), -1, SQLITE_STATIC);
		sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);
		error = sqlite3_step(stmt);
		if (error == SQLITE_ERROR)
		{
			sqlite3_finalize(stmt);
			sqlite3_close(dataBase);
			return false;
		}
		if (sqlite3_column_int(stmt, 0) == 0)
		{
			sqlite3_finalize(stmt);
			sqlite3_close(dataBase);
			return false;
		}
		sqlite3_finalize(stmt);
		sqlite3_close(dataBase);
		return true;
	}
	static void getUserInfo(User& user, std::string login)
	{
		sqlite3* dataBase;
		int error = sqlite3_open_v2("Users.db", &dataBase, SQLITE_OPEN_READWRITE, nullptr);
		if (error)
		{
			sqlite3_close(dataBase);
			return;
		}
		const char* SQL = "SELECT * FROM users WHERE login = ?;";
		sqlite3_stmt* stmt;
		error = sqlite3_prepare_v2(dataBase, SQL, -1, &stmt, nullptr);
		if (error != SQLITE_OK)
		{
			sqlite3_finalize(stmt);
			sqlite3_close(dataBase);
			return;
		}
		sqlite3_bind_text(stmt, 1, login.c_str(), -1, SQLITE_STATIC);
		error = sqlite3_step(stmt);
		if (error == SQLITE_ERROR)
		{
			sqlite3_close(dataBase);
			sqlite3_finalize(stmt);
			return;
		}
		user.login = (const char*)sqlite3_column_text(stmt, 1);
		user.password = (const char*)sqlite3_column_text(stmt, 2);
		user.isAdmin = sqlite3_column_int(stmt, 3);
		user.countOfBoughtCars = sqlite3_column_int(stmt, 4);
		user.percentageOfDiscount = sqlite3_column_double(stmt, 5);
		user.discountAmount = sqlite3_column_double(stmt, 6);
		user.balance = sqlite3_column_double(stmt, 7);
		user.name = (const char*)sqlite3_column_text(stmt, 8);
		sqlite3_finalize(stmt);
		sqlite3_close(dataBase);
	}
	static void userBoughtCar(std::string login, float balance, float price)
	{
		sqlite3* dataBase;
		int error = sqlite3_open_v2("Users.db", &dataBase, SQLITE_OPEN_READWRITE, nullptr);
		if (error)
		{
			sqlite3_close(dataBase);
			return;
		}
		const char* SQL = "SELECT * FROM users WHERE login = ?;";
		sqlite3_stmt* stmt;
		error = sqlite3_prepare_v2(dataBase, SQL, -1, &stmt, nullptr);
		if (error != SQLITE_OK)
		{
			sqlite3_finalize(stmt);
			sqlite3_close(dataBase);
			return;
		}
		sqlite3_bind_text(stmt, 1, login.c_str(), -1, SQLITE_STATIC);
		error = sqlite3_step(stmt);
		if (error == SQLITE_ERROR)
		{
			sqlite3_close(dataBase);
			sqlite3_finalize(stmt);
			return;
		}
		int countOfBoughtCars = sqlite3_column_int(stmt, 4);
		SQL = "UPDATE users SET count_of_bought_cars = ?, percentage_of_discount = ?, balance = ? WHERE login = ?;";
		sqlite3_finalize(stmt);
		error = sqlite3_prepare_v2(dataBase, SQL, -1, &stmt, nullptr);
		if (error != SQLITE_OK)
		{
			sqlite3_close(dataBase);
			return;
		}
		sqlite3_bind_int(stmt, 1, countOfBoughtCars + 1);
		if (countOfBoughtCars + 1 >= 20)
			sqlite3_bind_double(stmt, 2, 7);
		else if (countOfBoughtCars + 1 >= 10)
			sqlite3_bind_double(stmt, 2, 5);
		else if (countOfBoughtCars + 1 >= 5)
			sqlite3_bind_double(stmt, 2, 3);
		else
			sqlite3_bind_double(stmt, 2, 0);
		float newBalance = balance - price;
		sqlite3_bind_double(stmt, 3, newBalance);
		sqlite3_bind_text(stmt, 4, login.c_str(), -1, SQLITE_STATIC);
		error = sqlite3_step(stmt);
		if (error == SQLITE_ERROR)
		{
			sqlite3_close(dataBase);
			sqlite3_finalize(stmt);
			return;
		}
		sqlite3_finalize(stmt);
		sqlite3_close(dataBase);
	}
	static void updateBalance(float newBalance, std::string login)
	{
		sqlite3* db;
		int error = sqlite3_open_v2("Users.db", &db, SQLITE_OPEN_READWRITE, nullptr);
		if (error != SQLITE_OK)
		{
			sqlite3_close(db);
			return;
		}
		sqlite3_stmt* stmt;
		const char* SQL = "UPDATE users SET balance = ? WHERE login = ?;";
		error = sqlite3_prepare_v2(db, SQL, -1, &stmt, nullptr);
		if (error != SQLITE_OK)
		{
			sqlite3_finalize(stmt);
			sqlite3_close(db);
			return;
		}
		sqlite3_bind_double(stmt, 1, newBalance);
		sqlite3_bind_text(stmt, 2, login.c_str(), -1, SQLITE_STATIC);
		sqlite3_step(stmt);
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		return;
	}
	static void tradeIn(float priceOfCar, std::string login)
	{
		sqlite3* db;
		int error = sqlite3_open_v2("Users.db", &db, SQLITE_OPEN_READWRITE, nullptr);
		if (error)
		{
			sqlite3_close(db);
			return;
		}
		const char* SQL = "SELECT discount_amount FROM users WHERE login = ?;";
		sqlite3_stmt* stmt;
		error = sqlite3_prepare_v2(db, SQL, -1, &stmt, nullptr);
		if (error != SQLITE_OK)
		{
			sqlite3_finalize(stmt);
			sqlite3_close(db);
			return;
		}
		sqlite3_bind_text(stmt, 1, login.c_str(), -1, SQLITE_STATIC);
		error = sqlite3_step(stmt);
		if (error == SQLITE_ERROR)
		{
			sqlite3_finalize(stmt);
			sqlite3_close(db);
			return;
		}
		double discountAmount = sqlite3_column_double(stmt, 0);
		sqlite3_finalize(stmt);
		SQL = "UPDATE users SET discount_amount = ? WHERE login = ?;";
		error = sqlite3_prepare_v2(db, SQL, -1, &stmt, nullptr);
		if (error != SQLITE_OK)
		{
			sqlite3_finalize(stmt);
			sqlite3_close(db);
			return;
		}
		sqlite3_bind_double(stmt, 1, discountAmount + priceOfCar * 0.7);
		sqlite3_bind_text(stmt, 2, login.c_str(), -1, SQLITE_STATIC);
		sqlite3_step(stmt);
		sqlite3_finalize(stmt);
		sqlite3_close(db);
	}
	static void setDiscountAmount(std::string login, double newDiscountAmount)
	{
		sqlite3* db;
		int error = sqlite3_open_v2("Users.db", &db, SQLITE_OPEN_READWRITE, nullptr);
		if (error)
		{
			sqlite3_close(db);
			return;
		}
		const char* SQL = "UPDATE users SET discount_amount = ? WHERE login = ?;";
		sqlite3_stmt* stmt;
		error = sqlite3_prepare_v2(db, SQL, -1, &stmt, nullptr);
		if (error != SQLITE_OK)
		{
			sqlite3_finalize(stmt);
			sqlite3_close(db);
			return;
		}
		sqlite3_bind_double(stmt, 1, newDiscountAmount);
		sqlite3_bind_text(stmt, 2, login.c_str(), -1, SQLITE_STATIC);
		sqlite3_step(stmt);
		sqlite3_finalize(stmt);
		sqlite3_close(db);
	}
	static double getPercentageOfDiscount(std::string login)
	{
		sqlite3* db;
		int error = sqlite3_open_v2("Users.db", &db, SQLITE_OPEN_READWRITE, nullptr);
		if (error)
		{
			sqlite3_close(db);
			return 0.0;
		}
		const char* SQL = "SELECT percentage_of_discount FROM users WHERE login = ?;";
		sqlite3_stmt* stmt;
		error = sqlite3_prepare_v2(db, SQL, -1, &stmt, nullptr);
		if (error != SQLITE_OK)
		{
			sqlite3_finalize(stmt);
			sqlite3_close(db);
			return 0.0;
		}
		sqlite3_bind_text(stmt, 1, login.c_str(), -1, SQLITE_STATIC);
		error = sqlite3_step(stmt);
		if (error == SQLITE_ERROR)
		{
			sqlite3_finalize(stmt);
			sqlite3_close(db);
			return 0.0;
		}
		double percentageOfDiscount = sqlite3_column_double(stmt, 0);
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		return percentageOfDiscount;
	}
};