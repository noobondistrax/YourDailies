#ifndef USERREPOSITORY_H
#define USERREPOSITORY_H

#include <optional>
#include "userModel.h"
#include "database.h"

class UserRepository {
public:
    explicit UserRepository(Database& db) : m_db(db) {}

    UserModel loadByMail(const QString& mail);
    bool uCreate(const UserModel& userModel);
    bool uUpdate(const UserModel& userModel);
    bool uDelete(const UserModel& userModel);

private:
    Database& m_db;
};

#endif // USERREPOSITORY_H
