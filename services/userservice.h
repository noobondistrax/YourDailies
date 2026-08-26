#ifndef USERSERVICE_H
#define USERSERVICE_H

#include <QString>
#include "appcontext.h"
#include "userRepository.h"
#include "widgetservice.h"
#include "userModel.h"

class UserService
{
public:
    explicit UserService(UserRepository& userRepository, WidgetService& widgetService);
    ~UserService() = default;

	bool login(const QString& email, const QString& password, UserSession& outSession);
	bool registerUser(const QString& name, const QString& email, const QString& password);
    bool isUserActive(const UserModel& user) const;

private:
	UserRepository& m_userRepository;
	WidgetService& m_widgetService;

    bool verifyPassword(const QString& password, const QString& hash) const;
    QString hashPassword(const QString& plain) const;


    static const QString kDummyHash = "$argon2id$v=19$m=65536,t=2,p=1$O+EEKGud6QGM+vHcQ4YQ/A$HyZiKG5CGnRfX2s3isoxuu16oZxXRoNk7oe6BVnZg74";
};

#endif // USERSERVICE_H
