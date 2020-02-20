#ifndef FINGER_H
#define FINGER_H

#include <QWidget>
#include <QFile>
#include <QMessageBox>
#include <QDebug>

extern "C"
{
#include "gl11.h"
#include "CHeader.h"
#include "bclib.h"
#include "tfmapi.h"
#include "tfmtypes.h"
#include "bio_api.h"
#include "Ilv_definitions.h"


}
#define CAPACITIVE 2
#define OPTICAL    1


#define FUN_OK     0
#define FUN_ERROR -1

class finger : public QObject
{
    Q_OBJECT
public:
   // explicit finger(QWidget *parent = 0);

    static finger *ins();
    static finger *sIns;
    explicit finger(QObject *parent=0);

    int capEnroll();
    int capEnroll_BFD(int finger_ID);
    bool capScannerStatus;
    void set_malloc();
    void Print_Save_BIR_Template(void);
    void Print_Save_BIR_Template_BFD(int finger_id);
    int capScannerClose();
    void verify_save();
    int capVerify();
    int Verify(char *,char *);
    int initScanner();
    bool Enroll(int id,int *Quality);
    int Finger_Enrollment(int iFpindex );
    int Fingerprint_Verification();
    int Scaner_Type();

    int isotobir(char *from , char *to);
    int bir_iso(char *bir_file, char *iso_file);
    PT_LONG LoadDataFromFile( const char *pFileName, unsigned char **ppBufer, PT_LONG *pLenData );
    sint32 StoreDataToFile( const char *pFileName, const unsigned char *pBuf, uint32 lenData );

    int finger_id;
    char mid[5];
    
signals:
    
public slots:


private:
    PT_STATUS Status;
    PT_CONNECTION handle;
    PT_CHAR dsn[30];
    PT_BIR *ppNewTemplate;
    PT_INPUT_BIR pStoredTemplate;
    PT_BOOL pboResult,boCapture;
    PT_SESSION_CFG_V5 *pCfg;
    PT_WORD cfgLen;
    PT_WORD *pdwScanQuality;

    unsigned char data_my[480];
    bool capEnrollStatus;

    int status,cnt,l;
    long int len;
    char buf[2048],*temp,ch;
    FILE *fp,*fp1;
    
};

#endif // FINGER_H
