#include "registerpage.h"
#include "ui/ui_registerpage.h"
#include "designer.h"

RegisterPage::RegisterPage(QWidget *parent)
    : QWidget(parent)
    ,ui(new Ui::RegisterPage)
{
    ui->setupUi(this);

    Designer::styleAppBackground(ui->stackedWidget);
    Designer::passwordToggle(ui->register_password);
    Designer::passwordToggle(ui->register_password_confirm);
}

RegisterPage::~RegisterPage()
{
    delete ui;
}

