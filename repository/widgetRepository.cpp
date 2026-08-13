#include "widgetRepository.h"
#include "sqlite3.h"


bool WidgetRepository::saveUWidPosSize(const QList<WidgetModel> &widgets)
{
    sqlite3 *db = m_db.connection();
    if (!db) return false;

    if (sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr) != SQLITE_OK) {
        return false;
    }

    sqlite3_stmt *stmt = nullptr;
    const char *sql =
        "UPDATE user_widgets SET pos_x = ?, pos_y = ?, width = ?, height = ? WHERE user_widget_id = ?;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        return false;
    }

    for (const WidgetModel &widget : widgets) {
        sqlite3_bind_int(stmt, 1, widget.wPosX);
        sqlite3_bind_int(stmt, 2, widget.wPosY);
        sqlite3_bind_int(stmt, 3, widget.wWidth);
        sqlite3_bind_int(stmt, 4, widget.wHeight);
        sqlite3_bind_int(stmt, 5, widget.wUID);

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            // Ein einzelner Fehler verwirft die GESAMTE Transaktion - entweder
            // landen alle Positionen/Größen aus diesem Reflow in der DB, oder keine.
            sqlite3_finalize(stmt);
            sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
            return false;
        }

        // Statement für die nächste Zeile wiederverwenden statt neu zu prepare()n
        sqlite3_reset(stmt);
        sqlite3_clear_bindings(stmt);
    }

    sqlite3_finalize(stmt);

    if (sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr) != SQLITE_OK) {
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        return false;
    }
    return true;
}

QList<WidgetModel> WidgetRepository::loadUWidgets()
{
    /*
    sqlite3* db = m_db.connection();
    if (!db) return false;

    if (sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr) != SQLITE_OK) {
        return false;
    }

    sqlite3_stmt* stmt = nullptr;
    const char* sql =
        "UPDATE user_widgets SET pos_x = ?, pos_y = ?, width = ?, height = ? WHERE user_widget_id = ?;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        return false;
    }

    for (const WidgetModel& widget : widgets) {
        sqlite3_bind_int(stmt, 1, widget.wPosX);
        sqlite3_bind_int(stmt, 2, widget.wPosY);
        sqlite3_bind_int(stmt, 3, widget.wWidth);
        sqlite3_bind_int(stmt, 4, widget.wHeight);
        sqlite3_bind_int(stmt, 5, widget.wUID);

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            // Ein einzelner Fehler verwirft die GESAMTE Transaktion - entweder
            // landen alle Positionen/Größen aus diesem Reflow in der DB, oder keine.
            sqlite3_finalize(stmt);
            sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
            return false;
        }

        // Statement für die nächste Zeile wiederverwenden statt neu zu prepare()n
        sqlite3_reset(stmt);
        sqlite3_clear_bindings(stmt);
    }

    sqlite3_finalize(stmt);

    if (sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr) != SQLITE_OK) {
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        return false;
    }
    return blabla;
    */
}