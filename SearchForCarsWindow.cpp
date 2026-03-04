#include "searchforcarswindow.h"
#include "ui_searchforcarswindow.h"
#include "Functions.h"
#include <QValidator>

SearchForCarsWindow::SearchForCarsWindow(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::SearchForCarsWindow)
{
    ui->setupUi(this);
    buttonGroup = new QButtonGroup(this);
    buttonGroup->addButton(ui->batteryCheckBox);
    buttonGroup->addButton(ui->fuelCheckBox);
    connect(ui->searchPushButton, &QPushButton::clicked, this, &SearchForCarsWindow::onSearchPushButtonClicked);
    connect(ui->hybridTypeCheckBox, &QCheckBox::checkStateChanged, this, &SearchForCarsWindow::onCheckBoxEdited);
    connect(ui->backPushButton, &QPushButton::clicked, this, &SearchForCarsWindow::onBackPushButtonClicked);
    connect(ui->savePushButton, &QPushButton::clicked, this, &SearchForCarsWindow::onSavePushButtonClicked);
    setValidators();
    QGraphicsDropShadowEffect* shadowEffect = new QGraphicsDropShadowEffect;
    shadowEffect->setColor(QColor(215, 67, 57));
    shadowEffect->setXOffset(0);
    shadowEffect->setYOffset(0);
    shadowEffect->setBlurRadius(10);
    ui->searchPushButton->setGraphicsEffect(shadowEffect);
    ui->listWidget->setFocusPolicy(Qt::NoFocus);
    ui->listWidget->setStyleSheet("QListWidget::item:selected { background-color: #E0E8E6; border-radius: 12px; padding: 2px; border: none; outline: none; margin-right: 10px; } QScrollBar { padding: 0px; margin: 0px; padding-bottom: 10px; background-color: rgb(244, 240, 239); border: 1px solid rgb(55, 51, 53); width: 15px; border-radius: 7px;} QScrollBar::handle:vertical { border: 1px solid rgb(55, 51, 53); background-color: rgb(244, 240, 239); border-radius: 4px; margin: 2px; margin-top: 20px; margin-bottom: 15px; min-height: 25px; }  QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical { background: none; } QScrollBar::add-line:vertical { width: 11px; height: 20px; padding: 0px; margin: 0px; border: none; image: url(:/rcs/free-icon-down-arrow-2985150.png); padding-left: 2px; padding-right: 2px; margin-top: 0px; margin-bottom: 0px; background-size: contain; background-position: center;} QScrollBar::sub-line:vertical { width: 11px; height: 20px; padding: 0px; margin: 0px; border: none; image: url(:/rcs/free-icon-down-arrow-29851501.png); padding-left: 2px; padding-right: 2px; margin-top: 0px; margin-bottom: 0px; background-size: contain; background-position: center;}");
    ui->savePushButton->setDisabled(true);
    ui->savePushButton->setStyleSheet("QPushButton:hover { background-color: #CC3329; } QPushButton { color: rgb(244, 240, 239); border-radius: 12px; border-style: solid; border-width: 0px; background-color: #A62921; padding: 5px }");
}
SearchForCarsWindow::~SearchForCarsWindow()
{
    delete ui;
}
void SearchForCarsWindow::createSQLQuery(std::string& sql, QString lowerValue, QString upperValue, bool& isFirst, std::string parameterName)
{
    if (!isFirst)
    {
        sql += " AND";
    }
    else
        isFirst = false;
    sql += " ( " + parameterName + " >= " + lowerValue.toStdString();
    sql += " AND " + parameterName + " <= " + upperValue.toStdString() + " )";

}
void SearchForCarsWindow::onSearchPushButtonClicked()
{
    ui->infoAboutFileLabel->setText("");
    ui->savePushButton->setDisabled(true);
    ui->savePushButton->setStyleSheet("QPushButton:hover { background-color: #CC3329; } QPushButton { color: rgb(244, 240, 239); border-radius: 12px; border-style: solid; border-width: 0px; background-color: #A62921; padding: 5px }");
    ui->listWidget->clear();
    std::string sql = "";
    std::string tableName = " ";
    bool isFirst = true;
    QString qstr;
    int engineType = 0;
    if (ui->brandCheckBox->isChecked())
    {
        isFirst = false;
        sql += " brand = '";
        qstr = ui->brandLineEdit->text();
        sql = sql + qstr.toStdString() + "'";
    }
    if (ui->modelCheckBox->isChecked())
    {
        if (!isFirst)
        {
            sql += " AND";
        }
        else
            isFirst = false;
        qstr = ui->modelLineEdit->text();
        sql += " model = '" + qstr.toStdString() + "'";
    }
    if (ui->yearCheckBox->isChecked())
    {
        if (ui->yearLineEditFrom->text().toInt() <= ui->yearLineEditTo->text().toInt())
            createSQLQuery(sql, ui->yearLineEditFrom->text(), ui->yearLineEditTo->text(), isFirst, "year_of_production");
        else
            createSQLQuery(sql, ui->yearLineEditTo->text(), ui->yearLineEditFrom->text(), isFirst, "year_of_production");
    }
    if (ui->mileageCheckBox->isChecked())
    {
        if (ui->mileageLineEditFrom->text().toInt() <= ui->mileageLineEditTo->text().toInt())
            createSQLQuery(sql, ui->mileageLineEditFrom->text(), ui->mileageLineEditTo->text(), isFirst, "mileage");
        else
            createSQLQuery(sql, ui->mileageLineEditTo->text(), ui->mileageLineEditFrom->text(), isFirst, "mileage");
    }
    if (ui->priceCheckBox->isChecked())
    {
        std::string temp1 = ui->priceLineEditFrom->text().toStdString();
        if(temp1.find(',') != std::string::npos)
            temp1.replace(temp1.find(','), 1, ".");
        std::string temp2 = ui->priceLineEditTo->text().toStdString();
        if (temp2.find(',') != std::string::npos)
            temp2.replace(temp2.find(','), 1, ".");
        if(ui->priceLineEditFrom->text().toFloat() <= ui->priceLineEditTo->text().toFloat())
            createSQLQuery(sql, QString::fromStdString(temp1), QString::fromStdString(temp2), isFirst, "price");
        else
            createSQLQuery(sql, QString::fromStdString(temp2), QString::fromStdString(temp1), isFirst, "price");
    }
    if (ui->fuelCheckBox->isChecked())
    {
        if (!ui->hybridTypeCheckBox->isChecked())
        {
            tableName = "combustion_cars";
            engineType = 2;
        }
        std::string temp1 = ui->fuelLineEditFrom->text().toStdString();
        if (temp1.find(',') != std::string::npos)
            temp1.replace(temp1.find(','), 1, ".");
        std::string temp2 = ui->fuelLineEditTo->text().toStdString();
        if (temp2.find(',') != std::string::npos)
            temp2.replace(temp2.find(','), 1, ".");
        if(ui->fuelLineEditFrom->text().toFloat() <= ui->fuelLineEditTo->text().toFloat())
            createSQLQuery(sql, QString::fromStdString(temp1), QString::fromStdString(temp2), isFirst, "fuel_tank_capacity");
        else
            createSQLQuery(sql, QString::fromStdString(temp2), QString::fromStdString(temp1), isFirst, "fuel_tank_capacity");
    }
    if (ui->batteryCheckBox->isChecked())
    {
        if (!ui->hybridTypeCheckBox->isChecked())
        {
            tableName = "electric_cars";
            engineType = 1;
        }
        std::string temp1 = ui->batteryLineEditFrom->text().toStdString();
        if (temp1.find(',') != std::string::npos)
            temp1.replace(temp1.find(','), 1, ".");
        std::string temp2 = ui->batteryLineEditTo->text().toStdString();
        if (temp2.find(',') != std::string::npos)
            temp2.replace(temp2.find(','), 1, ".");
        if(ui->batteryLineEditFrom->text().toFloat() <= ui->batteryLineEditTo->text().toFloat())
            createSQLQuery(sql, QString::fromStdString(temp1), QString::fromStdString(temp2), isFirst, "battery_capacity");
        else
            createSQLQuery(sql, QString::fromStdString(temp2), QString::fromStdString(temp1), isFirst, "battery_capacity");
    }
    if (ui->hybridTypeCheckBox->isChecked())
    {
        tableName = "hybrid_cars";
        engineType = 3;
        if (!isFirst)
        {
            sql += " AND";
        }
        else
            isFirst = false;
        int hybridType = ui->hybridTypeComboBox->currentIndex();
        sql += " hybrid_type = " + std::to_string(hybridType) + " ";
    }
    if (ui->typeOfEngineCheckBox->isChecked())
    {
        if (ui->typeOfEngineComboBox->currentIndex() == 0)
            tableName = "electric_cars";
        if (ui->typeOfEngineComboBox->currentIndex() == 1)
            tableName = "combustion_cars";
        if (ui->typeOfEngineComboBox->currentIndex() == 2)
            tableName = "hybrid_cars";
    }
    std::string SQL = "SELECT * FROM ";
    RepositoryForCars<ElectricEngineCar> rep;
    int count = 1;
    if (tableName != " ")
    {
        if(!sql.empty())
            SQL += tableName + " WHERE" + sql + "AND is_sold = 0;";
        else
            SQL += tableName + " WHERE is_sold = 0;";
        rep.searchForCarInDB(SQL.c_str(), "Cars.db", engineType, count, ui->listWidget, jsonArray);
        ui->listWidget->repaint();
    }
    else
    {
        SQL += "combustion_cars WHERE" + sql + "AND is_sold = 0;";
        rep.searchForCarInDB(SQL.c_str(), "Cars.db", 2, count, ui->listWidget, jsonArray);
        SQL = "SELECT * FROM electric_cars WHERE" + sql + "AND is_sold = 0;";
        rep.searchForCarInDB(SQL.c_str(),"Cars.db", 1, count, ui->listWidget, jsonArray);
        SQL = "SELECT * FROM hybrid_cars WHERE" + sql + "AND is_sold = 0;";
        rep.searchForCarInDB(SQL.c_str(),"Cars.db", 3, count, ui->listWidget, jsonArray);
        ui->listWidget->repaint();
    }
    if (ui->listWidget->count() != 0)
    {
        ui->savePushButton->setEnabled(true);
        ui->savePushButton->setStyleSheet("QPushButton:hover { background-color: #CC3329; } QPushButton { color: rgb(244, 240, 239); border-radius: 12px; border-style: solid; border-width: 0px; background-color: rgb(215, 67, 57); padding: 5px }");
    }
    if (ui->listWidget->count() == 0)
        ui->listWidget->addItem("По данным параметрам машин не найдено");
}
void SearchForCarsWindow::onCheckBoxEdited()
{
    buttonGroup->setExclusive(false);
    if (!ui->hybridTypeCheckBox->isChecked())
    {
        buttonGroup->setExclusive(true);
    }
}
void SearchForCarsWindow::onBackPushButtonClicked()
{
    close();
}
void SearchForCarsWindow::onSavePushButtonClicked()
{
    QFile jsonFile("SearchResults.json");
    try
    {
        if (!jsonFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
        {
            throw Exception("Unable to open/create file", "JSON File");
        }
    }
    catch (Exception& ex)
    {
        QMessageBox::critical(this, "Error", QString::fromStdString(ex.whatAndContext()));
    }
    jsonFile.write(QJsonDocument(jsonArray).toJson());
    jsonFile.flush();
    jsonFile.close();
    ui->infoAboutFileLabel->setText(QString::fromUtf8("Файл успешно сохранен"));
}
void SearchForCarsWindow::setValidators()
{
    QIntValidator* yearValidator = new QIntValidator(0, 2024, this);
    ui->yearLineEditFrom->setValidator(yearValidator);
    ui->yearLineEditTo->setValidator(yearValidator);
    auto mileageValidator = new QIntValidator(0, 2000000, this);
    ui->mileageLineEditFrom->setValidator(mileageValidator);
    ui->mileageLineEditTo->setValidator(mileageValidator);
    auto priceValidator = new QDoubleValidator(0.0, 3000000.0, 3, this);
    ui->priceLineEditFrom->setValidator(priceValidator);
    ui->priceLineEditTo->setValidator(priceValidator);
    auto capacityValidator = new QDoubleValidator(0.0, 200.0, 3, this);
    ui->fuelLineEditFrom->setValidator(capacityValidator);
    ui->fuelLineEditTo->setValidator(capacityValidator);
    ui->batteryLineEditFrom->setValidator(capacityValidator);
    ui->batteryLineEditTo->setValidator(capacityValidator);

}