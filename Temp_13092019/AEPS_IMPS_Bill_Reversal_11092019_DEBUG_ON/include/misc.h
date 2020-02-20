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

extern "C"
{
#include "gl11.h"
#include "CHeader.h"
#include "glHeaderGSM.h"
}

class CMisc
{
public:
    CMisc();

    void AudioTest(char *);
    int GetAudioFlag();
    void DisplayMsgBox(char *);
    int GetMachineId(char *machineid);
    int get_line_init(char *buff, FILE  *fd);

    bool GPRS_isAvailable();
    int play(char *str);
    int sound_test (char * filename);
    int get_audio_file(int num, char *buff);
    int mp3play(char *buff);

    void ltrim( char *string, char *trim );
    void rtrim(char *string, char *trim);

    int ConnectGPRS();
    int DisConnectGPRS();

    int CHECK_IPADDRESS(char *);

};

#endif // MISC_H
