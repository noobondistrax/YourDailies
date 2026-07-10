#include "userRepository.h"

std::optional<UserModel> UserRepository::loadByMail(const QString& mail) {
    const char* sql = "SELECT user_id, username, email, password_hash, role, status, requested_at, confirmed_at"
                      "FROM users WHERE email = ? LIMIT 1;";
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(m_db.connection(), sql, -1, &stmt, nullptr) != SQLITE_OK) {
        qDebug() << "Fehler beim Vorbereiten der Query:" << sqlite3_errmsg(m_db.connection());
        return std::nullopt;
    }

    QByteArray mailUtf8 = mail.toUtf8();
    sqlite3_bind_text(stmt, 1, mailUtf8.constData(), -1, SQLITE_TRANSIENT);

    std::optional<UserModel> result = std::nullopt;

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        UserModel user;
        user.uID        = QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
        user.uName      = QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
        user.uMail      = QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
        user.uPwHash    = QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)));
        user.uRole      = QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4)));
        user.uStatus    = QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5)));
        user.uRequested = QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6)));
        user.uConfirmed = QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 7)));
        result = user;
    }

    sqlite3_finalize(stmt);
    return result;
}

bool UserRepository::uCreate(const UserModel& user) {
    const char* sql = "INSERT INTO users "
                      "(username, email, password_hash, role, status, requested_at) "
                      "VALUES (?, ?, ?, ?, ?, ?);";

    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(m_db.connection(), sql, -1, &stmt, nullptr) != SQLITE_OK) {
        qDebug() << "Fehler beim Vorbereiten der Query:" << sqlite3_errmsg(m_db.connection());
        return false;
    }

    QByteArray usernameUtf8 = user.uName.toUtf8();
    QByteArray emailUtf8    = user.uMail.toUtf8();
    QByteArray hashUtf8     = user.uPwHash.toUtf8();
    QByteArray roleUtf8     = user.uRole.toUtf8();
    QByteArray statusUtf8   = user.uStatus.toUtf8();
    QByteArray requestedUtf8 = user.uRequested.toUtf8();

    sqlite3_bind_text(stmt, 1, usernameUtf8.constData(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, emailUtf8.constData(),    -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, hashUtf8.constData(),     -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, roleUtf8.constData(),     -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 5, statusUtf8.constData(),   -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 6, requestedUtf8.constData(),-1, SQLITE_TRANSIENT);

    bool success = (sqlite3_step(stmt) == SQLITE_DONE);

    if (!success) {
        qDebug() << "Fehler beim Einfügen des Users:" << sqlite3_errmsg(m_db.connection());
    }

    sqlite3_finalize(stmt);
    return success;
}

bool UserRepository::uUpdate(const UserModel& user) {
    const char* sql = "UPDATE users SET "
                      "username = ?, email = ?, password_hash = ?, role = ?, status = ?, requested_at = ?, confirmed_at = ?"
                      "WHERE user_id = ?;";

    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(m_db.connection(), sql, -1, &stmt, nullptr) != SQLITE_OK) {
        qDebug() << "Fehler beim Vorbereiten der Query:" << sqlite3_errmsg(m_db.connection());
        return false;
    }

    QByteArray userIDUtf8   = user.uID.toUtf8();
    QByteArray usernameUtf8 = user.uName.toUtf8();
    QByteArray emailUtf8    = user.uMail.toUtf8();
    QByteArray hashUtf8     = user.uPwHash.toUtf8();
    QByteArray roleUtf8     = user.uRole.toUtf8();
    QByteArray statusUtf8   = user.uStatus.toUtf8();
    QByteArray requestedUtf8 = user.uRequested.toUtf8();
    QByteArray confirmedUtf8 = user.uConfirmed.toUtf8();

    sqlite3_bind_text(stmt, 1, usernameUtf8.constData(),    -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, emailUtf8.constData(),       -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, hashUtf8.constData(),        -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, roleUtf8.constData(),        -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 5, statusUtf8.constData(),      -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 6, requestedUtf8.constData(),   -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 7, confirmedUtf8.constData(),   -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 8, userIDUtf8.constData(),      -1, SQLITE_TRANSIENT);

    bool success = (sqlite3_step(stmt) == SQLITE_DONE);

    if (!success) {
        qDebug() << "Fehler beim Updaten des Users:" << sqlite3_errmsg(m_db.connection());
    }

    sqlite3_finalize(stmt);
    return success;
}

bool UserRepository::uDelete(const UserModel& user) {
    // DELETE FROM <Tabelle> WHERE <Bedingung>
    const char* sql = "DELETE FROM users WHERE user_id = ?;";

    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(m_db.connection(), sql, -1, &stmt, nullptr) != SQLITE_OK) {
        qDebug() << "Fehler beim Vorbereiten der Query:" << sqlite3_errmsg(m_db.connection());
        return false;
    }

    QByteArray userIDUtf8   = user.uID.toUtf8();

    bool success = (sqlite3_step(stmt) == SQLITE_DONE);

    if (!success) {
        qDebug() << "Fehler beim Löschen des Users:" << sqlite3_errmsg(m_db.connection());
    }

    sqlite3_finalize(stmt);
    return success;
}

