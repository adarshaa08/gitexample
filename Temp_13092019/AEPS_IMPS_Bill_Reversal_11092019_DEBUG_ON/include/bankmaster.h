#ifndef BANKMASTER_H
#define BANKMASTER_H

typedef struct
{

/*Request data*/

    unsigned char ucRequestId_ABMreq[30];
    unsigned char ucMethodId_ABMreq[20];
    unsigned char ucTellerID_ABMreq[20];
    unsigned char ucSessionId_ABMreq[40];
    unsigned char ucTokenId_ABMreq[20];
    unsigned char ucIsEncrypt_ABMreq[20];
    unsigned char ucRequestData_ABMreq[20];
    unsigned char ucSessionExpiryTime_ABMreq[20];
    unsigned char ucX_Auth_Token_ABMreq[20];
    unsigned char ucChannelID_ABMreq[20];
    unsigned char ucProductCode_ABMreq[20];
    unsigned char ucAmount_ABMreq[20];
    unsigned char ucMerchantContribution_ABMreq[20];
    unsigned char ucCharges_ABMreq[20];
    unsigned char ucRouteID_ABMreq[20];
    unsigned char ucSplitReferenceNo_ABMreq[20];
    unsigned char ucServiceID_ABMreq[20];
    unsigned char ucIdentifier_ABMreq[20];
    unsigned char ucTxnHash_ABMreq[20];

/*Response Data*/

    unsigned char ucRequestId[20];
    unsigned char ucDisplayMessage[30];
    unsigned char ucResponseMessage[50];
    unsigned char ucResponseCode[10];
    unsigned char ucMessageType[20];
    unsigned char ucRDBankName[50];
    unsigned char ucRDBankBin[10];
    unsigned char ucversionID[20];
    unsigned char ucMastersVersion[20];
    unsigned char ucMessageId[20];
    unsigned char ucSessionExpiryTime[30];
    unsigned char ucSessionId[20];
    unsigned char ucRouteID[70];
    unsigned char ucErrorCode[20];
    unsigned char ucXMLData[20];
    unsigned char ucAuthmanFlag[5];
    unsigned char ucServiceID[5];

}AEPS_BANKMAST;





#endif // BANKMASTER_H
