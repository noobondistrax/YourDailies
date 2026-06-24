#include "mainwindow.h"
#include "./ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    dataBase.open("test.db","/database");
}

MainWindow::~MainWindow()
{
    delete ui;
}
