#include "userservice.h"
#include "userSession.h"
#include "userModel.h"


UserService::UserService(UserRepository& userRepository)
	: m_userRepository(userRepository)
{

}

bool UserService::login(const QString& email, const QString& password, UserSession& outSession) {

	// Check ob Email und HashPasswort stimmmen   - repository loadByMail(const QString& mail)

	UserModel user = m_userRepository.loadByMail(email);

		
		//UserRepository::loadByMail(const QString & mail)


	// zum schluss return - Login erfolgreich (true) oder nicht (false)
}

//bool verifyPassword(const QString& password, const QString& hash) const;
// einmalig offline erzeugen, z.B.per argon2 CLI - Tool oder eurer eigenen Hash - Funktion
const QString UserService::kDummyHash = "$argon2id$v=19$m=65536,t=3,p=4$c29tZXNhbHQ$dummyHashValueHere...";

