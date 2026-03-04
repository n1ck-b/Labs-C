#include "caraddingwindow.h"
#include "ui_caraddingwindow.h"
#include "MainWindow.h"
#include "Functions.h"
#include <QMessageBox>
#include <QIntValidator>
#include <QDoubleValidator>

CarAddingWindow::CarAddingWindow(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::CarAddingWindow)
{
    ui->setupUi(this);
    connect(ui->engineTypeComboBox, &QComboBox::currentIndexChanged, this, &CarAddingWindow::checkForEngineType);
    connect(ui->backPushButton, &QPushButton::clicked, this, &CarAddingWindow::onBackPushButtonClicked);
    connect(ui->addPushButton, &QPushButton::clicked, this, &CarAddingWindow::onAddPushButtonClicked);
    setValidators();
    checkForEngineType();
}
CarAddingWindow::~CarAddingWindow()
{
    delete ui;
}
void CarAddingWindow::onAddPushButtonClicked()
{
    if ((ui->batteryLineEdit->text().isEmpty() &&ui->batteryLineEdit->isEnabled()) || ui->brandLineEdit->text().isEmpty() || (ui->fuelLineEdit->text().isEmpty() && ui->fuelLineEdit->isEnabled()) || ui->mileageLineEdit->text().isEmpty() || ui->modelLineEdit->text().isEmpty() || ui->priceLineEdit->text().isEmpty() || ui->yearLineEdit->text().isEmpty())
    {
        ui->warningLabel->setText(QString::fromUtf8("Заполните все поля"));
        return;
    }
    ui->warningLabel->clear();
    ElectricEngineCar electricCar;
    CombustionEngineCar combustionCar;
    HybridEngineCar hybridCar;
    const char* SQL;
    int engineType = ui->engineTypeComboBox->currentIndex() + 1;
    if (engineType == 1)
    {
        RepositoryForCars<ElectricEngineCar> rep1;
        QString temp = ui->yearLineEdit->text();
        electricCar.setYear(temp.toInt());
        temp = ui->mileageLineEdit->text();
        electricCar.setMileage(temp.toInt());
        std::string str = ui->priceLineEdit->text().toStdString();
        if (str.find(',') != std::string::npos)
            str.replace(str.find(','), 1, ".");
        electricCar.setPrice(stod(str));
        temp = ui->brandLineEdit->text();
        electricCar.setBrand(temp.toStdString());
        temp = ui->modelLineEdit->text();
        electricCar.setModel(temp.toStdString());
        str = ui->batteryLineEdit->text().toStdString();
        if (str.find(',') != std::string::npos)
            str.replace(str.find(','), 1, ".");
        electricCar.setBatteryCapacity(stod(str));
        SQL = "INSERT INTO electric_cars (year_of_production, mileage, price, brand, model, battery_capacity)"
            "VALUES (?, ?, ?, ?, ?, ?);";
        try
        {
            rep1.addCar(electricCar, SQL, "Cars.db");
        }
        catch (Exception& ex)
        {
            QMessageBox::critical(this, "Error", QString::fromStdString(ex.whatAndContext()));
        }
    }
    if (engineType == 2)
    {
        RepositoryForCars<CombustionEngineCar> rep2;
        QString temp = ui->yearLineEdit->text();
        combustionCar.setYear(temp.toInt());
        temp = ui->mileageLineEdit->text();
        combustionCar.setMileage(temp.toInt());
        std::string str = ui->priceLineEdit->text().toStdString();
        if (str.find(',') != std::string::npos)
            str.replace(str.find(','), 1, ".");
        combustionCar.setPrice(stod(str));
        temp = ui->brandLineEdit->text();
        combustionCar.setBrand(temp.toStdString());
        temp = ui->modelLineEdit->text();
        combustionCar.setModel(temp.toStdString());
        str = ui->fuelLineEdit->text().toStdString();
        if (str.find(',') != std::string::npos)
            str.replace(str.find(','), 1, ".");
        combustionCar.setFuelTankCapacity(stod(str));
        SQL = "INSERT INTO combustion_cars (year_of_production, mileage, price, brand, model, fuel_tank_capacity)"
            "VALUES (?, ?, ?, ?, ?, ?);";
        try
        {
            rep2.addCar(combustionCar, SQL, "Cars.db");
        }
        catch (Exception& ex)
        {
            QMessageBox::critical(this, "Error", QString::fromStdString(ex.whatAndContext()));
        }
    }
    if (engineType == 3)
    {
        RepositoryForCars<HybridEngineCar> rep3;
        QString temp = ui->yearLineEdit->text();
        hybridCar.setYear(temp.toInt());
        temp = ui->mileageLineEdit->text();
        hybridCar.setMileage(temp.toInt());
        std::string str = ui->priceLineEdit->text().toStdString();
        if (str.find(',') != std::string::npos)
            str.replace(str.find(','), 1, ".");
        hybridCar.setPrice(stod(str));
        temp = ui->brandLineEdit->text();
        hybridCar.setBrand(temp.toStdString());
        temp = ui->modelLineEdit->text();
        hybridCar.setModel(temp.toStdString());
        str = ui->fuelLineEdit->text().toStdString();
        if (str.find(',') != std::string::npos)
            str.replace(str.find(','), 1, ".");
        hybridCar.setFuelTankCapacity(stod(str));
        str = ui->batteryLineEdit->text().toStdString();
        if (str.find(',') != std::string::npos)
            str.replace(str.find(','), 1, ".");
        hybridCar.setBatteryCapacity(stod(str));
        hybridCar.setHybridType(ui->hybridTypeComboBox->currentIndex());
        SQL = "INSERT INTO hybrid_cars (year_of_production, mileage, price, brand, model, fuel_tank_capacity, battery_capacity, hybrid_type)"
            "VALUES (?, ?, ?, ?, ?, ?, ?, ?);";
        try
        {
            rep3.addCar(hybridCar, SQL, "Cars.db");
        }
        catch (Exception& ex)
        {
            QMessageBox::critical(this, "Error", QString::fromStdString(ex.whatAndContext()));
        }
    }
    close();
}
void CarAddingWindow::checkForEngineType()
{
    ui->fuelLineEdit->setEnabled(true);
    ui->batteryLineEdit->setEnabled(true);
    ui->hybridTypeComboBox->setEnabled(true);
    ui->fuelLineEdit->setStyleSheet("color: rgb(55, 51, 53); border-style: solid; border-width: 0px; background-color: rgb(200, 215, 210); padding: 5px");
    ui->hybridTypeComboBox->setStyleSheet("QComboBox { color: rgb(30, 36, 39); font: 600 9pt 'Montserrat'; border-style: solid; border-width: 0px; background-color: rgb(200, 215, 210); padding: 5px; } QComboBox::drop-down { border:none; } QComboBox::down-arrow { image: url(:/rcs/free-icon-down-arrow-2985150.png); wigth: 30px; height:30px; margin-right: 5px; }");
    ui->batteryLineEdit->setStyleSheet("color: rgb(55, 51, 53); border-style: solid; border-width: 0px; background-color: rgb(200, 215, 210); padding: 5px");
    int engineType = ui->engineTypeComboBox->currentIndex() + 1;
    if (engineType == 1)
    {
        ui->fuelLineEdit->setDisabled(true);
        ui->fuelLineEdit->setStyleSheet("QLineEdit:disabled { background-color: #9AB6AC; border: none; }");
        ui->hybridTypeComboBox->setDisabled(true);
        ui->hybridTypeComboBox->setStyleSheet("QComboBox::drop-down { border:none; } QComboBox::down-arrow { image: url(:/rcs/free-icon-down-arrow-2985150.png); wigth: 30px; height:30px; margin-right: 5px; } QComboBox:disabled { background-color: #9AB6AC; border: none; font-size: 9pt; }");
    }
    if (engineType == 2)
    {
        ui->batteryLineEdit->setDisabled(true);
        ui->batteryLineEdit->setStyleSheet("QLineEdit:disabled { background-color: #9AB6AC; border: none; }");
        ui->hybridTypeComboBox->setDisabled(true);
        ui->hybridTypeComboBox->setStyleSheet("QComboBox::drop-down { border:none; } QComboBox::down-arrow { image: url(:/rcs/free-icon-down-arrow-2985150.png); wigth: 30px; height:30px; margin-right: 5px; }QComboBox:disabled { background-color: #9AB6AC; border: none; font-size: 9pt; }");
    }
}
void CarAddingWindow::onBackPushButtonClicked()
{
    close();
}
void CarAddingWindow::closeEvent(QCloseEvent* event)
{
    emit carAddingWindowClosed();
    QDialog::closeEvent(event);
}
void CarAddingWindow::setValidators()
{
    QIntValidator* validatorForMileage = new QIntValidator(0, 2000000, this);
    ui->mileageLineEdit->setValidator(validatorForMileage);
    QIntValidator* validatorForYear = new QIntValidator(1800, 2024, this);
    ui->yearLineEdit->setValidator(validatorForYear);
    QDoubleValidator* validatorForPrice = new QDoubleValidator(1.0, 30000000.0, 3, this);
    ui->priceLineEdit->setValidator(validatorForPrice);
    QDoubleValidator* validatorForFuelAndBatteryCapacity = new QDoubleValidator(10.0, 1000.0, 3, this);
    ui->batteryLineEdit->setValidator(validatorForFuelAndBatteryCapacity);
    ui->fuelLineEdit->setValidator(validatorForFuelAndBatteryCapacity);
}