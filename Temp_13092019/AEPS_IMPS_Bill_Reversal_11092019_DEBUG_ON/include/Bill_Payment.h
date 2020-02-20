#ifndef BILL_PAYMENT_H
#define BILL_PAYMENT_H


typedef struct
{
    /*Request data*/
    unsigned char acRequest_ID[20];
    unsigned char acCategory_Req[20+1];
    unsigned char acServiceProvider_Req[20+1];
    unsigned char acMobileNumber_Req[20+1];
    unsigned char acCharge_Req[12+1];
    unsigned char acAmount_Req[12+1];
    unsigned char acTotalAmount_Req[12+1];


 /*Response data*/
    unsigned char acCategory_Rsp[20+1];
    unsigned char acServiceProvider_Rsp[20+1];
    unsigned char acMobileNumber_Rsp[20+1];
    unsigned char acCharge_Rsp[12+1];
    unsigned char acAmount_Rsp[12+1];
    unsigned char acTotalAmount_Rsp[12+1];




}BillDetails;


typedef struct
{

    /*Request data*/


     unsigned char ucRequestId_req[40];
     int  ucMethodId;
     unsigned char ucTellerId_req[20];
     unsigned char ucSessionId_req[50];
     unsigned char ucX_Auth_Token_req[10];
     unsigned char ucIsEncrypt_req[10];
     int ucChannelID_req;
     unsigned char ucSessionExpiryTime_req[20];
     unsigned char ucTokenId_req[10];


    unsigned char Req_asyncFlag_req[3];
    unsigned char Req_amount_req[15];
    unsigned char Req_mobileNumber_req[15];
    unsigned char Req_cashoraccount_req[15];
    unsigned char Req_txnType_req[20];
    unsigned char Req_requestbitmap_req[20];
    unsigned char Req_impsserviceflag_req[3];
    unsigned char Req_merchantMobileNumber_req[12];




    unsigned char AB_name_req[35+1];
    unsigned char AB_mobileno_req[15];
    unsigned char AB_address_req[50+1];

    unsigned char BD_UserID_req[20];
    unsigned char BD_CustomerID_req[20];
    unsigned char BD_CustomerMobNo_req[20];
    unsigned char BD_BillerId_req[20];
    unsigned char BD_BankMessage[75];
    unsigned char BD_PaymentAmount_req[20];    
    unsigned char BD_Authenticator1_req[20];
    unsigned char BD_Authenticator2_req[20];
    unsigned char BD_Authenticator3_req[20];


/////////
   int CA_Auth_id_req;
    unsigned char CA_MobileNo_req[12];

///////////Post transaction

    unsigned char PT_appId_req[10];
    unsigned char PT_Valuedate_req[3];
    int PT_analysisFlag_req;
    int PT_isInclusive_req;
    int PT_isClubbed_req;
   unsigned char  reversalFlag_req[3];

   unsigned char FTL_currency_req[5];
   unsigned char FTL_costCenter_req[10];

   unsigned char FTL_first_supportdata_req[1000];
   unsigned char FTL_first_amount_req[20];
   unsigned char FTL_first_transactionType_req[20];
   unsigned char FTL_first_creditDebitFlag_req[5];
   unsigned char FTL_first_remitterMobile_req[12];


   unsigned char FTL_second_supportdata_req[1000];
   unsigned char FTL_second_amount_req[20];
   unsigned char FTL_second_transactionType_req[20];
   unsigned char FTL_second_creditDebitFlag_req[5];
   unsigned char FTL_second_remitterMobile_req[12];


   unsigned char FTL_third_supportdata_req[1000];
   unsigned char FTL_third_amount_req[20];
   unsigned char FTL_third_transactionType_req[20];
   unsigned char FTL_third_creditDebitFlag_req[5];
   unsigned char FTL_third_remitterMobile_req[12];

   unsigned char FTL_fourth_supportdata_req[1000];
   unsigned char FTL_fourth_amount_req[20];
   unsigned char FTL_fourth_transactionType_req[20];
   unsigned char FTL_fourth_creditDebitFlag_req[5];
   unsigned char FTL_fourth_remitterMobile_req[12];

   unsigned char Billamount[15];

    /*Response Data*/

    char ucResponseCode[10];
    char ucResponseMswssage[90];
    char ucDisplayMswssage[40];
    char ucreturncode[2][5];
    char txnReferenceNo[2][25];
    char VRreason[2][25];
    char cbsTxnReferenceNo[2][25];
    char BAledgerBalance[2][20];
    char BAavailableBalance[2][20];
    char BAcurrency[2][5];
    char PRFilter3[25];

//    char VRMessageCode[2][20];
//    char VRTraceId[2][25];
//    char VRCustomerID[2][20];
//    char VRValid[2][5];
//    char VRBillerId[2][20];
//    char VRAuthenticator1[2][20];
//    char VRPayWithOutBill[2][5];
//    char VRPartialPayment[2][5];
//    char VRChecksum[2][20];
//    char PRTransactionId[2][25];
//    char PRPaymentStatus[2][25];
    char BillDate[20];
    char BillDueDate[20];
    char BillAmount[20];
    char PayWithOutBill[5];
    char PartialPayment[5];
    char uccode[3][25];
    char ucamount[3][25];
    char ucdescription[3][25];
    char ucchargeAcctNum[3][25];
    char Filter2[75];
    char BillNumber[20];


}BILLPAYMENT;


typedef struct
{
unsigned char ucCategory[50];
unsigned char ucServiceprovider[50];
unsigned char ucBillercode[40];
char ucTotal_Amount[20];
char ucCharges[10];

}BILLERDATA;


#endif // BILL_PAYMENT_H
