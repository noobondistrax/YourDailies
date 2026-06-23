#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "database.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Database db;
    //db.open(dbPfad);
    db.open("test123.db", "/sqlite");
    db.open("test345.db", "/users");
}

MainWindow::~MainWindow()
{
    delete ui;
}
