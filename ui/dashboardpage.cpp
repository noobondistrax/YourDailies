#include "dashboardpage.h"
#include "dashboardcanvas.h"
#include <QScrollArea>
#include <QVBoxLayout>

//explicit DashboardPage(QList<WidgetModel> widgets, WidgetService& widgetService, QWidget *parent = nullptr);
DashboardPage::DashboardPage(QList<WidgetModel> &widgets, WidgetService& widgetService, QWidget* parent)
    : QWidget(parent)
{

	auto* canvas = new DashboardCanvas(widgets, widgetService, this);

    auto *scrollArea = new QScrollArea(this);
    scrollArea->setWidget(canvas);
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(scrollArea);
}

DashboardPage::~DashboardPage() = default;