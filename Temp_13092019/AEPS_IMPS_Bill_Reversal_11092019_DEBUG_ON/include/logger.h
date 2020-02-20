#ifndef LOGGER_H
#define LOGGER_H

#include <QFile>
#include <QTextStream>

class Logger {

private:
    QFile infile;

public:
    Logger();
    void LogHeader();   
    void LogHandler(QtMsgType Type,QString msg);    
    void Close();
    ~Logger();
};

#endif // LOGGER_H
