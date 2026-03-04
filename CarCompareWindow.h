#ifndef CARCOMPAREWINDOW_H
#define CARCOMPAREWINDOW_H

#include <vector>
#include <QDialog>
#include <QString>
#include <QListWidget>
#include <QInputDialog>
#include "ui_carcomparewindow.h"
#include "Functions.h"

namespace Ui {
    class CarCompareWindow;
}

class CarCompareWindow : public QDialog
{
    Q_OBJECT

public:
    explicit CarCompareWindow(QWidget* parent = nullptr);
    ~CarCompareWindow();
    template<typename T>
    void infoAboutFirstCar(T car)
    {
        ui->CarYearLabel1->setText(QString::number(car.getYear()));
        ui->CarMileageLabel1->setText(QString::number(car.getMileage()));
        ui->CarPriceLabel1->setText(QString::number(car.getPrice()));
        ui->CarBrandLabel1->setText(QString::fromStdString(car.getBrand()));
        ui->CarModelLabel1->setText(QString::fromStdString(car.getModel()));
        if constexpr (std::is_same_v<ElectricEngineCar, T> || std::is_same_v<HybridEngineCar, T>)
            ui->CarBatteryLabel1->setText(QString::number(car.getBatteryCapacity()));
        if constexpr (std::is_same_v<CombustionEngineCar, T> || std::is_same_v<HybridEngineCar, T>)
            ui->CarFuelLabel1->setText(QString::number(car.getFuelTankCapacity()));
        if constexpr (std::is_same_v<HybridEngineCar, T>)
        {
            if (car.getHybridType() == 0)
                ui->CarHybridTypeLabel1->setText(QString::fromUtf8("Последовательный"));
            if (car.getHybridType() == 1)
                ui->CarHybridTypeLabel1->setText(QString::fromUtf8("Параллельный"));
        }
    }
    template<typename T>
    void infoAboutSecondCar(T car)
    {
        ui->CarYearLabel2->setText(QString::number(car.getYear()));
        ui->CarMileageLabel2->setText(QString::number(car.getMileage()));
        ui->CarPriceLabel2->setText(QString::number(car.getPrice()));
        ui->CarBrandLabel2->setText(QString::fromStdString(car.getBrand()));
        ui->CarModelLabel2->setText(QString::fromStdString(car.getModel()));
        if constexpr (std::is_same_v<ElectricEngineCar, T> || std::is_same_v<HybridEngineCar, T>)
            ui->CarBatteryLabel2->setText(QString::number(car.getBatteryCapacity()));
        if constexpr (std::is_same_v<CombustionEngineCar, T> || std::is_same_v<HybridEngineCar, T>)
            ui->CarFuelLabel2->setText(QString::number(car.getFuelTankCapacity()));
        if constexpr (std::is_same_v<HybridEngineCar, T>)
        {
            if (car.getHybridType() == 0)
                ui->CarHybridTypeLabel2->setText(QString::fromUtf8("Последовательный"));
            if (car.getHybridType() == 1)
                ui->CarHybridTypeLabel2->setText(QString::fromUtf8("Параллельный"));
        }
    }
    void compareCars();
private:
    Ui::CarCompareWindow* ui;
private slots:
    void onBackPushButtonClicked();
};

#endif // CARCOMPAREWINDOW_H
