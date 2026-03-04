#pragma once
#ifndef REPLENISHBALANCEWINDOW_H
#define REPLENISHBALANCEWINDOW_H

#include <QDialog>

namespace Ui {
    class ReplenishBalanceWindow;
}

class ReplenishBalanceWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ReplenishBalanceWindow(QWidget* parent = nullptr);
    ~ReplenishBalanceWindow();
signals:
    void replenishBalanceWindowClosed();
private:
    Ui::ReplenishBalanceWindow* ui;
    void setValidation();
private slots:
    void onCancelPushButtonClicked();
    void onReplenishPushButtonClicked();
protected:
    bool eventFilter(QObject* obj, QEvent* event) override;
};

#endif // REPLENISHBALANCEWINDOW_H
