#ifndef PRINT_H
#define PRINT_H


typedef struct
{
    int TxnType;
    unsigned char ucBCName[50];
    unsigned char ucAgentID[20];
    unsigned char ucBCLocation[100];
    unsigned char ucTerminalID[15];
    unsigned char ucCustAadharNo[15];
    unsigned char ucCustName[50];
    unsigned char ucSTAN[10];
    unsigned char ucRRN[25];
    unsigned char ucAUTHCode[90];
    unsigned char ucTranStatus[20];
    unsigned char ucACBalance[20];
    unsigned char ucAmount[25];
    unsigned char ucDate[20];
    unsigned char ucTime[20];
    unsigned char uctxntype[20];
    unsigned char ucBankName[50];
    unsigned char ucCustMobNo[11];
    unsigned char ucBeneAccNo[20];
    unsigned char ucBeneName[151];
    unsigned char ucCharges[10]; // 02052019
    unsigned char ucTotalAmt[20];
    unsigned char ucRefNo[15]; // 02052019
    unsigned char ucServiceProvider[60];
    unsigned char ucBBPS_TransID[25+1];
    unsigned char ucConsumerId[20];
    unsigned char ucbillamt[20];

}Printfpb;




#endif // PRINT_H
