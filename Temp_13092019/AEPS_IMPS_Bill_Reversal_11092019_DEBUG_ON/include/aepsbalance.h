#ifndef AEPSBALANCE_H
#define AEPSBALANCE_H


typedef struct
{

/*Request data*/

    unsigned char ucRequestId_BEreq[20];
    unsigned char ucMethodId_BEreq[20];
    unsigned char ucTellerID_BEreq[20];
    unsigned char ucSessionId_BEreq[40];
    unsigned char ucTokenId_BEreq[20];
    unsigned char ucIsEncrypt_BEreq[20];
    unsigned char ucAuth_id_BEreq[20];
    unsigned char ucAadharNo_BEreq[20];
    unsigned char ucmerMobileNo_BEreq[11];//merchant mob no
    unsigned char ucOtp_BEreq[20];
    unsigned char ucUID_BEreq[20];
    unsigned char ucAuthType_BEreq[20];
    unsigned char ucPID_BEreq[20];
    unsigned char ucHmac_BEreq[20];
    unsigned char ucSkey_BEreq[20];
    unsigned char ucPidData_BEreq[20];
    unsigned char ucRC_BEreq[20];
    unsigned char ucFp_Auth_BEreq[20];
    unsigned char ucAuth_UID_BEreq[20];
    unsigned char ucData_PID_BEreq[20];
    unsigned char ucAuth_Hmac_BEreq[20];
    unsigned char ucAuth_Skey_value_BEreq[20];
    unsigned char ucMeta_lov_BEreq[20];
    unsigned char ucData_type_BEreq[20];
    unsigned char ucamount_BEreq[20];
    unsigned char uccard_acceptor_ID_BEreq[20];
    unsigned char uccard_acceptor_namelocation_BEreq[20];
    unsigned char ucTerminal_Id_BEreq[20];
    unsigned char ucTransactionTime_BEreq[20];
    unsigned char ucTransactionDate_BEreq[20];
    unsigned char ucAppId_BEreq[20];
    unsigned char ucNewAPI_Flag_BEreq[20];
    unsigned char ucAPI_Version_BEreq[20];
    unsigned char ucSessionExpiryTime_BEreq[25];
    unsigned char ucX_Auth_Token_BEreq[20];
    unsigned char ucChannelID_BEreq[20];
    unsigned char ucProductCode_BEreq[20];
    //unsigned char ucAmount_BEreq[20];
    unsigned char ucMerchantContribution_BEreq[20];
    unsigned char ucCharges_BEreq[20];
    unsigned char ucRouteID_BEreq[20];
    unsigned char ucSplitReferenceNo_BEreq[20];
    unsigned char ucServiceID_BEreq[20];
    unsigned char ucIdentifier_BEreq[20];
    unsigned char ucTxnHash_BEreq[20];
    char		  cAepsConsent[150];
    unsigned char ucBankName_BEreq[50];
    unsigned char ucnBIN_BEreq[15];
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
    unsigned char ucCardAcceptorNameLocation[50];
    unsigned char ucledgerBalance[20];
    unsigned char ucavailableBalance[20];
    unsigned char uccurrency[20];
    unsigned char ucBLTerminalId[20];//BL for Balance List
    unsigned char ucMessageId[20];
    unsigned char ucSessionExpiryTime[20];
    unsigned char ucSessionId[20];
    unsigned char ucRouteID[20];
    unsigned char ucErrorCode[20];
    unsigned char ucXMLData[20];
    unsigned char ucAuthmanFlag[20];
    unsigned char ucServiceID[20];



}AEPS_BALENQ;


AEPS_BALENQ AEPSBALENQ;

#endif // AEPSBALANCE_H
