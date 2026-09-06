#include <QMessageBox>
#include <optional>
#include "pwReset.h"
#include "ui/ui_pwReset.h"
#include "designer.h"


pwReset::pwReset(AppContext& appContext, QWidget* parent)
	: QWidget(parent)
    , ui(new Ui::pwReset)
	, m_context(appContext)
{
	ui->setupUi(this);

    const QList<QLineEdit*> edits = this->findChildren<QLineEdit*>();

    for (QLineEdit* edit : edits)
    {
        QPalette palette = edit->palette();
        palette.setColor(QPalette::PlaceholderText, QColor(148, 148, 148, 120));
        edit->setPalette(palette);
    }

    Designer::passwordToggle(ui->pwr_password);
    Designer::passwordToggle(ui->pwr_password_confirm);

    connect(ui->pwr_password, &QLineEdit::textChanged, this, &pwReset::validatePasswordMatch);
    connect(ui->pwr_password_confirm, &QLineEdit::textChanged, this, &pwReset::validatePasswordMatch);
	connect(ui->pwr_pB_back, &QPushButton::clicked, this, &pwReset::onBackClicked);

    for (int i = 1; i <= m_context.userService().getSecQuestions().size(); ++i) {
        ui->pwr_sec_question->addItem(m_context.userService().getSecQuestions()[i - 1], QVariant(i));
    }

    connect(ui->pwr_pButton, &QPushButton::clicked, this, &pwReset::onResetClicked);
}

pwReset::~pwReset()
{}

void pwReset::onResetClicked()
{
	QString email = ui->pwr_mail->text();
	QString pw = ui->pwr_password->text();
	QString pwConfirm = ui->pwr_password_confirm->text();
	int secQuestionId = ui->pwr_sec_question->currentData().toInt();
	QString secAnswer = ui->pwr_sec_answer->text();

    if (secAnswer.isEmpty() || email.isEmpty() || pw.isEmpty()) {
        QMessageBox::warning(this, "Password Reset Failed", "Please fill in all fields.");
        return;
    }

	if (pw != pwConfirm) {
		QMessageBox::warning(this, "Password Reset Failed", "Passwords do not match.");
		return;
	}

    if (!m_context.userService().resetPassword(email, pw, secQuestionId, secAnswer)) {
        QMessageBox::warning(this, "Password Reset Failed", "Failed to reset password.");
        return;
    }

    QMessageBox::information(this, "Password Reset Successful", "Your password has been reset successfully.");

	emit passwordResetted();
}

void pwReset::onBackClicked()
{
	emit backToLogin();
}

// Function to live-validate if the password and confirm-password fields match.
void pwReset::validatePasswordMatch()
{
    if (ui->pwr_password_confirm->text().isEmpty()) {
        markField(ui->pwr_password_confirm, std::nullopt);
        return;
    }

    const bool match = (ui->pwr_password->text() == ui->pwr_password_confirm->text());
    markField(ui->pwr_password_confirm, match);
}


// Function to show no border if field is empty
void pwReset::markField(QLineEdit* field, std::optional<bool> valid)
{
    if (!valid.has_value()) {
        field->setStyleSheet("padding-left: 8px; background-color: rgb(206, 206, 206);");
        return;
    }

    field->setStyleSheet(valid.value()
        ? "padding-left: 8px; background-color: rgb(206, 206, 206); border: 2px solid green;"
        : "padding-left: 8px; background-color: rgb(206, 206, 206); border: 2px solid red;");
}

