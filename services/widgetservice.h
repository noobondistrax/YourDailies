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

private:
    WidgetRepository m_wr;
};



#endif