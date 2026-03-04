#pragma once
#ifndef CARADDINGWINDOW_H
#define CARADDINGWINDOW_H

#include <QString>
#include <QListWidget>
#include <QDialog>
#include <QInputDialog>

namespace Ui {
    class CarAddingWindow;
}

class CarAddingWindow : public QDialog
{
    Q_OBJECT

public:
    explicit CarAddingWindow(QWidget* parent = nullptr);
    ~CarAddingWindow();
    void checkForEngineType();
signals:
    void carAddingWindowClosed();
private:
    Ui::CarAddingWindow* ui;
private slots:
    void onBackPushButtonClicked();
    void onAddPushButtonClicked();
    void setValidators();
protected:
    void closeEvent(QCloseEvent* event) override;
};

#endif // CARADDINGWINDOW_H
