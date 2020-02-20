#ifndef JSON_H
#define JSON_H
#include "FPB.h"

QT_BEGIN_INCLUDE_NAMESPACE
class FPB;
QT_END_INCLUDE_NAMESPACE

typedef struct
{
    char cTagName[100];
    char cTagValue[100];
    char cDepth;
    char cPtagName[6][100];

}TAGS;



class Json
{
public:
    Json(FPB *);

    FPB *wid_obj;
    //mWallet(unifiedapplication *);

   int iTagCount(unsigned char *iPBuff, char *tagToFind);
   int iIsJsonValid(char *cPacket,char *RespC, char *RespMes,int *type);
   void vWriteTag(TAGS *tags);
   int iParseFun(char *arr, int count,char *iFirstTag, char *iLastTag,int Type);
   int iParsJSON(unsigned char *cPacket, char *RespC, char *RespMes);
   int iParseGP_ABM(char *ifilename,char *iFirstParseData,char *iLastParseData,int *txntype);

private:


};

#endif // JSON_H
