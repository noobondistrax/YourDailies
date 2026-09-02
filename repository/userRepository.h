#ifndef USERREPOSITORY_H
#define USERREPOSITORY_H

#include <optional>
#include "userModel.h"
#include "database.h"

class UserRepository {
public:
    explicit UserRepository(Database& db) : m_db(db) {}

    std::optional<UserModel> loadByMail(const QString& mail);
    bool uCreate(const UserModel& user, int questionId, const QString& secAnswer);
    bool uUpdate(const UserModel& user);
    bool uDelete(const UserModel& user);
    std::optional<SecAnswer> getSecAnswer(int userId);
	bool adminExists() const { return m_db.adminExists(); }

private:
    Database& m_db;
};

#endif // USERREPOSITORY_H
