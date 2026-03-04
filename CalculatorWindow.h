#pragma once
#ifndef CALCULATORWINDOW_H
#define CALCULATORWINDOW_H

#include <QDialog>

namespace Ui {
    class CalculatorWindow;
}

class CalculatorWindow : public QDialog
{
    Q_OBJECT

public:
    explicit CalculatorWindow(QWidget* parent = nullptr);
    ~CalculatorWindow();

private:
    Ui::CalculatorWindow* ui;
    void calculateLoan();
private slots:
    void onBackPushButtonClicked();
    void onCalculatePushButtonClicked();
};

#endif // CALCULATORWINDOW_H
