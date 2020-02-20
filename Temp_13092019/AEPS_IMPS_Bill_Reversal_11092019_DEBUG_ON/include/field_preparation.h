#ifndef FIELD_PREPARATION_H
#define FIELD_PREPARATION_H
//#include "dl_iso8583.h"
//#include "dl_iso8583_defs_1993.h"
//#include "dl_output.h"
#include <QWidget>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
//#include "dl_iso8583.h"
//#include "dl_iso8583_defs_1993.h"
//#include "dl_output.h"
#include <iostream>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<unistd.h>
#include<time.h>
#include<sys/time.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<dlfcn.h>
#include<string.h>
#include<termios.h>
#include<errno.h>
#include <sys/signal.h>
#include <sys/ioctl.h>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <stdio.h>
#include <QFile>
class Field_Preparation : public QWidget
{
    Q_OBJECT
public:
    explicit Field_Preparation(QWidget *parent = 0);

    char * packField63(void);
    char * packField63Fusion(void);
    char * packField63Exception(void);
    char * packField126(void);
    char * packField127(void);
    int get_ip(char *gprsip);
signals:

public slots:
    
};

#endif // FIELD_PREPARATION_H
