#include "loginpage.h"
#include "ui/ui_loginpage.h"
#include "designer.h"

LoginPage::LoginPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LoginPage)
{
    ui->setupUi(this);

    Designer::passwordToggle(ui->login_password);
}

LoginPage::~LoginPage()
{
    delete ui;
}

