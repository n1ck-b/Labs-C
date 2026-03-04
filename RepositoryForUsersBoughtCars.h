#pragma once
#include "Functions.h"
#include <QListWidget>
#include <string.h>
#include "List.h"

template <typename T>
class List;

template <typename T>
class RepositoryForUsersBoughtCars
{
public:
	static void printBoughtCars(std::string login, QListWidget* listWidget, int& count)
	{
		sqlite3* db;
		int error = sqlite3_open_v2("Users_Sold_Cars.db", &db, SQLITE_OPEN_READWRITE, nullptr);
		if (error)
		{
			sqlite3_close(db);
			return;
		}
		const char* SQL = "SELECT* FROM users_sold_cars WHERE user_login = ?;";
		sqlite3_stmt* stmt;
		error = sqlite3_prepare_v2(db, SQL, -1, &stmt, nullptr);
		if (error != SQLITE_OK)
		{
			sqlite3_finalize(stmt);
			sqlite3_close(db);
			return;
		}
		sqlite3_bind_text(stmt, 1, login.c_str(), -1, SQLITE_STATIC);
		RepositoryForCars<T> rep;
		int id;
		std::string tableName;
		T car;
		std::string sql;
		std::string carData;
		while (sqlite3_step(stmt) == SQLITE_ROW)
		{
			tableName = "";
			if (strcmp((const char*)sqlite3_column_text(stmt, 1), "electric") == 0 && std::is_same_v<ElectricEngineCar, T>)
			{
				sql = "SELECT year_of_production, mileage, price, brand, model, battery_capacity FROM ";
				tableName = "electric_cars";
			}
			else if (strcmp((const char*)sqlite3_column_text(stmt, 1), "combustion") == 0 && std::is_same_v<CombustionEngineCar, T>)
			{
				sql = "SELECT year_of_production, mileage, price, brand, model, fuel_tank_capacity FROM ";
				tableName = "combustion_cars";
			}
			else if (strcmp((const char*)sqlite3_column_text(stmt, 1), "hybrid") == 0 && std::is_same_v<HybridEngineCar, T>)
			{
				sql = "SELECT year_of_production, mileage, price, brand, model, fuel_tank_capacity, battery_capacity, hybrid_type FROM ";
				tableName = "hybrid_cars";
			}
			if (!tableName.empty())
			{
				if (count == 1)
					listWidget->clear();
				id = sqlite3_column_int(stmt, 2);
				sql = sql + tableName + " WHERE id = ?;";
				rep.getCar(0, tableName, "Cars.db", sql.c_str(), car, id);
				carData = std::to_string(count) + "-й автомобиль:\n" + "  Год выпуска: " + std::to_string(car.getYear()) + "\n" + "  Пробег: " + std::to_string(car.getMileage()) + " км\n" + "  Стоимость покупки : ";
				std::string str = std::to_string(car.getPrice());
				str = str.substr(0, str.find(',') + 4) + "р.";
				carData += str + "\n" + "  Марка: " + car.getBrand() + "\n" + "  Модель: " + car.getModel() + "\n";
				if constexpr (std::is_same_v<CombustionEngineCar, T> || std::is_same_v<HybridEngineCar, T>)
				{
					carData = carData + "  Объем топливного бака: ";
					str = std::to_string(car.getFuelTankCapacity());
					str = str.substr(0, str.find(',')) + " л";
					carData += str + "\n";
				}
				if constexpr (std::is_same_v<ElectricEngineCar, T> || std::is_same_v<HybridEngineCar, T>)
				{
					carData = carData + "  Емкость аккумулятора: ";
					str = std::to_string(car.getBatteryCapacity());
					str = str.substr(0, str.find(',')) + " Ач";
					carData += str + "\n";
				}
				if constexpr (std::is_same_v<HybridEngineCar, T>)
				{
					if (car.getHybridType() == 0)
						carData = carData + "  Тип гибридного двигателя: последовательный" + "\n";
					else
						carData = carData + "  Тип гибридного двигателя: параллельный" + "\n";
				}
				listWidget->addItem(QString::fromStdString(carData));
				count++;
			}
		}
		sqlite3_finalize(stmt);
		sqlite3_close(db);
	}
	static void addCar(int idInDB, std::string login, std::string engineType)
	{
		sqlite3* db;
		int error = sqlite3_open_v2("Users_Sold_Cars.db", &db, SQLITE_OPEN_READWRITE, nullptr);
		if (error)
		{
			sqlite3_close(db);
			return;
		}
		const char* SQL = "INSERT INTO users_sold_cars (type_of_engine, id_in_cars_db, user_login) VALUES (?, ?, ?);";
		sqlite3_stmt* stmt;
		error = sqlite3_prepare_v2(db, SQL, -1, &stmt, nullptr);
		if (error != SQLITE_OK)
		{
			sqlite3_finalize(stmt);
			sqlite3_close(db);
			return;
		}
		sqlite3_bind_text(stmt, 1, engineType.c_str(), -1, SQLITE_STATIC);
		sqlite3_bind_int(stmt, 2, idInDB);
		sqlite3_bind_text(stmt, 3, login.c_str(), -1, SQLITE_STATIC);
		error = sqlite3_step(stmt);
		if (error != SQLITE_DONE)
		{
			sqlite3_finalize(stmt);
			sqlite3_close(db);
			return;
		}
		sqlite3_finalize(stmt);
		sqlite3_close(db);
	}
	static void tradeInCar(std::string login, std::string engineType, int id)
	{
		sqlite3* db;
		int error = sqlite3_open_v2("Users_Sold_Cars.db", &db, SQLITE_OPEN_READWRITE, nullptr);
		if (error)
		{
			sqlite3_close(db);
			return;
		}
		sqlite3_stmt* stmt;
		const char* SQL = "SELECT id_in_cars_db FROM users_sold_cars WHERE id = ?;";
		error = sqlite3_prepare_v2(db, SQL, -1, &stmt, nullptr);
		sqlite3_bind_int(stmt, 1, id);
		error = sqlite3_step(stmt);
		if (error == SQLITE_ERROR)
		{
			sqlite3_finalize(stmt);
			sqlite3_close(db);
			return;
		}
		int idInCarsDb = sqlite3_column_int(stmt, 0);
		if (engineType == "electric")
			RepositoryForCars<ElectricEngineCar>::setCarAsSold(idInCarsDb, "electric_cars", false);
		else if (engineType == "combustion")
			RepositoryForCars<ElectricEngineCar>::setCarAsSold(idInCarsDb, "combustion_cars", false);
		else if (engineType == "hybrid")
			RepositoryForCars<ElectricEngineCar>::setCarAsSold(idInCarsDb, "hybrid_cars", false);
		sqlite3_finalize(stmt);
		SQL = "DELETE FROM users_sold_cars WHERE user_login = ? AND id = ?;";
		error = sqlite3_prepare_v2(db, SQL, -1, &stmt, nullptr);
		if (error != SQLITE_OK)
		{
			sqlite3_finalize(stmt);
			sqlite3_close(db);
			return;
		}
		sqlite3_bind_text(stmt, 1, login.c_str(), -1, SQLITE_STATIC);
		sqlite3_bind_int(stmt, 2, id);
		sqlite3_step(stmt);
		sqlite3_finalize(stmt);
		sqlite3_close(db);
	}
	static void getAllCars(std::string login, List<CombustionEngineCar>& combustionCarsList, List<ElectricEngineCar>& electricCarsList, List<HybridEngineCar>& hybridCarsList)
	{
		sqlite3* db;
		int error = sqlite3_open_v2("Users_Sold_Cars.db", &db, SQLITE_OPEN_READWRITE, nullptr);
		if (error)
		{
			sqlite3_close(db);
			return;
		}
		const char* SQL = "SELECT id, id_in_cars_db FROM users_sold_cars WHERE user_login = ? AND type_of_engine = ?;";
		sqlite3_stmt* stmt;
		error = sqlite3_prepare_v2(db, SQL, -1, &stmt, nullptr);
		if (error != SQLITE_OK)
		{
			sqlite3_finalize(stmt);
			sqlite3_close(db);
			return;
		}
		CombustionEngineCar combustionCar;
		sqlite3_bind_text(stmt, 1, login.c_str(), -1, SQLITE_STATIC);
		sqlite3_bind_text(stmt, 2, "combustion", -1, SQLITE_STATIC);
		while (sqlite3_step(stmt) == SQLITE_ROW)
		{
			RepositoryForCars<CombustionEngineCar> rep;
			rep.getCar(0, "combustion_cars", "Cars.db", "SELECT year_of_production, mileage, price, brand, model, fuel_tank_capacity FROM combustion_cars WHERE id = ?;", combustionCar, sqlite3_column_int(stmt, 1));
			combustionCarsList.pushBack(combustionCar, sqlite3_column_int(stmt, 0));
		}
		sqlite3_finalize(stmt);
		error = sqlite3_prepare_v2(db, SQL, -1, &stmt, nullptr);
		ElectricEngineCar electricCar;
		sqlite3_bind_text(stmt, 1, login.c_str(), -1, SQLITE_STATIC);
		sqlite3_bind_text(stmt, 2, "electric", -1, SQLITE_STATIC);
		while (sqlite3_step(stmt) == SQLITE_ROW)
		{
			RepositoryForCars<ElectricEngineCar> rep;
			rep.getCar(0, "electric_cars", "Cars.db", "SELECT year_of_production, mileage, price, brand, model, battery_capacity FROM electric_cars WHERE id = ?;", electricCar, sqlite3_column_int(stmt, 1));
			electricCarsList.pushBack(electricCar, sqlite3_column_int(stmt, 0));
		}
		sqlite3_finalize(stmt);
		error = sqlite3_prepare_v2(db, SQL, -1, &stmt, nullptr);
		HybridEngineCar hybridCar;
		sqlite3_bind_text(stmt, 1, login.c_str(), -1, SQLITE_STATIC);
		sqlite3_bind_text(stmt, 2, "hybrid", -1, SQLITE_STATIC);
		while (sqlite3_step(stmt) == SQLITE_ROW)
		{
			RepositoryForCars<HybridEngineCar> rep;
			rep.getCar(0, "hybrid_cars", "Cars.db", "SELECT year_of_production, mileage, price, brand, model, fuel_tank_capacity, battery_capacity, hybrid_type FROM hybrid_cars WHERE id = ?;", hybridCar, sqlite3_column_int(stmt, 1));
			hybridCarsList.pushBack(hybridCar, sqlite3_column_int(stmt, 0));
		}
		sqlite3_finalize(stmt);
		sqlite3_close(db);
	}
};