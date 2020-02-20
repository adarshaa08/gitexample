
//#include "FPB.h"
#include "ui_FPB.h"
#include "displaymessage.h"
#include "aadhaar.h"
#include "entrydlg.h"
#include "Login.h"
#include "impscommon.h"
#include "time.h"
#include "findcust.h"   //added on 25/07/2019
extern char * Convertstr(string str);
extern int BankDetails_Search;
extern POSTBENEVER postbeneverif;
extern searchWalkCust SearchWalkCust;
extern FINDCUSTOMER FINDCUST;    //added on 25/07/2019
POSTIMPS_RESP resp_postimps;
extern OTPBENEVERIF OtpBeneVer;
extern SUPPORTDATA_DEVICEINFO sd_deviceinfo;
extern FetchExtraBeneList FetchBeneList;

unsigned char eBene_beneficiaryId[30][20], eBene_nickName[30][30], eBene_beneficiaryName[30][30], eBene_beneficiaryBank[30][35];
unsigned  eBene_beneficiaryBankCity[30][30], eBene_beneficiaryBranch[30][35], eBene_beneficiaryBankIfsc[30][15],eBene_status[30][3]/*,eBene_beneficiaryBankBranch[15][40]*/;
unsigned char eBene_beneficiaryEmailId[30][40], eBene_eBene_applicationId[30][15], eBene_beneficiaryType[30][5], eBene_verificationStatus[30][20],eBene_beneficiarysequence[30][5];
int count_bene = 0, count_bc_merchant_acc = 0, index_bc_merchant_acc = 0;
extern char flag_bc_merchant_account;
extern int OTP_page, flag_bene_verfication_status;

#include<fstream>

//int index_bank = 0;

extern "C"
{
#include "gl11.h"
#include "http_client.h"
#include "uid_auth.h"
}
extern int ScannerType;
extern Login_leg2 configLoginLeg2;
extern OTPIMPSNEFT OtpImpsNeft;
extern sCustDetails CustDet;
SEARCHBYIFSC byifsc;
SEARCHBYBRANCH Searchbybranch;
DISPLAY_IMPS display_imps;
ADDBENE add_bene_resp;
CALCULATECHARGES_RESP cal_charges_resp;
int Calculate_impscharges = 0;
QString buff_tmp;
int isearchby;


//void FPB::on_pushButton_IMPS_Txn_Menu_2_clicked()
//{
//    if (CUST_TYPE == 'W')
//    {
//        ui->stackedWidget->setCurrentIndex(16);
//    }
//    else
//    {
//        QString str = (char *) FC_MobileNumber;
//        ui->Mobile_lineedit->setText(str);
//        ui->stackedWidget->setCurrentIndex(35);
//    }
//    return;
//}


int FPB::Addbeneficiarywithouttxn()
{
    QString buff_tmp;
    int iRet = 0;

    Json1::Value RequestedData, SubData, SubData2, CreateCustomer;
    Json1::Value ArrayList(Json1::arrayValue);
    Json1::Value addressesArraylist(Json1::arrayValue);
    Json1::Value CreateCustomerArraylist(Json1::arrayValue);


    uidmessage::instance()->hide_Message();
    uidmessage::instance()->Display_BusyMessage("Transaction Request in Progress");

    char nullbuff[5];
    memset(nullbuff, 0, sizeof(nullbuff));
    strcpy(nullbuff,"");

    char buff1[100];
    iGenerateRequestID((char *)username,(unsigned char *)buff1);

    RequestedData["RequestId"] = buff1;//"TLR1032_2620191138138";
    RequestedData["MethodId"] = 119;
    RequestedData["ChannelID"]= CHANNEL_ID;

    RequestedData["SessionId"] = (char *)configLoginLeg2.ucSessionId; //buff_tmp.toAscii().data();//"9d044213-09d9-400d-aa96-5e9a473ba494";
    RequestedData["TellerID"]= (char *)username; // Dhiral
    RequestedData["TokenId"]= "";
    RequestedData["X_Auth_Token"] ="";
    RequestedData["IsEncrypt"]= ISENCEYPT;
    RequestedData["Amount"]= "0";//CustDet.cTxnAmount;//"5075"; // Dhiral
    RequestedData["MerchantContribution"]= "0"; // Dhiral

    if(flag_bc_merchant_account == 'B')
        RequestedData["ProductCode"]= "1003|N"; //""; // Dhiral
    else
        RequestedData["ProductCode"]= "9999|Y"; //""; // Dhiral

    RequestedData["SessionExpiryTime"]= (char *)configLoginLeg2.ucSessionExpiryTime;//"10/11/2018 11:58:44 PM";
    RequestedData["Charges"]= "0.0"; // Dhiral

    /////////////////////////////RequestedData///////////////////////////////////////////
    qDebug()<<"11111";

    char arrBitmap[10];
    memset(arrBitmap, 0, sizeof(arrBitmap));
    //    if(Flag_IMPS_Walkin == 'S') // Add Walkin Customer
    //        memcpy(arrBitmap, (char *)"10011000", 8); // Add Walkin Customer Bitmap is ON
    //    else
    //        memcpy(arrBitmap, (char *)"10010000", 8);


    if(flag_bitmap == 'W')                                              // For New Walking Customer
    {
        if(Flag_IMPS_Walkin == 'S')
            memcpy(arrBitmap, "11011000", 8);     // To create and add Walkin Customer
        else
            memcpy(arrBitmap, "11010000", 8);     // For Non-Walkin Customer..Only Create Customer

    }
    else
    {
        if(Flag_IMPS_Walkin == 'S')
            memcpy(arrBitmap, "10011000", 8);     // To create and add Walkin Customer
        else
            memcpy(arrBitmap, "10010000", 8);     // For Non-Walkin Customer..Only Create Customer
    }


    memset(CustDet.cAccountNm, 0, sizeof(CustDet.cAccountNm));
    memset(CustDet.cCustomerNm, 0, sizeof(CustDet.cCustomerNm));
    memset(CustDet.cMobileNumber, 0, sizeof(CustDet.cMobileNumber));

    memcpy(CustDet.cAccountNm , (char *)"999999999999", 12);
    memcpy(CustDet.cCustomerNm, (char *)"999999999999", 12);
    memcpy(CustDet.cMobileNumber, FC_MobileNumber, strlen(FC_MobileNumber));


    QString Bitmap =(char *)arrBitmap;
    qDebug()<<"Bitmap = "<<Bitmap;

    SubData["asyncFlag"] = "Y";
    SubData["totalAmount"] = "0";
    SubData["impsserviceflag"] = "Y";
    SubData["txnType"] = (char *)Flag_Transaction_Type;
    SubData["cashoraccount"] = "CASH";
    SubData["mobileNumber"] = (char *) FC_MobileNumber;
    SubData["requestbitmap"] = (char *)arrBitmap;
    if(flag_bc_merchant_account != 'B')
        SubData["merchantMobileNumber"] = (char *)configLoginLeg2.ucmobileNumber;

    //--------------------------------------------     CustomerAuth       ---------------------------------------------------

    if(Bitmap.mid(0,1)=="1")
    {

        SubData["CustomerAuth"]["Auth_id"] = 1; // 1 = OTP, 3 = Central FP Auth
        SubData["CustomerAuth"]["MobileNo"] = (char *) FC_MobileNumber;
        SubData["CustomerAuth"]["AadharNo"] = nullbuff;


        SubData["CustomerAuth"]["Otp"]["RequestId"] = buff1;
        SubData["CustomerAuth"]["Otp"]["MethodId"] = 2;
        SubData["CustomerAuth"]["Otp"]["SessionId"] = (char *)configLoginLeg2.ucSessionId;
        SubData["CustomerAuth"]["Otp"]["TellerID"] = (char *)username;
        SubData["CustomerAuth"]["Otp"]["IsEncrypt"] = false ;
        SubData["CustomerAuth"]["Otp"]["X_Auth_Token"] ="";
        SubData["CustomerAuth"]["Otp"]["TokenId"] = "";
        SubData["CustomerAuth"]["Otp"]["SessionExpiryTime"] = (char *)configLoginLeg2.ucSessionExpiryTime;
        ArrayList.clear();

        // -----------------        OTP     -----------------

        SubData2["MethodId"] ="2";
        SubData2["RequestId"] =OtpImpsNeft.Commdata_OIN.ucRequestID_res;
        SubData2["CustomerMobileNo"] = (char *) FC_MobileNumber;
        SubData2["OtpPin"] = CustDet.cOtp;
        SubData2["MessageId"] = 39;
        SubData2["OtpParam"] = "";

        buff_tmp = serialize1(Convertstr(SubData2.toStyledString()));
        SubData["CustomerAuth"]["Otp"]["RequestData"] = buff_tmp.toAscii().data();

        // --------------------------------------------------

        qDebug()<<"33333";

        SubData["CustomerAuth"]["Aadhaar"]="";
        SubData["CustomerAuth"]["Fp_Auth"]="";

    }
    else
    {
        SubData["CustomerAuth"] = "";
    }

    //--------------------------------------------     CreateAccount       -------------------------------------------------

    if(flag_bc_merchant_account != 'B')
        SubData["CreateAccount"] = "";


    //--------------------------------------------     CreateCustomer       ---------------------------------------------

    if(Bitmap.mid(1,1)=="1")
    {
        CreateCustomerArraylist.clear();

        CreateCustomer["employment"]["taxDetails"] =  CreateCustomer.null;
        CreateCustomer["idProof"] =  CreateCustomer.null;
        CreateCustomer["addressProof"] =  CreateCustomer.null;

        CreateCustomer["residencyCode"] = "1";
        CreateCustomer["customerType"] = "0";
        CreateCustomer["currencyCode"] = "INR";

        // --- personalDetails ---

        CreateCustomer["personalDetails"]["title"] = ui->cmbTitle->currentText().trimmed().toAscii().data();
        CreateCustomer["personalDetails"]["firstNm"] = (char *)CustDet.cFirstName;
        CreateCustomer["personalDetails"]["lastNm"] = (char *)CustDet.cLastName;
        CreateCustomer["personalDetails"]["name"] = (char *)CustDet.cCustName;
        CreateCustomer["personalDetails"]["dob"] = (char *)CustDet.cDob;
        CreateCustomer["personalDetails"]["gender"] =(char *) CustDet.cGender;
        CreateCustomer["personalDetails"]["aadharNumber"] =  CreateCustomer.null;
        CreateCustomer["personalDetails"]["taxId"] =  CreateCustomer.null;
        CreateCustomer["personalDetails"]["accountNm"] = (char *)CustDet.cAccountNm;
        CreateCustomer["personalDetails"]["customerNm"] = (char *)CustDet.cCustomerNm;

#if DEBUG
        printf("CustDet.cGSTINNumber = %s\n", CustDet.cGSTINNumber);
#endif
        if(strcmp(CustDet.cGSTINNumber, "") == 0)
            CreateCustomer["personalDetails"]["cortexId"] =  CreateCustomer.null;
        else
            CreateCustomer["personalDetails"]["cortexId"] = (char *)CustDet.cGSTINNumber;

        CreateCustomer["personalDetails"]["code"] = "1";
        CreateCustomer["personalDetails"]["branchOwnership"] = "1011";
        CreateCustomer["personalDetails"]["photo"] =  CreateCustomer.null;

        // --- contactDetails ---

        CreateCustomer["contactDetails"]["mobile"] = (char *)CustDet.cMobileNumber;
        CreateCustomer["contactDetails"]["email"] =  CreateCustomer.null;

        if(flag_bitmap != 'W')
            CreateCustomer["contactDetails"]["photo"] =  CreateCustomer.null;

        SubData2.clear();
        addressesArraylist.clear();
        for(int i=0;i<1;i++)
        {
            SubData2["addressType"] = "P";
            SubData2["address1"] = (char *)CustDet.cAddress;
            SubData2["postalCode"] = (char *)CustDet.cPincode;
            SubData2["city"] = (char *)CustDet.cCity;
            SubData2["state"] = (char *)CustDet.cStatecode;
            SubData2["country"] = (char *)CustDet.cCountrycode;

            addressesArraylist.append(SubData2);
            SubData2.clear();
        }
        CreateCustomer["addresses"] = addressesArraylist;

        CreateCustomerArraylist.append(CreateCustomer);


        SubData["CreateCustomer"] = CreateCustomerArraylist;
    }
    else
    {
        SubData["CreateCustomer"] = "";
    }

    //--------------------------------------------     AddBeneficiary       ---------------------------------------------

    if(Bitmap.mid(3,1)=="1")
    {
        SubData["AddBeneficiary"]["appId"] = CLIENT_ID;
        SubData["AddBeneficiary"]["customerNumber"] = (char *)FC_CustomerNumber; //username;
        SubData["AddBeneficiary"]["beneficiaryType"] ="3";
        if(flag_bc_merchant_account == 'B')
            SubData["AddBeneficiary"]["nickName"] = "EMD";
        else
            SubData["AddBeneficiary"]["nickName"] = "";

        SubData["AddBeneficiary"]["beneficiaryAccount"] = (char *)ui->Bene_acc_no_edit->text().trimmed().toAscii().data();
        SubData["AddBeneficiary"]["beneficiaryAccountType"] = "10";
        SubData["AddBeneficiary"]["beneficiaryName"] = (char *) ui->Bene_Name_edit->text().trimmed().toAscii().data();
        SubData["AddBeneficiary"]["beneficiaryBank"] = (char *) ui->cmb_bankList->currentText().trimmed().toAscii().data();
        SubData["AddBeneficiary"]["beneficiaryBankCity"] = (char *)CustDet.cCity;
        SubData["AddBeneficiary"]["beneficiaryBankBranch"] = (char *)CustDet.cBranchName;
        SubData["AddBeneficiary"]["beneficiaryBankIfsc"] = (char*)ui->IFSC_edit->text().toAscii().trimmed().data();
        SubData["AddBeneficiary"]["beneficiaryAddress1"] = nullbuff;

        SubData["AddBeneficiary"]["beneficiaryState"] = (char *)CustDet.cBranchstate;
        SubData["AddBeneficiary"]["beneficiaryCity"] = (char *)CustDet.cCity;
        SubData["AddBeneficiary"]["beneficiaryZip"] =nullbuff;
        SubData["AddBeneficiary"]["beneficiaryEmailId"] =nullbuff;
        //    SubData["AddBeneficiary"]["beneficiaryMobileNumber"] ="";
        SubData["AddBeneficiary"]["beneficiaryBankMicr"] ="";

        if(flag_bc_merchant_account == 'B')
            SubData["AddBeneficiary"]["beneficiaryMaxLimit"] = (char *)FetchBeneList.ucbeneficiaryMaxLimit; // need to change
        else
            SubData["AddBeneficiary"]["beneficiaryMaxLimit"] = "25000";


    }
    else
    {
        SubData["AddBeneficiary"] = "";
    }

    //--------------------------------------------     AddWalkinCustomer       ---------------------------------------------

    if(Bitmap.mid(4,1)=="1")
    {
        if(flag_bc_merchant_account == 'B')
            SubData["AddWalkinCustomer"] = "";
        else
        {

            SubData["AddWalkinCustomer"]["name"] = (char *)SearchWalkCust.SWCL_name_resp;
            SubData["AddWalkinCustomer"]["mobileNumber"] = (char *) FC_MobileNumber;
            SubData["AddWalkinCustomer"]["address"] = (char *)CustDet.cAddress;
            SubData["AddWalkinCustomer"]["gender"] = (char *) CustDet.cGender;
            SubData["AddWalkinCustomer"]["idProofType"] = "";
            SubData["AddWalkinCustomer"]["idProofNumber"] = "";
            SubData["AddWalkinCustomer"]["AddressProofType"] = "";
            SubData["AddWalkinCustomer"]["addressProofNumber"] = "";
        }
    }
    else
    {
        SubData["AddWalkinCustomer"] = "";
    }

    //--------------------------------------------     AddExternalAccount       ---------------------------------------------

    if(flag_bc_merchant_account != 'B')
        SubData["AddExternalAccount"] = "";

    //-----------------------------------------------------------------------------------------------------------------------

    char buff2[100];
    memset(buff2,0,sizeof(buff2));
    iGenerateRequestID((char *)username,(unsigned char *)buff2);

    //    ArrayList.clear();

    //--------------------------------------------     IMPSServiceDataList       ---------------------------------------------

    SubData["IMPSServiceDataList"]["BeneAccNo"] = (char *)ui->Bene_acc_no_edit->text().trimmed().toAscii().data();
    SubData["IMPSServiceDataList"]["BeneIFSC"]  =  (char*)ui->IFSC_edit->text().toAscii().trimmed().data();
    SubData["IMPSServiceDataList"]["BeneName"] =  (char *) ui->Bene_Name_edit->text().trimmed().toAscii().data();
    SubData["IMPSServiceDataList"]["MerchantMobileNo"] = (char *)configLoginLeg2.ucmobileNumber;
    SubData["IMPSServiceDataList"]["PaymentRef"] = buff2;
    SubData["IMPSServiceDataList"]["RemeMobileNo"] = (char *) FC_MobileNumber;
    SubData["IMPSServiceDataList"]["RemeName"] =  (char *)SearchWalkCust.SWCL_name_resp;
    SubData["IMPSServiceDataList"]["TxnAmount"] = "0";
    //    ArrayList.append(IMPSServiceDataList);



    //    RequestedData["IMPSServiceDataList"] = ArrayList;


    buff_tmp.clear();
    buff_tmp = serialize1(Convertstr(SubData.toStyledString()));
    RequestedData["RequestData"] = buff_tmp.toAscii().data();

    char *Ret;
    Ret=Convertstr(RequestedData.toStyledString());

    memset(gSendData,0,sizeof(gSendData));
    strncpy((char*)gSendData, Ret, strlen(Ret));
 #if DEBUG
    printf("gSendData = %s\n",gSendData);
 #endif




    /****************************Encryption function*********************************/
   if(!strcmp((char *)ISENCEYPT,"true"))
    {
       int gSendData_len =strlen((char *)gSendData);
        RequestDetachEncryptAttach((char *)gSendData,2,gSendData_len);
        qDebug()<<"After encrypt function";
    }
    /***************************************************************/

    memset(str_url,0,sizeof(str_url));
#ifdef PRODUCTION
    strcpy(str_url,URL_CREATION(LOGIN_URL_PROD));
#else
    strcpy(str_url,URL_CREATION((char *)COMMON_URL));
#endif

    system("rm -rf /usr/FINO/FPB/RESP/ADD_Bene.TXT");
    int retk = file_download(str_url, (char *)gSendData,(char *)"/usr/FINO/FPB/RESP/ADD_Bene.TXT",120);
    if(retk!=0)
    {
        sleep(1);
        uidmessage::instance()->hide_Message();
        return FUNCTION_FAILED;
    }

    /****************************Decryption function*********************************/
    if(!strcmp((char *)ISENCEYPT,"true"))

    {
        int oBuff_len = strlen(oBuff);
#if DEBUG
        qDebug()<<"Length of buffer"<<oBuff_len;
        printf("Buffer%s\n",oBuff);
#endif
        ResponseDetachEncryptAttach(oBuff,2,oBuff_len);//TEMPORARY

        system("rm /usr/FINO/FPB/RESP/ADD_Bene.TXT");
        FILE *fp2 = NULL;
        fp2 =fopen("/usr/FINO/FPB/RESP/ADD_Bene.TXT","a");
        fwrite(oBuff,strlen((char *)oBuff),1,fp2);
        sleep(1);
        fclose(fp2);
    }
    /*******************************************************************************/
 //   file_download( "http://103.1.112.205:4444//UIService.svc/NonFinancialProcessRequest", Ret,"/usr/FINO/FPB/RESP/ADD_Bene.TXT",120);
    iRet = ADD_Bene_Parsing();
    return iRet;

}


int FPB::Addbeneficiary()
{
    int iRet = 0;
    char* jsondata;
    Json1::Value CreateCustomer;

    Json1::Value RequestedData,SubData,SubData2;
    Json1::Value addressesArraylist(Json1::arrayValue);
    Json1::Value CreateCustomerArraylist(Json1::arrayValue);

    char buff1[100];
    memset(buff1,0,sizeof(buff1));
    iGenerateRequestID((char *)username,(unsigned char *)buff1);

    RequestedData["RequestId"] = buff1;//"TLR1032_2620191138138";
    RequestedData["MethodId"] = "119";
    //RequestedData["ChannelID"]= "2";
    RequestedData["SessionId"] =  buff_tmp.toAscii().data();//"9d044213-09d9-400d-aa96-5e9a473ba494";
    RequestedData["TellerID"]= (char *)username; // Dhiral
    RequestedData["TokenId"]= "";
    RequestedData["X_Auth_Token"] ="";
    RequestedData["IsEncrypt"]= false;
    RequestedData["Amount"]= "0";//CustDet.cTxnAmount;//"5075"; // Dhiral
    RequestedData["MerchantContribution"]= "0"; // Dhiral
    RequestedData["ProductCode"]= "9999|Y"; //""; // Dhiral
    RequestedData["SessionExpiryTime"]= (char *)configLoginLeg2.ucSessionExpiryTime;//"10/11/2018 11:58:44 PM";
    /////////////////////////////RequestedData///////////////////////////////////////////

    SubData["asyncFlag"] = "Y"; //"N"; // DhiralADD_Bene_Parsing
    SubData["totalAmount"] = "0"; //CustDet.cTxnTotalAmount;//;"100"; // Dhiral
    SubData["impsserviceflag"] = "Y"; // Dhiral
    SubData["txnType"] = (char *)Flag_Transaction_Type; //"RECHARGE"; // Dhiral
    SubData["cashoraccount"] = "CASH";
    SubData["mobileNumber"] = "8291173892"; //CustDet.cMobileNumber;//"8055988351"; // Dhiral
    SubData["requestbitmap"] = "10010000";
    SubData["merchantMobileNumber"] = (char *)configLoginLeg2.ucmobileNumber;//"8055988351";


    SubData["parentRRN"] = (char *)configLoginLeg2.ucRRN_req;//"TLR1032_26201913202573";
    SubData["impsserviceflag"] = "Y"; // "N"; // Dhiral
    SubData["CreateAccount"] = "";
    SubData["AddWalkinCustomer"]["name"] = "Dhiral Rupani";
    SubData["AddWalkinCustomer"]["mobileNumber"] = "8291173892";
    SubData["AddWalkinCustomer"]["address"] = "Kamothe";
    SubData["AddWalkinCustomer"]["idProofType"] = "";
    SubData["AddWalkinCustomer"]["idProofNumber"] = "";
    SubData["AddWalkinCustomer"]["AddressProofType"] = "";
    SubData["AddWalkinCustomer"]["addressProofNumber"] = "";
    SubData["AddExternalAccount"] = "";
    ///////////////////////////////////////OTP//////////////////////////////////////////////////

    SubData["CustomerAuth"]["Auth_id"]="1";
    SubData["CustomerAuth"]["MobileNo"]="8291173892"; //CustDet.cMobileNumber;//"8096031999"; // Dhiral
    SubData["CustomerAuth"]["AadharNo"]="null";


    SubData["CustomerAuth"]["Otp"]["RequestId"]=buff1;//"TLR1032_2620191138138";
    SubData["CustomerAuth"]["Otp"]["MethodId"]="2";
    SubData["CustomerAuth"]["Otp"]["SessionId"]= buff_tmp.toAscii().data();//"d12be6fa-9641-47aa-b2f6-7227c8c4983d";
    SubData["CustomerAuth"]["Otp"]["TellerID"]= (char *)username; // "TLR1032"; // Dhiral
    SubData["CustomerAuth"]["Otp"]["IsEncrypt"] = false ;
    SubData["CustomerAuth"]["Otp"]["X_Auth_Token"] ="";
    SubData["CustomerAuth"]["Otp"]["TokenId"] ="";
    SubData["CustomerAuth"]["Otp"]["SessionExpiryTime"]=(char *)configLoginLeg2.ucSessionExpiryTime;//"";




    SubData["CustomerAuth"]["Otp"]["RequestData"]["MethodId"] ="2";
    SubData["CustomerAuth"]["Otp"]["RequestData"]["RequestId"] =OtpImpsNeft.Commdata_OIN.ucRequestID_res;//"161281";
    SubData["CustomerAuth"]["Otp"]["RequestData"]["CustomerMobileNo"] = "8291173892"; // CustDet.cMobileNumber;//"8268769453"; // Dhiral
    SubData["CustomerAuth"]["Otp"]["RequestData"]["OtpPin"] = CustDet.cOtp;// "9272";
    SubData["CustomerAuth"]["Otp"]["RequestData"]["MessageId"] = "39";
    SubData["CustomerAuth"]["Otp"]["RequestData"]["OtpParam"] = "";

    /////////////////////Fp_Auth/////////////////////////////////

    SubData["CustomerAuth"]["Aadhaar"]["data"]="";

    SubData["CustomerAuth"]["Fp_Auth"]["RequestId"]=buff1;//"TLR1032_2620191138138";
    SubData["CustomerAuth"]["Fp_Auth"]["MethodId"]="1";
    SubData["CustomerAuth"]["Fp_Auth"]["SessionId"]=(char *)configLoginLeg2.ucSessionId;//"d12be6fa-9641-47aa-b2f6-7227c8c4983d";
    SubData["CustomerAuth"]["Fp_Auth"]["TokenId"]="";
    SubData["CustomerAuth"]["Fp_Auth"]["TellerID"] ="TLR1032";
    SubData["CustomerAuth"]["Fp_Auth"]["X_Auth_Token"]="";
    SubData["CustomerAuth"]["Fp_Auth"]["IsEncrypt"]=false;
    SubData["CustomerAuth"]["Fp_Auth"]["SessionExpiryTime"]="";


    SubData["CustomerAuth"]["Fp_Auth"]["RequestData"]["RRN"] = "";//TLR1032_10112018202736391";
    SubData["CustomerAuth"]["Fp_Auth"]["RequestData"]["CustomerID"] =username;"100015623";
    SubData["CustomerAuth"]["Fp_Auth"]["RequestData"]["FingerID"] ="1";
    SubData["CustomerAuth"]["Fp_Auth"]["RequestData"]["VerifyTemplate"] ="";


    ///////////////////////////////CreateCustomer/////////////////////////
    CreateCustomerArraylist.clear();

    CreateCustomer["employment"] = "";
    CreateCustomer["idProof"] = "";
    CreateCustomer["residencyCode"] = "1";
    CreateCustomer["customerType"] = "0";
    CreateCustomer["currencyCode"] = "INR";
    CreateCustomer["CreateAccount"] = "{}";
    CreateCustomer["personalDetails"]["title"] = ui->cmbTitle->currentText().trimmed().toAscii().data();
    CreateCustomer["personalDetails"]["name"] = CustDet.cCustName;
    CreateCustomer["personalDetails"]["firstNm"] = CustDet.cCustName;// "Thirupathi";

    CreateCustomer["personalDetails"]["gender"] = CustDet.cGender;
    CreateCustomer["personalDetails"]["code"] = "1";
    CreateCustomer["personalDetails"]["branchOwnership"] = "1011";

    CreateCustomer["personalDetails"]["aadharNumber"] = "";
    CreateCustomer["personalDetails"]["taxId"] = "";
    CreateCustomer["personalDetails"]["accountNm"] = "";

    CreateCustomer["personalDetails"]["customerNm"] = "";
    if(strcmp(CustDet.cGSTINNumber, "") == 0)
        CreateCustomer["personalDetails"]["cortexId"] =  CreateCustomer.null;
    else
        CreateCustomer["personalDetails"]["cortexId"] =  (char *)CustDet.cGSTINNumber;
  //  CreateCustomer["personalDetails"]["cortexId"] = "";
    CreateCustomer["personalDetails"]["photo"] = "";

    CreateCustomer["contactDetails"]["mobile"] = CustDet.cMobileNumber;
    CreateCustomer["contactDetails"]["email"] = "";
    CreateCustomer["contactDetails"]["photo"] = "";

    SubData2.clear();
    addressesArraylist.clear();
    for(int i=0;i<1;i++)
    {

        SubData2["addressType"] = "P";
        SubData2["city"] =CustDet.cCity;
        SubData2["country"] = "IN";
        SubData2["mobile"] = CustDet.cMobileNumber;

        addressesArraylist.append(SubData2);
        SubData2.clear();
    }
    CreateCustomer["addresses"] = addressesArraylist;

    CreateCustomerArraylist.append(CreateCustomer);


    SubData["CreateCustomer"] = CreateCustomerArraylist;


    ///////////////////////////////////AddBeneficiary/////////////////////////////////////////////

    SubData["AddBeneficiary"]["appId"] ="FINOMER";
    SubData["AddBeneficiary"]["customerNumber"] =username;
    SubData["AddBeneficiary"]["beneficiaryType"] ="3";
    SubData["AddBeneficiary"]["nickName"] ="";
    SubData["AddBeneficiary"]["beneficiaryAccount"] =CustDet.cAccountNumber;//"96786787687";
    SubData["AddBeneficiary"]["beneficiaryAccountType"] ="10";
    SubData["AddBeneficiary"]["beneficiaryName"] =CustDet.cCustName;//"webtset";
    SubData["AddBeneficiary"]["beneficiaryBank"] =CustDet.cBankName;//"HDFCBANKLTD";
    SubData["AddBeneficiary"]["beneficiaryBankIfsc"] =CustDet.cIFSC_Code;//"HDFC0000240";
    SubData["AddBeneficiary"]["beneficiaryAddress1"] = CustDet.cAddress;

    SubData["AddBeneficiary"]["beneficiaryState"] = CustDet.cstate;
    SubData["AddBeneficiary"]["beneficiaryCity"] = CustDet.cCity;
    SubData["AddBeneficiary"]["beneficiaryZip"] ="";
    SubData["AddBeneficiary"]["beneficiaryEmailId"] ="";
    SubData["AddBeneficiary"]["beneficiaryMobileNumber"] ="";
    SubData["AddBeneficiary"]["beneficiaryBankMicr"] ="";
    SubData["AddBeneficiary"]["beneficiaryMaxLimit"] ="";


    serialize1(Convertstr(SubData.toStyledString()));
    RequestedData["RequestData"] = buff_tmp.toAscii().data();


    char *Ret;
    Ret=Convertstr(RequestedData.toStyledString());



#if DEBUG
    qDebug()<<"\nRequest Data :"<<Ret;
#endif
    memset(gSendData,0,sizeof(gSendData));
    strncpy((char*)gSendData, Ret, strlen(Ret));
 #if DEBUG
    printf("gSendData = %s\n",gSendData);
 #endif

    // serialize(Ret);
    // Need to check Method
    system("rm -rf /usr/FINO/FPB/RESP/ADD_Bene.TXT");

    memset(str_url,0,sizeof(str_url));
#ifdef PRODUCTION
    strcpy(str_url,URL_CREATION(LOGIN_URL_PROD));
#else
    strcpy(str_url,URL_CREATION((char *)COMMON_URL));
#endif

    system("rm /usr/FINO/FPB/RESP/ADD_Bene.TXT");
    int retk = file_download(str_url, (char *)gSendData,(char *)"/usr/FINO/FPB/RESP/ADD_Bene.TXT",120);
    if(retk!=0)
    {
        sleep(1);
        uidmessage::instance()->hide_Message();
        return FUNCTION_FAILED;
    }


 //   file_download( "http://103.1.112.205:4444//UIService.svc/NonFinancialProcessRequest", jsondata,"/usr/FINO/FPB/RESP/ADD_Bene.TXT",120);
    iRet = ADD_Bene_Parsing();
    return iRet;

}

int FPB::IMPSCharges()
{
    char* jsondata = NULL;
    char buff1[100];
    char cResChargeAmt[8+1];
    int iRet = 0;
    int TotalAmt = 0;
    int OrgAmt = 0;
    int charges = 0;
    memset(buff1,0,sizeof(buff1));
    memset(cResChargeAmt,0,sizeof(cResChargeAmt));

    uidmessage::instance()->hide_Message();
    uidmessage::instance()->Display_BusyMessage("Please Wait...");

    Json1::Value ArrayList(Json1::arrayValue),PostTransactionArrayList(Json1::arrayValue);
    Json1::Value acctFundTransfer, PostTransaction, IMPSServiceDataList;
    Json1::Value RequestedData,mainreq;

    ArrayList.clear();
    PostTransactionArrayList.clear();
    acctFundTransfer.clear();
    PostTransaction.clear();
    RequestedData.clear();
    mainreq.clear();


    iGenerateRequestID((char *)username,(unsigned char *)buff1);

    //--------------------------------  SUPPORT DATA  -------------------------------------
    char SupportAmt[20+1] ={0};
    char IMPS_SupportData[1000], IMPS_SupportData_Debit[1000];

    Clear_SupportData();
    Clear_DeviceInfo();

    int iretDI = DeviceInfo();

    if(iretDI != FUNCTION_SUCCESS)
        return FUNCTION_FAILED;

    //    char itimedate[20];
    //    memset(itimedate, 0, sizeof(itimedate));
    //    strcpy(itimedate,iTimestamp());
    //    printf("Time Stamp == %s\n",itimedate);

    //    QDate date = QDate::currentDate();
    //    QString cur_date = date.toString("dd/MM/yy");

    memcpy(postbeneverif.sd_SPROD_Debit, "1", 1);
    memcpy(postbeneverif.sd_SPROD, "5", 1);
    memcpy(postbeneverif.sd_PVTXNID, (char *)sd_deviceinfo.ucSupportData_PVTXNID_req, strlen((char *)sd_deviceinfo.ucSupportData_PVTXNID_req));
    memcpy(postbeneverif.sd_PVTXNDT, (char *)sd_deviceinfo.ucSupportData_PVTXNDT_req, strlen((char *)sd_deviceinfo.ucSupportData_PVTXNDT_req)); // cur_date.toAscii().data(), strlen(cur_date.toAscii().data()));
    memcpy(postbeneverif.sd_IPADDR, (char *)sd_deviceinfo.ucSupportData_IPADDR_req, strlen((char *)sd_deviceinfo.ucSupportData_IPADDR_req));
    memcpy(postbeneverif.sd_DEVICEID, (char *)sd_deviceinfo.ucSupportData_DEVICEID_req, strlen((char *)sd_deviceinfo.ucSupportData_DEVICEID_req));
    memcpy(postbeneverif.sd_APPID, CLIENT_ID, strlen(CLIENT_ID));
    memcpy(postbeneverif.sd_AUTHID, "@authid", 7);
    memcpy(postbeneverif.sd_LOCATION, "", 1);
    memcpy(postbeneverif.sd_CELLID, (char *)sd_deviceinfo.ucSupportData_CELLID_req, strlen((char *)sd_deviceinfo.ucSupportData_CELLID_req));
    memcpy(postbeneverif.sd_MCC, (char *)sd_deviceinfo.ucSupportData_MCC_req, strlen((char *)sd_deviceinfo.ucSupportData_MCC_req));
    memcpy(postbeneverif.sd_RPTFLG, "0", 1);
    memcpy(postbeneverif.sd_PRTXNID, (char *)sd_deviceinfo.ucSupportData_PRTXNID_req, strlen((char *)sd_deviceinfo.ucSupportData_PRTXNID_req));
    memcpy(postbeneverif.sd_PRTXNAMT, (char *)CustDet.cTxnAmount, strlen((const char*)CustDet.cTxnAmount));
    memcpy(postbeneverif.sd_SPLTSEQ, (char *)sd_deviceinfo.ucSupportData_SPLTSEQ_req, strlen((char *)sd_deviceinfo.ucSupportData_SPLTSEQ_req));
    memcpy(postbeneverif.sd_CHGAMT, (char *)sd_deviceinfo.ucSupportData_CHGAMT_req, strlen((char *)sd_deviceinfo.ucSupportData_CHGAMT_req));
    memcpy(postbeneverif.sd_ZRFUT1, (char *) ui->Bene_acc_no_edit->text().trimmed().toAscii().data(), strlen((char *) ui->Bene_acc_no_edit->text().trimmed().toAscii().data()));
    memcpy(postbeneverif.sd_ZRFUT2, (char*)ui->IFSC_edit->text().toAscii().trimmed().data(), strlen(ui->IFSC_edit->text().toAscii().trimmed().data()));
    memcpy(postbeneverif.sd_ZRFUT3, (char *)ui->cmb_bankList->currentText().trimmed().toAscii().data(), strlen((char *)ui->cmb_bankList->currentText().trimmed().toAscii().data()));

    if(flag_bc_merchant_account == 'B')
        memcpy(postbeneverif.sd_ZRFUT4, "EMD", 3);
    else
        memcpy(postbeneverif.sd_ZRFUT4, "", 1);

    memcpy(postbeneverif.sd_ZRFUT5,  (char *)ui->Bene_Name_edit->text().trimmed().toAscii().data(), strlen( (char *)ui->Bene_Name_edit->text().trimmed().toAscii().data()));
    memcpy(postbeneverif.sd_ZRFUT6, "", 1);
    memcpy(postbeneverif.sd_ZRFUT7, "", 1);
    memcpy(postbeneverif.sd_ZRFUT8, "", 1);
    memcpy(postbeneverif.sd_ZRFUT9, (char *)buff1, strlen((char *)buff1));
    memcpy(postbeneverif.sd_ZRFUN2, "0", 1);
    memcpy(postbeneverif.sd_ZRFUN3, username, strlen(username));
    memcpy(postbeneverif.sd_ZRFUN4, (char *)CustDet.cMobileNumber, strlen( (char *)CustDet.cMobileNumber));
    memcpy(postbeneverif.sd_NETID, "900000", 6);
    memcpy(postbeneverif.sd_MSGTYP, "210", 3);


    memset(SupportAmt,0,sizeof(SupportAmt));
    memset(IMPS_SupportData, 0, sizeof(IMPS_SupportData));
    memset(IMPS_SupportData_Debit, 0, sizeof(IMPS_SupportData_Debit));

    //    sprintf(SupportAmt,"DN1#%s",CustDet.cTxnAmount);
    //    sprintf(IMPS_SupportData,    "SPROD#%s~PVTXNID#%s~PVTXNDT#%s~IPADDR#%s~DEVICEID#%s~APPID#%s~AUTHID#%s~LOCATION#%s~CELLID#%s~MCC#%s~RPTFLG#%s~PRTXNID#%s~PRTXNAMT#%s~SPLTSEQ#%s~CHGAMT#%s~ZRFUN3#%s~ZRFUN4#%s~ZRFUT5#%s~ZRFUT8#%s~ZRFUT9#%s",postbeneverif.sd_SPROD,postbeneverif.sd_PVTXNID,postbeneverif.sd_PVTXNDT,postbeneverif.sd_IPADDR,postbeneverif.sd_DEVICEID,postbeneverif.sd_APPID,postbeneverif.sd_AUTHID,postbeneverif.sd_LOCATION,postbeneverif.sd_CELLID,postbeneverif.sd_MCC,postbeneverif.sd_RPTFLG,postbeneverif.sd_PRTXNID,postbeneverif.sd_PRTXNAMT,postbeneverif.sd_SPLTSEQ,postbeneverif.sd_CHGAMT_CREDIT,postbeneverif.sd_ZRFUN3,postbeneverif.sd_ZRFUN4,postbeneverif.sd_ZRFUT5,postbeneverif.sd_ZRFUT8,postbeneverif.sd_ZRFUT9); //"1",itimedate,"192.168.43.149","192.168.43.149","4888CAFA4374","FINOMER","@authid","","19.062504,73.0198985","0,0,0,0","0",itimedate,"100","0","11.8","20016200561", "HDFC0000406", "HDFC BANK LTD.", "", "Saurabh Ranjane", username, "9913384086", "900000", "210");
    sprintf(IMPS_SupportData_Debit,"SPROD#%s~PVTXNID#%s~PVTXNDT#%s~IPADDR#%s~DEVICEID#%s~APPID#%s~AUTHID#%s~LOCATION#%s~CELLID#%s~MCC#%s~RPTFLG#%s~PRTXNID#%s~PRTXNAMT#%s~SPLTSEQ#%s~CHGAMT#%s~ZRFUT1#%s~ZRFUT2#%s~ZRFUT3#%s~ZRFUT4#%s~ZRFUT5#%s~ZRFUN3#%s~ZRFUN4#%s~NETID#%s~MSGTYP#%s~ZRFUT8#%s~ZRFUT9#%s",postbeneverif.sd_SPROD_Debit,postbeneverif.sd_PVTXNID,postbeneverif.sd_PVTXNDT,postbeneverif.sd_IPADDR,postbeneverif.sd_DEVICEID,postbeneverif.sd_APPID,postbeneverif.sd_AUTHID,postbeneverif.sd_LOCATION,postbeneverif.sd_CELLID,postbeneverif.sd_MCC,postbeneverif.sd_RPTFLG,postbeneverif.sd_PRTXNID,postbeneverif.sd_PRTXNAMT,postbeneverif.sd_SPLTSEQ,postbeneverif.sd_CHGAMT,postbeneverif.sd_ZRFUT1,postbeneverif.sd_ZRFUT2,postbeneverif.sd_ZRFUT3,postbeneverif.sd_ZRFUT4,postbeneverif.sd_ZRFUT5,postbeneverif.sd_ZRFUN3,postbeneverif.sd_ZRFUN4,postbeneverif.sd_NETID,postbeneverif.sd_MSGTYP,postbeneverif.sd_ZRFUT8,postbeneverif.sd_ZRFUT9);


    //-------------------------------------------------------------------------------------------


    mainreq["RequestId"] =  buff1;//"TLR1032_10112018202736391";
    mainreq["MethodId"] = 104;
    mainreq["SessionId"] =  (char *)configLoginLeg2.ucSessionId ;//buff_tmp.toAscii().data();//"9d044213-09d9-400d-aa96-5e9a473ba494";
    mainreq["TellerID"]= (char *)username;
    mainreq["TokenId"]= "";
    mainreq["X_Auth_Token"] ="";

    if(flag_bc_merchant_account == 'B')
        mainreq["MerchantContribution"] = "0";
    else
        mainreq["MerchantContribution"] = CustDet.cTxnAmount;

    mainreq["Charges"]   = "0.0";
    mainreq["ChannelID"] = CHANNEL_ID_IMPS;



    if(flag_bc_merchant_account == 'B')
        mainreq["ProductCode"]= "1003|N";
    else
        mainreq["ProductCode"]= "9999|Y";

    mainreq["Amount"]= CustDet.cTxnAmount;//"5075";
    mainreq["IsEncrypt"]= ISENCEYPT;
    mainreq["SessionExpiryTime"]= (char *)configLoginLeg2.ucSessionExpiryTime;//"10/11/2018 11:58:44 PM";

    /////////////RequestData/////////////////////////////////////

    RequestedData["requestbitmap"] = "00000100";
    RequestedData["asyncFlag"] = "Y";
    RequestedData["totalAmount"] = CustDet.cTxnAmount;
    RequestedData["mobileNumber"] = CustDet.cMobileNumber;

    RequestedData["merchantMobileNumber"] = (char *)configLoginLeg2.ucmobileNumber;//"8055988351";
    RequestedData["txnType"] = CustDet.cTxnType;//"IMPS";

    RequestedData["cashoraccount"] = "CASH";
    //    RequestedData["parentRRN"] = (char *)configLoginLeg2.ucRRN_req;       // 07052019
    RequestedData["impsserviceflag"] = "Y";
    RequestedData["CreateAccount"] = "";


    //-----------------------------------------------             AddBeneficiary             --------------------------------------------------
    //    if(Bitmap.mid(3,1)=="1")
    //    {

    RequestedData["AddBeneficiary"]["appId"] = CLIENT_ID;
    RequestedData["AddBeneficiary"]["customerNumber"] =  (char *)FC_CustomerNumber;
    RequestedData["AddBeneficiary"]["beneficiaryType"] ="3";
    if(flag_bc_merchant_account == 'B')
        RequestedData["AddBeneficiary"]["nickName"] = "EMD";
    else
        RequestedData["AddBeneficiary"]["nickName"] ="";
    RequestedData["AddBeneficiary"]["beneficiaryAccount"] = (char *) ui->Bene_acc_no_edit->text().trimmed().toAscii().data(); // 07052019
    RequestedData["AddBeneficiary"]["beneficiaryAccountType"] ="10";
    RequestedData["AddBeneficiary"]["beneficiaryName"] = (char *)ui->Bene_Name_edit->text().trimmed().toAscii().data();       // 07052019
    RequestedData["AddBeneficiary"]["beneficiaryBank"] = (char *)ui->cmb_bankList->currentText().trimmed().toAscii().data();  // 07052019
    RequestedData["AddBeneficiary"]["beneficiaryBankCity"] = CustDet.cCity;
    RequestedData["AddBeneficiary"]["beneficiaryBankBranch"] = CustDet.cBranchName;
    RequestedData["AddBeneficiary"]["beneficiaryBankIfsc"] = (char*)ui->IFSC_edit->text().toAscii().trimmed().data();
    RequestedData["AddBeneficiary"]["beneficiaryAddress1"] = "";
    RequestedData["AddBeneficiary"]["beneficiaryState"] = CustDet.cstate;
    RequestedData["AddBeneficiary"]["beneficiaryCity"] = CustDet.cCity;
    RequestedData["AddBeneficiary"]["beneficiaryZip"] = "";
    RequestedData["AddBeneficiary"]["beneficiaryEmailId"] = "";
    RequestedData["AddBeneficiary"]["beneficiaryBankMicr"] = "";
    RequestedData["AddBeneficiary"]["beneficiaryMaxLimit"] = "";

    //    }
    //    else
    //    {
    //        RequestedData["AddBeneficiary"] = "";
    //    }


    RequestedData["AddWalkinCustomer"] = "";        // 07052019
    RequestedData["AddExternalAccount"] = "";
    RequestedData["CreateAccount"] = "";

    //CustomerAuth
    RequestedData["CustomerAuth"]["MobileNo"] = CustDet.cMobileNumber;
    RequestedData["CustomerAuth"]["AadharNo"] = "";
    RequestedData["CustomerAuth"]["Auth_id"] = 0;       // 07052019

    //Fp_Auth
    RequestedData["CustomerAuth"]["Fp_Auth"] = "";
    /*    RequestedData["CustomerAuth"]["Fp_Auth"]["MethodId"] =1;
     RequestedData["CustomerAuth"]["Fp_Auth"]["TellerID"] ="11";
     RequestedData["CustomerAuth"]["Fp_Auth"]["SessionId"] =(char *)configLoginLeg2.ucSessionId ;//buff_tmp.toAscii().data();;//"9d044213-09d9-400d-aa96-5e9a473ba494";
     RequestedData["CustomerAuth"]["Fp_Auth"]["TokenId"] ="";
     RequestedData["CustomerAuth"]["Fp_Auth"]["IsEncrypt"] =false;
     RequestedData["CustomerAuth"]["Fp_Auth"]["X_Auth_Token"] ="";
     RequestedData["CustomerAuth"]["Fp_Auth"]["SessionExpiryTime"] =(char *)configLoginLeg2.ucSessionExpiryTime;;//"10/11/201811: 58: 44PM";
     RequestedData["CustomerAuth"]["Fp_Auth"]["RequestData"]="";*/


    //otp
    RequestedData["CustomerAuth"]["Otp"] = "";      // 07052019
    /*
    RequestedData["CustomerAuth"]["Otp"]["RequestId"] =buff1;//"TLR1032_10112018202736391";
    RequestedData["CustomerAuth"]["Otp"]["MethodId"] =1;
    RequestedData["CustomerAuth"]["Otp"]["TellerID"] =username;//"TLR1032";
    RequestedData["CustomerAuth"]["Otp"]["SessionId"] =(char *)configLoginLeg2.ucSessionId ;//"9d044213-09d9-400d-aa96-5e9a473ba494";
    RequestedData["CustomerAuth"]["Otp"]["TokenId"] ="";
    RequestedData["CustomerAuth"]["Otp"]["IsEncrypt"] =false;
    RequestedData["CustomerAuth"]["Otp"]["X_Auth_Token"] ="";
    RequestedData["CustomerAuth"]["Otp"]["SessionExpiryTime"] =(char *)configLoginLeg2.ucSessionExpiryTime;//"10/11/201811: 58: 44PM";
    RequestedData["CustomerAuth"]["Otp"]["RequestData"] ="";
    */      // 07052019
    //Aadhaar

    RequestedData["CustomerAuth"]["Aadhaar"] = "";
    //PostTransaction
    PostTransactionArrayList.clear();
    PostTransaction["appId"] = CLIENT_ID;
    PostTransaction["valueDate"] ="";
    PostTransaction["isInclusive"] = 0;
    PostTransaction["isClubbed"] = 0;       // 07052019
    PostTransaction["analysisFlag"] = 1;
    PostTransaction["reversalFlag"] ="";
    PostTransaction["referenceNo"] = buff1;//;configLoginLeg2.ucRRN_req;//"TLR1032_26201913202573";

    ArrayList.clear();
    //     for(int i=0;i<2;i++)
    //     {

    if(flag_bc_merchant_account == 'B')
        acctFundTransfer["accountNumber"] = (char *)CustDet.cFindCustAccountNo;
    else
        acctFundTransfer["accountNumber"] = "";

    acctFundTransfer["amount"] = CustDet.cTxnAmount;//"5075";
    acctFundTransfer["currency"] = "INR";
    acctFundTransfer["creditDebitFlag"] = "D";
    if(flag_bc_merchant_account == 'B')
        acctFundTransfer["transactionType"] = "IMPSFTP2A";
    else
        acctFundTransfer["transactionType"] = "DMTIMPSP2A";
    acctFundTransfer["transactionComment"] = "Calculate Charges IMPS";
    acctFundTransfer["costCenter"] = 1011;
    acctFundTransfer["supportData"] = IMPS_SupportData_Debit;//"DN1#5075";
    acctFundTransfer["beneficiaryRefOrMmid"] = "";
    acctFundTransfer["beneficiaryMobile"] = "";
    acctFundTransfer["remitterMobile"] = (char *) CustDet.cMobileNumber; // configLoginLeg2.ucmobileNumber;
    acctFundTransfer["remitterMmid"] = "";
    acctFundTransfer["beneficiaryAccountNo"] = CustDet.cAccountNumber;//"";
    acctFundTransfer["beneficiaryIfsc"] = CustDet.cIFSC_Code;//"";
    acctFundTransfer["remarks"] = CustDet.cRemark;//"remarks";
    ArrayList.append(acctFundTransfer);

    acctFundTransfer["accountNumber"] = "";
    acctFundTransfer["amount"] = CustDet.cTxnAmount;//"5075";
    acctFundTransfer["currency"] = "INR";
    acctFundTransfer["creditDebitFlag"] = "C";
    if(flag_bc_merchant_account == 'B')
        acctFundTransfer["transactionType"] = "IMPSFTP2A";
    else
        acctFundTransfer["transactionType"] = "DMTIMPSP2A";
    acctFundTransfer["transactionComment"] = "Calculate Charges IMPS";
    acctFundTransfer["costCenter"] = 1011;
    acctFundTransfer["supportData"] = "";
    acctFundTransfer["beneficiaryRefOrMmid"] = "";
    acctFundTransfer["beneficiaryMobile"] = "";
    acctFundTransfer["remitterMobile"] = ""; // (char *)configLoginLeg2.ucmobileNumber;
    acctFundTransfer["remitterMmid"] = "";
    acctFundTransfer["beneficiaryAccountNo"] = ""; // CustDet.cAccountNumber;//"";
    acctFundTransfer["beneficiaryIfsc"] = "";
    acctFundTransfer["remarks"] = CustDet.cRemark;//"remarks";
    ArrayList.append(acctFundTransfer);

    //  }

    PostTransaction["acctFundTransferLegs"] = ArrayList;
    PostTransactionArrayList.append(PostTransaction);
    RequestedData["PostTransaction"] = PostTransactionArrayList;


    //-----------------------------------------------           IMPSServiceDataList          --------------------------------------------------
    ArrayList.clear();
    IMPSServiceDataList["BeneAccNo"] = (char *) CustDet.cAccountNumber;
    IMPSServiceDataList["BeneIFSC"] = (char*) CustDet.cIFSC_Code;
    IMPSServiceDataList["BeneName"] = (char *)ui->Bene_Name_edit->text().trimmed().toAscii().data();
    IMPSServiceDataList["MerchantMobileNo"] = (char *) (char *)configLoginLeg2.ucmobileNumber;
    IMPSServiceDataList["PaymentRef"] = buff1;
    IMPSServiceDataList["RemeMobileNo"] = (char *) CustDet.cMobileNumber;
    IMPSServiceDataList["RemeName"] = (char *)CustDet.cCustName;
    IMPSServiceDataList["TxnAmount"] =  (char*)CustDet.cTxnAmount;
    ArrayList.append(IMPSServiceDataList);

    RequestedData["IMPSServiceDataList"] = ArrayList;

    //-----------------------------------------------------------------------------------------------------------------------------------------



    serialize1(Convertstr(RequestedData.toStyledString()));

    mainreq["RequestData"] = buff_tmp.toAscii().data();

    jsondata = Convertstr(mainreq.toStyledString());
#if DEBUG
    qDebug()<<"------------->"<<jsondata;
#endif
    memset(gSendData,0,sizeof(gSendData));
    strncpy((char*)gSendData, jsondata, strlen(jsondata));
    /****************************Encryption function*********************************/
    if(!strcmp(ISENCEYPT,"true"))
    {
        int gSendData_len =strlen((char *)gSendData);
        RequestDetachEncryptAttach((char *)gSendData,1,gSendData_len);
    }
    /***************************************************************/

    //Need to check method for posting

    memset(str_url,0,sizeof(str_url));
#ifdef PRODUCTION
    strcpy(str_url,URL_CREATION(LOGIN_URL_PROD));
#else
    strcpy(str_url,URL_CREATION((char *)COMMON_URL));
#endif

    system("rm /usr/FINO/FPB/RESP/IMPS_Charges.TXT");
    int retk = file_download(str_url, (char *)gSendData,(char *)"/usr/FINO/FPB/RESP/IMPS_Charges.TXT",120);
    if(retk!=0)
    {
        sleep(1);
        uidmessage::instance()->hide_Message();
        return FUNCTION_FAILED;
    }


  //  iRet = file_download( "http://103.1.112.205:4444//UIService.svc/NonFinancialProcessRequest",(char *)gSendData,"/usr/FINO/FPB/RESP/IMPS_Charges.TXT",120);

    /****************************Decryption function*********************************/
    if(!strcmp(ISENCEYPT,"true"))
    {
        int oBuff_len = strlen(oBuff);
#if DEBUG
        qDebug()<<"Length of buffer"<<oBuff_len;
        printf("Buffer%s\n",oBuff);
#endif
        ResponseDetachEncryptAttach(oBuff,2,oBuff_len);//TEMPORARY

        system("rm /usr/FINO/FPB/RESP/IMPS_Charges.TXT");
        FILE *fp2 = NULL;
        fp2 =fopen("/usr/FINO/FPB/RESP/IMPS_Charges.TXT","a");
        fwrite(oBuff,strlen((char *)oBuff),1,fp2);
        sleep(1);
        fclose(fp2);
    }
    /*******************************************************************************/


    iRet = IMPS_Charges_parsing(cResChargeAmt);
    if(iRet == FUNC_OK || iRet == FUNCTION_SUCCESS)
    {

        QString Charges,Total_Amount,Cashcontri;
        Charges.clear();
        Cashcontri.clear();
        Total_Amount.clear();

        memset(CustDet.cTxnCahrges, 0, sizeof(CustDet.cTxnCahrges));

        TotalAmt = 0;
        OrgAmt = 0;
        charges = 0;
        OrgAmt = atoi(CustDet.cTxnAmount);
        charges = atoi(cResChargeAmt);
        CustDet.cChargesAmount = 0;
        // CustDet.CTotalAmount   = 0; // 12062019
        CustDet.cChargesAmount = charges;
        TotalAmt = OrgAmt + charges;
        memset(CustDet.CTotalAmount, 0, sizeof(CustDet.CTotalAmount)); // 12062019
        sprintf(CustDet.CTotalAmount, "%d", TotalAmt); // 12062019
#if DEBUG
        printf("CustDet.CTotalAmount = %s\n", CustDet.CTotalAmount); // 12062019
#endif
        // CustDet.CTotalAmount = TotalAmt; // 12062019
        ui->lbl_TotalAmount->clear();


        bool decimal;
        Charges = QString::number(charges);
        Total_Amount = QString::number(TotalAmt);
#if DEBUG
        qDebug()<<"Total_Amount 1 == "<<Total_Amount;
#endif
        decimal = Charges.contains('.');
        if(decimal == false)
        {
            Charges = Charges.append(".00");
        }

        memset(display_imps.ucDispCharges, 0, sizeof(display_imps.ucDispCharges));
        memcpy(display_imps.ucDispCharges, (char *)Charges.trimmed().toAscii().data(), strlen((const char*)Charges.trimmed().toAscii().data()));

        memcpy(CustDet.cTxnCahrges, (char *)Charges.trimmed().toAscii().data(), strlen((const char*)Charges.trimmed().toAscii().data()));

        memset((char *)resp_postimps.ucRDCharges_RES, 0, sizeof((char *)resp_postimps.ucRDCharges_RES));
        memcpy(resp_postimps.ucRDCharges_RES, (char *)Charges.trimmed().toAscii().data(), strlen((const char*)Charges.trimmed().toAscii().data()));


        decimal = Total_Amount.contains('.');
        if(decimal == false)
        {
#if DEBUG
            qDebug()<<"Total_Amount 2 == "<<Total_Amount;
#endif
            Total_Amount = Total_Amount.append(".00");
            Cashcontri = Total_Amount; // .append(".00");
        }
#if DEBUG
        qDebug()<<"Total_Amount 3 == "<<Total_Amount;
#endif
        ui->Cash_contri_label->setVisible(true);
        ui->lbl_CashContribution->setVisible(true);
        ui->lbl_CashContribution->clear();
        // ui->Calculate_charge_button->setFont(QFont("SansSerif",10));
        ui->Calculate_charge_button->setText("CHARGES = "+Charges);
        ui->Calculate_charge_button->setEnabled(false);
        ui->Calculate_charge_button->setStyleSheet("background-color: rgb(85, 85, 127); color: rgb(255, 255, 255)");
        ui->lbl_TotalAmount->setText(Total_Amount);
        ui->lbl_CashContribution->setText(Total_Amount);
        Calculate_impscharges = 1;
    }
    else
    {
        ui->lbl_TotalAmount->clear();
        ui->lbl_CashContribution->clear();
    }
    return iRet;
}


int FPB::IMPS_Charges_parsing(char *cResChargeAmt) // Prod_04092019
{
    display_imps.Flag_SGST_CGST = 0; // Prod_04092019
    char cResCode[10+1];
    char val[12+1];
    memset(cResCode, 0, sizeof(cResCode));
    memset(val, 0, sizeof(val));

    system("sed 's/\"ResponseData\":\"{/\"ResponseData\":{/g' /usr/FINO/FPB/RESP/IMPS_Charges.TXT > /tmp/1.txt");
    system("sed 's/}\",\"MessageId\"/},\"MessageId\"/g' /tmp/1.txt > /tmp/2.txt  ");
    system(" sed 's/\\\\//g' /tmp/2.txt > /usr/FINO/FPB/RESP/IMPS_Charges.TXT");

    Json1::Value root;
    root.clear();
    //QSqlQuery query;
    Json1::Reader reader;
    std::ifstream ReadData("/usr/FINO/FPB/RESP/IMPS_Charges.TXT");
    bool parsedSuccess = reader.parse(ReadData,root,false);

    ReadData.close();

    if(not parsedSuccess)
    {
        qDebug()<<"FpsMonthlyPdsData File not Found";
        return FUNCTION_FAILED;
    }
    else
    {
#if DEBUG
        qDebug()<<"RequestId   : "<<Convertstr(root["RequestId"].asString());
        qDebug()<<"DisplayMessage : "<<Convertstr(root["DisplayMessage"].asString());
        qDebug()<<"ResponseMessage    : "<<Convertstr(root["ResponseMessage"].asString());
        qDebug()<<"ResponseCode : "<<Convertstr(root["ResponseCode"].asString());
        qDebug()<<"cResCode : "<<cResCode;
        qDebug()<<"MessageType : "<<Convertstr(root["MessageType"].asString());
#endif
        //qDebug()<<"RespocountNo : "<<Convertstr(root["RespocountNo"].asString());
        /*       strcpy(Statement.ucRequestId,Convertstr(root["RequestId"].asString()));
        strcpy(Statement.ucDisplayMessage,Convertstr(root["DisplayMessage"].asString()));
        strcpy(Statement.ucResponseMessage,Convertstr(root["ResponseMessage"].asString()));
        strcpy(Statement.ucResponseCode,Convertstr(root["ResponseCode"].asString()));*/
        strcpy(cResCode,Convertstr(root["ResponseCode"].asString()));

        memset(cal_charges_resp.Commondata_CalCharges_RES.ucResponseMessage_resp, 0, sizeof(cal_charges_resp.Commondata_CalCharges_RES.ucResponseMessage_resp));
        memcpy(cal_charges_resp.Commondata_CalCharges_RES.ucResponseMessage_resp, (const char*)Convertstr(root["ResponseMessage"].asString()), strlen((const char*)Convertstr(root["ResponseMessage"].asString())));

        if(memcmp(cResCode,"0",1)==0)
        {
            Json1::Value ResponseData;
            ResponseData.clear();
            ResponseData = root.get("ResponseData",0);
            printf("\nSize : %d\n",ResponseData.size());
            if(ResponseData.size()==0)
            {
                qDebug("\n1:Key Register Data Not Found\n");
                return FUNCTION_FAILED;
            }
            else
            {
                Json1::Value ResponseData1,ResponseData2;
                ResponseData2.clear();
                ResponseData1 = ResponseData.get("PostTransactionResponse",0);
#if DEBUG
                qDebug()<<"ResponseData1.size() == "<<ResponseData1.size();
                qDebug()<<"Getting successfull PostTransactionResponse";
#endif
                for(int j=0;j<ResponseData1.size();j++)
                {
#if DEBUG
                    qDebug()<<"returnCode:"<<Convertstr(ResponseData1[j]["returnCode"].asString());
#endif

                    strcpy(val,Convertstr(ResponseData1[j]["PostTransactionResponse"]["chargesList"].asString()));
                    //qDebug()<<"chargesList = "<<Convertstr(ResponseData1[j]["PostTransactionResponse"]["chargesList"].asString());
                    //qDebug()<<"code:"<<Convertstr(ResponseData1[j]["PostTransactionResponse"]["chargesList"]["code"].asString());
                    //if(memcmp(val,"chargesList",strlen("chargesList")))
                    //{

                    ResponseData2 = ResponseData1[j].get("chargesList",0);
                    printf("\nSize : %d\n",ResponseData2.size());
                    if(ResponseData2.size()==0)
                    {
                        if(flag_bc_merchant_account == 'B')
                        {
                            memset(cResChargeAmt,0,sizeof(cResChargeAmt));
                            memcpy(cResChargeAmt, CALCULATE_CHARGES, strlen(CALCULATE_CHARGES));
#if DEBUG
                            qDebug()<<"cResChargeAmt:"<<cResChargeAmt;
#endif
                            display_imps.Flag_SGST_CGST = 0; // Prod_04092019

                            memset((char *)resp_postimps.ucRDCharges_SGST_RES, 0, sizeof((char *)resp_postimps.ucRDCharges_SGST_RES)); // Prod_04092019
                            memcpy(resp_postimps.ucRDCharges_SGST_RES, SGST_CALCULATE_CHARGES,strlen((const char*)SGST_CALCULATE_CHARGES)); // Prod_04092019

                            memset(display_imps.ucDispCharges_SGST, 0, sizeof(display_imps.ucDispCharges_SGST)); // Prod_04092019
                            memcpy(display_imps.ucDispCharges_SGST, SGST_CALCULATE_CHARGES, strlen((const char*)SGST_CALCULATE_CHARGES)); // Prod_04092019

                            memset((char *)resp_postimps.ucRDCharges_CGST_RES, 0, sizeof((char *)resp_postimps.ucRDCharges_CGST_RES)); // Prod_04092019
                            memcpy(resp_postimps.ucRDCharges_CGST_RES, CGST_CALCULATE_CHARGES,strlen((const char*)CGST_CALCULATE_CHARGES)); // Prod_04092019


                            memset(display_imps.ucDispCharges_CGST, 0, sizeof(display_imps.ucDispCharges_CGST)); // Prod_04092019
                            memcpy(display_imps.ucDispCharges_CGST, CGST_CALCULATE_CHARGES,strlen((const char*)CGST_CALCULATE_CHARGES)); // Prod_04092019

                        }
                        else
                        {
                            qDebug("\n1:Key Register Data Not Found\n");
                            return FUNC_ERROR;
                        }
                    }
                    else
                    {
                        for(int i = 0;i<ResponseData2.size();i++)
                        {
                            memset(cResCode,0,sizeof(cResCode));
                            memset(cResChargeAmt,0,sizeof(cResChargeAmt));
                            strcpy(cResCode,Convertstr(ResponseData2[i]["code"].asString()));
#if DEBUG
                            qDebug()<<"code:"<<Convertstr(ResponseData2[i]["code"].asString());
                            qDebug()<<"amount:"<<Convertstr(ResponseData2[i]["amount"].asString());
                            qDebug()<<"description:"<<Convertstr(ResponseData2[i]["description"].asString());
                            qDebug()<<"chargeAcctNum:"<<Convertstr(ResponseData2[i]["chargeAcctNum"].asString());
                            printf("cResCode = %s\n",cResCode);
#endif
                            if(memcmp(cResCode,"CHGDMTIMPS",strlen("CHGDMTIMPS"))==0)
                            {
                                memcpy(cResChargeAmt,Convertstr(ResponseData2[i]["amount"].asString()),strlen(Convertstr(ResponseData2[i]["amount"].asString())));
#if DEBUG
                                qDebug()<<"cResChargeAmt:"<<cResChargeAmt;
#endif
                            }

                            if(memcmp(cResCode,"SGST",strlen("SGST"))==0) // Prod_04092019
                            {
                                display_imps.Flag_SGST_CGST = 1;
                                bool decimal;
                                QString bal = QString::number(ResponseData2[i]["amount"].asFloat()); // QString::fromUtf8((const char *)Convertstr(ResponseData1[k]["amount"].asString()));
                                decimal = bal.contains('.');
                                if(decimal == false)
                                {
                                    bal = bal.append(".00");
                                }
                                memset((char *)resp_postimps.ucRDCharges_SGST_RES, 0, sizeof((char *)resp_postimps.ucRDCharges_SGST_RES));
                                memcpy(resp_postimps.ucRDCharges_SGST_RES, bal.toLatin1().data(),strlen(bal.toLatin1().data()));

                                memset(display_imps.ucDispCharges_SGST, 0, sizeof(display_imps.ucDispCharges_SGST));
                                memcpy(display_imps.ucDispCharges_SGST, bal.toLatin1().data(), strlen((const char*)bal.toLatin1().data()));

#if DEBUG
                                printf("resp_postimps.ucRDCharges_SGST_RES = %s\n", (char *)resp_postimps.ucRDCharges_SGST_RES); // 02052019
                                printf("display_imps.ucDispCharges_SGST = %s\n", (char *)display_imps.ucDispCharges_SGST); // 02052019

#endif
                            }

                            if(memcmp(cResCode,"CGST",strlen("CGST"))==0) // Prod_04092019
                            {
                                display_imps.Flag_SGST_CGST = 1;
                                bool decimal;
                                QString bal = QString::number(ResponseData2[i]["amount"].asFloat()); // QString::fromUtf8((const char *)Convertstr(ResponseData1[k]["amount"].asString()));
                                decimal = bal.contains('.');
                                if(decimal == false)
                                {
                                    bal = bal.append(".00");
                                }
                                memset((char *)resp_postimps.ucRDCharges_CGST_RES, 0, sizeof((char *)resp_postimps.ucRDCharges_CGST_RES));
                                memcpy(resp_postimps.ucRDCharges_CGST_RES, bal.toLatin1().data(),strlen(bal.toLatin1().data()));


                                memset(display_imps.ucDispCharges_CGST, 0, sizeof(display_imps.ucDispCharges_CGST));
                                memcpy(display_imps.ucDispCharges_CGST, bal.toLatin1().data(),strlen(bal.toLatin1().data()));


#if DEBUG
                                printf("resp_postimps.ucRDCharges_CGST_RES = %s\n", (char *)resp_postimps.ucRDCharges_CGST_RES); // 02052019
                                printf("display_imps.ucDispCharges_CGST = %s\n", (char *)display_imps.ucDispCharges_CGST); // 02052019

#endif

                            }


                        }
                    }
                    //}
                }
            }
        }
        else
        {
            CMisc miscObj;
            uidmessage::instance()->hide_Message();

            if(strcmp((char*)cal_charges_resp.Commondata_CalCharges_RES.ucResponseMessage_resp,"")!=0)
            {
                char ErrorMsg[250];
                memset(ErrorMsg, 0, sizeof(ErrorMsg));
                sprintf(ErrorMsg,"%s",(char *)cal_charges_resp.Commondata_CalCharges_RES.ucResponseMessage_resp);
                miscObj.DisplayMsgBox(ErrorMsg);
                if(strcmp((char *)cal_charges_resp.Commondata_CalCharges_RES.ucResponseMessage_resp,"Session Expired.")==0)
                {
                    uidmessage::instance()->hide_Message();
                    return SESSION_EXPIRED_FAILED;
                }
            }
            uidmessage::instance()->hide_Message();
            return FUNCTION_FAILED;
        }
    }
    return FUNC_OK;
}


int FPB::Search_by_IFSC_Code_Parsing()
{
    char cResCode[10+1];
    int isize = 0;
    system("sed 's/\"ResponseData\":\"{/\"ResponseData\":{/g' /usr/FINO/FPB/RESP/Search_IFSC_Code.TXT > /tmp/1.txt");
    system("sed 's/}\",\"MessageId\"/},\"MessageId\"/g' /tmp/1.txt > /tmp/2.txt  ");
    system(" sed 's/\\\\//g' /tmp/2.txt > /usr/FINO/FPB/RESP/Search_IFSC_Code.TXT");

    Json1::Value root;
    root.clear();
    //QSqlQuery query;
    Json1::Reader reader;
    std::ifstream ReadData("/usr/FINO/FPB/RESP/Search_IFSC_Code.TXT");
    bool parsedSuccess = reader.parse(ReadData,root,false);

    ReadData.close();


    memset(CustDet.cCity,0,sizeof(CustDet.cCity));
    memset(CustDet.cBranchName,0,sizeof(CustDet.cBranchName));
    memset(CustDet.cIFSC_Code,0,sizeof(CustDet.cIFSC_Code));
    memset(CustDet.cBranchstate,0,sizeof(CustDet.cBranchstate));
    memset(CustDet.cBranchDistrict,0,sizeof(CustDet.cBranchDistrict));
    memset(CustDet.cBankName,0,sizeof(CustDet.cBankName));

    if(not parsedSuccess)
    {

        qDebug()<<"FpsMonthlyPdsData File not Found";
        return FUNCTION_FAILED;
    }
    else
    {
  #if DEBUG
        qDebug()<<"RequestId   : "<<Convertstr(root["RequestId"].asString());
        qDebug()<<"DisplayMessage : "<<Convertstr(root["DisplayMessage"].asString());
        qDebug()<<"ResponseMessage    : "<<Convertstr(root["ResponseMessage"].asString());
        qDebug()<<"ResponseCode : "<<Convertstr(root["ResponseCode"].asString());
        qDebug()<<"MessageType : "<<Convertstr(root["MessageType"].asString());
        qDebug()<<"RespocountNo : "<<Convertstr(root["RespocountNo"].asString());

 #endif
        strcpy(cResCode,Convertstr(root["ResponseCode"].asString()));

        memset(byifsc.Commondata_SearchbyIFSC_RES.ucRequestID_resp, 0, sizeof(byifsc.Commondata_SearchbyIFSC_RES.ucRequestID_resp));
        memset(byifsc.Commondata_SearchbyIFSC_RES.ucDisplayMessage_resp, 0, sizeof(byifsc.Commondata_SearchbyIFSC_RES.ucDisplayMessage_resp));
        memset(byifsc.Commondata_SearchbyIFSC_RES.ucResponseMessage_resp, 0, sizeof(byifsc.Commondata_SearchbyIFSC_RES.ucResponseMessage_resp));
        memset(byifsc.Commondata_SearchbyIFSC_RES.ucResponseCode_resp, 0, sizeof(byifsc.Commondata_SearchbyIFSC_RES.ucResponseCode_resp));

        strcpy(byifsc.Commondata_SearchbyIFSC_RES.ucRequestID_resp,Convertstr(root["RequestId"].asString()));
        strcpy(byifsc.Commondata_SearchbyIFSC_RES.ucDisplayMessage_resp,Convertstr(root["DisplayMessage"].asString()));
        strcpy(byifsc.Commondata_SearchbyIFSC_RES.ucResponseMessage_resp,Convertstr(root["ResponseMessage"].asString()));
        strcpy(byifsc.Commondata_SearchbyIFSC_RES.ucResponseCode_resp,Convertstr(root["ResponseCode"].asString()));
        if(memcmp(cResCode,"0",1)==0)
        {
            Json1::Value ResponseData;
            ResponseData.clear();
            ResponseData = root.get("ResponseData",0);
            isize = ResponseData.size();
 #if DEBUG
            qDebug()<<"isize"<<isize;
            printf("\nSize : %d\n",isize);
#endif
            if(isize==0)
            {
                qDebug("\n1:Key Register Data Not Found\n");
                return FUNCTION_FAILED;
            }
            else
            {
                Json1::Value ifscInfoList;
                ifscInfoList.clear();
                ifscInfoList = ResponseData.get("ifscInfoList",0);

                memset(byifsc.ucifscCode_Resp, 0, sizeof(byifsc.ucifscCode_Resp));
                memset(byifsc.ucbankCode_Resp, 0, sizeof(byifsc.ucbankCode_Resp));
                memset(byifsc.ucbankDesc_Resp, 0, sizeof(byifsc.ucbankDesc_Resp));
                memset(byifsc.uccity_Resp, 0, sizeof(byifsc.uccity_Resp));
                memset(byifsc.ucbranchDesc_Resp, 0, sizeof(byifsc.ucbranchDesc_Resp));
                memset(byifsc.ucNBIN_Resp, 0, sizeof(byifsc.ucNBIN_Resp));

                //                memset(CustDet.cCity,0,sizeof(CustDet.cCity));
                //                memset(CustDet.cBranchName,0,sizeof(CustDet.cBranchName));
                //                memset(CustDet.cIFSC_Code,0,sizeof(CustDet.cIFSC_Code));
                //                memset(CustDet.cBranchstate,0,sizeof(CustDet.cBranchstate));
                //                memset(CustDet.cBranchDistrict,0,sizeof(CustDet.cBranchDistrict));
                //                memset(CustDet.cBankName,0,sizeof(CustDet.cBankName));

                for(int j=0;j<ifscInfoList.size();j++)
                {

 #if DEBUG
                    qDebug()<<"ifscCode:"<<Convertstr(ifscInfoList[j]["ifscCode"].asString());
                    qDebug()<<"bankCode :"<<Convertstr(ifscInfoList[j]["bankCode"].asString());
                    qDebug()<<"bankDesc:"<<Convertstr(ifscInfoList[j]["bankDesc"].asString());
                    qDebug()<<"city:"<<Convertstr(ifscInfoList[j]["city"].asString());
                    qDebug()<<"NBIN:"<<Convertstr(ifscInfoList[j]["NBIN"].asString());
 #endif


                    strcpy((char*)byifsc.ucifscCode_Resp,Convertstr(ifscInfoList[j]["ifscCode"].asString()));

                    strcpy((char*)byifsc.ucbankCode_Resp,Convertstr(ifscInfoList[j]["bankCode"].asString()));

                    strcpy((char*)byifsc.ucbankDesc_Resp,Convertstr(ifscInfoList[j]["bankDesc"].asString()));

                    strcpy((char*)byifsc.uccity_Resp,Convertstr(ifscInfoList[j]["city"].asString()));

                    strcpy((char*)byifsc.ucbranchDesc_Resp,Convertstr(ifscInfoList[j]["branchDesc"].asString()));

                    strcpy((char*)byifsc.ucNBIN_Resp,Convertstr(ifscInfoList[j]["NBIN"].asString()));

                    memset(CustDet.cBranchstate,0,sizeof(CustDet.cBranchstate));
                    memset(CustDet.cBranchDistrict,0,sizeof(CustDet.cBranchDistrict));
                    memset(CustDet.cBankName,0,sizeof(CustDet.cBankName));
                    memset(CustDet.cCity,0,sizeof(CustDet.cCity));


                    memcpy((char *)CustDet.cCity, (const char *)Convertstr(ifscInfoList[j]["city"].asString()), strlen((const char *)Convertstr(ifscInfoList[j]["city"].asString())));
                    memcpy((char *)CustDet.cBranchName, (const char *)Convertstr(ifscInfoList[j]["branchDesc"].asString()), strlen((const char *)Convertstr(ifscInfoList[j]["branchDesc"].asString())));
                    memcpy((char *)CustDet.cIFSC_Code, (const char *)Convertstr(ifscInfoList[j]["ifscCode"].asString()), strlen((const char *)Convertstr(ifscInfoList[j]["ifscCode"].asString())));
                    memcpy((char *)CustDet.cBranchstate,(char *)ui->cmb_state_2->currentText().trimmed().toAscii().data(), strlen((const char *)ui->cmb_state_2->currentText().trimmed().toAscii().data()));
                    memcpy((char *)CustDet.cBranchDistrict, (char *)ui->cmb_District_2->currentText().trimmed().toAscii().data(), strlen((const char *)ui->cmb_District_2->currentText().trimmed().toAscii().data()));
                    memcpy((char *)CustDet.cBankName, (const char *)Convertstr(ifscInfoList[j]["bankCode"].asString()), strlen((const char *)Convertstr(ifscInfoList[j]["bankCode"].asString())));

                    // ui->cmb_bankList->addItem(QString::fromAscii((const char*)Convertstr(ifscInfoList[j]["bankDesc"].asString())));

                }
            }
        }
        else
        {
            CMisc miscObj;
            uidmessage::instance()->hide_Message();

            if(strcmp(byifsc.Commondata_SearchbyIFSC_RES.ucResponseMessage_resp,"")!=0)
            {
                char ErrorMsg[250];
                memset(ErrorMsg, 0, sizeof(ErrorMsg));
                sprintf(ErrorMsg,"%s",(char *)byifsc.Commondata_SearchbyIFSC_RES.ucResponseMessage_resp);
                miscObj.DisplayMsgBox(ErrorMsg);
                if(strcmp((char *)byifsc.Commondata_SearchbyIFSC_RES.ucResponseMessage_resp,"Session Expired.")==0)
                {
                    uidmessage::instance()->hide_Message();
                    return SESSION_EXPIRED_FAILED;
                }
            }
            uidmessage::instance()->hide_Message();
            return FUNCTION_FAILED;
        }
    }
    return FUNC_OK;
}

int FPB::Search_by_Bank_Name_Parsing()
{
    char cResCode[10+1];
    int isize = 0;
    system("sed 's/\"ResponseData\":\"{/\"ResponseData\":{/g' /usr/FINO/FPB/RESP/Search_Bank_Name.TXT > /tmp/1.txt");
    system("sed 's/}\",\"MessageId\"/},\"MessageId\"/g' /tmp/1.txt > /tmp/2.txt  ");
    system(" sed 's/\\\\//g' /tmp/2.txt > /usr/FINO/FPB/RESP/Search_Bank_Name.TXT");

    QSqlQuery query;
    Json1::Value root;
    root.clear();
    //QSqlQuery query;
    Json1::Reader reader;
    std::ifstream ReadData("/usr/FINO/FPB/RESP/Search_Bank_Name.TXT");    CMisc miscObj;
    bool parsedSuccess = reader.parse(ReadData,root,false);

    ReadData.close();


    memset(CustDet.cCity,0,sizeof(CustDet.cCity));
    memset(CustDet.cBranchName,0,sizeof(CustDet.cBranchName));
    memset(CustDet.cIFSC_Code,0,sizeof(CustDet.cIFSC_Code));
    memset(CustDet.cBranchstate,0,sizeof(CustDet.cBranchstate));
    memset(CustDet.cBranchDistrict,0,sizeof(CustDet.cBranchDistrict));
    memset(CustDet.cBankName,0,sizeof(CustDet.cBankName));

    if(not parsedSuccess)
    {

        qDebug()<<"FpsMonthlyPdsData File not Found";
        return FUNCTION_FAILED;
    }
    else
    {
 #if DEBUG
        qDebug()<<"RequestId   : "<<Convertstr(root["RequestId"].asString());
        qDebug()<<"DisplayMessage : "<<Convertstr(root["DisplayMessage"].asString());
        qDebug()<<"ResponseMessage    : "<<Convertstr(root["ResponseMessage"].asString());
        qDebug()<<"ResponseCode : "<<Convertstr(root["ResponseCode"].asString());
        qDebug()<<"MessageType : "<<Convertstr(root["MessageType"].asString());
        qDebug()<<"RespocountNo : "<<Convertstr(root["RespocountNo"].asString());
#endif

        memset(Searchbybranch.Commondata_SearchbyBRANCH_RES.ucResponseMessage_resp, 0, sizeof(Searchbybranch.Commondata_SearchbyBRANCH_RES.ucResponseMessage_resp));
        memcpy(Searchbybranch.Commondata_SearchbyBRANCH_RES.ucResponseMessage_resp, (char*)Convertstr(root["ResponseMessage"].asString()), strlen((char*)Convertstr(root["ResponseMessage"].asString())));

        memset(cResCode, 0, sizeof(cResCode));
        strcpy(cResCode,Convertstr(root["ResponseCode"].asString()));

        /*       strcpy(Statement.ucRequestId,Convertstr(root["RequestId"].asString()));
        strcpy(Statement.ucDisplayMessage,Convertstr(root["DisplayMessage"].asString()));
        strcpy(Statement.ucResponseMessage,Convertstr(root["ResponseMessage"].asString()));
        strcpy(Statement.ucResponseCode,Convertstr(root["ResponseCode"].asString()));*/
        if(memcmp(cResCode,"0",1)==0)
        {
            BankDetails_Search = 1;
            Json1::Value ResponseData;
            ResponseData.clear();
            ResponseData = root.get("ResponseData",0);
            isize = ResponseData.size();
            qDebug()<<"isize"<<isize;
            printf("\nSize : %d\n",isize);
            if(isize==0)
            {
                qDebug("\n1:Key Register Data Not Found\n");
                return FUNCTION_FAILED;
            }
            else
            {
                Json1::Value ifscInfoList;
                ifscInfoList.clear();
                ifscInfoList = ResponseData.get("ifscInfoList",0);
                ui->cmb_BranchName_2->clear();

                memset(Searchbybranch.ucifscCode_Resp, 0, sizeof(Searchbybranch.ucifscCode_Resp));
                memset(Searchbybranch.ucbankCode_Resp, 0, sizeof(Searchbybranch.ucbankCode_Resp));
                memset(Searchbybranch.ucbankDesc_Resp, 0, sizeof(Searchbybranch.ucbankDesc_Resp));
                memset(Searchbybranch.uccity_Resp, 0, sizeof(Searchbybranch.uccity_Resp));
                memset(Searchbybranch.ucbranchDesc_Resp, 0, sizeof(Searchbybranch.ucbranchDesc_Resp));


                //                memset(CustDet.cCity,0,sizeof(CustDet.cCity));
                //                memset(CustDet.cBranchName,0,sizeof(CustDet.cBranchName));
                //                memset(CustDet.cIFSC_Code,0,sizeof(CustDet.cIFSC_Code));
                //                memset(CustDet.cBranchstate,0,sizeof(CustDet.cBranchstate));
                //                memset(CustDet.cBranchDistrict,0,sizeof(CustDet.cBranchDistrict));
                //                memset(CustDet.cBankName,0,sizeof(CustDet.cBankName));

                for(int j=0;j<ifscInfoList.size();j++)
                {
  #if DEBUG
                    qDebug()<<"code:"<<Convertstr(ifscInfoList[j]["ifscCode"].asString());
                    qDebug()<<"bankCode:"<<Convertstr(ifscInfoList[j]["bankCode"].asString());
                    qDebug()<<"description:"<<Convertstr(ifscInfoList[j]["bankDesc"].asString());
                    qDebug()<<"city:"<<Convertstr(ifscInfoList[j]["city"].asString());
                    qDebug()<<"branchDesc:"<<Convertstr(ifscInfoList[j]["branchDesc"].asString());
                    qDebug()<<"NBIN:"<<Convertstr(ifscInfoList[j]["NBIN"].asString());
#endif
                    memcpy(Searchbybranch.ucifscCode_Resp[j], (char*)Convertstr(ifscInfoList[j]["ifscCode"].asString()), strlen((char*)Convertstr(ifscInfoList[j]["ifscCode"].asString())));
                    memcpy(Searchbybranch.ucbankCode_Resp[j], (char*)Convertstr(ifscInfoList[j]["bankCode"].asString()), strlen((char*)Convertstr(ifscInfoList[j]["bankCode"].asString())));
                    memcpy(Searchbybranch.ucbankDesc_Resp[j], (char*)Convertstr(ifscInfoList[j]["bankDesc"].asString()), strlen((char*)Convertstr(ifscInfoList[j]["bankDesc"].asString())));
                    memcpy(Searchbybranch.uccity_Resp[j], (char*)Convertstr(ifscInfoList[j]["city"].asString()), strlen((char*)Convertstr(ifscInfoList[j]["city"].asString())));
                    memcpy(Searchbybranch.ucbranchDesc_Resp[j], (char*)Convertstr(ifscInfoList[j]["branchDesc"].asString()), strlen((char*)Convertstr(ifscInfoList[j]["branchDesc"].asString())));


                    memcpy((char *)CustDet.cCity, (const char *)Convertstr(ifscInfoList[j]["city"].asString()), strlen((const char *)Convertstr(ifscInfoList[j]["city"].asString())));
                    memcpy((char *)CustDet.cBranchName, (const char *)Convertstr(ifscInfoList[j]["branchDesc"].asString()), strlen((const char *)Convertstr(ifscInfoList[j]["branchDesc"].asString())));
                    memcpy((char *)CustDet.cIFSC_Code, (const char *)Convertstr(ifscInfoList[j]["ifscCode"].asString()), strlen((const char *)Convertstr(ifscInfoList[j]["ifscCode"].asString())));
//                    memcpy((char *)CustDet.cBranchstate,(char *)ui->cmb_state_2->currentText().trimmed().toAscii().data(), strlen((const char *)ui->cmb_state_2->currentText().trimmed().toAscii().data())); // 13092019
//                    memcpy((char *)CustDet.cBranchDistrict, (char *)ui->cmb_District_2->currentText().trimmed().toAscii().data(), strlen((const char *)ui->cmb_District_2->currentText().trimmed().toAscii().data())); // 13092019
                    memcpy((char *)CustDet.cBankName, (const char *)Convertstr(ifscInfoList[j]["bankCode"].asString()), strlen((const char *)Convertstr(ifscInfoList[j]["bankCode"].asString())));

 #if DEBUG
                    printf("j = %d Searchbybranch.ucbankDesc_Resp = %s\n", j,Searchbybranch.ucbranchDesc_Resp[j]);
                    printf("CustDet.cBranchName = %s\n", CustDet.cBranchName);
 #endif
                    ui->cmb_BranchName_2->addItem(QString::fromAscii((const char*)Searchbybranch.ucbranchDesc_Resp[j]));
                    // ui->cmb_bankList->addItem(QString::fromAscii((const char*)Searchbybranch.ucbankDesc_Resp[j]));

                }
                memcpy((char *)CustDet.cBranchstate,(char *)ui->cmb_state_2->currentText().trimmed().toAscii().data(), strlen((const char *)ui->cmb_state_2->currentText().trimmed().toAscii().data())); // 13092019
                memcpy((char *)CustDet.cBranchDistrict, (char *)ui->cmb_District_2->currentText().trimmed().toAscii().data(), strlen((const char *)ui->cmb_District_2->currentText().trimmed().toAscii().data())); // 13092019

            }
        }
        else
        {
            CMisc miscObj;
            uidmessage::instance()->hide_Message();

            if(strcmp(Searchbybranch.Commondata_SearchbyBRANCH_RES.ucResponseMessage_resp,"")!=0)
            {
                char ErrorMsg[250];
                memset(ErrorMsg, 0, sizeof(ErrorMsg));
                sprintf(ErrorMsg,"%s",(char *)Searchbybranch.Commondata_SearchbyBRANCH_RES.ucResponseMessage_resp);
                miscObj.DisplayMsgBox(ErrorMsg);
                if(strcmp((char *)Searchbybranch.Commondata_SearchbyBRANCH_RES.ucResponseMessage_resp,"Session Expired.")==0)
                {
                    uidmessage::instance()->hide_Message();
                    return SESSION_EXPIRED_FAILED;
                }
            }
            uidmessage::instance()->hide_Message();
            return FUNCTION_FAILED;
        }
    }

    ui->cmb_BranchName_2->setCurrentIndex(0);
    return FUNC_OK;
}


int FPB::POST_IMPS_Parsing()
{
    int flag_transRef = 0;
    char cResCode[10+1];
    char cChargesListCode[15+1]; // 02052019
    char ctransactionref[25];
    //memclear(cChargesListCode); // 02052019
    memset(cChargesListCode, 0, sizeof(cChargesListCode));
    memset(ctransactionref, 0, sizeof(ctransactionref));
    system("sed 's/\"ResponseData\":\"{/\"ResponseData\":{/g' /usr/FINO/FPB/RESP/POSTIMPS.TXT > /tmp/1.txt");
    system("sed 's/}\",\"MessageId\"/},\"MessageId\"/g' /tmp/1.txt > /tmp/2.txt  ");
    system(" sed 's/\\\\//g' /tmp/2.txt > /usr/FINO/FPB/RESP/POSTIMPS.TXT");

    Json1::Value root;
    root.clear();
    //QSqlQuery query;
    Json1::Reader reader;
    std::ifstream ReadData("/usr/FINO/FPB/RESP/POSTIMPS.TXT");
    bool parsedSuccess = reader.parse(ReadData,root,false);

    ReadData.close();

    if(not parsedSuccess)
    {
        qDebug()<<"FpsMonthlyPdsData File not Found";
        return FUNCTION_FAILED;
    }
    else
    {

#if DEBUG
        qDebug()<<"RequestId   : "<<Convertstr(root["RequestId"].asString());
        qDebug()<<"DisplayMessage : "<<Convertstr(root["DisplayMessage"].asString());
        qDebug()<<"ResponseMessage    : "<<Convertstr(root["ResponseMessage"].asString());
        qDebug()<<"ResponseCode : "<<Convertstr(root["ResponseCode"].asString());
        qDebug()<<"MessageType : "<<Convertstr(root["MessageType"].asString());
        qDebug()<<"AuthmanFlag : "<<Convertstr(root["AuthmanFlag"].asString());
#endif


        memset(resp_postimps.Commondata_POBV_RES.ucResponseMessage_resp, 0, sizeof(resp_postimps.Commondata_POBV_RES.ucResponseMessage_resp));
        memcpy(resp_postimps.Commondata_POBV_RES.ucResponseMessage_resp, Convertstr(root["ResponseMessage"].asString()), strlen(Convertstr(root["ResponseMessage"].asString())));

        strcpy(cResCode,Convertstr(root["ResponseCode"].asString()));
        memset(resp_postimps.Commondata_POBV_RES.ucResponseCode_resp, 0, sizeof(resp_postimps.Commondata_POBV_RES.ucResponseCode_resp));
        memcpy(resp_postimps.Commondata_POBV_RES.ucResponseCode_resp, Convertstr(root["ResponseCode"].asString()), strlen(Convertstr(root["ResponseCode"].asString())));



        if(memcmp(cResCode,"0",1)==0)
        {
            Json1::Value ResponseData, RespData_authResponse;
            Json1::Value RespData_IMPSServiceDataListResponse, RespData_PostTransactionResponse;

            ResponseData.clear();
            ResponseData = root.get("ResponseData",0);

            printf("\nSize : %d\n",ResponseData.size());

            if(ResponseData.size()==0)
                return FUNC_ERROR;
            else
            {

#if DEBUG
                qDebug()<<"ResponseData returnCode : "<<Convertstr(RespData_authResponse["returnCode"].asString());
                qDebug()<<"ResponseData responseBitMap : "<<Convertstr(RespData_authResponse["responseBitMap"].asString());
                qDebug()<<"authResponse returnCode : "<<Convertstr(RespData_authResponse["returnCode"].asString());
                qDebug()<<"authResponse responseMessage : "<<Convertstr(RespData_authResponse["responseMessage"].asString());

#endif

                RespData_authResponse.clear();
                RespData_authResponse = ResponseData.get("authResponse",0);


                ////qDebug()<<"CreateCustomerResponse : "<<Convertstr(ResponseData["CreateCustomerResponse"].asString());
                //// qDebug()<<"CreateAccountResponse : "<<Convertstr(ResponseData["CreateAccountResponse"].asString());
                //// qDebug()<<"AddBenificiaryResponse : "<<Convertstr(ResponseData["AddBenificiaryResponse"].asString());
                //// qDebug()<<"AddWalkinCustomerResponse : "<<Convertstr(ResponseData["AddWalkinCustomerResponse"].asString());

                unsigned char temp_transref[30]; // PROD_04092019
                memset(temp_transref, 0, sizeof(temp_transref)); // PROD_04092019

                RespData_PostTransactionResponse.clear();
                RespData_PostTransactionResponse = ResponseData.get("PostTransactionResponse",0);
                printf("\nSize : %d\n",RespData_PostTransactionResponse.size());
                flag_transRef = 0;


                if(RespData_PostTransactionResponse.size() == 4) // PROD_04092019
                {
                    flag_transRef = 1;
                    qDebug()<<"4 Leg Response";
                    memset(ctransactionref, 0, sizeof(ctransactionref));
                    memcpy(ctransactionref, (char *)Convertstr(RespData_PostTransactionResponse[0]["txnReferenceNo"].asString()), strlen((const char *)Convertstr(RespData_PostTransactionResponse[0]["txnReferenceNo"].asString())));
                    qDebug()<<"======    txnReferenceNo if == "<<Convertstr(RespData_PostTransactionResponse[0]["txnReferenceNo"].asString());
                    printf("======    ctransactionref flag111 = %s\n", ctransactionref);
                }
                else // PROD_04092019
                {
                    flag_transRef = 0;
                    qDebug()<<"Leg Response count == "<<RespData_PostTransactionResponse.size();
                    memset(ctransactionref, 0, sizeof(ctransactionref));
                    if(flag_bc_merchant_account == 'B' && (RespData_PostTransactionResponse.size() == 1))
                    {
                            memcpy(ctransactionref, (char *)Convertstr(RespData_PostTransactionResponse[0]["txnReferenceNo"].asString()), strlen((const char *)Convertstr(RespData_PostTransactionResponse[0]["txnReferenceNo"].asString())));
                            qDebug()<<"********  txnReferenceNo else == "<<Convertstr(RespData_PostTransactionResponse[0]["txnReferenceNo"].asString());
                    }
                    else
                    {
                        memcpy(ctransactionref, (char *)Convertstr(RespData_PostTransactionResponse[1]["txnReferenceNo"].asString()), strlen((const char *)Convertstr(RespData_PostTransactionResponse[1]["txnReferenceNo"].asString())));
                        qDebug()<<"********  txnReferenceNo else == "<<Convertstr(RespData_PostTransactionResponse[1]["txnReferenceNo"].asString());
                    }
                            printf("*********  ctransactionref flag ELSE = %s\n", ctransactionref);

                }

                for(int j=0;j<RespData_PostTransactionResponse.size();j++)
                {
                    memset(resp_postimps.ucRDbenename_RES, 0, sizeof(resp_postimps.ucRDbenename_RES));
                    memset(resp_postimps.ucRDTransactonAmount_RES, 0, sizeof(resp_postimps.ucRDTransactonAmount_RES));
                    memset(resp_postimps.ucRDBeneAccNo_RES, 0, sizeof(resp_postimps.ucRDBeneAccNo_RES));
                    memset(resp_postimps.ucRDTransactionType_RES, 0, sizeof(resp_postimps.ucRDTransactionType_RES));
                    memset(resp_postimps.ucRDBankName_RES, 0, sizeof(resp_postimps.ucRDBankName_RES));
                    memset(resp_postimps.ucRDTransactionRefNo_RES, 0, sizeof(resp_postimps.ucRDTransactionRefNo_RES));

                    //  memcpy(resp_postimps.ucRDTransactionRefNo_RES, (const char*)Convertstr(RespData_PostTransactionResponse[j]["txnReferenceNo"].asString()), strlen((const char*)Convertstr(RespData_PostTransactionResponse[j]["txnReferenceNo"].asString())));
#if DEBUG
                    qDebug()<<"PostTransactionResponse returnCode : "<<Convertstr(RespData_PostTransactionResponse[j]["returnCode"].asString());
                    qDebug()<<"PostTransactionResponse txnReferenceNo : "<<Convertstr(RespData_PostTransactionResponse[j]["txnReferenceNo"].asString());
                    qDebug()<<"PostTransactionResponse cbsTxnReferenceNo : "<<Convertstr(RespData_PostTransactionResponse[j]["cbsTxnReferenceNo"].asString());
#endif
                    Json1::Value ResponseData1;
                    ResponseData1.clear();
                    ResponseData1 = RespData_PostTransactionResponse[j].get("chargesList",0);
                    printf("\nchargesList Size : %d\n",ResponseData1.size());

//                    if(ResponseData1.size() == 0) // PROD_04092019
//                    {
//                        qDebug()<<"NO CHARGELIST test";
//                        memset((char *)resp_postimps.ucRDCharges_RES, 0, sizeof((char *)resp_postimps.ucRDCharges_RES));
//                        memcpy(resp_postimps.ucRDCharges_RES, (char *)display_imps.ucDispCharges,strlen((char *)display_imps.ucDispCharges));
//                    }
//                    else // PROD_04092019
                    {
                        for(int k=0;k<ResponseData1.size();k++)
                        {
//                            flag_transRef = 1;
//                            if(k==0)
//                            {
//                                memset(ctransactionref, 0, sizeof(ctransactionref));
//                                memcpy(ctransactionref, (char *)Convertstr(RespData_PostTransactionResponse[j]["txnReferenceNo"].asString()), strlen((const char *)Convertstr(RespData_PostTransactionResponse[j]["txnReferenceNo"].asString())));
//                            }
                            memset(cChargesListCode,0,sizeof(cChargesListCode)); // 02052019
                            memcpy(cChargesListCode, (const char *)Convertstr(ResponseData1[k]["code"].asString()), strlen((const char *)Convertstr(ResponseData1[k]["code"].asString()))); // 02052019
#if DEBUG
                            qDebug()<<"chargesList code : "<<Convertstr(ResponseData1[k]["code"].asString());
                            qDebug()<<"chargesList amount : "<<QString::number(ResponseData1[k]["amount"].asFloat()); // Convertstr(ResponseData1[k]["amount"].asString());
                            qDebug()<<"chargesList description : "<<Convertstr(ResponseData1[k]["description"].asString());
                            qDebug()<<"chargesList chargeAcctNum : "<<Convertstr(ResponseData1[k]["chargeAcctNum"].asString());
#endif
//                            if(memcmp(cChargesListCode,"CHGDMTIMPS",strlen("CHGDMTIMPS"))==0) // Prod_04092019
//                            { // 02052019
//                                bool decimal;
//                                QString bal = QString::number(ResponseData1[k]["amount"].asFloat()); // QString::fromUtf8((const char *)Convertstr(ResponseData1[k]["amount"].asString()));
//                                decimal = bal.contains('.');
//                                if(decimal == false)
//                                {
//                                    bal = bal.append(".00");
//                                }
//                                memset((char *)resp_postimps.ucRDCharges_RES, 0, sizeof((char *)resp_postimps.ucRDCharges_RES));
//                                //  memcpy(resp_postimps.ucRDCharges_RES,(const char *)Convertstr(ResponseData1[k]["amount"].asString()),strlen((const char *)Convertstr(ResponseData1[k]["amount"].asString()))); // 02052019
//                                memcpy(resp_postimps.ucRDCharges_RES, bal.toLatin1().data(),strlen(bal.toLatin1().data()));

//#if DEBUG
//                                printf("resp_postimps.ucRDCharges_RES = %s\n", (char *)resp_postimps.ucRDCharges_RES); // 02052019
//#endif
//                            } // 02052019


//                            if(memcmp(cChargesListCode,"SGST",strlen("SGST"))==0) // Prod_04092019
//                            {
//                                bool decimal;
//                                QString bal = QString::number(ResponseData1[k]["amount"].asFloat()); // QString::fromUtf8((const char *)Convertstr(ResponseData1[k]["amount"].asString()));
//                                decimal = bal.contains('.');
//                                if(decimal == false)
//                                {
//                                    bal = bal.append(".00");
//                                }
//                                memset((char *)resp_postimps.ucRDCharges_SGST_RES, 0, sizeof((char *)resp_postimps.ucRDCharges_SGST_RES));
//                                memcpy(resp_postimps.ucRDCharges_SGST_RES, bal.toLatin1().data(),strlen(bal.toLatin1().data()));

//                                memset(display_imps.ucDispCharges_SGST, 0, sizeof(display_imps.ucDispCharges_SGST));
//                                memcpy(display_imps.ucDispCharges_SGST, bal.toLatin1().data(), strlen((const char*)bal.toLatin1().data()));



//#if DEBUG
//                                printf("resp_postimps.ucRDCharges_SGST_RES = %s\n", (char *)resp_postimps.ucRDCharges_SGST_RES); // 02052019
//                                printf("display_imps.ucDispCharges_SGST = %s\n", (char *)display_imps.ucDispCharges_SGST); // 02052019

//#endif
//                            }

//                            if(memcmp(cChargesListCode,"CGST",strlen("CGST"))==0) // Prod_04092019
//                            {
//                                bool decimal;
//                                QString bal = QString::number(ResponseData1[k]["amount"].asFloat()); // QString::fromUtf8((const char *)Convertstr(ResponseData1[k]["amount"].asString()));
//                                decimal = bal.contains('.');
//                                if(decimal == false)
//                                {
//                                    bal = bal.append(".00");
//                                }
//                                memset((char *)resp_postimps.ucRDCharges_CGST_RES, 0, sizeof((char *)resp_postimps.ucRDCharges_CGST_RES));
//                                memcpy(resp_postimps.ucRDCharges_CGST_RES, bal.toLatin1().data(),strlen(bal.toLatin1().data()));


//                                memset(display_imps.ucDispCharges_CGST, 0, sizeof(display_imps.ucDispCharges_CGST));
//                                memcpy(display_imps.ucDispCharges_CGST, bal.toLatin1().data(),strlen(bal.toLatin1().data()));


//#if DEBUG
//                                printf("resp_postimps.ucRDCharges_CGST_RES = %s\n", (char *)resp_postimps.ucRDCharges_CGST_RES); // 02052019
//                                printf("display_imps.ucDispCharges_CGST = %s\n", (char *)display_imps.ucDispCharges_CGST); // 02052019

//#endif

//                            }

                        }
                    }
                    Json1::Value ResponseData2;
                    ResponseData2.clear();
                    ResponseData2 = RespData_PostTransactionResponse[j].get("balancesList",0);
                    printf("\nSize1 : %d\n",ResponseData2.size());
                    for(int i=0;i<ResponseData2.size();i++)
                    {

                        //                        QString g = QString::number(ResponseData2[i]["ledgerBalance"].asFloat());
                        //                        qDebug()<<"g = "<<g;
                        //                        QString f = QString::number(ResponseData2[i]["availableBalance"].asFloat());
                        //                        qDebug()<<"f = "<<f;
#if DEBUG

                        qDebug()<<"balancesList ledgerBalance : "<<Convertstr(ResponseData2[i]["ledgerBalance"].asString());         ////
                        qDebug()<<"balancesList availableBalance : "<<Convertstr(ResponseData2[i]["availableBalance"].asString());   ////
                        qDebug()<<"balancesList currency : "<<Convertstr(ResponseData2[i]["currency"].asString());
#endif
                        memset(resp_postimps.Commondata_POBV_RES.ucIMPS_AvilableBalance, 0, sizeof(resp_postimps.Commondata_POBV_RES.ucIMPS_AvilableBalance));
                        memcpy(resp_postimps.Commondata_POBV_RES.ucIMPS_AvilableBalance, (const char*)Convertstr(ResponseData2[i]["availableBalance"].asString()), strlen((const char*)Convertstr(ResponseData2[i]["availableBalance"].asString())));

                    }
                }



                double disp_TotalCharges = (atof((char *)resp_postimps.ucRDCharges_RES)-(atof((char *)display_imps.ucDispCharges_SGST) + atof((char *)display_imps.ucDispCharges_CGST))); // 02052019
#if DEBUG
                printf("disp_TotalCharges = %lf\n", disp_TotalCharges);
#endif
                bool decimal2;
                QString bal1 = QString::number(disp_TotalCharges);
                decimal2 = bal1.contains('.');
                if(decimal2 == false)
                {
                    bal1 = bal1.append(".00");
                }
                memset(display_imps.ucDispMinusCharges, 0, strlen((char *)display_imps.ucDispMinusCharges));
                //   sprintf((char *)display_imps.ucDispCharges, "%.2s", (char *)bal1.toAscii().trimmed().data());
                printf("bal1 length == %d\n", strlen(bal1.toAscii().trimmed().data()));
                memcpy(display_imps.ucDispMinusCharges, bal1.toAscii().trimmed().data(), strlen(bal1.toAscii().trimmed().data()));
#if DEBUG
                qDebug()<<"bal1 == "<<bal1;
                printf("\n    *******    display_imps.ucDispMinusCharges = %s    *******\n", display_imps.ucDispMinusCharges); // 16072019
#endif



                ////  qDebug()<<"AddExternalResponse : "<<Convertstr(ResponseData["AddExternalResponse"].asString());
                ////  qDebug()<<"AddWalkinCustomerResponse : "<<Convertstr(ResponseData["CreateAccountResponse"].asString());

                RespData_IMPSServiceDataListResponse.clear();
                RespData_IMPSServiceDataListResponse = ResponseData.get("IMPSServiceDataListResponse",0);

#if DEBUG
                qDebug()<<"ActCode : "<<Convertstr(RespData_IMPSServiceDataListResponse["ActCode"].asString());
                qDebug()<<"BankRRN : "<<Convertstr(RespData_IMPSServiceDataListResponse["BankRRN"].asString());
                qDebug()<<"BeneAccNo : "<<Convertstr(RespData_IMPSServiceDataListResponse["BeneAccNo"].asString());
                qDebug()<<"BeneIFSC : "<<Convertstr(RespData_IMPSServiceDataListResponse["BeneIFSC"].asString());
                qDebug()<<"BeneName : "<<Convertstr(RespData_IMPSServiceDataListResponse["BeneName"].asString());
                qDebug()<<"Response : "<<Convertstr(RespData_IMPSServiceDataListResponse["Response"].asString());
                qDebug()<<"TranRefNo : "<<Convertstr(RespData_IMPSServiceDataListResponse["TranRefNo"].asString());
                qDebug()<<"TxnAmount : "<<Convertstr(RespData_IMPSServiceDataListResponse["TxnAmount"].asString());
#endif
                memset(resp_postimps.ucRDbenename_RES,         0, sizeof(resp_postimps.ucRDbenename_RES));
                memset(resp_postimps.ucRDTransactonAmount_RES, 0, sizeof(resp_postimps.ucRDTransactonAmount_RES));
                memset(resp_postimps.ucRDBeneAccNo_RES,        0, sizeof(resp_postimps.ucRDBeneAccNo_RES));
                memset(resp_postimps.ucRDTransactionType_RES,  0, sizeof(resp_postimps.ucRDTransactionType_RES));
                memset(resp_postimps.ucRDBankName_RES,         0, sizeof(resp_postimps.ucRDBankName_RES));
                memset(resp_postimps.ucRDTransactionRefNo_RES, 0, sizeof(resp_postimps.ucRDTransactionRefNo_RES));

                //                memcpy(resp_postimps.ucRDbenename_RES, (const char*)Convertstr(RespData_IMPSServiceDataListResponse["BeneName"].asString()), strlen((const char*)Convertstr(RespData_IMPSServiceDataListResponse["BeneName"].asString())));
                //                memcpy(resp_postimps.ucRDTransactonAmount_RES, (const char*)Convertstr(RespData_IMPSServiceDataListResponse["TxnAmount"].asString()), strlen((const char*)Convertstr(RespData_IMPSServiceDataListResponse["TxnAmount"].asString())));
                //                memcpy(resp_postimps.ucRDBeneAccNo_RES, (const char*)Convertstr(RespData_IMPSServiceDataListResponse["BeneAccNo"].asString()), strlen((const char*)Convertstr(RespData_IMPSServiceDataListResponse["BeneAccNo"].asString())));
                //                memcpy(resp_postimps.ucRDTransactionType_RES, (const char*)Flag_Transaction_Type, strlen((const char *)Flag_Transaction_Type));
                //                memcpy(resp_postimps.ucRDTransactionRefNo_RES, (const char*)Convertstr(RespData_IMPSServiceDataListResponse["TranRefNo"].asString()), strlen((const char*)Convertstr(RespData_IMPSServiceDataListResponse["TranRefNo"].asString())));
                //                memcpy(resp_postimps.ucRDBankName_RES, (const char*)postbeneverif.beneBank_req, strlen((const char*)postbeneverif.beneBank_req));
                //                printf("postbeneverif.beneBank_req = %s and %s\n", postbeneverif.beneBank_req, resp_postimps.ucRDBankName_RES);

                memcpy(resp_postimps.ucRDbenename_RES, (const char*)ui->Bene_Name_edit->text().trimmed().toAscii().data(), strlen((const char*)ui->Bene_Name_edit->text().trimmed().toAscii().data()));
                memcpy(resp_postimps.ucRDTransactonAmount_RES, (const char*)ui->Fund_transfer_amount_edit->text().trimmed().toAscii().data(), strlen((const char*)ui->Fund_transfer_amount_edit->text().trimmed().toAscii().data()));
                memcpy(resp_postimps.ucRDBeneAccNo_RES, (const char*)ui->Bene_acc_no_edit->text().trimmed().toAscii().data(), strlen((const char*)ui->Bene_acc_no_edit->text().trimmed().toAscii().data()));
                memcpy(resp_postimps.ucRDTransactionType_RES, (const char*)Flag_Transaction_Type, strlen((const char *)Flag_Transaction_Type));
                //   memcpy(resp_postimps.ucRDTransactionRefNo_RES, (const char*)Convertstr(RespData_IMPSServiceDataListResponse["TranRefNo"].asString()), strlen((const char*)Convertstr(RespData_IMPSServiceDataListResponse["TranRefNo"].asString())));
                memcpy(resp_postimps.ucRDTransactionRefNo_RES, (const char*)ctransactionref, strlen((const char*)ctransactionref)); //1907


                memcpy(resp_postimps.ucRDBankName_RES, (const char*)ui->cmb_bankList->currentText().trimmed().toAscii().data(), strlen((const char*)ui->cmb_bankList->currentText().trimmed().toAscii().data()));
#if DEBUG
                printf("resp_postimps.ucRDTransactionRefNo_RES = %s\n", resp_postimps.ucRDTransactionRefNo_RES);
                printf("postbeneverif.beneBank_req = %s and %s\n", postbeneverif.beneBank_req, resp_postimps.ucRDBankName_RES);
#endif



                int imps_totalamount = atoi((char *)resp_postimps.ucRDCharges_RES) + atoi((char *)resp_postimps.ucRDTransactonAmount_RES); // 02052019
                //                int imps_amount = atoi((const char*)Convertstr(RespData_IMPSServiceDataListResponse["TxnAmount"].asString())); // 02052019
                int imps_amount = atoi((const char*)ui->Fund_transfer_amount_edit->text().trimmed().toAscii().data()); // 02052019

                bool decimal , decimal1;
                QString bal = QString::number(imps_totalamount);
                decimal = bal.contains('.');
                if(decimal == false)
                {
                    bal = bal.append(".00");
                }

                memset((char *)resp_postimps.ucTOTALAMOUNT_IMPS, 0, sizeof((char *)resp_postimps.ucTOTALAMOUNT_IMPS));
                memcpy((char *)resp_postimps.ucTOTALAMOUNT_IMPS, bal.toLatin1().data(),strlen(bal.toLatin1().data()));
#if DEBUG
                printf("resp_postimps.ucTOTALAMOUNT_IMPS =====  %s\n",resp_postimps.ucTOTALAMOUNT_IMPS);
#endif
                //sprintf((char *)resp_postimps.ucTOTALAMOUNT_IMPS, "%f", imps_totalamount); // 02052019
                bal.clear();
                bal = QString::number(imps_amount);
                decimal1 = bal.contains('.');
                if(decimal1 == false)
                {
                    bal = bal.append(".00");
                }
                memset(resp_postimps.ucRDTransactonAmount_RES, 0, sizeof(resp_postimps.ucRDTransactonAmount_RES));
                memcpy(resp_postimps.ucRDTransactonAmount_RES, bal.toLatin1().data(),strlen(bal.toLatin1().data()));
#if DEBUG
                printf("resp_postimps.ucRDTransactonAmount_RES =====  %s\n",resp_postimps.ucRDTransactonAmount_RES);
#endif

            }
        }
        else
        {
            CMisc miscObj;
            uidmessage::instance()->hide_Message();

            if(strcmp(resp_postimps.Commondata_POBV_RES.ucResponseMessage_resp,"")!=0)
            {
                char ErrorMsg[250];
                memset(ErrorMsg, 0, sizeof(ErrorMsg));
                sprintf(ErrorMsg,"%s",(char *)resp_postimps.Commondata_POBV_RES.ucResponseMessage_resp);
                miscObj.DisplayMsgBox(ErrorMsg);
                if(strcmp((char *)resp_postimps.Commondata_POBV_RES.ucResponseMessage_resp,"Session Expired.")==0)
                {
                    uidmessage::instance()->hide_Message();
                    return SESSION_EXPIRED_FAILED;
                }

            }
            uidmessage::instance()->hide_Message();
            return FUNCTION_FAILED;
        }
    }
    return FUNCTION_SUCCESS;
}


int FPB::ADD_Bene_Parsing()
{
    char cResCode[10+1];

    system("sed 's/\"ResponseData\":\"{/\"ResponseData\":{/g' /usr/FINO/FPB/RESP/ADD_Bene.TXT > /tmp/1.txt");
    system("sed 's/}\",\"MessageId\"/},\"MessageId\"/g' /tmp/1.txt > /tmp/2.txt  ");
    system(" sed 's/\\\\//g' /tmp/2.txt > /usr/FINO/FPB/RESP/ADD_Bene.TXT");


    //system("sed 's/\"ResponseData\":\"{/\"ResponseData\":{/g' /usr/FINO/FPB/RESP/ADD_Bene.TXT > /tmp/1.txt");
    //system("sed 's/}\",\"MessageId\"/},\"MessageId\"/g' /tmp/1.txt > /tmp/2.txt  ");
    //system(" sed 's/\\\\//g' /tmp/2.txt > /usr/FINO/FPB/RESP/ADD_Bene.TXT");

    Json1::Value root;
    root.clear();
    //QSqlQuery query;
    Json1::Reader reader;
    std::ifstream ReadData("/usr/FINO/FPB/RESP/ADD_Bene.TXT");
    bool parsedSuccess = reader.parse(ReadData,root,false);

    ReadData.close();

    if(not parsedSuccess)
    {

        qDebug()<<"FpsMonthlyPdsData File not Found";
        return FUNCTION_FAILED;
    }
    else
    {

   #if DEBUG
        qDebug()<<"RequestId   : "<<Convertstr(root["RequestId"].asString());
        qDebug()<<"DisplayMessage : "<<Convertstr(root["DisplayMessage"].asString());
        qDebug()<<"ResponseMessage    : "<<Convertstr(root["ResponseMessage"].asString());
        qDebug()<<"ResponseCode : "<<Convertstr(root["ResponseCode"].asString());
        qDebug()<<"MessageType : "<<Convertstr(root["MessageType"].asString());
        qDebug()<<"RespocountNo : "<<Convertstr(root["RespocountNo"].asString());
  #endif

        strcpy(cResCode,Convertstr(root["ResponseCode"].asString()));

        /*       strcpy(Statement.ucRequestId,Convertstr(root["RequestId"].asString()));
        strcpy(Statement.ucDisplayMessage,Convertstr(root["DisplayMessage"].asString()));
        strcpy(Statement.ucResponseMessage,Convertstr(root["ResponseMessage"].asString()));
        strcpy(Statement.ucResponseCode,Convertstr(root["ResponseCode"].asString()));*/
        memset(add_bene_resp.ucRDResponseMessage_Resp, 0, sizeof(add_bene_resp.ucRDResponseMessage_Resp));
        memset(add_bene_resp.ucRDResponseCode_Resp, 0, sizeof(add_bene_resp.ucRDResponseCode_Resp));

        memcpy(add_bene_resp.ucRDResponseMessage_Resp, (const char*)Convertstr(root["ResponseMessage"].asString()), strlen((const char*)Convertstr(root["ResponseMessage"].asString())));
        memcpy(add_bene_resp.ucRDResponseCode_Resp, (const char*)Convertstr(root["ResponseCode"].asString()), strlen((const char*)Convertstr(root["ResponseCode"].asString())));

        if(memcmp(cResCode,"0",1)==0)
        {
            Json1::Value ResponseData;
            ResponseData.clear();
            ResponseData = root.get("ResponseData",0);
            printf("\nSize : %d\n",ResponseData.size());
            if(ResponseData.size()==0)
            {
                qDebug("\n1:Key Register Data Not Found\n");
                return FUNCTION_FAILED;
            }
            else
            {
                Json1::Value ResponseData1;
                ResponseData1 = ResponseData.get("chargesList",0);
                //qDebug()<<"ResponseData1 "<< ResponseData1;
                for(int j=0;j<ResponseData1.size();j++)
                {
                    //                    qDebug()<<"ifsc code:"<<Convertstr(ResponseData1[j]["ifscCode"].asString());
                    //                    qDebug()<<"bankCode:"<<Convertstr(ResponseData1[j]["bankCode"].asString());
                    //                    qDebug()<<"bankDesc:"<<Convertstr(ResponseData1[j]["bankDesc"].asString());
                    //                    qDebug()<<"city:"<<Convertstr(ResponseData1[j]["city"].asString());
                    //                    qDebug()<<"branchDesc:"<<Convertstr(ResponseData1[j]["branchDesc"].asString());
                    //                    qDebug()<<"NBIN:"<<Convertstr(ResponseData1[j]["NBIN"].asString());
                }
            }
        }
        else
        {
            CMisc miscObj;
            uidmessage::instance()->hide_Message();

            if(strcmp((char*)add_bene_resp.ucRDResponseMessage_Resp,"")!=0)
            {
                char ErrorMsg[250];
                memset(ErrorMsg, 0, sizeof(ErrorMsg));
                sprintf(ErrorMsg,"%s",(char *)add_bene_resp.ucRDResponseMessage_Resp);
                miscObj.DisplayMsgBox(ErrorMsg);
                if(strcmp((char *)add_bene_resp.ucRDResponseMessage_Resp,"Session Expired.")==0)
                {
                    uidmessage::instance()->hide_Message();
                    return SESSION_EXPIRED_FAILED;
                }
            }
            uidmessage::instance()->hide_Message();
            return FUNCTION_FAILED;
        }
    }
    return FUNC_OK;
}

int FPB::FetchExtraBene_Parsing()
{
    char cResCode[10+1];

    system("sed 's/\"ResponseData\":\"{/\"ResponseData\":{/g' /usr/FINO/FPB/RESP/FetchExtraBene.TXT > /tmp/1.txt");
    system("sed 's/}\",\"MessageId\"/},\"MessageId\"/g' /tmp/1.txt > /tmp/2.txt  ");
    system(" sed 's/\\\\//g' /tmp/2.txt > /usr/FINO/FPB/RESP/FetchExtraBene.TXT");

    Json1::Value root;
    root.clear();
    //QSqlQuery query;
    Json1::Reader reader;
    std::ifstream ReadData("/usr/FINO/FPB/RESP/FetchExtraBene.TXT");
    bool parsedSuccess = reader.parse(ReadData,root,false);

    ReadData.close();

    if(not parsedSuccess)
    {

        qDebug()<<"FpsMonthlyPdsData File not Found";
        return FUNC_ERROR;
    }
    else
    {
  #if DEBUG
        qDebug()<<"RequestId   : "<<Convertstr(root["RequestId"].asString());
        qDebug()<<"DisplayMessage : "<<Convertstr(root["DisplayMessage"].asString());
        qDebug()<<"ResponseMessage    : "<<Convertstr(root["ResponseMessage"].asString());

        qDebug()<<"ResponseCode : "<<Convertstr(root["ResponseCode"].asString());
        qDebug()<<"MessageType : "<<Convertstr(root["MessageType"].asString());

#endif
        memclear((char *)add_bene_resp.ucRDResponseMessage_Resp);
        memset(add_bene_resp.ucRDResponseMessage_Resp, 0, sizeof(add_bene_resp.ucRDResponseMessage_Resp));
        memcpy(add_bene_resp.ucRDResponseMessage_Resp, (char *)Convertstr(root["ResponseMessage"].asString()), strlen((const char *)Convertstr(root["ResponseMessage"].asString())));
        strcpy(cResCode,Convertstr(root["ResponseCode"].asString()));
        //        qDebug()<<"ResponseData returnCode : "<<Convertstr(root["ResponseData"]["returnCode"].asString());
        //        qDebug()<<"ResponseData noOfRows: "<<Convertstr(root["ResponseData"]["noOfRows"].asString());
        //        qDebug()<<"AuthmanFlag : "<<Convertstr(root["AuthmanFlag"].asString());

        memset(eBene_beneficiaryId,       0, sizeof(eBene_beneficiaryId));
        memset(eBene_nickName,            0, sizeof(eBene_nickName));
        memset(eBene_beneficiaryName,     0, sizeof(eBene_beneficiaryName));
        memset(eBene_beneficiaryBank,     0, sizeof(eBene_beneficiaryBank));
        memset(eBene_beneficiaryBankCity, 0, sizeof(eBene_beneficiaryBankCity));
        //   memset(eBene_beneficiaryBankBranch,0,sizeof(eBene_beneficiaryBankBranch));
        memset(eBene_beneficiaryBranch,   0, sizeof(eBene_beneficiaryBranch));
        memset(eBene_beneficiaryBankIfsc, 0, sizeof(eBene_beneficiaryBankIfsc));
        memset(eBene_status,              0, sizeof(eBene_status));
        memset(eBene_beneficiaryEmailId,  0, sizeof(eBene_beneficiaryEmailId));
        memset(eBene_eBene_applicationId, 0, sizeof(eBene_eBene_applicationId));
        memset(eBene_beneficiaryType,     0, sizeof(eBene_beneficiaryType));
        memset(eBene_verificationStatus,  0, sizeof(eBene_verificationStatus));

        if(memcmp(cResCode,"0",1)==0)
        {
            Json1::Value ResponseData;
            ResponseData.clear();
            ResponseData = root.get("ResponseData",0);
            printf("\nSize : %d\n",ResponseData.size());

            if(ResponseData.size()==0)
            {
                qDebug("\n1:Key Register Data Not Found\n");
                return FUNC_ERROR;
            }
            else
            {
                Json1::Value ResponseData1;
                ResponseData1 = ResponseData.get("externalBeneficiaryInfoList",0);
                if(ResponseData1.size()==0)
                {
                    qDebug("\nexternalBeneficiaryInfoList failed\n");
                    if(strcmp((char*)add_bene_resp.ucRDResponseMessage_Resp,"")!=0)
                    {
                        CMisc miscObj;
                        char ErrorMsg[250];
                        memset(ErrorMsg, 0, sizeof(ErrorMsg));
                        sprintf(ErrorMsg,"%s",(char *)add_bene_resp.ucRDResponseMessage_Resp);
                        miscObj.DisplayMsgBox(ErrorMsg);
                        memclear((char *)add_bene_resp.ucRDResponseMessage_Resp);
                        memset(add_bene_resp.ucRDResponseMessage_Resp, 0, sizeof(add_bene_resp.ucRDResponseMessage_Resp));

                        uidmessage::instance()->hide_Message();
                    }
                    return FUNCTION_FAILED;

                }
                count_bene = 0;
                for(int j=0;j<ResponseData1.size();j++)
                {
                    count_bene++;
#if DEBUG
                    printf("count_bene = %d\n",count_bene);
#endif
                    strncpy((char*)eBene_beneficiaryId[j], (const char*)Convertstr(ResponseData1[j]["beneficiaryId"].asString()), strlen((const char*)Convertstr(ResponseData1[j]["beneficiaryId"].asString())));
                    strncpy((char*)eBene_nickName[j], (const char*)Convertstr(ResponseData1[j]["nickName"].asString()), strlen((const char*)Convertstr(ResponseData1[j]["nickName"].asString())));
                    strncpy((char*)eBene_beneficiaryName[j], (const char*)Convertstr(ResponseData1[j]["beneficiaryName"].asString()), strlen((const char*)Convertstr(ResponseData1[j]["beneficiaryName"].asString())));
                    strncpy((char*)eBene_beneficiaryBank[j], (const char*)Convertstr(ResponseData1[j]["beneficiaryBank"].asString()), strlen((const char*)Convertstr(ResponseData1[j]["beneficiaryBank"].asString())));
                    strncpy((char*)eBene_beneficiaryBankCity[j], (const char*)Convertstr(ResponseData1[j]["beneficiaryBankCity"].asString()), strlen((const char*)Convertstr(ResponseData1[j]["beneficiaryBankCity"].asString())));
                    //strncpy((char*)eBene_beneficiaryBankBranch[j], (const char*)Convertstr(ResponseData1[j]["beneficiaryBankBranch"].asString()), strlen((const char*)Convertstr(ResponseData1[j]["beneficiaryBankBranch"].asString())));
                    strncpy((char*)eBene_beneficiaryBranch[j], (const char*)Convertstr(ResponseData1[j]["beneficiaryBranch"].asString()), strlen((const char*)Convertstr(ResponseData1[j]["beneficiaryBranch"].asString())));
                    strncpy((char*)eBene_beneficiaryBankIfsc[j], (const char*)Convertstr(ResponseData1[j]["beneficiaryBankIfsc"].asString()), strlen((const char*)Convertstr(ResponseData1[j]["beneficiaryBankIfsc"].asString())));
                    strncpy((char*)eBene_status[j], (const char*)Convertstr(ResponseData1[j]["status"].asString()), strlen((const char*)Convertstr(ResponseData1[j]["status"].asString())));
                    strncpy((char*)eBene_beneficiaryEmailId[j], (const char*)Convertstr(ResponseData1[j]["beneficiaryEmailId"].asString()), strlen((const char*)Convertstr(ResponseData1[j]["beneficiaryEmailId"].asString())));
                    strncpy((char*)eBene_eBene_applicationId[j], (const char*)Convertstr(ResponseData1[j]["applicationId"].asString()), strlen((const char*)Convertstr(ResponseData1[j]["applicationId"].asString())));
                    strncpy((char*)eBene_beneficiaryType[j], (const char*)Convertstr(ResponseData1[j]["beneficiaryType"].asString()), strlen((const char*)Convertstr(ResponseData1[j]["beneficiaryType"].asString())));
                    strncpy((char*)eBene_verificationStatus[j], (const char*)Convertstr(ResponseData1[j]["verificationStatus"].asString()), strlen((const char*)Convertstr(ResponseData1[j]["verificationStatus"].asString())));
                    strncpy((char*)eBene_beneficiarysequence[j], (const char*)Convertstr(ResponseData1[j]["sequence"].asString()), strlen((const char*)Convertstr(ResponseData1[j]["sequence"].asString())));
#if DEBUG
                    printf("j = %d beneficiaryId = %s \n",j,eBene_beneficiaryId[j]);
                    printf("j = %d nickName = %s \n",j,eBene_nickName[j]);
                    printf("j = %d beneficiaryName = %s \n",j,eBene_beneficiaryName[j]);
                    printf("j = %d beneficiaryBank = %s \n",j,eBene_beneficiaryBank[j]);
                    printf("j = %d beneficiaryBankCity = %s \n",j,eBene_beneficiaryBankCity[j]);
                    //  printf("j = %d beneficiaryBankBranch = %s \n",j,eBene_beneficiaryBankBranch[j]);
                    printf("j = %d beneficiaryBranch = %s \n",j,eBene_beneficiaryBranch[j]);
                    printf("j = %d beneficiaryBankIfsc = %s \n",j,eBene_beneficiaryBankIfsc[j]);
                    printf("j = %d status = %s \n",j,eBene_status[j]);
                    printf("j = %d beneficiaryEmailId = %s \n",j,eBene_beneficiaryEmailId[j]);
                    printf("j = %d applicationId = %s \n",j,eBene_eBene_applicationId[j]);
                    printf("j = %d beneficiaryType = %s \n",j,eBene_beneficiaryType[j]);
                    printf("j = %d verificationStatus = %s \n\n",j,eBene_verificationStatus[j]);
                    printf("j = %d sequence = %s \n\n",j,eBene_beneficiarysequence[j]);
#endif
                    if(flag_bc_merchant_account == 'B')
                    {
#if DEBUG
                        qDebug()<<"BC Merchant Account";
                        printf("BC Merchant Account Count %d , Index %d\n", count_bc_merchant_acc, index_bc_merchant_acc);
#endif
                        if(memcmp((const char*)Convertstr(ResponseData1[j]["nickName"].asString()), "EMD",3)==0)
                        {
                            memset(FetchBeneList.ucbeneficiaryMaxLimit, 0, sizeof(FetchBeneList.ucbeneficiaryMaxLimit));
                            strncpy((char*)FetchBeneList.ucbeneficiaryMaxLimit, (const char*)Convertstr(ResponseData1[j]["beneficiaryMaxLimit"].asString()), strlen((const char*)Convertstr(ResponseData1[j]["beneficiaryMaxLimit"].asString())));
#if DEBUG
                            printf("FetchBeneList.ucbeneficiaryMaxLimit = %s\n",FetchBeneList.ucbeneficiaryMaxLimit);
#endif
                            count_bc_merchant_acc++;
                            index_bc_merchant_acc = j;
                            //return FUNC_OK;
                        }
                        //                        else
                        //                        {
                        //                            CMisc miscObj;
                        //                            count_bc_merchant_acc = 0;
                        //                            miscObj.DisplayMsgBox("Bene details not found");
                        //                            uidmessage::instance()->hide_Message();
                        //                            return FUNC_NEXT;

                        //                        }

                    }



                    //                    qDebug()<<"beneficiaryId :"<<Convertstr(ResponseData1[j]["beneficiaryId"].asString());
                    //                    qDebug()<<"nickName :"<<Convertstr(ResponseData1[j]["nickName"].asString());
                    //                    qDebug()<<"beneficiaryName :"<<Convertstr(ResponseData1[j]["beneficiaryName"].asString());
                    //                    qDebug()<<"beneficiaryBank  :"<<Convertstr(ResponseData1[j]["beneficiaryBank"].asString());
                    //                    qDebug()<<"beneficiaryBankCity :"<<Convertstr(ResponseData1[j]["beneficiaryBankCity"].asString());
                    //                    qDebug()<<"beneficiaryBranch :"<<Convertstr(ResponseData1[j]["beneficiaryBranch"].asString());
                    //                    qDebug()<<"beneficiaryBankIfsc  :"<<Convertstr(ResponseData1[j]["beneficiaryBankIfsc"].asString());
                    //                    qDebug()<<"status :"<<Convertstr(ResponseData1[j]["status"].asString());
                    //                    qDebug()<<"beneficiaryEmailId :"<<Convertstr(ResponseData1[j]["beneficiaryEmailId"].asString());
                    //                    qDebug()<<"applicationId :"<<Convertstr(ResponseData1[j]["applicationId"].asString());
                    //                    qDebug()<<"beneficiaryType :"<<Convertstr(ResponseData1[j]["beneficiaryType"].asString());
                    //                    qDebug()<<"verificationStatus :"<<Convertstr(ResponseData1[j]["verificationStatus"].asString());
                    //                    qDebug()<<"";

                }

                if(flag_bc_merchant_account == 'B')
                {
                    if(count_bc_merchant_acc != 0)
                    {
                        return FUNC_OK;
                    }
                    else
                    {
                        CMisc miscObj;
                        count_bc_merchant_acc = 0;
                        miscObj.DisplayMsgBox((char *)"Bene details not found");
                        uidmessage::instance()->hide_Message();
                        return FUNC_NEXT;
                    }
                }

            }
        }
        else
        {
            CMisc miscObj;
            uidmessage::instance()->hide_Message();

            if(strcmp((char*)add_bene_resp.ucRDResponseMessage_Resp,"")!=0)
            {
                char ErrorMsg[250];
                memset(ErrorMsg, 0, sizeof(ErrorMsg));

                if(strcmp((char *)add_bene_resp.ucRDResponseMessage_Resp,"Session Expired.")==0)
                {
                    memclear((char *)add_bene_resp.ucRDResponseMessage_Resp);
                    uidmessage::instance()->hide_Message();
                    return SESSION_EXPIRED_FAILED;
                }
                if(flag_bc_merchant_account == 'B' || (strcmp((char *)add_bene_resp.ucRDResponseMessage_Resp,"Record Not Found")==0))
                {
                    if(memcmp(cResCode,"100",1)==0)
                    {
                        sprintf(ErrorMsg,"%s",(char *)"Bene details not found.");
                        miscObj.DisplayMsgBox(ErrorMsg);
                        count_bc_merchant_acc = 0;
                        uidmessage::instance()->hide_Message();
                        return FUNC_NEXT;
                    }
                }

                sprintf(ErrorMsg,"%s",(char *)add_bene_resp.ucRDResponseMessage_Resp);
                miscObj.DisplayMsgBox(ErrorMsg);
            }
            memclear((char *)add_bene_resp.ucRDResponseMessage_Resp);
            memset(add_bene_resp.ucRDResponseMessage_Resp, 0, sizeof(add_bene_resp.ucRDResponseMessage_Resp));

            uidmessage::instance()->hide_Message();
            return FUNCTION_FAILED;
        }
    }
    return FUNC_OK;
}

int FPB::PostIMPS()
{
    uidmessage::instance()->hide_Message();
    uidmessage::instance()->Display_BusyMessage("Transaction Request in Progress");

    int iRet = 0;
    char* jsondata;
    QSqlQuery query;
    Json1::Value ArrayList(Json1::arrayValue), PostTransactionArrayList(Json1::arrayValue);
    Json1::Value addressesArraylist(Json1::arrayValue);
    Json1::Value CreateCustomerArraylist(Json1::arrayValue);
    Json1::Value acctFundTransfer, PostTransaction, IMPSServiceDataList, CreateCustomer;
    Json1::Value RequestedData, mainreq, SubData2;
    char buff1[100];
    query.clear();
    memset(buff1,0,sizeof(buff1));
    iGenerateRequestID((char *)username,(unsigned char *)buff1);
    //sprintf(buff1,"1234567890");

    QDate date = QDate::currentDate();
    QString cur_date = date.toString("dd/MM/yy");

    char itimedate[20];
    memset(itimedate, 0, sizeof(itimedate));
    strcpy(itimedate,iTimestamp());
    printf("Time Stamp == %s\n",itimedate);


    //--------------------------------  SUPPORT DATA  -------------------------------------

    Clear_SupportData();
    Clear_DeviceInfo();

    int iretDI = DeviceInfo();

    if(iretDI != FUNCTION_SUCCESS)
        return FUNCTION_FAILED;


    memcpy(CustDet.cAccountNm , (char *)"999999999999", 12);
    memcpy(CustDet.cCustomerNm, (char *)"999999999999", 12);
    memcpy(CustDet.cMobileNumber, FC_MobileNumber, strlen(FC_MobileNumber));

    memset((char *)postbeneverif.ABene_beneficiaryAccount_req, 0, strlen((char *)postbeneverif.ABene_beneficiaryAccount_req));
    memset((char *)postbeneverif.beneName_req,                 0, strlen((char *)postbeneverif.beneName_req));
    memset((char *)postbeneverif.ucRequestId_req,              0, sizeof((char *)postbeneverif.ucRequestId_req));
    memset((char *)postbeneverif.beneBank_req,                 0, strlen((char *)postbeneverif.beneBank_req));
    memset((char *)postbeneverif.Custmobileno_req,             0, sizeof((char *)postbeneverif.Custmobileno_req));
    memset((char *)postbeneverif.MerchantMobileNo_IMPSSDL_req, 0, sizeof((char *)postbeneverif.MerchantMobileNo_IMPSSDL_req));
    memset((char *)postbeneverif.requestbitmap_req,            0, sizeof((char *)postbeneverif.requestbitmap_req));

    memcpy((char *)postbeneverif.ABene_beneficiaryAccount_req, ui->Bene_acc_no_edit->text().trimmed().toAscii().data(),strlen(ui->Bene_acc_no_edit->text().trimmed().toAscii().data()));
    memcpy((char *)postbeneverif.beneName_req, (const char*)ui->Bene_Name_edit->text().trimmed().toAscii().data(),strlen((const char*)ui->Bene_Name_edit->text().trimmed().toAscii().data()));
    memcpy((char *)postbeneverif.ucRequestId_req, buff1,strlen(buff1));

    memcpy((char *)postbeneverif.beneBank_req, (const char *)ui->cmb_bankList->currentText().trimmed().toAscii().data(),strlen((const char *)ui->cmb_bankList->currentText().trimmed().toAscii().data()));
#if DEBUG
    printf("postbeneverif.beneBank_req = %s\n",postbeneverif.beneBank_req);
    printf("\n\n ui->cmb_bankList = %s \n\n",(const char *)ui->cmb_bankList->currentText().trimmed().toAscii().data());
#endif
    memcpy((char *)postbeneverif.Custmobileno_req,FC_MobileNumber, strlen(FC_MobileNumber));
    memcpy((char *)postbeneverif.MerchantMobileNo_IMPSSDL_req,(const char*)configLoginLeg2.ucmobileNumber,strlen((const char*)configLoginLeg2.ucmobileNumber));


    memcpy(postbeneverif.sd_SPROD_Debit, "1", 1);
    memcpy(postbeneverif.sd_SPROD, "5", 1);
    memcpy(postbeneverif.sd_PVTXNID, (char *)sd_deviceinfo.ucSupportData_PVTXNID_req, strlen((char *)sd_deviceinfo.ucSupportData_PVTXNID_req));
    memcpy(postbeneverif.sd_PVTXNDT, (char *)sd_deviceinfo.ucSupportData_PVTXNDT_req, strlen((char *)sd_deviceinfo.ucSupportData_PVTXNDT_req)); // cur_date.toAscii().data(), strlen(cur_date.toAscii().data()));
    memcpy(postbeneverif.sd_IPADDR, (char *)sd_deviceinfo.ucSupportData_IPADDR_req, strlen((char *)sd_deviceinfo.ucSupportData_IPADDR_req));
    memcpy(postbeneverif.sd_DEVICEID, (char *)sd_deviceinfo.ucSupportData_DEVICEID_req, strlen((char *)sd_deviceinfo.ucSupportData_DEVICEID_req));
    memcpy(postbeneverif.sd_APPID, CLIENT_ID, strlen(CLIENT_ID));
    memcpy(postbeneverif.sd_AUTHID, "@authid", 7);
    memcpy(postbeneverif.sd_LOCATION, "", 1);
    memcpy(postbeneverif.sd_CELLID, (char *)sd_deviceinfo.ucSupportData_CELLID_req, strlen((char *)sd_deviceinfo.ucSupportData_CELLID_req));
    memcpy(postbeneverif.sd_MCC, (char *)sd_deviceinfo.ucSupportData_MCC_req, strlen((char *)sd_deviceinfo.ucSupportData_MCC_req));
    memcpy(postbeneverif.sd_RPTFLG, "0", 1);
    memcpy(postbeneverif.sd_PRTXNID, (char *)sd_deviceinfo.ucSupportData_PRTXNID_req, strlen((char *)sd_deviceinfo.ucSupportData_PRTXNID_req));
    memcpy(postbeneverif.sd_PRTXNAMT, (char*)ui->Fund_transfer_amount_edit->text().toAscii().trimmed().data(), strlen(ui->Fund_transfer_amount_edit->text().toAscii().trimmed().data()));
    memcpy(postbeneverif.sd_SPLTSEQ, (char *)sd_deviceinfo.ucSupportData_SPLTSEQ_req, strlen((char *)sd_deviceinfo.ucSupportData_SPLTSEQ_req));
    //    memcpy(postbeneverif.sd_CHGAMT, (char *)sd_deviceinfo.ucSupportData_CHGAMT_req, strlen((char *)sd_deviceinfo.ucSupportData_CHGAMT_req));


    //--------------------------------                     13052019                  ---------------------------------------------------

    //    memcpy(postbeneverif.sd_SPROD_Debit, "1", 1);
    //    memcpy(postbeneverif.sd_SPROD, "5", 1);
    //    memcpy(postbeneverif.sd_PVTXNID, "910010027654", 12); // Remove Hard coded
    //    memcpy(postbeneverif.sd_PVTXNDT, cur_date.toAscii().data(), strlen(cur_date.toAscii().data()));
    //    memcpy(postbeneverif.sd_IPADDR, "10.37.24.161", 12);
    //    memcpy(postbeneverif.sd_DEVICEID, "18F0E48699A8", 12);
    //    memcpy(postbeneverif.sd_APPID, "FINOMERNP", 9);
    //    memcpy(postbeneverif.sd_AUTHID, "@authid", 7);
    //    memcpy(postbeneverif.sd_LOCATION, "", 1);
    //    memcpy(postbeneverif.sd_CELLID, "", 1);
    //    memcpy(postbeneverif.sd_MCC, "a4a23,50,404,20", 15);
    //    memcpy(postbeneverif.sd_RPTFLG, "0", 1);
    //    memcpy(postbeneverif.sd_PRTXNID, itimedate, strlen(itimedate));
    //    memcpy(postbeneverif.sd_PRTXNAMT, (char*)ui->Fund_transfer_amount_edit->text().toAscii().trimmed().data(), strlen(ui->Fund_transfer_amount_edit->text().toAscii().trimmed().data()))
    //    memcpy(postbeneverif.sd_SPLTSEQ, "0", 1);

    //---------------------------------------------------------------------------------------------------------------------------------

    char temp_amt[20];
    sprintf(temp_amt,"%d",CustDet.cChargesAmount);
    memcpy(postbeneverif.sd_CHGAMT, (char *)temp_amt, strlen(temp_amt));
    memcpy(postbeneverif.sd_CHGAMT_CREDIT, (char *)"0", 1);

    memcpy(postbeneverif.sd_ZRFUT1, (char *)postbeneverif.ABene_beneficiaryAccount_req, strlen((char *)postbeneverif.ABene_beneficiaryAccount_req));
    memcpy(postbeneverif.sd_ZRFUT2, (char*)ui->IFSC_edit->text().toAscii().trimmed().data(), strlen(ui->IFSC_edit->text().toAscii().trimmed().data()));
    memcpy(postbeneverif.sd_ZRFUT3, (char *)postbeneverif.beneBank_req, strlen((char *)postbeneverif.beneBank_req));
    if(flag_bc_merchant_account == 'B')
        memcpy(postbeneverif.sd_ZRFUT4, "EMD", 3);
    memcpy(postbeneverif.sd_ZRFUT5, (const char*)postbeneverif.beneName_req, strlen((const char*)postbeneverif.beneName_req));
    //    memcpy(postbeneverif.sd_ZRFUT6, "", 1);
    //    memcpy(postbeneverif.sd_ZRFUT7, "", 1);
    memcpy(postbeneverif.sd_ZRFUT8, "", 1);
    memcpy(postbeneverif.sd_ZRFUT9, (char *)postbeneverif.ucRequestId_req, strlen((char *)postbeneverif.ucRequestId_req));
    memcpy(postbeneverif.sd_ZRFUN3, username, strlen(username));
    memcpy(postbeneverif.sd_ZRFUN4, (char *)postbeneverif.Custmobileno_req, strlen((char *)postbeneverif.Custmobileno_req));
    //    memcpy(postbeneverif.sd_ZRFUN3, username, strlen(username));
    //    memcpy(postbeneverif.sd_ZRFUN4, (char *)postbeneverif.Custmobileno_req, strlen((char *)postbeneverif.Custmobileno_req));
    memcpy(postbeneverif.sd_NETID, "900000", 6);
    memcpy(postbeneverif.sd_MSGTYP, "210", 3);




    //______________________________   AMOUNT   _______________________________________

    //    memset(postbeneverif, 0, sizeof(postbeneverif));

    memcpy((char *)postbeneverif.ucAmount_req,"1",1);        // totalAmount
    //memcpy((char *)postbeneverif.ucAmount_PTAFT1_req,"3",1); // D-DMTCASHD
    //memcpy((char *)postbeneverif.ucAmount_PTAFT2_req,"3",1); // C-DMTCASHD
    memcpy((char *)postbeneverif.ucAmount_PTAFT3_req,"1",1); // D-DMTIMPSBENV
    memcpy((char *)postbeneverif.ucAmount_PTAFT4_req,"1",1); // C-DMTIMPSBENV
    memcpy((char *)postbeneverif.ucAmount_IMPSSDL_req,"1",1); // TxnAmount
    //memcpy((char *)postbeneverif.ucAmount_main_req,"1",1);    // Last Request Data
    postbeneverif.ucAmount_PTAFT1_req = 3;
    postbeneverif.ucAmount_PTAFT2_req = 3;
    postbeneverif.ucAmount_main_req = 1;

    //_____________________________   Mobile Number   _________________________________

    //    memcpy((char *)CustDet.cTxnAmount, "2", 1); // Temp Hardcoded 24052019 removed

    //_________________________________________________________________________________


    char charges[15];
    memset(charges,0,sizeof(charges));
    sprintf(charges,"%d", CustDet.cChargesAmount);

    //int kkkk = 102920;
    mainreq["RequestId"] =  buff1;
    //  if(flag_bc_merchant_account == 'B')
    if(flag_bitmap == 'W' || flag_bc_merchant_account == 'B')
        mainreq["MethodId"] = 104; //"104"; // 30042019 home
    else
        mainreq["MethodId"] = "104"; // 30042019 home

    mainreq["SessionId"] =  OtpImpsNeft.Commdata_OIN.ucSessionID_req;//(char *)configLoginLeg2.ucSessionId;//buff_tmp.toAscii().data();
    mainreq["TellerID"]= username;
    mainreq["TokenId"]= mainreq.null;
    mainreq["X_Auth_Token"] = "";
#if DEBUG
    qDebug()<<"SeSSION id"<<OtpImpsNeft.Commdata_OIN.ucSessionID_req;

    qDebug()<<"flag_bc_merchant_account == "<<flag_bc_merchant_account;
#endif
    if(flag_bc_merchant_account == 'B')
        mainreq["ProductCode"]= "1003|N";
    else
        mainreq["ProductCode"]= "9999|Y"; //"1102|N";

    mainreq["Charges"]=(char*)charges; //"10.0"; //1304

    mainreq["ChannelID"]= CHANNEL_ID_IMPS;
    mainreq["Amount"]= (char *)CustDet.cTxnAmount; // Temp Hardcoded 24052019 removed
    if(flag_bc_merchant_account == 'B')
        mainreq["MerchantContribution"] = "0";
    else
        mainreq["MerchantContribution"] = (char *)CustDet.cTxnAmount; //"100"; //0; //1304
    mainreq["IsEncrypt"]= ISENCEYPT;

    //  if(CHANNEL_ID_IMPS == 3)
    //      mainreq["JsonFormatter"]= true;  // Hardcoded 24052019 removed
    mainreq["SessionExpiryTime"]= OtpImpsNeft.Commdata_OIN.ucSessionExpiryTime_req;;//(char *)configLoginLeg2.ucSessionExpiryTime;//"10/11/2018 11:58:44 PM";
 #if DEBUG
    qDebug()<<"SeSSION EXPIRY LEG2"<<OtpImpsNeft.Commdata_OIN.ucSessionExpiryTime_req;
#endif


    if(flag_bitmap == 'W')                                              // For New Walking Customer
    {
        if(Flag_IMPS_Walkin == 'S')
            memcpy(postbeneverif.requestbitmap_req, "11011100", 8);     // To create and add Walkin Customer
        else
            memcpy(postbeneverif.requestbitmap_req, "11010100", 8);     // For Non-Walkin Customer..Only Create Customer

    }
    else if(Flag_IMPS_Walkin == 'S') // To Add Walking Customer 12062019
    {
        if(flag_bitmap == 'N')                                         // For New Beneficiary
            memcpy(postbeneverif.requestbitmap_req, "10011100", 8);
        else                                                                // For Registered Beneficiary
            memcpy(postbeneverif.requestbitmap_req, "10001100", 8);
    }
    else
    {
        if(flag_bitmap == 'N')                                         // For New Beneficiary
            memcpy(postbeneverif.requestbitmap_req, "10010100", 8);
        else                                                                // For Registered Beneficiary
            memcpy(postbeneverif.requestbitmap_req, "10000100", 8);
    }
#if DEBUG
    printf("\n************\nIMPS BITMAP == %s, flag_bitmap = %c, and  CUST_TYPE = %c\n************\n",(char *)postbeneverif.requestbitmap_req, flag_bitmap, CUST_TYPE);
#endif

    memset(postbeneverif.transactionComment_req, 0, sizeof(postbeneverif.transactionComment_req));
    memset(postbeneverif.accfundtransfer_txntype, 0, sizeof(postbeneverif.accfundtransfer_txntype));

    if(memcmp(Flag_Transaction_Type, "NEFT",4)==0)
        memcpy(postbeneverif.transactionComment_req, "FT NEFT", 7);
    else
        memcpy(postbeneverif.transactionComment_req, "FT IMPS", 7);

    if(flag_bc_merchant_account == 'B')
    {
        if(memcmp(Flag_Transaction_Type, "NEFT",4)==0)
            memcpy(postbeneverif.accfundtransfer_txntype, "NEFTFTP2A", 9);
        else
            memcpy(postbeneverif.accfundtransfer_txntype, "IMPSFTP2A", 9);
    }
    else
    {
        if(memcmp(Flag_Transaction_Type, "NEFT",4)==0)
            memcpy(postbeneverif.accfundtransfer_txntype, "DMTNEFT", 7);
        else
            memcpy(postbeneverif.accfundtransfer_txntype, "DMTIMPSP2A", 10);
    }

    QString Bitmap =(char *) postbeneverif.requestbitmap_req;
#if DEBUG
    qDebug()<<"Bitmap = "<<Bitmap;
#endif
    //RequestData
    RequestedData["requestbitmap"] = (char *)postbeneverif.requestbitmap_req;
    RequestedData["asyncFlag"] = "Y";
    RequestedData["totalAmount"] = (char *)CustDet.cTxnAmount; // Temp Hardcoded 24052019 removed
    RequestedData["mobileNumber"] = (char *)postbeneverif.Custmobileno_req; //"8454984086"; //CustDet.cMobileNumber; // Manendar = 7045993236 //1304
    if(flag_bc_merchant_account != 'B')
        RequestedData["merchantMobileNumber"] = (char *)postbeneverif.MerchantMobileNo_IMPSSDL_req; //"7400370122"; //"8055988351"; //1304
    RequestedData["txnType"] = (char *)Flag_Transaction_Type; //CustDet.cTxnType;
    //    memcpy(resp_postimps.ucRDTransactiCustDet.cBenificiaryNameonType_RES, (const char*)"IMPS", 4);

    RequestedData["cashoraccount"] = "CASH";
    RequestedData["impsserviceflag"] = "Y";


    //-----------------------------------------------              CustomerAuth              --------------------------------------------------

    //CustomerAuth
    RequestedData["CustomerAuth"]["MobileNo"] = (char *)postbeneverif.Custmobileno_req;
    RequestedData["CustomerAuth"]["AadharNo"] = RequestedData.null;

    if(flag_bitmap == 'W' || flag_bc_merchant_account == 'B')                                              // For New Walking Customer
        RequestedData["CustomerAuth"]["Auth_id"] = 1;
    else
        RequestedData["CustomerAuth"]["Auth_id"] = "1";

    RequestedData["CustomerAuth"]["Aadhaar"] = "";

    //    if(flag_bitmap != 'R')
    //        RequestedData["CustomerAuth"]["Fp_Auth"] = ""; // Hardcoded 24052019 removed

    //otp
    RequestedData["CustomerAuth"]["Otp"]["RequestId"] = buff1;//"TLR1032_10112018202736391";
    RequestedData["CustomerAuth"]["Otp"]["MethodId"] = 2;
    RequestedData["CustomerAuth"]["Otp"]["TellerID"] =username;//"TLR1032";
    RequestedData["CustomerAuth"]["Otp"]["SessionId"] =OtpImpsNeft.Commdata_OIN.ucSessionID_req;//buff_tmp.toAscii().data();//OtpImpsNeft.Commdata_OIN.ucSessionID_req;//"9d044213-09d9-400d-aa96-5e9a473ba494";
    RequestedData["CustomerAuth"]["Otp"]["TokenId"] = RequestedData.null;  // RequestedData.null; // Hardcoded 24052019 removed
    RequestedData["CustomerAuth"]["Otp"]["IsEncrypt"] = false;
    RequestedData["CustomerAuth"]["Otp"]["X_Auth_Token"] ="";
    RequestedData["CustomerAuth"]["Otp"]["SessionExpiryTime"] = OtpImpsNeft.Commdata_OIN.ucSessionExpiryTime_req;//"10/11/201811: 58: 44PM";
 #if DEBUG
    qDebug()<<"Session Expiry"<<OtpImpsNeft.Commdata_OIN.ucSessionExpiryTime_req;
#endif
    //    if(flag_bc_merchant_account != 'B')
    //    {
    //        RequestedData["CustomerAuth"]["Otp"]["RequestData"]["MethodId"] = "2";
    //        RequestedData["CustomerAuth"]["Otp"]["RequestData"]["RequestId"] = (char *)OtpBeneVer.ucRequestID_Resp; //OtpImpsNeft.Commdata_OIN.ucRequestID_res;//"161281"; //1304
    //        RequestedData["CustomerAuth"]["Otp"]["RequestData"]["CustomerMobileNo"] = (char *)postbeneverif.Custmobileno_req; //"8454984086"; //CustDet.cMobileNumber; //1304
    //        RequestedData["CustomerAuth"]["Otp"]["RequestData"]["OtpPin"]= CustDet.cOtp;//"";
    //        RequestedData["CustomerAuth"]["Otp"]["RequestData"]["MessageId"] = 39;//Change made  for encryption

    //    }
    //    else
    {
        SubData2["MethodId"] ="2";
        SubData2["RequestId"] = (char *)OtpBeneVer.ucRequestID_Resp;
        SubData2["CustomerMobileNo"] = (char *)postbeneverif.Custmobileno_req;
        SubData2["OtpPin"] = CustDet.cOtp;//"";
        SubData2["MessageId"] = 39;
        SubData2["OtpParam"] = "";

        //    ArrayList.append(SubData2);
        //    subRequestData["RequestData"] = ArrayList;
        buff_tmp.clear();
        buff_tmp = serialize1(Convertstr(SubData2.toStyledString()));
        //SubData["RequestData"]["CustomerAuth"]["Otp"] = buff_tmp.toAscii().data();
        RequestedData["CustomerAuth"]["Otp"]["RequestData"] = buff_tmp.toAscii().data();
    }

    //
    //-----------------------------------------------             CreateCustomer             --------------------------------------------------
    if(Bitmap.mid(1,1)=="1")
    {
        CreateCustomerArraylist.clear();

        CreateCustomer["employment"]["taxDetails"] =  CreateCustomer.null;
        CreateCustomer["idProof"] =  CreateCustomer.null;
        CreateCustomer["addressProof"] =  CreateCustomer.null;

        CreateCustomer["residencyCode"] = "1";
        CreateCustomer["customerType"] = "0";
        CreateCustomer["currencyCode"] = "INR";

        // --- personalDetails ---

        CreateCustomer["personalDetails"]["title"] = ui->cmbTitle->currentText().trimmed().toAscii().data();
        CreateCustomer["personalDetails"]["firstNm"] = (char *)CustDet.cFirstName;
        CreateCustomer["personalDetails"]["lastNm"] = (char *)CustDet.cLastName;
        CreateCustomer["personalDetails"]["name"] = (char *)CustDet.cCustName;
        CreateCustomer["personalDetails"]["lastNm"] = (char *)CustDet.cLastName;
        CreateCustomer["personalDetails"]["dob"] = (char *)CustDet.cDob;
        CreateCustomer["personalDetails"]["gender"] =(char *) CustDet.cGender;
        CreateCustomer["personalDetails"]["aadharNumber"] =  CreateCustomer.null;
        CreateCustomer["personalDetails"]["taxId"] =  CreateCustomer.null;
        CreateCustomer["personalDetails"]["accountNm"] = (char *)CustDet.cAccountNm;
        CreateCustomer["personalDetails"]["customerNm"] = (char *)CustDet.cCustomerNm;
        if(strcmp(CustDet.cGSTINNumber, "") == 0)
            CreateCustomer["personalDetails"]["cortexId"] =  CreateCustomer.null;
        else
            CreateCustomer["personalDetails"]["cortexId"] =  (char *)CustDet.cGSTINNumber;
      //  CreateCustomer["personalDetails"]["cortexId"] =  CreateCustomer.null;
        CreateCustomer["personalDetails"]["code"] = "1";
        CreateCustomer["personalDetails"]["branchOwnership"] = "1011";
        CreateCustomer["personalDetails"]["photo"] =  CreateCustomer.null;

        // --- contactDetails ---

        CreateCustomer["contactDetails"]["mobile"] = (char *)CustDet.cMobileNumber;
        CreateCustomer["contactDetails"]["email"] =  CreateCustomer.null;

        if(flag_bitmap != 'W')
            CreateCustomer["contactDetails"]["photo"] =  CreateCustomer.null;

        SubData2.clear();
        addressesArraylist.clear();
        for(int i=0;i<1;i++)
        {
            SubData2["addressType"] = "P";
            SubData2["address1"] = (char *)CustDet.cAddress;
            SubData2["postalCode"] = (char *)CustDet.cPincode;
            SubData2["city"] = (char *)CustDet.cCity;
            SubData2["state"] = (char *)CustDet.cStatecode;
            SubData2["country"] = (char *)CustDet.cCountrycode;
            //            SubData2["mobile"] = (char *)CustDet.cMobileNumber;

            addressesArraylist.append(SubData2);
            SubData2.clear();
        }
        CreateCustomer["addresses"] = addressesArraylist;

        CreateCustomerArraylist.append(CreateCustomer);


        RequestedData["CreateCustomer"] = CreateCustomerArraylist;
    }
    else
    {
        if(flag_bc_merchant_account != 'B')
            RequestedData["CreateCustomer"] = "";
    }
    // RequestedData["CreateAccount"] = "";

    //-----------------------------------------------             CreateAccount              --------------------------------------------------

    if(Bitmap.mid(2,1)=="1")
    {
        RequestedData["CreateAccount"] = "";
    }
    else
    {
        if(flag_bc_merchant_account != 'B')
            RequestedData["CreateAccount"] = "";
    }

    //-----------------------------------------------             AddBeneficiary             --------------------------------------------------
    if(Bitmap.mid(3,1)=="1" || flag_bc_merchant_account == 'B')
    {

        RequestedData["AddBeneficiary"]["appId"] = CLIENT_ID; //"FINOTLR";
        if(flag_bitmap == 'W')
            RequestedData["AddBeneficiary"]["customerNumber"] ="999999999999";
        else
            RequestedData["AddBeneficiary"]["customerNumber"] = username;
        RequestedData["AddBeneficiary"]["beneficiaryType"] ="3";
        if(flag_bc_merchant_account == 'B')
            RequestedData["AddBeneficiary"]["nickName"] = "EMD";
        else
            RequestedData["AddBeneficiary"]["nickName"] = RequestedData.null;
        RequestedData["AddBeneficiary"]["beneficiaryAccount"] = (char *)postbeneverif.ABene_beneficiaryAccount_req; //"5015000001120"; //CustDet.cAccountNumber;//"96786787687"; //1304
        RequestedData["AddBeneficiary"]["beneficiaryAccountType"] ="10";
        RequestedData["AddBeneficiary"]["beneficiaryName"] = (char *)postbeneverif.beneName_req; //"Saurabh Ranjane";// CustDet.cBenificiaryName;//"webtset"; //1304
        RequestedData["AddBeneficiary"]["beneficiaryBank"] = (char *)postbeneverif.beneBank_req; //"HDFC BANK LTD"; //CustDet.cBBankName;//"HDFCBANKLTD"; //1304
        RequestedData["AddBeneficiary"]["beneficiaryBankBranch"] = (char *)CustDet.cBranchName; //"HDFC BANK LTD"; //CustDet.cBBankName;//"HDFCBANKLTD"; //1304
        RequestedData["AddBeneficiary"]["beneficiaryBankCity"] = (char *)CustDet.cCity; // Hardcoded 24052019 removed
        RequestedData["AddBeneficiary"]["beneficiaryBankIfsc"] = (char*)ui->IFSC_edit->text().toAscii().trimmed().data();  //"HDFC0000240"; //CustDet.cIFSC_Code; //1304
        RequestedData["AddBeneficiary"]["beneficiaryAddress1"] = RequestedData.null; //CustDet.cAddress;//" ";
        RequestedData["AddBeneficiary"]["beneficiaryState"] = RequestedData.null; //CustDet.cstate; // Hardcoded 24052019 removed
        RequestedData["AddBeneficiary"]["beneficiaryCity"] = RequestedData.null; // CustDet.cCity; // Hardcoded 24052019 removed
        RequestedData["AddBeneficiary"]["beneficiaryZip"] = RequestedData.null;
        RequestedData["AddBeneficiary"]["beneficiaryEmailId"] = RequestedData.null;
        RequestedData["AddBeneficiary"]["beneficiaryBankMicr"] = RequestedData.null;
        if(flag_bitmap == 'W')                                              // For New Walking Customer
            RequestedData["AddBeneficiary"]["beneficiaryMaxLimit"] ="25000";
        else if(flag_bc_merchant_account == 'B')
            RequestedData["AddBeneficiary"]["beneficiaryMaxLimit"] = (char *)FetchBeneList.ucbeneficiaryMaxLimit;
        else
            RequestedData["AddBeneficiary"]["beneficiaryMaxLimit"] = RequestedData.null;

    }
    else
    {
        RequestedData["AddBeneficiary"] = "";
    }
    //-----------------------------------------------            AddWalkinCustomer           --------------------------------------------------
    if(Bitmap.mid(4,1)=="1")
    {
  #if DEBUG
        qDebug()<<"-----  In AddWalkinCustomer  -----";
 #endif
        RequestedData["AddWalkinCustomer"]["name"] = (char *)CustDet.cCustName;
        RequestedData["AddWalkinCustomer"]["mobileNumber"] = (char *)CustDet.cMobileNumber;
        RequestedData["AddWalkinCustomer"]["address"] = (char *)CustDet.cAddress;
        RequestedData["AddWalkinCustomer"]["gender"] =(char *) CustDet.cGender;
        RequestedData["AddWalkinCustomer"]["idProofType"] = "";
        RequestedData["AddWalkinCustomer"]["idProofNumber"] = "";
        RequestedData["AddWalkinCustomer"]["AddressProofType"] = "";
        RequestedData["AddWalkinCustomer"]["addressProofNumber"] = "";
    }
    else
    {
        RequestedData["AddWalkinCustomer"] = "";
    }

    //-----------------------------------------------             PostTransaction            --------------------------------------------------
    if(Bitmap.mid(5,1)=="1")
    {

        PostTransactionArrayList.clear();
        PostTransaction["appId"] = CLIENT_ID; //"FINOTLR";
        PostTransaction["valueDate"] = RequestedData.null;
        PostTransaction["isInclusive"] = 0;
        PostTransaction["isClubbed"] = 0;
        PostTransaction["analysisFlag"] = RequestedData.null;
        PostTransaction["reversalFlag"] = RequestedData.null;
        PostTransaction["referenceNo"] = itimedate; //"040419183605108"; //buff1; //"TLR1032_26201913202573"; //1304

        ArrayList.clear();
        //      for(int i=0;i<2;i++)
        //      {

        char SupportAmt[20+1] ={0};
        memset(SupportAmt,0,sizeof(SupportAmt));
        sprintf(SupportAmt,"DN1#%s",CustDet.CTotalAmount); // CustDet.cTxnAmount); // 12062019
#if DEBUG
        printf("SupportAmt = %s\n",SupportAmt);
#endif
        // Remove Hardcoded below support data
        char IMPS_SupportData[1000], IMPS_SupportData_Debit[1000];

        memset(IMPS_SupportData, 0, sizeof(IMPS_SupportData));

        sprintf(IMPS_SupportData,"SPROD#%s~PVTXNID#%s~PVTXNDT#%s~IPADDR#%s~DEVICEID#%s~APPID#%s~AUTHID#%s~LOCATION#%s~CELLID#%s~MCC#%s~RPTFLG#%s~PRTXNID#%s~PRTXNAMT#%s~SPLTSEQ#%s~CHGAMT#%s~ZRFUN3#%s~ZRFUN4#%s~ZRFUT5#%s~ZRFUT8#%s~ZRFUT9#%s",postbeneverif.sd_SPROD,postbeneverif.sd_PVTXNID,postbeneverif.sd_PVTXNDT,postbeneverif.sd_IPADDR,postbeneverif.sd_DEVICEID,postbeneverif.sd_APPID,postbeneverif.sd_AUTHID,postbeneverif.sd_LOCATION,postbeneverif.sd_CELLID,postbeneverif.sd_MCC,postbeneverif.sd_RPTFLG,postbeneverif.sd_PRTXNID,postbeneverif.sd_PRTXNAMT,postbeneverif.sd_SPLTSEQ,postbeneverif.sd_CHGAMT_CREDIT,postbeneverif.sd_ZRFUN3,postbeneverif.sd_ZRFUN4,postbeneverif.sd_ZRFUT5,postbeneverif.sd_ZRFUT8,postbeneverif.sd_ZRFUT9); //"1",itimedate,"192.168.43.149","192.168.43.149","4888CAFA4374","FINOMER","@authid","","19.062504,73.0198985","0,0,0,0","0",itimedate,"100","0","11.8","20016200561", "HDFC0000406", "HDFC BANK LTD.", "", "Saurabh Ranjane", username, "9913384086", "900000", "210");

        memset(IMPS_SupportData_Debit, 0, sizeof(IMPS_SupportData_Debit));

        //strcpy(IMPS_SupportData,"SPROD#1~PVTXNID#909412027412~PVTXNDT#04\\/04\\/19~IPADDR#10.135.67.12~DEVICEID#508F4C96C260~APPID#FINOMER~AUTHID#@authid~LOCATION#~CELLID#19.0624954,73.0CHGAMT#10.0~ZRFUT1#5015000001120~ZRFUT2#HDFC0000007~ZRFUT3#HDFC BANK LTD~ZRFUT4#~ZRFUT5#Binod~ZRFUN3#100013782~ZRFUN4#8454984086~NETID#900000~MSGTYP#210~ZRFUT8#~ZRFUT9#101631163_040419183605108");

        sprintf(IMPS_SupportData_Debit,"SPROD#%s~PVTXNID#%s~PVTXNDT#%s~IPADDR#%s~DEVICEID#%s~APPID#%s~AUTHID#%s~LOCATION#%s~CELLID#%s~MCC#%s~RPTFLG#%s~PRTXNID#%s~PRTXNAMT#%s~SPLTSEQ#%s~CHGAMT#%s~ZRFUT1#%s~ZRFUT2#%s~ZRFUT3#%s~ZRFUT4#%s~ZRFUT5#%s~ZRFUN3#%s~ZRFUN4#%s~NETID#%s~MSGTYP#%s~ZRFUT8#%s~ZRFUT9#%s",postbeneverif.sd_SPROD_Debit,postbeneverif.sd_PVTXNID,postbeneverif.sd_PVTXNDT,postbeneverif.sd_IPADDR,postbeneverif.sd_DEVICEID,postbeneverif.sd_APPID,postbeneverif.sd_AUTHID,postbeneverif.sd_LOCATION,postbeneverif.sd_CELLID,postbeneverif.sd_MCC,postbeneverif.sd_RPTFLG,postbeneverif.sd_PRTXNID,postbeneverif.sd_PRTXNAMT,postbeneverif.sd_SPLTSEQ,postbeneverif.sd_CHGAMT,postbeneverif.sd_ZRFUT1,postbeneverif.sd_ZRFUT2,postbeneverif.sd_ZRFUT3,postbeneverif.sd_ZRFUT4,postbeneverif.sd_ZRFUT5,postbeneverif.sd_ZRFUN3,postbeneverif.sd_ZRFUN4,postbeneverif.sd_NETID,postbeneverif.sd_MSGTYP,postbeneverif.sd_ZRFUT8,postbeneverif.sd_ZRFUT9); //"1",itimedate,"192.168.43.149","192.168.43.149","4888CAFA4374","FINOMER","@authid","","19.062504,73.0198985","0,0,0,0","0",itimedate,"100","0","11.8","20016200561", "HDFC0000406", "HDFC BANK LTD.", "", "Saurabh Ranjane", username, "9913384086", "900000", "210");


        if(flag_bc_merchant_account != 'B')
        {
            // ---- D ----

            acctFundTransfer["accountNumber"] = "";
 #if DEBUG
            qDebug()<<"accountNumber"<<(char *)configLoginLeg2.ucBLaccountNo;
  #endif
            acctFundTransfer["amount"] = (char *)CustDet.CTotalAmount; // (char*)CustDet.cTxnAmount; // 12062019
 #if DEBUG
            printf("amount = %s\n)",(char *)CustDet.CTotalAmount);
 #endif
            acctFundTransfer["currency"] = "INR";
            acctFundTransfer["creditDebitFlag"] = "D";
            acctFundTransfer["transactionType"] = "DMTCASHD"; //IMPSFTP2A";//"DMTCASHW";

            //            if(memcmp(Flag_Transaction_Type, "NEFT",4)==0)
            //                acctFundTransfer["transactionComment"] = "FT NEFT";
            //            else
            acctFundTransfer["transactionComment"] = (char *)postbeneverif.transactionComment_req; // "FT IMPS";

            //    acctFundTransfer["costCenter"] = "1011";
            if(flag_bitmap == 'W')
                acctFundTransfer["costCenter"] = 1011;
            else
                acctFundTransfer["costCenter"] = "1011";

            //sprintf(IMPS_SupportData,"SPROD#%s~PVTXNID#%s~PVTXNDT#%s~IPADDR#%s~DEVICEID#%s~APPID#%s~AUTHID#%s~LOCATION#%s~CELLID#%s~MCC#%s~RPTFLG#%s~PRTXNID#%s~PRTXNAMT#%s~SPLTSEQ#%s~CHGAMT#%s~ZRFUT1#%s~ZRFUT2#%s~ZRFUT3#%s~ZRFUT4#%s~ZRFUT5#%s~ZRFUN3#%s~ZRFUN4#%s~NETID#%s~MSGTYP#%s","1",itimedate,"192.168.43.149","192.168.43.149","4888CAFA4374","FINOMER","@authid","","19.062504,73.0198985","0,0,0,0","0",itimedate,"100","0","11.8","20016200561", "HDFC0000406", "HDFC BANK LTD.", "", "Saurabh Ranjane", username, "9913384086", "900000", "210");


            // strcpy(IMPS_SupportData,"SPROD#5~PVTXNID#909412027412~PVTXNDT#04\\/04\\/19~IPADDR#10.135.67.12~DEVICEID#508F4C96C260~APPID#FINOMER~AUTHID#@authid~LOCATION#~CELLID#19.0624954,73.0198883~MCC#c460f01,cb44,404,20~RPTFLG#0~PRTXNID#040419183605108~PRTXNAMT#800~SPLTSEQ#0~CHGAMT#0~ZRFUN3#100013782~ZRFUN4#8454984086~ZRFUT5#Dinesh Jondhale paunikar~ZRFUT8#~ZRFUT9#101631163_040419183605108");
            acctFundTransfer["supportData"] = SupportAmt; //IMPS_SupportData; //"SPROD#1~PVTXNID#110320182840582530~PVTXNDT#~IPADDR#10.15.14.164~DEVICEID#~APPID#FINOTLR~AUTHID#@authid~LOCATION#~CELLID#19.05396099999999773.0008734~MCC#~RPTFLG#0~PRTXNID#10112018175933319~PRTXNAMT#100~SPLTSEQ#0~CHGAMT#11.8~ZRFUT1#0103104000250603~ZRFUT2#IBKL0000103~ZRFUT3#IDBILTD~ZRFUT4#~ZRFUT5#dineshj~ZRFUT6#~ZRFUT7#~ZRFUT8#~ZRFUT9#TLR1032_10112018175850895~ZRFUT10#~ZRFUN1#0~ZRFUN2#0~ZRFUN3#100020442~ZRFUN4#8055988351~ZRFUDT#~NETID#900000~MSGTYP#210";//SupportAmt;//"DN1#5075";
            acctFundTransfer["beneficiaryRefOrMmid"] = "";
            acctFundTransfer["beneficiaryMobile"] = "";
            acctFundTransfer["remitterMobile"] =  (char *)postbeneverif.Custmobileno_req;
            acctFundTransfer["remitterMmid"] = "";
            acctFundTransfer["beneficiaryAccountNo"] = (char *)postbeneverif.ABene_beneficiaryAccount_req;
            acctFundTransfer["beneficiaryIfsc"] = (char*)ui->IFSC_edit->text().toAscii().trimmed().data();
            acctFundTransfer["remarks"] = CustDet.cRemark;
            ArrayList.append(acctFundTransfer);

            // ---- C ----

            acctFundTransfer["accountNumber"] = "";
            acctFundTransfer["amount"] = (char *)CustDet.CTotalAmount; // (char*)CustDet.cTxnAmount; //12062019
#if DEBUG
            qDebug()<<"accountNumber"<<(char *)configLoginLeg2.ucBLaccountNo;
            qDebug()<<"amount"<<(char *)configLoginLeg2.ucavailableBalance;
 #endif
            acctFundTransfer["currency"] = "INR";
            acctFundTransfer["creditDebitFlag"] = "C";
            acctFundTransfer["transactionType"] = "DMTCASHD";
            acctFundTransfer["transactionComment"] = (char *)postbeneverif.transactionComment_req;
            if(flag_bitmap == 'W')
                acctFundTransfer["costCenter"] = 1011;
            else
                acctFundTransfer["costCenter"] = "1011";

            acctFundTransfer["supportData"] = IMPS_SupportData;
            // Remove Hardcoded below support data
            //    acctFundTransfer["supportData"] = "SPROD#1~PVTXNID#110320182840582530~PVTXNDT#~IPADDR#10.15.14.164~DEVICEID#~APPID#FINOTLR~AUTHID#@authid~LOCATION#~CELLID#19.05396099999999773.0008734~MCC#~RPTFLG#0~PRTXNID#10112018175933319~PRTXNAMT#100~SPLTSEQ#0~CHGAMT#11.8~ZRFUT1#0103104000250603~ZRFUT2#IBKL0000103~ZRFUT3#IDBILTD~ZRFUT4#~ZRFUT5#dineshj~ZRFUT6#~ZRFUT7#~ZRFUT8#~ZRFUT9#TLR1032_10112018175850895~ZRFUT10#~ZRFUN1#0~ZRFUN2#0~ZRFUN3#100020442~ZRFUN4#8055988351~ZRFUDT#~NETID#900000~MSGTYP#210";//SupportAmt;//"DN1#5075";//SupportAmt;//"DN1#5075";
            acctFundTransfer["beneficiaryRefOrMmid"] = "";
            acctFundTransfer["beneficiaryMobile"] = "";
            acctFundTransfer["remitterMobile"] =   (char *)postbeneverif.Custmobileno_req;
            acctFundTransfer["remitterMmid"] = "";
            acctFundTransfer["beneficiaryAccountNo"] = "";
            acctFundTransfer["beneficiaryIfsc"] = "";
            acctFundTransfer["remarks"] = CustDet.cRemark;
            ArrayList.append(acctFundTransfer);

            //memset(IMPS_SupportData, 0, sizeof(IMPS_SupportData));
            //strcpy(IMPS_SupportData,"SPROD#1~PVTXNID#909412027412~PVTXNDT#04\\/04\\/19~IPADDR#10.135.67.12~DEVICEID#508F4C96C260~APPID#FINOMER~AUTHID#@authid~LOCATION#~CELLID#19.0624954,73.0198883~MCC#c460f01,cb44,404,20~RPTFLG#0~PRTXNID#040419183605108~PRTXNAMT#800~SPLTSEQ#0~CHGAMT#10.0~ZRFUT1#5015000001120~ZRFUT2#HDFC0000007~ZRFUT3#HDFC BANK LTD~ZRFUT4#~ZRFUT5#Binod~ZRFUN3#100013782~ZRFUN4#8454984086~NETID#900000~MSGTYP#210~ZRFUT8#~ZRFUT9#101631163_040419183605108");

        }
        // ---- D ----

        if(flag_bc_merchant_account == 'B')
            acctFundTransfer["accountNumber"] = (char *)CustDet.cFindCustAccountNo;
        else
            acctFundTransfer["accountNumber"] = "";
        acctFundTransfer["amount"] = (char*)CustDet.cTxnAmount; // Temp Hardcoded 24052019 removed
        acctFundTransfer["currency"] = "INR";
        acctFundTransfer["creditDebitFlag"] = "D";
        if(flag_bc_merchant_account == 'B')
            acctFundTransfer["transactionType"] = (char *)postbeneverif.accfundtransfer_txntype; // "IMPSFTP2A";
        else
            acctFundTransfer["transactionType"] = (char *)postbeneverif.accfundtransfer_txntype; // "DMTIMPSP2A";
        acctFundTransfer["transactionComment"] = (char *)postbeneverif.transactionComment_req; //  "FT IMPS";
        //   acctFundTransfer["costCenter"] = "1011";
        if(flag_bitmap == 'W' || flag_bc_merchant_account == 'B')
            acctFundTransfer["costCenter"] = 1011;
        else
            acctFundTransfer["costCenter"] = "1011";

        acctFundTransfer["supportData"] = IMPS_SupportData_Debit; //IMPS_SupportData; //1304
        acctFundTransfer["beneficiaryRefOrMmid"] = "";
        acctFundTransfer["beneficiaryMobile"] = "";
        acctFundTransfer["remitterMobile"] =  (char *)postbeneverif.Custmobileno_req;  //"8454984086"; //1304
        acctFundTransfer["remitterMmid"] = "";
        acctFundTransfer["beneficiaryAccountNo"] = (char *)postbeneverif.ABene_beneficiaryAccount_req; //"5015000001120"; //1304
        acctFundTransfer["beneficiaryIfsc"] = (char*)ui->IFSC_edit->text().toAscii().trimmed().data(); //"HDFC0000007"; //1304
        acctFundTransfer["remarks"] = CustDet.cRemark;
        ArrayList.append(acctFundTransfer);

        // ---- C ----

        acctFundTransfer["accountNumber"] = ""; // acctFundTransfer.null;
        acctFundTransfer["amount"] = (char*)CustDet.cTxnAmount; // Temp Hardcoded 24052019 removed
        acctFundTransfer["currency"] = "INR";
        acctFundTransfer["creditDebitFlag"] = "C";

        if(flag_bc_merchant_account == 'B')
            acctFundTransfer["transactionType"] = (char *)postbeneverif.accfundtransfer_txntype; // "IMPSFTP2A";
        else
            acctFundTransfer["transactionType"] = (char *)postbeneverif.accfundtransfer_txntype; // "DMTIMPSP2A";

        acctFundTransfer["transactionComment"] = (char *)postbeneverif.transactionComment_req; //  "FT IMPS";
        //  acctFundTransfer["costCenter"] = "1011";
        if(flag_bitmap == 'W' || flag_bc_merchant_account == 'B')
            acctFundTransfer["costCenter"] = 1011;
        else
            acctFundTransfer["costCenter"] = "1011";
        acctFundTransfer["supportData"] = "";
        acctFundTransfer["beneficiaryRefOrMmid"] = ""; //  acctFundTransfer.null;
        acctFundTransfer["beneficiaryMobile"] = ""; //  acctFundTransfer.null;

        if(flag_bc_merchant_account == 'B')
            acctFundTransfer["remitterMobile"] = acctFundTransfer.null;
        else
            acctFundTransfer["remitterMobile"] =   (char *)postbeneverif.Custmobileno_req;

        acctFundTransfer["remitterMmid"] = ""; //  acctFundTransfer.null;
        acctFundTransfer["beneficiaryAccountNo"] = ""; //  acctFundTransfer.null;
        acctFundTransfer["beneficiaryIfsc"] = ""; //  acctFundTransfer.null;
        acctFundTransfer["remarks"] = CustDet.cRemark;
        ArrayList.append(acctFundTransfer);


        PostTransaction["acctFundTransferLegs"] = ArrayList;
        PostTransactionArrayList.append(PostTransaction);
        RequestedData["PostTransaction"] = PostTransactionArrayList;

    }
    else
    {
        RequestedData["PostTransaction"] = "";
    }


    ArrayList.clear();

    //-----------------------------------------------               DMSDetails             --------------------------------------------------


    if(Bitmap.mid(6,1)=="1")
    {

    }
    else
    {
        if(flag_bc_merchant_account != 'B')
            RequestedData["DMSDetails"] = "";
    }
    //-----------------------------------------------            AddExternalAccount          --------------------------------------------------
    if(Bitmap.mid(7,1)=="1")
    {

    }
    else
    {
        if(flag_bc_merchant_account != 'B')
            RequestedData["AddExternalAccount"] = "";

    }

    //RequestedData["DMSDetails"] = "";

    //-----------------------------------------------            AddExternalAccount          --------------------------------------------------

    // RequestedData["AddExternalAccount"] = "";


    //-----------------------------------------------           IMPSServiceDataList          --------------------------------------------------

    IMPSServiceDataList["BeneAccNo"] = (char *)postbeneverif.ABene_beneficiaryAccount_req; //"5015000001120"; //CustDet.cAccountNumber;//"976776776555"; //1304
    IMPSServiceDataList["BeneIFSC"] = (char*)ui->IFSC_edit->text().toAscii().trimmed().data(); //"HDFC0000007"; //CustDet.cIFSC_Code;//"HDFC0000240"; //1304
    IMPSServiceDataList["BeneName"] = (char *)postbeneverif.beneName_req; //"Saurabh Ranjane"; //CustDet.cBenificiaryName;//"BeneVerification"; //1304
    IMPSServiceDataList["MerchantMobileNo"] = (char *)postbeneverif.MerchantMobileNo_IMPSSDL_req; // "7400370122"; //(char *)configLoginLeg2.ucmobileNumber;//"9167252942"; //1304
#if DEBUG
    qDebug()<<"MerchantMobileNo"<<(char *)configLoginLeg2.ucmobileNumber;
#endif


    IMPSServiceDataList["PaymentRef"] = buff1; //"101631163_040419183605108"; //"262019115938299"; //1304
    IMPSServiceDataList["RemeMobileNo"] = (char *) postbeneverif.Custmobileno_req; //"8454984086"; //1304
    if(flag_bitmap == 'W' || flag_bc_merchant_account == 'B')
        IMPSServiceDataList["RemeName"] = (char *)CustDet.cCustName;
    else
        IMPSServiceDataList["RemeName"] = (char *)SearchWalkCust.SWCL_name_resp;
    IMPSServiceDataList["TxnAmount"] = (char*)CustDet.cTxnAmount; // Temp Hardcoded 24052019
    ArrayList.append(IMPSServiceDataList);

    RequestedData["IMPSServiceDataList"] = ArrayList;

    //-----------------------------------------------------------------------------------------------------------------------------------------

    serialize1(Convertstr(RequestedData.toStyledString()));

    mainreq["RequestData"] = buff_tmp.toAscii().data();

    jsondata = Convertstr(mainreq.toStyledString());
#if DEBUG
    qDebug()<<"------------->"<<jsondata;
#endif    memset(gSendData,0,sizeof(gSendData));
    strncpy((char*)gSendData, jsondata, strlen(jsondata));

    /****************************Encryption function*********************************/
    if(!strcmp(ISENCEYPT,"true"))
    {
        int gSendData_len =strlen((char *)gSendData);
        RequestDetachEncryptAttach((char *)gSendData,2,gSendData_len);
    }
    /***************************************************************/

    memset(str_url,0,sizeof(str_url));
#ifdef PRODUCTION
    strcpy(str_url,URL_CREATION(LOGIN_URL_PROD));
#else
    strcpy(str_url,URL_CREATION((char *)COMMON_URL));
#endif

    system("rm /usr/FINO/FPB/RESP/POSTIMPS.TXT");
    int retk = file_download(str_url, (char *)gSendData,(char *)"/usr/FINO/FPB/RESP/POSTIMPS.TXT",120);
    if(retk!=0)
    {
        sleep(1);
        uidmessage::instance()->hide_Message();
        return FUNCTION_FAILED;
    }

   // file_download( "http://103.1.112.205:4444//UIService.svc/NonFinancialProcessRequest",(char *)gSendData,"/usr/FINO/FPB/RESP/POSTIMPS.TXT",120);
    /****************************Decryption function*********************************/
    if(!strcmp(ISENCEYPT,"true"))
    {
        int oBuff_len = strlen(oBuff);
#if DEBUG
        qDebug()<<"Length of buffer"<<oBuff_len;
        printf("Buffer%s\n",oBuff);
#endif
        ResponseDetachEncryptAttach(oBuff,2,oBuff_len);//TEMPORARY

        system("rm /usr/FINO/FPB/RESP/POSTIMPS.TXT");
        FILE *fp2 = NULL;
        fp2 =fopen("/usr/FINO/FPB/RESP/POSTIMPS.TXT","a");
        fwrite(oBuff,strlen((char *)oBuff),1,fp2);
        sleep(1);
        fclose(fp2);
    }
    /*******************************************************************************/


    iRet = POST_IMPS_Parsing();


    if(iRet == FUNCTION_SUCCESS || iRet == FUNC_OK)
    {
        char szDate[16], szTime[16];
        memset(szDate, 0, sizeof(szDate));
        memset(szTime, 0, sizeof(szTime));
        GetLocalTime(szDate, szTime);

        char iquery[200];
        query.clear();

        query.exec("delete from LastWithdrawalData");
        query.clear();
        memset(iquery,0,sizeof(iquery));
        sprintf(iquery, "INSERT INTO LastWithdrawalData (LAST_RRN, LAST_DATE) VALUES('%s','%s')", resp_postimps.ucRDTransactionRefNo_RES, szDate);
#if DEBUG
        printf("QUERY 1 === %s\n",iquery);
#endif
        if(!query.exec(iquery))
        {
            qDebug() << "In Insert_Transaction RRN = " << query.lastError();
            query.clear();
            query.finish();
        }
        query.clear();
        query.finish();


        QString bal = QString::fromUtf8((const char*)resp_postimps.Commondata_POBV_RES.ucIMPS_AvilableBalance);
        bool decimal;
        decimal = bal.contains('.');
        if(decimal == false)
        {
            bal = bal.append(".00");
        }
        memset(Balance_Display,0,strlen(Balance_Display));
        sprintf(Balance_Display,"Balance : %s\n",bal.toLatin1().data()); // ,strlen(bal.toLatin1().data())); // 13052019
        strncpy(Mer_Latest_Balance, bal.toLatin1().data(),strlen(bal.toLatin1().data()));
        ui->Dash_Latest_Balance_Label_pg12->setText(Balance_Display);

        //    ui->SubTransaction_Label1->setText((char *)configLoginLeg2.ucname);
        //    ui->SubTransaction_Label1->setFont(QFont("SansSerif",10));
        //    ui->SubTransaction_Label1->setAlignment(Qt::AlignRight | Qt::AlignBottom);
#if DEBUG
        printf("Balance_Display = %s\n",Balance_Display);
#endif
        ui->SubTransaction_Label2->setText(QString::fromUtf8((const char*)Balance_Display));
        ui->SubTransaction_Label2->setFont(QFont("SansSerif",9));
        ui->SubTransaction_Label2->setAlignment(Qt::AlignRight | Qt::AlignBottom);
    }

    uidmessage::instance()->hide_Message();
    return iRet;

}




void FPB::on_Personal_Back_button_clicked()
{
    //    CMisc miscObj;
    //    uidmessage::instance()->hide_Message();
    //    int ret = miscObj.DisplayMsgBox_SHG("Are you sure, you want to discard the process?");
    //    uidmessage::instance()->hide_Message();
    //    if(ret == true)
    //    {
    //        ui->stackedWidget->setCurrentIndex(12);
    //    }
    uidmessage::instance()->hide_Message();
    ui->stackedWidget->setCurrentIndex(15);
    return;
}


int FPB::BankList() //Vaibhav
{
    //qDebug()<<"In BankSelection  :"<<Buff_BankName;
    CMisc miscObj;
    QSqlQuery query;
    query.clear();
    char Display[150];
    memset(Display,0,strlen(Display));

    //    if(!query.exec("select BANK_NAME from imps_mapping where IMPS_STATUS = '1'"))

    if(!query.exec("select BANK_NAME from imps_mapping"))
    {
        qDebug() << query.lastError();
        return FUNC_ERROR;
    }
    else
    {
        ui->cmb_bankList->clear(); // clear list
        ui->cmb_bankList->setFont(QFont("Courier",10));
        while(query.next())
        {
            if((query.value(0).toString() != NULL))// && (query.value(1).toString() != NULL))
            {
                memset(Display,0,strlen(Display));

                if(strlen((char *)query.value(0).toString().toAscii().data())>36)
                {
                    memcpy(Display,(char *)query.value(0).toString().toAscii().data(),35);
                    strcat(Display," \n");
                    strcat(Display,(char *)query.value(0).toString().mid(35,strlen((char *)query.value(0).toString().toAscii().trimmed().data())).toAscii().trimmed().data());
 #if DEBUG
                    printf("------------   BANK : %s   -------------\n",Display);
  #endif
                }
                else
                {
                    memcpy((char *)Display, (char *)query.value(0).toString().toAscii().trimmed().data(), strlen((const char *)query.value(0).toString().toAscii().trimmed().data()));

                }
                //ui->cmb_bankList->addItem(query.value(0).toString());
                ui->cmb_bankList->addItem(Display);
            }
        }
    }

    return FUNC_OK;

}



void FPB::on_Bene_previous_button_clicked()
{
    ui->Bene_acc_no_edit->setDisabled(false);
    ui->cmb_bankList->setDisabled(false);
    flag_bene_verfication_status = 0;
    if(CUST_TYPE == 'W')
    {
        ui->stackedWidget->setCurrentIndex(12);
    }
    else
    {
        ui->stackedWidget->setCurrentIndex(35);
    }
}


void FPB::on_Bene_IFSC_Back_button_clicked()
{
    ui->stackedWidget->setCurrentIndex(18);
    return;
}
void FPB::on_IFSC_cancel_Button_2_clicked()
{
    ui->cmb_state_2->setVisible(true);
    ui->cmb_District_2->setVisible(true);
    ui->Bank_name_label_2->setVisible(true);
    ui->cmb_BranchName_2->setVisible(true);
    ui->lblIFSC_Code_2->setVisible(true);
    ui->txtIFSC_code_2->setVisible(true);
    ui->stackedWidget->setCurrentIndex(19);
}

void FPB::on_IMPS_Select_Button_clicked()
{
    memset(Flag_Transaction_Type, 0, sizeof(Flag_Transaction_Type));
    memcpy(Flag_Transaction_Type, "IMPS", 4);
#if DEBUG
    printf("*****      TRANSACTION TYPE : %s     *****\n", Flag_Transaction_Type);
#endif
    ui->IMPS_Select_Button->setStyleSheet("color: rgb(255, 255, 255); background-color:  rgb(85, 0, 127)");
    ui->NEFT_Button->setStyleSheet("color: rgb(85, 0, 127); background-color:  rgb(255, 255, 255)");
    return;
}

void FPB::on_NEFT_Button_clicked()
{
    memset(Flag_Transaction_Type, 0, sizeof(Flag_Transaction_Type));
    memcpy(Flag_Transaction_Type, "NEFT", 4);
#if DEBUG
    printf("*****      TRANSACTION TYPE : %s     *****\n", Flag_Transaction_Type);
#endif
    ui->NEFT_Button->setStyleSheet("color: rgb(255, 255, 255); background-color:  rgb(85, 0, 127)");
    ui->IMPS_Select_Button->setStyleSheet("color: rgb(85, 0, 127); background-color:  rgb(255, 255, 255)");
    return;
}

void FPB::on_Calculate_charge_button_clicked()
{
    memset(CustDet.cTxnType, 0, sizeof(CustDet.cTxnType));
    memcpy(CustDet.cTxnType, (char *)Flag_Transaction_Type, strlen((const char *)Flag_Transaction_Type));

    CMisc miscObj;
    char ErrorMsg[200];
    memset(ErrorMsg, 0, sizeof(ErrorMsg));
    float cc_amount = atof(ui->Fund_transfer_amount_edit->text().toAscii().trimmed().data());
    if(strcmp(ui->Fund_transfer_amount_edit->text().toAscii().trimmed().data(), "")==0)
    {
        uidmessage::instance()->hide_Message();
        miscObj.DisplayMsgBox((char *)"Amount is mandatory  ");
        uidmessage::instance()->hide_Message();
        return;
    }

    QSqlQuery query;
    QString temp_amt;
    unsigned char query_amount[15], query_minAmount[15], query_maxAmount[15];
    memset(query_amount, 0, sizeof(query_amount));
    memset(query_minAmount, 0, sizeof(query_minAmount));
    memset(query_maxAmount, 0, sizeof(query_maxAmount));

    query.clear();
    if(flag_bc_merchant_account == 'B') // Must be uncomment before production
        query.prepare("SELECT RFU, MinTransLimit,MaxTransLimit from GetProfile where TransactionType = 'IMPSFTP2A' AND ProductTypeID = '1003'");
    else
        query.prepare("SELECT RFU, MinTransLimit,MaxTransLimit from GetProfile where TransactionType = 'DMTIMPSP2A' AND ProductTypeID = '9999'");

    //        query.prepare("SELECT RFU2, MIN_TRANS_LIMIT from PROFILE_TYPE_TRANSACTION where TRANSACTION_TYPE = 'DMTIMPSP2A' AND PRODUCT_TYPE_ID = '9999'");

    if(!query.exec())
    {
        qDebug() << query.lastError()<<"data base entry error";
        return ;
    }
    else
    {
        while(query.next())
        {
            temp_amt = query.value(0).toString().trimmed();
            memset(query_minAmount, 0, sizeof(query_minAmount));
            memset(query_maxAmount, 0, sizeof(query_maxAmount));
            memcpy((char *)query_minAmount, (char *)query.value(1).toString().trimmed().toAscii().data(), strlen((char *)query.value(1).toString().trimmed().toAscii().data()));
            memcpy((char *)query_maxAmount, (char *)query.value(2).toString().trimmed().toAscii().data(), strlen((char *)query.value(2).toString().trimmed().toAscii().data()));
#if DEBUG
            qDebug()<<"temp_amt = "<<temp_amt;
            printf("Minimum Txn Amount = %s and Maximum Transaction Amount = %s\n",query_minAmount, query_maxAmount);
 #endif
        }
    }

    query.finish();
    temp_amt = temp_amt.mid(0, temp_amt.indexOf("|"));
 #if DEBUG
    qDebug()<<"temp_amt sd = "<<temp_amt;
  #endif

    memset(query_amount, 0, sizeof(query_amount));
    memcpy(query_amount, temp_amt.toAscii().trimmed().data(), strlen(temp_amt.toAscii().trimmed().data()));

    float CustTotalAmount = atof((const char*)query_amount) - atof((const char*)Cust_CurrMonthAmount);
#if DEBUG
    printf("\nQuery Cust Amount = %s\n", query_amount);
    printf("\nCustTotalAmount = %f and Merchant Amount = %s\n", CustTotalAmount, (char *)Balance_Display);
    printf("\natof(ui->Fund_transfer_amount_edit->text().toAscii().trimmed().data()) = %f\n", atof(ui->Fund_transfer_amount_edit->text().toAscii().trimmed().data()));
#endif

    QString Acc_Bal;
    Acc_Bal = QString::fromUtf8((const char*)Balance_Display).remove("Balance : ");
 #if DEBUG
    qDebug()<<"Acc_Bal"<<Acc_Bal.trimmed();
#endif

    if(ui->Fund_transfer_amount_edit->text().trimmed().toFloat() > Acc_Bal.toFloat()) // For Merchant Limit
    {
        uidmessage::instance()->hide_Message();
        sprintf(ErrorMsg,"Amount should not be greater than Merchant balance(%s)",Acc_Bal.toAscii().trimmed().data());
        miscObj.DisplayMsgBox(ErrorMsg);
        uidmessage::instance()->hide_Message();
        return;
    }


    if(atof(ui->Fund_transfer_amount_edit->text().toAscii().trimmed().data()) > CustTotalAmount) // For Customer Limit
    {
        uidmessage::instance()->hide_Message();
        sprintf(ErrorMsg,"Amount should not be greater than customer available limit(%.2f)",CustTotalAmount);
        miscObj.DisplayMsgBox(ErrorMsg);
        uidmessage::instance()->hide_Message();
        return;
    }

    if(cc_amount < atof((char *)query_minAmount) || cc_amount > atof((const char*)query_maxAmount))
    {
        uidmessage::instance()->hide_Message();
        if(cc_amount < atof((char *)query_minAmount))
        {
            sprintf(ErrorMsg,"Amount should be greater than MinTransaction limit (%s)",query_minAmount);
        }
        else if(cc_amount > atof((const char*)query_maxAmount))
        {
            sprintf(ErrorMsg,"Amount should not be greater than MaxTransaction limit (%s)",query_maxAmount);
        }

        miscObj.DisplayMsgBox(ErrorMsg);
        uidmessage::instance()->hide_Message();
        return;
    }
    int iRet = 0;
    bool decimal;
    QString temp_amount;
    temp_amount.clear();
    temp_amount = ui->Fund_transfer_amount_edit->text().trimmed();
    decimal = temp_amount.contains('.');
    if(decimal == false)
    {
        temp_amount = temp_amount.append(".00");
    }
    memset(CustDet.cTxnAmount, 0, sizeof(CustDet.cTxnAmount));
    memcpy(CustDet.cTxnAmount, temp_amount.trimmed().toAscii().data(), strlen(temp_amount.trimmed().toAscii().data()));  //  ui->Fund_transfer_amount_edit->text().trimmed().toAscii().data());

    iRet = IMPSCharges();
    uidmessage::instance()->hide_Message();
    if(iRet != FUNCTION_SUCCESS || iRet != FUNC_OK)
    {
        if(iRet == SESSION_EXPIRED_FAILED)
            ui->stackedWidget->setCurrentIndex(0);
        else
            ui->stackedWidget->setCurrentIndex(21);
        return;
    }
    else if(iRet == FUNCTION_SUCCESS)
    {
        ui->stackedWidget->setCurrentIndex(21);
        return;
    }
}

void FPB::on_Discard_otp_Button_2_clicked()
{
    if(strcmp(CustDet.cAddBeneWithouTxn, "Y") == 0 || OTP_page == 4)
        ui->stackedWidget->setCurrentIndex(19);
    else
        ui->stackedWidget->setCurrentIndex(21);
    return;
}

void FPB::on_Personal_Back_button_26_clicked()
{
    isearchby = 0;
    ui->lblIFSC_Code_2->setVisible(true);
    ui->lblIFSC_Code_2->setGeometry(20,90,281,25);
    ui->txtIFSC_code_2->setVisible(true);
    ui->cmb_state_2->hide();
    ui->cmb_District_2->hide();
    ui->Bank_name_label_2->hide();
    ui->cmb_BranchName_2->hide();
    ui->cmb_bankList_4->hide();
    ui->Personal_Back_button_26->setStyleSheet("color: rgb(255, 255, 255); background-color:  rgb(85, 0, 127)");
    ui->Next_back_button_36->setStyleSheet("color: rgb(85, 0, 127); background-color:  rgb(255, 255, 255)");
    ui->stackedWidget->setCurrentIndex(20);
    return;
}

int FPB::Search_by_IFSC_Code()
{
    int iRet = 0;
    char* jsondata;
    Json1::Value ArrayList(Json1::arrayValue),PostTransactionArrayList(Json1::arrayValue);;

    Json1::Value acctFundTransfer,PostTransaction,IMPSServiceDataList;

    Json1::Value RequestedData,mainreq;
    char buff1[100];
    memset(buff1,0,sizeof(buff1));
    iGenerateRequestID((char *)username,(unsigned char *)buff1);
    //sprintf(buff1,"1234567890");

    //int kkkk = 102920;
    mainreq["RequestId"] =  buff1;
    mainreq["MethodId"] = "15";
    mainreq["TellerID"]= username;
    mainreq["SessionId"] = (char *)configLoginLeg2.ucSessionId;//buff_tmp.toAscii().data();
    mainreq["TokenId"]= "";
    mainreq["IsEncrypt"]= ISENCEYPT;
    mainreq["X_Auth_Token"] ="";
    mainreq["SessionExpiryTime"]= (char *)configLoginLeg2.ucSessionExpiryTime;//"10/11/2018 11:58:44 PM";
    mainreq["ChannelID"]= CHANNEL_ID_IMPS;
    mainreq["ProductCode"]= "";
    mainreq["Amount"]= "";
    //qDebug()<<"SeSSION EXPIRY LEG2"<<OtpImpsNeft.Commdata_OIN.ucSessionExpiryTime_req;

    RequestedData["ifscCode"] = CustDet.cIFSC_Code;
    RequestedData["bankCode"] = "";
    RequestedData["city"] = "";//(char *)configLoginLeg2.ucBLaccountNo;//"00000100";

    serialize1(Convertstr(RequestedData.toStyledString()));

    mainreq["RequestData"] = buff_tmp.toAscii().data();

    jsondata = Convertstr(mainreq.toStyledString());
#if DEBUG
    qDebug()<<"------------->"<<jsondata;
#endif

    memset(gSendData,0,sizeof(gSendData));
    strncpy((char*)gSendData, jsondata, strlen(jsondata));
 #if DEBUG
    printf("gSendData = %s\n",gSendData);
 #endif
    /****************************Encryption function*********************************/
   if(!strcmp((char *)ISENCEYPT,"true"))
    {
       int gSendData_len =strlen((char *)gSendData);
        RequestDetachEncryptAttach((char *)gSendData,1,gSendData_len);
        qDebug()<<"After encrypt function";
    }
    /***************************************************************/

    memset(str_url,0,sizeof(str_url));
#ifdef PRODUCTION
    strcpy(str_url,URL_CREATION(LOGIN_URL_PROD));
#else
    strcpy(str_url,URL_CREATION((char *)COMMON_URL));
#endif

    system("rm /usr/FINO/FPB/RESP/Search_IFSC_Code.TXT");
    int retk = file_download(str_url, (char *)gSendData,(char *)"/usr/FINO/FPB/RESP/Search_IFSC_Code.TXT",120);
    if(retk!=0)
    {
        sleep(1);
        uidmessage::instance()->hide_Message();
        return FUNCTION_FAILED;
    }




    /****************************Decryption function*********************************/
    if(!strcmp((char *)ISENCEYPT,"true"))

    {
        int oBuff_len = strlen(oBuff);
#if DEBUG
        qDebug()<<"Length of buffer"<<oBuff_len;
        printf("Buffer%s\n",oBuff);
#endif
        ResponseDetachEncryptAttach(oBuff,2,oBuff_len);//TEMPORARY

        system("rm /usr/FINO/FPB/RESP/Search_IFSC_Code.TXT");
        FILE *fp2 = NULL;
        fp2 =fopen("/usr/FINO/FPB/RESP/Search_IFSC_Code.TXT","a");
        fwrite(oBuff,strlen((char *)oBuff),1,fp2);
        sleep(1);
        fclose(fp2);
    }
    /*******************************************************************************/


    iRet = Search_by_IFSC_Code_Parsing();
    return iRet;
}

int FPB::Search_by_Bank_name()
{
    char* jsondata;
    int iRet = 0;


    Json1::Value RequestedData,mainreq;
    char buff1[100];
    memset(buff1,0,sizeof(buff1));
    iGenerateRequestID((char *)username,(unsigned char *)buff1);
    //sprintf(buff1,"1234567890");
#if DEBUG
    printf("CustDet.cBankName = %s\n\n",CustDet.cBankName);
    printf("CustDet.cCity = %s\n\n",CustDet.cCity);
    printf("CustDet.cBranchstate = %s\n\n",CustDet.cBranchstate);
#endif
    //int kkkk = 102920;
    mainreq["RequestId"] =  buff1;
    mainreq["MethodId"] = 15;
    mainreq["TellerID"]= username;
    mainreq["SessionId"] = (char *)configLoginLeg2.ucSessionId;//buff_tmp.toAscii().data();
    mainreq["TokenId"]= "";
    mainreq["IsEncrypt"]= false;
    mainreq["X_Auth_Token"] ="";
    mainreq["SessionExpiryTime"]= (char *)configLoginLeg2.ucSessionExpiryTime;//"10/11/2018 11:58:44 PM";
    mainreq["ChannelID"]= CHANNEL_ID_IMPS;
    mainreq["ProductCode"]= "";
    mainreq["Amount"]= "";
    //qDebug()<<"SeSSION EXPIRY LEG2"<<OtpImpsNeft.Commdata_OIN.ucSessionExpiryTime_req;
    RequestedData["ifscCode"] = "";
    RequestedData["bankCode"] = CustDet.cBankcode; //"ICIC";
    RequestedData["branch"] = "";//CustDet.cBranchName;
    RequestedData["city"] = CustDet.cBranchDistrict; // "Mumbai";//CustDet.cBranchDistrict;//(char *)configLoginLeg2.ucBLaccountNo;//"00000100";

    serialize1(Convertstr(RequestedData.toStyledString()));

    mainreq["RequestData"] = buff_tmp.toAscii().data();

    jsondata = Convertstr(mainreq.toStyledString());
#if DEBUG
    qDebug()<<"------------->"<<jsondata;
#endif
    memset(gSendData,0,sizeof(gSendData));
     strncpy((char*)gSendData, jsondata, strlen(jsondata));
  #if DEBUG
     printf("gSendData = %s\n",gSendData);
  #endif
//     /****************************Encryption function*********************************/
//    if(!strcmp((char *)ISENCEYPT,"true"))
//     {
//        int gSendData_len =strlen((char *)gSendData);
//         RequestDetachEncryptAttach((char *)gSendData,1,gSendData_len);
//         qDebug()<<"After encrypt function";
//     }
//     /***************************************************************/

     memset(str_url,0,sizeof(str_url));
 #ifdef PRODUCTION
     strcpy(str_url,URL_CREATION(LOGIN_URL_PROD));
 #else
     strcpy(str_url,URL_CREATION((char *)COMMON_URL));
 #endif

     system("rm /usr/FINO/FPB/RESP/Search_Bank_Name.TXT");
     int retk = file_download(str_url, (char *)gSendData,(char *)"/usr/FINO/FPB/RESP/Search_Bank_Name.TXT",120);
     if(retk!=0)
     {
         sleep(1);
         uidmessage::instance()->hide_Message();
         return FUNCTION_FAILED;
     }



   //  file_download( "http://103.1.112.205:4444//UIService.svc/NonFinancialProcessRequest", jsondata,"/usr/FINO/FPB/RESP/Search_IFSC_Code.TXT",120);

//     /****************************Decryption function*********************************/
//     if(!strcmp((char *)ISENCEYPT,"true"))

//     {
//         int oBuff_len = strlen(oBuff);
// #if DEBUG
//         qDebug()<<"Length of buffer"<<oBuff_len;
//         printf("Buffer%s\n",oBuff);
// #endif
//         ResponseDetachEncryptAttach(oBuff,2,oBuff_len);//TEMPORARY

//         system("rm /usr/FINO/FPB/RESP/Search_Bank_Name.TXT");
//         FILE *fp2 = NULL;
//         fp2 =fopen("/usr/FINO/FPB/RESP/Search_Bank_Name.TXT","a");
//         fwrite(oBuff,strlen((char *)oBuff),1,fp2);
//         sleep(1);
//         fclose(fp2);
//     }
//     /*******************************************************************************/



  //  file_download( "http://103.1.112.205:4444//UIService.svc/NonFinancialProcessRequest", jsondata,"/usr/FINO/FPB/RESP/Search_Bank_Name.TXT",120);
    iRet = Search_by_Bank_Name_Parsing();
    return iRet;
}


void FPB::on_pushButton_17_clicked()
{
    memset(Flag_Transaction_Type, 0, sizeof(Flag_Transaction_Type));
    if(flag_bc_merchant_account == 'B' )
    {
        ui->Bill_payment_Button->setVisible(false);
        ui->Reversal_Button->setVisible(false);

        if(!strcmp((char *)FINDCUST.ucRDCLnumber,username) == 0)//added on 25/07/2019 by CIFF Number
        {
            ui->Remittance_button->setVisible(false);
        }
        else
        {
            ui->Remittance_button->setVisible(true);
        }

//        if(!strcmp((char *)CustDet.cFindCustAccountNo,(char*)configLoginLeg2.ucmerchantAccount) == 0) //added on 25/07/2019 by Account Number
//        {
//            ui->Remittance_button->setVisible(false);
//        }
//        else
//        {
//            ui->Remittance_button->setVisible(true);
//        }
    }

    memcpy(Flag_Transaction_Type, "IMPS", 4);
    ui->IMPS_Select_Button->setStyleSheet("color: rgb(255, 255, 255); background-color:  rgb(85, 0, 127)");
    ui->NEFT_Button->setStyleSheet("color: rgb(85, 0, 127); background-color:  rgb(255, 255, 255)");
    ui->stackedWidget->setCurrentIndex(15);
    return;
}


void FPB::on_Customer_detail_back_Button_3_clicked()
{
    ///////////////////////////////////////////// 02052019 ///////////////////////////////////////////////////

    CMisc miscObj;
    uidmessage::instance()->hide_Message();
    int ret = miscObj.DisplayMsgBox_SHG("Are you sure, you want to discard the process?");
    uidmessage::instance()->hide_Message();
    if(ret == true)
    {
        ui->stackedWidget->setCurrentIndex(12);
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    ui->Search_lineEdit->setFocus();
    //    ui->stackedWidget->setCurrentIndex(12);
    return;
}


void FPB::on_Fund_transfer_amount_edit_textChanged(const QString &arg1)
{
    ui->Calculate_charge_button->setText("Calculate Charge");
    ui->Calculate_charge_button->setEnabled(true);
    ui->Calculate_charge_button->setStyleSheet("background-color: rgb(85, 0, 127); color: rgb(255, 255, 255)");
    ui->lbl_TotalAmount->setText("     0.00");
    ui->lbl_CashContribution->setText("     0.00");
    ui->Cash_contri_label->setVisible(false);
    ui->lbl_CashContribution->setVisible(false);
    Calculate_impscharges =0;
    return;
}

void FPB::on_cmb_BranchName_2_activated(int index)
{
    int index1 = ui->cmb_bankList->findText(QString::fromUtf8((const char*)Searchbybranch.ucbankDesc_Resp[index]));
    ui->cmb_bankList->setCurrentIndex(index1);
#if DEBUG
    printf("\nCOMBO Bank List = %s\n",ui->cmb_bankList->currentText().trimmed().toAscii().data());
#endif
    memset(CustDet.cBankName, 0, sizeof(CustDet.cBankName));
    memcpy(CustDet.cBankName, (const char*)ui->cmb_bankList->currentText().trimmed().toAscii().data(), strlen((const char*)ui->cmb_bankList->currentText().trimmed().toAscii().data()));

    ui->Ifsc_label->setVisible(true);
    ui->label_19->setVisible(true);
    ui->Bank_code_label->setText(QString::fromUtf8((const char*)Searchbybranch.ucbankCode_Resp[index]));
    ui->Bank_name_label->setText(QString::fromUtf8((const char*)Searchbybranch.ucbankDesc_Resp[index]));
    ui->Branch_label->setText(QString::fromUtf8((const char*)Searchbybranch.ucbranchDesc_Resp[index]));
    ui->City_label->setText(QString::fromUtf8((const char*)Searchbybranch.uccity_Resp[index]));
    ui->Ifsc_label->setText(QString::fromUtf8((const char*)Searchbybranch.ucifscCode_Resp[index]));
    qDebug()<<"Bank code = "<<QString::fromUtf8((const char*)Searchbybranch.ucbankCode_Resp[index]);
#ifdef DEBUG
    qDebug()<<"Bank_name_label = "<<QString::fromUtf8((const char*)Searchbybranch.ucbankDesc_Resp[index]);
    qDebug()<<"Branch_label = "<<QString::fromUtf8((const char*)Searchbybranch.ucbranchDesc_Resp[index]);
    qDebug()<<"City_label = "<<QString::fromUtf8((const char*)Searchbybranch.uccity_Resp[index]);
    qDebug()<<"Ifsc_label = "<<QString::fromUtf8((const char*)Searchbybranch.ucifscCode_Resp[index]);

    printf("CustDet.cBranchName = %s\n", CustDet.cBranchName);
#endif


    ui->stackedWidget->setCurrentIndex(36);
    return;
}

void FPB::on_pushButton_7_clicked()
{
    ui->IFSC_edit->clear();

    if(isearchby ==0)
    {
        ui->IFSC_edit->setText(ui->txtIFSC_code_2->text());
    }
    else
    {
        ui->IFSC_edit->setText(ui->Ifsc_label->text());

    }
    ui->stackedWidget->setCurrentIndex(19);
    return;
}


void FPB::on_pushButton_6_clicked()
{
    ui->stackedWidget->setCurrentIndex(19);
    return;
}
