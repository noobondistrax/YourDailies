#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "designer.h"
#include "userHandling.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setFixedSize(size());

    // Designer::styleAppBackground(ui->stackedWidget);
    // Designer::passwordToggle(ui->login_password);
    // Designer::passwordToggle(ui->register_password);
    // Designer::passwordToggle(ui->register_password_confirm);

    // dataBase.open("yourdailies.db","/database");

    // programmStart(dataBase);

}

MainWindow::~MainWindow()
{
    delete ui;
}
