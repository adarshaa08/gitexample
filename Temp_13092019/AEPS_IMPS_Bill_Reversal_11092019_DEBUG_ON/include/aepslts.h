#ifndef AEPSLTS_H
#define AEPSLTS_H

typedef struct
{

    /*Request data*/


    char ucRequestId_ALTSreq[30];
    unsigned char ucMethodId_ALTSreq[5];
    unsigned char ucTellerID_ALTSreq[15];
    unsigned char ucSessionId_ALTSreq[40];
    unsigned char ucTokenId_ALTSreq[20];
    unsigned char ucIsEncrypt_ALTSreq[9];
    unsigned char ucStartDate_ALTSreq[12];
    unsigned char ucEndDate_ALTSreq[12];
    unsigned char ucX_CORRELATION_ID_ALTSreq[40];
    unsigned char ucUserId_ALTSreq[20];
    unsigned char ucTransactionType_ALTSreq[20];
    unsigned char ucSessionExpiryTime_ALTSreq[25];
    unsigned char ucX_Auth_Token_ALTSreq[20];
    unsigned char ucChannelID_ALTSreq[5];
    unsigned char ucProductCode_ALTSreq[20];
    unsigned char ucRouteID_ALTSreq[50];
    unsigned char ucServiceID_ALTSreq[5];

    /*Response Data*/

    unsigned char ucRequestId[20];
    unsigned char ucDisplayMessage[20];
    unsigned char ucResponseMessage[35];
    unsigned char ucResponseCode[20];
    unsigned char ucMessageType[20];
    unsigned char ucreturnCode[20];
    unsigned char ucAuthResponse[20];
    unsigned char ucAORreturnCode[20];//AOR for AEPS Offus response
    unsigned char ucAORresponseMessage[20];
    unsigned char uctxnReferenceNo[20];
    unsigned char ucAuthCode[20];
    unsigned char ucTransactionTime[20];
    unsigned char ucTransactionDate[20];
    unsigned char ucAuthUID[20];
    unsigned char ucTerminalId[20];
    unsigned char uccardAcceptorCode[20];
    unsigned char ucCardAcceptorNameLocation[20];
    unsigned char ucledgerBalance[20];
    unsigned char ucavailableBalance[20];
    unsigned char uccurrency[20];
    unsigned char ucBLTerminalId[20];//BL for Balance List
    unsigned char ucMessageId[20];
    unsigned char ucSessionExpiryTime[20];
    unsigned char ucSessionId[20];
    unsigned char ucRouteID[40];
    unsigned char ucErrorCode[20];
    unsigned char ucXMLData[20];
    unsigned char ucAuthmanFlag[20];
    unsigned char ucServiceID[20];


}AEPS_LTS;


AEPS_LTS AEPSLTS;










#endif // AEPSLTS_H
