#ifndef WIDGETREPOSITORY_H
#define WIDGETREPOSITORY_H

#include "widgetModel.h"
#include "database.h"

class WidgetRepository
{
public:
    explicit WidgetRepository(Database& db) : m_db(db) {}

    bool saveUWidPosSize(const QList<WidgetModel> &widgets);

private:
    Database& m_db;
};

#endif // WIDGETREPOSITORY_H
