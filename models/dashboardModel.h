#ifndef DASHBOARDMODEL_H
#define DASHBOARDMODEL_H

#include <vector>

struct DashboardModel {
    int dashID;
    int uID;
    std::vector<int> dashWidgetList;
};

#endif // DASHBOARDMODEL_H
