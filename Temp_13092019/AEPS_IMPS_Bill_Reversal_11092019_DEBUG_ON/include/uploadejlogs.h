#ifndef UPLOADEJLOGS_H
#define UPLOADEJLOGS_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QVariant>
#include <QFile>
#include <QWidget>
#include <QTimer>

#include "misc.h"
#include "curltxns.h"
#include "print.h"
#include "popupmsgdlg.h"



class CUploadEJLogs : public QWidget
{
public:
    explicit CUploadEJLogs(QWidget *parent = 0);
    //~CUploadEJLogs();

    //CUploadEJLogs();
    int gl_cnt;
    int SendReqGetResp();
    int Upload_EJ_Logs();
    int getServerIP_PortFromDB(char *, char *);
    int getSoapReqDataFromDB(char *, char *, char *);
    int ReadRespFile(char *);
};

#endif // UPLOADEJLOGS_H
