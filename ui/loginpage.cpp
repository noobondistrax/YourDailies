#include "loginpage.h"
#include "ui/ui_loginpage.h"
#include "designer.h"
#include <QMessageBox>

LoginPage::LoginPage(AppContext& appContext, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LoginPage)
    , m_context(appContext)
{
    ui->setupUi(this);

    Designer::passwordToggle(ui->login_password);

    connect(ui->login_pB_login, &QPushButton::clicked, this, &LoginPage::onLoginClicked);
    connect(ui->login_pB_register, &QPushButton::clicked, this, &LoginPage::onRegisterClicked);
    connect(ui->login_pB_forget, &QPushButton::clicked, this, &LoginPage::onForgetClicked);
}

LoginPage::~LoginPage()
{
    delete ui;
}

void LoginPage::onLoginClicked() {
	const QString email = ui->login_mail->text();
	const QString password = ui->login_password->text();

    UserSession session;
	bool result = m_context.userService().login(email, password, session);

	// if any Data (PW or E-Mail) is wrong, show a warning and return
	if (!result) {
        QMessageBox::warning(this, "Login Failed", "Invalid Logindata");
		return;
    }

	// Check if the user account is active
    if (!m_context.userService().isUserActive(session.user())) {
        QMessageBox::warning(this, "Login Failed", "Account is not active. Please contact support.");
		return;
    }

    m_context.startSession(std::move(session));

    emit loginSucceeded();
}