#include "dashboardpage.h"
#include "ui/ui_dashboardpage.h"
#include <QScrollArea>

DashboardPage::DashboardPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DashboardPage)
{
    ui->setupUi(this);

    auto *canvas = new DashboardCanvas(this);

    auto *scrollArea = new QScrollArea(this);
    scrollArea->setWidget(canvas);
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

}

DashboardPage::~DashboardPage()
{
    delete ui;
}