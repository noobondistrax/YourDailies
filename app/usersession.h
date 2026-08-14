#ifndef USERSESSION_H
#define USERSESSION_H

#include <QList>
#include "widgetModel.h"

class UserSession
{
public:
    UserSession() = default;
    ~UserSession() = default;

private:
    QList<WidgetModel> UserWidgetList;
};

#endif