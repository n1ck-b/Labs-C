#pragma once
#include <QJsonArray>
#include <QJsonObject>
#include "catalogwindow.h"
#include "ui_catalogwindow.h"
#include "MainWindow.h"
#include "Exception.h"
#include "Functions.h"
#include "Iterator.h"
#include "List.h"

template <typename T>
class Iterator;

template <typename T>
class List;

template <typename T>
class RepositoryForCars
{
public:
	int findIDinDB(int index, const std::string& tableName) const
	{
		int id = 0;
		sqlite3* dataBase;
		int error = sqlite3_open_v2("Cars.db", &dataBase, SQLITE_OPEN_READWRITE, nullptr);
		if (error)
		{
			throw Exception(sqlite3_errmsg(dataBase), "DataBase");
		}
		sqlite3_stmt* stmt;
		std::string sql = "SELECT id FROM " + tableName + ";";
		const char* SQL = sql.c_str();
		error = sqlite3_prepare_v2(dataBase, SQL, -1, &stmt, nullptr);
		if (error != SQLITE_OK)
		{
			std::cout << "Не удалось выполнить запрос: " << sqlite3_errmsg(dataBase) << std::endl;
			return 0;
		}
		for (int i = 0; i < index; i++)
		{
			sqlite3_step(stmt);
			id = sqlite3_column_int(stmt, 0);
		}
		sqlite3_finalize(stmt);
		sqlite3_close(dataBase);
		return id;
	}
	void addCar(T& carClass, const char* SQL, std::string dbName) const
	{
		sqlite3* dataBase;
		int error = sqlite3_open_v2(dbName.c_str(), &dataBase, SQLITE_OPEN_READWRITE, nullptr);
		if (error)
		{
			throw Exception(sqlite3_errmsg(dataBase), "Cars DataBase");
		}
		sqlite3_stmt* stmt;
		error = sqlite3_prepare_v2(dataBase, SQL, -1, &stmt, nullptr);
		if (error != SQLITE_OK)
		{
			std::cout << "Не удалось подготовить SQL запрос: " << sqlite3_errmsg(dataBase) << std::endl;
			sqlite3_close(dataBase);
			return;
		}
		sqlite3_bind_int(stmt, 1, carClass.yearOfProduction);
		sqlite3_bind_int(stmt, 2, carClass.mileage);
		sqlite3_bind_double(stmt, 3, carClass.price);
		sqlite3_bind_text(stmt, 4, carClass.brand.c_str(), -1, SQLITE_STATIC);
		sqlite3_bind_text(stmt, 5, carClass.model.c_str(), -1, SQLITE_STATIC);
		if constexpr (std::is_same_v<CombustionEngineCar, T>)
			sqlite3_bind_double(stmt, 6, carClass.getFuelTankCapacity());
		if constexpr (std::is_same_v<ElectricEngineCar, T>)
			sqlite3_bind_double(stmt, 6, carClass.getBatteryCapacity());
		if constexpr (std::is_same_v<HybridEngineCar, T>)
		{
			sqlite3_bind_double(stmt, 6, carClass.getFuelTankCapacity());
			sqlite3_bind_double(stmt, 7, carClass.getBatteryCapacity());
			sqlite3_bind_int(stmt, 8, carClass.getHybridType());
		}
		sqlite3_step(stmt);
		sqlite3_finalize(stmt);
		sqlite3_close(dataBase);
	}
	void print(const char* SQL, std::string dbName, int typeOfEngine, QListWidget* listWidget, int& count) const
	{
		sqlite3* dataBase;
		int error = sqlite3_open_v2(dbName.c_str(), &dataBase, SQLITE_OPEN_READWRITE, nullptr);
		if (error)
		{
			throw Exception(sqlite3_errmsg(dataBase), "Cars DataBase");
		}
		sqlite3_stmt* stmt;
		error = sqlite3_prepare_v2(dataBase, SQL, -1, &stmt, nullptr);
		std::string carData;
		while (sqlite3_step(stmt) == SQLITE_ROW)
		{
			carData = std::to_string(count) + "-й автомобиль:\n" + "  Год выпуска: " + std::to_string(sqlite3_column_int(stmt, 0)) + "\n" + "  Пробег: " + std::to_string(sqlite3_column_int(stmt, 1)) + " км\n" + "  Стоимость: ";
			std::string str = std::to_string(sqlite3_column_double(stmt, 2));
			str = str.substr(0, str.find(',') + 4) + " р.";
			carData += str + "\n" + "  Марка: " + (const char*)sqlite3_column_text(stmt, 3) + "\n" + "  Модель: " + (const char*)sqlite3_column_text(stmt, 4) + "\n";
			if (typeOfEngine == 2 || typeOfEngine == 3)
			{
				carData = carData + "  Объем топливного бака: ";
				str = std::to_string(sqlite3_column_double(stmt, 5));
				str = str.substr(0, str.find(',') + 2) + " л";
				carData += str + "\n";
			}
			if (typeOfEngine == 1)
			{
				carData = carData + "  Емкость аккумулятора: ";
				str = std::to_string(sqlite3_column_double(stmt, 5));
				str = str.substr(0, str.find(',') + 2) + " Ач";
				carData += str + "\n";
			}
			if (typeOfEngine == 3)
			{
				carData = carData + "  Емкость аккумулятора: ";
				str = std::to_string(sqlite3_column_double(stmt, 6));
				str = str.substr(0, str.find(',') + 2) + " Ач";
				carData += str + "\n";
				int hybridType = sqlite3_column_int(stmt, 7);
				if (hybridType == 0)
					carData = carData + "  Тип гибридного двигателя: последовательный" + "\n";
				else
					carData = carData + "  Тип гибридного двигателя: параллельный" + "\n";
			}
			listWidget->addItem(QString::fromStdString(carData));
			count++;
		}
		sqlite3_finalize(stmt);
		sqlite3_close(dataBase);
	}
	void deleteCar(int index, const std::string& tableName, std::string dbName, int idInDB = 0) const
	{
		int id;
		if (idInDB == 0)
			id = findIDinDB(index, tableName);
		else
			id = idInDB;
		sqlite3* dataBase;
		int error = sqlite3_open_v2(dbName.c_str(), &dataBase, SQLITE_OPEN_READWRITE, nullptr);
		if (error)
		{
			throw Exception(sqlite3_errmsg(dataBase), "Cars DataBase");
		}
		if (error)
		{
			sqlite3_close(dataBase);
			return;
		}
		std::string sql = "DELETE FROM " + tableName + " WHERE id = ? ; ";
		const char* SQL = sql.c_str();
		sqlite3_stmt* stmt;
		error = sqlite3_prepare_v2(dataBase, SQL, -1, &stmt, nullptr);
		if (error != SQLITE_OK)
		{
			sqlite3_close(dataBase);
			return;
		}
		sqlite3_bind_int(stmt, 1, id);
		sqlite3_step(stmt);
		sqlite3_finalize(stmt);
		sqlite3_close(dataBase);
	}
	void searchForCarInDB(const char* SQL, std::string dbName, int typeOfEngine, int& count, QListWidget* listWidget, QJsonArray& jsonArray) const
	{
		sqlite3* dataBase;
		int error = sqlite3_open_v2(dbName.c_str(), &dataBase, SQLITE_OPEN_READWRITE, nullptr);
		if (error)
		{
			throw Exception(sqlite3_errmsg(dataBase), "Cars DataBase");
			return;
		}
		sqlite3_stmt* stmt;
		error = sqlite3_prepare_v2(dataBase, SQL, -1, &stmt, nullptr);
		if (error != SQLITE_OK)
		{
			sqlite3_finalize(stmt);
			sqlite3_close(dataBase);
			return;
		}
		std::string carData;
		QJsonObject jsonObject;
		while (sqlite3_step(stmt) == SQLITE_ROW)
		{
			carData = std::to_string(count) + "-й автомобиль:\n" + "  Год выпуска: " + std::to_string(sqlite3_column_int(stmt, 1)) + "\n" + "  Пробег в км: " + std::to_string(sqlite3_column_int(stmt, 2)) + "\n" + "  Стоимость: ";
			std::string str = std::to_string(sqlite3_column_double(stmt, 3));
			str = str.substr(0, str.find(',') + 4) + " р.";
			carData += str + "\n" + "  Марка: " + (const char*)sqlite3_column_text(stmt, 4) + "\n" + "  Модель: " + (const char*)sqlite3_column_text(stmt, 5) + "\n";
			jsonObject["year_of_production"] = QString::fromStdString(std::to_string(sqlite3_column_int(stmt, 1)));
			jsonObject["mileage"] = QString::fromStdString(std::to_string(sqlite3_column_int(stmt, 2)));
			jsonObject["price"] = QString::fromStdString(str);
			jsonObject["brand"] = (const char*)sqlite3_column_text(stmt, 4);
			jsonObject["model"] = (const char*)sqlite3_column_text(stmt, 5);
			if (typeOfEngine == 2 || typeOfEngine == 3)
			{
				carData = carData + "  Объем топливного бака: ";
				str = std::to_string(sqlite3_column_double(stmt, 6));
				str = str.substr(0, str.find(',') + 4);
				carData += str + "\n";
				jsonObject["fuel_tank_capacity"] = QString::fromStdString(str);
			}
			if (typeOfEngine == 1)
			{
				carData = carData + "  Емкость батареи: ";
				str = std::to_string(sqlite3_column_double(stmt, 6));
				str = str.substr(0, str.find(',') + 4);
				carData += str + "\n";
				jsonObject["battery_capacity"] = QString::fromStdString(str);
			}
			if (typeOfEngine == 3)
			{
				carData = carData + "  Емкость аккумулятора: ";
				str = std::to_string(sqlite3_column_double(stmt, 7));
				str = str.substr(0, str.find(',') + 4);
				carData += str + "\n";
				int hybridType = sqlite3_column_int(stmt, 8);
				if (hybridType == 0)
				{
					carData = carData + "  Тип гибридного двигателя: последовательный" + "\n";
					jsonObject["hybrid_type"] = "serial";
				}
				else
				{
					carData = carData + "  Тип гибридного двигателя: параллельный" + "\n";
					jsonObject["hybrid_type"] = "parallel";
				}
			}
			jsonArray.append(jsonObject);
			listWidget->addItem(QString::fromStdString(carData));
			count++;
		}
		sqlite3_finalize(stmt);
		sqlite3_close(dataBase);
	}
	void getCar(int index, std::string tableName, std::string dbName, const char* SQL, T& carClass, int idInDB = 0) const
	{
		int id;
		if (idInDB == 0)
			id = findIDinDB(index, tableName);
		else
			id = idInDB;
		sqlite3* dataBase;
		int error = sqlite3_open_v2(dbName.c_str(), &dataBase, SQLITE_OPEN_READWRITE, nullptr);
		if (error)
		{
			throw Exception(sqlite3_errmsg(dataBase), "Cars DataBase");
			return;
		}
		sqlite3_stmt* stmt;
		error = sqlite3_prepare_v2(dataBase, SQL, -1, &stmt, nullptr);
		if (error != SQLITE_OK)
		{
			sqlite3_finalize(stmt);
			sqlite3_close(dataBase);
			return;
		}
		sqlite3_bind_int(stmt, 1, id);
		sqlite3_step(stmt);
		carClass.setYear(sqlite3_column_int(stmt, 0));
		carClass.setMileage(sqlite3_column_int(stmt, 1));
		carClass.setPrice(static_cast<float>(sqlite3_column_double(stmt, 2)));
		carClass.setBrand((const char*)sqlite3_column_text(stmt, 3));
		carClass.setModel((const char*)sqlite3_column_text(stmt, 4));
		if constexpr (std::is_same_v<CombustionEngineCar, T> || std::is_same_v<HybridEngineCar, T>)
			carClass.setFuelTankCapacity(static_cast<float>(sqlite3_column_double(stmt, 5)));
		if constexpr (std::is_same_v<ElectricEngineCar, T>)
			carClass.setBatteryCapacity(static_cast<float>(sqlite3_column_double(stmt, 5)));
		if constexpr (std::is_same_v<HybridEngineCar, T>)
		{
			carClass.setBatteryCapacity(static_cast<float>(sqlite3_column_double(stmt, 6)));
			carClass.setHybridType(sqlite3_column_int(stmt, 7));
		}
		sqlite3_finalize(stmt);
		sqlite3_close(dataBase);
	}
	void getAllCars(List<CombustionEngineCar>& combustionCarsList, List<ElectricEngineCar>& electricCarsList, List<HybridEngineCar>& hybridCarsList)
	{
		sqlite3* dataBase;
		int error = sqlite3_open_v2("Cars.db", &dataBase, SQLITE_OPEN_READWRITE, nullptr);
		if (error)
		{
			throw Exception(sqlite3_errmsg(dataBase), "Cars DataBase");
		}
		const char* SQL = "SELECT * FROM combustion_cars WHERE is_sold = 0";
		sqlite3_stmt* stmt;
		error = sqlite3_prepare_v2(dataBase, SQL, -1, &stmt, nullptr);
		CombustionEngineCar combustionCar;
		while (sqlite3_step(stmt) == SQLITE_ROW)
		{
			combustionCar.setYear(sqlite3_column_int(stmt, 1));
			combustionCar.setMileage(sqlite3_column_int(stmt, 2));
			combustionCar.setPrice(static_cast<float>(sqlite3_column_double(stmt, 3)));
			combustionCar.setBrand((const char*)sqlite3_column_text(stmt, 4));
			combustionCar.setModel((const char*)sqlite3_column_text(stmt, 5));
			combustionCar.setFuelTankCapacity(static_cast<float>(sqlite3_column_double(stmt, 6)));
			combustionCarsList.pushBack(combustionCar, sqlite3_column_int(stmt, 0));
		}
		SQL = "SELECT * FROM electric_cars WHERE is_sold = 0";
		error = sqlite3_prepare_v2(dataBase, SQL, -1, &stmt, nullptr);
		ElectricEngineCar electricCar;
		while (sqlite3_step(stmt) == SQLITE_ROW)
		{
			electricCar.setYear(sqlite3_column_int(stmt, 1));
			electricCar.setMileage(sqlite3_column_int(stmt, 2));
			electricCar.setPrice(static_cast<float>(sqlite3_column_double(stmt, 3)));
			electricCar.setBrand((const char*)sqlite3_column_text(stmt, 4));
			electricCar.setModel((const char*)sqlite3_column_text(stmt, 5));
			electricCar.setBatteryCapacity(static_cast<float>(sqlite3_column_double(stmt, 6)));
			electricCarsList.pushBack(electricCar, sqlite3_column_int(stmt, 0));
		}
		SQL = "SELECT * FROM hybrid_cars WHERE is_sold = 0";
		error = sqlite3_prepare_v2(dataBase, SQL, -1, &stmt, nullptr);
		HybridEngineCar hybridCar;
		while (sqlite3_step(stmt) == SQLITE_ROW)
		{
			hybridCar.setYear(sqlite3_column_int(stmt, 1));
			hybridCar.setMileage(sqlite3_column_int(stmt, 2));
			hybridCar.setPrice(static_cast<float>(sqlite3_column_double(stmt, 3)));
			hybridCar.setBrand((const char*)sqlite3_column_text(stmt, 4));
			hybridCar.setModel((const char*)sqlite3_column_text(stmt, 5));
			hybridCar.setFuelTankCapacity(static_cast<float>(sqlite3_column_double(stmt, 6)));
			hybridCar.setBatteryCapacity(static_cast<float>(sqlite3_column_double(stmt, 7)));
			hybridCar.setHybridType(sqlite3_column_int(stmt, 8));
			hybridCarsList.pushBack(hybridCar, sqlite3_column_int(stmt, 0));
		}
		sqlite3_finalize(stmt);
		sqlite3_close(dataBase);
	}
	static void setCarAsSold(int id, std::string tableName, bool isSold = true)
	{
		sqlite3* db;
		int error = sqlite3_open_v2("Cars.db", &db, SQLITE_OPEN_READWRITE, nullptr);
		if (error)
		{
			sqlite3_close(db);
			return;
		}
		std::string SQL = "UPDATE " + tableName + " SET is_sold = ? WHERE id = ?;";
		sqlite3_stmt* stmt;
		error = sqlite3_prepare_v2(db, SQL.c_str(), -1, &stmt, nullptr);
		if (error != SQLITE_OK)
		{
			sqlite3_finalize(stmt);
			sqlite3_close(db);
			return;
		}
		if(isSold)
			sqlite3_bind_int(stmt, 1, 1);
		else
			sqlite3_bind_int(stmt, 1, 0);
		sqlite3_bind_int(stmt, 2, id);
		sqlite3_step(stmt);
		sqlite3_finalize(stmt);
		sqlite3_close(db);
	}
};