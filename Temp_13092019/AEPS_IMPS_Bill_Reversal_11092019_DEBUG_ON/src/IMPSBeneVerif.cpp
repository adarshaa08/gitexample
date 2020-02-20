//#include "impscommon.h"
//#include "ui_FPB.h"
//#include "json.h"
//#include "Login.h"
//#include "displaymessage.h"
//#include "print.h"
////#include "FPB.h"
//#include <QSqlDatabase>
//#include<fstream>

#include "ui_FPB.h"
#include "displaymessage.h"
#include "Login.h"
#include "impscommon.h"
#include "time.h"
#include "findcust.h"
extern char * Convertstr(string str);
#include<fstream>

OTPBENEVERIF OtpBeneVer;
extern Login_leg2 configLoginLeg2;
extern int OTP_page;
extern searchWalkCust SearchWalkCust;
POSTBENEVER_RESP resp_pBenever;
POSTBENEVER postbeneverif;
POSTBENEVER_CHARGES_RESP beneverifcharges;
extern FINDCUSTOMER FINDCUST;
extern sCustDetails CustDet;
extern SUPPORTDATA_DEVICEINFO sd_deviceinfo;
extern unsigned char Total_BeneVerif_ChargesAmount[10];
extern char flag_bc_merchant_account;
int Flag_BeneVerif_Charges = 0;
char Flag_Transaction_Type[10];


void FPB::on_Verify_button_clicked()
{
    int iRet = 0;
    Flag_BeneVerif_Charges = 0;
    ui->Next_Otp_Button_2->setVisible(false);
    ui->FINISH_PushButton_Pg34->setVisible(true);

    iRet = Otpbenever();
    if(iRet != FUNCTION_SUCCESS)
    {
        ui->Next_Otp_Button_2->setVisible(true);
        ui->FINISH_PushButton_Pg34->setVisible(false);
        uidmessage::instance()->hide_Message();
        if(iRet == SESSION_EXPIRED_FAILED)
            ui->stackedWidget->setCurrentIndex(0);
        else
            ui->stackedWidget->setCurrentIndex(18);
        return;
    }
    else if(iRet == FUNCTION_SUCCESS)
    {
        uidmessage::instance()->hide_Message();
        ui->OTP_Lineedit->clear();
        ui->stackedWidget->setCurrentIndex(22);
        return;
    }
}


int FPB::Otpbenever()
{
    int iRet = 0;
    CMisc miscObj;
    Json1::Value RequestedData,SubData1,SubData2;
    char nullbuff[5];
    memset(nullbuff, 0, sizeof(nullbuff));
    strcpy(nullbuff,"");
    char buff1[100];
    memset(buff1,0,sizeof(buff1));
    uidmessage::instance()->hide_Message();
    uidmessage::instance()->Display_BusyMessage("Please Wait...");

    iGenerateRequestID((char *)username,(unsigned char *)buff1);

    RequestedData["RequestId"] = buff1;//"TLR1032_2620191138138";
    RequestedData["MethodId"] = 1;
    RequestedData["ChannelID"]= CHANNEL_ID_IMPS;
    RequestedData["SessionId"] = (char *)configLoginLeg2.ucSessionId; //buff_tmp.toAscii().data();//"9d044213-09d9-400d-aa96-5e9a473ba494";
    RequestedData["TellerID"]= (char *)username; // Dhiral
    RequestedData["TokenId"]= "";
    RequestedData["X_Auth_Token"] ="";
    RequestedData["IsEncrypt"]= false;
    RequestedData["ProductCode"] ="";
    RequestedData["Amount"] ="";
    RequestedData["SessionExpiryTime"]= (char *)configLoginLeg2.ucSessionExpiryTime;//"10/11/2018 11:58:44 PM";

    SubData1["MethodId"] = "1";
    SubData1["CustomerMobileNo"] = (char *)FC_MobileNumber;
    SubData1["MessageId"] = 42;
    SubData1["OtpParam"]["AccountNo"] = "";
    buff_tmp.clear();
    //    serialize1(Convertstr1(SubData2.toStyledString()));
    //    SubData1["OtpParam"] = buff_tmp.toAscii().data();

    buff_tmp.clear();
    serialize1(Convertstr1(SubData1.toStyledString()));
    RequestedData["RequestData"] = buff_tmp.toAscii().data();

    //    serialize1(Convertstr1(SubData2.toStyledString()));
    //    RequestedData["RequestData"] = buff_tmp.toAscii().data();



    char *Ret;
    Ret=Convertstr1(RequestedData.toStyledString());
#if DEBUG
    qDebug()<<"\nRequest Data :"<<Ret;
#endif
    system("rm -rf /usr/FINO/FPB/RESP/BENEVERIF_OTP.TXT");\
    memset(oBuff, 0, sizeof(oBuff));


    memset(str_url,0,sizeof(str_url));
#ifdef PRODUCTION
    strcpy(str_url,URL_CREATION(LOGIN_URL_PROD));
#else
    strcpy(str_url,URL_CREATION((char *)OTP_POST_IMPS));
#endif

    system("rm /usr/FINO/FPB/RESP/BENEVERIF_OTP.TXT");
    int ierror = file_download(str_url, (char *)Ret,(char *)"/usr/FINO/FPB/RESP/BENEVERIF_OTP.TXT",120);
    if(ierror!=0)
    {
        sleep(1);
        uidmessage::instance()->hide_Message();
        return FUNCTION_FAILED;
    }

    //   int ierror = file_download( "http://103.1.112.205:4444/UIService.svc/UtilityOtp", Ret,"/usr/FINO/FPB/RESP/BENEVERIF_OTP.TXT",120);

    QString OTPBuff = QString::fromLocal8Bit(oBuff);
    bool check = OTPBuff.contains("ResponseCode");
    if(check == false && ierror != FUNC_ERROR)
    {
        miscObj.DisplayMsgBox((char *)"Invalid Data");
        uidmessage::instance()->hide_Message();
        return FUNCTION_FAILED;
    }


    OTPBuff.remove("\\");
    OTPBuff.remove("{");
    OTPBuff.remove("}");
    OTPBuff.remove('"');
#if DEBUG
    qDebug()<<"OTPBuff"<<OTPBuff;
#endif

    QStringList query = OTPBuff.split(",");

#if DEBUG

    for(int i=0 ; i < query.length() ; i++)
        qDebug()<<"Query Response"<<query.at(i);
#endif

    QStringList RequestId = query.filter("RequestId");
    QStringList DisplayMessage = query.filter("DisplayMessage");
    QStringList ResponseCode = query.filter("ResponseCode");
    QStringList ResponseMessage = query.filter("ResponseMessage");

    uidmessage::instance()->hide_Message();

    QString str ,strToFind,value,value1,value2;
    str = ResponseCode.at(0);
    strToFind = ":";
    value1 = str.mid(str.indexOf(strToFind)+strToFind.length());
    //memcpy(OTP.ucResponseCode,value1.trimmed().toAscii().data(), strlen(value1.trimmed().toAscii().data()));
#if DEBUG
    qDebug()<<"value1"<<value1;
#endif
    if(value1 =="0")
    {
        str = DisplayMessage.at(2);
        strToFind = ":";
        value2 = str.mid(str.indexOf(strToFind)+strToFind.length());
#if DEBUG
        qDebug()<<"value2"<<value2;
#endif
        //memcpy(OTP.ucDisplayMessage,value2.trimmed().toAscii().data(), strlen(value2.trimmed().toAscii().data()));
        str= RequestId.at(2);
        strToFind= "RequestId:";
        value = str.mid(str.indexOf(strToFind)+strToFind.length());
#if DEBUG
        qDebug()<<"value = "<<value;
#endif
        memset(OtpBeneVer.ucRequestID_Resp , 0, sizeof(OtpBeneVer.ucRequestID_Resp));
        memcpy(OtpBeneVer.ucRequestID_Resp ,value.trimmed().toAscii().data(), strlen(value.trimmed().toAscii().data()));

        char otp_mobileno[20], otp_message[400];
        memset(otp_mobileno, 0, sizeof(otp_mobileno));
        memset(otp_message, 0, sizeof(otp_message));
        char custmob[11];
        memset(custmob, 0, sizeof(custmob));
        memcpy(custmob, FC_MobileNumber,strlen(FC_MobileNumber));
        sprintf(otp_mobileno,"******%s", &custmob[6]);
        sprintf(otp_message, "One Time Password (OTP) has been\n sent to your registered mobile %s,\n Please enter the same here",otp_mobileno);
        ui->Otp_display_label->setText(otp_message);

        OTP_page = 4;
        // ui->stackedWidget->setCurrentIndex(22);
        return FUNCTION_SUCCESS;
    }
    else
    {
        char ErrorMsg[205];
        memset(ErrorMsg, 0, sizeof(ErrorMsg));
        str = ResponseMessage.at(0);
        strToFind = ":";
        value2 = str.mid(str.indexOf(strToFind)+strToFind.length());
        memcpy(OtpBeneVer.ucResponsemsg_Resp,value2.trimmed().toAscii().data(), strlen(value2.trimmed().toAscii().data()));
        if(strcmp((char *)OtpBeneVer.ucResponsemsg_Resp,"Session Expired.")==0)
        {
            uidmessage::instance()->hide_Message();
            sprintf(ErrorMsg,"%s",(char *)OtpBeneVer.ucResponsemsg_Resp);
            miscObj.DisplayMsgBox(ErrorMsg);
            uidmessage::instance()->hide_Message();
            //            ui->stackedWidget->setCurrentIndex(0);
            return SESSION_EXPIRED_FAILED;
        }
        sprintf(ErrorMsg,"%s",/*(char *)OTP.ucResponseCode,*/(char *)OtpBeneVer.ucResponsemsg_Resp);
        miscObj.DisplayMsgBox(ErrorMsg);
        uidmessage::instance()->hide_Message();
        return FUNCTION_FAILED;
    }


    //    iRet = BeneVerifOTP_Parsing();
    //    if(iRet != FUNC_OK)
    //        return FUNCTION_FAILED;
    //    return FUNCTION_SUCCESS;

}


int FPB::BeneVerificationCharges()
{
    int iRet = 0;
    Flag_BeneVerif_Charges = 1;
    iRet = PostBeneVerification();
    return iRet;

}

int FPB::PostBeneVerification()
{
    int iRet = 0;
    Json1::Value ArrayList(Json1::arrayValue),PostTransactionArrayList(Json1::arrayValue);
    Json1::Value acctFundTransfer,PostTransaction,IMPSServiceDataList,BillDeskData;
    Json1::Value RequestedData,mainreq, subdata;
    char buff1[100];
    memset(buff1,0,sizeof(buff1));

    //--------------------------------  CLEAR SUPPORT DATA  -------------------------------------
    Clear_SupportData();
    //-------------------------------------------------------------------------------------------
    memset(postbeneverif.Custmobileno_req, 0, sizeof(postbeneverif.Custmobileno_req));
    memset(postbeneverif.MerchantMobileNo_IMPSSDL_req, 0, sizeof(postbeneverif.MerchantMobileNo_IMPSSDL_req));
    memset(postbeneverif.cashoraccount_req, 0, sizeof(postbeneverif.cashoraccount_req));
    memset(postbeneverif.txnType_req, 0, sizeof(postbeneverif.txnType_req));
    memset(postbeneverif.requestbitmap_req, 0, sizeof(postbeneverif.requestbitmap_req));
    memset(postbeneverif.appId_req, 0, sizeof(postbeneverif.appId_req));
    memset(postbeneverif.analysisFlag_req, 0, sizeof(postbeneverif.analysisFlag_req));
    memset(postbeneverif.debit_creditDebitFlag_req, 0, sizeof(postbeneverif.debit_creditDebitFlag_req));
    memset(postbeneverif.impsserviceflag_req, 0, sizeof(postbeneverif.impsserviceflag_req));
    memset(postbeneverif.reversalFlag_req, 0, sizeof(postbeneverif.reversalFlag_req));
    memset(postbeneverif.Accountno_req, 0, sizeof(postbeneverif.Accountno_req));
    memset(postbeneverif.currency_req, 0, sizeof(postbeneverif.currency_req));
    memset(postbeneverif.first_supportdata_req, 0, sizeof(postbeneverif.first_supportdata_req));
    memset(postbeneverif.BillerId_req, 0, sizeof(postbeneverif.BillerId_req));
    memset(postbeneverif.second_supportdata_req, 0, sizeof(postbeneverif.second_supportdata_req));
    memset(postbeneverif.debit_creditDebitFlag_req, 0, sizeof(postbeneverif.debit_creditDebitFlag_req));
    memset(postbeneverif.first_transactionType_req, 0, sizeof(postbeneverif.first_transactionType_req));
    memset(postbeneverif.transactionComment_req, 0, sizeof(postbeneverif.transactionComment_req));
    memset(postbeneverif.credit_creditDebitFlag_req, 0, sizeof(postbeneverif.credit_creditDebitFlag_req));
    memset(postbeneverif.second_transactionType_req, 0, sizeof(postbeneverif.second_transactionType_req));
    memset(postbeneverif.costCenter_req, 0, sizeof(postbeneverif.costCenter_req));

    memset((char *)postbeneverif.ucCharges_req,    0, sizeof((char *)postbeneverif.ucCharges_req));
    memset((char *)postbeneverif.ucOriginalCharges,    0, sizeof((char *)postbeneverif.ucOriginalCharges));

    //---------------------------------------------------------------------------------------------

    uidmessage::instance()->hide_Message();

    if(Flag_BeneVerif_Charges == 1)
        uidmessage::instance()->Display_BusyMessage("Please Wait...");
    else
        uidmessage::instance()->Display_BusyMessage("Transaction Request in Progress");

    char itimedate[20];
    memset(itimedate, 0, sizeof(itimedate));
    strcpy(itimedate,iTimestamp());

#if DEBUG
    printf("Time Stamp == %s\n",itimedate);
#endif

    QDate date = QDate::currentDate();
    QString cur_date = date.toString("dd/MM/yy");

    iGenerateRequestID((char *)username,(unsigned char *)postbeneverif.ucRequestId_req);

    memset(Flag_Transaction_Type, 0, sizeof(Flag_Transaction_Type));
    memcpy(Flag_Transaction_Type, "IMPS", 4);

    postbeneverif.ucMethodId = 104;
    memcpy(postbeneverif.ucTellerId_req,username,sizeof(username));
    memcpy((char *)postbeneverif.ucSessionId_req,configLoginLeg2.ucSessionId,strlen((char *)configLoginLeg2.ucSessionId));
    memcpy((char *)postbeneverif.ucIsEncrypt_req,"false",5); // Not used


    memcpy((char *)postbeneverif.asyncFlag_req,"N",1);

    if(flag_bc_merchant_account == 'B')
        memcpy((char *)postbeneverif.ucProductCode_req,"1003|N",6); // 15052019
    else
        memcpy((char *)postbeneverif.ucProductCode_req,"9999|Y",6);

    if(Flag_BeneVerif_Charges == 1) // Beneverification Charges
    {
        strcpy((char *)postbeneverif.ucCharges_req,"");
        memcpy((char *)postbeneverif.ucMerchantContribution_req,"0",1); // 06052019

    }
    else                                                        // Bene Verification
    {
        memcpy((char *)postbeneverif.ucCharges_req, (char *)Total_BeneVerif_ChargesAmount, strlen((const char *)Total_BeneVerif_ChargesAmount));
        memcpy((char *)postbeneverif.ucOriginalCharges, (char *)beneverifcharges.ucRDPTOriginalCharges_RES, strlen((char *)beneverifcharges.ucRDPTOriginalCharges_RES));
        memcpy((char *)postbeneverif.ucMerchantContribution_req,"1",1); // 06052019
    }

    //______________________________   AMOUNT   _______________________________________

    if(Flag_BeneVerif_Charges == 1)
    {
        memcpy((char *)postbeneverif.ucAmount_req,"1",1);        // totalAmount
        postbeneverif.ucAmount_main_req = 1;
        memcpy((char *)postbeneverif.ucAmount_PTAFT3_req,"1",1); // D-DMTIMPSBENV
        memcpy((char *)postbeneverif.ucAmount_PTAFT4_req,"1",1); // C-DMTIMPSBENV
        memcpy((char *)postbeneverif.ucAmount_IMPSSDL_req,"1",1); // TxnAmount

    }
    else
    {
        memcpy((char *)postbeneverif.ucAmount_req,"1",1);        // totalAmount
        //memcpy((char *)postbeneverif.ucAmount_PTAFT1_req,"3",1); // D-DMTCASHD
        //memcpy((char *)postbeneverif.ucAmount_PTAFT2_req,"3",1); // C-DMTCASHD
        memcpy((char *)postbeneverif.ucAmount_PTAFT3_req,"1",1); // D-DMTIMPSBENV
        memcpy((char *)postbeneverif.ucAmount_PTAFT4_req,"1",1); // C-DMTIMPSBENV
        memcpy((char *)postbeneverif.ucAmount_IMPSSDL_req,"1",1); // TxnAmount
        //memcpy((char *)postbeneverif.ucAmount_main_req,"1",1);    // Last Request Data
        postbeneverif.ucAmount_PTAFT1_req = atoi((char *)postbeneverif.ucCharges_req);
        postbeneverif.ucAmount_PTAFT2_req = atoi((char *)postbeneverif.ucCharges_req);
        postbeneverif.ucAmount_main_req = 1;
    }


    //_____________________________   Mobile Number   _________________________________


    memcpy((char *)postbeneverif.Custmobileno_req, FC_MobileNumber, strlen( FC_MobileNumber));
    memcpy((char *)postbeneverif.MerchantMobileNo_IMPSSDL_req,configLoginLeg2.ucmobileNumber,strlen(configLoginLeg2.ucmobileNumber));
    //_________________________________________________________________________________


    memcpy((char *)postbeneverif.cashoraccount_req,"CASH",4);
    memcpy((char *)postbeneverif.txnType_req, Flag_Transaction_Type, strlen(Flag_Transaction_Type));

    if(Flag_BeneVerif_Charges == 1)
        postbeneverif.Auth_id_req = 0;
    else
        postbeneverif.Auth_id_req = 1;


    //    if(Flag_BeneVerif_Charges == 1) // 13092019 C
    //        memcpy((char *)postbeneverif.requestbitmap_req,"00000100",8);
    //    else // 13092019 C
    //        memcpy((char *)postbeneverif.requestbitmap_req,"10000100",8);


    //----------------------------------------  13092019 C  --------------------------------------------------

    //    ---------------------------------------------    IMPS BITMAP FORMAT     ---------------------------------------

    //    1] Authentication
    //    2] CreateCustomer
    //    3] CreateAccount
    //    4] Add Beneficiary
    //    5] Add Walking
    //    6] PostTransaction
    //    7] Add External
    //    8] DMS Details

    //    ---------------------------------------------------------------------------------------------------------------

    //    if(flag_bitmap == 'W')                                              // For New Walking Customer // comment after discussion with Dinesh for ony walkin in Bene Verification
    //    {
    //        if(Flag_IMPS_Walkin == 'S')
    //            memcpy(postbeneverif.requestbitmap_req, "11011100", 8);     // To create and add Walkin Customer
    //        else
    //            memcpy(postbeneverif.requestbitmap_req, "11010100", 8);     // For Non-Walkin Customer..Only Create Customer

    //    }
    if(Flag_BeneVerif_Charges == 1) // 13092019 C
        memcpy((char *)postbeneverif.requestbitmap_req,"00000100",8);
    else // 13092019 C
    {
        if(Flag_IMPS_Walkin == 'S') // To Add Walking Customer 12062019
        {
/*            if(flag_bitmap == 'N')                                         // For New Beneficiary
                memcpy(postbeneverif.requestbitmap_req, "10011100", 8);
            else                            */                                    // For Registered Beneficiary
                memcpy(postbeneverif.requestbitmap_req, "10001100", 8);
        }
        else
        {
/*            if(flag_bitmap == 'N')                                         // For New Beneficiary
                memcpy(postbeneverif.requestbitmap_req, "10010100", 8);
            else   */                                                             // For Registered Beneficiary
                memcpy(postbeneverif.requestbitmap_req, "10000100", 8);
        }

    }
    //---------------------------------------------------------------------------------------------------------

    if(Flag_BeneVerif_Charges == 1)
        memcpy((char *)postbeneverif.appId_req, CLIENT_ID, strlen(CLIENT_ID));
    else
        memcpy((char *)postbeneverif.appId_req,CLIENT_ID, strlen(CLIENT_ID)); //"FINOMERNP",9);

    memcpy((char *)postbeneverif.analysisFlag_req,"",1);
    postbeneverif.isInclusive_req =0;
    postbeneverif.isClubbed_req =1;

    memset((char *)postbeneverif.beneBank_req,                 0, strlen((char *)postbeneverif.beneBank_req));
    memset((char *)postbeneverif.ABene_beneficiaryAccount_req, 0, strlen((char *)postbeneverif.ABene_beneficiaryAccount_req));
    memset((char *)postbeneverif.beneName_req,                 0, strlen((char *)postbeneverif.beneName_req));
    memset((char *)postbeneverif.debit_creditDebitFlag_req,    0, sizeof((char *)postbeneverif.debit_creditDebitFlag_req));
    memset((char *)postbeneverif.impsserviceflag_req,          0, sizeof((char *)postbeneverif.impsserviceflag_req));
    memset((char *)postbeneverif.reversalFlag_req,             0, sizeof((char *)postbeneverif.reversalFlag_req));
    memset((char *)postbeneverif.Accountno_req,                0, sizeof((char *)postbeneverif.Accountno_req));

    memcpy((char *)postbeneverif.beneBank_req, (char *)ui->cmb_bankList->currentText().trimmed().toAscii().data(),strlen((const char *)ui->cmb_bankList->currentText().trimmed().toAscii().data()));
#if DEBUG
    printf("ui->cmb_bankList == %s\n",(char *)ui->cmb_bankList->currentText().trimmed().toAscii().data());
    printf("postbeneverif.beneBank_req == %s\n",(char *)postbeneverif.beneBank_req);
#endif
    if(Flag_BeneVerif_Charges == 1)
        memcpy((char *)postbeneverif.ABene_beneficiaryAccount_req, "", 1);
    else
        memcpy((char *)postbeneverif.ABene_beneficiaryAccount_req, ui->Bene_acc_no_edit->text().trimmed().toAscii().data(),strlen(ui->Bene_acc_no_edit->text().trimmed().toAscii().data()));

    memcpy((char *)postbeneverif.beneName_req, ui->Bene_Name_edit->text().trimmed().toAscii().data(),strlen(ui->Bene_Name_edit->text().trimmed().toAscii().data()));
    memcpy((char *)postbeneverif.debit_creditDebitFlag_req,"D",1);
    memcpy((char *)postbeneverif.impsserviceflag_req,"Y",1);
    strcpy((char *)postbeneverif.reversalFlag_req,"");
    strcpy((char *)postbeneverif.Accountno_req,"");


    //--------------------------------  SUPPORT DATA  -------------------------------------

    Clear_SupportData();
    Clear_DeviceInfo();

    int iretDI = DeviceInfo();

    if(iretDI != FUNCTION_SUCCESS)
        return FUNCTION_FAILED;

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
    //    memcpy(postbeneverif.sd_PRTXNAMT, (char*)ui->Fund_transfer_amount_edit->text().toAscii().trimmed().data(), strlen(ui->Fund_transfer_amount_edit->text().toAscii().trimmed().data()));
    memcpy(postbeneverif.sd_SPLTSEQ, (char *)sd_deviceinfo.ucSupportData_SPLTSEQ_req, strlen((char *)sd_deviceinfo.ucSupportData_SPLTSEQ_req));
    //    memcpy(postbeneverif.sd_CHGAMT, (char *)sd_deviceinfo.ucSupportData_CHGAMT_req, strlen((char *)sd_deviceinfo.ucSupportData_CHGAMT_req));


    //-------------------------------------------------         13052019        -----------------------------------------------------------------

    //    memcpy(postbeneverif.sd_SPROD_Debit, "1", 1);
    //    memcpy(postbeneverif.sd_SPROD, "5", 1);
    //    memcpy(postbeneverif.sd_PVTXNID, "910010027654", 12);
    //    memcpy(postbeneverif.sd_PVTXNDT, cur_date.toAscii().data(), strlen(cur_date.toAscii().data()));
    //    memcpy(postbeneverif.sd_IPADDR, "10.37.24.161", 12);
    //    memcpy(postbeneverif.sd_DEVICEID, "18F0E48699A8", 12);
    ////    if(Flag_BeneVerif_Charges == 1)                                       // 13052019
    //        memcpy(postbeneverif.sd_APPID, CLIENT_ID, strlen(CLIENT_ID));
    ////    else                                                                  // 13052019
    ////        memcpy(postbeneverif.sd_APPID, "FINOMERNP", 9);                   // 13052019
    //    memcpy(postbeneverif.sd_AUTHID, "@authid", 7);
    //    memcpy(postbeneverif.sd_LOCATION, "", 1);
    //    memcpy(postbeneverif.sd_CELLID, "", 1);
    //    memcpy(postbeneverif.sd_MCC, "a4a23,50,404,20", 15);
    //    memcpy(postbeneverif.sd_RPTFLG, "0", 1);
    //    memcpy(postbeneverif.sd_PRTXNID, itimedate, strlen(itimedate));
    memcpy(postbeneverif.sd_PRTXNAMT, "1", 1);
    //    memcpy(postbeneverif.sd_SPLTSEQ, "0", 1);
    memcpy(postbeneverif.sd_CHGAMT, (char *)postbeneverif.ucOriginalCharges, strlen((char *)postbeneverif.ucOriginalCharges)); //////

    //--------------------------------------------------------------------------------------------------------------------------------------------

    memcpy(postbeneverif.sd_ZRFUT1, (char *)postbeneverif.ABene_beneficiaryAccount_req, strlen((char *)postbeneverif.ABene_beneficiaryAccount_req));
    if(Flag_BeneVerif_Charges == 1)
        memcpy(postbeneverif.sd_ZRFUT2, "", 1);
    else
        memcpy(postbeneverif.sd_ZRFUT2, (char*)ui->IFSC_edit->text().toAscii().trimmed().data(), strlen(ui->IFSC_edit->text().toAscii().trimmed().data()));
    memcpy(postbeneverif.sd_ZRFUT3, (char *)postbeneverif.beneBank_req, strlen((char *)postbeneverif.beneBank_req));
    memcpy(postbeneverif.sd_ZRFUT4, "", 1);
    memcpy(postbeneverif.sd_ZRFUT5, "", 1);
    memcpy(postbeneverif.sd_ZRFUT6, "", 1);
    memcpy(postbeneverif.sd_ZRFUT7, "", 1);
    memcpy(postbeneverif.sd_ZRFUT8, "", 1);
    memcpy(postbeneverif.sd_ZRFUT9, (char *)postbeneverif.ucRequestId_req, strlen((char *)postbeneverif.ucRequestId_req));
    memcpy(postbeneverif.sd_ZRFUN2, "0", 1);
    memcpy(postbeneverif.sd_ZRFUN3, username, strlen(username));
    memcpy(postbeneverif.sd_ZRFUN4, (char *)postbeneverif.Custmobileno_req, strlen((char *)postbeneverif.Custmobileno_req));
    memcpy(postbeneverif.sd_NETID, "900000", 6);
    memcpy(postbeneverif.sd_MSGTYP, "210", 3);


    char IMPS_SupportData[1000], IMPS_SupportData_Debit[1000], SupportAmt[20+1];

    memset(SupportAmt,0,sizeof(SupportAmt));

    sprintf(SupportAmt,"DN1#%s", (char *)postbeneverif.ucCharges_req);

    memset(IMPS_SupportData, 0, sizeof(IMPS_SupportData));
    sprintf(IMPS_SupportData,"SPROD#%s~PVTXNID#%s~PVTXNDT#%s~IPADDR#%s~DEVICEID#%s~APPID#%s~AUTHID#%s~LOCATION#%s~CELLID#%s~MCC#%s~RPTFLG#%s~PRTXNID#%s~PRTXNAMT#%s~SPLTSEQ#%s~CHGAMT#%s~ZRFUT1#%s~ZRFUT2#%s~ZRFUT3#%s~ZRFUT4#%s~ZRFUT5#%s~ZRFUT6#%s~ZRFUT7#%s~ZRFUT8#%s~ZRFUT9#%s~ZRFUN3#%s~ZRFUN4#%s",postbeneverif.sd_SPROD,postbeneverif.sd_PVTXNID,postbeneverif.sd_PVTXNDT,postbeneverif.sd_IPADDR,postbeneverif.sd_DEVICEID,postbeneverif.sd_APPID,postbeneverif.sd_AUTHID,postbeneverif.sd_LOCATION,postbeneverif.sd_CELLID,postbeneverif.sd_MCC,postbeneverif.sd_RPTFLG,postbeneverif.sd_PRTXNID,postbeneverif.sd_PRTXNAMT,postbeneverif.sd_SPLTSEQ,postbeneverif.sd_CHGAMT,postbeneverif.sd_ZRFUT1,postbeneverif.sd_ZRFUT2,postbeneverif.sd_ZRFUT3,postbeneverif.sd_ZRFUT4,postbeneverif.sd_ZRFUT5,postbeneverif.sd_ZRFUT6,postbeneverif.sd_ZRFUT7,postbeneverif.sd_ZRFUT8,postbeneverif.sd_ZRFUT9,postbeneverif.sd_ZRFUN3,postbeneverif.sd_ZRFUN4); //"1",itimedate,"192.168.43.149","192.168.43.149","4888CAFA4374","FINOMER","@authid","","19.062504,73.0198985","0,0,0,0","0",itimedate,"100","0","11.8","20016200561", "HDFC0000406", "HDFC BANK LTD.", "", "Saurabh Ranjane", username, "9913384086", "900000", "210");

    memset(IMPS_SupportData_Debit, 0, sizeof(IMPS_SupportData_Debit));
    if(Flag_BeneVerif_Charges == 1)
        sprintf(IMPS_SupportData_Debit,"SPROD#%s~PVTXNID#%s~PVTXNDT#%s~IPADDR#%s~DEVICEID#%s~APPID#%s~AUTHID#%s~LOCATION#%s~CELLID#%s~MCC#%s~RPTFLG#%s~PRTXNID#%s~PRTXNAMT#%s~SPLTSEQ#%s~CHGAMT#~ZRFUT1#~ZRFUT2#~ZRFUT3#~ZRFUT4#~ZRFUT5#~ZRFUT6#~ZRFUT7# ~ZRFUT8# ~ZRFUT9#%s~ZRFUN3#%s~ZRFUN4#%s~NETID#%s~MSGTYP#%s",postbeneverif.sd_SPROD_Debit,postbeneverif.sd_PVTXNID,postbeneverif.sd_PVTXNDT,postbeneverif.sd_IPADDR,postbeneverif.sd_DEVICEID,postbeneverif.sd_APPID,postbeneverif.sd_AUTHID,postbeneverif.sd_LOCATION,postbeneverif.sd_CELLID,postbeneverif.sd_MCC,postbeneverif.sd_RPTFLG,postbeneverif.sd_PRTXNID,postbeneverif.sd_PRTXNAMT,postbeneverif.sd_SPLTSEQ/*,postbeneverif.sd_CHGAMT,postbeneverif.sd_ZRFUT1,postbeneverif.sd_ZRFUT2,postbeneverif.sd_ZRFUT3,postbeneverif.sd_ZRFUT4,postbeneverif.sd_ZRFUT5,postbeneverif.sd_ZRFUT6,postbeneverif.sd_ZRFUT7,postbeneverif.sd_ZRFUT8,postbeneverif.sd_ZRFUN2*/,postbeneverif.sd_ZRFUT9,postbeneverif.sd_ZRFUN3,postbeneverif.sd_ZRFUN4,postbeneverif.sd_NETID,postbeneverif.sd_MSGTYP); //"1",itimedate,"192.168.43.149","192.168.43.149","4888CAFA4374","FINOMER","@authid","","19.062504,73.0198985","0,0,0,0","0",itimedate,"100","0","11.8","20016200561", "HDFC0000406", "HDFC BANK LTD.", "", "Saurabh Ranjane", username, "9913384086", "900000", "210"); // 06052019
    else
        sprintf(IMPS_SupportData_Debit,"SPROD#%s~PVTXNID#%s~PVTXNDT#%s~IPADDR#%s~DEVICEID#%s~APPID#%s~AUTHID#%s~LOCATION#%s~CELLID#%s~MCC#%s~RPTFLG#%s~PRTXNID#%s~PRTXNAMT#%s~SPLTSEQ#%s~CHGAMT#%s~ZRFUT1#%s~ZRFUT2#%s~ZRFUT3#%s~ZRFUT4#%s~ZRFUT5#%s~ZRFUT6#%s~ZRFUT7#%s~ZRFUT8#%s~ZRFUT9#%s~ZRFUN3#%s~ZRFUN4#%s~NETID#%s~MSGTYP#%s",postbeneverif.sd_SPROD_Debit,postbeneverif.sd_PVTXNID,postbeneverif.sd_PVTXNDT,postbeneverif.sd_IPADDR,postbeneverif.sd_DEVICEID,postbeneverif.sd_APPID,postbeneverif.sd_AUTHID,postbeneverif.sd_LOCATION,postbeneverif.sd_CELLID,postbeneverif.sd_MCC,postbeneverif.sd_RPTFLG,postbeneverif.sd_PRTXNID,postbeneverif.sd_PRTXNAMT,postbeneverif.sd_SPLTSEQ,postbeneverif.sd_CHGAMT,postbeneverif.sd_ZRFUT1,postbeneverif.sd_ZRFUT2,postbeneverif.sd_ZRFUT3,postbeneverif.sd_ZRFUT4,postbeneverif.sd_ZRFUT5,postbeneverif.sd_ZRFUT6,postbeneverif.sd_ZRFUT7,postbeneverif.sd_ZRFUT8,postbeneverif.sd_ZRFUT9,postbeneverif.sd_ZRFUN3,postbeneverif.sd_ZRFUN4,postbeneverif.sd_NETID,postbeneverif.sd_MSGTYP); //"1",itimedate,"192.168.43.149","192.168.43.149","4888CAFA4374","FINOMER","@authid","","19.062504,73.0198985","0,0,0,0","0",itimedate,"100","0","11.8","20016200561", "HDFC0000406", "HDFC BANK LTD.", "", "Saurabh Ranjane", username, "9913384086", "900000", "210");

    //    "supportData": "SPROD#1~PVTXNID#910010027654~PVTXNDT#10\/04\/19~IPADDR#10.37.24.161~DEVICEID#18F0E48699A8~APPID#FINOMERNP~AUTHID#@authid~
    //    LOCATION#~CELLID#19.0630937,73.0200238~MCC#1a4a23,50,404,20~RPTFLG#0~PRTXNID#041019105422768~PRTXNAMT#1~SPLTSEQ#0~CHGAMT#2~ZRFUN4#8454984086~
    //            ZRFUT1#01987654322~ZRFUT2#HDFC0000240~ZRFUT3#HDFC BANK LTD~ZRFUT4#~ZRFUT5#~ZRFUT6# ~ZRFUT7# ~ZRFUT8# ~ZRFUT9#101629502_041019105423769~
    //            ZRFUN3#101753896~ZRFUN4#8454984086~NETID#900000~MSGTYP#210",
    //strcpy(IMPS_SupportData,"SPROD#2~PVTXNID#~PVTXNDT#~IPADDR#10.127.117.102~DEVICEID#18F0E48699A8~APPID#FINOMER~AUTHID#@authid~LOCATION#~CELLID#19.0560723,73.0176474~MCC#1a4a11,50,404,20~RPTFLG#0~PRTXNID#~PRTXNAMT#2000~SPLTSEQ#0~CHGAMT#0.0~ZRFUT1#VODAOB~ZRFUT2#Vodafone~ZRFUT3#DATACARD~ZRFUT4#@traceId~ZRFUT5#504,8898334504~ZRFUT6#~ZRFUT7#~ZRFUT8#~ZRFUT9#@txnId~ZRFUT10#~ZRFUN1#~ZRFUN2#~ZRFUN3#~ZRFUN4#8898334504~ZRFUNDT#");




    memcpy((char *)postbeneverif.currency_req,"INR",3);
    memcpy((char *)postbeneverif.first_supportdata_req,(char *)SupportAmt,strlen((char *)SupportAmt)); //IMPS_SupportData,sizeof(IMPS_SupportData));
    memcpy((char *)postbeneverif.BillerId_req,"VODAOB",6);
    memcpy((char *)postbeneverif.second_supportdata_req,IMPS_SupportData,strlen(IMPS_SupportData));
    memcpy((char *)postbeneverif.debit_creditDebitFlag_req,"D",1);
    memcpy((char *)postbeneverif.first_transactionType_req,"DMTCASHD",8);
    memcpy((char *)postbeneverif.second_transactionType_req,"DMTIMPSBENV",11);
    if(Flag_BeneVerif_Charges == 1)
        memcpy((char *)postbeneverif.transactionComment_req,"Calculate Charges DMTIMPSBENV",29);
    else
        memcpy((char *)postbeneverif.transactionComment_req,"FT DMTIMPSBENV",14);

    memcpy((char *)postbeneverif.credit_creditDebitFlag_req,"C",1);
    memcpy((char *)postbeneverif.costCenter_req,"1011",4);
    postbeneverif.costCenter_int_req = 1011;
    QString Bitmap =(char *) postbeneverif.requestbitmap_req;


    mainreq["RequestId"] =  (char *)postbeneverif.ucRequestId_req;
    mainreq["MethodId"] = postbeneverif.ucMethodId;
    mainreq["SessionId"] = (char *)postbeneverif.ucSessionId_req;
    mainreq["TellerID"]= username;
    mainreq["TokenId"]= "";
    mainreq["X_Auth_Token"] ="";

    if(Flag_BeneVerif_Charges == 1)
        mainreq["ProductCode"]= "";
    else
        mainreq["ProductCode"]= (char *)postbeneverif.ucProductCode_req;

    if(Flag_BeneVerif_Charges == 1)
        mainreq["Charges"]= (char *)postbeneverif.ucCharges_req; // beneverifcharges.ucRDPTOriginalCharges_RES
    else
        mainreq["Charges"] = (char *)postbeneverif.ucOriginalCharges;

    mainreq["MerchantContribution"]= (char *)postbeneverif.ucMerchantContribution_req;


    mainreq["ChannelID"]= CHANNEL_ID_IMPS;
    mainreq["Amount"]= postbeneverif.ucAmount_main_req; //(char *)postbeneverif.ucAmount_main_req;
    //  mainreq["MerchantContribution"]= "100"; //0;
    mainreq["IsEncrypt"]= ISENCEYPT;
    //   mainreq["JsonFormatter"]= true;
    mainreq["SessionExpiryTime"]=(char *)configLoginLeg2.ucSessionExpiryTime;;//OtpImpsNeft.Commdata_OIN.ucSessionExpiryTime_req;;////"10/11/2018 11:58:44 PM";
    //  qDebug()<<"SeSSION EXPIRY LEG2"<<OtpImpsNeft.Commdata_OIN.ucSessionExpiryTime_req;

    //RequestData
    RequestedData["requestbitmap"] = (char *)postbeneverif.requestbitmap_req;
    RequestedData["asyncFlag"] = (char *)postbeneverif.asyncFlag_req;
    RequestedData["totalAmount"] = (char *) postbeneverif.ucAmount_req;
    RequestedData["mobileNumber"] =(char *) postbeneverif.Custmobileno_req;//"7045993236"; //CustDet.cMobileNumber;

    RequestedData["merchantMobileNumber"] = (char *)configLoginLeg2.ucmobileNumber; // postbeneverif.MerchantMobileNo_IMPSSDL_req; // (char *) configLoginLeg2.ucmobileNumber;"7400370122"; //"8055988351";
    RequestedData["txnType"] = (char *) postbeneverif.txnType_req;//"IMPS"; //CustDet.cTxnType;

    RequestedData["cashoraccount"] = (char *) postbeneverif.cashoraccount_req; //"CASH";
    RequestedData["impsserviceflag"] = (char *) postbeneverif.impsserviceflag_req;

    //Only for bill payment
    if(Flag_BeneVerif_Charges != 1)
        RequestedData["parentRRN"] =(char *)postbeneverif.ucRequestId_req;


    //    if(Bitmap.mid(0,1)=="1")
    //    {
    //-----------------------------------------------              CustomerAuth              --------------------------------------------------

    //CustomerAuth
    RequestedData["CustomerAuth"]["MobileNo"] = (char *)postbeneverif.Custmobileno_req; //8268769453";
    if(Flag_BeneVerif_Charges == 1) // Beneverification Charges
    {
        RequestedData["CustomerAuth"]["AadharNo"] ="";
        RequestedData["CustomerAuth"]["Otp"] ="";
    }

    RequestedData["CustomerAuth"]["Auth_id"] = (int)postbeneverif.Auth_id_req;
    RequestedData["CustomerAuth"]["Aadhaar"] = "";
    RequestedData["CustomerAuth"]["Fp_Auth"] = "";

    if(Flag_BeneVerif_Charges != 1)
    {
        //otp
        RequestedData["CustomerAuth"]["Otp"]["RequestId"] = (char *)postbeneverif.ucRequestId_req;
        RequestedData["CustomerAuth"]["Otp"]["MethodId"] = 2;
        RequestedData["CustomerAuth"]["Otp"]["TellerID"] =username;//"TLR1032";
        RequestedData["CustomerAuth"]["Otp"]["SessionId"] = (char *)postbeneverif.ucSessionId_req;
        RequestedData["CustomerAuth"]["Otp"]["TokenId"] ="";
        RequestedData["CustomerAuth"]["Otp"]["IsEncrypt"] = false;
        RequestedData["CustomerAuth"]["Otp"]["X_Auth_Token"] ="";
        RequestedData["CustomerAuth"]["Otp"]["SessionExpiryTime"] = (char *)configLoginLeg2.ucSessionExpiryTime;
        subdata["MethodId"] ="2";
        subdata["RequestId"] = (char *)OtpBeneVer.ucRequestID_Resp;
        subdata["CustomerMobileNo"] = (char *)postbeneverif.Custmobileno_req;
        subdata["OtpPin"]= ui->OTP_Lineedit->text().trimmed().toAscii().data();//CustDet.cOtp;//"";
        subdata["MessageId"] =524;
        subdata["OtpParam"] ="";

        buff_tmp.clear();
        serialize1(Convertstr1(subdata.toStyledString()));
        RequestedData["CustomerAuth"]["Otp"]["RequestData"] = buff_tmp.toAscii().data();
        buff_tmp.clear();
    }
    //-----------------------------------------------             CreateCustomer             --------------------------------------------------
    if(Bitmap.mid(1,1)=="1")
    {

    }
    else
    {
        RequestedData["CreateCustomer"] = "";
    }
    //-------------------------------------")----------             CreateAccount              --------------------------------------------------
    if(Bitmap.mid(2,1)=="1")
    {

    }
    else
    {
        RequestedData["CreateAccount"] = "";
    }


    //-----------------------------------------------             AddBeneficiary             --------------------------------------------------
    if(Bitmap.mid(3,1)=="1")
    {

        RequestedData["AddBeneficiary"]["appId"] = (char *)postbeneverif.appId_req;
        RequestedData["AddBeneficiary"]["customerNumber"] = (char *)FC_CustomerNumber;
        RequestedData["AddBeneficiary"]["beneficiaryType"] ="3";

        if(flag_bc_merchant_account == 'B')
            RequestedData["AddBeneficiary"]["nickName"] ="EMD";
        else
            RequestedData["AddBeneficiary"]["nickName"] ="";

        RequestedData["AddBeneficiary"]["beneficiaryAccount"] =  (char *)postbeneverif.ABene_beneficiaryAccount_req; //"5015000001120"; //CustDet.cAccountNumber;//"96786787687";
        RequestedData["AddBeneficiary"]["beneficiaryAccountType"] ="10";
        RequestedData["AddBeneficiary"]["beneficiaryName"] = (char *)postbeneverif.beneName_req; // CustDet.cBenificiaryName;//"webtset";
        RequestedData["AddBeneficiary"]["beneficiaryBank"] = (char *)postbeneverif.beneBank_req; // "HDFC BANK LTD"; //CustDet.cBBankName;//"HDFCBANKLTD";
        RequestedData["AddBeneficiary"]["beneficiaryBankIfsc"] = (char*)ui->IFSC_edit->text().toAscii().trimmed().data(); // "HDFC0000240"; //CustDet.cIFSC_Code;
        RequestedData["AddBeneficiary"]["beneficiaryAddress1"] = CustDet.cAddress;
        //        SubData["AddBeneficiary"]["beneficiaryBankCity"] = (char *)CustDet.cCity;
        //        SubData["AddBeneficiary"]["beneficiaryBankBranch"] = (char *)CustDet.cBranchName;
        RequestedData["AddBeneficiary"]["beneficiaryState"] = (char *)CustDet.cBranchstate; //CustDet.cstate; //CustDet.cstate;//" ";
        RequestedData["AddBeneficiary"]["beneficiaryCity"] = (char *)CustDet.cCity; //CustDet.cCity;//" ";
        RequestedData["AddBeneficiary"]["beneficiaryZip"] =" ";
        RequestedData["AddBeneficiary"]["beneficiaryEmailId"] =" ";
        RequestedData["AddBeneficiary"]["beneficiaryBankMicr"] =" ";
        RequestedData["AddBeneficiary"]["beneficiaryMaxLimit"] =" ";


    }
    else
    {
        RequestedData["AddBeneficiary"] = "";
    }
    //-----------------------------------------------            AddWalkinCustomer           --------------------------------------------------
    if(Bitmap.mid(4,1)=="1") // 13092019 C
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
        PostTransaction["appId"] = (char *)postbeneverif.appId_req; // "FINOMERNP"; // 22052019
        PostTransaction["valueDate"] ="";
        PostTransaction["isInclusive"] = postbeneverif.isInclusive_req;
        PostTransaction["isClubbed"] = postbeneverif.isClubbed_req;
        if(Flag_BeneVerif_Charges == 1)
            PostTransaction["analysisFlag"] = 1;
        else
            PostTransaction["analysisFlag"] = (char *)postbeneverif.analysisFlag_req;

        PostTransaction["reversalFlag"] ="";
        PostTransaction["referenceNo"] =itimedate;

        ArrayList.clear();

        if(Flag_BeneVerif_Charges != 1)
        {

            acctFundTransfer["accountNumber"] = "";
            acctFundTransfer["amount"] = postbeneverif.ucAmount_PTAFT1_req;
            acctFundTransfer["currency"] = (char *) postbeneverif.currency_req;
            acctFundTransfer["creditDebitFlag"] = (char *) postbeneverif.debit_creditDebitFlag_req;
            acctFundTransfer["transactionType"] = (char *) postbeneverif.first_transactionType_req;
            acctFundTransfer["transactionComment"] = (char *)postbeneverif.transactionComment_req;
            acctFundTransfer["costCenter"] = (char *) postbeneverif.costCenter_req;
            acctFundTransfer["supportData"] = (char *) postbeneverif.first_supportdata_req ;
            acctFundTransfer["beneficiaryRefOrMmid"] = "";
            acctFundTransfer["beneficiaryMobile"] = "";
            acctFundTransfer["remitterMobile"] = (char *) postbeneverif.Custmobileno_req;
            acctFundTransfer["remitterMmid"] = "";
            acctFundTransfer["beneficiaryAccountNo"] = (char *) postbeneverif.ABene_beneficiaryAccount_req;
            acctFundTransfer["beneficiaryIfsc"] = (char *) postbeneverif.sd_ZRFUT2;
            acctFundTransfer["remarks"] ="";// CustDet.cRemark;
            ArrayList.append(acctFundTransfer);

            acctFundTransfer["accountNumber"] = "";
            acctFundTransfer["amount"] = postbeneverif.ucAmount_PTAFT2_req;
            acctFundTransfer["currency"] = (char *) postbeneverif.currency_req;
            acctFundTransfer["creditDebitFlag"] = (char *) postbeneverif.credit_creditDebitFlag_req;
            acctFundTransfer["transactionType"] = (char *) postbeneverif.first_transactionType_req;
            acctFundTransfer["transactionComment"] = (char *)postbeneverif.transactionComment_req;
            acctFundTransfer["costCenter"] = (char *) postbeneverif.costCenter_req;
            acctFundTransfer["supportData"] = IMPS_SupportData;
            acctFundTransfer["beneficiaryRefOrMmid"] = "";
            acctFundTransfer["beneficiaryMobile"] = "";
            acctFundTransfer["remitterMobile"] = "";
            acctFundTransfer["remitterMmid"] = "";
            acctFundTransfer["beneficiaryAccountNo"] = "";
            acctFundTransfer["beneficiaryIfsc"] = (char *) postbeneverif.sd_ZRFUT2;
            acctFundTransfer["remarks"] ="";// CustDet.cRemark;
            ArrayList.append(acctFundTransfer);

        }

        if(Flag_BeneVerif_Charges == 1)
            acctFundTransfer["accountNumber"] = (char *)CustDet.cFindCustAccountNo;
        else
            acctFundTransfer["accountNumber"] = "";
        acctFundTransfer["amount"] = (char *) postbeneverif.ucAmount_PTAFT3_req;
        acctFundTransfer["currency"] = (char *) postbeneverif.currency_req;
        acctFundTransfer["creditDebitFlag"] = (char *) postbeneverif.debit_creditDebitFlag_req;
        acctFundTransfer["transactionType"] = (char *) postbeneverif.second_transactionType_req;
        acctFundTransfer["transactionComment"] = (char *)postbeneverif.transactionComment_req;
        if(Flag_BeneVerif_Charges == 1)
            acctFundTransfer["costCenter"] = (int)postbeneverif.costCenter_int_req;
        else
            acctFundTransfer["costCenter"] = (char *) postbeneverif.costCenter_req;
        acctFundTransfer["supportData"] = IMPS_SupportData_Debit;
        acctFundTransfer["beneficiaryRefOrMmid"] = "";
        acctFundTransfer["beneficiaryMobile"] = "";
        acctFundTransfer["remitterMobile"] = (char *) postbeneverif.Custmobileno_req;
        acctFundTransfer["remitterMmid"] = "";
        if(Flag_BeneVerif_Charges == 1)
            acctFundTransfer["beneficiaryAccountNo"] = "";
        else
            acctFundTransfer["beneficiaryAccountNo"] = (char *) postbeneverif.ABene_beneficiaryAccount_req;
        acctFundTransfer["beneficiaryIfsc"] = (char *) postbeneverif.sd_ZRFUT2;
        acctFundTransfer["remarks"] ="";// CustDet.cRemark;
        ArrayList.append(acctFundTransfer);


        acctFundTransfer["accountNumber"] = "";
        acctFundTransfer["amount"] = (char *) postbeneverif.ucAmount_PTAFT4_req;
        acctFundTransfer["currency"] = (char *) postbeneverif.currency_req;
        acctFundTransfer["creditDebitFlag"] = (char *) postbeneverif.credit_creditDebitFlag_req;
        acctFundTransfer["transactionType"] = (char *) postbeneverif.second_transactionType_req;
        acctFundTransfer["transactionComment"] = (char *)postbeneverif.transactionComment_req;
        if(Flag_BeneVerif_Charges == 1)
            acctFundTransfer["costCenter"] = (int)postbeneverif.costCenter_int_req;
        else
            acctFundTransfer["costCenter"] = (char *) postbeneverif.costCenter_req;
        acctFundTransfer["supportData"] = "";
        acctFundTransfer["beneficiaryRefOrMmid"] = "";
        acctFundTransfer["beneficiaryMobile"] = "";
        acctFundTransfer["remitterMobile"] = "";
        acctFundTransfer["remitterMmid"] = "";
        acctFundTransfer["beneficiaryAccountNo"] = "";
        acctFundTransfer["beneficiaryIfsc"] = (char *) postbeneverif.sd_ZRFUT2;
        acctFundTransfer["remarks"] ="";// CustDet.cRemark;
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
    //-----------------------------------------------               DMSDetails             ----------------------------------------------------
    if(Bitmap.mid(6,1)=="1")
    {

    }
    else
    {
        RequestedData["DMSDetails"] = "";
    }
    //-----------------------------------------------            AddExternalAccount          --------------------------------------------------
    if(Bitmap.mid(7,1)=="1")
    {

    }
    else
    {
        RequestedData["AddExternalAccount"] = "";

    }
    //-----------------------------------------------           IMPSServiceDataList          --------------------------------------------------
    //Bene_Name_edit
    IMPSServiceDataList["BeneAccNo"] =  (char *) postbeneverif.ABene_beneficiaryAccount_req; //CustDet.cAccountNumber;//"976776776555";

    if(Flag_BeneVerif_Charges == 1)
        IMPSServiceDataList["BeneIFSC"] = "";
    else
        IMPSServiceDataList["BeneIFSC"] = (char*)ui->IFSC_edit->text().toAscii().trimmed().data(); //"HDFC0000240"; //CustDet.cIFSC_Code;//"HDFC0000240";

    if(Flag_BeneVerif_Charges == 1)
        IMPSServiceDataList["BeneName"] = (char*)FINDCUST.ucRDCLfullName_resp; // 06052019
    else
        IMPSServiceDataList["BeneName"] = (char *)postbeneverif.beneName_req; //CustDet.cBenificiaryName;//"BeneVerification";
    IMPSServiceDataList["MerchantMobileNo"] = (char *)postbeneverif.MerchantMobileNo_IMPSSDL_req; //(char *)configLoginLeg2.ucmobileNumber;//"9167252942";
    IMPSServiceDataList["PaymentRef"] =  (char *)postbeneverif.ucRequestId_req; //"262019115938299";
    IMPSServiceDataList["RemeMobileNo"] = (char *) postbeneverif.Custmobileno_req;
    if(Flag_BeneVerif_Charges == 1)
        IMPSServiceDataList["RemeName"] = (char*)FINDCUST.ucRDCLfullName_resp; // 06052019
    else // 13092019 C
    {
        if(strcmp((char *)SearchWalkCust.SWCL_name_resp, "")==0)
            IMPSServiceDataList["RemeName"] = (char*)FINDCUST.ucRDCLfullName_resp; // 06052019
        else
            IMPSServiceDataList["RemeName"] = (char *)SearchWalkCust.SWCL_name_resp;
    }
    IMPSServiceDataList["TxnAmount"] = (char *)postbeneverif.ucAmount_IMPSSDL_req;// CustDet.cTxnAmount;//"1"; // Remove Hardcoded
    ArrayList.append(IMPSServiceDataList);

    RequestedData["IMPSServiceDataList"] = ArrayList;

    ////////////////////////////////BillDeskData

    //    BillDeskData["UserID"] = (char *) postbeneverif.Custmobileno_req;
    //    BillDeskData["CustomerID"] = (char *) postbeneverif.Custmobileno_req;
    //    BillDeskData["CustomerMobNo"] = (char *) postbeneverif.Custmobileno_req;
    //    BillDeskData["BillerId"] = (char *) postbeneverif.BillerId_req;
    //    BillDeskData["BankMessage"] = ""; //"262019115938299";
    //    BillDeskData["PaymentAmount"] = (char *) postbeneverif.ucAmount_req;
    //    BillDeskData["Authenticator1"] = (char *) postbeneverif.Custmobileno_req;;
    //    // ArrayList.append(BillDeskData);

    //    RequestedData["BillDeskData"] = BillDeskData;

    //-----------------------------------------------------------------------------------------------------------------------------------------
    QString buff_tmp;
    buff_tmp.clear();
    buff_tmp = serialize1(Convertstr(RequestedData.toStyledString()));

    mainreq["RequestData"] = buff_tmp.toAscii().data();

    memset(gSendData, 0, sizeof(gSendData));
    memcpy(gSendData, (char *)Convertstr(mainreq.toStyledString()), strlen((const char *)Convertstr(mainreq.toStyledString())));
#if DEBUG
    printf("gSendData = %s\n",gSendData);
#endif
    /****************************Encryption function*********************************/
    if(!strcmp(ISENCEYPT,"true"))
    {
        int gSendData_len =strlen((char *)gSendData);
        RequestDetachEncryptAttach((char *)gSendData,2,gSendData_len);
    }
    /***************************************************************/


    //-------------------------------------------------------------

    memset(str_url,0,sizeof(str_url));
#ifdef PRODUCTION
    strcpy(str_url,URL_CREATION(IMPS_POST_BENE_VERIFICATION_URL_PROD));
#else
    strcpy(str_url,URL_CREATION((char *)IMPS_POST_BENE_VERIFICATION_URL));
#endif

    int ret=-3;
    ret = NetworkConnect(0,str_url, 2);
    if(ret != FUNCTION_SUCCESS)
    {
        qDebug()<<"NetworkConnect function failed";
        uidmessage::instance()->hide_Message();
        if(ret == RESPONSE_TIMEOUT)
        {
            qDebug()<<"Response Timeout";
        }
        return FUNCTION_FAILED;
    }

    /****************************Decryption function*********************************/
    if(!strcmp(ISENCEYPT,"true"))
    {
        int oBuff_len = strlen(oBuff);
#if DEBUG
        qDebug()<<"Length of buffer"<<oBuff_len;
        printf("Buffer%s\n",oBuff);
#endif
        ResponseDetachEncryptAttach(oBuff,2,oBuff_len);//TEMPORARY

        system("rm /usr/FINO/FPB/RESP/OUTPUT.TXT");
        FILE *fp2 = NULL;
        fp2 =fopen("/usr/FINO/FPB/RESP/OUTPUT.TXT","a");
        fwrite(oBuff,strlen((char *)oBuff),1,fp2);
        sleep(1);
        fclose(fp2);
    }
    /*******************************************************************************/
    if(Flag_BeneVerif_Charges == 1)
        iRet = BeneVerification_Charges_Parsing();
    else
        iRet = POST_BENEVERF_Parsing();
    return iRet;

}

int FPB::BeneVerification_Charges_Parsing()
{
    char cResCode[10+1], cResCode1[20];
    system("sed 's/\"ResponseData\":\"{/\"ResponseData\":{/g' /usr/FINO/FPB/RESP/OUTPUT.TXT > /tmp/1.txt");
    system("sed 's/}\",\"MessageId\"/},\"MessageId\"/g' /tmp/1.txt > /tmp/2.txt  ");
    system(" sed 's/\\\\//g' /tmp/2.txt > /usr/FINO/FPB/RESP/OUTPUT.TXT");

    Json1::Value root;
    root.clear();
    //QSqlQuery query;
    Json1::Reader reader;
    std::ifstream ReadData("/usr/FINO/FPB/RESP/OUTPUT.TXT");
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
        qDebug()<<"ResponseMessage    : "<<Convertstr(root["ResponseMessage"].asString());
        qDebug()<<"ResponseCode : "<<Convertstr(root["ResponseCode"].asString());
#endif
        memset(beneverifcharges.Commondata_POBV_RES.ucResponseMessage_resp, 0, sizeof(beneverifcharges.Commondata_POBV_RES.ucResponseMessage_resp));
        memcpy(beneverifcharges.Commondata_POBV_RES.ucResponseMessage_resp, Convertstr(root["ResponseMessage"].asString()), strlen(Convertstr(root["ResponseMessage"].asString())));

        strcpy(cResCode,Convertstr(root["ResponseCode"].asString()));
        memset(beneverifcharges.Commondata_POBV_RES.ucResponseCode_resp, 0, sizeof(beneverifcharges.Commondata_POBV_RES.ucResponseCode_resp));
        memcpy(beneverifcharges.Commondata_POBV_RES.ucResponseCode_resp, Convertstr(root["ResponseCode"].asString()), strlen(Convertstr(root["ResponseCode"].asString())));

        if(memcmp(cResCode,"0",1)==0)
        {
            Json1::Value ResponseData;
            Json1::Value RespData_PostTransactionResponse;
            float BeneVerifCharges_Amount = 0.00;
            ResponseData.clear();
            ResponseData = root.get("ResponseData",0);
#if DEBUG
            printf("\nSize : %d\n",ResponseData.size());
#endif
            if(ResponseData.size()==0)
                return FUNC_ERROR;
            else
            {
#if DEBUG
                qDebug()<<"ResponseData returnCode : "<<Convertstr(ResponseData["returnCode"].asString());
                qDebug()<<"ResponseData responseBitMap : "<<Convertstr(ResponseData["responseBitMap"].asString());
#endif
                RespData_PostTransactionResponse.clear();
                RespData_PostTransactionResponse = ResponseData.get("PostTransactionResponse",0);
#if DEBUG
                printf("\nSize : %d\n",RespData_PostTransactionResponse.size());
#endif
                for(int j=0;j<RespData_PostTransactionResponse.size();j++)
                {
#if DEBUG
                    qDebug()<<"PostTransactionResponse returnCode : "<<Convertstr(RespData_PostTransactionResponse[j]["returnCode"].asString());
#endif
                    Json1::Value ResponseData1;
                    ResponseData1.clear();
                    ResponseData1 = RespData_PostTransactionResponse[j].get("chargesList",0);
#if DEBUG
                    printf("\nSize : %d\n",ResponseData1.size());
#endif

                    if(ResponseData1.size()==0)
                    {
                        if(flag_bc_merchant_account == 'B')
                        {
                            BeneVerifCharges_Amount = atof(BENE_VERIFICATION_CHARGES);
#if DEBUG
                            printf("BC Merchant VERIFICATION CHARGES = %f\n", BeneVerifCharges_Amount);
#endif
                        }
                        else
                        {
                            qDebug("\n1:Key Register Data Not Found\n");
                            return FUNC_ERROR;
                        }
                    }
                    else
                    {
                        for(int k=0;k<ResponseData1.size();k++)
                        {
#if DEBUG
                            qDebug()<<"chargesList code : "<<Convertstr(ResponseData1[k]["code"].asString());
                            qDebug()<<"chargesList amount : "<<ResponseData1[k]["amount"].asFloat();
                            qDebug()<<"chargesList description : "<<Convertstr(ResponseData1[k]["description"].asString());
                            qDebug()<<"chargesList chargeAcctNum : "<<Convertstr(ResponseData1[k]["chargeAcctNum"].asString());
#endif

                            memset(cResCode1,0,sizeof(cResCode1));
                            memcpy(cResCode1, (char *)Convertstr(ResponseData1[k]["code"].asString()), strlen((const char*)Convertstr(ResponseData1[k]["code"].asString())));
#if DEBUG
                            printf("cResCode1 = %s\n",cResCode1);
#endif
                            if(memcmp(cResCode1,"CHGDMTBENV",strlen("CHGDMTBENV"))==0)
                            {
                                memset(beneverifcharges.ucRDPTOriginalCharges_RES, 0, sizeof(beneverifcharges.ucRDPTOriginalCharges_RES));
                                memcpy((char *)beneverifcharges.ucRDPTOriginalCharges_RES, (char *)Convertstr(ResponseData1[k]["amount"].asString()), strlen((char *)Convertstr(ResponseData1[k]["amount"].asString())));
                                BeneVerifCharges_Amount = ResponseData1[k]["amount"].asFloat();
                                BeneVerifCharges_Amount = BeneVerifCharges_Amount + 1.0;
#if DEBUG
                                printf("VERIFICATION CHARGES = %f\n", BeneVerifCharges_Amount);
#endif
                            }
                        }

                    }

                }

                memset(beneverifcharges.ucRDPTCharges_RES, 0, sizeof(beneverifcharges.ucRDPTCharges_RES));
                sprintf((char *)beneverifcharges.ucRDPTCharges_RES, "%f", BeneVerifCharges_Amount);
#if DEBUG
                printf("TOTAL VERIFICATION CHARGES = %s\n", beneverifcharges.ucRDPTCharges_RES);
#endif
            }
        }
        else
        {
            CMisc miscObj;
            uidmessage::instance()->hide_Message();

            if(strcmp(beneverifcharges.Commondata_POBV_RES.ucResponseMessage_resp,"")!=0)
            {
                char ErrorMsg[250];
                memset(ErrorMsg, 0, sizeof(ErrorMsg));
                sprintf(ErrorMsg,"%s",(char *)beneverifcharges.Commondata_POBV_RES.ucResponseMessage_resp);
                miscObj.DisplayMsgBox(ErrorMsg);
                if(strcmp((char *)beneverifcharges.Commondata_POBV_RES.ucResponseMessage_resp,"Session Expired.")==0)
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

int FPB::POST_BENEVERF_Parsing()
{
    char cResCode[10+1];

    memset(&resp_pBenever, 0, sizeof(resp_pBenever));

    system("sed 's/\"ResponseData\":\"{/\"ResponseData\":{/g' /usr/FINO/FPB/RESP/OUTPUT.TXT > /tmp/1.txt");
    system("sed 's/}\",\"MessageId\"/},\"MessageId\"/g' /tmp/1.txt > /tmp/2.txt  ");
    system(" sed 's/\\\\//g' /tmp/2.txt > /usr/FINO/FPB/RESP/OUTPUT.TXT");

    //    system("rm /tmp/1.txt");
    //    system("rm /tmp/2.txt");

    qDebug()<<"before ADD_Bene_Parsing in";
    CMisc miscObj;
    Json1::Value root;
    root.clear();
    //QSqlQuery query;
    Json1::Reader reader;
    std::ifstream ReadData("/usr/FINO/FPB/RESP/OUTPUT.TXT");
    bool parsedSuccess = reader.parse(ReadData,root,false);

    ReadData.close();

    //    if(not parsedSuccess)
    //    {
    //        qDebug()<<"FpbData File not Found";
    //        return FUNC_ERROR;
    //    }
    //    else
    {
#if DEBUG
        qDebug()<<"RequestId   : "<<Convertstr(root["RequestId"].asString());
        qDebug()<<"DisplayMessage : "<<Convertstr(root["DisplayMessage"].asString());
        qDebug()<<"ResponseMessage    : "<<Convertstr(root["ResponseMessage"].asString());
        qDebug()<<"ResponseCode : "<<Convertstr(root["ResponseCode"].asString());
        qDebug()<<"MessageType : "<<Convertstr(root["MessageType"].asString());
        //        qDebug()<<"ResponseData returnCode : "<<Convertstr(root["ResponseData"]["returnCode"].asString());
        //        qDebug()<<"ResponseData noOfRows: "<<Convertstr(root["ResponseData"]["noOfRows"].asString());
        //        qDebug()<<"AuthmanFlag : "<<Convertstr(root["AuthmanFlag"].asString());
#endif
        memcpy(resp_pBenever.Commondata_POBV_RES.ucRequestID_resp, Convertstr(root["RequestId"].asString()), strlen(Convertstr(root["RequestId"].asString())));
        memcpy(resp_pBenever.Commondata_POBV_RES.ucDisplayMessage_resp, Convertstr(root["DisplayMessage"].asString()), strlen(Convertstr(root["DisplayMessage"].asString())));

        memcpy(resp_pBenever.Commondata_POBV_RES.ucResponseMessage_resp, Convertstr(root["ResponseMessage"].asString()), strlen(Convertstr(root["ResponseMessage"].asString())));

        strcpy(cResCode,Convertstr(root["ResponseCode"].asString()));
        memcpy(resp_pBenever.Commondata_POBV_RES.ucResponseCode_resp, Convertstr(root["ResponseCode"].asString()), strlen(Convertstr(root["ResponseCode"].asString())));

        memcpy(resp_pBenever.Commondata_POBV_RES.ucMessageType_resp, Convertstr(root["MessageType"].asString()), strlen(Convertstr(root["MessageType"].asString())));


        if(memcmp(cResCode,"0",1)==0)
        {
            Json1::Value ResponseData, RespData_PostTransactionResponse;
            ResponseData.clear();
            ResponseData = root.get("ResponseData",0);
#if DEBUG
            printf("\nSize : %d\n",ResponseData.size());
#endif
            if(ResponseData.size()==0)
            {
                qDebug("\n1:Key Register Data Not Found\n");
                return FUNC_ERROR;
            }
            else
            {

                RespData_PostTransactionResponse.clear();
                RespData_PostTransactionResponse = ResponseData.get("PostTransactionResponse",0);
#if DEBUG
                printf("\nSize : %d\n",RespData_PostTransactionResponse.size());
#endif
                for(int j=0;j<RespData_PostTransactionResponse.size();j++)
                {
                    Json1::Value ResponseData2;
                    ResponseData2.clear();
                    ResponseData2 = RespData_PostTransactionResponse[j].get("balancesList",0);
#if DEBUG
                    printf("\nSize : %d\n",ResponseData2.size());
#endif
                    if(ResponseData2.size() != 0)
                    {
                        for(int i=0;i<ResponseData2.size();i++)
                        {
#if DEBUG
                            qDebug()<<"balancesList availableBalance : "<<Convertstr(ResponseData2[i]["availableBalance"].asString());   ////
#endif
                            memset(resp_pBenever.Commondata_POBV_RES.ucBeneVerif_AvilableBalance, 0, sizeof(resp_pBenever.Commondata_POBV_RES.ucBeneVerif_AvilableBalance));
                            memcpy(resp_pBenever.Commondata_POBV_RES.ucBeneVerif_AvilableBalance, (const char*)Convertstr(ResponseData2[i]["availableBalance"].asString()), strlen((const char*)Convertstr(ResponseData2[i]["availableBalance"].asString())));

                        }
#if DEBUG
                        printf("Merchant Avilable Balance = %s\n",resp_pBenever.Commondata_POBV_RES.ucBeneVerif_AvilableBalance);
#endif
                    }
                }

                memset(resp_pBenever.Commondata_POBV_RES.ucIMPSSLR_BankRRN_resp,0,sizeof(resp_pBenever.Commondata_POBV_RES.ucIMPSSLR_BankRRN_resp));
                memset(resp_pBenever.Commondata_POBV_RES.ucIMPSSLR_BeneAccNo_resp,0,sizeof(resp_pBenever.Commondata_POBV_RES.ucIMPSSLR_BeneAccNo_resp));
                memset(resp_pBenever.Commondata_POBV_RES.ucIMPSSLR_BeneIFSC_resp,0,sizeof(resp_pBenever.Commondata_POBV_RES.ucIMPSSLR_BeneIFSC_resp));
                memset(resp_pBenever.Commondata_POBV_RES.ucIMPSSLR_BeneName_resp,0,sizeof(resp_pBenever.Commondata_POBV_RES.ucIMPSSLR_BeneName_resp));
                memset(resp_pBenever.Commondata_POBV_RES.ucIMPSSLR_ActCode_resp,0,sizeof(resp_pBenever.Commondata_POBV_RES.ucIMPSSLR_ActCode_resp));
                memset(resp_pBenever.Commondata_POBV_RES.ucIMPSSLR_Response_resp,0,sizeof(resp_pBenever.Commondata_POBV_RES.ucIMPSSLR_Response_resp));
#if DEBUG
                qDebug()<<"BankRRN :"<<Convertstr(ResponseData["IMPSServiceDataListResponse"]["BankRRN"].asString());
                qDebug()<<"BeneAccNo :"<<Convertstr(ResponseData["IMPSServiceDataListResponse"]["BeneAccNo"].asString());
                printf("BeneAccNo = %s\n",resp_pBenever.Commondata_POBV_RES.ucIMPSSLR_BeneAccNo_resp);
                qDebug()<<"BeneIFSC :"<<Convertstr(ResponseData["IMPSServiceDataListResponse"]["BeneIFSC"].asString());
                qDebug()<<"BeneName  :"<<Convertstr(ResponseData["IMPSServiceDataListResponse"]["BeneName"].asString());
                qDebug()<<"ActCode :"<<Convertstr(ResponseData["IMPSServiceDataListResponse"]["ActCode"].asString());
                qDebug()<<"Response :"<<Convertstr(ResponseData["IMPSServiceDataListResponse"]["Response"].asString());
#endif
                memcpy(resp_pBenever.Commondata_POBV_RES.ucIMPSSLR_BankRRN_resp, Convertstr(ResponseData["IMPSServiceDataListResponse"]["BankRRN"].asString()), strlen(Convertstr(ResponseData["IMPSServiceDataListResponse"]["BankRRN"].asString())));
                memcpy(resp_pBenever.Commondata_POBV_RES.ucIMPSSLR_BeneAccNo_resp, Convertstr(ResponseData["IMPSServiceDataListResponse"]["BeneAccNo"].asString()), strlen(Convertstr(ResponseData["IMPSServiceDataListResponse"]["BeneAccNo"].asString())));

                memcpy(resp_pBenever.Commondata_POBV_RES.ucIMPSSLR_BeneIFSC_resp, Convertstr(ResponseData["IMPSServiceDataListResponse"]["BeneIFSC"].asString()), strlen(Convertstr(ResponseData["IMPSServiceDataListResponse"]["BeneIFSC"].asString())));

                memcpy(resp_pBenever.Commondata_POBV_RES.ucIMPSSLR_BeneName_resp, Convertstr(ResponseData["IMPSServiceDataListResponse"]["BeneName"].asString()), strlen(Convertstr(ResponseData["IMPSServiceDataListResponse"]["BeneName"].asString())));

                memcpy(resp_pBenever.Commondata_POBV_RES.ucIMPSSLR_ActCode_resp, Convertstr(ResponseData["IMPSServiceDataListResponse"]["ActCode"].asString()), strlen(Convertstr(ResponseData["IMPSServiceDataListResponse"]["ActCode"].asString())));

                memcpy(resp_pBenever.Commondata_POBV_RES.ucIMPSSLR_Response_resp, Convertstr(ResponseData["IMPSServiceDataListResponse"]["Response"].asString()), strlen(Convertstr(ResponseData["IMPSServiceDataListResponse"]["Response"].asString())));

                //                Json1::Value ResponseData2;
                //                ResponseData2.clear();
                //                ResponseData2 = RespData_PostTransactionResponse[j].get("balancesList",0);
                //                printf("\nSize : %d\n",ResponseData2.size());
                //                for(int i=0;i<ResponseData2.size();i++)
                //                {

                //                    //                        QString g = QString::number(ResponseData2[i]["ledgerBalance"].asFloat());
                //                    //                        qDebug()<<"g = "<<g;
                //                    //                        QString f = QString::number(ResponseData2[i]["availableBalance"].asFloat());
                //                    //                        qDebug()<<"f = "<<f;

                //                    qDebug()<<"balancesList ledgerBalance : "<<Convertstr(ResponseData2[i]["ledgerBalance"].asString());         ////
                //                    qDebug()<<"balancesList availableBalance : "<<Convertstr(ResponseData2[i]["availableBalance"].asString());   ////
                //                    qDebug()<<"balancesList currency : "<<Convertstr(ResponseData2[i]["currency"].asString());
                //                    memset(resp_postimps.Commondata_POBV_RES.ucIMPS_AvilableBalance, 0, sizeof(resp_postimps.Commondata_POBV_RES.ucIMPS_AvilableBalance));
                //                    memcpy(resp_postimps.Commondata_POBV_RES.ucIMPS_AvilableBalance, (const char*)Convertstr(ResponseData2[i]["availableBalance"].asString()), strlen((const char*)Convertstr(ResponseData2[i]["availableBalance"].asString())));

                //                }

            }

            return FUNC_OK;
        }
        else
        {
            char ErrorMsg[205];
            memset(ErrorMsg, 0, sizeof(ErrorMsg));
            uidmessage::instance()->hide_Message();
            if(strcmp((char *)resp_pBenever.Commondata_POBV_RES.ucResponseMessage_resp,"Session Expired.")==0)
            {
                sprintf(ErrorMsg,"%s",(char *)resp_pBenever.Commondata_POBV_RES.ucResponseMessage_resp);
                miscObj.DisplayMsgBox(ErrorMsg);
                uidmessage::instance()->hide_Message();
                //                ui->stackedWidget->setCurrentIndex(0);
                return SESSION_EXPIRED_FAILED;
            }
            if(strcmp((char *)resp_pBenever.Commondata_POBV_RES.ucResponseMessage_resp,"")!=0)
            {
                sprintf(ErrorMsg,"%s",(char *)resp_pBenever.Commondata_POBV_RES.ucResponseMessage_resp);
                miscObj.DisplayMsgBox(ErrorMsg);
            }
            uidmessage::instance()->hide_Message();
            return FUNCTION_FAILED;

        }

    }
    return FUNC_OK;
}

int FPB::BeneVerifOTP_Parsing()
{
    char cResCode[10+1];

    system("sed 's/\"ResponseData\":\"{/\"ResponseData\":{/g' /usr/FINO/FPB/RESP/BENEVERIF_OTP.TXT > /tmp/1.txt");
    system("sed 's/}\",\"MessageId\"/},\"MessageId\"/g' /tmp/1.txt > /tmp/2.txt  ");
    system(" sed 's/\\\\//g' /tmp/2.txt > /usr/FINO/FPB/RESP/BENEVERIF_OTP.TXT");

    //    system("rm /tmp/1.txt");
    //    system("rm /tmp/2.txt");

    Json1::Value root;
    root.clear();
    //QSqlQuery query;
    Json1::Reader reader;
    std::ifstream ReadData("/usr/FINO/FPB/RESP/BENEVERIF_OTP.TXT");
    bool parsedSuccess = reader.parse(ReadData,root,false);

    ReadData.close();

    //    if(not parsedSuccess)
    //    {
    //        qDebug()<<"FpbData File not Found";
    //        return FUNC_ERROR;
    //    }
    //    else
    {
        qDebug()<<"RequestId   : "<<Convertstr(root["RequestId"].asString());
        qDebug()<<"DisplayMessage : "<<Convertstr(root["DisplayMessage"].asString());
        qDebug()<<"ResponseMessage    : "<<Convertstr(root["ResponseMessage"].asString());
        strcpy(cResCode,Convertstr(root["ResponseCode"].asString()));
        qDebug()<<"ResponseCode : "<<Convertstr(root["ResponseCode"].asString());
        qDebug()<<"MessageType : "<<Convertstr(root["MessageType"].asString());
        qDebug()<<"ResponseData returnCode : "<<Convertstr(root["ResponseData"]["returnCode"].asString());
        qDebug()<<"ResponseData noOfRows: "<<Convertstr(root["ResponseData"]["noOfRows"].asString());
        qDebug()<<"AuthmanFlag : "<<Convertstr(root["AuthmanFlag"].asString());

        //        if(memcmp(cResCode,"0",1)==0)
        //        {
        //            Json1::Value ResponseData;
        //            ResponseData.clear();
        //            ResponseData = root.get("ResponseData",0);
        //            printf("\nSize : %d\n",ResponseData.size());
        //            if(ResponseData.size()==0)
        //            {
        //                qDebug("\n1:Key Register Data Not Found\n");
        //                return FUNC_ERROR;
        //            }
        //            else
        //            {
        //                Json1::Value ResponseData1;
        //                ResponseData1 = ResponseData.get("externalBeneficiaryInfoList",0);
        //                //qDebug()<<"ResponseData1 "<< ResponseData1;
        //                for(int j=0;j<ResponseData1.size();j++)
        //                {
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

        //                }
        //            }
        //        }
    }
    return FUNC_OK;
}

void FPB::on_FINISH_PushButton_Pg34_clicked()
{
    ui->Next_Otp_Button_2->setVisible(true);
    ui->FINISH_PushButton_Pg34->setVisible(false);
    if(OTP_page == 4)
        ui->stackedWidget->setCurrentIndex(19);
    else
    {
        if(Flag_IMPS_Walkin == 'S' || CUST_TYPE == 'W' )
        {
            Flag_IMPS_Walkin = '\0';
            CUST_TYPE = '\0';
            ui->stackedWidget->setCurrentIndex(12);
            return;
        }
        else
        {
            ui->stackedWidget->setCurrentIndex(15);
            return;
        }
    }
    // ui->stackedWidget->setCurrentIndex(15);
    return;
}
