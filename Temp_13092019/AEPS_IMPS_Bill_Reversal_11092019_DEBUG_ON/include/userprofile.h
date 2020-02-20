//#ifndef USERPROFILE_H
//#define USERPROFILE_H

//typedef struct
//{
//    /*Request data*/
//    unsigned char ucRequestID_req[25];
//    unsigned char ucMethodID_req[5];
//    unsigned char ucTellerID_req[10];
//    unsigned char ucSessionID_req[50];
//    unsigned char ucTokenID_req[25];
//    unsigned char ucIsEncrypt_req[25];
//    unsigned char ucUser_id_req[25];
//    unsigned char ucClient_id_req[25];
//    unsigned char ucSessionExpiryTime_req[25];
//    unsigned char ucX_Auth_Token_req[25];


//    int 		  iCount;
//    unsigned char ucRequestId[50];
//    unsigned char ucDisplayMessage[20];
//    unsigned char ucResponseMessage[60];
//    unsigned char ucResponseCode[20];
//    unsigned char ucMessageType[20];
//    unsigned char ucreturnCode[20];
//    unsigned char ucMessageId[20];
//    unsigned char ucSessionExpiryTime[20];
//    unsigned char ucSessionId[50];
//    unsigned char ucRouteID[60];
//    unsigned char ucErrorCode[20];
//    unsigned char ucXMLData[20];
//    unsigned char ucAuthmanFlag[20];
//    unsigned char ucServiceID[20];


//}FOR_PASS;





//typedef struct
//{
//  unsigned char questionGroup[10];
//  unsigned char questionId[10];
//  unsigned char questionDesc[100];
//  unsigned char answer[25];

//}T_FOR_PASS;

////! \brief Define a template list.
//typedef struct list_template
//{
//  T_FOR_PASS           FOR_PASS_STAT;
//  struct list_template *next;
//}StrFPR_PASS;


//typedef struct
//{
//    /*Request data*/
//    unsigned char ucRequestID_req[25];
//    unsigned char ucMethodID_req[5];
//    unsigned char ucTellerID_req[10];
//    unsigned char ucSessionID_req[50];
//    unsigned char ucTokenID_req[25];
//    unsigned char ucIsEncrypt_req[25];
//    unsigned char ucUser_id_req[25];
//    unsigned char ucChannelID_req[5];
//    unsigned char ucMessageId_req[25];
//    unsigned char ucCustomerMobileNo_req[25];
//    unsigned char ucProductCode_req[20];
//    unsigned char ucAmount_req[20];
//    unsigned char ucSessionExpiryTime_req[25];
//    unsigned char ucX_Auth_Token_req[25];
//    unsigned char ucRDFMethodID_req[5];

//    int 		  iCount;
//    unsigned char ucRequestId[50];
//    unsigned char ucDisplayMessage[20];
//    unsigned char ucResponseMessage[60];
//    unsigned char ucResponseCode[20];
//    unsigned char ucMessageType[20];
//    unsigned char ucreturnCode[20];
//    unsigned char ucMessageId[20];
//    unsigned char ucSessionExpiryTime[20];
//    unsigned char ucSessionId[50];
//    unsigned char ucRouteID[60];
//    unsigned char ucErrorCode[20];
//    unsigned char ucXMLData[20];
//    unsigned char ucAuthmanFlag[20];
//    unsigned char ucServiceID[20];
//    unsigned char ucQstID[20];
//    unsigned char ucQstgroup[20];
//    unsigned char ucanswer[25];
//    unsigned char ucRDRequestId[25];
//    unsigned char ucnewpassword[70];
//    unsigned char ucencrypkey[60];
//    unsigned char ucoldpassword[70];


//}FOR_OTP;


//typedef struct
//{

//    /*Request data*/
//    unsigned char ucRequestId_NwPassreq[25];
//    unsigned char ucMethodId_NwPassreq[5];
//    unsigned char ucTellerID_NwPassreq[20];
//    unsigned char ucSessionId_NwPassreq[40];
//    unsigned char ucTokenId_NwPassreq[20];
//    unsigned char ucChannelID_NwPassreq[20];
//    unsigned char ucUserid_NwPassreq[50];
//    unsigned char ucIsEncrypt_NwPassreq[10];
//    unsigned char ucServiceID_NwPassreq[20];
//    unsigned char ucQstgroup_NwPassreq[11];
//    unsigned char ucMessageID_NwPassreq[11];
//    unsigned char ucSessionExpiryTime_NwPassreq[30];
//    unsigned char ucQstID_NwPassreq[40];
//    unsigned char ucAnswer_NwPassreq[20];
//    unsigned char ucMobileNo_NwPassreq[20];
//    unsigned char ucAuth_id_NwPassreq[20];
//    unsigned char ucRDMethodId_NwPassreq[5];
//    unsigned char ucOTPPIN_NwPassreq[20];
//    unsigned char ucNewPass_NwPassreq[70];
//    unsigned char ucOTPRequestId_NwPassreq[10];

//}NewPass;


//typedef struct
//{
//unsigned char ucRequestId_CPreq[30];
//unsigned char ucMethodId_CPreq[5];
//unsigned char ucTellerID_CPreq[15];
//unsigned char ucSessionId_CPreq[40];
//unsigned char ucTokenId_CPreq[20];
//unsigned char ucIsEncrypt_CPreq[9];
//unsigned char uccustomerNo_CPreq[15];
//unsigned char ucauthid_CPreq[5];
//unsigned char ucRDMethodID_CPreq[40];
//unsigned char ucRDRequestID_CPreq[20];
//unsigned char ucRDOTPPIN_CPreq[20];
//unsigned char ucMessageID_CPreq[20];
//unsigned char ucSessionExpiryTime_CPreq[25];
//unsigned char ucUserID_CPreq[50];
//unsigned char ucOldPassword_CPreq[70];
//unsigned char ucNewPassword_CPreq[70];
//}ChangePass;






//typedef struct
//{
//unsigned char ucRequestId_Lreq[30];
//unsigned char ucMethodId_Lreq[5];
//unsigned char ucTellerID_Lreq[15];
//unsigned char ucSessionId_Lreq[40];
//unsigned char ucTokenId_Lreq[20];
//unsigned char ucChannelID_Lreq[5];
//unsigned char ucIsEncrypt_Lreq[9];
//unsigned char ucclientid_Lreq[15];
//unsigned char ucSessionExpiryTime_Lreq[25];


//char cLResponse_code[10];
//char cLResponseMessage[60];

//}Logout;



//typedef struct
//{
//unsigned char ucRequestId_Rreq[30];

////unsigned char cRRequestId[20];
////unsigned char cRMessageType[20];
////unsigned char cRDisplayMessage[50];
////unsigned char cRResponse_code[10];
////unsigned char cRResponseMessage[60];
////unsigned char uRreturnCode[10];
////unsigned char uRresponseMessage[50];
// char uRglAccountNo[20];
// char uRbalance[20];
// char uRMessageId[10];
// char uRSessionExpiryTime[20];
// char uRSessionId[20];
// char uRRouteID[30];
//char uRErrorCode[10];
//char uRXMLData[20];
// char uRAuthmanFlag[10];
// char uRServiceID[10];
// char cRRequestId[20];
// char cRMessageType[20];
// char cRDisplayMessage[50];
// char cRResponse_code[10];
// char cRResponseMessage[60];
// char uRreturnCode[10];
// char uRresponseMessage[50];

//}Refresh;
//#endif // USERPROFILE_H



#ifndef USERPROFILE_H
#define USERPROFILE_H

typedef struct
{
    /*Request data*/
    unsigned char ucRequestID_req[25];
    unsigned char ucMethodID_req[5];
    unsigned char ucTellerID_req[10];
    unsigned char ucSessionID_req[50];
    unsigned char ucTokenID_req[25];
    unsigned char ucIsEncrypt_req[25];
    unsigned char ucUser_id_req[25];
    unsigned char ucClient_id_req[25];
    unsigned char ucSessionExpiryTime_req[25];
    unsigned char ucX_Auth_Token_req[25];


    int 		  iCount;
    unsigned char ucRequestId[50];
    unsigned char ucDisplayMessage[20];
    unsigned char ucResponseMessage[60];
    unsigned char ucResponseCode[20];
    unsigned char ucMessageType[20];
    unsigned char ucreturnCode[20];
    unsigned char ucMessageId[20];
    unsigned char ucSessionExpiryTime[20];
    unsigned char ucSessionId[50];
    unsigned char ucRouteID[60];
    unsigned char ucErrorCode[20];
    unsigned char ucXMLData[20];
    unsigned char ucAuthmanFlag[20];
    unsigned char ucServiceID[20];


}FOR_PASS;





typedef struct
{
  unsigned char questionGroup[10];
  unsigned char questionId[10];
  unsigned char questionDesc[100];
  unsigned char answer[50];

}T_FOR_PASS;

//! \brief Define a template list.
//typedef struct list_template
//{
//  T_FOR_PASS           FOR_PASS_STAT;
//  struct list_template *next;
//}StrFPR_PASS;


typedef struct
{
    /*Request data*/
    unsigned char ucRequestID_req[25];
    unsigned char ucMethodID_req[5];
    unsigned char ucTellerID_req[10];
    unsigned char ucSessionID_req[50];
    unsigned char ucTokenID_req[25];
    unsigned char ucIsEncrypt_req[25];
    unsigned char ucUser_id_req[25];
    unsigned char ucChannelID_req[5];
    unsigned char ucMessageId_req[25];
    unsigned char ucCustomerMobileNo_req[25];
    unsigned char ucProductCode_req[20];
    unsigned char ucAmount_req[20];
    unsigned char ucSessionExpiryTime_req[25];
    unsigned char ucX_Auth_Token_req[25];
    unsigned char ucRDFMethodID_req[5];

    int 		  iCount;
    unsigned char ucRequestId[50];
    unsigned char ucDisplayMessage[20];
    unsigned char ucResponseMessage[60];
    unsigned char ucResponseCode[20];
    unsigned char ucMessageType[20];
    unsigned char ucreturnCode[20];
    unsigned char ucMessageId[20];
    unsigned char ucSessionExpiryTime[20];
    unsigned char ucSessionId[50];
    unsigned char ucRouteID[60];
    unsigned char ucErrorCode[20];
    unsigned char ucXMLData[20];
    unsigned char ucAuthmanFlag[20];
    unsigned char ucServiceID[20];
    unsigned char ucQstID[20];
    unsigned char ucQstgroup[20];
    unsigned char ucanswer[50];
    unsigned char ucRDRequestId[25];
    unsigned char ucnewpassword[70];
    unsigned char ucencrypkey[60];
    unsigned char ucoldpassword[70];


}FOR_OTP;


typedef struct
{

    /*Request data*/
    unsigned char ucRequestId_NwPassreq[25];
    unsigned char ucMethodId_NwPassreq[5];
    unsigned char ucTellerID_NwPassreq[20];
    unsigned char ucSessionId_NwPassreq[40];
    unsigned char ucTokenId_NwPassreq[20];
    unsigned char ucChannelID_NwPassreq[20];
    unsigned char ucUserid_NwPassreq[50];
    unsigned char ucIsEncrypt_NwPassreq[10];
    unsigned char ucServiceID_NwPassreq[20];
    unsigned char ucQstgroup_NwPassreq[11];
    unsigned char ucMessageID_NwPassreq[11];
    unsigned char ucSessionExpiryTime_NwPassreq[30];
    unsigned char ucQstID_NwPassreq[40];
    unsigned char ucAnswer_NwPassreq[50];
    unsigned char ucMobileNo_NwPassreq[20];
    unsigned char ucAuth_id_NwPassreq[20];
    unsigned char ucRDMethodId_NwPassreq[5];
    unsigned char ucOTPPIN_NwPassreq[20];
    unsigned char ucNewPass_NwPassreq[70];
    unsigned char ucOTPRequestId_NwPassreq[10];

}NewPass;


typedef struct
{
unsigned char ucRequestId_CPreq[30];
unsigned char ucMethodId_CPreq[5];
unsigned char ucTellerID_CPreq[15];
unsigned char ucSessionId_CPreq[40];
unsigned char ucTokenId_CPreq[20];
unsigned char ucIsEncrypt_CPreq[9];
unsigned char uccustomerNo_CPreq[15];
unsigned char ucauthid_CPreq[5];
unsigned char ucRDMethodID_CPreq[40];
unsigned char ucRDRequestID_CPreq[20];
unsigned char ucRDOTPPIN_CPreq[20];
unsigned char ucMessageID_CPreq[20];
unsigned char ucSessionExpiryTime_CPreq[25];
unsigned char ucUserID_CPreq[50];
unsigned char ucOldPassword_CPreq[70];
unsigned char ucNewPassword_CPreq[70];
}ChangePass;






typedef struct
{
unsigned char ucRequestId_Lreq[30];
unsigned char ucMethodId_Lreq[5];
unsigned char ucTellerID_Lreq[15];
unsigned char ucSessionId_Lreq[40];
unsigned char ucTokenId_Lreq[20];
unsigned char ucChannelID_Lreq[5];
unsigned char ucIsEncrypt_Lreq[9];
unsigned char ucclientid_Lreq[15];
unsigned char ucSessionExpiryTime_Lreq[25];


char cLResponse_code[10];
char cLResponseMessage[60];

}Logout;



typedef struct
{
unsigned char ucRequestId_Rreq[30];

//unsigned char cRRequestId[20];
//unsigned char cRMessageType[20];
//unsigned char cRDisplayMessage[50];
//unsigned char cRResponse_code[10];
//unsigned char cRResponseMessage[60];
//unsigned char uRreturnCode[10];
//unsigned char uRresponseMessage[50];
 char uRglAccountNo[20];
 char uRbalance[20];
 char uRMessageId[10];
 char uRSessionExpiryTime[20];
 char uRSessionId[20];
 char uRRouteID[30];
char uRErrorCode[10];
char uRXMLData[20];
 char uRAuthmanFlag[10];
 char uRServiceID[10];
 char cRRequestId[20];
 char cRMessageType[20];
 char cRDisplayMessage[50];
 char cRResponse_code[10];
 char cRResponseMessage[60];
 char uRreturnCode[10];
 char uRresponseMessage[50];

}Refresh;


typedef struct
{

    char RespocountNo[12+1];
    char product[7+1];
    char transactionType[15+1];
    char channel[8+1];
    char transactionDate[15+1];
    char customerNo[15+1];
    char accountNo[20+1];
    char transactionAmount[10+1];
    char branchCode[5+1];
    char userId[12+1];
    char sequenceNumber[6+1];
    char referenceId[15+1];
    char status[20+1];
    char reversalFlag[5+1];
    char transactionDesc[50+1];
    char time[15+1];
    char customerName[30+1];
    char customerMobile[10+1];
    char splitSequence[2+1];
    char debitCredit[4+1];
    char chargeAmount[10+1];
    char payoutAmount[10+1];
    char commissionAmount[8+1];
    char tdsAmount[10+1];
    char originalTxnSequence[5+1];
    char parentTxnId[15+1];
    char parentTxnAmount[10+1];
    char parentTxnAmountCurrent[10+1];
    char chargeAmountCurrent[10+1];
    char transactionTypeName[150+1];
    char TransactionTypeProduct[16+1];
    char TransactionTypeDescription[50+1];
    char Status[30+1];
    char StatusDesc[50+1];
    char rfuDateDesc[50+1];
    char rfuDateVal[40];
    char rfuNumeric1Desc[40];
    char rfuNumeric1Val[40];
    char rfuNumeric2Desc[40];
    char rfuNumeric2Val[40];
    char rfuNumeric3Desc[40];
    char rfuNumeric3Val[40];
    char rfuNumeric4Desc[40];
    char rfuNumeric4Val[40];
    char rfuText1Desc[40];
    char rfuText1Val[40];
    char rfuText2Desc[40];
    char rfuText2Val[40];
    char rfuText3Desc[40];
    char rfuText3Val[40];
    char rfuText4Desc[40];
    char rfuText4Val[40];
    char rfuText5Desc[40];
    char rfuText5Val[40];
    char rfuText6Desc[40];
    char rfuText6Val[40];
    char rfuText7Desc[40];
    char rfuText7Val[40];
    char rfuText8Desc[40];
    char rfuText8Val[40];
    char rfuText9Desc[40];
    char rfuText9Val[40];
    char rfuText10Desc[40];
    char rfuText10Val[40];
    char debitAmount[10+1];
    char creditAmount[10+1];
    char cGST[20];
    char sGST[20];
    char iGST[20];
    char uGST[20];
    char subType[20];
    char RFUMapping[200];
    char endingbalance[20];
    int iTxnSCount;
    int iPratentCount;
}TxnStatement;

typedef struct
{
    char ucRequestID_req[25];
    char ucRequestId[25];
    char ucDisplayMessage[25];
    char ucResponseMessage[60];
    char ucResponseCode[4];
    char ucMessageType[10];
    int icount;

}Merchant_detail;




#endif // USERPROFILE_H
