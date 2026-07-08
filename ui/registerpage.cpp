#include <QPalette>
#include <QList>
#include "registerpage.h"
#include "ui/ui_registerpage.h"
#include "designer.h"


RegisterPage::RegisterPage(QWidget *parent)
    : QWidget(parent)
    ,ui(new Ui::RegisterPage)
{
    ui->setupUi(this);

    const QList<QLineEdit*> edits = this->findChildren<QLineEdit*>();

    for (QLineEdit* edit : edits)
    {
        QPalette palette = edit->palette();
        palette.setColor(QPalette::PlaceholderText, QColor(148, 148, 148, 120));
        edit->setPalette(palette);
    }

    Designer::passwordToggle(ui->register_password);
    Designer::passwordToggle(ui->register_password_confirm);
}

RegisterPage::~RegisterPage()
{
    delete ui;
}

