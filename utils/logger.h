#ifndef LOGGER_H
#define LOGGER_H

#include <QString>

const QString logFilePath123 = "log/log.csv";

enum class LogLevel
{
    Info,
    Warn,
    Error

};

class Logger
{
public:
    static Logger& instance();

    void log(LogLevel level, const QString& message);


private:
    Logger();

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    QString levelToString(LogLevel level) const;
    void writeLog(QString& value);
};

#endif // LOGGER