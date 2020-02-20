#ifndef SELECTSCANNER_H
#define SELECTSCANNER_H

#include <QWidget>
#include <QDebug>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

extern "C"{
#include "gl11.h"
}


class SelectScanner : public QWidget
{
    Q_OBJECT
public:
    explicit SelectScanner(QWidget *parent = 0);

    int scanner_status_opt1();
signals:
    
public slots:
    
};

#endif // SELECTSCANNER_H
