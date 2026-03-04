#pragma once
#ifndef ACCOUNTWINDOW_H
#define ACCOUNTWINDOW_H

#include <QDialog>
#include "Functions.h"
#include "List.h"
#include "Node.h"

namespace Ui {
    class AccountWindow;
}

template <typename T>
class List;

class AccountWindow : public QDialog
{
    Q_OBJECT

public:
    explicit AccountWindow(QWidget* parent = nullptr);
    ~AccountWindow();
signals:
    void logoutFromAccount();
private:
    Ui::AccountWindow* ui;
    List<CombustionEngineCar> combustionCarsList;
    List<ElectricEngineCar> electricCarsList;
    List<HybridEngineCar> hybridCarsList;
private slots:
    void onBackPushButtonClicked();
    void onLogoutPushButtonClicked();
    void addCarsToList();
    void onReplenishPushButtonClicked();
    void onReplenishWindowClosed();
    void onTradeinPushButtonClicked();
};

#endif // ACCOUNTWINDOW_H
