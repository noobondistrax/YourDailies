#ifndef APPCONTEXT_H
#define APPCONTEXT_H

#pragma once

#include <memory>
#include <optional>

#include "database.h"
#include "logger.h"
#include "usersession.h"
#include "userservice.h"
#include "dashboardservice.h"
#include "widgetservice.h"

class AppContext
{
public:
    AppContext();
    ~AppContext();

    // -- Dienste: AppContext besitzt sie, gibt aber nur Referenzen heraus --
    Database&         database()         { return m_database; }
    // UserService&      userService()      { return *m_userService; }
    // DashboardService& dashboardService() { return *m_dashboardService; }
    // WidgetService&    widgetService()    { return *m_widgetService; }

    // -- UserSession: existiert erst NACH erfolgreichem Login --
    // bool hasActiveSession() const { return m_session.has_value(); }
    // UserSession& session();              // Zugriff nur wenn eingeloggt
    // void startSession(UserSession session);
    void endSession();

private:
    Database m_database;

    // std::unique_ptr<UserService>      m_userService;
    // std::unique_ptr<DashboardService> m_dashboardService;
    // std::unique_ptr<WidgetService>    m_widgetService;

    // std::optional<UserSession> m_session;
};

#endif // APPCONTEXT_H
