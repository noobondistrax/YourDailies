#ifndef DASHBOARDREPOSITORY_H
#define DASHBOARDREPOSITORY_H

#include "database.h"

class DashboardRepository
{
public:
	explicit DashboardRepository(Database& dB);
    ~DashboardRepository() = default;

	bool isAdmin() const { return m_database.adminExists(); }

private:
	Database& m_database;
};

#endif // DASHBOARDREPOSITORY_H
