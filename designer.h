#ifndef DESIGNER_H
#define DESIGNER_H

#pragma once
#include <QWidget>

class QLineEdit;

class Designer
{
public:
    static void styleAppBackground(QWidget *widget);
    static void passwordToggle(QLineEdit *lineEdit);
};

#endif // DESIGNER_H
