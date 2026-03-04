#include "calculatorwindow.h"
#include "ui_calculatorwindow.h"
#include <QValidator>
#include <cmath>

CalculatorWindow::CalculatorWindow(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::CalculatorWindow)
{
    ui->setupUi(this);
    connect(ui->backPushButton, &QPushButton::clicked, this, &CalculatorWindow::onBackPushButtonClicked);
    connect(ui->calculatePushButton, &QPushButton::clicked, this, &CalculatorWindow::onCalculatePushButtonClicked);
    QDoubleValidator* doubleValidator = new QDoubleValidator(0.0, 30000000.0, 3, this);
    ui->loanAmountLineEdit->setValidator(doubleValidator);
    QIntValidator* termValidator = new QIntValidator(0, 120, this);
    ui->loanTermLineEdit->setValidator(termValidator);
    QIntValidator* rateValidator = new QIntValidator(0, 100, this);
    ui->loanRateLineEdit->setValidator(rateValidator);
    ui->amountWarningLabel->hide();
    ui->rateWarningLabel->hide();
    ui->termWarningLabel->hide();
}

CalculatorWindow::~CalculatorWindow()
{
    delete ui;
}
void CalculatorWindow::onBackPushButtonClicked()
{
    close();
}
void CalculatorWindow::calculateLoan()
{
    int loanTerm = ui->loanTermLineEdit->text().toInt();
    float loanAmount = ui->loanAmountLineEdit->text().toFloat();
    double loanRate = (ui->loanRateLineEdit->text().toInt());
    double monthLoanRate = (loanRate / 12) / 100;
    double annuityRate = (monthLoanRate * pow(1 + monthLoanRate, loanTerm)) / (pow(1 + monthLoanRate, loanTerm) - 1);
    double monthlyPayment = loanAmount * annuityRate;
    double overpayment = monthlyPayment * loanTerm - loanAmount;
    double sumOfPayments = monthlyPayment * loanTerm;
    double percentageOfOverpayment = (overpayment / loanAmount) * 100;
    ui->monthlyPaymentAmountLabel->setText(QString::number(monthlyPayment) + " р.");
    ui->overpaymentAmountLabel->setText(QString::number(overpayment) + " р.");
    ui->sumOfPaymentsAmountLabel->setText(QString::number(sumOfPayments) + " р.");
    ui->overpaymentPercentageAmountLAbel->setText(QString::number(percentageOfOverpayment) + "%");
}
void CalculatorWindow::onCalculatePushButtonClicked()
{
    bool isAllLabelsFilledIn = true;
    if (ui->loanAmountLineEdit->text().isEmpty())
    {
        ui->amountWarningLabel->show();
        isAllLabelsFilledIn = false;
    }
    else
        ui->amountWarningLabel->hide();
    if (ui->loanTermLineEdit->text().isEmpty())
    {
        ui->termWarningLabel->show();
        isAllLabelsFilledIn = false;
    }
    else
        ui->termWarningLabel->hide();
    if (ui->loanRateLineEdit->text().isEmpty())
    {
        ui->rateWarningLabel->show();
        isAllLabelsFilledIn = false;
    }
    else
        ui->rateWarningLabel->hide();
    if (ui->loanRateLineEdit->text().toInt() > 100)
    {
        ui->rateWarningLabel->setText(QString::fromUtf8("Значение превышает 100%"));
        ui->rateWarningLabel->show();
    }
    else
    {
        ui->rateWarningLabel->setText(QString::fromUtf8("Заполните данное поле"));
    }
    if(isAllLabelsFilledIn && ui->loanRateLineEdit->text().toInt() <= 100)
        calculateLoan();
}