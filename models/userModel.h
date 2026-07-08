#ifndef USERMODEL_H
#define USERMODEL_H

#include <QString>

struct UserModel {
    int uID;
    QString uName;
    QString uMail;
    QString uPwHash;
    QString uRole;
    QString uStatus;
    QString uRequested;
    QString uConfirmed;
};

#endif // USERMODEL_H