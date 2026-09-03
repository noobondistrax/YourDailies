#ifndef DASHBOARDSERVICE_H
#define DASHBOARDSERVICE_H

#include "dashboardRepository.h"

class DashboardService
{
public:
    explicit DashboardService(DashboardRepository& dashboardRepository);
    ~DashboardService() = default;

private:
	DashboardRepository& m_dashboardRepository;
};

#endif