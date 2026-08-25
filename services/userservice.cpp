#include "userservice.h"
#include "userSession.h"
#include "userModel.h"
#include <sodium.h>


UserService::UserService(UserRepository& userRepository)
	: m_userRepository(userRepository)
{

}

bool UserService::login(const QString& email, const QString& password, UserSession& outSession) {

	std::optional<UserModel> result = m_userRepository.loadByMail(email);

	if (!result.has_value()) {
		verifyPassword(password, kDummyHash);
		qDebug() << "Kein Benutzer gefunden oder Datenbankfehler";
		return false;
	}

	UserModel& user = result.value();
	const bool verified = verifyPassword(password, user.uPwHash);
	
	if (!verified) {
		return false;
	}

	outSession.setUser(std::move(user));
	//outSession.setWidgets(std::move(..))

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

