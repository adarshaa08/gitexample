#ifndef LTS_H
#define LTS_H

typedef struct
{

/*Request data*/

    unsigned char ucRequestId_LTSreq[30];
    unsigned char ucMethodId_LTSreq[5];
    unsigned char ucTellerID_LTSreq[15];
    unsigned char ucSessionId_LTSreq[40];
    unsigned char ucTokenId_LTSreq[20];
    unsigned char ucIsEncrypt_LTSreq[9];
    unsigned char ucX_CORRELATION_ID_LTSreq[40];
    unsigned char ucUserId_LTSreq[20];
    unsigned char ucTransactionType_LTSreq[20];
    unsigned char ucSessionExpiryTime_LTSreq[25];
    unsigned char ucX_Auth_Token_LTSreq[20];
    unsigned char ucChannelID_LTSreq[5];
    unsigned char ucProductCode_LTSreq[20];
    unsigned char ucRouteID_LTSreq[50];
    unsigned char ucServiceID_LTSreq[5];

/*Response Data*/

    unsigned char ucRequestId[20];
    unsigned char ucDisplayMessage[20];
    unsigned char ucResponseMessage[90];
    unsigned char ucResponseCode[20];
    unsigned char ucMessageType[20];
    unsigned char ucreturnCode[20];
    unsigned char ucX_CORRELATION_ID[20];
    unsigned char ucTrace[20];
    unsigned char ucRec1PCODE1[20];
    unsigned char ucRec1MSGTYPE[20];
    unsigned char ucRec1RESPCODE[20];
    unsigned char ucRec1ResponseMsg[90];
    unsigned char ucRec2PCODE2[20];
    unsigned char ucRec2RESPCODE[20];
    unsigned char ucMessageId[20];
    unsigned char ucSessionExpiryTime[20];
    unsigned char ucSessionId[20];
    unsigned char ucRouteID[20];
    unsigned char ucErrorCode[20];
    unsigned char ucXMLData[20];
    unsigned char ucAuthmanFlag[20];
    unsigned char ucServiceID[20];


}_LTS;


_LTS LTS;



#endif // LTS_H
