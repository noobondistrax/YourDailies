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

    // Fixer, gültiger Argon2id-Hash - z.B. Hash von "dummy_password_never_used"
    // WICHTIG: Muss mit exakt denselben Argon2-Parametern erzeugt sein wie eure echten Hashes!
    static const QString kDummyHash;
};

#endif // USERSERVICE_H
