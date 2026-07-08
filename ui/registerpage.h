#ifndef REGISTERPAGE_H
#define REGISTERPAGE_H

#include <QWidget>
#include "globals.h"

namespace Ui {
class RegisterPage;
}

class RegisterPage : public QWidget
{
    Q_OBJECT

public:
    explicit RegisterPage(QWidget *parent = nullptr);
    ~RegisterPage();

private:
    Ui::RegisterPage *ui;
};

#endif // REGISTERPAGE_H
