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
	bool isAdmin() const { return m_user.uRole == "admin"; }  // check if user is admin

    //void setDashboard(DashboardPage dashboard) { m_dashboard = std::move(dashboard); }
    void setWidgets(QList<WidgetModel> w) { m_widgets = std::move(w); }
    //void setSettings(Settings s) { m_settings = std::move(s); }

    //const DashboardPage& dashboard() const { return m_dashboard; }
    QList<WidgetModel>& widgets() { return m_widgets; }
    //const Settings& settings() const { return m_settings; }
    
    bool isActive() const { return m_active; }  // check if UserSession is or is not active
	void changeActiveState(bool status) { /* true for set active; false for set inactive*/ m_active = status; } // set UserSession active or inactive

private:
    QList<WidgetModel> m_widgets;
    UserModel m_user;
	bool m_active = false;
	
    //UserSettings m_settings;
};

#endif