#ifndef ISOMESSAGE_H
#define ISOMESSAGE_H
#include <QString>


#include "unifiedapplication.h"


QT_BEGIN_INCLUDE_NAMESPACE
class unifiedapplication;
QT_BEGIN_INCLUDE_NAMESPACE

class isomessage
{
public:
    isomessage(unifiedapplication*);
    unifiedapplication *wid;
    QString URN;
    QString auth_ID_Resp;
    QString processingCode;
    QString STAN;
    QString TxnDate;
    QString TxnTime;
    QString NIIVal;
    QString RRN;
    QString Resp;
    QString TermId;
    QString reqAmt;
    QString TxnCode;
    QString CustName;
    QString AccBal;
    QString AccNo;
    QString invoiceNumber;
    QString Date;
    QString Time;
    QString primary_BitMap;
    QString fromAccountNumber;
    QString toAccountNumber;
    QString miniStat_Details;
    QString LogOnKey;
    int Len;

    //--------------Common Structure-------------------------
     QString PrepareTxnRequest(ISO *,merged_transaction_structure *,int tranType, int iRev);
     int PrepareTxnRequest_UBI(ISO *IsoBuff,merged_transaction_structure *IsoMsg,int reversal_tran);
     int EKYCTxnRequestPacket(merged_transaction_structure *IsoMsg);
     int addToPacket(unsigned char *szReqBuf, int offset, unsigned char *szData, int iLenData);
     void vAscii2BCD (unsigned char *pucSrc, unsigned char  *pucDst, int inSize);
     char cHexDigit_to_Nibble1 (unsigned char c);
     QString convertBitMaptoBIN128(QString,QString); // added by akshay
     QString PrepareTxnRequest_BFD(ISO *IsoBuff,merged_transaction_structure *IsoMsg,int tranType ,int iRev);
    //--------------------------------------------------- ----
    bool rupayUnPack(QString) ;

private:

    QString hexToString(QString convertStr) ;
    QString convertBINtoBitMap(QChar *p1,int len);
    int checkforMessageType(QString str);
    QString convertBitMaptoBIN(QString);
    int checkBitMap(QString , QString,merged_transaction_response_structure *);
    int checkBitMap_UBI(QString , QString,merged_transaction_response_structure *);
    int checkBitMap_BFD(QString , QString,merged_transaction_response_structure *);
    int processingAccountDetails(QString);
    void ClearResponseStructure();
};

#endif // ISOMESSAGE_H
