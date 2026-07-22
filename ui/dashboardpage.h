#ifndef DASHBOARDPAGE_H
#define DASHBOARDPAGE_H

#include <QWidget>
#include "appcontext.h"

// for testing
#include <QList>
#include "widgetModel.h"


class DashboardPage : public QWidget
{
    Q_OBJECT

public:
    explicit DashboardPage(AppContext& context, QWidget *parent = nullptr);
    ~DashboardPage();

private:
    void buildWidgets();
    QList<WidgetModel> m_data;
};

#endif // DASHBOARDPAGE_H
