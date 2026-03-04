#pragma once
#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QDialog>

namespace Ui {
    class LoginWindow;
}

class LoginWindow : public QDialog
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget* parent = nullptr);
    ~LoginWindow();
signals:
    void successfulRegistration();
    void successfulAuthentication();
private:
    Ui::LoginWindow* ui;
private slots:
    void onBackPushButtonClicked();
    void onVisiblePushButtonClicked();
    void inputValidation();
    void onLoginPushButtonClicked();
    void onRegistrationPushButton();
    void onSuccessfulRegistration();
};

#endif // LOGINWINDOW_H
