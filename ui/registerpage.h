#ifndef REGISTERPAGE_H
#define REGISTERPAGE_H

#include <QWidget>
#include "globals.h"
#include "appcontext.h"

namespace Ui {
class RegisterPage;
}

class RegisterPage : public QWidget
{
    Q_OBJECT

public:
    explicit RegisterPage(AppContext& context, QWidget *parent = nullptr);
    ~RegisterPage();

private:
    Ui::RegisterPage *ui;
	AppContext& m_context;
};

#endif // REGISTERPAGE_H
