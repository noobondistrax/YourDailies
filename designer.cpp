#include "designer.h"

#include <QLineEdit>
#include <QAction>
#include <QIcon>
#include <QObject>

/*
 * Aufrufe:
 *      Designer::styleAppBackground(ui->stackedWidget);    - für Gradient Background
 *      Designer::passwordToggle(ui->login_password);       - Toggles für Passwordfields
 *      Designer::passwordToggle(ui->register_password);
 *      Designer::passwordToggle(ui->register_password_confirm);
 */




{
    if (widget)
        widget->setStyleSheet("QStackedWidget#stackedWidget {background-color: qlineargradient("
                              "spread:pad,"
                              "x1:0,"
                              "y1:0,"
                              "x2:1,"
                              "y2:1,"
                              "stop:0 #959595,"
                              "stop:0.6 #bbbbbb,"
                              "stop:1 #E3E0DC);}");
}

void Designer::passwordToggle(QLineEdit *lineEdit)
{
    if (!lineEdit) return;

    lineEdit->setEchoMode(QLineEdit::Password);

    QAction *toggleAction = lineEdit->addAction(
        QIcon(":/icons/img/eye_closed.png"),
        QLineEdit::TrailingPosition
        );

    QObject::connect(toggleAction, &QAction::triggered, lineEdit, [lineEdit, toggleAction]() {
        bool hidden = (lineEdit->echoMode() == QLineEdit::Password);

        lineEdit->setEchoMode(hidden ? QLineEdit::Normal : QLineEdit::Password);
        toggleAction->setIcon(QIcon(hidden ? ":/icons/img/eye_open.png" : ":/icons/img/eye_closed.png"));
    });
}