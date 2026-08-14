/*
#include "appcontext.h"
#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

#include <sodium.h>
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

    

    //AppContext appContext;

    

    //MainWindow w(AppContext& appContext);
    // w.show();

    return QApplication::exec();
}
*/

#include <sodium.h>
#include <QDebug>
#include <QCoreApplication>

int main(int argc, char* argv[])
{
    QCoreApplication a(argc, argv);   // reicht für Konsolen-Test, kein GUI nötig

    if (sodium_init() < 0) {
        qDebug() << "libsodium init FAILED";
        return 1;
    }

    const char* password = "test123";
    char hashedBuffer[crypto_pwhash_STRBYTES];

    int result = crypto_pwhash_str(
        hashedBuffer,
        password, strlen(password),
        crypto_pwhash_OPSLIMIT_INTERACTIVE,
        crypto_pwhash_MEMLIMIT_INTERACTIVE
    );

    qDebug() << "Hash result:" << result << "(0 = OK)";
    qDebug() << "Hash:" << hashedBuffer;

    int verifyResult = crypto_pwhash_str_verify(hashedBuffer, password, strlen(password));
    qDebug() << "Verify result:" << verifyResult << "(0 = OK)";

    return 0;
}