#ifndef PWRESET_H
#define PWRESET_H	

#include <QWidget>
#include <QLineEdit>
#include "appcontext.h"

namespace Ui {
class pwReset;
}

class pwReset : public QWidget
{
	Q_OBJECT

public:
	explicit pwReset(AppContext& appContext, QWidget* parent = nullptr);
	~pwReset();

signals:
	void passwordResetted();
	void backToLogin();

private slots:
	void onResetClicked();
	void onBackClicked();

private:
	Ui::pwReset *ui;
	AppContext& m_context;

	void validatePasswordMatch();
	void markField(QLineEdit* field, std::optional<bool> valid);
};

#endif // PWRESET_H