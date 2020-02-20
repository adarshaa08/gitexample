#ifndef OPTICALSCANNER_H
#define OPTICALSCANNER_H

#include <QWidget>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<signal.h>
#include <mcheck.h>
#include <QPushButton>
#include <QMessageBox>

#include "finomisc.h"

extern "C"
{
#include "bio_api.h"
#include "gl11.h"
}


class OpticalScanner : public QWidget
{
    Q_OBJECT
public:
    explicit OpticalScanner(QWidget *parent = 0);

    int opt_wrappers(int, int, int, int);
    
signals:
    
public slots:
    
};

#endif // OPTICALSCANNER_H
