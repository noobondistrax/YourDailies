#include "database.h"
#include <QFile>

Database::Database()
{
}

Database::~Database()
{
    if (m_db) {
        sqlite3_close(m_db);
    }
}


/*
 * Aufruf:
 * dataBase.open("yourdailies.db","/database");
 *
 * Params:
 * 1. DB Dateiname
 * 2. Speicherpfad von "/" ausgehend
 *
 */

bool Database::open(const QString& fileName, const QString& wantedPath) {

    QString path = QCoreApplication::applicationDirPath() + wantedPath;     // QString Objekt anlegen - QString für Ordnerpfad
    QString dbPath = path + "/" + fileName;                                 // QString Objekt anlegen - Pfad für Datei

    QDir().mkpath(path);                                                    // legt den Ordner an, falls er nicht existiert

    int rc = sqlite3_open(dbPath.toUtf8().constData(), &m_db);

    if (rc != SQLITE_OK) {
        qDebug() << "Fehler beim Öffnen:" << sqlite3_errmsg(m_db);
        return false;
    }
    sqlite3_exec(m_db, "PRAGMA foreign_keys = ON;", nullptr, nullptr, nullptr);

    qDebug() << "Datenbank erfolgreich geöffnet!";

    createUsersTable();
    createUsersTable();
    createUserSettingsTable();
    createWidgetsTable();
    createUserWidgetsTabele();
    createReminderTable();
    createAppointmentsTable();
    createAppointmentParticipantsTable();
    createAuditLogsTable();

    return true;
}

void Database::createUsersTable() {
    const char* sql = "CREATE TABLE IF NOT EXISTS users ("
                      "user_id INTEGER PRIMARY KEY, "       // in SQL: 'VARCHAR(255)', but SQLite ignores the length "(255)" and convert 'VARCHAR' to 'TEXT'
                      "username TEXT UNIQUE, "
                      "email TEXT UNIQUE, "
                      "password_hash TEXT, "
                      "role TEXT, "
                      "status TEXT, "
                      "requested_at TEXT, "                 // in SQL "DATE", "DATETIME" or "TIMESTAMP" can be used - but SQLite convert it to TEXT
                      "confirmed_at TEXT"
                      ");";

    char* errMsg = nullptr;
    int rc = sqlite3_exec(m_db, sql, nullptr, nullptr, &errMsg);

    if (rc != SQLITE_OK) {
        qDebug() << "Fehler beim Erstellen der 'users' Tabelle:" << errMsg;
        m_tableStatus[tableNames::users] = tableStatus::failed;
        sqlite3_free(errMsg);
        return;
    }
    qDebug() << "'users' Tabelle erfolgreich erstellt (oder existierte schon)!";
    m_tableStatus[tableNames::users] = tableStatus::created;
}

void Database::createUserSettingsTable() {
    const char* sql = "CREATE TABLE IF NOT EXISTS user_settings ("
                      "user_setting_id INTEGER PRIMARY KEY, "
                      "user_id INTEGER, "
                      "theme TEXT, "
                      "language TEXT, "
                      "timezone TEXT, "
                      "FOREIGN KEY (user_id) REFERENCES users(user_id)"
                      ");";

    char* errMsg = nullptr;
    int rc = sqlite3_exec(m_db, sql, nullptr, nullptr, &errMsg);

    if (rc != SQLITE_OK) {
        qDebug() << "Fehler beim Erstellen der 'user_settings' Tabelle:" << errMsg;
        m_tableStatus[tableNames::user_settings] = tableStatus::failed;
        sqlite3_free(errMsg);
        return;
    }

    qDebug() << "'user_settings' Tabelle erfolgreich erstellt (oder existierte schon)!";
    m_tableStatus[tableNames::user_settings] = tableStatus::created;
}

void Database::createWidgetsTable() {
    const char* sql = "CREATE TABLE IF NOT EXISTS widgets ("
                      "widget_id INTEGER PRIMARY KEY, "
                      "widget_name TEXT, "
                      "plugin_name TEXT, "
                      "description TEXT, "
                      "is_active INTEGER "                              // in SQL 'BOOL' or 'BOOLEAN' - SQL converts into 'TINYINT' only Postgres works with bool
                      ");";                                             // SQLite convert bools into 'INTEGER'

    char* errMsg = nullptr;
    int rc = sqlite3_exec(m_db, sql, nullptr, nullptr, &errMsg);

    if (rc != SQLITE_OK) {
        qDebug() << "Fehler beim Erstellen der 'widgets' Tabelle:" << errMsg;
        m_tableStatus[tableNames::widgets] = tableStatus::failed;
        sqlite3_free(errMsg);
        return;
    }

    qDebug() << "'widgets' Tabelle erfolgreich erstellt (oder existierte schon)!";
    m_tableStatus[tableNames::widgets] = tableStatus::created;
}

void Database::createUserWidgetsTabele() {
    const char* sql = "CREATE TABLE IF NOT EXISTS user_widgets ("
                      "user_widget_id INTEGER PRIMARY KEY, "
                      "user_id INTEGER, "
                      "widget_id INTEGER, "
                      "is_visible INTEGER, "
                      "config_json TEXT, "
                      "position_x INTEGER, "
                      "position_y INTEGER, "
                      "width INTEGER, "
                      "height INTEGER, "
                      "sort_order TEXT, "
                      "FOREIGN KEY (user_id) REFERENCES users(user_id), "
                      "FOREIGN KEY (widget_id) REFERENCES widgets(widget_id)"
                      ");";

    char* errMsg = nullptr;
    int rc = sqlite3_exec(m_db, sql, nullptr, nullptr, &errMsg);

    if (rc != SQLITE_OK) {
        qDebug() << "Fehler beim Erstellen der 'user_widgets' Tabelle:" << errMsg;
        m_tableStatus[tableNames::user_widgets] = tableStatus::failed;
        sqlite3_free(errMsg);
        return;
    }

    qDebug() << "'user_widgets' Tabelle erfolgreich erstellt (oder existierte schon)!";
    m_tableStatus[tableNames::user_widgets] = tableStatus::created;
}

void Database::createReminderTable() {
    const char* sql = "CREATE TABLE IF NOT EXISTS reminders ("
                      "reminder_id INTEGER PRIMARY KEY, "
                      "user_id INTEGER, "
                      "appointment_id INTEGER, "
                      "titel TEXT, "
                      "description TEXT, "
                      "remind_at TEXT, "
                      "status TEXT, "
                      "FOREIGN KEY (user_id) REFERENCES users(user_id), "
                      "FOREIGN KEY (appointment_id) REFERENCES appointments(appointment_id)"
                      ");";

    char* errMsg = nullptr;
    int rc = sqlite3_exec(m_db, sql, nullptr, nullptr, &errMsg);

    if (rc != SQLITE_OK) {
        qDebug() << "Fehler beim Erstellen der 'reminders' Tabelle:" << errMsg;
        m_tableStatus[tableNames::reminders] = tableStatus::failed;
        sqlite3_free(errMsg);
        return;
    }

    qDebug() << "'reminders' Tabelle erfolgreich erstellt (oder existierte schon)!";
    m_tableStatus[tableNames::reminders] = tableStatus::created;
}

void Database::createAppointmentsTable() {
    const char* sql = "CREATE TABLE IF NOT EXISTS appointments ("
                      "appointment_id INTEGER PRIMARY KEY, "
                      "user_id INTEGER, "
                      "titel TEXT, "
                      "description TEXT, "
                      "start_at TEXT, "
                      "end_at TEXT, "
                      "location TEXT, "
                      "FOREIGN KEY (user_id) REFERENCES users(user_id)"
                      ");";

    char* errMsg = nullptr;
    int rc = sqlite3_exec(m_db, sql, nullptr, nullptr, &errMsg);

    if (rc != SQLITE_OK) {
        qDebug() << "Fehler beim Erstellen der 'appointments' Tabelle:" << errMsg;
        m_tableStatus[tableNames::appointments] = tableStatus::failed;
        sqlite3_free(errMsg);
        return;
    }

    qDebug() << "'appointments' Tabelle erfolgreich erstellt (oder existierte schon)!";
    m_tableStatus[tableNames::appointments] = tableStatus::created;
}

void Database::createAppointmentParticipantsTable() {
    const char* sql = "CREATE TABLE IF NOT EXISTS appointment_participants ("
                      "appointment_id INTEGER, "
                      "user_id INTEGER, "
                      "FOREIGN KEY (appointment_id) REFERENCES appointments(appointment_id), "
                      "FOREIGN KEY (user_id) REFERENCES users(user_id), "
                      "PRIMARY KEY (appointment_id, user_id)"
                      ");";

    char* errMsg = nullptr;
    int rc = sqlite3_exec(m_db, sql, nullptr, nullptr, &errMsg);

    if (rc != SQLITE_OK) {
        qDebug() << "Fehler beim Erstellen der 'appointments' Tabelle:" << errMsg;
        m_tableStatus[tableNames::appointment_participants] = tableStatus::failed;
        sqlite3_free(errMsg);
        return;
    }

    qDebug() << "'appointments' Tabelle erfolgreich erstellt (oder existierte schon)!";
    m_tableStatus[tableNames::appointment_participants] = tableStatus::created;
}

void Database::createAuditLogsTable() {
    const char* sql = "CREATE TABLE IF NOT EXISTS audit_logs ("
                      "log_id INTEGER PRIMARY KEY, "
                      "user_id INTEGER, "
                      "action_type TEXT, "
                      "target_type TEXT, "
                      "target_id TEXT, "
                      "created_at TEXT, "
                      "details TEXT, "
                      "FOREIGN KEY (user_id) REFERENCES users(user_id)"
                      ");";

    char* errMsg = nullptr;
    int rc = sqlite3_exec(m_db, sql, nullptr, nullptr, &errMsg);

    if (rc != SQLITE_OK) {
        qDebug() << "Fehler beim Erstellen der 'audit_logs' Tabelle:" << errMsg;
        m_tableStatus[tableNames::audit_logs] = tableStatus::failed;
        sqlite3_free(errMsg);
        return;
    }

    qDebug() << "'audit_logs' Tabelle erfolgreich erstellt (oder existierte schon)!";
    m_tableStatus[tableNames::audit_logs] = tableStatus::created;
}

// use for example: if (db.isTableRunning(Database::tableNames::appointments, Database::tableStatus::created)) {}
bool Database::dbTableStatus(tableNames name,tableStatus status) {
    return m_tableStatus.value(name, tableStatus::unknown) == status;
}

bool Database::adminExists() {
    const char* sql = "SELECT 1 FROM users WHERE role = ? LIMIT 1;";
    sqlite3_stmt* stmt;

    sqlite3_prepare_v2(m_db, sql, -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, "admin", -1, SQLITE_STATIC);

    bool exists = (sqlite3_step(stmt) == SQLITE_ROW);

    sqlite3_finalize(stmt);
    return exists;
}

