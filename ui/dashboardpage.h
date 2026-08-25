#ifndef DASHBOARDPAGE_H
#define DASHBOARDPAGE_H

#include <QWidget>
#include <QList>
#include "widgetModel.h"
#include "widgetservice.h"

class DashboardPage : public QWidget
{
    Q_OBJECT

public:
    explicit DashboardPage(QList<WidgetModel> &widgets, WidgetService& widgetService, QWidget *parent = nullptr);
    ~DashboardPage();

private:
    void buildWidgets();
};

#endif // DASHBOARDPAGE_H
