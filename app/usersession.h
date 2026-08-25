#ifndef USERSESSION_H
#define USERSESSION_H

#include <QList>
#include "userModel.h"
#include "widgetModel.h"
#include "dashboardPage.h"

class UserSession
{
public:
    UserSession() = default;
    ~UserSession() = default;

    UserModel& user() { return m_user; }
    const UserModel& user() const { return m_user; }
    void setUser(UserModel user) { m_user = std::move(user); }
    void setDashboard(DashboardPage dashboard) { m_dashboard = std::move(dashboard); }
    void setWidgets(QList<WidgetModel> w) { m_widgets = std::move(w); }
    //void setSettings(Settings s) { m_settings = std::move(s); }

    const UserModel& user() const { return m_user; }
    const DashboardPage& dashboard() const { return m_dashboard; }
    const QList<WidgetModel>& widgets() const { return m_widgets; }
    //const Settings& settings() const { return m_settings; }

private:
    QList<WidgetModel> m_widgets;
    UserModel m_user;
	DashboardPage m_dashboard;
    //UserSettings m_settings;
};

#endif