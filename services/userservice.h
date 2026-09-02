#ifndef USERSERVICE_H
#define USERSERVICE_H

#include <QString>
#include <qlist.h>
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
	bool registerUser(const QString& name, const QString& email, const QString& password, int questionId, const QString& secAnswer);
    bool isUserActive(const UserModel& user) const;
	bool resetPassword(const QString& email, const QString& newPassword, int questionId, const QString& secAnswer);

	QList<QString> getSecQuestions() const { return m_secQuestions; }

private:
	UserRepository& m_userRepository;
	WidgetService& m_widgetService;

    bool verifyHash(const QString& plain, const QString& hash) const;
    QString hashing(const QString& plain) const;


    static const QString kDummyHash = "$argon2id$v=19$m=65536,t=2,p=1$O+EEKGud6QGM+vHcQ4YQ/A$HyZiKG5CGnRfX2s3isoxuu16oZxXRoNk7oe6BVnZg74";

	QList<QString> m_secQuestions = {
		"Wie hieß dein / e erste / r Lehrer / In ? ",
		"Was ist dein Lieblingshobby?",
		"In welcher Stadt bist du geboren1?",
		"Welcher ist dein Lieblingsverein?",
		"Was ist dein Lieblingsessen?",
		"Was ist dein Lieblingstier?",
		"In welcher Schule warst Du in der 6. Klasse?"
	};
};

#endif // USERSERVICE_H
