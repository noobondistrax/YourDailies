#ifndef DATABASE_H
#define DATABASE_H

#include "sqlite3.h"
#include <QMap>
#include "globals.h"

class Database
{
public:
    enum class tableNames {
        users,
        user_settings,
        widgets,
        user_widgets,
        reminders,
        appointments,
        appointment_participants,
        audit_logs
    };

    enum class tableStatus {
        unknown,
        created,
        failed
    };

    Database();
    ~Database();

    bool open(const QString& fileName, const QString& wantedPath);
    bool dbTableStatus(tableNames,tableStatus);


private:
    QMap<tableNames, tableStatus> m_tableStatus;

    sqlite3* m_db = nullptr;
    void createUsersTable();
    void createUserSettingsTable();
    void createWidgetsTable();
    void createUserWidgetsTabele();
    void createReminderTable();
    void createAppointmentsTable();
    void createAppointmentParticipantsTable();
    void createAuditLogsTable();
};

#endif // DATABASE_H