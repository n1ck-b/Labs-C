#include "registrationwindow.h"
#include "ui_registrationwindow.h"
#include "LoginWindow.h"
#include "Functions.h"
#include "SessionStatus.h"

extern SessionStatus* sessionStatus;

RegistrationWindow::RegistrationWindow(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::RegistrationWindow)
{
    ui->setupUi(this);
    connect(ui->backPushButton, &QPushButton::clicked, this, &RegistrationWindow::onBackPushButtonClicked);
    connect(ui->nameLineEdit, &QLineEdit::textChanged, this, &RegistrationWindow::onAnyTextEdited);
    connect(ui->loginLineEdit, &QLineEdit::textChanged, this, &RegistrationWindow::onAnyTextEdited);
    connect(ui->passwordLineEdit, &QLineEdit::textChanged, this, &RegistrationWindow::onAnyTextEdited);
    connect(ui->registrationPushButton, &QPushButton::clicked, this, &RegistrationWindow::onRegistrationPushButtonClicked);
    connect(ui->visiblePushButton, &QPushButton::clicked, this, &RegistrationWindow::onVisiblePushButtonClicked);
    connect(ui->loginLineEdit, &QLineEdit::textChanged, this, &RegistrationWindow::onLoginLabelTextEdited);
    ui->passwordLineEdit->setEchoMode(QLineEdit::Password);
    ui->registrationPushButton->setDisabled(true);
    ui->registrationPushButton->setStyleSheet("QPushButton:hover { background-color: #CC3329; } QPushButton { color: rgb(244, 240, 239); border-radius: 12px; border-style: solid; border-width: 0px; background-color: #A62921; padding: 5px }");
    ui->loginWarningLabel->hide();
}

RegistrationWindow::~RegistrationWindow()
{
    delete ui;
}
void RegistrationWindow::onBackPushButtonClicked()
{
    LoginWindow* window = new LoginWindow(this);
    close();
    window->show();
}
void RegistrationWindow::onAnyTextEdited()
{
    if (!ui->nameLineEdit->text().isEmpty() && !ui->loginLineEdit->text().isEmpty() && !ui->passwordLineEdit->text().isEmpty())
    {
        ui->registrationPushButton->setEnabled(true);
        ui->registrationPushButton->setStyleSheet("QPushButton:hover { background-color: #CC3329; } QPushButton { color: rgb(244, 240, 239); border-radius: 12px; border-style: solid; border-width: 0px; background-color: rgb(215, 67, 57); padding: 5px }");
        ui->warningLabel->hide();
    }
    else
    {
        ui->registrationPushButton->setDisabled(true);
        ui->registrationPushButton->setStyleSheet("QPushButton:hover { background-color: #CC3329; } QPushButton { color: rgb(244, 240, 239); border-radius: 12px; border-style: solid; border-width: 0px; background-color: #A62921; padding: 5px }");
        ui->warningLabel->show();
    }
}
void RegistrationWindow::onRegistrationPushButtonClicked()
{
    QString qstr = ui->loginLineEdit->text();
    if (RepositoryForUsers::isUserExisting(qstr.toStdString()))
        ui->loginWarningLabel->show();
    else
    {
        User user;
        qstr = ui->nameLineEdit->text();
        user.setName(qstr.toStdString());
        qstr = ui->loginLineEdit->text();
        user.setLogin(qstr.toStdString());
        qstr = ui->passwordLineEdit->text();
        user.setPassword(qstr.toStdString());
        RepositoryForUsers::addUser(user);
        sessionStatus->setIsAutorized(true);
        sessionStatus->setLogin(user.getLogin());
        sessionStatus->setName(user.getName());
        sessionStatus->setIsAdmin(false);
        sessionStatus->setBalance(0);
        emit successfulRegistration();
        close();
    }
}
void RegistrationWindow::onVisiblePushButtonClicked()
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
void RegistrationWindow::onLoginLabelTextEdited()
{
    ui->loginWarningLabel->hide();
}