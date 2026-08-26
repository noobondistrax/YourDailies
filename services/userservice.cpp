#include "userservice.h"
#include "userSession.h"
#include "userModel.h"
#include <sodium.h>


UserService::UserService(UserRepository& userRepository, WidgetService& widgetService)
	: m_userRepository(userRepository), m_widgetService(widgetService)
{

}

bool UserService::login(const QString& email, const QString& password, UserSession& session) {

	std::optional<UserModel> result = m_userRepository.loadByMail(email);

	if (!result.has_value()) {
		verifyPassword(password, kDummyHash);
		qDebug() << "Kein Benutzer gefunden oder Datenbankfehler";
		return false;
	}


	// build Usermodel
	UserModel& user = result.value();
	const bool verified = verifyPassword(password, user.uPwHash);
	
	if (!verified) {
		return false;
	}
	
	// build users Widgets
	std::optional<QList<WidgetModel>> optWidgets = m_widgetService.getWidgets(user.uID);
	QList<WidgetModel> widgetList = m_widgetService.getWidgets(user.uID).value_or(QList<WidgetModel>{});

	// save in Session
	session.setUser(std::move(user));
	session.setWidgets(std::move(widgetList));


	return true;
}

bool UserService::registerUser(const QString& name, const QString& email, const QString& password) {
	
	const QString hash = hashPassword(password);
	if (hash.isEmpty()) {
		qDebug() << "Passwort-Hashing fehlgeschlagen";
		return false;
	}

	UserModel user;
	user.uName = name;
	user.uMail = email;
	user.uPwHash = hash;
	user.uRequested = QDateTime::currentDateTime().toString(Qt::ISODate);
	
	if (!m_userRepository.adminExists()) {
		user.uRole = "admin";
		user.uStatus = "active";
		user.uConfirmed = QDateTime::currentDateTime().toString(Qt::ISODate);
	}
	else {
		user.uRole = "user";
		user.uStatus = "pending";
		user.uConfirmed = ""; // not confirmed yet
	}

	return m_userRepository.uCreate(user);
}



bool UserService::verifyPassword(const QString& password, const QString& hash) const
{
	if (hash.isEmpty()) {
		return false;
	}

	QByteArray passwordUtf8 = password.toUtf8();
	QByteArray hashUtf8 = hash.toUtf8();

	return crypto_pwhash_str_verify(hashUtf8.constData(), passwordUtf8.constData(),	static_cast<unsigned long long>(passwordUtf8.size())) == 0;
}


QString UserService::hashPassword(const QString& plain) const {
	char hashed[crypto_pwhash_STRBYTES];
	QByteArray plainUtf8 = plain.toUtf8();

	const int rc = crypto_pwhash_str(hashed,
		plainUtf8.constData(), plainUtf8.size(),
		crypto_pwhash_OPSLIMIT_INTERACTIVE,
		crypto_pwhash_MEMLIMIT_INTERACTIVE);

	if (rc != 0) {
		// z.B. Out-of-Memory - kommt in der Praxis extrem selten vor
		return {};
	}

	return QString::fromUtf8(hashed);
}


bool UserService::isUserActive(const UserModel& user) const {
	if (user.uStatus == "active") {
		return true;
	}
	return false;	
}