#include "mainwindow.h"
#include "ui/ui_mainwindow.h"


MainWindow::MainWindow(AppContext& appContext, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setFixedSize(size());
}

MainWindow::~MainWindow()
{
    delete ui;
}
