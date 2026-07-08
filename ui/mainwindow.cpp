#include "mainwindow.h"
#include "ui/ui_mainwindow.h"
#include "loginpage.h"
#include "registerpage.h"
#include "globals.h"


MainWindow::MainWindow(AppContext& appContext, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)

{
    ui->setupUi(this);

    setFixedSize(size());

    progStart(appContext);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::progStart(AppContext& context) {
    if(context.adminExists()) {
        auto *loginpage = new LoginPage();
        ui->stackedWidget->addWidget(loginpage);
        ui->stackedWidget->setCurrentWidget(loginpage);
    } else {
        auto *registerpage = new RegisterPage();
        ui->stackedWidget->addWidget(registerpage);
        ui->stackedWidget->setCurrentWidget(registerpage);
    }
}