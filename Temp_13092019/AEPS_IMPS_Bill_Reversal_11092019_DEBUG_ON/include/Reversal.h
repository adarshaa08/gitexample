#ifndef REVERSAL_H
#define REVERSAL_H


typedef struct
{
    char product[5+1];
    char trantype[20+1];
    char channel[10];
    char date[20];
    char customerNo[25];
    char accountNo[25];
    char amount[25];
    char branch[30];
    char userId[30];
    char sequenceNo[5+1];
    char referenceNo[20];
    char status[10];
    char reversalFlag[10];
    char narrative[40+1];
    char time[20+1];
    char customerName[30+1];
    char customerMobile[20+1];
    char splitSequence[5+1];
    char debitCredit[5+1];
    char chargeAmount[20+1];
    char reverseCommissionAmount[20+1];
    char reverseTargetAmount[20+1];
    char rfuNumeric1Desc[30+1];
    char rfuNumeric1Val[30+1];
    char rfuNumeric2Desc[30+1];
    char rfuNumeric2Val[30+1];
    char rfuNumeric3Desc[30+1];
    char rfuNumeric3Val[30+1];
    char rfuNumeric4Desc[30+1];
    char rfuNumeric4Val[30+1];
    char rfuText1Desc[30+1];
    char rfuText1Val[30+1];
    char rfuText2Desc[30+1];
    char rfuText2Val[50+1];
    char rfuText3Desc[30+1];
    char rfuText3Val[30+1];
    char rfuText4Desc[30+1];
    char rfuText4Val[30+1];
    char rfuText5Desc[30+1];
    char rfuText5Val[30+1];
    char rfuText6Desc[30+1];
    char rfuText6Val[30+1];
    char rfuText7Desc[30+1];
    char rfuText7Val[30+1];
    char rfuText8Desc[30+1];
    char rfuText8Val[30+1];
    char rfuText9Desc[30+1];
    char rfuText9Val[30+1];
    char rfuText10Desc[30+1];
    char rfuText10Val[30+1];
    char originalSequenceNo[5+1];
    char parentTxnId[20];
    char parentTxnAmount[20];
    char chargecurr[10];
    char parentcurr[10];
    char payoutamt[10];
    char tdsamount[15];
    char rfudateDesc[30];
    char rfudateval[15];


}Reversaldata;

typedef struct
{
    char Revdetail_responsecode[10];
    char Revdetail_response_msg[50];

    char Revotp_responsecode[10];
    char Revotp_response_msg[50];
    char Revotp_displaymsg[10];
    char Revotp_requestid[25];



}Reversal;

typedef struct
{
    char Ref_RequestId_req[25];
    char Ref_MethodId_req[5];
    char Ref_TellerID_req[25];
    char Ref_SessionId_req[50];
    char Ref_IsEncrypt_req[10];
    char Ref_asyncFlag_req[5];
    char Ref_totalAmount_req[20];
    char Ref_txnType_req[20];
    char Ref_impsserviceflag_req[5];
    char Ref_cashoraccount_req[10];
    char Ref_requestbitmap_req[15];
    char Ref_mobileNumber_req[15];
    char Ref_Authid_req[5];
    char Ref_otpMethodId_req[5];
    char Ref_otpRequestId_req[25];
    char Ref_otppin_req[10];
    char Ref_otpMessageId_req[15];
    char Ref_otpISencrypt_req[10];
    char Ref_SessionExpiryTime_req[40];
    char Ref_ABappId_req[20];
    char Ref_ABcustomerNumber_req[25];
    char Ref_ABbeneficiaryType_req[25];
    char Ref_ABbeneficiaryAccount_req[25];
    char Ref_ABbeneficiaryAccountType_req[40];
    char Ref_ABbeneficiaryName_req[40];
    char Ref_ABbeneficiaryBank_req[40];
    char Ref_ABbeneficiaryBankIfsc_req[40];
    char Ref_PTisInclusive_req[5];
    char Ref_PTisClubbed_req[5];
    char Ref_PTchargeOverride_req[5];
    char Ref_amount_req[15];
    char Ref_currency_req[10];
    char Ref_DebitFlag_req[5];
    char Ref_transactionType_req[20];
    char Ref_transactionComment_req[70];
    char Ref_costCenter_req[10];
    char Ref_ChannelID_req[5];
    char Ref_ProductCode_req[20];
    char Ref_RemeName_req[25];
    char Ref_BeneName_req[25];
    char Ref_SplitReferenceNo_req[100];
    char Ref_CreditFlag_req[5];
    char Ref_merchantMobileNumber_req[15];
    char Ref_remarks_req[50];
    char Ref_analysisFlag_req[5];
    char Ref_supportdata2[20];
    char BPbillerId_req[32];
    char BPbillerName_req[58];
    char BPbillerType_req[32];
    char support_ZRFUT5[50];
    char BPauthenticator1_req[25];
    char BPauthenticator2_req[25];
    char BPauthenticator3_req[25];
    char Ref_CustomerName_req[30+1];//07082019

    char ucResponseCode[10];
    char ucResponseMswssage[90];
    char ucDisplayMswssage[40];
    char ucreturncode[5];
    char txnReferenceNo[25];
    char VRreason[25];
    char cbsTxnReferenceNo[25];
    char BAledgerBalance[20];
    char BAavailableBalance[20];
    char BAcurrency[5];
    char Ref_RRN[50];
    char Ref_totalamount[20];
    char RefsupportData_SPLTSEQ[2];
    char RefsupportData_CHGAMT[10];
    char RefsupportData_LKNST[5];
    char RefCustomernumber[15];
    char uccode[2][25];
    char ucamount[2][25];
    char ucdescription[2][25];
    char ucchargeAcctNum[2][25];
    char ucTotal_Amount[2][20];
    char PRFilter3[25];//07082019



}RevRefire;



#endif // REVERSAL_H
