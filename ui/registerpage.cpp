#include <QPalette>
#include <qcombobox.h>
#include <QList>
#include <qmessagebox.h>
#include "registerpage.h"
#include "ui/ui_registerpage.h"
#include "designer.h"


RegisterPage::RegisterPage(AppContext& context, QWidget *parent)
    : QWidget(parent)
    , m_context(context)
    , ui(new Ui::RegisterPage)
{
    ui->setupUi(this);

    const QList<QLineEdit*> edits = this->findChildren<QLineEdit*>();

    for (QLineEdit* edit : edits)
    {
        QPalette palette = edit->palette();
        palette.setColor(QPalette::PlaceholderText, QColor(148, 148, 148, 120));
        edit->setPalette(palette);
    }

	for (int i = 1; i <= m_context.userService().getSecQuestions().size(); ++i)  {
		ui->register_sec_question->addItem(m_context.userService().getSecQuestions()[i - 1], QVariant(i));
	}

    Designer::passwordToggle(ui->register_password);
    Designer::passwordToggle(ui->register_password_confirm);

    // Live-validate if normal- and confirmfields are matching.
    connect(ui->register_email, &QLineEdit::textChanged, this, &RegisterPage::validateEmailMatch);
    connect(ui->register_email_confirm, &QLineEdit::textChanged, this, &RegisterPage::validateEmailMatch);

    connect(ui->register_password, &QLineEdit::textChanged, this, &RegisterPage::validatePasswordMatch);
    connect(ui->register_password_confirm, &QLineEdit::textChanged, this, &RegisterPage::validatePasswordMatch);

    // Event on clicking the registerbutton
    connect(ui->register_pB_register, &QPushButton::clicked, this, &RegisterPage::onRegisterClicked);
}

RegisterPage::~RegisterPage()
{
    delete ui;
}

// Function for validating if the email and confirm-email fields match.
void RegisterPage::validateEmailMatch()
{
    // Solange das Confirm-Feld noch leer ist: neutral, noch keine Bewertung moeglich
    if (ui->register_email_confirm->text().isEmpty()) {
        markField(ui->register_email_confirm, std::nullopt);
        return;
    }

    const bool match = (ui->register_email->text() == ui->register_email_confirm->text());
    markField(ui->register_email_confirm, match);
}


// Function for validating if the password and confirm-password fields match.
void RegisterPage::validatePasswordMatch()
{
    if (ui->register_password_confirm->text().isEmpty()) {
        markField(ui->register_password_confirm, std::nullopt);
        return;
    }

    const bool match = (ui->register_password->text() == ui->register_password_confirm->text());
    markField(ui->register_password_confirm, match);
}


// Function to show no border if field is empty
void RegisterPage::markField(QLineEdit* field, std::optional<bool> valid)
{
    if (!valid.has_value()) {
        field->setStyleSheet("padding-left: 8px; background-color: rgb(206, 206, 206);");
        return;
    }

    field->setStyleSheet(valid.value()
        ? "padding-left: 8px; background-color: rgb(206, 206, 206); border: 2px solid green;"
        : "padding-left: 8px; background-color: rgb(206, 206, 206); border: 2px solid red;");
}


void RegisterPage::onRegisterClicked()
{
    const QString name = ui->register_name->text();
    const QString email1 = ui->register_email->text();
    const QString email2 = ui->register_email_confirm->text();
    const QString password1 = ui->register_password->text();
    const QString password2 = ui->register_password_confirm->text();
    int questionId = ui->register_sec_question->currentData().toInt();
	const QString secAnswer = ui->register_sec_answer->text();

	if (name.isEmpty() || email1.isEmpty() || email2.isEmpty() || password1.isEmpty() || password2.isEmpty() || secAnswer.isEmpty() ) {        
        QMessageBox::warning(this, "Registration Failed", "Please fill in all fields.");
		return;
	}

	if (email1 != email2 || password1 != password2) {
		QMessageBox::warning(this, "Registration Failed", "E-Mmails or Passwords do not match.");
		return;
	}

    if (!m_context.userService().registerUser(name, email1, password1, questionId, secAnswer)) {
        QMessageBox::warning(this, "Registration Failed", "Failed to register user.");
    }

	QMessageBox::information(this, "Registration Successful", "Your registration was successful. Please wait for admin approval."); 

	emit RegisterSucceeded();
}
