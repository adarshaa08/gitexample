#ifndef APPDOWNLOAD_H
#define APPDOWNLOAD_H
#define FTP_SUCCESS 0
#define FTP_FAILED -2

#include <QDebug>
#include <QFile>
#include <QSqlQuery>
#include <QVariant>
#include <QMessageBox>
#include <QPushButton>

#include "finomisc.h"
#include "usb.h"
#include "popmsgdlg.h"
#include "curltxns.h"
#include "ethernet.h"
#include "popmsgdlg.h"
#include<unifiedapplication.h>

#include "displaymessage.h"

class AppDownload
{

public:

    AppDownload();
    void ltrim( char *, char *);
    void on_Bttn_FTP_clicked();
    void on_Bttn_USB_clicked();
    int FTP_download_function(char *filename, int connection_type);
};

#endif // APPDOWNLOAD_H
