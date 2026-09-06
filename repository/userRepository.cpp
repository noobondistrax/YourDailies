#include "userRepository.h"


std::optional<UserModel> UserRepository::loadByMail(const QString& mail) {

    const char* sql = "SELECT user_id, username, email, password_hash, role, status, requested_at, confirmed_at "
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
        user.uID        = sqlite3_column_int(stmt, 0);
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

bool UserRepository::uCreate(const UserModel& user, int questionId, const QString& secAnswer) {
    if (sqlite3_exec(m_db.connection(), "BEGIN TRANSACTION;", nullptr, nullptr, nullptr) != SQLITE_OK) {
        qDebug() << "Fehler beim Starten der Transaktion:" << sqlite3_errmsg(m_db.connection());
        return false;
    }

    // 1. setup user
    const char* sql = "INSERT INTO users "
                      "(username, email, password_hash, role, status, requested_at, confirmed_at) "
                      "VALUES (?, ?, ?, ?, ?, ?, ?);";

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
    QByteArray confirmedUtf8 = user.uConfirmed.toUtf8();


    sqlite3_bind_text(stmt, 1, usernameUtf8.constData(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, emailUtf8.constData(),    -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, hashUtf8.constData(),     -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, roleUtf8.constData(),     -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 5, statusUtf8.constData(),   -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 6, requestedUtf8.constData(),-1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 7, confirmedUtf8.constData(),-1, SQLITE_TRANSIENT);

    bool success = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);

    if (!success) {
        qDebug() << "Fehler beim Einfügen des Users:" << sqlite3_errmsg(m_db.connection());
        sqlite3_exec(m_db.connection(), "ROLLBACK;", nullptr, nullptr, nullptr);
        return false;
    }

    // Gerade vergebene user_id merken - wird fuer den zweiten INSERT als Fremdschluessel gebraucht
    const sqlite3_int64 newUserId = sqlite3_last_insert_rowid(m_db.connection());

    // 2. Sicherheitsantwort anlegen
    const char* sqlAnswer = "INSERT INTO user_security_answers "
        "(user_id, question_id, answer_hash) "
        "VALUES (?, ?, ?);";

    sqlite3_stmt* stmtAnswer = nullptr;

    if (sqlite3_prepare_v2(m_db.connection(), sqlAnswer, -1, &stmtAnswer, nullptr) != SQLITE_OK) {
        qDebug() << "Fehler beim Vorbereiten der Answer-Query:" << sqlite3_errmsg(m_db.connection());
        sqlite3_exec(m_db.connection(), "ROLLBACK;", nullptr, nullptr, nullptr);
        return false;
    }

    QByteArray secAnswerUtf8 = secAnswer.toUtf8();

    sqlite3_bind_int64(stmtAnswer, 1, newUserId);
    sqlite3_bind_int(stmtAnswer, 2, questionId);
    sqlite3_bind_text(stmtAnswer, 3, secAnswerUtf8.constData(), -1, SQLITE_TRANSIENT);

    bool answerSuccess = (sqlite3_step(stmtAnswer) == SQLITE_DONE);
    sqlite3_finalize(stmtAnswer);

    if (!answerSuccess) {
        qDebug() << "Fehler beim Einfügen der Sicherheitsantwort:" << sqlite3_errmsg(m_db.connection());
        sqlite3_exec(m_db.connection(), "ROLLBACK;", nullptr, nullptr, nullptr);
        return false;
    }

    if (sqlite3_exec(m_db.connection(), "COMMIT;", nullptr, nullptr, nullptr) != SQLITE_OK) {
        qDebug() << "Fehler beim Commit:" << sqlite3_errmsg(m_db.connection());
        sqlite3_exec(m_db.connection(), "ROLLBACK;", nullptr, nullptr, nullptr);
        return false;
    }

    return true;
}

bool UserRepository::uUpdate(const UserModel& user) {
    const char* sql = "UPDATE users SET "
                    "username = ? , email = ? , password_hash = ? , role = ? , status = ? , requested_at = ? , confirmed_at = ? "
                    "WHERE user_id = ? ;";

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
    QByteArray confirmedUtf8 = user.uConfirmed.toUtf8();

    sqlite3_bind_text(stmt, 1, usernameUtf8.constData(),    -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, emailUtf8.constData(),       -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, hashUtf8.constData(),        -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, roleUtf8.constData(),        -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 5, statusUtf8.constData(),      -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 6, requestedUtf8.constData(),   -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 7, confirmedUtf8.constData(),   -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 8, user.uID);

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

    //QByteArray userIDUtf8   = user.uID.toUtf8();
    sqlite3_bind_int(stmt, 1, user.uID);

    bool success = (sqlite3_step(stmt) == SQLITE_DONE);

    if (!success) {
        qDebug() << "Fehler beim Löschen des Users:" << sqlite3_errmsg(m_db.connection());
    }

    sqlite3_finalize(stmt);
    return success;
}

std::optional<SecAnswer> UserRepository::getSecAnswer(int userId) {
    const char* sql = "SELECT user_id, question_id, answer_hash "
        "FROM user_security_answers WHERE user_id = ? LIMIT 1;";
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(m_db.connection(), sql, -1, &stmt, nullptr) != SQLITE_OK) {
        qDebug() << "Fehler beim Vorbereiten der Query:" << sqlite3_errmsg(m_db.connection());
        return std::nullopt;
    }

    sqlite3_bind_int(stmt, 1, userId);

    std::optional<SecAnswer> result = std::nullopt;

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        SecAnswer answer;
        answer.questionId = sqlite3_column_int(stmt, 1);
		answer.answerHash = QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
        result = answer;
    }

    sqlite3_finalize(stmt);
    return result;
}