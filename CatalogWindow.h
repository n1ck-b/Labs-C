#pragma once
#ifndef CATALOGWINDOW_H
#define CATALOGWINDOW_H

#include <QString>
#include <QListWidget>
#include <QDialog>
#include <QInputDialog>
#include "CarAddingWindow.h"
#include "Functions.h"
#include "List.h"
#include "Node.h"

namespace Ui {
    class CatalogWindow;
}

template <typename T>
class List;

class CatalogWindow : public QDialog
{
    Q_OBJECT

public:
    explicit CatalogWindow(QWidget* parent = nullptr);
    ~CatalogWindow();
    void addItemsToList();
    void getDataForComparison();
    CarAddingWindow* carAddingWindow;
    template <typename T>
    void getIterator(int index, typename List<T>::Iterator& it)
    {
        for (int i = 0; i < index; ++i)
        {
            ++it;
        }
    }
    void getDataForOneCarFromAllCars(int& engineType, int carIndex, CombustionEngineCar& combustionCar, ElectricEngineCar& electricCar, HybridEngineCar& hybridCar);
private:
    Ui::CatalogWindow* ui;
    int clickCountForComparison = 0;
    int carIndexForComparison1 = 0;
    int carIndexForComparison2 = 0;
    List<CombustionEngineCar> combustionCarsList;
    List<ElectricEngineCar> electricCarsList;
    List<HybridEngineCar> hybridCarsList;
    void addItemsToLists();
    void checkForRights();
private slots:
    void onBackPushButtonClicked();
    void onAddPushButtonClicked();
    void choosedItemInList();
    void onDeletePushButtonClicked();
    void onComparePushButtonClicked();
    void onCarAddingWindowClosed();
    void onFilterComboBoxChanged();
    void onBuyPushButtonClicked();
    void onPurchaseWindowClosed();
};

#endif // CATALOGWINDOW_H