#include "widgetservice.h"

WidgetService::WidgetService(WidgetRepository& wr)
    : m_wr(wr)
{

}

bool WidgetService::widgetSavePosSize(const QList<WidgetModel> &wm) {
    if (wm.isEmpty() || !m_wr.saveUWidPosSize(wm)) { return false; }
    return true;
}

std::optional<QList<WidgetModel>> WidgetService::getWidgets(int userId)
{
    if (userId == 0) {
        return std::nullopt;
    }

    return m_wr.loadUWidgets(userId);
}