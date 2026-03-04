#include "loginwindow.h"
#include "ui_loginwindow.h"
#include "RegistrationWindow.h"
#include "RepositoryForUsers.h"
#include "SessionStatus.h"

extern SessionStatus* sessionStatus;

LoginWindow::LoginWindow(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
    connect(ui->backPushButton, &QPushButton::clicked, this, &LoginWindow::onBackPushButtonClicked);
    connect(ui->visiblePushButton, &QPushButton::clicked, this, &LoginWindow::onVisiblePushButtonClicked);
    connect(ui->passwordLineEdit, &QLineEdit::textChanged, this, &LoginWindow::inputValidation);
    connect(ui->loginLineEdit, &QLineEdit::textChanged, this, &LoginWindow::inputValidation);
    connect(ui->loginPushButton, &QPushButton::clicked, this, &LoginWindow::onLoginPushButtonClicked);
    connect(ui->registrationPushButton, &QPushButton::clicked, this, &LoginWindow::onRegistrationPushButton);
    ui->passwordLineEdit->setEchoMode(QLineEdit::Password);
    ui->loginPushButton->setDisabled(true);
    ui->loginPushButton->setStyleSheet("QPushButton:hover { background-color: #CC3329; } QPushButton { color: rgb(244, 240, 239); border-radius: 12px; border-style: solid; border-width: 0px; background-color: #A62921; padding: 5px }");
    ui->warningLabel->hide();
}

LoginWindow::~LoginWindow()
{
    delete ui;
}
void LoginWindow::onBackPushButtonClicked()
{
    close();
}
void LoginWindow::onVisiblePushButtonClicked()
{
    if (ui->passwordLineEdit->echoMode() == QLineEdit::Password)
    {
        ui->visiblePushButton->setStyleSheet("border: none; image: url(:/rcs/hide.png);");
        ui->passwordLineEdit->setEchoMode(QLineEdit::Normal);
    }
    else if (ui->passwordLineEdit->echoMode() == QLineEdit::Normal)
    {
        ui->visiblePushButton->setStyleSheet("border: none; image: url(:/rcs/view.png);");
        ui->passwordLineEdit->setEchoMode(QLineEdit::Password);
    }
}
void LoginWindow::inputValidation()
{
    if (!ui->loginLineEdit->text().isEmpty() && !ui->passwordLineEdit->text().isEmpty())
    {
        ui->loginPushButton->setStyleSheet("QPushButton:hover { background-color: #CC3329; } QPushButton { color: rgb(244, 240, 239); border-radius: 12px; border-style: solid; border-width: 0px; background-color: rgb(215, 67, 57); padding: 5px }");
        ui->loginPushButton->setEnabled(true);
    }
    else
    {
        ui->loginPushButton->setDisabled(true);
        ui->loginPushButton->setStyleSheet("QPushButton:hover { background-color: #CC3329; } QPushButton { color: rgb(244, 240, 239); border-radius: 12px; border-style: solid; border-width: 0px; background-color: #A62921; padding: 5px }");
    }
}
void LoginWindow::onLoginPushButtonClicked()
{
    if (!RepositoryForUsers::isUserExisting(ui->loginLineEdit->text().toStdString()) || !RepositoryForUsers::isPasswordCorrect(ui->loginLineEdit->text().toStdString(), ui->passwordLineEdit->text().toStdString()))
    {
        ui->warningLabel->show();
    }
    else
    {
        ui->warningLabel->hide();
        User user;
        RepositoryForUsers::getUserInfo(user, ui->loginLineEdit->text().toStdString());
        sessionStatus->setIsAutorized(true);
        sessionStatus->setIsAdmin(user.getIsAdmin());
        sessionStatus->setLogin(user.getLogin());
        sessionStatus->setName(user.getName());
        sessionStatus->setBalance(user.getBalance());
        sessionStatus->setDiscountAmount(user.getDiscountAmount());
        sessionStatus->setPercentageOfDiscount(user.getPercentageOfDiscount());
        emit successfulAuthentication();
        close();
    }
}
void LoginWindow::onRegistrationPushButton()
{
    close();
    RegistrationWindow* window = new RegistrationWindow(this);
    window->show();
    connect(window, &RegistrationWindow::successfulRegistration, this, &LoginWindow::onSuccessfulRegistration);
}
void LoginWindow::onSuccessfulRegistration()
{
    emit successfulRegistration();
}