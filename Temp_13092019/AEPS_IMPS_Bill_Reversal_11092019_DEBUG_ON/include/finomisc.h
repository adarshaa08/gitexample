#ifndef MISC_H
#define MISC_H

#include <string.h>
#include <QDebug>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QMessageBox>
#include <QFile>
#include <QLabel>

#include "displaymessage.h"
#include "FPB.h"

extern "C"
{
#include "gl11.h"
#include "CHeader.h"
#include "glHeaderGSM.h"

}

//char MER_FLAG = 0;

class CMisc
{
public:
    CMisc();

    void AudioTest(char *);
    int GetAudioFlag();
    void DisplayMsgBox(char *);
    int DisplayMsgBox_SHG(char *);
    void rupay_DisplayMsgBox(char *);
    int DisplayMsgBox_MerchntDash(char *, char *);
    int GetMachineId(char *machineid);
    int get_line_init(char *buff, FILE  *fd);

    int play(char *str);
    int sound_test (char * filename);
    int get_audio_file(int num, char *buff);
    int mp3play(char *buff);

    void ltrim( char *string, char *trim );
    void rtrim(char *string, char *trim);

    int chngepasswd(void);
    int updateversion(void);
    int logout(void);

    int ConnectGPRS_new();
    int ConnectGPRS();
    int DisConnectGPRS();
    int DisConnectGPRS_new();
    bool GPRS_isAvailable();

    int CHECK_IPADDRESS(char *);
};

#endif // MISC_H
