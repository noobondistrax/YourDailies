#ifndef WIDGETREPOSITORY_H
#define WIDGETREPOSITORY_H

#include "widgetModel.h"
#include "database.h"
#include <QList>

class WidgetRepository
{
public:
    explicit WidgetRepository(Database& db) : m_db(db) {}

    bool saveUWidPosSize(const QList<WidgetModel> &widgets);
    QList<WidgetModel> loadUWidgets();

private:
    Database& m_db;
};

#endif // WIDGETREPOSITORY_H
