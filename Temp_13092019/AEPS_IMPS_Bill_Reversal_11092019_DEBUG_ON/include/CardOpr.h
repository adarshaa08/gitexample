#ifndef CARDOPR_H
#define CARDOPR_H

#include <QDebug>

extern "C"{
#include "gl11.h"
//#include "tfmtypes.h"
//#include "tfmapi.h"
//#include "usb_api.h"
}


class cardopr
{
public:
    unsigned char FINGER_DATA[7000];
    unsigned char CARD_TYPE;

    int MSE_RESTORE(int);
    int SelectFile(int );
    int SelectSAM_File(int );
    int MSE_Set(unsigned char *);
    int GetChallenge(unsigned char *);
    int GetChallengeFromSAM(unsigned char *);
    int InternalAuthenticate(unsigned char *,unsigned char *, int , int );
    int ExternalAuthenticate(unsigned char *,unsigned char *, int , int );
    int GetData(int , char *, int );
    void rchar(char *,char );
    int GetCardNo(unsigned char *);
    int GetMFI_Code();
    int GetOperatorCardNo(unsigned char *);
    int init_smart_reader_2();
    int init_smart_reader_1();
    int close_smart_reader();
    int MutualAuthBEN_SAM(int );
    int MutualAuthCSP_SAM(int );
    int MutualAuthBEN_SAM_FP(int );
    int readBEN_FPData();
    int ReadFingerData(char *, int );

    int GetBenFingerIndexes(char *);
    int readOprFPData(int );
    int JustCheckCardPresence();
    int copy_template(char *file);

private:

};



#endif // CARDOPR.H
