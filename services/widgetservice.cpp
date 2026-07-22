#include "widgetservice.h"

WidgetService::WidgetService(WidgetRepository& wr)
    : m_wr(wr)
{

}

bool WidgetService::widgetSavePosSize(const QList<WidgetModel> &wm) {
    if (wm.isEmpty() || !m_wr.saveUWidPosSize(wm)) { return false; }
    return true;
}