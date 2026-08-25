#ifndef WIDGETSERVICE_H
#define WIDGETSERVICE_H

#include "widgetRepository.h"
#include "widgetModel.h"

class WidgetService
{
public:
    WidgetService(WidgetRepository& wr);
    ~WidgetService() = default;

    bool widgetSavePosSize(const QList<WidgetModel> &wm);
    std::optional<QList<WidgetModel>> getWidgets(int userId);


private:
    WidgetRepository m_wr;
    
};



#endif