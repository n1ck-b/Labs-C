#include "accountwindow.h"
#include "ui_accountwindow.h"
#include "SessionStatus.h"
#include "ReplenishBalanceWindow.h"

extern SessionStatus* sessionStatus;


AccountWindow::AccountWindow(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::AccountWindow)
{
    ui->setupUi(this);
    connect(ui->backPushButton, &QPushButton::clicked, this, &AccountWindow::onBackPushButtonClicked);
    connect(ui->logoutPushButton, &QPushButton::clicked, this, &AccountWindow::onLogoutPushButtonClicked);
    connect(ui->replenishBalancePushButton, &QPushButton::clicked, this, &AccountWindow::onReplenishPushButtonClicked);
    connect(ui->tradeInPushButton, &QPushButton::clicked, this, &AccountWindow::onTradeinPushButtonClicked);
    std::string temp = std::to_string(sessionStatus->getBalance());
    temp = temp.substr(0, temp.find(',') + 4) + " р.";
    ui->balanceAmountLabel->setText(QString::fromStdString(temp));
    temp = std::to_string(sessionStatus->getDiscountAmount());
    temp = temp.substr(0, temp.find(',') + 4) + +" р.";
    ui->tradeInAmountLabel->setText(QString::fromStdString(temp));
    std::string str = std::to_string(sessionStatus->getPercentageOfDiscount());
    str = str.substr(0, str.find(',') + 2);
    str += "%";
    ui->percentageofDiscountAmountLabel->setText(QString::fromStdString(str));
    ui->listWidget->addItem(QString::fromUtf8("У вас нет купленных машин"));
    ui->listWidget->setStyleSheet("QListWidget::item:selected { background-color: #E0E8E6; border-radius: 12px; padding: 2px; border: none; outline: none; margin-right: 10px; } QScrollBar { padding: 0px; margin: 0px; padding-bottom: 10px; background-color: rgb(244, 240, 239); border: 1px solid rgb(55, 51, 53); width: 15px; border-radius: 7px;} QScrollBar::handle:vertical { border: 1px solid rgb(55, 51, 53); background-color: rgb(244, 240, 239); border-radius: 4px; margin: 2px; margin-top: 20px; margin-bottom: 15px; min-height: 25px; }  QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical { background: none; } QScrollBar::add-line:vertical { width: 11px; height: 20px; padding: 0px; margin: 0px; border: none; image: url(:/rcs/free-icon-down-arrow-2985150.png); padding-left: 2px; padding-right: 2px; margin-top: 0px; margin-bottom: 0px; background-size: contain; background-position: center;} QScrollBar::sub-line:vertical { width: 11px; height: 20px; padding: 0px; margin: 0px; border: none; image: url(:/rcs/free-icon-down-arrow-29851501.png); padding-left: 2px; padding-right: 2px; margin-top: 0px; margin-bottom: 0px; background-size: contain; background-position: center;}");
    ui->listWidget->setFocusPolicy(Qt::NoFocus);
    ui->warningLabel->hide();
    addCarsToList();
    if (ui->listWidget->item(0)->text() == "У вас нет купленных машин")
    {
        ui->remarkLabel->hide();
        ui->tradeInPushButton->hide();
        ui->remarkLabelBottom->hide();
    }
    RepositoryForUsersBoughtCars<ElectricEngineCar>::getAllCars(sessionStatus->getLogin(), combustionCarsList, electricCarsList, hybridCarsList);
}

AccountWindow::~AccountWindow()
{
    delete ui;
}
void AccountWindow::onBackPushButtonClicked()
{
    close();
}
void AccountWindow::onLogoutPushButtonClicked()
{
    sessionStatus->setIsAutorized(false);
    sessionStatus->setDefault();
    emit logoutFromAccount();
    close();
}
void AccountWindow::addCarsToList()
{
    int count = 1;
    RepositoryForUsersBoughtCars<ElectricEngineCar>::printBoughtCars(sessionStatus->getLogin(), ui->listWidget, count);
    RepositoryForUsersBoughtCars<CombustionEngineCar>::printBoughtCars(sessionStatus->getLogin(), ui->listWidget, count);
    RepositoryForUsersBoughtCars<HybridEngineCar>::printBoughtCars(sessionStatus->getLogin(), ui->listWidget, count);
}
void AccountWindow::onReplenishPushButtonClicked()
{
    ReplenishBalanceWindow* window = new ReplenishBalanceWindow(this);
    window->show();
    connect(window, &ReplenishBalanceWindow::replenishBalanceWindowClosed, this, &AccountWindow::onReplenishWindowClosed);
}
void AccountWindow::onReplenishWindowClosed()
{
    std::string temp = std::to_string(sessionStatus->getBalance());
    temp = temp.substr(0, temp.find(',') + 4) + " р.";
    ui->balanceAmountLabel->setText(QString::fromStdString(temp));
}
void AccountWindow::onTradeinPushButtonClicked()
{
    if (ui->listWidget->selectedItems().size() == 0)
        ui->warningLabel->show();
    else
    {
        ui->warningLabel->hide();
        combustionCarsList.clear();
        electricCarsList.clear();
        hybridCarsList.clear();
        RepositoryForUsersBoughtCars<ElectricEngineCar>::getAllCars(sessionStatus->getLogin(), combustionCarsList, electricCarsList, hybridCarsList);
        if (ui->listWidget->currentIndex().row() + 1 <= electricCarsList.size())
        {
            auto it = electricCarsList.begin();
            Node<ElectricEngineCar> node;
            for (int i = 0; i < ui->listWidget->currentIndex().row(); ++i)
            {
                ++it;
            }
            node = *it;
            RepositoryForUsersBoughtCars<ElectricEngineCar>::tradeInCar(sessionStatus->getLogin(), "electric", node.id);
            RepositoryForUsers::tradeIn(node.car.getPrice(), sessionStatus->getLogin());
            sessionStatus->setDiscountAmount(node.car.getPrice() * 0.7);
        }
        if (ui->listWidget->currentIndex().row() + 1 > electricCarsList.size() && ui->listWidget->currentIndex().row() + 1 <= combustionCarsList.size() + electricCarsList.size())
        {
            auto it = combustionCarsList.begin();
            Node<CombustionEngineCar> node;
            for (int i = 0; i < ui->listWidget->currentIndex().row() - electricCarsList.size(); ++i)
            {
                ++it;
            }
            node = *it;
            RepositoryForUsersBoughtCars<CombustionEngineCar>::tradeInCar(sessionStatus->getLogin(), "combustion", node.id);
            RepositoryForUsers::tradeIn(node.car.getPrice(), sessionStatus->getLogin());
            sessionStatus->setDiscountAmount(node.car.getPrice() * 0.7);
        }
        if (ui->listWidget->currentIndex().row() + 1 > electricCarsList.size() + combustionCarsList.size())
        {
            auto it = hybridCarsList.begin();
            Node<HybridEngineCar> node;
            for (int i = 0; i < ui->listWidget->currentIndex().row() - electricCarsList.size() - combustionCarsList.size(); ++i)
            {
                ++it;
            }
            node = *it;
            RepositoryForUsersBoughtCars<HybridEngineCar>::tradeInCar(sessionStatus->getLogin(), "hybrid", node.id);
            RepositoryForUsers::tradeIn(node.car.getPrice(), sessionStatus->getLogin());
            sessionStatus->setDiscountAmount(node.car.getPrice() * 0.7);
        }
        std::string str = std::to_string(sessionStatus->getDiscountAmount());
        str = str.substr(0, str.find(',') + 4) + " р.";
        ui->tradeInAmountLabel->setText(QString::fromStdString(str));
        ui->listWidget->clear();
        addCarsToList();
    }
}