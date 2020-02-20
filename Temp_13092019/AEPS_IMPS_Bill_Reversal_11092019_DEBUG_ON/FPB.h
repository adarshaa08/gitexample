#ifndef FPB_H
#define FPB_H

#include <QWidget>
//#include <QtGui>
#include <QDebug>
#include <QInputDialog>
#include <QListWidgetItem>
#include <QFile>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QCoreApplication>
#include <QPalette>
#include <QPixmap>
#include <QCloseEvent>
#include <QTime>
#include <QEvent>
#include <QTextStream>
#include <QKeyEvent>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QStringList>
#include <QTimer>
#include <QEvent>
#include <time.h>
#include<QXmlStreamReader>
#include <qpointer.h>
#include<fstream>
///#include "Login.h"
//#include "gl11.h"
#include "HostComm.h"
#include "database.h"
#include "auth_1.6.pb-c.h"
#include "json_VT.h"
#include "jsonforwards_VT.h"
#include "include/curl.h"
#include "selectscanner.h"


int iGenerateRequestID(char*, unsigned char *);

extern "C"{
#include "gl11.h"
#include "CHeader.h"
#include "glHeaderGSM.h"
#include "uid_auth.h"
}


//unsigned char JSONTagBuff[5000];//Used to get exact JSON packet from received data
//unsigned char gSendData[1024*15]; //Used to send data through network
//unsigned char Recvdata[1024*50]; //Used to receive
//unsigned char TagBuffer[4000]; //Used to store intermediate JSON data
//unsigned char		gHexBuffer[4028];//used in RD-Service and Login to store fp for central auth

extern unsigned char TagBuffer[4000]; //Used to store intermediate JSON data
extern unsigned char gSendData[1024*30]; //Used to send data through network
extern unsigned char Recvdata[1024*50]; //Used to receive
extern unsigned char JSONTagBuff[5000];
extern unsigned char BMRecvdata[1024*50]; //Used to receive
extern char gbankname[120];
extern char BankName_disp[100];
extern char AadhaarNo_disp[15];
extern char MobileNum_disp[12];
extern char Amount_disp[15];
extern char ibm[140];
extern char CUST_TYPE;
extern int TRANS_TYPE;
extern char str_url[500+1];
extern char FC_CustomerNumber[15];
extern char FC_MobileNumber[12];
extern char FC_AcceptorLocation[100];
extern int Flag_FirstTime_Login;
extern int Flag_eAgreement_Login;
extern int Flag_UpdateApp;
extern int Flag_eAgreement_Acceptance;
extern int show_bal;
extern QString buff_tmp;
extern unsigned char Cust_CurrMonthAmount[15];
extern int Calculate_impscharges;
extern int Flag_BeneVerif_Charges;
extern char Flag_Transaction_Type[10];
extern char Flag_IMPS_Walkin;

#define AEPS_BAL  10
#define AEPS_WITH 11
#define AEPS_MINI 12
#define AEPS_LTS  13
#define AEPS_LTTS 14

//-----------------------------------   FINOPDS   -------------------------------------

#define APP_VERSION "1.0.1"
#define UPDATE_APP  "770071002"
#define CLIENT_ID  "FINOPDS"
#define CHANNEL_ID "3"
#define CHANNEL_ID_IMPS 3
#define ISENCEYPT "false"
#define ISENCEYPTJSON false
#define BENE_VERIFICATION_CHARGES "1.0"
#define CALCULATE_CHARGES "0.0"
#define SGST_CALCULATE_CHARGES "0.0" // Prod_04092019
#define CGST_CALCULATE_CHARGES "0.0" // Prod_04092019

//-----------------------------------   FINOMER   -------------------------------------

//#define APP_VERSION "1.7.9"
//#define UPDATE_APP  "770071002"
//#define CLIENT_ID  "FINOMER"
//#define CHANNEL_ID "1"
//#define CHANNEL_ID_IMPS 1
//#define ISENCEYPT "true"
//#define ISENCEYPTJSON true
//#define BENE_VERIFICATION_CHARGES "1.0"
//#define CALCULATE_CHARGES "0.0"
//#define SGST_CALCULATE_CHARGES "0.0" // Prod_04092019
//#define CGST_CALCULATE_CHARGES "0.0" // Prod_04092019

//-------------------------------------------------------------------------------------

//---------------------------------------------     METHOD ID    --------------------------------------------------

#define MID_BANKMASTER 1                                // Bank Master Method ID (IMPS_MAPPING)
#define MID_TRANSACTIONSTATUS 16                        // Transaction Status

//-----------------------------------------------------------------------------------------------------------------




//#define CLIENT_ID  "FINOMER"
//#define CHANNEL_ID "1"
#define DEBUG 1

#ifdef PRODUCTION //----------------------------------------------------------------------------

#define URL_SSL          "https://partner.finopaymentbank.in/" // Production URL
#define LOGIN_URL_PROD   "UIService.svc/ProcessLoginAuthRequest"
#define GETPROFILE_URL_PROD  "UIService.svc/GetMastersData"
#define MER_REFRESH_URL_PROD "UIService.svc/NonFinancialProcessRequest"
#define CHANGEPASSWD_OTP_URL_PROD "UIService.svc/ProcessLoginRequest"
#define LOGOUT_URL_PROD "UIService.svc/ProcessLoginRequest"
#define AEPS_BLENQ_URL_PROD "UIService.svc/AEPSProcessRequest"
#define AEPS_BANKMASTER_URL_PROD "/UIService.svc/GetMastersData"
#define AEPS_LTS_URL_PROD "UIService.svc/AEPSProcessRequest"
#define AEPS_WITHDRAWAL_URL_PROD "UIService.svc/AEPSProcessRequest"
#define AEPS_LTTS_URL_PROD "UIService.svc/AEPSProcessRequest"
#define FORGET_URL_PROD  "UIService.svc/UtilityOtpNS"
#define OLDNEW_CHANGE_PASSWORD_URL_PROD "UIService.svc/UtilityOtp" //"UIService.svc/UtilityOtpNS"
#define IMPS_SEARCH_WALKING_CUST_URL_PROD "UIService.svc/ProcessRegistrationRequest"
#define IMPS_POST_BENE_VERIFICATION_URL_PROD "UIService.svc/NonFinancialProcessRequest"
#define OTP_POST_IMPS_PROD "UIService.svc/UtilityOtp"
#define COMMON_URL_PROD "UIService.svc/NonFinancialProcessRequest"
#define TRANSACTION_STATUS_PROD "UIService.svc/ProcessDashboardReportRequest"

#elif UAT_HTTP   //-----------------------------------------------------------------------------

#define URL_SSL          "https://partneruat.finopaymentbank.in/" // UAT HTTP URL

#else            //-----------------------------------------------------------------------------

//#define URL_SSL          "https://partneruat.finopaymentbank.in/" // UAT URL
#define URL_SSL          "https://partner.finopaymentbank.in/" // UAT URL
#define LOGIN_URL        "UIService.svc/ProcessLoginRequest"
#define GETPROFILE_URL   "UIService.svc/GetMastersData"
#define MER_REFRESH_URL  "UIService.svc/NonFinancialProcessRequest"
#define CHANGEPASSWD_OTP_URL "UIService.svc/ProcessLoginRequest"
#define LOGOUT_URL "UIService.svc/ProcessLoginRequest"
#define AEPS_BLENQ_URL "UIService.svc/AEPSProcessRequest"
#define AEPS_BANKMASTER_URL "UIService.svc/GetMastersData"
#define AEPS_LTS_URL "UIService.svc/AEPSProcessRequest"
#define AEPS_WITHDRAWAL_URL "UIService.svc/AEPSProcessRequest"
#define AEPS_LTTS_URL "UIService.svc/AEPSProcessRequest"
#define FORGET_URL  "UIService.svc/UtilityOtpNS"
#define OLDNEW_CHANGE_PASSWORD_URL "UIService.svc/UtilityOtp" //"UIService.svc/UtilityOtpNS"
#define MERCHANT_HISTORY_URL "UIService.svc/ProcessDashboardReportRequest"
#define LOGINAUTH_URL    "UIService.svc/ProcessLoginAuthRequest"

#define FIND_CUST "UIService.svc/ProcessRegistrationRequest"
#define IMPS_SEARCH_WALKING_CUST_URL "UIService.svc/NonFinancialProcessRequest"
#define IMPS_POST_BENE_VERIFICATION_URL "UIService.svc/NonFinancialProcessRequest"
#define OTP_POST_IMPS "UIService.svc/UtilityOtp"
#define COMMON_URL "UIService.svc/NonFinancialProcessRequest"
#define BILL_PAYMENT_URL "UIService.svc/ProcessBillPaymentRequest"
#define REVERSAL_URL "UIService.svc/NonFinancialProcessRequest"

#define TRANSACTION_STATUS "UIService.svc/ProcessDashboardReportRequest"


#endif // UAT    //-----------------------------------------------------------------------------

#define RD_DATA "P"
extern char Mer_Latest_Balance[30];
extern char Balance_Display[25];
extern char oBuff[25*1024];  // File Download

typedef struct
{
    int Txntype;
    char uid[16];
    char bin[16];
    char bankname[120];
    char Amount[16];
    char username[64];
    char Mobileno[12];
    char todate[10];
    char fromdate[10];
}Transaction;

typedef struct
{
    QString MC_data;
    QString PID_DATA;
    QString dpld;
    QString rdsld;
    QString rdsver;
    QString dc;
    QString mi;
    QString skey;
    QString ci;
    QString hmac;
    QString error_info;
    QString error_code;
}RD_SERVICE_STRUCTURE;

QT_BEGIN_INCLUDE_NAMESPACE
class Json;
QT_END_INCLUDE_NAMESPACE

#define OTP_FAILED -11
#define SESSION_EXPIRED_FAILED -9
#define FUNCTION_SUCCESS 0
#define FUNCTION_FAILED  -1
#define FUNC_BACK -5
#define FUNC_NEXT -7
#define FUNC_NEW_CUST -10
#define SOCKFUNC_FAILED -6
#define CAPACITIVE 1
#define OPTICAL 2
#define WALKING 30
#define CASA 31
#define OTP_PAGE 32
#define MINISTATEMENT  20

typedef struct
{
    unsigned char iDate[25];
    unsigned char Trantype[25];
    unsigned char Amount[25];
    unsigned char RRN[25];
    unsigned char AadharNo[25];
    unsigned char CustomerMobileNo[25];
    unsigned char TransactionComments[50];
    unsigned char iStatus[25];
    unsigned char LTS_Status[50];
    unsigned char Bank_Name[50];
    /*  unsigned char *template_ptr;	*/
}T_AEPS_LTS;

//! \brief Define a template list.
typedef struct list
{
    T_AEPS_LTS           AEPS_LTS_data;
    struct list *next;
}StrAEPS_LTS;

using namespace std;

namespace Ui {
class FPB;typedef struct
{
    int Txntype;
    char uid[16];
    char bin[16];
    char bankname[120];
    char Amount[16];
    char username[64];
    char Mobileno[12];
    char todate[10];
    char fromdate[10];
}Transaction;
}

class FPB : public QWidget
{
    Q_OBJECT
    
public:
    explicit FPB(QWidget *parent = 0);
    ~FPB();

    Json *json;
    Database dbaccess,dbaccess1;
    char *URL_CREATION(char *);
    int file_download(char *,char *,char *,int);
    void input_validation(void);
    int iGenerateRequestID(char *UserName,unsigned char *ReqID);
    char * iTimestamp();
    int makeJSONReqLEG1();
    int makeJSONReqLEG2();
    int makeJSONReqGetProfile(void);
    int DownloadAppSettings(void);
    int iMakeRequest(int TYPE);
    int iLogInLeg1(int tagID, unsigned char *ivalue);
    int iLogInLeg2(int, unsigned char *);
    int iParsLeg1(unsigned char *Data);
    int iParsLeg2(unsigned char *Data);
    int iGetTagResponse(char *Data,char *tag,char *buff,int offset);
    //int iParsJSON(unsigned char *cPacket, int (*iParsLeg1)(char *), char *RespC, char *RespMes);
    //void vWriteTag(TAGS *tags,  int (*callBackFunction)(char *));
    int iTagCount(unsigned char *iPBuff, char *tagToFind);
    // int iParseGP_ABM(char *ifilename,char *iFirstParseData,char *iLastParseData,int *txntype);
    int iIsJsonValid(char *cPacket,char *RespC, char *RespMes,int *itype);
    int iSetGet_Profile(int tagID, unsigned char *ivalue);
    void clear_fun(void);
    int iLoginpage();
    int iPasswordpage();
    //int vReadFile(char *cPath, char *buffer);
    //int testFunc(char *Str);
    int Check_Conditions();
    int Check_Battery_Voltage();
    //******Bank Master************/
    int iAEPSBnkMaster();
    int iSetAEPSBANKMAST(int tagID, unsigned char *ivalue);
    int iSetIMPSBANKMAST(int tagID, unsigned char *ivalue);
    int iSetAEPSMiniState(int tagID, unsigned char *ivalue);
    int makeJSONAEPSBANKMAST();
    static int iParseBank_Master( char *Data);
    static int iParseGet_Profile(char *Data);
    int AEPSProcess();
    int BankSelection(char *);
    int Validate_UID();
    void UnCheckFP();
    int rd_service_FP_capture();
    void process_rd_response(char *buffer_data);
    int iDisplayConsent(QString message);
    void memclear(char * fun_name);
    int Stringtrim(QString str);
    int check_RD_connection();
    int makeJSONAEPSWith(void);
    int makeJSONAEPSBALENQ(void);
    int makeJSONAEPSMini(void);
    int makeJSONLTS(void);
    int iSetAEPS_BalEnq(int tagID, unsigned char *ivalue);
    int iSetAEPS_With(int tagID, unsigned char *ivalue);
    int iSetLTS(int tagID, unsigned char *value);
    int Prepare_AEPS_BAL_Func(void);
    int Prepare_AEPS_WITHDRWL_Func(void);
    int Prepare_AEPS_MINISTAT_Func(void);
    int print_aes_transaction();
    int iParseAEPS_BalEnq(unsigned char *Data);
    int iParseAEPS_With(unsigned char *template_data);
    int iParseAEPS_MINIStatement(unsigned char*,char *,char * ,int *);
    int iParse_LTS(unsigned char *template_data);
    int print_aes_mini_stat(Transaction *Txn,int iCount);
    int GetLocalTime(char *szDate, char *szTime);
    int prn_txt(char *prn_buf, int iFontSize);
    int LTS_FUNC(void);
    int Prepare_AEPS_LTTS_Func();
    int iSetAEPSLTS(int tagID, unsigned char *ivalue);
    int iParse_AEPSLTS(unsigned char *Data);
    int makeJSONAEPSLTS();
    int iParse_AEPSLTS(unsigned char *Data,char *RespC,char *RespMes);
    int idisplayTxnDetailes(char display[][50],int icount);
    int idisplayConfirmation(char display[][120]);
    int FTP_download_function(char *filename);
    int AutoLTS(void);

    int Login_Data();
    void jsonpars1();
    int findcust(int/*char *, int*/);
    void masterfile();
    void ProdFeatureID();
    void commanfun(char *, char *);
    void TagNames(char *);
    void spiltfunction();
    int Version_check();
    int Prepare_OTP_Func(int);
    int iParseOTP(unsigned char *,char *,char *);
    int Prepare_Change_Func();
    int makeJSONOTP(int ivalue);
    int iSetOTP(int tagID, unsigned char *ivalue);
    int iSetNEWPASS(int tagID, unsigned char *ivalue);
    int iSetCHANGE(int iTagId, unsigned char *ivalue);
    int Prepare_newpass_Func();
    int makeJSONFINALOTP();
    int Prepare_encryption_Func(void);
    int iSetFORGET(int tagID, unsigned char *ivalue);
    int makeJSONFORGET();
    int iParseFORGET(unsigned char *Data,char *RespC,char *RespMes);
    int iSetFETCH(int tagID, unsigned char *ivalue);
    int fetchbene();
    int makeJSON(int ivalue);
    int iSetSEARCHWALK(int tagID, unsigned char *ivalue);
    int WalkinCust();
    int iSetWALKLMT(int tagID, unsigned char *ivalue);
    int Walkin_Cust_Limit(void);
    int WalkinLmt();
    int Resend_function();
    int Cashoutotp();
    int iSetCASHOUTOTP(int tagID, unsigned char *ivalue);
    int iSetCUSTLMT(int tagID, unsigned char *ivalue);
    int CustAvailLmt();
    int iSetOTPIMPS(int tagID, unsigned char *ivalue);
    int Otpimpsnft();
    int iSetOTPBENEGEN(int tagID, unsigned char *ivalue);
    int Otpgenbene();
    int Logout_Func(void);
    QString serialize1(char *jsondata);
    int NetworkConnect(int iSmallBigfile, char *IP_URL, int iSOC_CURL);
    int Refresh_func(void);
    int Change_pass_Func(void);
    int get_mac_id(char *macid , char *IP_Address);
    char *get_Lat_long(void);
    int login_download_func(void);
    char *ibank(void);
    int Enter_Amount_capture(void);
    int RequestDetachEncryptAttach(char *cPacket, int iPacketType, int iPacketLen);
    void remove_slash(unsigned char * Data);
    void remove_space(unsigned char * Data);
    int encryption(unsigned char * passpharse,unsigned char * salt,unsigned char * plaintext,unsigned char * DATA,int *outdata_len);
    void Derivedkey(unsigned char* passpharse,unsigned char* salt,unsigned char* concat_hash);
    int ResponseDetachEncryptAttach(char *cPacket, int iPacketType, int iPacketLen);
    void Decryption(unsigned char* encypted_text, unsigned char* passpharse,unsigned char* plain_text);
    int OTP_change_parse(QString temp);
    int Device_change_OTP();
    int Login1_data();
    int Merchant_history();
    int Mer_statement();
    void Balance_Hide_Fun();
    int validate_connection();
    int countLeapYears(QDate d);
    int getDifference(QDate dt1, QDate dt2);
    int findAge(QDate dt1, QDate dt2); //int current_date, int current_month, int current_year, int birth_date, int birth_month, int birth_year);
    int BankList() ;
    int Addbeneficiarywithouttxn();
    int Addbeneficiary();
    int IMPSCharges();
    int IMPS_Charges_parsing(char *cResChargeAmt);
    int Search_by_IFSC_Code_Parsing();
    int PostIMPS();
    int FetchExtraBene_Parsing();
    int ADD_Bene_Parsing();
    int POST_IMPS_Parsing();
    int Search_by_Bank_Name_Parsing();
    int Search_by_IFSC_Code();
    int Search_by_Bank_name();
    int LoginCardAcceptorLocation();
    int BeneVerifOTP_Parsing(void);
    int Otpbenever(void);
    int PostBeneVerification(void);
    int POST_BENEVERF_Parsing(void);
    int DeleteBene();
    int Deletedata();
    //------------------- IMPS -----------------------------
    int iSetFINDCUSTOMER(int tagID, unsigned char *value);
    int iBankMaster(void);
    int Bank_Master_Parsing();
    int FetchExtraBene(void);
    char *Convertstr1(string str);
    void clear_IMPS(void);
    int IMPS_AddBenePrint(void);
    int BeneVerificationCharges(void);
    int BeneVerification_Charges_Parsing(void);
    char username[25];
    unsigned char EncPIDData[10000];
    unsigned char FINALENCDATA[10000];
    int json_creation(int flag,unsigned char*);
    int billpayment_charges();
    int Bill_Charges_parsing();
    int PostBillPayment();
    int Post_bill_parsing();
    int Fetch_bill();
    int Fetch_bill_parsing();
    void Clear_SupportData(void);
    int DeviceInfo(void);
    void Clear_DeviceInfo(void);
    int GetReversalRequest();
    int GetReversal_Parsing();
    int Reversal_otp_request();
    int Reversal_Refund_otp_Parsing();
    int Reversal_Refund_Request();
    int Reversal_Refund_Parsing();
    int Reversal_Refire_Request();
    int iSetREFIRE(int tagID, unsigned char *ivalue);
    int makeJSONREFIRE();
    int makeJSONREFUND();
    int iSetREFUND(int tagID, unsigned char *ivalue);
    QString setSplitSeqNumber();
    QString getSplitSequence(QString);
    int getAmount(QString );
    QString getReversalSumOfChargesFromParentTxnId(QString );
    int getChargeAmountFromDB(QString);
    void getreferenceno(QString );
    QString getsupportdataref();
    void getreversaldata(QString , int);

    int hash_value(unsigned char * ,unsigned char * ,unsigned char * ,int );
    int Login_encryption(char *, int ,unsigned char *);
    int LoginDetachEncryptAttach(char *cPacket, int iPacketLen,unsigned char * static_key,int iPacketType);
    QString getReversalSumOfAmountFromParentTxnId(QString );
    QString getReversalSumOfCharges(QString );
    QString setSplitValue();
    int iReq_SecretQuestion();
    int iReq_SecretQuestion_parsing();
    int iReq_SecretQuestion_Submit();
    int iReq_SecretQuestion_Submit_Parse();
    void Insert_func(char *);
    int secretanswer_encryption(char * ,char * , char *);
    int Forget_question_parsing();
    char *IFSC_APPEND(char *myString, char *arr);
    //-------------------  Bill Payments  ---------------------

    QString spilt_function(char * Data);
    int makeJSONBILLREFIRE();
    int iSetBILLREFIRE(int , unsigned char *);
    int Reversal_Billpayment_Request();
    int Post_billreversal_parsing();
    int Reversal_Print();
    int Billpayment_Print();
    int bill_input_validation();

    //---------------------------------------------------------

    int TransactionStatus(void);
    int TranStatus_Parsing(void);
    int deserialize1(char * );

    int Mini_Statement(int miniflag);
    QString spiltRFUvalue(char *, int position );
    int Ministatement_Parse(int miniflag);
    QString RFU_Display(QString Tagvalue, int pos );
    int Mini_Print();
    int Agent_Auth();
    QString fetch_data();
    QString Bill_label(QString );
    QString strUID;
    QString entryDlgText;
    QString QLTSflag;


private :



private slots:

    void getInputText(QString argstr);

    void on_pushButton_clicked();

    //void on_GetProfile_pushbutton_clicked();

    void on_listWidget_Issuer_clicked(const QModelIndex &index);

    void on_PAGE4_OK_pushButton_8_clicked();

    void on_pushButton_3_clicked();

    void on_rdRightThumb_3_clicked(bool checked);

    void on_rdleft_3_clicked(bool checked);

    void on_rdLeftIndex_3_clicked(bool checked);

    void on_rdLeftMiddle_3_clicked(bool checked);

    void on_rdLeftRing_3_clicked(bool checked);

    void on_rdLeftLittle_3_clicked(bool checked);

    void on_rdRightIndex_3_clicked(bool checked);

    void on_rdRightMiddle_3_clicked(bool checked);

    void on_rdRightRing_3_clicked(bool checked);

    void on_rdRightLittle_3_clicked(bool checked);

    void on_BALENQ_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_PAGE32_exit_clicked();

    void on_PAGE32_exit_2_clicked();

    int on_OK_SBN_pushButton_10_clicked();

    void on_BACK_SBN_pushButton_9_clicked();

    void on_BtnBackIssuer_clicked();

    void on_lineEditUID1_textChanged(const QString &arg1);

    void on_lineEditUID2_textChanged(const QString &arg1);

    void on_lineEditUID3_textChanged(const QString &arg1);

    void on_PAGE4_BACK_pushButton_7_clicked();

    void on_WITHDR_pushButton_2_clicked();

    void on_OK_Mobile_Num_Page9_clicked();

    void on_lineEdit_Mobile_Num_Page9_textChanged(const QString &arg1);

    void on_BACK_Mobile_Num_Page9_clicked();

    void on_MINIST_pushButton_5_clicked();

   //void on_pushButton_4_clicked();

    void on_PushButton_Back_Txn_Menu_clicked();

  //  void on_pushButton_AEPS_Txn_Menu_clicked();

    void on_PushButton_Back_SubTxn_Menu_clicked();

    void on_LTS_pushButton_TxnMenu_clicked();

    void on_LTTS_pushButton_4_clicked();

    void on_OK_pushButton_9_clicked();

    void on_BACK_pushButton_8_clicked();

    void on_Button_display_clicked();

    void on_BankMaster_clicked();

  //  void on_AppUpdate_PushButton_clicked();

    void on_Next_Otp_Button_clicked();

    void on_Forget_password_next_button_clicked();

    void on_Forget_button_clicked();

    void on_Forget_password_Back_button_2_clicked();

    void on_Discard_otp_Button_clicked();

    // void on_Refresh_otp_button_clicked();

    // void on_pushButton_5_clicked();

    void on_MinimizePB_clicked();

    void on_Search_Button_clicked();

  //  void on_pushButton_IMPS_Txn_Menu_2_clicked();

    void on_Next_back_button_clicked();

    void on_person_add_back_button_clicked();

 //   void on_PAGE32_MainMenu_exit_button_clicked();

    void on_MerchanData_clicked();

    void on_PushButton_ChangePasswd_pg26_clicked();

    void on_PushButton_UpdatePasswd_pg26_clicked();

    void on_PushButton_Logout_pg26_clicked();

    void on_Refresh_Button_clicked();

    void on_PushButton_Back_pg26_clicked();

    void on_Balance_show_button_clicked();

    void on_pushButton_Dash_AEPS_pg12_clicked();

    void on_OldNew_ChangePasswd_Back_button__clicked();

    void on_OldNew_ChangePasswd_Next_button__clicked();

    void on_Resend_otp_button_clicked();

    //    void on_radioButton_clicked(bool checked);

    //    void on_eAgreement_radioButton_clicked(bool checked);

    // void on_Eagreement_Back_Button_pg27_clicked();

    void on_Eagreement_IAccept_Button_pg27_clicked();

    // void on_eAgreement_checkBox_clicked(bool checked);

    void on_OK_Enter_Amout_Page28_clicked();

    void on_BACK_Enter_Amout_Page28_clicked();



    void on_Next_Button_Confirmation_clicked();


    void on_Button_Confirmation_clicked();


    void on_FirstTimeLogin_pushButton_pg30_clicked();




    void on_OTP_submit_button_clicked();

    void on_OTP_back_button_clicked();

    void on_PushButton_Back_pg26_3_clicked();

    void on_PushButton_Back_pg26_2_clicked();

    void on_Transaction_Summary_3_clicked();


    void on_Balance_Show_pushButton_UID_page_clicked();

    void on_Balance_Show_pushButton_FromToDate_page_clicked();

    void on_Balance_Show_pushButton_BankSelection_page_clicked();

    void on_Balance_Show_pushButton_BankName_page_clicked();



    void on_Balance_Show_pushButton_Amount_page_3_clicked();


    void on_Balance_Show_pushButton_Confirm_page_3_clicked();

    void on_Balance_Show_pushButton_Transaction_page_2_clicked();

    void on_Balance_Show_pushButton_BankName_page_2_clicked();


    void on_OK_pushButton_pg34_clicked();

    void on_Forgot_Password_Button_User_pg1_clicked();
    void on_person_add_next_button_clicked();
    void on_Bene_next_button_clicked();
    void on_Bene_IFSC_button_clicked();
    void on_Ok_IMPS_Button_clicked();
    void on_BtnNew_Bene_clicked();
    void on_Next_Otp_Button_2_clicked();
    void on_Personal_Back_button_clicked();


    void on_Bene_previous_button_clicked();

    void on_Bene_IFSC_Back_button_clicked();


    void on_Calculate_charge_button_clicked();

    void on_Previous_IMPS_Button_clicked();

    void on_Discard_otp_Button_2_clicked();


    void on_Personal_Back_button_26_clicked();

    void on_Bene_Search_Button_clicked();

    void on_Next_back_button_36_clicked();

    void on_IFSC_ok_button_2_clicked();

    void on_Search_IFSC_button_2_clicked();

    void on_Verify_button_clicked();

    void on_IFSC_cancel_Button_2_clicked();

    void on_cmb_state_2_activated(const QString &arg1);

    void on_BtnRegistered_Bene_clicked();

    void on_cmb_Registered_Bene_List_activated(const QString &arg1);


    void on_Option_Button_clicked();

    void on_Remittance_button_clicked();

    void on_Back_PushButton_Pg35_clicked();

    void on_FINISH_PushButton_Pg34_clicked();

    void on_Search_lineEdit_textChanged(const QString &arg1);

    void on_pushButton_17_clicked();

    void on_Customer_detail_back_Button_3_clicked();

    void on_pushButton_5_clicked();

    void on_Fund_transfer_amount_edit_textChanged(const QString &arg1);

    void on_cmb_BranchName_2_activated(int index);

    void on_pushButton_7_clicked();

    void on_pushButton_6_clicked();

    void on_Print_Receipt_Button_Pg34_clicked();


    void on_dateEdit_3_dateChanged(const QDate &date);

    void on_Person_state_edit_comboBox_activated(const QString &arg1);

    void on_comboBox_3_currentIndexChanged(int index);

    void on_Bill_payment_Button_clicked();

    void on_Bill_category_activated(const QString &arg1);

    void on_Bill_service_provider_activated(const QString &arg1);

    void on_Bill_Back_Button_3_clicked();

    void on_Bill_payment_Next_button_clicked();

    void on_Calculate_charges_button_clicked();

    void on_Fetch_bil_button_clicked();

    void on_Bill_payment_back_button_clicked();

    void on_Bill_payment_Next_button_2_clicked();

    void on_Confirmation_back_button_clicked();

    void on_Confirmation_next_button_clicked();


    void on_first_editline_textEdited();

    void on_second_editline_textEdited();

    void on_Enter_amount_edit_textEdited(const QString &arg1);

    void on_pushButton_Ok_clicked();


    void on_Back_Reversal_data_dsp_clicked();

    void on_btnReversal_confirmation_back_clicked();

    void on_btnRefund_clicked();

    void on_btnRefire_clicked();

    void on_Reversal_conf_button_clicked();

    void on_Reversal_next_button_clicked();

    void on_Reversal_Button_clicked();

    void on_listWidget_Reversal_data_clicked(const QModelIndex &index);

    void on_Addquest_backbutton_clicked();

    void on_btn_SecretQuestion_submit_clicked();

    void on_Submit_backbutton_clicked();

    void on_btn_SecretQuestion_submit_2_clicked();


    void on_IMPS_Select_Button_clicked();

    void on_NEFT_Button_clicked();

    void on_Transaction_Status_wdRRN_pushButton_PG12_clicked();

    void on_TransactionID_TransStWdRRN_OK_pushButton_pg41_clicked();


    void on_Minstatement_clicked();

    void on_listWidget_clicked(const QModelIndex &index);



    void on_TransactionStatus__exit_button_PG10_clicked();


    void on_TS_ParentChild_listWidget_PG11_clicked(const QModelIndex &index);


private:
    Ui::FPB *ui;
};

#endif // FPB_H
