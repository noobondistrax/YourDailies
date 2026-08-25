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



bool UserService::verifyPassword(const QString& password, const QString& hash) const
{
	if (hash.isEmpty()) {
		return false;
	}

	QByteArray passwordUtf8 = password.toUtf8();
	QByteArray hashUtf8 = hash.toUtf8();

	return crypto_pwhash_str_verify(hashUtf8.constData(), passwordUtf8.constData(),	static_cast<unsigned long long>(passwordUtf8.size())) == 0;
}

