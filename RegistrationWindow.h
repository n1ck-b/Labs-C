#pragma once
#ifndef REGISTRATIONWINDOW_H
#define REGISTRATIONWINDOW_H

#include <QDialog>
#include <QString>

namespace Ui {
    class RegistrationWindow;
}

class RegistrationWindow : public QDialog
{
    Q_OBJECT

public:
    explicit RegistrationWindow(QWidget* parent = nullptr);
    ~RegistrationWindow();
signals:
    void successfulRegistration();
private:
    Ui::RegistrationWindow* ui;
private slots:
    void onBackPushButtonClicked();
    void onAnyTextEdited();
    void onRegistrationPushButtonClicked();
    void onVisiblePushButtonClicked();
    void onLoginLabelTextEdited();
};

#endif // REGISTRATIONWINDOW_H
