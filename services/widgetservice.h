#ifndef WIDGETSERVICE_H
#define WIDGETSERVICE_H

#include "widgetRepository.h"

class WidgetService
{
public:
    WidgetService(WidgetRepository& wr);
    ~WidgetService() = default;

private:
    WidgetRepository m_wr;
};

#endif