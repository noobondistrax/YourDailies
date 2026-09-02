#ifndef USERMODEL_H
#define USERMODEL_H

#include <QString>

struct UserModel {
    int uID = 0;            // int
    QString uName;
    QString uMail;
    QString uPwHash;        
    QString uRole;          // Admin?
	QString uStatus;        // konsistenz zu Bezeichnung: 'active', 'inactive', 'pending'?
    QString uRequested;     // Datum der Anfrage
	QString uConfirmed;     // Datum der Bestätigung
};

struct SecAnswer {
	int questionId = 0;
    QString answerHash;
};

#endif // USERMODEL_H