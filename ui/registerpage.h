#ifndef REGISTERPAGE_H
#define REGISTERPAGE_H

#include <QWidget>
#include "globals.h"
#include "appcontext.h"

class QLineEdit;

namespace Ui {
class RegisterPage;
}

class RegisterPage : public QWidget
{
    Q_OBJECT

public:
    explicit RegisterPage(AppContext& context, QWidget *parent = nullptr);
    ~RegisterPage();

signals:
    void RegisterSucceeded();

private slots:
    void validateEmailMatch();
    void validatePasswordMatch();
    void onRegisterClicked();

private:
    Ui::RegisterPage *ui;
	AppContext& m_context;

	// markField is a helper function to change the color of the QLineEdit field based on the validation (normal- and confirmfields) result.
    static void markField(QLineEdit* field, std::optional<bool> valid);
};

#endif // REGISTERPAGE_H
