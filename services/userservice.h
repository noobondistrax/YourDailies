#ifndef USERSERVICE_H
#define USERSERVICE_H

#include <QString>
#include "appcontext.h"
#include "userRepository.h"

class UserService
{
public:
    explicit UserService(UserRepository& m_userRepository);
    ~UserService() = default;

	bool login(const QString& email, const QString& password, UserSession& outSession);

private:
	UserRepository& m_userRepository;

    bool verifyPassword(const QString& password, const QString& hash) const;


    static const QString kDummyHash = "$argon2id$v=19$m=65536,t=2,p=1$O+EEKGud6QGM+vHcQ4YQ/A$HyZiKG5CGnRfX2s3isoxuu16oZxXRoNk7oe6BVnZg74";
};

#endif // USERSERVICE_H
