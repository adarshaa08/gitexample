#include <QtGui/QApplication>
#include "FPB.h"
int ScannerType = CAPACITIVE;
//Added the commit
void myMessageHandler(QtMsgType type, const char *msg)
{
    QString txt;
    QString time = QTime::currentTime().toString();
    switch (type) {
    case QtDebugMsg:
        txt = QString("%1 : %2").arg(time, msg);
        break;
    case QtWarningMsg:
        txt = QString("Warning: %1").arg(msg);
    break;
    case QtCriticalMsg:
        txt = QString("Warning: %1").arg(msg);
    break;
    case QtFatalMsg:
        txt = QString("Warning: %1").arg(msg);
        abort();
    }
    QFile outFile("/usr/FINO/FPB/debug");
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream ts(&outFile);
    ts << txt << endl;
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SelectScanner scannerObj;
    QMessageBox msg;
/*    if(QFile::exists("/usr/FINO/FPB/debug"))
    {
        qInstallMsgHandler(myMessageHandler);
    }*/

    gl11_optcap_pwron();
  //  sleep(2);
    int ret;

    ret = scannerObj.scanner_status_opt1();
    if(ret == 1)
    {
        ScannerType = OPTICAL;
        qDebug() << "Optical Scanner Selected";
    }
    else if(ret == 2)
    {
        ScannerType = CAPACITIVE;
        qDebug() << "Capacitive Scanner Selected";
    }
    else
    {
        msg.setText("Scanner Initialization FAIL..");
        msg.exec();
        return -1;
    }

    system("rm /mnt/jffs2/finger_print_opt");

    QFont newFont("Courier", 14, QFont::Bold, true);
    QApplication::setFont(newFont);
    FPB w;
    w.show();
    
    return a.exec();
}
