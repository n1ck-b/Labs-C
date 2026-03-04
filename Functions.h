#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "sqlite/sqlite3.h"
#include "Car.h"
#include "ElectricEngineCar.h"
#include "CombustionEngineCar.h"
#include "HybridEngineCar.h"
#include "RepositoryForCars.h"
#include "RepositoryForUsers.h"
#include "RepositoryForUsersBoughtCars.h"
#include "Exception.h"
#include "CarAddingWindow.h"
#include "CatalogWindow.h"
#include "CarCompareWindow.h"
#include "MainWindow.h"
#include "SearchForCarsWindow.h"
#include "List.h"
#include "Node.h"
#include "Iterator.h"
//#include "User.h"
//#include "SessionStatus.h"
#include <typeinfo>
#include <vector>

using namespace std;

int tableRowCount(int& count, const string& tableName);
void dbRowsCount(int& hybridCount, int& combustionCount, int& electricCount);
