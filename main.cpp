#include "appcontext.h"
#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

#include <QDebug>

int main(int argc, char *argv[])
{   
    QApplication a(argc, argv);

    a.setStyle("Fusion");

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "YourDailies_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
     
    AppContext appContext;

    MainWindow mainWindow(appContext);
    mainWindow.show();

    return QApplication::exec();
}