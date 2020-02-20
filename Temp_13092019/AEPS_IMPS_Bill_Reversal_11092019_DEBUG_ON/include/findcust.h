#ifndef FINDCUST_H
#define FINDCUST_H

typedef struct
{
    /*	Request data*/

    unsigned char ucRequestID_FCreq[25];
    unsigned char ucMethodID_FCreq[5];
    unsigned char ucSessionId_FCreq[50];
    unsigned char ucTellerID_FCreq[10];

    unsigned char ucTokenID_FCreq[25];
    unsigned char ucX_Auth_Token_FCreq[25];
    unsigned char ucIsEncrypt_FCreq[25];
    unsigned char ucAmount_FCreq[11];
    unsigned char ucMerchantContribution_FCreq[25];
    unsigned char ucProductCode_FCreq[25];
    unsigned char ucRDcustomerName_FCreq[25];
    unsigned char ucRDmobileNumber_FCreq[11];
    unsigned char ucRDaadharNo_FCreq[13];
    unsigned char ucRDpanNo_FCreq[11];
    unsigned char ucRDdob_FCreq[11];
    unsigned char ucRDcustomerNo_FCreq[10];
    unsigned char ucRDidproofNo_FCreq[25];
    unsigned char ucRDaddressproofNo_FCreq[50];
    unsigned char ucRDchannelId_FCreq[25];
    unsigned char ucRDaccountNo_FCreq[15];
    unsigned char ucSessionExpiryTime_FCreq[25];

    /*	Response data*/


    unsigned char ucRDCLnumber_resp[12];
    unsigned char ucRDCLfullName_resp[35];

    unsigned char ucRDCMAaddressLine1[50]; // RDCMA = Response Data customerList Mailing Address
    unsigned char ucRDCMAaddressLine2[50];
    unsigned char ucRDCMAcity[20];
    unsigned char ucRDCMAstate[5];
    unsigned char ucRDCMAcountry[5];
    unsigned char ucRDCLmobilePhoneNumber[11];
    unsigned char ucRDCLnumber[12];
    unsigned char ucRDCLaadharNumber[12+1]; // 02052019
    unsigned char ucRDCLdescription[10][25];
    unsigned char ucRDCLunclearedBalance[10][15];
    unsigned char ucRDCLtaxIdNumber[12];
    unsigned char ucRDCLACCnumber[10][20];
    unsigned char ucRDCLACavailableBalance[10][15];

    //        unsigned char ucRequestId[25];
    //        unsigned char ucDisplayMessage[30];
    //        unsigned char ucResponseMessage[50];
    //        unsigned char ucResponseCode[30];
    //        unsigned char ucMessageType[30];
    //        unsigned char ucreturnCode[3];
    //        unsigned char ucAuthResponse[30];
    //        unsigned char ucAORreturnCode[5];//AOR AEPS Offus Response
    //        unsigned char ucresponseMessage[50];
    //        unsigned char ucAORtxnReferenceNo[20];//AOR AEPS Offus Response
    //        unsigned char ucAuthCode[30];
    //        unsigned char ucTransactionTime[20];
    //        unsigned char ucTransactionDate[20];
    //        unsigned char ucAuthUID[20];
    //        unsigned char ucAORTerminalId[20];
    //        unsigned char uccardAcceptorCode[20];
    //        unsigned char ucCardAcceptorNameLocation[50];
    //        unsigned char ucAORledgerBalance[15];
    //        unsigned char ucavailableBalance[15];
    //        unsigned char uccurrency[9];
    //        unsigned char ucPTRreturnCode[3];//PTR Post Transaction response
    //        unsigned char ucPTRresponseMessege[50];//PTR Post Transaction response msg added by vaishnavi 15-12-18
    //        /*unsigned char ucPTRtxnReferenceNo[15];
    //        unsigned char uccbsTxnReferenceNo[15];
    //        unsigned char ucPTRledgerBalance[10]; //PTR Post Transaction response
    //        unsigned char ucPTRavailableBalance[10];
    //        unsigned char ucPTRcurrency[5];*/
    //        unsigned char ucTerminalId[10];
    //        unsigned char ucMessageId[10];
    //        unsigned char ucSessionExpiryTime[15];
    //        unsigned char ucSessionId[50];
    //        unsigned char ucRouteID[55];
    //        unsigned char ucErrorCode[5];
    //        unsigned char ucXMLData[10];
    //        unsigned char ucAuthmanFlag[8];
    //        unsigned char ucServiceID[3];

}FINDCUSTOMER;


//FINDCUSTOMER FINDCUST;

#endif // FINDCUST_H
