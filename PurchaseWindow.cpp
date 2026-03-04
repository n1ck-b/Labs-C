#include "purchasewindow.h"
#include "ui_purchasewindow.h"
#include "SessionStatus.h"

extern SessionStatus* sessionStatus;

PurchaseWindow::PurchaseWindow(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::PurchaseWindow)
{
    ui->setupUi(this);
    connect(ui->backPushButton, &QPushButton::clicked, this, &PurchaseWindow::onBackPushButtonClicked);
    connect(ui->purchasePushButton, &QPushButton::clicked, this, &PurchaseWindow::onPurchasePushButtonClicked);
    connect(ui->discountAmountCheckBox, &QCheckBox::checkStateChanged, this, &PurchaseWindow::onDiscountAmountCheckBoxChecked);
    connect(ui->discountAmountCheckBox, &QCheckBox::checkStateChanged, this, &PurchaseWindow::showOrHideBalance);
    ui->warningLabel->hide();
    showOrHideBalance();
}

PurchaseWindow::~PurchaseWindow()
{
    delete ui;
}
void PurchaseWindow::onBackPushButtonClicked()
{
    close();
}
void PurchaseWindow::onPurchasePushButtonClicked()
{
    if (!ui->discountAmountCheckBox->isChecked() && sessionStatus->getBalance() < carPrice - carPrice * (sessionStatus->getPercentageOfDiscount() / 100) || ui->discountAmountCheckBox->isChecked() && sessionStatus->getBalance() < carPrice - carPrice * (sessionStatus->getPercentageOfDiscount() / 100) - sessionStatus->getDiscountAmount())
    {
        ui->warningLabel->show();
    }
    else
    {
        RepositoryForUsersBoughtCars<ElectricEngineCar>::addCar(carIdInDB, sessionStatus->getLogin(), engineType);
        std::string tableName;
        if (engineType == "electric")
        {
            tableName = "electric_cars";
        }
        else if (engineType == "combustion")
        {
            tableName = "combustion_cars";
        }
        else if (engineType == "hybrid")
        {
            tableName = "hybrid_cars";
        }
        RepositoryForCars<ElectricEngineCar>::setCarAsSold(carIdInDB, tableName);
        double newDiscountAmount = sessionStatus->getDiscountAmount();
        if (ui->discountAmountCheckBox->isChecked())
        {
            if (carPrice - sessionStatus->getDiscountAmount() - carPrice * (sessionStatus->getPercentageOfDiscount() / 100) >= 0)
            {
                carPrice = carPrice - carPrice * (sessionStatus->getPercentageOfDiscount() / 100) - sessionStatus->getDiscountAmount();
                newDiscountAmount = 0;
            }
            if (carPrice - sessionStatus->getDiscountAmount() - carPrice * (sessionStatus->getPercentageOfDiscount() / 100) < 0)
            {
                newDiscountAmount = sessionStatus->getDiscountAmount() - carPrice + carPrice * (sessionStatus->getPercentageOfDiscount() / 100);
                carPrice = 0;
            }
        }
        else
            carPrice -= carPrice * (sessionStatus->getPercentageOfDiscount() / 100);
        RepositoryForUsers::userBoughtCar(sessionStatus->getLogin(), sessionStatus->getBalance(), carPrice);
        RepositoryForUsers::setDiscountAmount(sessionStatus->getLogin(), newDiscountAmount);
        sessionStatus->setDiscountAmount(newDiscountAmount);
        float balance = sessionStatus->getBalance();
        sessionStatus->setBalance(balance - carPrice);
        sessionStatus->setPercentageOfDiscount(RepositoryForUsers::getPercentageOfDiscount(sessionStatus->getLogin()));
        emit purchaseWindowClosed();
        close();
    }
}
void PurchaseWindow::showOrHideBalance()
{
    if (ui->discountAmountCheckBox->isChecked())
    {
        if (sessionStatus->getBalance() < carPrice - sessionStatus->getDiscountAmount() - carPrice * (sessionStatus->getPercentageOfDiscount() / 100))
        {
            ui->warningLabel->show();
            ui->infoAboutBalanceLabel->hide();
            ui->balanceLabel->hide();
        }
        else
        {
            ui->warningLabel->hide();
            ui->infoAboutBalanceLabel->show();
            ui->balanceLabel->show();
            if (carPrice - sessionStatus->getDiscountAmount() - carPrice * (sessionStatus->getPercentageOfDiscount() / 100) >= 0)
                ui->balanceLabel->setText(QString::number(sessionStatus->getBalance() - carPrice + carPrice * (sessionStatus->getPercentageOfDiscount() / 100) + sessionStatus->getDiscountAmount()) + " р.");
            else
                ui->balanceLabel->setText(QString::number(sessionStatus->getBalance()) + " р.");
        }
    }
    else
    {
        if (sessionStatus->getBalance() < carPrice - carPrice * (sessionStatus->getPercentageOfDiscount() / 100))
        {
            ui->warningLabel->show();
            ui->infoAboutBalanceLabel->hide();
            ui->balanceLabel->hide();
        }
        else
        {
            ui->warningLabel->hide();
            ui->infoAboutBalanceLabel->show();
            ui->balanceLabel->show();
            ui->balanceLabel->setText(QString::number(sessionStatus->getBalance() - carPrice + carPrice * (sessionStatus->getPercentageOfDiscount() / 100)) + " р.");
        }
    }
}
void PurchaseWindow::onDiscountAmountCheckBoxChecked()
{
    if (ui->discountAmountCheckBox->isChecked())
    {
        std::string str;
        if (carPrice - carPrice * (sessionStatus->getPercentageOfDiscount() / 100) - sessionStatus->getDiscountAmount() >= 0)
        {
            str = to_string(carPrice - carPrice * (sessionStatus->getPercentageOfDiscount() / 100) - sessionStatus->getDiscountAmount());
            str = str.substr(0, str.find(',') + 4) + " р.";
        }
        else
        {
            str = "0 р.";
        }
        if (sessionStatus->getPercentageOfDiscount() == 0)
        {
            ui->priceValueLabelWithDiscount->clear();
            ui->priceValueLabel->setStyleSheet("background-color: #E0D5D2; padding: 5px;");
            ui->priceValueLabel->setText(QString::fromStdString(str));
        }
        else
        {
            ui->priceValueLabel->setStyleSheet("background-color: #E0D5D2; padding: 5px; text-decoration: line-through;");
            ui->priceValueLabelWithDiscount->setText(QString::fromStdString(str));
        }
    }
    if (!ui->discountAmountCheckBox->isChecked())
    {
        std::string str = to_string(carPrice - carPrice * (sessionStatus->getPercentageOfDiscount() / 100));
        str = str.substr(0, str.find(',') + 4) + " р.";
        if (sessionStatus->getPercentageOfDiscount() == 0)
        {
            ui->priceValueLabelWithDiscount->clear();
            ui->priceValueLabel->setStyleSheet("background-color: #E0D5D2; padding: 5px;");
            ui->priceValueLabel->setText(QString::fromStdString(str));
        }
        else
        {
            ui->priceValueLabel->setStyleSheet("background-color: #E0D5D2; padding: 5px; text-decoration: line-through;");
            ui->priceValueLabelWithDiscount->setText(QString::fromStdString(str));
        }
    }
}
void PurchaseWindow::showDiscount()
{
    if (sessionStatus->getPercentageOfDiscount() != 0)
    {
        std::string str = to_string(carPrice - carPrice * (sessionStatus->getPercentageOfDiscount() / 100));
        str = str.substr(0, str.find(',') + 4) + " р.";
        ui->priceValueLabel->setStyleSheet("background-color: #E0D5D2; padding: 5px; text-decoration: line-through;");
        ui->priceValueLabelWithDiscount->setText(QString::fromStdString(str));
        if (sessionStatus->getPercentageOfDiscount() == 5)
            ui->remarkLabel->setText(QString::fromUtf8("*Действует скидка на 1-ый автомобиль\nили за покупку 10 авто"));
        else if (sessionStatus->getPercentageOfDiscount() == 3)
            ui->remarkLabel->setText(QString::fromUtf8("*Действует скидка за покупку 5 авто"));
        else if (sessionStatus->getPercentageOfDiscount() == 7)
            ui->remarkLabel->setText(QString::fromUtf8("*Действует скидка за покупку 20 авто"));
    }
    else
        ui->remarkLabel->setText(QString::fromUtf8("*У вас нет скидок за кол-во покупок"));
}