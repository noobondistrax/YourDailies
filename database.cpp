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

bool Database::open(const QString& fileName, const QString& wantedPath)
{

    QString path = QCoreApplication::applicationDirPath() + wantedPath;     // QString Objekt anlegen - QString für Ordnerpfad
    QString dbPath = path + "/" + fileName;                                 // QString Objekt anlegen - Pfad für Datei

    if (!QFile::exists(dbPath)) {
        QDir().mkpath(path);                                                // legt den Ordner an, falls er nicht existiert

        int rc = sqlite3_open(dbPath.toUtf8().constData(), &m_db);

        if (rc != SQLITE_OK) {
            qDebug() << "Fehler beim Öffnen:" << sqlite3_errmsg(m_db);
            return false;
        }

        createTable();
    }

    qDebug() << "Datenbank erfolgreich geöffnet!";
    return true;
}

bool Database::createTable()
{
    const char* sql = "CREATE TABLE IF NOT EXISTS aufgaben ("
                      "id INTEGER PRIMARY KEY, "
                      "titel TEXT);";

    char* errMsg = nullptr;
    int rc = sqlite3_exec(m_db, sql, nullptr, nullptr, &errMsg);

    if (rc != SQLITE_OK) {
        qDebug() << "Fehler beim Erstellen der Tabelle:" << errMsg;
        sqlite3_free(errMsg);
        return false;
    }

    qDebug() << "Tabelle erfolgreich erstellt (oder existierte schon)!";
    return true;
}