#ifndef DASHBOARDPAGE_H
#define DASHBOARDPAGE_H

#include <QWidget>

// for testing
#include <QList>
#include "widgetModel.h"


class DashboardPage : public QWidget
{
    Q_OBJECT

public:
    explicit DashboardPage(QWidget *parent = nullptr);
    ~DashboardPage();

private:
    void buildWidgets();
    QList<WidgetModel> m_data;
};

#endif // DASHBOARDPAGE_H
