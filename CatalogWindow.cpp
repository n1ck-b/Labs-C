#include "catalogwindow.h"
#include "ui_catalogwindow.h"
#include "CarAddingWindow.h"
#include "CarCompareWindow.h"
#include <QMessageBox>
#include "SessionStatus.h"
#include "PurchaseWindow.h"

extern SessionStatus* sessionStatus;

CatalogWindow::CatalogWindow(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::CatalogWindow)
{
    ui->setupUi(this);
    ui->deletePushButton->setDisabled(true);
    ui->deletePushButton->setStyleSheet("QPushButton:hover { background-color: #CC3329; } QPushButton { color: rgb(244, 240, 239); border-radius: 12px; border-style: solid; border-width: 0px; background-color: #A62921; padding: 5px }");
    ui->buyPushButton->setDisabled(true);
    ui->buyPushButton->setStyleSheet("QPushButton:hover { background-color: #CC3329; } QPushButton { color: rgb(244, 240, 239); border-radius: 12px; border-style: solid; border-width: 0px; background-color: #A62921; padding: 5px }");
    connect(ui->backPushButton, &QPushButton::clicked, this, &CatalogWindow::onBackPushButtonClicked);
    connect(ui->addPushButton, &QPushButton::clicked, this, &CatalogWindow::onAddPushButtonClicked);
    connect(ui->listWidget, &QListWidget::itemSelectionChanged, this, &CatalogWindow::choosedItemInList);
    connect(ui->deletePushButton, &QPushButton::clicked, this, &CatalogWindow::onDeletePushButtonClicked);
    connect(ui->comparePushButton, &QPushButton::clicked, this, &CatalogWindow::onComparePushButtonClicked);
    connect(ui->filterComboBox, &QComboBox::currentIndexChanged, this, &CatalogWindow::onFilterComboBoxChanged);
    connect(ui->buyPushButton, &QPushButton::clicked, this, &CatalogWindow::onBuyPushButtonClicked);
    addItemsToList();
    ui->listWidget->setStyleSheet("QListWidget::item:selected { background-color: #E0E8E6; border-radius: 12px; padding: 2px; border: none; outline: none; margin-right: 10px; } QScrollBar { padding: 0px; margin: 0px; padding-bottom: 10px; background-color: rgb(244, 240, 239); border: 1px solid rgb(55, 51, 53); width: 15px; border-radius: 7px;} QScrollBar::handle:vertical { border: 1px solid rgb(55, 51, 53); background-color: rgb(244, 240, 239); border-radius: 4px; margin: 2px; margin-top: 20px; margin-bottom: 15px; min-height: 25px; }  QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical { background: none; } QScrollBar::add-line:vertical { width: 11px; height: 20px; padding: 0px; margin: 0px; border: none; image: url(:/rcs/free-icon-down-arrow-2985150.png); padding-left: 2px; padding-right: 2px; margin-top: 0px; margin-bottom: 0px; background-size: contain; background-position: center;} QScrollBar::sub-line:vertical { width: 11px; height: 20px; padding: 0px; margin: 0px; border: none; image: url(:/rcs/free-icon-down-arrow-29851501.png); padding-left: 2px; padding-right: 2px; margin-top: 0px; margin-bottom: 0px; background-size: contain; background-position: center;}");
    ui->listWidget->setFocusPolicy(Qt::NoFocus);
    ui->filterComboBox->setFocusPolicy(Qt::NoFocus);
    addItemsToLists();
    checkForRights();
}
CatalogWindow::~CatalogWindow()
{
    delete ui;
}
void CatalogWindow::onBackPushButtonClicked()
{
    close();
    MainWindow* mainWindow = new MainWindow(this);
    mainWindow->show();
}
void CatalogWindow::onAddPushButtonClicked()
{
    carAddingWindow = new CarAddingWindow(this);
    carAddingWindow->show();
    connect(carAddingWindow, &CarAddingWindow::carAddingWindowClosed, this, &CatalogWindow::onCarAddingWindowClosed);
}
void CatalogWindow::addItemsToList()
{
    int count = 1;
    RepositoryForCars<ElectricEngineCar> rep;
    try
    {
        const char* SQL = "SELECT year_of_production, mileage, price, brand, model, fuel_tank_capacity "
            "FROM combustion_cars WHERE is_sold = 0;";
        rep.print(SQL, "Cars.db", 2, ui->listWidget, count);
        SQL = "SELECT year_of_production, mileage, price, brand, model, battery_capacity "
            "FROM electric_cars WHERE is_sold = 0;";
        rep.print(SQL, "Cars.db", 1, ui->listWidget, count);
        SQL = "SELECT year_of_production, mileage, price, brand, model, fuel_tank_capacity, battery_capacity, hybrid_type "
            "FROM hybrid_cars WHERE is_sold = 0;";
        rep.print(SQL, "Cars.db", 3, ui->listWidget, count);
    }
    catch (Exception& ex)
    {
        QMessageBox::critical(this, "Error", QString::fromStdString(ex.whatAndContext()));
    }
}
void CatalogWindow::choosedItemInList()
{
    ui->deletePushButton->setEnabled(true);
    ui->deletePushButton->setStyleSheet("QPushButton:hover { background-color: #CC3329; } QPushButton { color: rgb(244, 240, 239); border-radius: 12px; border-style: solid; border-width: 0px; background-color: rgb(215, 67, 57); padding: 5px }");
    ui->buyPushButton->setEnabled(true);
    ui->buyPushButton->setStyleSheet("QPushButton:hover { background-color: #CC3329; } QPushButton { color: rgb(244, 240, 239); border-radius: 12px; border-style: solid; border-width: 0px; background-color: rgb(215, 67, 57); padding: 5px }");
    if (ui->listWidget->selectedItems().size() == 0)
    {
        ui->deletePushButton->setDisabled(true);
        ui->deletePushButton->setStyleSheet("QPushButton:hover { background-color: #CC3329; } QPushButton { color: rgb(244, 240, 239); border-radius: 12px; border-style: solid; border-width: 0px; background-color: #A62921; padding: 5px }");
    }
    if (ui->listWidget->selectedItems().size() == 0 || !sessionStatus->getIsAuthorized())
    {
        ui->buyPushButton->setDisabled(true);
        ui->buyPushButton->setStyleSheet("QPushButton:hover { background-color: #CC3329; } QPushButton { color: rgb(244, 240, 239); border-radius: 12px; border-style: solid; border-width: 0px; background-color: #A62921; padding: 5px }");
    }
}
void CatalogWindow::onDeletePushButtonClicked()
{
   int index = ui->listWidget->currentIndex().row() + 1;
   ui->listWidget->takeItem(index - 1);
   RepositoryForCars<ElectricEngineCar> rep;
   int rowsCountInElectricCarsTable;
   int rowsCountInCombustionCarsTable;
   int rowsCountInHybridCarsTable;
   dbRowsCount(rowsCountInHybridCarsTable, rowsCountInCombustionCarsTable, rowsCountInElectricCarsTable);
   string tableName;
   if (index <= rowsCountInCombustionCarsTable)
       tableName = "combustion_cars";
   if (index > rowsCountInCombustionCarsTable && index <= rowsCountInCombustionCarsTable + rowsCountInElectricCarsTable)
   {
       tableName = "electric_cars";
       index -= rowsCountInCombustionCarsTable;
   }
   if (index > rowsCountInCombustionCarsTable + rowsCountInElectricCarsTable)
   {
       tableName = "hybrid_cars";
       index -= (rowsCountInCombustionCarsTable + rowsCountInElectricCarsTable);
   }
   try
   {
       rep.deleteCar(index, tableName, "Cars.db");
   }
   catch (Exception& ex)
   {
       QMessageBox::critical(this, "Error", QString::fromStdString(ex.whatAndContext()));
   }
   ui->listWidget->clear();
   ui->deletePushButton->setDisabled(true);
   ui->deletePushButton->setStyleSheet("QPushButton:hover { background-color: #CC3329; } QPushButton { color: rgb(244, 240, 239); border-radius: 12px; border-style: solid; border-width: 0px; background-color: #A62921; padding: 5px }");
   addItemsToList();
}
void CatalogWindow::getDataForOneCarFromAllCars(int& engineType, int carIndex, CombustionEngineCar& combustionCar, ElectricEngineCar& electricCar, HybridEngineCar& hybridCar)
{
    if (carIndex <= combustionCarsList.size())
    {
        engineType = 2;
        auto it = combustionCarsList.begin();
        getIterator<CombustionEngineCar>(carIndex - 1, it);
        combustionCar = (*it).car;
    }
    if (carIndex > combustionCarsList.size() && carIndex <= combustionCarsList.size() + electricCarsList.size())
    {
        engineType = 1;
        auto it = electricCarsList.begin();
        getIterator<ElectricEngineCar>(carIndex - electricCarsList.size() - 1, it);
        electricCar = (*it).car;
    }
    if (carIndex > combustionCarsList.size() + electricCarsList.size())
    {
        engineType = 3;
        auto it = hybridCarsList.begin();
        getIterator<HybridEngineCar>(carIndex - combustionCarsList.size() - electricCarsList.size() - 1, it);
        hybridCar = (*it).car;
    }
}
void CatalogWindow::getDataForComparison()
{
    vector<ElectricEngineCar>electricCar(2);
    vector<CombustionEngineCar>combustionCar(2);
    vector<HybridEngineCar>hybridCar(2);
    vector<int>engineType(2);
    CarCompareWindow* window = new CarCompareWindow(this);
    window->show();
    try
    {
        if (ui->filterComboBox->currentIndex() == 0)
        {
            getDataForOneCarFromAllCars(engineType[0], carIndexForComparison1, combustionCar[0], electricCar[0], hybridCar[0]);
            getDataForOneCarFromAllCars(engineType[1], carIndexForComparison2, combustionCar[1], electricCar[1], hybridCar[1]);
        }
        if (ui->filterComboBox->currentIndex() == 1)
        {
            engineType[0] = 1;
            engineType[1] = 1;
            auto it = electricCarsList.begin();
            getIterator<ElectricEngineCar>(carIndexForComparison1 - 1, it);
            electricCar[0] = (*it).car;
            it = electricCarsList.begin();
            getIterator<ElectricEngineCar>(carIndexForComparison2 - 1, it);
            electricCar[1] = (*it).car;
        }
        if (ui->filterComboBox->currentIndex() == 2)
        {
            engineType[0] = 2;
            engineType[1] = 2;
            auto it = combustionCarsList.begin();
            getIterator<CombustionEngineCar>(carIndexForComparison1 - 1, it);
            combustionCar[0] = (*it).car;
            it = combustionCarsList.begin();
            getIterator<CombustionEngineCar>(carIndexForComparison2 - 1, it);
            combustionCar[1] = (*it).car;
        }
        if (ui->filterComboBox->currentIndex() == 3)
        {
            engineType[0] = 3;
            engineType[1] = 3;
            auto it = hybridCarsList.begin();
            getIterator<HybridEngineCar>(carIndexForComparison1 - 1, it);
            hybridCar[0] = (*it).car;
            it = hybridCarsList.begin();
            getIterator<HybridEngineCar>(carIndexForComparison2 - 1, it);
            hybridCar[1] = (*it).car;
        }
        if (engineType[0] == 1)
            window->infoAboutFirstCar(electricCar[0]);
        if (engineType[0] == 2)
            window->infoAboutFirstCar(combustionCar[0]);
        if (engineType[0] == 3)
            window->infoAboutFirstCar(hybridCar[0]);
        if (engineType[1] == 1)
            window->infoAboutSecondCar(electricCar[1]);
        if (engineType[1] == 2)
            window->infoAboutSecondCar(combustionCar[1]);
        if (engineType[1] == 3)
            window->infoAboutSecondCar(hybridCar[1]);
        window->compareCars();
        ui->compareLabel->setText("Для сравнения двух автомобилей необходимо\nвыбрать первый, нажать кнопку, затем выбрать\nвторой и нажать снова.");
    }
    catch (Exception& ex)
    {
        QMessageBox::critical(this, "Error", QString::fromStdString(ex.whatAndContext()));
    }
}
void CatalogWindow::onComparePushButtonClicked()
{
    clickCountForComparison++;
    if (clickCountForComparison == 1)
    {
        ui->compareLabel->setText("Теперь выберите 2-ой автомобиль");
        carIndexForComparison1 = ui->listWidget->currentIndex().row() + 1;
    }
    if (clickCountForComparison == 2)
    {
        carIndexForComparison2 = ui->listWidget->currentIndex().row() + 1;
        clickCountForComparison = 0;
        getDataForComparison();
    }
}
void CatalogWindow::onCarAddingWindowClosed()
{
    carAddingWindow->close();
    ui->listWidget->clear();
    addItemsToList();
}
void CatalogWindow::addItemsToLists()
{
    RepositoryForCars<ElectricEngineCar> rep;
    rep.getAllCars(combustionCarsList, electricCarsList, hybridCarsList);
}
void CatalogWindow::onFilterComboBoxChanged()
{
    clickCountForComparison = 0;
    ui->compareLabel->setText("Для сравнения двух автомобилей необходимо\nвыбрать первый, нажать кнопку, затем выбрать\nвторой и нажать снова.");
    ui->listWidget->clear();
    if (ui->filterComboBox->currentIndex() == 0)
        addItemsToList();
    std::string carData;
    std::string temp;
    int count = 1;
    if (ui->filterComboBox->currentIndex() == 1)
    {
        ElectricEngineCar car;
        Node<ElectricEngineCar> node;
        for (List<ElectricEngineCar>::Iterator it(electricCarsList.begin()); it != electricCarsList.end(); ++it)
        {
           node = *it;
           car = node.car;
           carData = std::to_string(count) + "-й автомобиль:\n" + "  Год выпуска: " + std::to_string(car.getYear()) + "\n" + "  Пробег: " + std::to_string(car.getMileage()) + " км\n" + "  Стоимость: ";
           temp = std::to_string(car.getPrice());
           temp = temp.substr(0, temp.find(',') + 4) + " р.";
           carData += temp + "\n" + "  Марка: " + car.getBrand() + "\n" + "  Модель: " + car.getModel() + "\n" + "  Емкость аккумулятора: ";
           temp = std::to_string(car.getBatteryCapacity());
           temp = temp.substr(0, temp.find(',') + 2) + " Ач";
           carData += temp + "\n";
           count++;
           ui->listWidget->addItem(QString::fromStdString(carData));
        }
    }
    count = 1;
    if (ui->filterComboBox->currentIndex() == 2)
    {
        CombustionEngineCar car;
        Node<CombustionEngineCar> node;
        for (List<CombustionEngineCar>::Iterator it(combustionCarsList.begin()); it != combustionCarsList.end(); it++)
        {
            node = *it;
            car = node.car;
            carData = std::to_string(count) + "-й автомобиль:\n" + "  Год выпуска: " + std::to_string(car.getYear()) + "\n" + "  Пробег в км: " + std::to_string(car.getMileage()) + "\n" + "  Стоимость: ";
            temp = std::to_string(car.getPrice());
            temp = temp.substr(0, temp.find(',') + 4) + " р.";
            carData += temp + "\n" + "  Марка: " + car.getBrand() + "\n" + "  Модель: " + car.getModel() + "\n" + "  Объем топливного бака: ";
            temp = std::to_string(car.getFuelTankCapacity());
            temp = temp.substr(0, temp.find(',') + 2) + " л";
            carData += temp + "\n";
            count++;
            ui->listWidget->addItem(QString::fromStdString(carData));
        }
    }
    count = 1;
    if (ui->filterComboBox->currentIndex() == 3)
    {
        HybridEngineCar car;
        Node<HybridEngineCar> node;
        for (List<HybridEngineCar>::Iterator it(hybridCarsList.begin()); it != hybridCarsList.end(); ++it)
        {
            node = *it;
            car = node.car;
            carData = std::to_string(count) + "-й автомобиль:\n" + "  Год выпуска: " + std::to_string(car.getYear()) + "\n" + "  Пробег в км: " + std::to_string(car.getMileage()) + "\n" + "  Стоимость: ";
            temp = std::to_string(car.getPrice());
            temp = temp.substr(0, temp.find(',') + 4) + " р.";
            carData += temp + "\n" + "  Марка: " + car.getBrand() + "\n" + "  Модель: " + car.getModel() + "\n" + "  Объем топливного бака: ";
            temp = std::to_string(car.getFuelTankCapacity());
            temp = temp.substr(0, temp.find(',') + 2) + " л";
            carData += temp + "\n" + "  Емкость аккумулятора: ";
            temp = std::to_string(car.getBatteryCapacity());
            temp = temp.substr(0, temp.find(',') + 2) + " Ач";
            carData += temp + "\n";
            if (car.getHybridType() == 0)
                carData = carData + "  Тип гибридного двигателя: последовательный" + "\n";
            else
                carData = carData + "  Тип гибридного двигателя: параллельный" + "\n";
            count++;
            ui->listWidget->addItem(QString::fromStdString(carData));
        }
    }
}
void CatalogWindow::onBuyPushButtonClicked()
{
    PurchaseWindow* window = new PurchaseWindow(this);
    connect(window, &PurchaseWindow::purchaseWindowClosed, this, &CatalogWindow::onPurchaseWindowClosed);
    if (ui->listWidget->currentIndex().row() + 1 <= combustionCarsList.size())
    {
        auto it = combustionCarsList.begin();
        Node<CombustionEngineCar> node;
        for (int i = 0; i < ui->listWidget->currentIndex().row(); ++i)
        {
            ++it;
        }
        node = *it;
        window->displayInfoAboutCar<CombustionEngineCar>(node.car, node.id);
    }
    if (ui->listWidget->currentIndex().row() + 1 > combustionCarsList.size() && ui->listWidget->currentIndex().row() + 1 <= combustionCarsList.size() + electricCarsList.size())
    {
        auto it = electricCarsList.begin();
        Node<ElectricEngineCar> node;
        for (int i = 0;i < ui->listWidget->currentIndex().row() - combustionCarsList.size(); ++i)
        {
            ++it;
        }
        node = *it;
        window->displayInfoAboutCar<ElectricEngineCar>(node.car, node.id);
    }
    if (ui->listWidget->currentIndex().row() + 1 > combustionCarsList.size() + electricCarsList.size() && ui->listWidget->currentIndex().row() + 1 <= combustionCarsList.size() + electricCarsList.size() + hybridCarsList.size())
    {
        auto it = hybridCarsList.begin();
        for (int i = 0; i < ui->listWidget->currentIndex().row() - combustionCarsList.size() - electricCarsList.size(); ++i)
        {
            ++it;
        }
        Node<HybridEngineCar> node;
        node = *it;
        window->displayInfoAboutCar<HybridEngineCar>(node.car, node.id);
    }
    window->show();
}
void CatalogWindow::checkForRights()
{
    if (!sessionStatus->getIsAdmin())
    {
        ui->addPushButton->hide();
        ui->deletePushButton->hide();
        ui->actionsLabel->hide();
    }
    if (!sessionStatus->getIsAuthorized())
        ui->warningLabel->show();
    else
        ui->warningLabel->hide();
}
void CatalogWindow::onPurchaseWindowClosed()
{
    combustionCarsList.clear();
    electricCarsList.clear();
    hybridCarsList.clear();
    ui->listWidget->clear();
    addItemsToList();
    addItemsToLists();
}