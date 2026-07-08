#include "logger.h"
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QDateTime>

/* Aufruf der Loggerfunktion:
 * Logger::instance().log(LogLevel::Info, "Programm gestartet");
 * oder
 * Logger::instance().log(LogLevel::Warn, "Benutzer existiert bereits");
 * ... usw.
 */

Logger::Logger() {

}

Logger& Logger::instance() {
    static Logger logger;
    return logger;
}

void Logger::log(LogLevel level, const QString& message)
{
    QString logRow = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss") + "[" + levelToString(level) + "] " + message;
    qDebug() << logRow;
    writeLog(logRow);
}

QString Logger::levelToString(LogLevel level) const
{
    switch (level)
    {
    case LogLevel::Info:
        return "INFO";

    case LogLevel::Warn:
        return "WARN";

    case LogLevel::Error:
        return "ERROR";
    }

    return "UNKNOWN";
}

void Logger::writeLog(QString& value) {
    QFile file(logFilePath123);

    if (!file.open(QIODevice::Append | QIODevice::Text))
    {
        qDebug() << "Datei konnte nicht geöffnet werden:" << logFilePath123;
    }

    QTextStream out(&file);
    out << value << "\n";
    file.close();
}