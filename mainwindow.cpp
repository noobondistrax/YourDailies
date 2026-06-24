#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "designer.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setFixedSize(size());

    Designer::styleAppBackground(ui->stackedWidget);
    Designer::passwordToggle(ui->login_password);

    dataBase.open("test.db","/database");
}

MainWindow::~MainWindow()
{
    delete ui;
}
