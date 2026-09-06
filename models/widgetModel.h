#ifndef WIDGETMODEL_H
#define WIDGETMODEL_H

#include <QString>

struct WidgetModel {
    int wUID = 0;
    int wID = 0;
    QString wName;
    QString wPluginName;
    QString wDesc;
    bool wIsActive = false;
    bool wIsVisible = true;
    int wPosX = 0;
    int wPosY = 0;
    int wWidth = 0;
    int wHeight = 0;
};

#endif // WIDGETMODEL_H