#pragma once
#ifndef PURCHASEWINDOW_H
#define PURCHASEWINDOW_H

#include <QDialog>
#include "Functions.h"
#include "ui_PurchaseWindow.h"

namespace Ui {
    class PurchaseWindow;
}

class PurchaseWindow : public QDialog
{
    Q_OBJECT

public:
    explicit PurchaseWindow(QWidget* parent = nullptr);
    ~PurchaseWindow();
    template <typename T>
    void displayInfoAboutCar(T car, int id)
    {
        carIdInDB = id;
        carPrice = car.getPrice();
        ui->brandValueLabel->setText(QString::fromStdString(car.getBrand()));
        ui->modelValueLabel->setText(QString::fromStdString(car.getModel()));
        ui->yearValueLabel->setText(QString::number(car.getYear()));
        std::string str = to_string(car.getMileage()) + " км";
        ui->mileageValueLabel->setText(QString::fromStdString(str));
        str = to_string(car.getPrice());
        str = str.substr(0, str.find(',') + 4) + " р.";
        ui->priceValueLabel->setText(QString::fromStdString(str));
        if constexpr (std::is_same_v<CombustionEngineCar, T>)
        {
            engineType = "combustion";
            str = to_string(car.getFuelTankCapacity());
            str = str.substr(0, str.find(',')) + " л";
            ui->fuelTankValueLabel->setText(QString::fromStdString(str));
            ui->batteryCapacityLabel->hide();
            ui->batteryCapacityValueLabel->hide();
            ui->hybridTypeLabel->hide();
            ui->hybridTypeValueLabel->hide();
        }
        if constexpr (std::is_same_v<ElectricEngineCar, T>)
        {
            engineType = "electric";
            ui->fuelTankLabel->setText(QString::fromUtf8("Емкость\nаккумулятора:"));
            str = to_string(car.getBatteryCapacity());
            str = str.substr(0, str.find(',')) + " Ач";
            ui->fuelTankValueLabel->setText(QString::fromStdString(str));
            ui->batteryCapacityLabel->hide();
            ui->batteryCapacityValueLabel->hide();
            ui->hybridTypeLabel->hide();
            ui->hybridTypeValueLabel->hide();
        }
        if constexpr (std::is_same_v<HybridEngineCar, T>)
        {
            engineType = "hybrid";
            str = to_string(car.getFuelTankCapacity());
            str = str.substr(0, str.find(',')) + " л";
            ui->fuelTankValueLabel->setText(QString::fromStdString(str));
            str = to_string(car.getBatteryCapacity());
            str = str.substr(0, str.find(',')) + " Ач";
            ui->batteryCapacityValueLabel->setText(QString::fromStdString(str));
            if (car.getHybridType() == 0)
                ui->hybridTypeValueLabel->setText(QString::fromUtf8("Последовательный"));
            else
                ui->hybridTypeValueLabel->setText(QString::fromUtf8("Параллельный"));
        }
        showOrHideBalance();
        showDiscount();
    }
signals:
    void purchaseWindowClosed();
private:
    Ui::PurchaseWindow* ui;
    int carIdInDB;
    std::string engineType;
    float carPrice;
private slots:
    void onBackPushButtonClicked();
    void onPurchasePushButtonClicked();
    void showOrHideBalance();
    void onDiscountAmountCheckBoxChecked();
    void showDiscount();
};

#endif // PURCHASEWINDOW_H
