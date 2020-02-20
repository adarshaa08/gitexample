#ifndef IMPSCOMMON_H
#define IMPSCOMMON_H

extern char flag_bitmap;

typedef struct
{
    char ucRequestID_req[25];
    char ucMethodID_req[5];
    char ucTellerID_req[10];
    char ucSessionID_req[50];
    char ucTokenID_req[25];
    char ucIsEncrypt_req[10];
    char ucChannelID_req[25];
    char ucSessionExpiryTime_req[25];
    char ucRouteID_req[25];
    char ucServiceID_req[25];
    char ucIdentifier_req[25];
    char ucTxnHash_req[25];
    char ucRequestID_res[25];

}Common_data;

typedef struct
{
    char ucRequestID_resp[25];
    char ucDisplayMessage_resp[100];
    char ucResponseMessage_resp[150];
    char ucResponseCode_resp[10];
    char ucMessageType_resp[15];
    char ucIMPSSLR_BeneName_resp[50];
    char ucIMPSSLR_BeneAccNo_resp[20+1];
    char ucIMPSSLR_BeneIFSC_resp[15];
    char ucIMPSSLR_BankRRN_resp[15];
    char ucIMPSSLR_ActCode_resp[5+1];
    char ucIMPSSLR_Response_resp[100];
    char ucIMPS_AvilableBalance[30];
    char ucBeneVerif_AvilableBalance[30];

}Common_data_resp;


typedef struct
{
    unsigned char ucCustomerMob[11];
    unsigned char ucCustAccount[20];
    Common_data Commdata_SWK;
    unsigned char ucResponseCode_resp[5];
    unsigned char ucResponseMessage_resp[100];
    unsigned char SWCL_name_resp[30];
}searchWalkCust;




typedef struct
{
    unsigned char uccustid[20];
    Common_data Commdata_EBL;
    char ucbeneficiaryMaxLimit[20];
}FetchExtraBeneList;




typedef struct
{
    unsigned char ucCustMobNo_OGB[11];
    unsigned char ucMessageID_OGB[5];
    unsigned char ucBenename_OGB[50];
    unsigned char ucBeneAccNo_OGB[30];
    unsigned char ucAccountNo_OGB[30];
    Common_data Commdata_OGB;
}OTPGenForBene;





typedef struct
{
    unsigned char ucCustomerMob[11];
    unsigned char uctransgroup[15];
    Common_data Commdata_WCL;
    char ucWCL_RDResponseMessage_resp[150];
    char ucWCL_RDResponseCode_resp[5];
    char ucWCL_RDWCLmaxDailyLimit_resp[15];
    char ucWCL_RDWCLmaxMonthlyLimit_resp[15];
    char ucWCL_RDWCLcurrDayAmount_resp[15];
    char ucWCL_RDWCLcurrMonthAmount_resp[15];
}walkinCust;



typedef struct
{
    unsigned char uccustMobNo_COG[11];
    unsigned char ucMessageID_COG[5];
    unsigned char ucAmount_COG[30];
    Common_data Commdata_COG;
}CashOutOTPGen;

typedef struct
{
    unsigned char ucAccoutNo_CAL[30];
    Common_data Commdata_CAL;
}CustAvailbleLimit;

typedef struct
{
    unsigned char ucCustMobNo_OIN[11];
    unsigned char ucMessageID_OIN[5];
    unsigned char ucBenename_OIN[50];
    unsigned char ucBeneAccNo_OIN[30];
    unsigned char ucAmount_OIN[30];
    unsigned char ucAccountNo_OIN[30];
    unsigned char ucRDMethodID_req[5];
    unsigned char ucCustomerCareNo[15];
    Common_data Commdata_OIN;
}OTPIMPSNEFT;

typedef struct
{
    unsigned char ucCustomerMobileNo_OIN[11];
    unsigned char ucProductCode_OIN[5];
    unsigned char ucOPAccountNo_OIN[50];
    unsigned char ucAmount_OIN[30];
    Common_data Commdata_OIN;
    unsigned char ucRequestID_Resp[15];
    unsigned char ucResponsemsg_Resp[200];
    unsigned char ucResponseCode_Resp[5];
}OTPBENEVERIF;

typedef struct
{
    Common_data_resp Commondata_SearchbyIFSC_RES;
    unsigned char ucifscCode_Resp[12];
    unsigned char ucbankCode_Resp[15];
    unsigned char ucbankDesc_Resp[30];
    unsigned char uccity_Resp[20];
    unsigned char ucbranchDesc_Resp[30];
    unsigned char ucNBIN_Resp[10];
}SEARCHBYIFSC;

typedef struct
{
    Common_data_resp Commondata_SearchbyBRANCH_RES;
    unsigned char ucifscCode_Resp[200][12];
    unsigned char ucbankCode_Resp[200][15];
    unsigned char ucbankDesc_Resp[200][100];
    unsigned char uccity_Resp[200][20];
    unsigned char ucbranchDesc_Resp[200][30];
}SEARCHBYBRANCH;


typedef struct
{
    Common_data_resp Commondata_AddBane_RES;
    unsigned char ucRDResponseMessage_Resp[150];
    unsigned char ucRDResponseCode_Resp[150];

}ADDBENE;

typedef struct
{
    char cCustName[35+1];
    char cFirstName[20];
    char cLastName[15];
    char cDob[7+1];
    char cAge[3+1];
    char cGender[8+1];
    char cMobileNumber[10+1];
    char cAccountNm[19+1];
    char cCustomerNm[19+1];
    char cAddress[30+1];
    char cCity[20+1];
    char cstate[25+1];
    char cStatecode[4+1];
    char cCountrycode[4+1];
    char cPincode[6+1];
    char cGSTINNumber[19+1];
    char cAccountNumber[19+1];
    char cBankName[30+1];
    char cBenificiaryName[20+1];
    char cIFSC_Code[20+1];
    char cRemark[30+1];
    char cBranchstate[15+1];
    char cBranchDistrict[15+1];
    char cBBankName[20+1];
    char cBranchName[50+1];
    char cTxnAmount[10+1];
    char cTxnCahrges[7+1];
    char cTxnType[7+1];
    char cAddBeneWithouTxn[1+1];
    char cTxnTotalAmount[8+1];
    char cOtp[6+1];
    char cSearch_IFSC_Code[10];
    char cDefault_IFSC_Code[12];
    int cChargesAmount;
    char CTotalAmount[10+1];
    char cBankcode[10];
    char cFindCustAccountNo[20];
    char cFindCustAccountType[50];

}sCustDetails;

typedef struct
{

    /*Request data*/

    unsigned char ucAmount_req[10];
    int ucAmount_PTAFT1_req;
    int ucAmount_PTAFT2_req;
    //unsigned char ucAmount_PTAFT1_req[10];
    //unsigned char ucAmount_PTAFT2_req[10];
    unsigned char ucAmount_PTAFT3_req[10];
    unsigned char ucAmount_PTAFT4_req[10];
    unsigned char ucAmount_IMPSSDL_req[10];
    //unsigned char ucAmount_main_req[10];
    int ucAmount_main_req;
    unsigned char ucAmount_charges_req[10];
    unsigned char ucChannelID_req[3];
    unsigned char ucOriginalCharges[10];
    unsigned char ucCharges_req[10];
    unsigned char ucTellerId_req[20];

    unsigned char ucIsEncrypt_req[10];
    unsigned char ucMerchantContribution_req[9];
    int  ucMethodId;
    unsigned char ucProductCode_req[12];
    unsigned char ucRequestId_req[40];
    unsigned char ucSessionExpiryTime_req[20];
    unsigned char ucSessionId_req[25];
    unsigned char ucX_Auth_Token_req[10];
    unsigned char ucTokenId_req[10];
    unsigned char txnType_req[20];
    unsigned char requestbitmap_req[20];
    int  Auth_id_req;
    unsigned char Custmobileno_req[12];
    unsigned char MerchantMobileNo_IMPSSDL_req[12];
    unsigned char cashoraccount_req[15];

    ///////////Post transaction

    unsigned char appId_req[10];
    unsigned char Valuedate_req[3];
    unsigned char analysisFlag_req[3];
    int isInclusive_req;
    int isClubbed_req;
    unsigned char  reversalFlag_req[3];
    unsigned char  Accountno_req[25];
    unsigned char currency_req[10];
    unsigned char first_supportdata_req[1000];
    unsigned char second_supportdata_req[1000];
    unsigned char debit_creditDebitFlag_req[5];
    unsigned char credit_creditDebitFlag_req[5];
    unsigned char first_transactionType_req[15];
    unsigned char second_transactionType_req[15];
    unsigned char transactionComment_req[35];
    unsigned char accfundtransfer_txntype[20];
    unsigned char costCenter_req[10];
    int costCenter_int_req;
    unsigned char beneficiaryRefOrMmid_req[10];
    unsigned char  impsserviceflag_req[3];

    /////////Post transaction

    ///BillDeskDetail
    unsigned char BillerId_req[20];

    unsigned char CAuth_MethodId_req[10];
    unsigned char CAuth_MessageId_req[5];
    unsigned char CAuth_OtpPin_req[15];
    unsigned char merchantMobileNumber_req[15];
    unsigned char ABene_CustomerNumber_req[20];
    unsigned char ABene_beneficiaryType_req[10];
    unsigned char ABene_beneficiaryAccount_req[20];
    unsigned char ABene_beneficiaryAccountType_req[20];
    unsigned char beneName_req[40];
    unsigned char beneBank_req[50];
    unsigned char beneBankBranch_req[25];
    unsigned char beneBankCity_req[25];
    unsigned char beneBankIfsc_req[25];

    unsigned char asyncFlag_req[5];
    unsigned char impsserviceflag[5];
    unsigned char  third_transactionType_req[15];
    unsigned char parentRRN_req[40];    /*Response Data*/

    unsigned char sd_SPROD[5];
    unsigned char sd_PVTXNID[20];
    unsigned char sd_PVTXNDT[15];
    unsigned char sd_IPADDR[20];
    unsigned char sd_DEVICEID[25];
    unsigned char sd_APPID[12];
    unsigned char sd_AUTHID[12];
    unsigned char sd_LOCATION[15];
    unsigned char sd_CELLID[50];
    unsigned char sd_MCC[30+1];
    unsigned char sd_RPTFLG[5];
    unsigned char sd_PRTXNID[30];
    unsigned char sd_PRTXNAMT[20];
    unsigned char sd_SPLTSEQ[5];
    unsigned char sd_CHGAMT_CREDIT[10];
    unsigned char sd_CHGAMT[10];
    unsigned char sd_ZRFUT1[20];
    unsigned char sd_ZRFUT2[15];
    unsigned char sd_ZRFUT3[50];
    unsigned char sd_ZRFUT4[5];
    unsigned char sd_ZRFUT5[30];
    unsigned char sd_ZRFUT6[5];
    unsigned char sd_ZRFUT7[5];
    unsigned char sd_ZRFUT8[5];
    unsigned char sd_ZRFUT9[40];
    unsigned char sd_ZRFUN2[5];
    unsigned char sd_ZRFUN3[25];
    unsigned char sd_ZRFUN4[12];
    unsigned char sd_NETID[12];
    unsigned char sd_MSGTYP[10];
    unsigned char sd_SPROD_Debit[5];

}POSTBENEVER;

typedef struct
{
    Common_data_resp Commondata_POBV_RES;
}POSTBENEVER_RESP;

typedef struct
{
    Common_data_resp Commondata_POBV_RES;
    unsigned char ucRDPTOriginalCharges_RES[10][10];
    unsigned char ucRDPTCharges_RES[10][10];
}POSTBENEVER_CHARGES_RESP;


typedef struct
{
    Common_data_resp Commondata_CalCharges_RES;
}CALCULATECHARGES_RESP;

typedef struct
{
    Common_data_resp Commondata_POBV_RES;
    unsigned char ucRDBeneAccNo_RES[20];
    unsigned char ucRDbenename_RES[35];
    unsigned char ucRDTransactonAmount_RES[20];
    unsigned char ucRDCharges_RES[20];
    unsigned char ucRDCharges_SGST_RES[10];
    unsigned char ucRDCharges_CGST_RES[10];
    unsigned char ucRDBankName_RES[50];
    unsigned char ucRDTransactionType_RES[10];
    unsigned char ucRDTransactionRefNo_RES[15];
    unsigned char ucTOTALAMOUNT_IMPS[40]; // 02052019

}POSTIMPS_RESP;


typedef struct
{
    int Flag_SGST_CGST; // Prod_04092019
    Common_data_resp Commondata_Display_IMPS;
    unsigned char ucDispCharges[10];
    unsigned char ucDispCharges_SGST[10];
    unsigned char ucDispCharges_CGST[10];
    unsigned char ucDispMinusCharges[10]; // Prod_04092019
}DISPLAY_IMPS;


typedef struct
{

    char ucRequestID_req[25];
    char ucMethodID_req[5];
    char ucTellerID_req[10];
    char ucSessionID_req[50];
    char ucTokenID_req[25];
    char ucIsEncrypt_req[10];
    char uccustomerNumber_req[25];
    char ucappId_req[15];
    char ucbeneficiaryType_req[5];
    char ucbeneficiarySequence_req[5];
    char ucchannelId_req[5];

    char ucResponseCode[10];
    char ucResponseMessage[60];
    char ucDisplayMessage[60];

}DELETEBENE;


typedef struct
{
    unsigned char ucSupportData_PVTXNID_req[15];   // 36 PVTXNID Previous Successful Txn ID ..length
    unsigned char ucSupportData_PVTXNDT_req[15];  // 37 PVTXNDT Previous Successful Date 03\/03\/19
    unsigned char ucSupportData_IPADDR_req[15];   // 38 IPADDR len 13
    unsigned char ucSupportData_DEVICEID_req[20]; // 39 DEVICEID Device Serial Number (Mac ID) 980CA5F2BEF1
    unsigned char ucSupportData_APPID_req[20];   // 20 APPID
    unsigned char ucSupportData_MCCCellID_req[5]; // 40 Cell ID
    unsigned char ucSupportData_MCCLAC_req[5];   //  41 LAC
    unsigned char ucSupportData_MCCMCC_req[5];   // 42 MCC
    unsigned char ucSupportData_MCCMNC_req[5];   // 43 MNC
    unsigned char ucSupportData_MCC_req[25];   // 43 MNC
    unsigned char ucSupportData_PRTXNID_req[20]; // 44 PRTXNID  Timestamps (Substring of RequestID) 030319050252729
    unsigned char ucSupportData_SPLTSEQ_req[2]; // 45 SPLTSEQ  1 - Txn Amount>PerTransactionLimit (Total) | 0: Txn Amount<=PerTransactionLimit (Total)
    unsigned char ucSupportData_CHGAMT_req[2];  // 46 CHGAMT Charges (AEPS - 0)
    unsigned char ucSupportData_CELLID_req[40];   // 36 PVTXNID Previous Successful Txn ID ..length
    unsigned char ucSupportData_CELLIDLatitude_req[15];   // 36 PVTXNID Previous Successful Txn ID ..length
    unsigned char ucSupportData_CELLIDLongitude_req[15];  // 37 PVTXNDT Previous Successful Date 03\/03\/19

}SUPPORTDATA_DEVICEINFO;

#endif // IMPSCOMMON_H
