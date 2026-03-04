#include "MainWindow.h"
#include "Functions.h"
#include "LoginWindow.h"
#include "SessionStatus.h"
#include "AccountWindow.h"
#include "CalculatorWindow.h"

extern SessionStatus* sessionStatus;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindowClass)
{
    ui->setupUi(this);
    setStyles();
    connect(ui->catalogPushButton, &QPushButton::clicked, this, &MainWindow::onCatalogPushButtonClicked);
    connect(ui->exitPushButton, &QPushButton::clicked, this, &MainWindow::onExitPushButtonClicked);
    connect(ui->searchPushButton, &QPushButton::clicked, this, &MainWindow::onSearchPushButtonClicked);
    connect(ui->loginPushButton, &QPushButton::clicked, this, &MainWindow::onLoginPushButtonClicked);
    connect(ui->calculatorPushButton, &QPushButton::clicked, this, &MainWindow::onCalculatorPushButtonClicked);
    checkForAuthentication();
}

MainWindow::~MainWindow()
{
    delete catalogWindow;
    delete ui;
}

void MainWindow::onCatalogPushButtonClicked()
{
    close();
    catalogWindow = new CatalogWindow(this);
    catalogWindow->show();
}

void MainWindow::onExitPushButtonClicked()
{
    exit(0);
}
void MainWindow::onSearchPushButtonClicked()
{
    SearchForCarsWindow* window = new SearchForCarsWindow(this);
    window->show();
}
void MainWindow::setStyles()
{
    QGraphicsDropShadowEffect* shadowEffect = new QGraphicsDropShadowEffect;
    shadowEffect->setColor(QColor(215, 67, 57));
    shadowEffect->setXOffset(0);
    shadowEffect->setYOffset(0);
    shadowEffect->setBlurRadius(10);
    ui->loginPushButton->setGraphicsEffect(shadowEffect);
}
void MainWindow::onLoginPushButtonClicked()
{
    if (!sessionStatus->getIsAuthorized())
    {
        LoginWindow* window = new LoginWindow(this);
        window->show();
        connect(window, &LoginWindow::successfulAuthentication, this, &MainWindow::onSuccessfulAuthenticationOrRegistration);
        connect(window, &LoginWindow::successfulRegistration, this, &MainWindow::onSuccessfulAuthenticationOrRegistration);
    }
    else
    {
        AccountWindow* window = new AccountWindow(this);
        window->show();
        connect(window, &AccountWindow::logoutFromAccount, this, &MainWindow::checkForAuthentication);
    }
}
void MainWindow::onSuccessfulAuthenticationOrRegistration()
{
    ui->loginPushButton->setMinimumWidth(120);
    ui->loginPushButton->setText(QString::fromUtf8("Мой аккаунт"));
    ui->loginPushButton->setStyleSheet("QPushButton:hover { background-color: #CC3329; } QPushButton { color: rgb(244, 240, 239); font: 500 10pt 'Montserrat'; border-radius: 12px; border-style: solid; border-width: 0px; background-color: rgb(215, 67, 57); padding: 5px; margin-right: 5px; }");
}
void MainWindow::checkForAuthentication()
{
    if (sessionStatus->getIsAuthorized())
    {
        ui->loginPushButton->setMinimumWidth(120);
        ui->loginPushButton->setText(QString::fromUtf8("Мой аккаунт"));
        ui->loginPushButton->setStyleSheet("QPushButton:hover { background-color: #CC3329; } QPushButton { color: rgb(244, 240, 239); font: 500 10pt 'Montserrat'; border-radius: 12px; border-style: solid; border-width: 0px; background-color: rgb(215, 67, 57); padding: 5px; margin-right: 5px; }");
    }
    else
    {
        ui->loginPushButton->setMinimumWidth(80);
        ui->loginPushButton->setText(QString::fromUtf8("Вход"));
    }
}
void MainWindow::onCalculatorPushButtonClicked()
{
    CalculatorWindow* window = new CalculatorWindow(this);
    window->show();
}