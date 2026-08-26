#ifndef APPCONTEXT_H
#define APPCONTEXT_H

#include <memory>
#include <optional>

#include "database.h"
#include "usersession.h"
#include "userRepository.h"
#include "userservice.h"
#include "dashboardRepository.h"
#include "dashboardservice.h"
#include "widgetRepository.h"
#include "widgetservice.h"

class AppContext
{
public:
    AppContext();
    ~AppContext();

    // -- Dienste: AppContext besitzt sie, gibt aber nur Referenzen heraus --
    Database&               database()              { return m_database; }
    UserRepository&         userRepository()        { return *m_userRepository; }
    UserService&            userService()           { return *m_userService; }
    DashboardRepository&    dashboardRepository()   { return *m_dashboardRepository; }
    DashboardService&       dashboardService()      { return *m_dashboardService; }
    WidgetRepository&       widgetRepository()      { return *m_widgetRepository; }
    WidgetService&          widgetService()         { return *m_widgetService; }

    // -- UserSession: existiert erst NACH erfolgreichem Login --
    bool hasActiveSession() const { return m_session.isActive(); }
    UserSession& session();              // Zugriff nur wenn eingeloggt
    void startSession(UserSession session);
    void endSession();
    bool adminExists();

private:
    Database m_database;

    std::unique_ptr<UserRepository> m_userRepository;
    std::unique_ptr<UserService> m_userService;
    std::unique_ptr<DashboardRepository> m_dashboardRepository;
    std::unique_ptr<DashboardService> m_dashboardService;
    std::unique_ptr<WidgetRepository> m_widgetRepository;
    std::unique_ptr<WidgetService> m_widgetService;

    UserSession m_session;
};

#endif // APPCONTEXT_H
