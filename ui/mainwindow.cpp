#include "mainwindow.h"
#include "ui/ui_mainwindow.h"
#include "loginpage.h"
#include "registerpage.h"
#include "dashboardpage.h"
#include "pwReset.h"


MainWindow::MainWindow(AppContext& appContext, QWidget* parent)
    : QMainWindow(parent)
    , m_context(appContext)
    , ui(new Ui::MainWindow)

{
    ui->setupUi(this);
    setFixedSize(size());

    progStart();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::progStart() {
    if(m_context.adminExists()) {
        showLoginPage();

    } else {
        showRegisterPage();
    }
}

void MainWindow::showDashboard()
{
    auto* dashboardpage = new DashboardPage(m_context.dashboardService(), m_context.session().widgets(), m_context.widgetService(), m_context.session().isAdmin(), this);
    ui->stackedWidget->addWidget(dashboardpage);
    ui->stackedWidget->setCurrentWidget(dashboardpage);
}

void MainWindow::showLoginPage() 
{
    auto* loginpage = new LoginPage(m_context);
    ui->stackedWidget->addWidget(loginpage);
    ui->stackedWidget->setCurrentWidget(loginpage);

    connect(loginpage, &LoginPage::loginSucceeded, this, &MainWindow::showDashboard);
    connect(loginpage, &LoginPage::registerClicked, this, &MainWindow::showRegisterPage);
	connect(loginpage, &LoginPage::forgotPasswordRequested, this, &MainWindow::showPWReset);
}

void MainWindow::showRegisterPage() {
	auto* registerpage = new RegisterPage(m_context);
	ui->stackedWidget->addWidget(registerpage);
	ui->stackedWidget->setCurrentWidget(registerpage);
	connect(registerpage, &RegisterPage::RegisterSucceeded, this, &MainWindow::showLoginPage);
}

void MainWindow::showPWReset() {
    auto* pwreset = new pwReset(m_context);
    ui->stackedWidget->addWidget(pwreset);
    ui->stackedWidget->setCurrentWidget(pwreset);
	connect(pwreset, &pwReset::passwordResetted, this, &MainWindow::showLoginPage);
}