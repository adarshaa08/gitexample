#ifndef AEPSWTHDRAW_H
#define AEPSWTHDRAW_H
typedef struct
{
    /*	Request data*/

    unsigned char ucRequestID_AWreq[25];
    unsigned char ucMethodID_AWreq[5];
    unsigned char ucTellerID_AWreq[10];
    unsigned char ucSessionID_AWreq[50];
    unsigned char ucTokenID_AWreq[25];
    unsigned char ucIsEncrypt_AWreq[25];
    char		  cAepsConsent[150];
    unsigned char ucmobileNumber_AWreq[11];
    unsigned char ucmerchantMobileNumber_AWreq[11];
    unsigned char uctotalAmount_AWreq[11];
    unsigned char ucAuth_id_AWreq[30];
    unsigned char ucAadharNo_AWreq[13];
    unsigned char ucOtp_AWreq[5];
    unsigned char ucMobileNo_AWreq[11];

    unsigned char ucUID_AWreq[14];
    unsigned char ucAuthType_AWreq[4];
    unsigned char ucPID_AWreq[2000];
    unsigned char ucHmac_AWreq[70];
    unsigned char ucSkey_AWreq[400];
    unsigned char ucPidData_AWreq[6000];
    unsigned char ucRC_AWreq[2];
    unsigned char ucFp_Auth_AWreq[100];
    unsigned char ucPTappId_AWreq[8];//PT for Post Transaction
    unsigned char ucPTisClubbed_AWreq[2];
    unsigned char ucPTisInclusive_AWreq[2];
    unsigned char ucPTvalueDate_AWreq[15];
    unsigned char ucPTanalysisFlag_AWreq[10];
    unsigned char ucPTreversalFlag_AWreq[10];
    unsigned char ucPTreferenceNo_AWreq[20];
    unsigned char ucAFTaccountNumber_AWreq[20];//AFT for acctFundTransferLegs
    unsigned char ucAFTamount_AWreq[9];
    unsigned char ucAFTcurrency_AWreq[5];
    unsigned char ucAFTcreditDebitFlag_AWreq[3];
    unsigned char ucAFTtransactionType_AWreq[15];
    unsigned char ucAFTtransactionComment_AWreq[3];
    unsigned char ucAFTcostCenter_AWreq[5];


    unsigned char ucAFTsupportData_PVTXNID_AWreq[15];   // 36 PVTXNID Previous Successful Txn ID ..length
    unsigned char ucAFTsupportData_PVTXNDT_AWreq[15];  // 37 PVTXNDT Previous Successful Date 03\/03\/19
    unsigned char ucAFTsupportData_IPADDR_AWreq[15];   // 38 IPADDR len 13
    unsigned char ucAFTsupportData_DEVICEID_AWreq[20]; // 39 DEVICEID Device Serial Number (Mac ID) 980CA5F2BEF1
    unsigned char ucAFTsupportData_APPID_AWreq[20];   // 20 APPID
    unsigned char ucAFTsupportData_MCCCellID_AWreq[15]; // 40 Cell ID
    unsigned char ucAFTsupportData_MCCLAC_AWreq[15];   //  41 LAC
    unsigned char ucAFTsupportData_MCCMCC_AWreq[15];   // 42 MCC
    unsigned char ucAFTsupportData_MCCMNC_AWreq[15];   // 43 MNC
    unsigned char ucAFTsupportData_PRTXNID_AWreq[20]; // 44 PRTXNID  Timestamps (Substring of RequestID) 030319050252729
    unsigned char ucAFTsupportData_SPLTSEQ_AWreq[2]; // 45 SPLTSEQ  1 - Txn Amount>PerTransactionLimit (Total) | 0: Txn Amount<=PerTransactionLimit (Total)
    unsigned char ucAFTsupportData_CHGAMT_AWreq[2];  // 46 CHGAMT Charges (AEPS - 0)
    unsigned char ucAFTsupportData_CELLIDLatitude_AWreq[15];   // 36 PVTXNID Previous Successful Txn ID ..length
    unsigned char ucAFTsupportData_CELLIDLongitude_AWreq[15];  // 37 PVTXNDT Previous Successful Date 03\/03\/19


    unsigned char ucAFTbeneficiaryRefOrMmid_AWreq[5];
    unsigned char ucAFTbeneficiaryMobile_AWreq[11];
    unsigned char ucAFTremitterMobile_AWreq[11];
    unsigned char ucAFTremitterMmid_AWreq[5];
    unsigned char ucAFTbeneficiaryAccountNo_AWreq[50];
    unsigned char ucAFTbeneficiaryIfsc_AWreq[50];
    unsigned char ucAFTremarks_AWreq[50];

    unsigned char ucaccountNumber_AWreq[50];
    //unsigned char ucamount_AWreq[50];
    unsigned char uccurrency_AWreq[50];
    unsigned char uccreditDebitFlag_AWreq[50];
    unsigned char uctransactionType_AWreq[50];
    unsigned char uctransactionComment_AWreq[50];
    unsigned char uccostCenter_AWreq[50];
    unsigned char ucsupportData_AWreq[50];
    unsigned char ucbeneficiaryRefOrMmid_AWreq[50];
    unsigned char ucbeneficiaryMobile_AWreq[50];
    unsigned char ucremitterMobile_AWreq[50];
    unsigned char ucremitterMmid_AWreq[50];
    unsigned char ucbeneficiaryAccountNo_AWreq[50];
    unsigned char ucbeneficiaryIfsc_AWreq[50];
    unsigned char ucremarks_AWreq[50];

    unsigned char ucAuth_UID_AWreq[50];
    unsigned char ucData_PID_AWreq[50];
    unsigned char ucAuth_Hmac_AWreq[50];
    unsigned char ucAuth_Skey_value_AWreq[50];
    unsigned char ucMeta_lov_AWreq[50];
    unsigned char ucData_type_AWreq[50];
    //unsigned char ucaccountNumber_AWreq[50];
    //unsigned char ucamount_AWreq[50];
    unsigned char uccard_acceptor_ID_AWreq[50];
    unsigned char uccard_acceptor_namelocation_AWreq[50];
    unsigned char ucNBIN_AWreq[50];
    //unsigned char uccreditDebitFlag_AWreq[50];
    unsigned char ucTerminal_Id_AWreq[50];
    unsigned char ucTransactionTime_AWreq[50];
    unsigned char ucTransactionDate_AWreq[50];
    unsigned char ucAppId_AWreq[50];
    unsigned char ucNewAPI_Flag_AWreq[50];
    unsigned char ucAPI_Version_AWreq[50];

    unsigned char ucbankName_AWreq[50];
    unsigned char ucuserAccountNo_AWreq[50];


    unsigned char ucSessionExpiryTime_AWreq[25];
    unsigned char ucX_Auth_Token_AWreq[25];
    unsigned char ucChannelID_AWreq[25];
    unsigned char ucProductCode_AWreq[25];
    unsigned char ucAmount_AWreq[25];
    unsigned char ucMerchantContribution_AWreq[25];
    unsigned char ucCharges_AWreq[25];
    unsigned char ucRouteID_AWreq[25];
    unsigned char ucSplitReferenceNo_AWreq[25];
    unsigned char ucServiceID_AWreq[25];
    unsigned char ucIdentifier_AWreq[25];
    unsigned char ucTxnHash_AWreq[25];

    /*	Response data*/

    unsigned char ucRequestId[25];
    unsigned char ucDisplayMessage[30];
    unsigned char ucResponseMessage[50];
    unsigned char ucResponseCode[30];
    unsigned char ucMessageType[30];
    unsigned char ucreturnCode[3];
    unsigned char ucAuthResponse[30];
    unsigned char ucAORreturnCode[5];//AOR AEPS Offus Response
    unsigned char ucresponseMessage[50];
    unsigned char ucAORtxnReferenceNo[20];//AOR AEPS Offus Response
    unsigned char ucAuthCode[30];
    unsigned char ucTransactionTime[20];
    unsigned char ucTransactionDate[20];
    unsigned char ucAuthUID[20];
    unsigned char ucAORTerminalId[20];
    unsigned char uccardAcceptorCode[20];
    unsigned char ucCardAcceptorNameLocation[50];
    unsigned char ucAORledgerBalance[15];
    unsigned char ucavailableBalance[15];
    unsigned char uccurrency[9];
    unsigned char ucPTRreturnCode[3];//PTR Post Transaction response
    unsigned char ucPTRresponseMessege[50];//PTR Post Transaction response msg added by vaishnavi 15-12-18
    unsigned char ucPTRtxnReferenceNo[20];
    unsigned char uccbsTxnReferenceNo[20];
    unsigned char ucPTRledgerBalance[15]; //PTR Post Transaction response
    unsigned char ucPTRavailableBalance[15];
    unsigned char ucPTRcurrency[5];
    unsigned char ucTerminalId[10];
    unsigned char ucMessageId[10];
    unsigned char ucSessionExpiryTime[15];
    unsigned char ucSessionId[50];
    unsigned char ucRouteID[55];
    unsigned char ucErrorCode[5];
    unsigned char ucXMLData[10];
    unsigned char ucAuthmanFlag[8];
    unsigned char ucServiceID[3];

}AEPS_WITHDRAWAL;


AEPS_WITHDRAWAL AEPSWITH;
#endif // AEPSWTHDRAW_H
