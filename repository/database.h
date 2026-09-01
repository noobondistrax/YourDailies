#ifndef DATABASE_H
#define DATABASE_H

#include <QString>
#include <QCoreApplication>
#include <QDir>
#include <QMap>
#include "sqlite3.h"

class Database
{
public:
    enum class tableNames {
        users,
        user_security_answers,
        user_settings,
        widgets,
        user_widgets,
        reminders,
        appointments,
        appointment_participants
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
    bool adminExists();
    sqlite3* connection() const { return m_db; }


private:
    QMap<tableNames, tableStatus> m_tableStatus;

    sqlite3* m_db = nullptr;
    void createUsersTable();
	void createUserSecAnswersTable();
    void createUserSettingsTable();
    void createWidgetsTable();
    void createUserWidgetsTabele();
    void createReminderTable();
    void createAppointmentsTable();
    void createAppointmentParticipantsTable();
    void createAuditLogsTable();
};

#endif // DATABASE_H