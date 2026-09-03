#ifndef DASHBOARDPAGE_H
#define DASHBOARDPAGE_H

#include <QWidget>
#include <QList>
#include "widgetModel.h"
#include "widgetservice.h"
#include "dashboardservice.h"

class QToolButton;

class DashboardPage : public QWidget
{
    Q_OBJECT

public:
    explicit DashboardPage(DashboardService dashboardService, QList<WidgetModel>& widgets, WidgetService& widgetService, bool userIsAdmin, QWidget* parent = nullptr);
    ~DashboardPage();

signals:
    void addWidgetClicked();
	void settingsClicked();
	void adminClicked();
	void logoutClicked();

private:
	DashboardService m_dashboardService;
	bool m_userIsAdmin;
	QWidget* buildSidebar();
	QToolButton* buildSidebarButton(const QString& iconPath, const QString& tooltip);
};

#endif // DASHBOARDPAGE_H
