#include "mainwindow.h"
#include "ui/ui_mainwindow.h"
#include "loginpage.h"
#include "registerpage.h"
#include "dashboardpage.h"


MainWindow::MainWindow(AppContext& appContext, QWidget* parent)
    : QMainWindow(parent)
    , m_context(appContext)
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
        auto *loginpage = new LoginPage(context);
        ui->stackedWidget->addWidget(loginpage);
        ui->stackedWidget->setCurrentWidget(loginpage);

		connect(loginpage, &LoginPage::loginSucceeded, this, &MainWindow::showDashboard);
    } else {
        auto *registerpage = new RegisterPage(context);
        ui->stackedWidget->addWidget(registerpage);
        ui->stackedWidget->setCurrentWidget(registerpage);
    }
}

void MainWindow::showDashboard()
{
    auto* dashboardpage = new DashboardPage(m_context.session().widgets(), m_context.widgetService(), this);
    ui->stackedWidget->addWidget(dashboardpage);
    ui->stackedWidget->setCurrentWidget(dashboardpage);
}