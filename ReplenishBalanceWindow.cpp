#include "replenishbalancewindow.h"
#include "ui_replenishbalancewindow.h"
#include <QDoubleValidator>
#include "SessionStatus.h"

extern SessionStatus* sessionStatus;

ReplenishBalanceWindow::ReplenishBalanceWindow(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::ReplenishBalanceWindow)
{
    ui->setupUi(this);
    setValidation();
    ui->warningLabel->hide();
    connect(ui->cancelPushButton, &QPushButton::clicked, this, &ReplenishBalanceWindow::onCancelPushButtonClicked);
    connect(ui->replenishPushButton, &QPushButton::clicked, this, &ReplenishBalanceWindow::onReplenishPushButtonClicked);
}

ReplenishBalanceWindow::~ReplenishBalanceWindow()
{
    delete ui;
}
void ReplenishBalanceWindow::setValidation()
{
    ui->cardNumberLineEdit->setInputMask("9999 9999 9999 9999");
    ui->cardNumberLineEdit->setText("1234 5678 9012 3456");
    ui->cardNumberLineEdit->setStyleSheet("color: rgb(77,77,77); font: 600 9pt 'Montserrat'; border-style: solid; border-width: 0px; padding: 5px; background-color: rgb(200, 215, 210); ");
    ui->validityPeriodLineEdit->setInputMask("99/99");
    ui->validityPeriodLineEdit->setText("00/00");
    ui->validityPeriodLineEdit->setStyleSheet("color: rgb(77,77,77); font: 600 9pt 'Montserrat'; border-style: solid; border-width: 0px; padding: 5px; background-color: rgb(200, 215, 210); ");
    ui->CVCLineEdit->setInputMask("999");
    QDoubleValidator* doubleValidator = new QDoubleValidator(0.0, 350000.0, 3, this);
    doubleValidator->setNotation(QDoubleValidator::StandardNotation);
    ui->sumLineEdit->setValidator(doubleValidator);
    ui->cardNumberLineEdit->setCursorPosition(0);
    ui->validityPeriodLineEdit->setCursorPosition(0);
    ui->CVCLineEdit->setCursorPosition(0);
    ui->carHolderNameLineEdit->installEventFilter(this);
    ui->validityPeriodLineEdit->installEventFilter(this);
    ui->cardNumberLineEdit->installEventFilter(this);
    ui->CVCLineEdit->installEventFilter(this);
    ui->sumLineEdit->installEventFilter(this);
}
bool ReplenishBalanceWindow::eventFilter(QObject* obj, QEvent* event)
{
    QString qstr = ui->cardNumberLineEdit->text();
    if (obj == ui->cardNumberLineEdit && qstr == "1234 5678 9012 3456" && event->type() == QEvent::MouseButtonPress)
    {
        ui->cardNumberLineEdit->clear();
        ui->cardNumberLineEdit->setStyleSheet("color: rgb(55, 51, 53); font: 600 9pt 'Montserrat'; border-style: solid; border-width: 0px; padding: 5px; background-color: rgb(200, 215, 210); ");
        ui->cardNumberLineEdit->setCursorPosition(0);
        return true;
    }
    if (obj == ui->cardNumberLineEdit && qstr[0] == " " && event->type() == QEvent::FocusOut)
    {
        ui->cardNumberLineEdit->setText("1234 5678 9012 3456");
        ui->cardNumberLineEdit->setStyleSheet("color: rgb(77,77,77); font: 600 9pt 'Montserrat'; border-style: solid; border-width: 0px; padding: 5px; background-color: rgb(200, 215, 210); ");
    }
    if (obj == ui->CVCLineEdit && ui->CVCLineEdit->text().isEmpty() && event->type() == QEvent::MouseButtonPress)
    {
        ui->CVCLineEdit->setCursorPosition(0);
        return true;
    }
    if (obj == ui->sumLineEdit && ui->sumLineEdit->text().isEmpty() && event->type() == QEvent::MouseButtonPress)
    {
        ui->sumLineEdit->setCursorPosition(0);
        return true;
    }
    if (obj == ui->carHolderNameLineEdit && ui->carHolderNameLineEdit->text().isEmpty() && event->type() == QEvent::MouseButtonPress)
    {
        ui->carHolderNameLineEdit->setCursorPosition(0);
        return true;
    }
    qstr = ui->validityPeriodLineEdit->text();
    if (obj == ui->validityPeriodLineEdit && qstr == "00/00" && event->type() == QEvent::MouseButtonPress)
    {
        ui->validityPeriodLineEdit->clear();
        ui->validityPeriodLineEdit->setStyleSheet("color: rgb(55, 51, 53); font: 600 9pt 'Montserrat'; border-style: solid; border-width: 0px; padding: 5px; background-color: rgb(200, 215, 210); ");
        ui->validityPeriodLineEdit->setCursorPosition(0);
        return true;
    }
    if (obj == ui->validityPeriodLineEdit && qstr[0] == "/" && event->type() == QEvent::FocusOut)
    {
        ui->validityPeriodLineEdit->setText("00/00");
        ui->validityPeriodLineEdit->setStyleSheet("color: rgb(77,77,77); font: 600 9pt 'Montserrat'; border-style: solid; border-width: 0px; padding: 5px; background-color: rgb(200, 215, 210); ");
    }
    return false;
}
void ReplenishBalanceWindow::onCancelPushButtonClicked()
{
    close();
}
void ReplenishBalanceWindow::onReplenishPushButtonClicked()
{
    if ((ui->cardNumberLineEdit->text()) == "1234 5678 9012 3456" || (ui->validityPeriodLineEdit->text()) == "00/00" || ui->CVCLineEdit->text().isEmpty() || ui->carHolderNameLineEdit->text().isEmpty() || ui->sumLineEdit->text().isEmpty())
        ui->warningLabel->show();
    else
    {
        ui->warningLabel->hide();
        float balance = ui->sumLineEdit->text().toFloat();
        float oldBalance = sessionStatus->getBalance();
        sessionStatus->setBalance(oldBalance + balance);
        RepositoryForUsers::updateBalance(sessionStatus->getBalance(), sessionStatus->getLogin());
        emit replenishBalanceWindowClosed();
        close();
    }
}