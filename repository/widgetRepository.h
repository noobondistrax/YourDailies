#ifndef WIDGETREPOSITORY_H
#define WIDGETREPOSITORY_H

#include "widgetModel.h"
#include "database.H"

class WidgetRepository
{
public:
    explicit WidgetRepository(Database& db) : m_db(db) {}

    // Weitere Methoden (loadWidgets(), saveWidget(), deleteWidget() ...) kommen
    // in Phase 3 dazu - hier erstmal nur, was Schritt 5 braucht.

    // Schreibt alle übergebenen Widgets in EINER Transaktion. Wird vom
    // WidgetService aufgerufen, nachdem ein Drag oder Resize im
    // DashboardCanvas abgeschlossen ist (gezogenes Widget + alle Blocker,
    // die durch Ausweichen ihre Position/Größe geändert haben).
    // Rückgabewert: true, wenn die Transaktion committet wurde.
    bool savePositions(const QList<WidgetModel> &widgets);
private:
    Database& m_db;
};

#endif // WIDGETREPOSITORY_H
