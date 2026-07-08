#include "appcontext.h"

AppContext::AppContext()
{
    Logger::instance().log(LogLevel::Info, "Programm gestartet");

    if (!m_database.open("yourdailies.db","/database")) {
        Logger::instance().log(LogLevel::Error, "Datenbank konnte nicht geoeffnet werden");
    }

    // Einzige Stelle im Programm, an der Dienste erzeugt werden (R-08)
    m_userService      = std::make_unique<UserService>(m_database);
    m_dashboardService = std::make_unique<DashboardService>(m_database);
    m_widgetService    = std::make_unique<WidgetService>(m_database);

    Logger::instance().log(LogLevel::Info, "Dienste erfolgreich erzeugt");

}

AppContext::~AppContext()
{
    Logger::instance().log(LogLevel::Info, "Anwendung wird beendet");
}


UserSession& AppContext::session()
{
    Q_ASSERT(m_session.has_value() && "Kein aktiver User - vorher hasActiveSession() pruefen");
    return *m_session;
}

void AppContext::startSession(UserSession session)
{
    m_session = std::move(session);
    Logger::instance().log(LogLevel::Info,"UserSession gestartet fuer User: " + m_session->user().username());
}

void AppContext::endSession()
{
    if (m_session) {
        Logger::instance().log(LogLevel::Info,"UserSession beendet fuer User: " + m_session->user().username());
    }
    m_session.reset();
}
