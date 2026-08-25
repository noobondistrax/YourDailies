#ifndef WIDGETMODEL_H
#define WIDGETMODEL_H

#include <QString>

struct WidgetModel {
    int wUID;
    int wID;
    QString wName;
    QString wPluginName;
    QString wDesc;
    bool wIsActive;
    bool wIsVisible;
    int wPosX;
    int wPosY;
    int wWidth;
    int wHeight;
};

#endif // WIDGETMODEL_H