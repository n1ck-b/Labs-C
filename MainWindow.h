#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"
#include "CatalogWindow.h"
#include "SearchForCarsWindow.h"
#include "LoginWindow.h"
#include <QPropertyAnimation>
#include <QGraphicsDropShadowEffect>

class CatalogWindow;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindowClass* ui;
    CatalogWindow* catalogWindow;
    void setStyles();
private slots:
    void onCatalogPushButtonClicked();
    void onExitPushButtonClicked();
    void onSearchPushButtonClicked();
    void onLoginPushButtonClicked();
    void onSuccessfulAuthenticationOrRegistration();
    void checkForAuthentication();
    void onCalculatorPushButtonClicked();
};
