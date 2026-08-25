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

std::optional<QList<WidgetModel>> WidgetRepository::loadUWidgets(int uID)
{
    const char* sql =
        "SELECT uw.user_widget_id, uw.widget_id, "
        "w.widget_name, w.plugin_name, w.description, w.is_active, "
        "uw.is_visible, uw.pos_x, uw.pos_y, uw.width, uw.height "
        "FROM user_widgets uw "
        "JOIN widgets w ON uw.widget_id = w.widget_id "
        "WHERE uw.user_id = ?;";

    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(m_db.connection(), sql, -1, &stmt, nullptr) != SQLITE_OK) {
        qDebug() << "Fehler beim Vorbereiten der Query:" << sqlite3_errmsg(m_db.connection());
        return std::nullopt;
    }

    sqlite3_bind_int(stmt, 1, uID);

    QList<WidgetModel> widgets;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        WidgetModel widget;

        widget.wUID = sqlite3_column_int(stmt, 0);
        widget.wID = sqlite3_column_int(stmt, 1);
        widget.wName = QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
        widget.wPluginName = QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)));
        widget.wDesc = QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4)));
        widget.wIsActive = sqlite3_column_int(stmt, 5) != 0;
        widget.wIsVisible = sqlite3_column_int(stmt, 6) != 0;
        widget.wPosX = sqlite3_column_int(stmt, 7);
        widget.wPosY = sqlite3_column_int(stmt, 8);
        widget.wWidth = sqlite3_column_int(stmt, 9);
        widget.wHeight = sqlite3_column_int(stmt, 10);

        widgets.append(widget);
    }

    sqlite3_finalize(stmt);
    return widgets;
}