#ifndef LOGINPAGE_H
#define LOGINPAGE_H

#include <QWidget>
#include "appcontext.h"

namespace Ui {
class LoginPage;
}

class LoginPage : public QWidget
{
    Q_OBJECT

public:
    explicit LoginPage(AppContext& appContext, QWidget *parent = nullptr);
    ~LoginPage();

signals:
    void loginSucceeded();
	void registerClicked();
    void forgotPasswordRequested();

private slots:
    void onLoginClicked();
    void onRegisterClicked();
    void onForgetClicked();

private:
    Ui::LoginPage *ui;
    AppContext& m_context;
};

#endif // LOGINPAGE_H
