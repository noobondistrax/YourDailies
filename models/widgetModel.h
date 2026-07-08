#ifndef WIDGETMODEL_H
#define WIDGETMODEL_H

#include <QString>

struct WidgetModel {
    int wID;
    QString wName;
    QString wPluginName;
    QString wDesc;
    bool wIsActive;
    int wPosX;                  // Morgen weitermachen!!!!!
    int wPosY;                  // wie verknüpfen mit user_widgets???????
    int wSizeX;                 // Plus noch die anderen Daten aus user_widgets????
    int wSizeY;
};

#endif // WIDGETMODEL_H