#include <stdio.h>
#ifndef ISO8583_H
#define ISO8583_H 

#define ISO_BITMAP       0
#define ISO_NUMERIC      1
#define ISO_ALPHANUMERIC 2
#define ISO_BINARY       3
#define DE48BITMAPINDEX 129
#define DE127BITMAPINDEX 146 

#include <QString>

typedef struct {
    int format;
    int lenflds;
    int flds;
    const char *dsc;
} isodef;

typedef struct {
    char *fld[224];
    int fldlen[224];
} isomsg;

#define LEN_ERROR 2



typedef struct {
        long int Recpt;
        int AcqId;
        int IssId;

        bool Venable;
        char Mtype[8];
        char Pan[32];
        char UID[16];
        char ProsCode[16];
        char TxnAmt[16];
        char StanNo[8];
        char TxnTime[16];
        char TxnDate[16];
        char ExpDate[16];
        char DateCapture[8];
        char PosEntryMode[8];
        char MCC[8];
        char funcode[8];
        char NII[8];
        char PosCondCode[8];

        char Track2Data[64];
        char RRN[16];
        char BIN[8];
        char Agent_id[16];
        char AuthIdResp[1024];
        char RespCode[8];
        char TermId[32];
        char CardAccpIDCode[32];
        char CardAccpName[128];
        char skey[64];
        char hmac[512];
        char ac[16];
        char ci[16];
        char sa[16];
        char lk[1024];
        char field63[1024];
        char field126[1024];
        char field127[1024];
        char MerId[16];
        char PinBlk[64];
        char AdditAmt[12];
        char data[1024];
        char Name[64];
        char conCode[32];
        char settlmt[128];
        char *biometric;
        char currcode[8];
        char ACId[32];

        char TxnDesc[32];
        char Modified121[128];   //Added By Praveen

        int dataLen;
        int AcqTableId;
        int IssuerTableId;
        char transType[20];
        char BitMap[32];
        char TxnDateTime[32];
        char field60[1024];
        char field62[1024];

        int field62_len;
        int field63_len;
        //int NII;

}ISO;


typedef struct
{
    char cSTAN[8];
    char cTxnDate[16];
    char cTxnTime[16];
    char cRRN[16];
    char cResp[8];
    char cTermId[16];
    char cAvailBal[32];
    char cTxnCode[16];
    char cCustName[64];
    char cAccBal[32];
    char cAccNo[32];
    char cInvNo[14];
    char cDate[16];
    char cTime[16];
}RespParams;

typedef struct _CUST_TRNS_INFO
{
    char	uszAccountType[2 +1];
    char    uszAccountCode[19 +1];
    char	uchTransactionType[1+1];
    char	uszTransactionDateTime[14 +1];
    char	uszTransactionAmount[17 +3];
    char	uszDescription[50 +1];
    char	ProductCode[13+1];

} CUST_TRANS_INFO;

typedef struct _Report
{
    char TID[15];
    char CreditCnt[7];
    char CreditAmt[12];
    char DebitCnt[7];
    char DebitAmt[12];
    char Enrollment_Date[20];
    char EnrollmentCnt[7];
    char FIGCnt[7];
    char Total_Txn[7];
}Report;

//------------------------Common Structure-------------------------------

typedef struct
{
    QString response_URN;
    QString auth_ID_Resp;
    QString response_processingCode;
    QString response_STAN;
    QString response_TxnDate;
    QString response_TxnTime;
    QString response_NIIVal;
    QString response_RRN;
    QString transaction_Resp;
    QString response_TermId;
    QString response_reqAmt;
    QString response_TxnCode;
    QString response_TranStatus;
    QString response_CustName;
    QString response_AccBal;
    QString response_BeneAccNO;
    QString response_AccNo;
    QString response_invoiceNumber;
    QString response_Date;
    QString response_Time;
    QString response_primary_BitMap;
    QString fromAccountNumber;
    QString toAccountNumber;
    QString miniStat_Details;
    QString response_LogOnKey;
    QString reponse_batchNo;
    QString response_POSID;
    QString response_AGENTID;
    QString Terminal_Location;
    QString CuurencyCode;
    QString TERMINAL_ID;
    QString AgentID;
    QString Track2Data;
    QString AccNo;
    QString Card_Acquirer_ID;
    QString response_unique_auth_code;
    QString BFD_DATA;
    QString FIGRRN;

}merged_transaction_response_structure;

typedef struct
{
    QString LEFT_RING_VALUE;
    QString RIGHT_INDEX_VALUE;
    QString LEFT_THUMB_VALUE;
    QString LEFT_LITTLE_VALUE;
    QString RIGHT_LITTLE_VALUE;
    QString RIGHT_MIDDLE_VALUE;
    QString LEFT_MIDDLE_VALUE;
    QString LEFT_INDEX_VALUE;
    QString RIGHT_RING_VALUE;
    QString RIGHT_THUMB_VALUE;

 }BFD_TAGS_DATA;

typedef struct
{
    QString URN;
    QString PROC_CODE;
    QString TRAN_AMT;
    QString SETTLEMENT_AMT;
    QString CARDHOLDER_BILLING;
    QString TRAN_DATETIME;
    QString CARDHOLDER_BILLING_FEE;
    QString SETTELMENT_CONV_RATE;
    QString BILLING_CONV_RATE;
    QString STAN;
    QString LOCAL_TIME;
    QString LOCAL_DATE;
    QString EXPIRY_DATE;
    QString SETTLEMENT_DATE;
    QString CONVERSION_DATE;
    QString CAPTURE_DATE;
    QString MERCHANT_TYPE;
    QString COUNTRY_CODE;
    QString PAN_EXT_COUNTRY_CODE;
    QString FORW_COUNTRY_CODE;
    QString POS_ENTRY_MODE;
    QString CARD_SEQ_NUMBER;
    QString NII;
    QString POS_CONDITION_CODE;
    QString POS_CAPTURE_CODE;
    QString AUTH_RES_LEN;
    QString TRAN_FEE_AMT;
    QString SETTELMENT_FEE;
    QString TRANSACTION_PROC_FEE;
    QString SETTELMENT_PROC_FEE;
    QString ACQUIRING_ID_CODE;
    QString ISSUER_ID_CODE;
    //QString EXTENDED_PAN;
    QString TRACK_2_DATA;
    QString TRACK_3_DATA;
    QString RRN;
    QString APPROVAL_CODE;
    QString ACTION_CODE;
    QString SERVICE_REST_CODE;
    QString CARD_ACCEPTOR_TID;
    QString CARD_ACCEPTOR_CODE;
    QString CARD_ACCEPTOR_NAME_LOCATION;
    QString ADD_RES_DATA;
    QString TRACK_1_DATA;
    QString ADDITIONAL_DATA_ISO;
    QString ADDITIONAL_DATA_NATIONAL;
    QString ADDITIONAL_DATA;
    QString TRAN_CURRENCY_CODE;
    QString SETTEL_CURRENCY_CODE;
    QString BILLING_CURRENCY_CODE;
    QString PIN_DATA;
    QString SECURITY_CONTROL_INFORMATION;
    QString ADDITIONAL_AMOUNTS;
    QString REV_55;
    QString REV_56;
    QString NATIONAL_57;
    QString FINANCIAL_TOKEN;
    QString CAF_UPDATE_TOKEN;
    QString TERMINAL_FHM_DATA;
    QString CARD_ISSUER_DATA;
    QString HOST_CAF_EXPONENT;
    QString POS_TOKENS_ATM_PIN_OFFSET;
    QString PRIMARY_MAC;
    QString EXTENDED_BITMAP;
    QString SETTELMENT_CODE;
    QString EXTENDED_PAN;
    QString RECV_INST_COUNTRY_CODE;
    QString SETTL_INST_COUNTRY_CODE;
    QString NMI_CODE;
    QString MESSAGE_NO;
    QString MESSAGE_NO_LAST;
    QString ACTION_DATE_YYMMDD;
    QString CR_NO;
    QString CR_REVERSAL_NO;
    QString DR_NO;
    QString DR_REVERSAL_NO;
    QString TRANSFER_NO;
    QString TRANFER_REVERSAL_NO;
    QString INQUIRY_NO;
    QString AUTH_NO;
    QString PROCESS_FEE_AMT_CR;
    QString TXN_FEE_AMT_CR;
    QString PROCESS_FEE_AMT_DR;
    QString TXN_FEE_AMT_DR;
    QString AMOUNT_CR;
    QString REVERSAL_AMT_CR;
    QString AMOUNT_DR;
    QString REVERSAL_AMT_DR;
    QString ORIGINAL_DATA_ELEMENTS;
    QString FHM_FILE_UPDATE_CODE;
    QString FILE_SECURITY_CODE;
    QString RESPONSE_INDICATOR;
    QString SERVICE_INDICATOR;
    QString REPLACEMENT_AMOUNTS;
    QString MSG_SECURITY_CODE;
    QString NET_SETTLEMENT_AMT;
    QString PAYEE;
    QString SETTL_INST_CODE;
    QString RECEIVING_INSTITUTION_ID;
    QString FHM_FILE_NAME;
    QString FHM_ACCOUNT_ID1;
    QString FHM_ACCOUNT_ID2;
    QString TRAN_DesRIPTION;
    QString REV_105;
    QString REV_106;
    QString REV_107;
    QString REV_108;
    QString REV_109;
    QString REV_110;
    QString REV_111;
    QString FHM_PREAUTH_HOLD_INFO;
    QString FHM_HOT_CARD_INFO;
    QString FHM_PBF_CUST_SEGMENT_INFO;
    QString FHM_PBF_USER_INFO;
    QString FHM_CAF_NONCURRENCY_DISPENSE;
    QString FHM_CAF_EMV;
    QString FHM_CAF_PBF_DATA;
    QString FHM_BANKING_CHECK_INFO;
    QString FHM_PBF_EXPANDED_SEGMENT;
    QString FHM_CAF_EXPANDED_ATM_SEGMENT;
    QString FHM_CAF_EXPANDED_POS_SEGMENT;
    QString POS_INVOICE_ATM_DEPOSIT_AMT;
    QString FHM_PBF_PREAUTH_SEGMENT;
    QString POS_SETTLEMENT_DATA;
    QString FHM_CAF_EXP_ACCOUNT;
    QString POS_USER_DATA;
    QString SECONDARY_MAC;


    QString TERMINAL_ID;
    QString field60;   // Added by vaibhav on 24th Dec 2015
    QString HwSerialNo;  // akshay j added on 16 DEc 15
    QString CustomerName; // need to check later added on 21 DEC 15
    QString Balance;      // need to check later added on 21 DEC 15
    char FI_TRAN_TYPE_FLAG;   // akshay j added 22 DEC 15
    int TRAN_TYPE_FLAG;  // akshay j added 22 12 15
    int TRAN_SUBTYPE_FLAG;
    int TRAN_SUBTYPE_FLAG_SHG; // Dhiral
    int SHG_TXN_Counter;
    char FI_MWALLET_FLAG; // Dhiral_BC_MWallet
    char BC_Login_FLAG;   // Dhiral BC Visit Traking

    QString EKYC_TYPE; // Ekyc VID

    QString Header; // Added by vaibhav on 25th DEC 2015

    QString Field09;
    QString Field08;
    QString Field40;
    QString Field102;
    QString Field103;
    QString Field104;  // Dhiral for SHG
    QString Field108;
    QString Field109;
    QString Field110;
    QString Field112;
    QString Field113;
    QString Field114;
    QString Field115;
    QString Field116;
    QString Field117;
    QString Field121;   // addded by akshay on 26 DEC 15
    QString Field123;
    QString Field125;
    QString Field126;
    QString Field127;
    QString Field24;
    QString Field32;
    QString Field63;    // added by akshay on 27 DEC 15
    QString Field_18;
    QString Field_62;
    QString Field_06;
    QString Field_48;
    QString Field61;
    QString Field10;

}merged_transaction_structure; // akshay J changes 23 NOV 15
//-----------------------------------------------------------------------

//typedef struct _EKYC
//{
//    unsigned char ucUID[12+1];
//    unsigned char ucCustomerName[50];
//    unsigned char ucDOB[11+1];
//    unsigned char ucGender[6+1];
//    unsigned char ucPhoneNum[10+1];
//    unsigned char ucCO[50];
//    unsigned char ucDistrict[20];
//    unsigned char ucHouseNum[100];
//    unsigned char ucPinCode[6+2];
//    unsigned char ucPostOfice[50];
//    unsigned char ucState[50];
//    unsigned char ucRRN[20];
//    unsigned char ucResCode[5];
//    unsigned char ucVTC[50];
//    //unsigned char *ucImage;
//    unsigned char ucImage[25000];
//    int           uiImageSize;
//    unsigned char ucemailID[50];
//    unsigned char NomineeName[50];
//    unsigned char NomineeReltn[50];
//    unsigned char Minor[2+1];
//    unsigned char RelationCode[2+1];
//    unsigned char GaurdianCode[2+1];
//    unsigned char ucNomineeDOB[11+1];
//    unsigned char JobCardId[20+1];
//    unsigned char PensionID[15+1];
//    unsigned char CIF[15+1];
//    unsigned char AccountNumber[20+1];
//    unsigned char NomAddress[40+1];
//    unsigned char AuthCode[50];
//    unsigned char EmailPhoneFlag;

//}EKYC;
typedef struct _EKYC
{
        QString ucUID;
        QString ucCustomerName;
        QString ucDOB;
        QString ucGender;
        QString ucPhoneNum;
        QString ucCO;
        QString ucDistrict;
        QString lamdmark;
        QString ucHouseNum;
        QString ucPinCode;
        QString ucPostOfice;
        QString ucState;
        QString ucRRN;
        QString ucResCode;
        QString ucVTC;
        QString ucImage;
        int uiImageSize;
        QString ucemailID;
        QString NomineeName;
        QString NomineeReltn;
        QString Minor;
        QString RelationCode;
        QString GaurdianCode;
        QString ucNomineeDOB;
        QString JobCardId;
        QString PensionID;
        QString CIF;
        QString AccountNumber;
        QString NomAddress;
        QString AuthCode;
        QString EmailPhoneFlag;

        QString MemberDetail;
        QString AddressDet;

}EKYC;

typedef struct
{
    QString SHG_ACC_No;
    QString SHG_UID1;  // Dhiral for SHG
    QString SHG_UID2;  // Dhiral for SHG
    QString SHG_UID3;  // Dhiral for SHG
    QString SHG_UID1_CustomerNameFirst;  // Dhiral for SHG
    QString SHG_UID1_CustomerNameMiddle;  // Dhiral for SHG
    QString SHG_UID1_CustomerNameLast;  // Dhiral for SHG
    QString SHG_UID2_CustomerNameFirst;  // Dhiral for SHG
    QString SHG_UID2_CustomerNameMiddle;  // Dhiral for SHG
    QString SHG_UID2_CustomerNameLast;  // Dhiral for SHG
    QString SHG_UID3_CustomerNameFirst;  // Dhiral for SHG
    QString SHG_UID3_CustomerNameMiddle;  // Dhiral for SHG
    QString SHG_UID3_CustomerNameLast;  // Dhiral for SHG
    QString SHG_UID1_CIF;  // Dhiral for SHG
    QString SHG_UID2_CIF;  // Dhiral for SHG
    QString SHG_UID3_CIF;  // Dhiral for SHG
    QString SHG_RRN1;
    QString SHG_RRN2;
    QString SHG_AUTH_ID_LEG2;


}SHG;


typedef struct      // Dhiral
{
    QString ig_SHG_UID1; // Dhiral
    QString ig_SHG_UID1_FRNAME; // Dhiral
    QString ig_SHG_UID1_SENAME; // Dhiral
    QString ig_SHG_UID1_THNAME; // Dhiral
    QString ig_SHG_UID1_CIF; // Dhiral
    QString ig_SHG_UID2; // Dhiral
    QString ig_SHG_UID2_FRNAME; // Dhiral
    QString ig_SHG_UID2_SENAME; // Dhiral
    QString ig_SHG_UID2_THNAME; // Dhiral
    QString ig_SHG_UID2_CIF; // Dhiral
    QString ig_SHG_UID3; // Dhiral
    QString ig_SHG_UID3_FRNAME; // Dhiral
    QString ig_SHG_UID3_SENAME; // Dhiral
    QString ig_SHG_UID3_THNAME; // Dhiral
    QString ig_SHG_UID3_CIF; // Dhiral
    int FLAG_UID3_AVL;
    int UID_Selection_Status_Flag;

}ig_SHG;

typedef struct
{
 QString ResidentUID;
 QString Mobile_No;
 QString Email_ID;
 QString OTP;
 QString ResidentTimeStamp;
 QString ResidentSKey;
 QString ResidentPID;
 QString ResidentHMAC;
 QString OperatorUID;
 QString OperatorSKey;
 QString OperatorPID;
 QString OperatorHMAC;
 QString Ci;
 QString Consent;

}MOU_LEG2_Details;


//typedef struct
//{
// QString L0;
// QString L1;
// QString L2;
// QString L3;
// QString L4;
// QString L5;
// QString L6;
// QString L7;
// QString L8;
// QString L9;
// QString L10;
// QString L11;
// QString L12;
// QString L13;
// QString L14;
// QString L15;
// QString L16;
// QString L17;
// QString L18;
//}dummy_XML;



//void iso8583_init(isomsg *m);
//void iso8583_dump(FILE *fp,isodef *d, isomsg *m);
//void iso8583_free(isomsg *m);
//int getMTI(unsigned char mesclass,unsigned char mestype,isomsg *);

//int HexByte2AsciiStr(const unsigned char  *hex_val, int hex_len,char *str);
//int AsciiStr2HexByte(const char *ascii_str, int ascii_len,unsigned char* hex_val, int *hex_len);

//char getHexChar (unsigned char bt);
//int set_field(int ActualFldNumber,char *FieldData,isodef *d,isomsg *m,int ICCLen);
//t set_field(int,char *,isodef *,isomsg *);
//int GetSubFieldIndexs(char *FieldNumber,int *ActualFldNumber,int *SubFldIndex);
//int setMTI(unsigned char mesclass,unsigned char mesfun,isomsg *m);

#endif /* iso8583.h */
