#ifndef AEPSMINI_H
#define AEPSMINI_H
typedef struct
{

    /*Request Data*/
    unsigned char ucRequestId_AMinireq[40];
    unsigned char ucMethodId_AMinireq[20];
    unsigned char ucTellerID_AMinireq[20];
    unsigned char ucSessionId_AMinireq[40];
    unsigned char ucTokenId_AMinireq[20];
    unsigned char ucIsEncrypt_AMinireq[20];
    unsigned char ucAuth_id_AMinireq[20];
    unsigned char ucAadharNo_AMinireq[20];
    unsigned char ucBankName_AMinireq[20];
    unsigned char ucBankBin_AMinireq[20];
    unsigned char ucmerMobileNo_AMinireq[11];//merchant mob no
    unsigned char ucOtp_AMinireq[20];
    unsigned char ucUID_AMinireq[20];
    unsigned char ucAuthType_AMinireq[20];
    unsigned char ucPID_AMinireq[20];
    unsigned char ucHmac_AMinireq[20];
    unsigned char ucSkey_AMinireq[20];
    unsigned char ucPidData_AMinireq[20];
    unsigned char ucRC_AMinireq[20];
    unsigned char ucFp_Auth_AMinireq[20];
    unsigned char ucAUA_Key[50];
    unsigned char ucMeta_udc[50];
    unsigned char ucAuth_UID_AMinireq[20];
    unsigned char ucData_PID_AMinireq[20];
    unsigned char ucAuth_Hmac_AMinireq[20];
    unsigned char ucAuth_Skey_value_AMinireq[20];
    unsigned char ucMeta_lov_AMinireq[20];
    unsigned char ucData_type_AMinireq[20];
    unsigned char ucamount_AMinireq[20];
    unsigned char uccard_acceptor_ID_AMinireq[20];
    unsigned char uccard_acceptor_namelocation_AMinireq[20];
    unsigned char ucTerminal_Id_AMinireq[20];
    unsigned char ucTransactionTime_AMinireq[20];
    unsigned char ucTransactionDate_AMinireq[20];
    unsigned char ucAppId_AMinireq[20];
    unsigned char ucNewAPI_Flag_AMinireq[20];
    unsigned char ucAPI_Version_AMinireq[20];
    unsigned char ucSessionExpiryTime_AMinireq[25];
    unsigned char ucX_Auth_Token_AMinireq[20];
    unsigned char ucChannelID_AMinireq[20];
    unsigned char ucProductCode_AMinireq[20];
    //unsigned char ucAmount_AMinireq[20];
    unsigned char ucMerchantContribution_AMinireq[20];
    unsigned char ucCharges_AMinireq[20];
    unsigned char ucRouteID_AMinireq[50];
    unsigned char ucSplitReferenceNo_AMinireq[20];
    unsigned char ucServiceID_AMinireq[20];
    unsigned char ucIdentifier_AMinireq[20];
    unsigned char ucTxnHash_AMinireq[20];
    char cAepsConsent[150];
    /*Response Data*/

    int 		  iCount;
    unsigned char ucRequestId[50];
    unsigned char ucDisplayMessage[20];
    unsigned char ucResponseMessage[60];
    unsigned char ucResponseCode[20];
    unsigned char ucMessageType[20];
    unsigned char ucreturnCode[20];
    unsigned char uclocalDate[10];
    unsigned char uclocalTime[10];
    unsigned char ucRRN[30];
    unsigned char ucUIDAuthCode[60];
    unsigned char ucnameLocation[50];
    unsigned char ucBalance[10];
    unsigned char ucAuthCode[60];
    unsigned char ucTerminalId[20];
    unsigned char ucMessageId[20];
    unsigned char ucSessionExpiryTime[20];
    unsigned char ucSessionId[50];
    unsigned char ucRouteID[60];
    unsigned char ucErrorCode[20];
    unsigned char ucXMLData[20];
    unsigned char ucAuthmanFlag[20];
    unsigned char ucServiceID[20];


}AEPS_MINISTATEMENT;





typedef struct
{
  unsigned char Date[25];
  unsigned char ModeOfTxn[25];
  unsigned char Type[25];
  unsigned char RefNo[25];
  unsigned char DebitCredit[25];
  unsigned char Amount[11];
}T_AEPS_MINI;

//! \brief Define a template list.
typedef struct list_template
{
  T_AEPS_MINI           AEPS_MINI_STAT;
  struct list_template *next;
}StrAEPS_MINI;





#endif // AEPSMINI_H
