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
		verifyHash(password, kDummyHash);
		qDebug() << "Kein Benutzer gefunden oder Datenbankfehler";
		return false;
	}


	// build Usermodel
	UserModel& user = result.value();
	const bool verified = verifyHash(password, user.uPwHash);
	
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

/*
* 0 = failed
* 1 = success user
* 2 = success admin
*/
int UserService::registerUser(const QString& name, const QString& email, const QString& password, int questionId, const QString& secAnswer) {
	
	const QString hashPW = hashing(password);

	if (hashPW.isEmpty()) {
		qDebug() << "Password-Hashing failed";
		return 0;
	}

	const QString hashSec = hashing(secAnswer.toLower().trimmed());
	if (hashSec.isEmpty()) {
		qDebug() << "Securityquestion-answer-hashing failed";
		return 0;
	}

	UserModel user;
	user.uName = name;
	user.uMail = email;
	user.uPwHash = hashPW;
	user.uRequested = QDateTime::currentDateTime().toString(Qt::ISODate);

	bool isAdmin = false;

	if (!m_userRepository.adminExists()) {
		isAdmin = true;
		user.uRole = "admin";
		user.uStatus = "active";
		user.uConfirmed = QDateTime::currentDateTime().toString(Qt::ISODate);
	}
	else {
		user.uRole = "user";
		user.uStatus = "pending";
		user.uConfirmed = ""; // not confirmed yet
	}

	bool success = m_userRepository.uCreate(user, questionId, hashSec);
	if (success) {
		return isAdmin ? 2 : 1;
	}
	else {
		qDebug() << "registration failed";
		return 0;
	}
}

bool UserService::resetPassword(const QString& email, const QString& newPassword, int questionId, const QString& plainAnswer) {
	std::optional<UserModel> optUser = m_userRepository.loadByMail(email);
	if (!optUser.has_value()) {
		verifyHash(newPassword, kDummyHash);
		qDebug() << "No User found or Database error";
		return false;
	}

	UserModel user = optUser.value();

	std::optional<SecAnswer> optSecAnswer = m_userRepository.getSecAnswer(user.uID);
	if (!optSecAnswer.has_value()) {
		qDebug() << "No Security Answer found or Database error	";
		return false;
	}

	SecAnswer answerStruct = optSecAnswer.value();
	if (answerStruct.questionId != questionId) {
		qDebug() << "Security question does not match";
		return false;
	}

	if (!verifyHash(plainAnswer.toLower().trimmed(), answerStruct.answerHash)) {
		qDebug() << "Security answer is incorrect";
		return false;
	}
		
	user.uPwHash = hashing(newPassword);
	
	return m_userRepository.uUpdate(user);
}

bool UserService::verifyHash(const QString& plain, const QString& hash) const
{
	if (hash.isEmpty()) {
		return false;
	}

	QByteArray plainUtf8 = plain.toUtf8();
	QByteArray hashUtf8 = hash.toUtf8();

	return crypto_pwhash_str_verify(hashUtf8.constData(), plainUtf8.constData(),	static_cast<unsigned long long>(plainUtf8.size())) == 0;
}

QString UserService::hashing(const QString& plain) const {
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