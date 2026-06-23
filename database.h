#ifndef DATABASE_H
#define DATABASE_H

#include "sqlite3.h"
#include "globals.h"

class Database
{
public:
    Database();
    ~Database();

    bool open(const QString& fileName, const QString& wantedPath);


private:
    sqlite3* m_db = nullptr;
    bool createTable();
};

#endif // DATABASE_H