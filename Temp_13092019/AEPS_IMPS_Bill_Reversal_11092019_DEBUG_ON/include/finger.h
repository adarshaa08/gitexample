#ifndef FINGER_H
#define FINGER_H

#include <QtGui/QWidget>
#include <QLabel>
#include <QLine>
#include <QPushButton>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QString>
#include <QDebug>
#include <QMessageBox>
#include <QDialog>
#include <QFile>
#include <QTimer>

#include "popmsgdlg.h"
#include "finomisc.h"
#include "auth_1.6.pb-c.h"
#include "unifiedapplication.h"

//#include "tfmapi.h"
//#include "tfmtypes.h"

extern "C"{
//#include "CHeader.h"
#include "gl11.h"
#include "tfmapi.h"
#include "tfmtypes.h"
#include "bio_api.h"
int isotobir(char *from , char *to);
}

#include "bclib.h"

class Finger : public QWidget
{
    Q_OBJECT
    
public:
    Finger(QWidget *parent = 0);

    unsigned char CARD_TYPE;
    int capEnroll(int);
    int captureFinger(void); // Adding for AEPS
    void set_malloc();
    void Print_Save_BIR_Template(void);
    int capScannerClose();
    void verify_save();
    int capVerify();
    int verify_captured_finger_with_livescan(char *file_location);
    int birtoiso(char *from , char *to);
    sint32 StoreDataToFile( const char *pFileName, const unsigned char *pBuf, uint32 lenData );
    sint32 LoadDataFromFile( const char *pFileName, unsigned char **ppBufer, uint32 *pLenData );

    ~Finger();
public slots:
    int Enroll(int);
    bool Verify();
    int FP_verify(int, int);
    int capScannerInitialize();
    PT_LONG LoadFromFile( const char *pFileName, unsigned char **ppBufer, PT_LONG *pLenData );


private:
    PT_STATUS Status;
    PT_CONNECTION handle;// typedef unsigned long PT_CONNECTION
    PT_CHAR dsn[30];
    PT_BIR *ppNewTemplate;
    PT_INPUT_BIR pStoredTemplate;
    PT_BOOL pboResult,boCapture;
    PT_SESSION_CFG_V5 *pCfg;
    PT_WORD cfgLen;
    PT_WORD *pdwScanQuality;

    unsigned char data_my[480];
    bool capScannerStatus,capEnrollStatus;
    int status,cnt,l;
    long int len;
    char buf[2048],*temp,ch;
    FILE *fp,*fp1;



    QLabel *label,*label1;
    QPushButton *button,*button1;
    QHBoxLayout *layout,*layout1;
    QVBoxLayout *vlayout;
    QMessageBox *msgbox;

};

#endif // FINGER_H
