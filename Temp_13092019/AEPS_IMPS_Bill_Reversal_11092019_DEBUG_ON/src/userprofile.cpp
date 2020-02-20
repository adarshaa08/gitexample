#include "userprofile.h"
#include "ui_FPB.h"
#include "json.h"
#include "FPB.h"
#include "Login.h"
#include "displaymessage.h"
#include "print.h"
#include<fstream>
#include "impscommon.h"

extern Login_leg2 configLoginLeg2;
extern Login_leg1 configLoginLeg1;
extern RD_SERVICE_STRUCTURE RD_Service_data;
extern Transaction Txn;
extern Printfpb ConfigPrint;
FOR_PASS FORPASS_State;
T_FOR_PASS secret_data[10];
extern TxnStatement* MiniStatement;
FOR_OTP OTP;
NewPass sndnewpass;
ChangePass sndchange;
Secret_Quest *Secret_question;
int SecretQue_count;
Secret_resp Sec_Resp;
Select_secretQue selected_que[3];

Logout sndlogout;
int CountSQ = 0;
extern char * Convertstr(string str);
int OTP_page = -1;
extern char oBuff[25*1024];  // File Download
extern char display[100][100];
int show_bal=1;
TxnStatement *Statement, iZero = {0}, iTransStatus, ChildList[500];
Merchant_detail Merdetail;
extern sCustDetails CustDet;
extern POSTBENEVER_RESP resp_pBenever;
extern POSTIMPS_RESP resp_postimps;
extern DISPLAY_IMPS display_imps;
extern searchWalkCust SearchWalkCust;
extern walkinCust walkinCustLimit;
extern ADDBENE add_bene_resp;
extern int Resend_flag;
extern int Reversal_type;
int flag_bene_verfication_status = 0;
char cDispalyQue[300];
int Flag_Display_WLMsg = 0;


void FPB::on_PushButton_ChangePasswd_pg26_clicked()
{
    ui->Old_password_edit_pg26->clear();
    ui->New_password_edit_pg26->clear();
    ui->Confirm_password_edit_pg26->clear();
    Check_Conditions();
    //    printf("Mobile in change pass %s",configLoginLeg2.ucmobileNumber);
    int iRet =-2,iRet1 =-2;
    memset(&OTP,0,sizeof(OTP));
    memset(&sndchange,0,sizeof(sndchange));
    OTP_page = 2;
    iRet = Prepare_encryption_Func();
    uidmessage::instance()->hide_Message();
    if(iRet != FUNCTION_SUCCESS)
    {
        if(iRet == SESSION_EXPIRED_FAILED)
            ui->stackedWidget->setCurrentIndex(0);
        else
            ui->stackedWidget->setCurrentIndex(25);
        return;
    }
    else if(iRet == FUNCTION_SUCCESS)
    {
        ui->Old_password_edit_pg26->setFocus();
        ui->stackedWidget->setCurrentIndex(26);
        return;
    }
    return;
}

void FPB::on_PushButton_UpdatePasswd_pg26_clicked()
{
    QString Potversion,Currentversion;
    CMisc miscObj;
    QString  Version = APP_VERSION;
    Potversion = Version.remove('.');
    Currentversion = QString::fromUtf8((const char*)configLoginLeg2.ucCurrentVersion);
    if(Potversion < Currentversion)
    {
        int k = iDisplayConsent ("Do you want to update \napplication ?");
        if(k == false)
        {
            uidmessage::instance()->hide_Message();
            return;
        }
        else
        {
            uidmessage::instance()->hide_Message();
            int iret_AU = FTP_download_function((char *)configLoginLeg2.ucCurrentVersion);
            if(iret_AU == FUNCTION_FAILED)
            {
                ui->stackedWidget->setCurrentIndex(12);
            }
        }
    }
    else
    {
        miscObj.DisplayMsgBox((char *)"Current version is updated version");
    }
    return;
}

void FPB::on_PushButton_Logout_pg26_clicked()
{
    if(iDisplayConsent ("Do you want to Logout ?") == false)
        return;
    Check_Conditions();
    int iRet =-2;
    iRet = Logout_Func();
    system("cd /usr/FINO/FPB/RESP/");
    system("rm /usr/FINO/FPB/RESP/OUTPUT.TXT");
    system("rm /usr/FINO/FPB/RESP/Intjson.txt");

    if(iRet != FUNCTION_SUCCESS)
    {

        if(iRet == SESSION_EXPIRED_FAILED)
            ui->stackedWidget->setCurrentIndex(0);
        else
            ui->stackedWidget->setCurrentIndex(0);
        return;
    }
    else if(iRet == FUNCTION_SUCCESS)
    {
        ui->lineEdit_userid->clear();
        ui->stackedWidget->setCurrentIndex(0); // TEMPORARY
        return ;
    }
    return;
}

void FPB::on_PushButton_Back_pg26_clicked()
{
    ui->stackedWidget->setCurrentIndex(12);
    return ;
}

int FPB::Logout_Func()
{
    uidmessage::instance()->hide_Message();
    uidmessage::instance()->Display_BusyMessage("Logout in process,please wait...");

    CMisc miscObj;
    int ret = -3;
    char ErrorMsg[100];
    char* jsondata;
    memset(ErrorMsg,0,sizeof(ErrorMsg));
    iGenerateRequestID((char *)username,(unsigned char *)&sndlogout.ucRequestId_Lreq);

    //    Json1::Value ArrayList(Json1::arrayValue);
    Json1::Value RequestedData,mainreq;

    mainreq["RequestId"] =(char *)sndlogout.ucRequestId_Lreq;
    mainreq["MethodId"] = "3";
    mainreq["TellerID"]= username;
    mainreq["SessionId"] =  (char *)configLoginLeg2.ucSessionId;
    mainreq["ChannelID"]= CHANNEL_ID;
    mainreq["SessionExpiryTime"]= (char *)configLoginLeg2.ucSessionExpiryTime;
    mainreq["IsEncrypt"]= false;
    mainreq["ProductCode"]= "";
    mainreq["Amount"] = "";
    RequestedData["client_id"] = CLIENT_ID;
    RequestedData["access_token"] = "";
    QString Buff = serialize1(Convertstr(RequestedData.toStyledString()));
    mainreq["RequestData"] = Buff.toAscii().data();
    jsondata = Convertstr(mainreq.toStyledString());
#if DEBUG
    qDebug()<<"------------->"<<jsondata;
#endif
    //-------------------------------------------------------------
    memset(gSendData,0,sizeof(gSendData));
    strncpy((char*)gSendData, jsondata, strlen(jsondata));
#if DEBUG
    printf("gSendData = %s\n\n",gSendData);
#endif
    //-------------------------------------------------------------

    memset(str_url,0,sizeof(str_url));
#ifdef PRODUCTION
    strcpy(str_url,URL_CREATION(LOGOUT_URL_PROD));
#else
    strcpy(str_url,URL_CREATION((char *)LOGOUT_URL));
#endif

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

    //  ret =file_download( "http://103.1.112.205:4444//UIService.svc/ProcessLoginRequest", jsondata,"/usr/FINO/FPB/RESP/LOGOUT.txt",120);


    system("mkdir /usr/FINO/FPB/RESP/");
    system("cd /usr/FINO/FPB/RESP/");
    system("rm /usr/FINO/FPB/RESP/Intjson.txt");
    //system("sed 's/\"{\"/{\"/g' /usr/FINO/FPB/RESP/LOGOUT.txt > /usr/FINO/FPB/RESP/Intjson.txt");
    //system("sed 's/\"}\",/\"},/g' /usr/FINO/FPB/RESP/Intjson.txt > /usr/FINO/FPB/RESP/LOGOUT.txt");
    system("sed 's/\"{\"/{\"/g' /usr/FINO/FPB/RESP/OUTPUT.TXT > /usr/FINO/FPB/RESP/Intjson.txt");
    system("sed 's/\"}\",/\"},/g' /usr/FINO/FPB/RESP/Intjson.txt > /usr/FINO/FPB/RESP/OUTPUT.TXT");
    Json1::Value root;
    root.clear();
    Json1::Reader reader;
    std::ifstream ReadData("/usr/FINO/FPB/RESP/OUTPUT.TXT");
    bool parsedSuccess = reader.parse(ReadData,root,false);
    ReadData.close();
    if(not parsedSuccess)
    {
        qDebug()<<" File not Found";
        return FUNCTION_FAILED;
    }
    else
    {
        strcpy(sndlogout.cLResponse_code ,Convertstr(root["ResponseCode"].asString()));
        strcpy(sndlogout.cLResponseMessage ,Convertstr(root["ResponseMessage"].asString()));
#if DEBUG
        printf("sndlogout.cLResponse_code%s",sndlogout.cLResponse_code);
        printf("sndlogout.cLResponseMessage%s",sndlogout.cLResponseMessage);
#endif
        if(strcmp(sndlogout.cLResponse_code,"0")==0)
        {
            uidmessage::instance()->hide_Message();
            sprintf(ErrorMsg,"%s",/*sndlogout.cLResponse_code,*/sndlogout.cLResponseMessage);
            miscObj.DisplayMsgBox(ErrorMsg);
            uidmessage::instance()->hide_Message();
            return FUNCTION_SUCCESS;
        }
        else
        {

            if(strcmp((char *)sndlogout.cLResponseMessage,"Session Expired.")==0)
            {
                uidmessage::instance()->hide_Message();
                sprintf(ErrorMsg,"%s",(char *)sndlogout.cLResponseMessage);
                miscObj.DisplayMsgBox(ErrorMsg);
                uidmessage::instance()->hide_Message();
                return SESSION_EXPIRED_FAILED;
            }
            sprintf(ErrorMsg,"%s",/*sndlogout.cLResponse_code,*/sndlogout.cLResponseMessage);
            miscObj.DisplayMsgBox(ErrorMsg);
            uidmessage::instance()->hide_Message();
            return FUNCTION_FAILED;
        }
    }
    return FUNCTION_SUCCESS;
}

void FPB::on_OK_pushButton_pg34_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    return;
}

void FPB::on_Forgot_Password_Button_User_pg1_clicked()
{
    CMisc miscObj;

    if(ui->lineEdit_userid->text().isEmpty())
    {
        miscObj.DisplayMsgBox((char *)"Username is Mandatory");
        return;
    }

    memset(&FORPASS_State,0,sizeof(FORPASS_State));
    Check_Conditions();
    ui->User_id_password_edit->setText(username);
    ui->User_id_password_edit->setReadOnly(true);
    ui->New_password_edit->clear();
    ui->Confirm_password_edit->clear();
    ui->Secret_answer_edit->clear();
    OTP_page = 1;
    int iRet = -2;
    iRet = Prepare_Change_Func();
    if(iRet != FUNCTION_SUCCESS)
    {
        ui->stackedWidget->setCurrentIndex(0);
    }
    else if(iRet == FUNCTION_SUCCESS)
    {
        ui->stackedWidget->setCurrentIndex(24);
    }
    return;
}

int FPB::FTP_download_function(char *filename)
{
    int ret = 0;
    char buff[1024];
    int flag_temp = -1;
    char lcstr_buff[100];
    //char appver[64];
    memset(buff,0,sizeof(buff));
    memset(lcstr_buff,0,sizeof(lcstr_buff));
#if DEBUG
    printf("UPDATE_APP filename = %s\n",filename);
#endif
    for(int i=0;i<5;i++)
    {
        memset(buff,0,sizeof(buff));
        sprintf(buff,"wget -c -T 24   ftp://Visiontek_FPB:Visiontek'$'123@sftp.finobank.com/FPB_Visiontek/%s.zip -O /usr/%s.zip",filename,filename);


        // sprintf(buff,"wget -c -T 24   ftp://Visiontek_FPB:V1si0nFp8@ftp.finopaytech.com/FPBVisiontek/%s.zip -O /usr/%s.zip",filename,filename);
        // sprintf(buff,"wget -c -T 24   ftp://visiontek_field:H'$'jmkjbb33@ftp.finopaytech.com/%s.zip -O /usr/%s.zip",filename,filename);
        // sprintf(buff,"wget -c -T 24   ftps://Visiontek_FPB:Fino@2020@ftp://ftp.finopaytech.com/FPBVisiontek/%s.zip -O /usr/%s.zip",filename,filename);

        uidmessage::instance()->Display_BusyMessage("Downloading Application..Please wait..");
        ret=system(buff);

        if(ret == 0)
        {
            uidmessage::instance()->hide_Message();
            flag_temp = 1;
            i=6;
            //                return FTP_SUCCESS;
        }
        if(ret != 0)
        {
            uidmessage::instance()->hide_Message();
        }
    }

    if(flag_temp == 1)
    {
        flag_temp = -1;
        gl11_lcdbklt_on();
        uidmessage::instance()->hide_Message();
        uidmessage::instance()->Display_BusyMessage("File download success");
        gl11_buzz_on();
        sleep(1);
        gl11_buzz_off();
        uidmessage::instance()->hide_Message();
    }
    else
    {
        gl11_lcdbklt_on();
        uidmessage::instance()->hide_Message();
        uidmessage::instance()->Display_BusyMessage("File download failed");
        gl11_buzz_on();
        sleep(1);
        gl11_buzz_off();
        uidmessage::instance()->hide_Message();
        //            return FUNCTION_FAILED;
        memset(buff,0,sizeof(buff));
        sprintf(buff,"rm -rf /root/usr/%s",filename);
        ret = system(buff);
        return FUNCTION_FAILED;
    }


    chdir("/usr/"); // change to root directory
    system("rm -rf FPB_VT");
    system("rm -rf install.sh");
    uidmessage::instance()->Display_BusyMessage("updating application..");
    gl11_buzz_on();
    sleep(2);
    gl11_buzz_off();
    uidmessage::instance()->hide_Message();
    memset(lcstr_buff,0,sizeof(lcstr_buff));
    sprintf(lcstr_buff,"unzip %s.zip",filename);
#if DEBUG
    printf("FIlename to unzip : %s\n",lcstr_buff);
#endif
    system(lcstr_buff);
    system("chmod +x install.sh");
    system("./install.sh");
    return FUNCTION_SUCCESS;
}

void FPB::on_Balance_show_button_clicked()
{
    //printf("Mer_Latest_Balance = %s\n",Mer_Latest_Balance);
    //    strcpy(Mer_Latest_Balance,"12878.05");
    if(show_bal == 0)
    {
        QString temp1;
        QString bal = QString::fromUtf8(Mer_Latest_Balance);
        bool decimal;
        decimal = bal.contains('.');
        if(decimal == false)
        {
            bal = bal.append(".00");
        }

        int i=0;
        int star_size = bal.length();
        temp1.clear();
        for(i=0;i<star_size;i++)
        {
            temp1 = temp1.append('*');
        }

        //       qDebug()<<"Available Balance"<<bal;
        QString BL = "Balance : " + temp1;
        //      qDebug()<<"show_bal(1) BL == "<<BL<<"temp1 == "<<temp1;
        ui->Dash_Latest_Balance_Label_pg12->setText(BL);
        show_bal = 1;
        return;
    }

    if(show_bal == 1)
    {
        QString temp1 = QString :: fromUtf8(Mer_Latest_Balance);
        QString BL = "Balance : " + temp1;
        //      qDebug()<<"show_bal(0) BL == "<<BL<<"temp1 == "<<temp1;
        ui->Dash_Latest_Balance_Label_pg12->setText(BL);
        show_bal = 0;
        return;
    }
    return;
}

void FPB::on_OldNew_ChangePasswd_Back_button__clicked()
{
    if(Flag_FirstTime_Login == 1)
    {
        //      qDebug()<<"Flag_FirstTime_Login11";
        ui->stackedWidget->setCurrentIndex(0);
    }
    else
        ui->stackedWidget->setCurrentIndex(25);
    return;
}


void FPB::on_OldNew_ChangePasswd_Next_button__clicked()
{
    if(strcmp(ui->New_password_edit_pg26->text().trimmed().toAscii().data(),ui->Confirm_password_edit_pg26->text().trimmed().toAscii().data())!=0)
    {
        qDebug()<<"Password not match";
    }
    Check_Conditions();
    //   printf("Mobile Number123 %s",configLoginLeg2.ucmobileNumber);
    ui->OTP_Lineedit->clear();

    int iRet =-2,iRet1 = -2;
    iRet = Prepare_OTP_Func(2);

    if(iRet != FUNCTION_SUCCESS)
    {
        uidmessage::instance()->hide_Message();
        if(iRet == SESSION_EXPIRED_FAILED)
            ui->stackedWidget->setCurrentIndex(0);
        else
        {
            if(Flag_FirstTime_Login == 1)
            {
                //            if(iRet == FUNC_BACK)
                ui->stackedWidget->setCurrentIndex(26);
                //            else
                //                ui->stackedWidget->setCurrentIndex(0);
            }
            else
            {
                if(iRet == FUNC_BACK)
                    ui->stackedWidget->setCurrentIndex(26);
                else
                    ui->stackedWidget->setCurrentIndex(25);
            }
        }
        return;
    }
    else if(iRet == FUNCTION_SUCCESS)
    {
        //     qDebug()<<"*******************NEW PASSWORD 1 == "<<ui->New_password_edit_pg26->text().trimmed().toAscii().data();
        //      printf("*******************NEW PASSWORD 3131 OTP.ucnewpassword == %s\n",OTP.ucnewpassword);
        uidmessage::instance()->hide_Message();
        ui->stackedWidget->setCurrentIndex(22);
        return;
    }
    return;
}


void FPB::on_Resend_otp_button_clicked()
{

    Check_Conditions();
    ui->OTP_Lineedit->clear();
    int iRet =-2,iRet1 = -2;
    if(OTP_page == 5)
    {
        Resend_flag = 1;
        iRet = Reversal_otp_request();
        Resend_flag = 0;
    }
    if(OTP_page == 7)
    {
        Resend_flag = 1;
        iRet = Agent_Auth();
        Resend_flag = 0;
    }
    else if(OTP_page == 3)
    {
        Resend_flag = 1;
        uidmessage::instance()->hide_Message();
        uidmessage::instance()->Display_BusyMessage("Transaction Request in Progress");
        iRet = Otpimpsnft();
        Resend_flag = 0;
    }

    else
    {
        iRet = Resend_function();
    }

    if(iRet != FUNCTION_SUCCESS)
    {
        ui->OTP_Lineedit->clear();
        uidmessage::instance()->hide_Message();
        if(iRet == SESSION_EXPIRED_FAILED)
            ui->stackedWidget->setCurrentIndex(0);
        else
            ui->stackedWidget->setCurrentIndex(22);
    }
    else if(iRet == FUNCTION_SUCCESS)
    {
        uidmessage::instance()->hide_Message();
        ui->OTP_Lineedit->clear();
        ui->OTP_Lineedit->setFocus();
        ui->stackedWidget->setCurrentIndex(22);
    }
    return;
}


//int FPB::Resend_function()
//{
//    HostComm hComm;
//    CMisc miscObj;
//    int OTP_len = 0, response_timeout1 = 120, ret = -3, res = -3, iRet = -3;
//    char OTP_Buff[10000];
//    char ErrorMsg[100];
//    memset(ErrorMsg,0,sizeof(ErrorMsg));
//    memset(OTP.ucMethodID_req,0, sizeof(OTP.ucMethodID_req));
//    memset(OTP.ucTellerID_req,0, sizeof(OTP.ucTellerID_req));
//    memset(OTP.ucSessionID_req,0, sizeof(OTP.ucSessionID_req));
//    memset(OTP.ucMessageId_req,0, sizeof(OTP.ucMessageId_req));
//    memset(OTP.ucIsEncrypt_req,0, sizeof(OTP.ucIsEncrypt_req));
//    memset(OTP.ucCustomerMobileNo_req,0, sizeof(OTP.ucCustomerMobileNo_req));
//    memset(OTP.ucChannelID_req,0, sizeof(OTP.ucChannelID_req));

//    iGenerateRequestID((char *)username,(unsigned char *)&OTP.ucRequestID_req);
//    memcpy(OTP.ucMethodID_req,"1",1);
//    memcpy(OTP.ucTellerID_req,username,strlen((char *)username));
//    memcpy(OTP.ucChannelID_req,"1",1);
//    memcpy(OTP.ucIsEncrypt_req,"false",5);
//    memcpy(OTP.ucCustomerMobileNo_req,configLoginLeg2.ucmobileNumber, strlen((char *)configLoginLeg2.ucmobileNumber));
//    memcpy(OTP.ucMessageId_req,"42",2);
//    memcpy(OTP.ucRDFMethodID_req,"4",1);
//    memcpy(OTP.ucSessionID_req,configLoginLeg2.ucSessionId,strlen((char *)configLoginLeg2.ucSessionId));
//    printf("OTP.ucencrypkey456%s\n",OTP.ucencrypkey);

//    ret	= makeJSONOTP(11);
//    if(ret!=FUNC_OK)
//    {
//        qDebug()<<"JSON Creation failed";
//        return FUNC_ERROR;
//    }

//    uidmessage::instance()->hide_Message();
//    hComm.close_connection();
//    res = hComm.open_connect(SERVERIP,SERVERPORT);
//    if(res != 0)
//    {
//        qDebug() << "Failed to connect to the Server";
//        uidmessage::instance()->hide_Message();
//        uidmessage::instance()->DisplayMessage("Server Not Connected");
//        qDebug()<<"FUNCTION_FAILED";
//        ui->stackedWidget->setCurrentIndex(0);
//        return FUNCTION_FAILED;
//    }


//    int len_gSendData = strlen((char *)gSendData);
//    memset(OTP_Buff,0,sizeof(OTP_Buff));
//    if((hComm.host_2Sendrecv((unsigned char *)gSendData,len_gSendData,( unsigned char*)OTP_Buff,(int *)&OTP_len,(int)response_timeout1,0))!=SUCCESS)
//    {
//        printf("No Response From Server\n");
//        hComm.close_connection();
//        miscObj.DisplayMsgBox((char *)"Response Time-Out");
//        uidmessage::instance()->hide_Message();
//        qDebug()<<"***   Response Time-Out   ***";
//        ui->stackedWidget->setCurrentIndex(1);
//    }
//    hComm.close_connection();


//    QString OTPBuff = QString::fromLocal8Bit(OTP_Buff);
//    bool check = OTPBuff.contains("ResponseCode");
//    if(check == false)
//    {
//        miscObj.DisplayMsgBox("INVALID DATA ");
//        uidmessage::instance()->hide_Message();
//        return FUNCTION_FAILED;
//    }


//    OTPBuff.remove("\\");
//    OTPBuff.remove("{");
//    OTPBuff.remove("}");
//    OTPBuff.remove('"');
//    qDebug()<<"OTPBuff"<<OTPBuff;
//    QStringList query = OTPBuff.split(",");
//    for(int i=0 ; i < query.length() ; i++)
//        qDebug()<<"Query Response"<<query.at(i);
//    QStringList RequestId = query.filter("RequestId");
//    QStringList DisplayMessage = query.filter("DisplayMessage");
//    QStringList ResponseCode = query.filter("ResponseCode");
//    QStringList ResponseMessage = query.filter("ResponseMessage");

//    uidmessage::instance()->hide_Message();
//    QString str ,strToFind,value,value1,value2;
//    str = ResponseCode.at(0);
//    strToFind = ":";
//    value1 = str.mid(str.indexOf(strToFind)+strToFind.length());
//    memcpy(OTP.ucResponseCode,value1.trimmed().toAscii().data(), strlen(value1.trimmed().toAscii().data()));
//    qDebug()<<"value1"<<value1;
//    if(value1 =="0")
//    {
//        str = DisplayMessage.at(2);
//        strToFind = ":";
//        value2 = str.mid(str.indexOf(strToFind)+strToFind.length());
//        memcpy(OTP.ucRDRequestId,value2.trimmed().toAscii().data(), strlen(value2.trimmed().toAscii().data()));
//        str= RequestId.at(2);
//        strToFind= "RequestId:";
//        value = str.mid(str.indexOf(strToFind)+strToFind.length());
//        memcpy(OTP.ucRDRequestId,value.trimmed().toAscii().data(), strlen(value.trimmed().toAscii().data()));
//        qDebug()<<"OTP.ucRDRequestId"<<OTP.ucRDRequestId;
//        ui->stackedWidget->setCurrentIndex(22);
//        return FUNCTION_SUCCESS;
//    }
//    else
//    {
//        str = ResponseMessage.at(0);
//        strToFind = ":";
//        value2 = str.mid(str.indexOf(strToFind)+strToFind.length());
//        memcpy(OTP.ucResponseMessage,value2.trimmed().toAscii().data(), strlen(value2.trimmed().toAscii().data()));
//        sprintf(ErrorMsg,"Response Code:%s \n Message:%s",(char *)OTP.ucResponseCode,(char *)OTP.ucResponseMessage);
//        miscObj.DisplayMsgBox(ErrorMsg);
//        uidmessage::instance()->hide_Message();
//        return FUNCTION_FAILED;
//    }


//    return FUNCTION_SUCCESS;
//}



void FPB::on_Forget_button_clicked()
{
    memset(&FORPASS_State,0,sizeof(FORPASS_State));
    Check_Conditions();
    ui->User_id_password_edit->setText(username);
    ui->User_id_password_edit->setReadOnly(true);
    ui->New_password_edit->clear();
    ui->Confirm_password_edit->clear();
    ui->Secret_answer_edit->clear();
    OTP_page = 1;
    int iRet = -2;
    iRet = Prepare_Change_Func();
    if(iRet != FUNCTION_SUCCESS)
    {
        ui->stackedWidget->setCurrentIndex(0);
    }
    else if(iRet == FUNCTION_SUCCESS)
    {
        ui->stackedWidget->setCurrentIndex(24);
    }
    return;
}

int FPB::Prepare_Change_Func()
{

    CMisc miscObj;
    int FOR_len = 0, response_timeout1 = 120, ret = -3, res = -3, iRet = -3;
    char ErrorMsg[100];


    uidmessage::instance()->hide_Message();
    uidmessage::instance()->Display_BusyMessage("Please Wait...");


    memset(ErrorMsg,0,sizeof(ErrorMsg));
    memset(FORPASS_State.ucMethodID_req,0, sizeof(FORPASS_State.ucMethodID_req));
    memset(FORPASS_State.ucTellerID_req,0, sizeof(FORPASS_State.ucTellerID_req));
    memset(FORPASS_State.ucSessionID_req,0, sizeof(FORPASS_State.ucSessionID_req));
    memset(FORPASS_State.ucUser_id_req,0, sizeof(FORPASS_State.ucUser_id_req));
    memset(FORPASS_State.ucIsEncrypt_req,0, sizeof(FORPASS_State.ucIsEncrypt_req));

    memset(username,0,sizeof(username));
    strcpy(username,ui->lineEdit_userid->text().toAscii().data());
    iGenerateRequestID((char *)username,(unsigned char *)&FORPASS_State.ucRequestID_req);
    memcpy(FORPASS_State.ucMethodID_req,"8",1);
    memcpy(FORPASS_State.ucTellerID_req,username,sizeof(username));
    memcpy(FORPASS_State.ucIsEncrypt_req,"false",5);


    ret	= makeJSONFORGET();
    if(ret!=FUNC_OK)
    {
        qDebug()<<"JSON Creation failed";
        return FUNC_ERROR;
    }


#ifdef UAT_HTTP     // File Download

    hComm.close_connection();
    res = hComm.open_connect(SERVERIP,SERVERPORT);
    if(res != 0)
    {
        qDebug() << "Failed to connect to the Server";
        uidmessage::instance()->hide_Message();
        uidmessage::instance()->DisplayMessage("Server Not Connected");
        qDebug()<<"FUNCTION_FAILED";
        ui->stackedWidget->setCurrentIndex(2);
        return FUNCTION_FAILED;
    }

    uidmessage::instance()->hide_Message();
    uidmessage::instance()->Display_BusyMessage("Transmitting .....");

    int len_gSendData = strlen((char *)gSendData);
    memset(BLEQ_Buff,0,sizeof(BLEQ_Buff));
    memset(oBuff,0,sizeof(oBuff));
    if((hComm.host_2Sendrecv((unsigned char *)gSendData,len_gSendData,( unsigned char*)oBuff,(int *)&BLEQ_len,(int)response_timeout1,0))!=SUCCESS)
    {
        printf("No Response From Server\n");
        hComm.close_connection();
        miscObj.DisplayMsgBox((char *)"Response Time-Out");
        uidmessage::instance()->hide_Message();
        qDebug()<<"***   Response Time-Out   ***";
        ui->stackedWidget->setCurrentIndex(1);
        return FUNCTION_FAILED;
    }
    hComm.close_connection();

#else     // File Download full condition
#if DEBUG
    printf("gSendData = %s\n\n",gSendData);
#endif
    memset(str_url,0,sizeof(str_url));
#ifdef PRODUCTION
    strcpy(str_url,URL_CREATION(AEPS_WITHDRAWAL_URL_PROD));
#else
    strcpy(str_url,URL_CREATION((char *)CHANGEPASSWD_OTP_URL));
#endif

    int retk = file_download(str_url, (char *)gSendData,"/usr/FINO/FPB/RESP/OUTPUT.TXT",120);
    if(retk!=0)
    {
        qDebug()<<"retk = "<<retk;
        sleep(1);
        uidmessage::instance()->hide_Message();
        return FUNCTION_FAILED;
    }

#endif


    ret=Forget_question_parsing();
    printf("AEPSLTS.ucResponseCode%s/nAEPSLTS.ucResponseMessage%s/n",FORPASS_State.ucResponseCode,FORPASS_State.ucResponseMessage);
    if(ret!=FUNC_OK)
    {
        sprintf(ErrorMsg,"%s",(char *)FORPASS_State.ucResponseMessage);
        miscObj.DisplayMsgBox(ErrorMsg);
        uidmessage::instance()->hide_Message();
        return FUNCTION_FAILED;
    }
    uidmessage::instance()->hide_Message();

    int j=0;
    ui->Secret_Question->clear();
    for(j=0;j<CountSQ;j=j++)
    {
        //            // qDebug()<<"display_data[j].AEPS_LTS_data.iDate"<<l++;
#if DEBUG
        printf("secret_data[j].FOR_PASS_STAT.questionGroupj=%d%s\n",j,secret_data[j].questionGroup);
        printf("FOR_PASS_STAT.questionIdj=%d%s\n",j,secret_data[j].questionId);
        printf("secret_data[j].FOR_PASS_STAT.questionDescj=%d%s\n",j,secret_data[j].questionDesc);
        printf("secret_data[j].FOR_PASS_STAT.answerj=%d%s\n",j,secret_data[j].answer);
#endif
        char Display[100];
        memset(Display,0,strlen(Display));

        if(strlen((char *)secret_data[j].questionDesc)>36)
        {
            memcpy(Display,secret_data[j].questionDesc,35);
            strcat(Display," \n");
            strcat(Display,(char *)&secret_data[j].questionDesc[35]);
        }
        else
        {
            strcpy((char *)Display,(char *)secret_data[j].questionDesc);
        }
        ui->Secret_Question->addItem(Display);

    }
    ui->User_id_password_edit->setReadOnly(false);
    ui->User_id_password_edit->setText(username);
    //  ui->stackedWidget->setCurrentIndex(24);
    // memset(&FORPASS_State,0,sizeof(FORPASS_State));
    return FUNCTION_SUCCESS;
}


int FPB::makeJSONFORGET()//----------------------
{
    char tempbuff[30];
    int iRet=-2,slen;

    memset(gSendData,0,sizeof(gSendData));


    iRet = iMakeRequest(10);//changed by vaishnavi
    if (iRet!=FUNC_OK)
    {
        uidmessage::instance()->hide_Message();
        return FUNC_ERROR;
    }

    //consolprint(Recvdata);
    slen=strlen((char *)Recvdata);
    slen-=2;
    memset(gSendData,0,sizeof(gSendData));
#ifdef UAT_HTTP     // File Download
    strncpy((char *)gSendData,"POST /UIService.svc/ProcessLoginRequest HTTP/1.1\r\n",52);
    strncat((char *)gSendData,"Host: 202.58.125.20\r\n",27);//9013
    strncat((char *)gSendData,"Content-Type: application/json; charset=utf-8\r\n",49);
    sprintf(tempbuff,"Content-Length: %d\r\n",slen);
    strncat((char *)gSendData,tempbuff,strlen(tempbuff));
    strcat((char *)gSendData,(char *)Recvdata);
#else
    strncpy((char *)gSendData,(char *)Recvdata,strlen((char *)Recvdata));
#endif


    system("cd /usr/FINO/FPB/REQ/");
    system("rm /usr/FINO/FPB/REQ/FOR_REQ.TXT");
    FILE *fp2 = NULL;
    fp2 =fopen("/usr/FINO/FPB/REQ/FOR_REQ.TXT","a");
    fwrite(gSendData,strlen((char *)gSendData),1,fp2);
    //ttestall(0,100);
    sleep(1);
    fclose(fp2);

    return FUNC_OK;

}


int FPB::iSetFORGET(int tagID, unsigned char *value)
{
    switch(tagID)
    {
    case  1:
        strcpy((char *)value,(char *)FORPASS_State.ucRequestID_req);
        printf("isetFORGET%s/n",value);
        break;
    case  2:
        strcpy((char *)value,(char *)FORPASS_State.ucMethodID_req);//Method id
        printf("isetFORGET%s/n",value);
        break;
    case  3:
        strcpy((char *)value,(char *)FORPASS_State.ucTellerID_req);//teller id
        printf("isetFORGET%s/n",value);
        break;
    case  4:
        strcpy((char *)value,(char *)FORPASS_State.ucIsEncrypt_req);//is encrypt
        printf("isetFORGET%s/n",value);
        break;
    default:
        return FUNC_ERROR;
    }
    return FUNC_OK;
}

int FPB::iSetCHANGE(int tagID, unsigned char *value)
{
    switch(tagID)
    {
    case 1:
        strcpy((char *)value,(char *)sndchange.ucRequestId_CPreq);
        break;
    case 2:
        strcpy((char *)value,(char *)sndchange.ucTellerID_CPreq);//Method id
        break;
    case 3:
        strcpy((char *)value,(char *)sndchange.ucIsEncrypt_CPreq);//teller id
        break;
    case 4:
        strcpy((char *)value,(char *)sndchange.ucSessionId_CPreq);//session key
        break;
    case 5:
        strcpy((char *)value,(char *)sndchange.ucMethodId_CPreq);//token id
        break;
    case 6:
        strcpy((char *)value,(char *)sndchange.uccustomerNo_CPreq);//is encrypt
        break;
    case 7:
        strcpy((char *)value,(char *)sndchange.ucauthid_CPreq);
        break;
    case 8:
        strcpy((char *)value,(char *)sndchange.ucRDMethodID_CPreq);
        break;
    case 9:
        strcpy((char *)value,(char *)sndchange.ucRDRequestID_CPreq);
        break;
    case 10:
        strcpy((char *)value,(char *)sndchange.ucRDOTPPIN_CPreq);
        break;
    case 11:
        strcpy((char *)value,(char *)sndchange.ucMessageID_CPreq);
        break;
    case 12:
        strcpy((char *)value,(char *)sndchange.ucSessionExpiryTime_CPreq);
        break;
    case 13:
        strcpy((char *)value,(char *)sndchange.ucUserID_CPreq);
        break;
    case 14:
        strcpy((char *)value,(char *)sndchange.ucOldPassword_CPreq);
        break;
    case 15:
        strcpy((char *)value,(char *)sndchange.ucNewPassword_CPreq);
        break;

    default:
        return FUNC_ERROR;
    }
    return FUNC_OK;
}




void FPB::on_Forget_password_next_button_clicked()
{
    memset(&OTP,0,sizeof(OTP));
    Check_Conditions();
    ui->OTP_Lineedit->clear();
    int iRet =-2,iRet1 = -2;
    iRet = Prepare_OTP_Func(1);
    if(iRet != FUNCTION_SUCCESS)
    {
        uidmessage::instance()->hide_Message();
        ui->stackedWidget->setCurrentIndex(24);
    }
    else if(iRet == FUNCTION_SUCCESS)
    {
        iRet1 = Prepare_encryption_Func();
        ui->stackedWidget->setCurrentIndex(22);
    }
    return;

}


int FPB::Prepare_OTP_Func(int flag)
{
    uidmessage::instance()->hide_Message();
    uidmessage::instance()->Display_BusyMessage("Please Wait...");

    //   printf("Mobile Number456 %s",configLoginLeg2.ucmobileNumber);
    HostComm hComm;
    CMisc miscObj;
    int OTP_len = 0, response_timeout1 = 120, ret = -3, res = -3, iRet = -3;
    char OTP_Buff[10000];
    char ErrorMsg[100];
    QString new_pass,confirm_pas,answer,old_pass;


    if(flag==1)
    {
        new_pass =  ui->New_password_edit->text().trimmed();
        confirm_pas =  ui->Confirm_password_edit->text().trimmed();
        answer = ui->Secret_answer_edit->text().trimmed();
    }
    if(flag==2)
    {
        old_pass =  ui->Old_password_edit_pg26->text().trimmed();
        new_pass =  ui->New_password_edit_pg26->text().trimmed();
        confirm_pas =  ui->Confirm_password_edit_pg26->text().trimmed();
        if(old_pass.isEmpty())
        {
            miscObj.DisplayMsgBox((char *)"Please Enter Old Password");
            return FUNC_BACK;
        }

    }

    if(new_pass.isEmpty())
    {
        miscObj.DisplayMsgBox((char *)"Please Enter New Password");
        return FUNC_BACK;
    }
    if(confirm_pas.isEmpty())
    {
        miscObj.DisplayMsgBox((char *)"Please Enter Confirm Password");
        return FUNC_BACK;
    }
    if(new_pass.contains("fino",Qt::CaseInsensitive))
    {
        miscObj.DisplayMsgBox((char *)"Password cannot contain Fino word");
        return FUNC_BACK;
    }

    bool isdigit= true,isUpper= true,isLower= true,isSymbol= true,checkpass = true;
    QRegExp exp1 ("[A-Z]");
    QRegExp exp2 ("[a-z]");
    QRegExp exp3 ("[0-9]");
    QRegExp exp4("[^A-Za-z0-9]");
    if(exp1.indexIn(new_pass)< 0)
    {
        isdigit = false;
    }
    else
    {
        isdigit = true;
    }

    if(exp2.indexIn(new_pass)< 0)
    {
        isUpper = false;
    }
    else
    {
        isUpper = true;
    }



    if(exp3.indexIn(new_pass)< 0)
    {
        isLower = false;
    }
    else
    {
        isLower = true;
    }


    if(!(exp4.indexIn(new_pass)< 0))
    {
        isSymbol = true;
    }
    else
    {
        isSymbol = false;
    }

    checkpass =(isdigit && isUpper && isLower && isSymbol);
    if(checkpass == false)
    {

        miscObj.DisplayMsgBox((char *)"Password Contain atleast 1 Uppercase,1 lowercase,1 digit and 1 special charcter");
        ui->New_password_edit->clear();
        ui->Confirm_password_edit->clear();
        return FUNC_BACK;
    }

    int y = QString::compare(new_pass, old_pass, Qt::CaseSensitive);
    if(!y)
    {
        miscObj.DisplayMsgBox((char *)"New and Old password cannot be same");
        ui->New_password_edit->clear();
        ui->Confirm_password_edit->clear();
        return FUNC_BACK;
    }
    if(flag==1)
    {
        int x = QString::compare(new_pass, confirm_pas, Qt::CaseSensitive);
        if(x)
        {
            miscObj.DisplayMsgBox((char *)"Password donot Match");
            ui->New_password_edit->clear();
            ui->Confirm_password_edit->clear();
            return FUNC_BACK;
        }

        if(answer.isEmpty())
        {
            miscObj.DisplayMsgBox((char *)"Answer is mandatory");
            ui->New_password_edit->clear();
            ui->Confirm_password_edit->clear();
            return FUNC_BACK;
        }
    }
    if(flag==2)
    {
        //   qDebug()<<"new_pass"<<new_pass;
        //   qDebug()<<"confirm pass"<<confirm_pas;
        int x = QString::compare(new_pass, confirm_pas, Qt::CaseSensitive);
        if(x)
        {
            miscObj.DisplayMsgBox((char *)"Password donot Match");
            ui->New_password_edit->clear();
            ui->Confirm_password_edit_pg26->clear();
            return FUNC_BACK;
        }
    }

    if(flag==1)
    {
        int cb = ui->Secret_Question->currentIndex();
#if DEBUG
        printf("secret_data[j].FOR_PASS_STAT.answerj=%s\n",secret_data[cb].questionId);
        printf("secret_data[j].FOR_PASS_STAT.answerj=%s\n",secret_data[cb].questionGroup);
#endif
        sprintf(( char *)OTP.ucQstID,"%s",secret_data[cb].questionId);
        sprintf(( char *)OTP.ucQstgroup,"%s",secret_data[cb].questionGroup);
        memcpy(OTP.ucanswer,ui->Secret_answer_edit->text().trimmed().toAscii().data(),strlen((char *)ui->Secret_answer_edit->text().trimmed().toAscii().data()));
        memcpy(OTP.ucnewpassword,new_pass.trimmed().toAscii().data(),strlen(new_pass.trimmed().toAscii().data()));
    }

    if(flag==2)
    {
        memset(OTP.ucnewpassword,0, sizeof(OTP.ucnewpassword));
        memset(OTP.ucoldpassword,0, sizeof(OTP.ucoldpassword));


        memcpy(OTP.ucnewpassword,ui->New_password_edit_pg26->text().trimmed().toAscii().data(),strlen((char *)ui->New_password_edit_pg26->text().trimmed().toAscii().data()));
        memcpy(OTP.ucoldpassword,ui->Old_password_edit_pg26->text().trimmed().toAscii().data(),strlen((char *)ui->Old_password_edit_pg26->text().trimmed().toAscii().data()));


    }





    memset(ErrorMsg,0,sizeof(ErrorMsg));
    memset(OTP.ucMethodID_req,0, sizeof(OTP.ucMethodID_req));
    memset(OTP.ucTellerID_req,0, sizeof(OTP.ucTellerID_req));
    memset(OTP.ucSessionID_req,0, sizeof(OTP.ucSessionID_req));
    memset(OTP.ucMessageId_req,0, sizeof(OTP.ucMessageId_req));
    memset(OTP.ucIsEncrypt_req,0, sizeof(OTP.ucIsEncrypt_req));
    memset(OTP.ucCustomerMobileNo_req,0, sizeof(OTP.ucCustomerMobileNo_req));
    memset(OTP.ucChannelID_req,0, sizeof(OTP.ucChannelID_req));

    iGenerateRequestID((char *)username,(unsigned char *)&OTP.ucRequestID_req);
    memcpy(OTP.ucMethodID_req,"1",1);
    memcpy(OTP.ucRDFMethodID_req,"1",1);

    memcpy(OTP.ucTellerID_req,username,strlen((char *)username));
    memcpy(OTP.ucChannelID_req,CHANNEL_ID,strlen(CHANNEL_ID));
    memcpy(OTP.ucIsEncrypt_req,"false",5);

    if(OTP_page == 1)
    {
        memcpy(OTP.ucCustomerMobileNo_req,configLoginLeg1.ucmobileNumber, strlen((char *)configLoginLeg1.ucmobileNumber));
    }
    else
    {
        memcpy(OTP.ucCustomerMobileNo_req,configLoginLeg2.ucmobileNumber, strlen((char *)configLoginLeg2.ucmobileNumber));////
    }

    memcpy(OTP.ucSessionID_req,configLoginLeg2.ucSessionId,strlen((char *)configLoginLeg2.ucSessionId));
    memcpy(OTP.ucMessageId_req,"42",2);
    ret	= makeJSONOTP(11);
    if(ret!=FUNC_OK)
    {
        qDebug()<<"JSON Creation failed";
        return FUNC_ERROR;
    }

#ifdef UAT_HTTP     // File Download

    uidmessage::instance()->hide_Message();
    hComm.close_connection();
    res = hComm.open_connect(SERVERIP,SERVERPORT);
    if(res != 0)
    {
        qDebug() << "Failed to connect to the Server";
        uidmessage::instance()->hide_Message();
        uidmessage::instance()->DisplayMessage("Server Not Connected");
        qDebug()<<"FUNCTION_FAILED";
        ui->stackedWidget->setCurrentIndex(0);
        return FUNCTION_FAILED;
    }


    int len_gSendData = strlen((char *)gSendData);
    memset(OTP_Buff,0,sizeof(OTP_Buff));
    memset(oBuff, 0, sizeof(oBuff));
    if((hComm.host_2Sendrecv((unsigned char *)gSendData,len_gSendData,( unsigned char*)oBuff,(int *)&OTP_len,(int)response_timeout1,0))!=SUCCESS)
    {
        printf("No Response From Server\n");
        hComm.close_connection();
        miscObj.DisplayMsgBox((char *)"Response Time-Out");
        uidmessage::instance()->hide_Message();
        qDebug()<<"***   Response Time-Out   ***";
        ui->stackedWidget->setCurrentIndex(1);
    }
    hComm.close_connection();


#else     // File Download full condition

#if DEBUG
    printf("gSendData = %s\n\n",gSendData);
#endif

    memset(str_url,0,sizeof(str_url));
#ifdef PRODUCTION
    strcpy(str_url,URL_CREATION(OLDNEW_CHANGE_PASSWORD_URL_PROD));
#else
    if(OTP_page == 1)
    {
        strcpy(str_url,URL_CREATION((char *)FORGET_URL));
    }
    else
    {
        strcpy(str_url,URL_CREATION((char *)OLDNEW_CHANGE_PASSWORD_URL));
    }
#endif

    int retk = file_download(str_url, (char *)gSendData,"/usr/FINO/FPB/RESP/OUTPUT.TXT",120);
    if(retk!=0)
    {
        qDebug()<<"retk = "<<retk;
        sleep(1);
        uidmessage::instance()->hide_Message();
        return FUNCTION_FAILED;
    }

#endif
    system("cd /usr/FINO/FPB/RESP/");
    system("rm /usr/FINO/FPB/RESP/OUTPUT.TXT");
    QString OTPBuff = QString::fromLocal8Bit(oBuff);
    bool check = OTPBuff.contains("ResponseCode");
    if(check == false)
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
    memcpy(OTP.ucResponseCode,value1.trimmed().toAscii().data(), strlen(value1.trimmed().toAscii().data()));
    if(value1 =="0")
    {
        //       printf("*******************NEW PASSWORD 1212 OTP.ucnewpassword == %s\n",OTP.ucnewpassword);
        str = DisplayMessage.at(2);
        strToFind = ":";
        value2 = str.mid(str.indexOf(strToFind)+strToFind.length());
        memcpy(OTP.ucDisplayMessage,value2.trimmed().toAscii().data(), strlen(value2.trimmed().toAscii().data()));
        str= RequestId.at(2);
        strToFind= "RequestId:";
        value = str.mid(str.indexOf(strToFind)+strToFind.length());
        memcpy(OTP.ucRDRequestId,value.trimmed().toAscii().data(), strlen(value.trimmed().toAscii().data()));
#if DEBUG
        qDebug()<<"OTP.ucRDRequestId"<<OTP.ucRDRequestId;
        qDebug()<<"Insude the if case";
        printf("Mobile Number OTP Page %s",configLoginLeg2.ucmobileNumber);
#endif
        char otp_mobileno[20], otp_message[700];
        memset(otp_mobileno, 0, sizeof(otp_mobileno));
        memset(otp_message, 0, sizeof(otp_message));
        if(OTP_page == 1)
        {
            sprintf(otp_mobileno,"******%s",&configLoginLeg1.ucmobileNumber[6]);
        }
        else
        {
            sprintf(otp_mobileno,"******%s",&configLoginLeg2.ucmobileNumber[6]);
        }
        //     printf("otp_mobileno = %s",otp_mobileno);
        sprintf(otp_message, "One Time Password (OTP) has been\n sent to your registered mobile %s,\n Please enter the same here",otp_mobileno);
        //     printf("otp_message = %s",otp_message);
        ui->Otp_display_label->setText(otp_message);

        // ui->stackedWidget->setCurrentIndex(22);
        return FUNCTION_SUCCESS;
    }
    else
    {
        str = ResponseMessage.at(0);
        strToFind = ":";
        value2 = str.mid(str.indexOf(strToFind)+strToFind.length());
        memcpy(OTP.ucResponseMessage,value2.trimmed().toAscii().data(), strlen(value2.trimmed().toAscii().data()));
        if(strcmp((char *)OTP.ucResponseMessage,"Session Expired.")==0)
        {
            uidmessage::instance()->hide_Message();
            sprintf(ErrorMsg,"%s",/*(char *)LTS.ucResponseCode,*/(char *)OTP.ucResponseMessage);
            miscObj.DisplayMsgBox(ErrorMsg);
            uidmessage::instance()->hide_Message();
            //            ui->stackedWidget->setCurrentIndex(0);
            return SESSION_EXPIRED_FAILED;
        }
        sprintf(ErrorMsg,"%s",/*(char *)OTP.ucResponseCode,*/(char *)OTP.ucResponseMessage);
        miscObj.DisplayMsgBox(ErrorMsg);
        uidmessage::instance()->hide_Message();
        return FUNCTION_FAILED;
    }


    return FUNCTION_SUCCESS;
}


int FPB::makeJSONOTP(int ivalue)//----------------------
{
    char tempbuff[30];
    int iRet=-2,slen;
    memset(gSendData,0,sizeof(gSendData));

    iRet = iMakeRequest(ivalue);//changed by vaishnavi
    if (iRet!=FUNC_OK)
    {
        uidmessage::instance()->hide_Message();
        return FUNC_ERROR;
    }

    //consolprint(Recvdata);
    slen=strlen((char *)Recvdata);
    slen-=2;
    memset(gSendData,0,sizeof(gSendData));//AEPSProcessRequest

#ifdef UAT_HTTP
    if(ivalue == 11)
    {
        if(OTP_page==1)
        {
            strncpy((char *)gSendData,"POST /UIService.svc/UtilityOtpNS HTTP/1.1\r\n",45);
        }
        if(OTP_page==2)
        {
            strncpy((char *)gSendData,"POST /UIService.svc/UtilityOtp HTTP/1.1\r\n",43);
        }
    }
    else if(ivalue == 13 || ivalue == 21)
    {
        strncpy((char *)gSendData,"POST /UIService.svc/ProcessLoginRequest HTTP/1.1\r\n",52);
    }
    strncat((char *)gSendData,"Host: 202.58.125.20\r\n",27);//9013
    strncat((char *)gSendData,"Content-Type: application/json; charset=utf-8\r\n",49);
    sprintf(tempbuff,"Content-Length: %d\r\n",slen);
    strncat((char *)gSendData,tempbuff,strlen(tempbuff));
    strcat((char *)gSendData,(char *)Recvdata);
#else
    strncpy((char *)gSendData,(char *)Recvdata,strlen((char *)Recvdata));
#endif
#if DEBUG
    FILE *fp2 = NULL;
    if(ivalue == 11)
    {
        system("cd /usr/FINO/FPB/REQ/");
        system("rm /usr/FINO/FPB/REQ/OTP_REQ.TXT");
        fp2 =fopen("/usr/FINO/FPB/REQ/OTP_REQ.TXT","a");
    }
    else if(ivalue == 13)
    {
        system("cd /usr/FINO/FPB/REQ/");
        system("rm /usr/FINO/FPB/REQ/ENC_REQ.TXT");
        fp2 =fopen("/usr/FINO/FPB/REQ/ENC_REQ.TXT","a");
    }

    else if(ivalue == 21)
    {
        system("cd /usr/FINO/FPB/REQ/");
        system("rm /usr/FINO/FPB/REQ/CHANGE.TXT");
        fp2 =fopen("/usr/FINO/FPB/REQ/CHANGE.TXT","a");
    }

    fwrite(gSendData,strlen((char *)gSendData),1,fp2);
    //ttestall(0,100);
    sleep(1);
    fclose(fp2);
#endif
    return FUNC_OK;

}


int FPB::iSetOTP(int tagID, unsigned char *value)
{
    switch(tagID)
    {
    case  1:
        strcpy((char *)value,(char *)OTP.ucRequestID_req);
        break;
    case  2:
        strcpy((char *)value,(char *)OTP.ucMethodID_req);//Method id
        break;
    case  3:
        strcpy((char *)value,(char *)OTP.ucTellerID_req);//teller id
        break;
    case  4:
        strcpy((char *)value,(char *)OTP.ucIsEncrypt_req);//is encrypt
        break;
    case  5:
        strcpy((char *)value,(char *)OTP.ucChannelID_req);//is encrypt
        break;
    case  6:
        strcpy((char *)value,(char *)OTP.ucCustomerMobileNo_req);//is encrypt
        break;
    case  7:
        strcpy((char *)value,(char *)OTP.ucMessageId_req);//is encrypt
        break;
    case  8:
        strcpy((char *)value,(char *)OTP.ucRDFMethodID_req);//is encrypt
        break;
    case  9:
        strcpy((char *)value,(char *)OTP.ucSessionID_req);//is encrypt
        break;
    default:
        return FUNC_ERROR;
    }
    return FUNC_OK;
}


void FPB::on_Next_Otp_Button_clicked()
{
    CMisc miscObj;
    char Message[1000];
    memset(Message, 0, sizeof(Message));


    QString OTP_edit;

    OTP_edit = ui->OTP_Lineedit->text();
    if(OTP_edit.isEmpty())
    {
        miscObj.DisplayMsgBox((char *)"Please Enter OTP.  ");
        return;
    }

    if(OTP_edit.length()<4)
    {
        miscObj.DisplayMsgBox((char *)"OTP should be minimum 4 digits. ");
        return;
    }

    strcpy(CustDet.cOtp,ui->OTP_Lineedit->text().trimmed().toAscii().data());
    int iRet =-2;
    if(OTP_page==1)
    {
        iRet = Prepare_newpass_Func();
    }
    if(OTP_page ==2 || OTP_page ==6  )
    {
        iRet = Change_pass_Func();
    }
    if(OTP_page==3)
    {
        if(memcmp(CustDet.cAddBeneWithouTxn,"Y",1)==0)
        {
            iRet = Addbeneficiarywithouttxn(); //Addbeneficiary();
            uidmessage::instance()->hide_Message();
            flag_bitmap = '\0';
            qDebug()<<"iRet = "<<iRet;
        }
        else
        {
            char ErrorMsg[250];
            memset(ErrorMsg, 0, sizeof(ErrorMsg));

            iRet = PostIMPS();

            uidmessage::instance()->hide_Message();
            flag_bitmap = '\0';

        }
    }
    if(OTP_page == 4)
    {
        ui->Next_Otp_Button_2->setVisible(false);
        ui->FINISH_PushButton_Pg34->setVisible(true);
        iRet = PostBeneVerification();
        uidmessage::instance()->hide_Message();
    }

    if(OTP_page == 5)
    {
        if(Reversal_type == 0)
        {
            iRet = Reversal_Refund_Request();
        }
        else if(Reversal_type == 1)
        {
            iRet = Reversal_Refire_Request();
        }
    }
    if(OTP_page == 7)
    {

        iRet = Mini_Statement(0);
        qDebug()<<"iRet"<<iRet;
        uidmessage::instance()->hide_Message();

    }

    if(iRet != FUNCTION_SUCCESS && iRet != FUNC_OK)
    {
        Flag_Display_WLMsg = 0;
        if(iRet == FUNC_BACK)
        {
            uidmessage::instance()->hide_Message();
            ui->stackedWidget->setCurrentIndex(22);
            return;
        }
        if(OTP_page == 3)
        {
            uidmessage::instance()->hide_Message();
            ui->OTP_Lineedit->clear();
            if(iRet == SESSION_EXPIRED_FAILED)
            {
                ui->stackedWidget->setCurrentIndex(0);
                return;
            }

            // OTP Verification failed, Redirecting to FallBack ...  , Maximum OTP request attempt failed.Please try after some time.
            if((strcmp((char *)resp_postimps.Commondata_POBV_RES.ucResponseMessage_resp,"OTP expired")==0) || (strcmp((char *)add_bene_resp.ucRDResponseMessage_Resp,"OTP expired")==0)
                    || (strcmp((char *)resp_postimps.Commondata_POBV_RES.ucResponseMessage_resp,"OTP Verification failed")==0)
                    || (strcmp((char *)add_bene_resp.ucRDResponseMessage_Resp,"OTP Verification failed")==0) || (strcmp((char *)resp_postimps.Commondata_POBV_RES.ucResponseMessage_resp,"OTP Authentication Failed")==0)
                    || (strcmp((char *)add_bene_resp.ucRDResponseMessage_Resp,"OTP Authentication Failed")==0)) // OTP already Consumed
            {
                uidmessage::instance()->hide_Message();
                ui->OTP_Lineedit->clear();
                ui->OTP_Lineedit->setFocus();
                ui->stackedWidget->setCurrentIndex(22);
                return;
            }

            Flag_Display_WLMsg = 1;
            int ret_wcl = Walkin_Cust_Limit();

            if(ret_wcl == FUNCTION_SUCCESS)
            {
                int temp_cust_amt = atoi((const char*)walkinCustLimit.ucWCL_RDWCLmaxMonthlyLimit_resp) - atoi((const char*)walkinCustLimit.ucWCL_RDWCLcurrMonthAmount_resp);
                bool decimal;
                QString bal = QString::number(temp_cust_amt);
                decimal = bal.contains('.');
                if(decimal == false)
                {
                    bal = bal.append(".00");
                }

                ui->Ft_limit_disp->setText(bal);
                bal.clear();
            }
            Flag_Display_WLMsg = 0;
            clear_IMPS();
            ui->Next_Otp_Button_2->setVisible(true);
            ui->FINISH_PushButton_Pg34->setVisible(false);
            ui->stackedWidget->setCurrentIndex(15);
            return;
        }
        if(OTP_page == 4)
        {
            uidmessage::instance()->hide_Message();
            ui->OTP_Lineedit->clear();
            if(iRet == SESSION_EXPIRED_FAILED)
            {
                ui->stackedWidget->setCurrentIndex(0);
                return;
            }

            if(((strcmp((char *)resp_pBenever.Commondata_POBV_RES.ucResponseMessage_resp,"OTP expired"))==0) || (strcmp((char *)resp_pBenever.Commondata_POBV_RES.ucResponseMessage_resp,"OTP Verification failed")==0) || (strcmp((char *)resp_pBenever.Commondata_POBV_RES.ucResponseMessage_resp,"OTP Authentication Failed")==0)) // OTP already Consumed
            {
                uidmessage::instance()->hide_Message();
                ui->OTP_Lineedit->clear();
                ui->OTP_Lineedit->setFocus();
                ui->stackedWidget->setCurrentIndex(22);
                return;
            }


            //            if(strcmp((char *)resp_pBenever.Commondata_POBV_RES.ucResponseMessage_resp,"OTP expired")==0) // OTP already Consumed
            //            {
            //                ui->stackedWidget->setCurrentIndex(22);
            //                return;
            //            }
            ui->Next_Otp_Button_2->setVisible(true);
            ui->FINISH_PushButton_Pg34->setVisible(false);
            if (CUST_TYPE == 'W')
                ui->stackedWidget->setCurrentIndex(12);
            else
                ui->stackedWidget->setCurrentIndex(15);
            return;
        }
        if(OTP_page == 7)
        {
            uidmessage::instance()->hide_Message();
            if(iRet == OTP_FAILED)
            {
                ui->OTP_Lineedit->clear();
                ui->OTP_Lineedit->setFocus();
                ui->stackedWidget->setCurrentIndex(22);
                return;
            }
            else if(iRet == SESSION_EXPIRED_FAILED)
            {
                ui->stackedWidget->setCurrentIndex(0);
                return;
            }
            else
                ui->stackedWidget->setCurrentIndex(15);
            return;
        }
        if(OTP_page == 5)
        {
            uidmessage::instance()->hide_Message();
            if(iRet == OTP_FAILED)
            {
                ui->OTP_Lineedit->clear();
                ui->OTP_Lineedit->setFocus();
                ui->stackedWidget->setCurrentIndex(22);
                return;
            }
            else if(iRet == SESSION_EXPIRED_FAILED)
            {
                ui->stackedWidget->setCurrentIndex(0);
                return;
            }
            else
                ui->stackedWidget->setCurrentIndex(15);
            return;
        }
        ui->Old_password_edit_pg26->clear();
        ui->New_password_edit_pg26->clear();
        ui->Confirm_password_edit_pg26->clear();

        if(iRet == SESSION_EXPIRED_FAILED)
            ui->stackedWidget->setCurrentIndex(0);
        else
        {
            if(Flag_FirstTime_Login == 1)
            {
                if(iRet == OTP_FAILED)
                {
                    ui->OTP_Lineedit->clear();
                    ui->OTP_Lineedit->setFocus();
                    ui->stackedWidget->setCurrentIndex(22);
                    return;
                }
                else
                    ui->stackedWidget->setCurrentIndex(0);
                return;
            }
            else
            {
                Flag_FirstTime_Login=0;
                if(OTP_page ==1)
                {
                    if(iRet == OTP_FAILED)
                    {
                        ui->OTP_Lineedit->clear();
                        ui->OTP_Lineedit->setFocus();
                        ui->stackedWidget->setCurrentIndex(22);
                        return;
                    }
                    else
                        ui->stackedWidget->setCurrentIndex(0);
                    return;
                }
                if(OTP_page ==2 || OTP_page ==6  )
                {
                    if(iRet == OTP_FAILED)
                    {
                        ui->OTP_Lineedit->clear();
                        ui->OTP_Lineedit->setFocus();
                        ui->stackedWidget->setCurrentIndex(22);
                        return;
                    }
                    else
                    {
                        OTP_page = -1;
                        ui->stackedWidget->setCurrentIndex(25);
                        return;
                    }
                }
            }
        }
        return;
    }
    else if(iRet == FUNCTION_SUCCESS || iRet == FUNC_OK)
    {
        Flag_Display_WLMsg = 0;
        if(OTP_page ==1)
        {
            OTP_page = -1;
            ui->stackedWidget->setCurrentIndex(0);
        }
        if(OTP_page ==2 || OTP_page ==6  )
        {
            OTP_page = -1;
            if(Flag_FirstTime_Login == 1)
            {

                //***************************************  eAgreement  *******************************************

                //                if(Flag_eAgreement_Login == 1)
                //                {
                //                    qDebug()<<"eAgreement ui->stackedWidget->setCurrentIndex(27)";
                //                    uidmessage::instance()->hide_Message();
                //                    ui->Eagreement_Merchant_Name_pg27->setText((char *)configLoginLeg2.ucname);
                //                    ui->stackedWidget->setCurrentIndex(27);
                //                    return;
                //                }

                //************************************************************************************************
                int i_ldf = login_download_func();
                if(i_ldf == FUNCTION_SUCCESS)
                {

                    int ilRet = LoginCardAcceptorLocation();

                    if(ilRet != FUNCTION_SUCCESS)
                        ui->stackedWidget->setCurrentIndex(0);
                    else
                        ui->stackedWidget->setCurrentIndex(12);
                    return;
                }
                else
                {
                    qDebug()<<"Chnage Passwd ldf failed";
                    ui->stackedWidget->setCurrentIndex(0);
                    return;
                }


            }

            ui->stackedWidget->setCurrentIndex(12);
        }
        if(OTP_page == 3)
        {
            ui->Next_Otp_Button_2->setVisible(false);
            ui->FINISH_PushButton_Pg34->setVisible(true);

            ui->Print_Receipt_Button_Pg34->setVisible(true); // 02052019
            ui->Discard_otp_Button_2->hide(); // 02052019

            //            uidmessage::instance()->hide_Message();
            if(memcmp(CustDet.cAddBeneWithouTxn,"Y",1)==0)
            {

                uidmessage::instance()->hide_Message();
                //-------------------------------------------------------------------------------

                char Message1[10][100];
                memset(Message1,0,sizeof(Message1));
                int l =0;
                sprintf(Message1[l],"          ");
                sprintf(Message1[l+1],"Account Number : %s", (char *)ui->Bene_acc_no_edit->text().trimmed().toAscii().data() );
                sprintf(Message1[l+2],"Beneficiary Name : %s", (char *) ui->Bene_Name_edit->text().trimmed().toAscii().data());
                sprintf(Message1[l+3],"Bank Name : %s",  (char *) ui->cmb_bankList->currentText().trimmed().toAscii().data());
                sprintf(Message1[l+4],"          ");
                sprintf(Message1[l+5],"%s", "Beneficiary added successful");
                sprintf(Message1[l+6],"          ");

                ui->listWidget_2->clear();
                for(int k=0;k<7;k++)
                    ui->listWidget_2->addItem((char *)Message1[k]);



                memcpy((char *)ConfigPrint.ucCustName,  (const char *)SearchWalkCust.SWCL_name_resp, strlen((const char *)SearchWalkCust.SWCL_name_resp));
                memcpy((char *)ConfigPrint.ucCustMobNo, (const char *)FC_MobileNumber, strlen((const char *)FC_MobileNumber));
                memcpy((char *)ConfigPrint.ucBeneAccNo, (const char *)ui->Bene_acc_no_edit->text().trimmed().toAscii().data(), strlen((const char *)ui->Bene_acc_no_edit->text().trimmed().toAscii().data()));
                memcpy((char *)ConfigPrint.ucBeneName, (const char *)ui->Bene_Name_edit->text().trimmed().toAscii().data(),strlen((const char *)ui->Bene_Name_edit->text().trimmed().toAscii().data()));
                memcpy((char *)ConfigPrint.ucTotalAmt, "0.00", 4);



            }
            else
            {
                Flag_Display_WLMsg = 1;
                int ret_wcl = Walkin_Cust_Limit();

                if(ret_wcl == FUNCTION_SUCCESS)
                {
                    int temp_cust_amt = atoi((const char*)walkinCustLimit.ucWCL_RDWCLmaxMonthlyLimit_resp) - atoi((const char*)walkinCustLimit.ucWCL_RDWCLcurrMonthAmount_resp);
                    bool decimal;
                    QString bal = QString::number(temp_cust_amt);
                    decimal = bal.contains('.');
                    if(decimal == false)
                    {
                        bal = bal.append(".00");
                    }

                    ui->Ft_limit_disp->setText(bal);
                    bal.clear();
                }
                uidmessage::instance()->hide_Message();

                //--------------------------------------    // Prod_04092019     ----------------------------------

                               char Message1[15][100];
                               memset(Message1,0,sizeof(Message1));
                               int l =0;
                               sprintf(Message1[l++],"          ");
                               sprintf(Message1[l++],"Account Number      : %s", (char *)ui->Bene_acc_no_edit->text().trimmed().toAscii().data() );
                               sprintf(Message1[l++],"Beneficiary Name    : %s", (char *) ui->Bene_Name_edit->text().trimmed().toAscii().data());
                               sprintf(Message1[l++],"Transaction Amount  : %s",  (char *)resp_postimps.ucRDTransactonAmount_RES);

                               if(display_imps.Flag_SGST_CGST == 1) // Prod_04092019
                               {
                                   sprintf(Message1[l++],"Charges             : %s",  (char *)display_imps.ucDispMinusCharges); // Prod_04092019 // ucDispCharges
                                   if(strcmp((char *)display_imps.ucDispMinusCharges, "0.00")==0) // Prod_04092019
                                   {
                                       sprintf(Message1[l++],"Charges (CGST)      : %s",  (char *)"0.00"); // 16072019
                                       sprintf(Message1[l++],"Charges (SGST)      : %s",  (char *)"0.00"); // 16072019
                                   }
                                   else
                                   {
                                       sprintf(Message1[l++],"Charges (CGST)      : %s",  (char *)display_imps.ucDispCharges_CGST); // 16072019
                                       sprintf(Message1[l++],"Charges (SGST)      : %s",  (char *)display_imps.ucDispCharges_SGST); // 16072019
                                   }
                               }
                               else // Prod_04092019
                               {
                                   sprintf(Message1[l++],"Charges             : %s",  (char *)display_imps.ucDispCharges); // Prod_04092019 // ucDispCharges
                               }
                               sprintf(Message1[l++],"Bank Name           : %s",  (char *) ui->cmb_bankList->currentText().trimmed().toAscii().data());
                               sprintf(Message1[l++],"Transaction Type    : %s", (char *)resp_postimps.ucRDTransactionType_RES);
                               sprintf(Message1[l++],"Transaction Ref No. : %s", (char *)resp_postimps.ucRDTransactionRefNo_RES);
                               sprintf(Message1[l++],"          ");
                               sprintf(Message1[l++],"%s", "Fund Transfer Successful");
                               sprintf(Message1[l++],"          ");

                               ui->listWidget_2->clear();
                               for(int k=0;k<l;k++) // Prod_04092019
                                   ui->listWidget_2->addItem((char *)Message1[k]);

               //-----------------------------------------------------------------------------------------------------------


                ui->Bene_Name_edit->clear();
                ui->Bene_Name_edit->setText(resp_pBenever.Commondata_POBV_RES.ucIMPSSLR_BeneName_resp);

                /////////////////////////////////////////////////////////////// 02052019 ////////////////////////////////////////////////////////////

                memcpy((char *)ConfigPrint.ucCustName,  (const char *)SearchWalkCust.SWCL_name_resp, strlen((const char *)SearchWalkCust.SWCL_name_resp));
                memcpy((char *)ConfigPrint.ucCustMobNo, (const char *)FC_MobileNumber, strlen((const char *)FC_MobileNumber));
                memcpy((char *)ConfigPrint.ucBeneAccNo, (const char *)resp_postimps.ucRDBeneAccNo_RES, strlen((const char *)resp_postimps.ucRDBeneAccNo_RES));
                memcpy((char *)ConfigPrint.ucBeneName, (const char *)resp_postimps.ucRDbenename_RES,strlen((const char *)resp_postimps.ucRDbenename_RES));
                memcpy((char *)ConfigPrint.ucAmount, (const char *)resp_postimps.ucRDTransactonAmount_RES,strlen((const char *)resp_postimps.ucRDTransactonAmount_RES));
                memcpy((char *)ConfigPrint.ucCharges, (const char *)resp_postimps.ucRDCharges_RES, strlen((const char *)resp_postimps.ucRDCharges_RES));
                memcpy((char *)ConfigPrint.uctxntype, (const char *)resp_postimps.ucRDTransactionType_RES,strlen((const char *)resp_postimps.ucRDTransactionType_RES));
                memcpy((char *)ConfigPrint.ucTotalAmt, (const char *)resp_postimps.ucTOTALAMOUNT_IMPS, strlen((const char *)resp_postimps.ucTOTALAMOUNT_IMPS));
                memcpy((char *)ConfigPrint.ucRefNo, (const char *)resp_postimps.ucRDTransactionRefNo_RES,strlen((const char *)resp_postimps.ucRDTransactionRefNo_RES));

                /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

                //                ui->Print_Receipt_Button_Pg34->hide(); // 02052019
                //                ui->Discard_otp_Button_2->setVisible(true); // 02052019

            }
            clear_IMPS();
            Flag_Display_WLMsg = 0;
            ui->stackedWidget->setCurrentIndex(34);
            return;

            //            ui->stackedWidget->setCurrentIndex(15);
            //            return;
        }
        if(OTP_page == 4)
        {
            ui->Next_Otp_Button_2->setVisible(false);
            ui->FINISH_PushButton_Pg34->setVisible(true);
            uidmessage::instance()->hide_Message();

            Flag_Display_WLMsg = 1;

            int ret_wcl = Walkin_Cust_Limit();

            if(ret_wcl == FUNCTION_SUCCESS)
            {
                int temp_cust_amt = atoi((const char*)walkinCustLimit.ucWCL_RDWCLmaxMonthlyLimit_resp) - atoi((const char*)walkinCustLimit.ucWCL_RDWCLcurrMonthAmount_resp);
                bool decimal;
                QString bal = QString::number(temp_cust_amt);
                decimal = bal.contains('.');
                if(decimal == false)
                {
                    bal = bal.append(".00");
                }

                ui->Ft_limit_disp->setText(bal);
                bal.clear();
            }


            QString bal = QString::fromUtf8((const char*)resp_pBenever.Commondata_POBV_RES.ucBeneVerif_AvilableBalance);
            bool decimal;
            decimal = bal.contains('.');
            if(decimal == false)
            {
                bal = bal.append(".00");
            }
            memset(Balance_Display,0,strlen(Balance_Display));
            sprintf(Balance_Display,"Balance : %s\n",bal.toLatin1().data());
            strncpy(Mer_Latest_Balance, bal.toLatin1().data(),strlen(bal.toLatin1().data()));
            ui->Dash_Latest_Balance_Label_pg12->setText(Balance_Display);

            uidmessage::instance()->hide_Message();
            //-------------------------------------------------------------------------------

            char Message1[10][100];
            memset(Message1,0,sizeof(Message1));
            int l =0;
            sprintf(Message1[l],"          ");
            sprintf(Message1[l+1],"Bene Name     : %s", (char *)resp_pBenever.Commondata_POBV_RES.ucIMPSSLR_BeneName_resp);
            sprintf(Message1[l+2],"Bene Acc. No. : %s", (char *)resp_pBenever.Commondata_POBV_RES.ucIMPSSLR_BeneAccNo_resp);
            sprintf(Message1[l+3],"Bene IFSC     : %s",  (char *)resp_pBenever.Commondata_POBV_RES.ucIMPSSLR_BeneIFSC_resp);
            sprintf(Message1[l+4],"Bank RRN      : %s",  (char *)resp_pBenever.Commondata_POBV_RES.ucIMPSSLR_BankRRN_resp);
            sprintf(Message1[l+5],"          ");
            sprintf(Message1[l+6],"%s", " Bene Verification Successful");
            sprintf(Message1[l+7],"          ");

            ui->listWidget_2->clear();
            for(int k=0;k<8;k++)
                ui->listWidget_2->addItem((char *)Message1[k]);



            memset(CustDet.cDefault_IFSC_Code, 0, sizeof(CustDet.cDefault_IFSC_Code));
            memcpy(CustDet.cDefault_IFSC_Code, (char *)resp_pBenever.Commondata_POBV_RES.ucIMPSSLR_BeneIFSC_resp, strlen((const char *)resp_pBenever.Commondata_POBV_RES.ucIMPSSLR_BeneIFSC_resp));

            ui->Bene_Name_edit->clear();
            ui->IFSC_edit->clear();
            ui->Bene_Name_edit->setText(resp_pBenever.Commondata_POBV_RES.ucIMPSSLR_BeneName_resp);
            ui->IFSC_edit->setText(resp_pBenever.Commondata_POBV_RES.ucIMPSSLR_BeneIFSC_resp);
            ui->Bene_acc_no_edit->setDisabled(true);
            ui->cmb_bankList->setDisabled(true);
            flag_bene_verfication_status = 1;
            Flag_Display_WLMsg = 0;
            ui->stackedWidget->setCurrentIndex(34);
            return;
        }
        if(OTP_page == 5)
        {
            uidmessage::instance()->hide_Message();
            ui->stackedWidget->setCurrentIndex(43);
            return;
        }
        if(OTP_page == 7)
        {
            uidmessage::instance()->hide_Message();
            ui->stackedWidget->setCurrentIndex(32);
            return;
        }
        return;
    }
    return;
}

int FPB::Prepare_newpass_Func()
{
    uidmessage::instance()->hide_Message();
    uidmessage::instance()->Display_BusyMessage("Please Wait...");
    //    printf("Mobile Number456 %s",configLoginLeg2.ucmobileNumber);
    int encoutlen1=0;
    CMisc miscObj;
    QString OTP_edit;
    int ret = -3;
    char ErrorMsg[100];
    unsigned char out_enc[100];
    int out_len = 0;
    unsigned char FINALPASSWORD[100];
    unsigned char FINALUSERID[100];

    OTP_edit = ui->OTP_Lineedit->text();
    if(OTP_edit.isEmpty())
    {
        miscObj.DisplayMsgBox((char *)"Please Enter OTP.  ");
        return FUNC_BACK;
    }
    if(OTP_edit.length()<4)
    {
        miscObj.DisplayMsgBox((char *)"OTP should be minimum 4 digits. ");
        return FUNC_BACK;
    }

    strcpy((char*)OTP.ucnewpassword,ui->New_password_edit->text().trimmed().toAscii().data());
    int k = uid_aes_encrypt((unsigned char*)OTP.ucnewpassword,strlen((char*)OTP.ucnewpassword), out_enc, &out_len, (unsigned char *)__DEFAULT_KEY, (unsigned char *)__DEFAULT_IV);
    memset(FINALPASSWORD,0,sizeof(FINALPASSWORD));
    uid_encode_b64((unsigned char *)out_enc, out_len, FINALPASSWORD, &encoutlen1);//1bbWOYy0YUtik6tg2BQDsA==
    QString ENC_Data ;
    ENC_Data.clear();
    ENC_Data = QString::fromUtf8((const char*)FINALPASSWORD).trimmed();
    memset(FINALPASSWORD,0,sizeof(FINALPASSWORD));
    memcpy(FINALPASSWORD,ENC_Data.toLatin1().data(),strlen(ENC_Data.toLatin1().data()));

    memset(out_enc,0,sizeof(out_enc));
    encoutlen1=0;

    k = uid_aes_encrypt((unsigned char*)username,strlen((char*)username), out_enc, &out_len, (unsigned char *)__DEFAULT_KEY, (unsigned char *)__DEFAULT_IV);
    memset(FINALUSERID,0,sizeof(FINALUSERID));
    uid_encode_b64((unsigned char *)out_enc, out_len, FINALUSERID, &encoutlen1);
    ENC_Data.clear();
    ENC_Data = QString::fromUtf8((const char*)FINALUSERID).trimmed();
    memset(FINALUSERID,0,sizeof(FINALUSERID));
    memcpy(FINALUSERID,ENC_Data.toLatin1().data(),strlen(ENC_Data.toLatin1().data()));


    QString Display = ui->OTP_Lineedit->text();

    iGenerateRequestID((char *)username,(unsigned char *)&sndnewpass.ucRequestId_NwPassreq);
    memset(ErrorMsg,0,sizeof(ErrorMsg));
    memset(sndnewpass.ucTellerID_NwPassreq,0, sizeof(sndnewpass.ucTellerID_NwPassreq));
    memset(sndnewpass.ucMethodId_NwPassreq,0, sizeof(sndnewpass.ucMethodId_NwPassreq));
    memset(sndnewpass.ucChannelID_NwPassreq,0, sizeof(sndnewpass.ucChannelID_NwPassreq));
    memset(sndnewpass.ucQstgroup_NwPassreq,0, sizeof(sndnewpass.ucQstgroup_NwPassreq));
    memset(sndnewpass.ucIsEncrypt_NwPassreq,0, sizeof(sndnewpass.ucIsEncrypt_NwPassreq));
    memset(sndnewpass.ucQstID_NwPassreq,0, sizeof(sndnewpass.ucQstID_NwPassreq));
    memset(sndnewpass.ucAnswer_NwPassreq,0, sizeof(sndnewpass.ucAnswer_NwPassreq));
    memset(sndnewpass.ucMobileNo_NwPassreq,0, sizeof(sndnewpass.ucMobileNo_NwPassreq));
    memset(sndnewpass.ucAuth_id_NwPassreq,0, sizeof(sndnewpass.ucAuth_id_NwPassreq));
    memset(sndnewpass.ucMethodId_NwPassreq,0, sizeof(sndnewpass.ucMethodId_NwPassreq));
    memset(sndnewpass.ucMobileNo_NwPassreq,0, sizeof(sndnewpass.ucMobileNo_NwPassreq));
    memset(sndnewpass.ucOTPPIN_NwPassreq,0, sizeof(sndnewpass.ucOTPPIN_NwPassreq));
    memset(sndnewpass.ucMessageID_NwPassreq,0, sizeof(sndnewpass.ucMessageID_NwPassreq));
    memset(sndnewpass.ucNewPass_NwPassreq,0, sizeof(sndnewpass.ucNewPass_NwPassreq));
    memset(sndnewpass.ucOTPRequestId_NwPassreq,0, sizeof(sndnewpass.ucOTPRequestId_NwPassreq));


    iGenerateRequestID((char *)username,(unsigned char *)&sndnewpass.ucRequestId_NwPassreq);
    memcpy(sndnewpass.ucTellerID_NwPassreq,username,strlen(username));
    memcpy(sndnewpass.ucIsEncrypt_NwPassreq,"false",5);
    memcpy(sndnewpass.ucMethodId_NwPassreq,"13",2);
    memcpy(sndnewpass.ucUserid_NwPassreq,FINALUSERID, strlen((char *)FINALUSERID));
    memcpy(sndnewpass.ucQstID_NwPassreq,OTP.ucQstID, strlen((char *)OTP.ucQstID));
    memcpy(sndnewpass.ucQstgroup_NwPassreq,OTP.ucQstgroup, strlen((char *)OTP.ucQstgroup));
    secretanswer_encryption((char *)OTP.ucencrypkey,(char *)OTP.ucanswer,(char *)sndnewpass.ucAnswer_NwPassreq);



    if(OTP_page == 1)
    {
        memcpy(sndnewpass.ucMobileNo_NwPassreq,configLoginLeg1.ucmobileNumber, strlen((char *)configLoginLeg1.ucmobileNumber));


    }
    else
    {
        memcpy(sndnewpass.ucMobileNo_NwPassreq,configLoginLeg2.ucmobileNumber, strlen((char *)configLoginLeg2.ucmobileNumber));
    }
    memcpy(sndnewpass.ucAuth_id_NwPassreq,"1", 1);
    memcpy(sndnewpass.ucRDMethodId_NwPassreq,"2", 1);

    memcpy(sndnewpass.ucOTPPIN_NwPassreq,ui->OTP_Lineedit->text().trimmed().toAscii().data(), strlen((char *)ui->OTP_Lineedit->text().trimmed().toAscii().data()));

    memcpy(sndnewpass.ucMessageID_NwPassreq,"42",2);

    sprintf((char *)sndnewpass.ucNewPass_NwPassreq,"%s|%s",OTP.ucencrypkey,FINALPASSWORD);//PtRBW+X4voniatuyPRYnUg== -> Nov@2018
    memset(sndnewpass.ucOTPRequestId_NwPassreq,0, sizeof(sndnewpass.ucOTPRequestId_NwPassreq));
    memcpy(sndnewpass.ucOTPRequestId_NwPassreq,OTP.ucRDRequestId, strlen((char *)OTP.ucRDRequestId));

    ret	= makeJSONFINALOTP();
    if(ret!=FUNC_OK)
    {
        qDebug()<<"JSON Creation failed";
        return FUNC_ERROR;
    }

#ifdef UAT_HTTP     // File Download

    uidmessage::instance()->hide_Message();
    hComm.close_connection();
    res = hComm.open_connect(SERVERIP,SERVERPORT);
    if(res != 0)
    {
        qDebug() << "Failed to connect to the Server";
        uidmessage::instance()->hide_Message();
        uidmessage::instance()->DisplayMessage("Server Not Connected");
        qDebug()<<"FUNCTION_FAILED";
        ui->stackedWidget->setCurrentIndex(0);
        return FUNCTION_FAILED ;
    }


    int len_gSendData = strlen((char *)gSendData);
    memset(OTPVAL_Buff,0,sizeof(OTPVAL_Buff));
    memset(oBuff,0,sizeof(oBuff));
    if((hComm.host_2Sendrecv((unsigned char *)gSendData,len_gSendData,( unsigned char*)oBuff,(int *)&OTPVAL_len,(int)response_timeout1,0))!=SUCCESS)
    {
        printf("No Response From Server\n");
        hComm.close_connection();
        miscObj.DisplayMsgBox((char *)"Response Time-Out");
        uidmessage::instance()->hide_Message();
        qDebug()<<"***   Response Time-Out   ***";
        ui->stackedWidget->setCurrentIndex(1);
    }
    hComm.close_connection();

#else     // File Download full condition

#if DEBUG
    printf("gSendData = %s\n\n",gSendData);
#endif

    memset(str_url,0,sizeof(str_url));
#ifdef PRODUCTION
    strcpy(str_url,URL_CREATION(CHANGEPASSWD_OTP_URL_PROD));
#else
    strcpy(str_url,URL_CREATION((char *)CHANGEPASSWD_OTP_URL));
#endif

    int retk = file_download(str_url, (char *)gSendData,"/usr/FINO/FPB/RESP/OUTPUT.TXT",120);
    if(retk!=0)
    {
        qDebug()<<"retk = "<<retk;
        sleep(1);
        uidmessage::instance()->hide_Message();
        return FUNCTION_FAILED;
    }

#endif

    system("cd /usr/FINO/FPB/RESP/");
    system("rm /usr/FINO/FPB/RESP/OUTPUT.TXT");
    QString OTPVALBuff = QString::fromLocal8Bit(oBuff);
    //   QString ENCBuff = "{RequestId":null,"DisplayMessage":null,"ResponseMessage":"Contract Error","ResponseCode":"1","MessageType":"Exclam","ResponseData":null,"MessageId":null,"SessionExpiryTime":null,"SessionId":null,"RouteID":"YUIService To ProcessLoginRequest","ErrorCode":0,"XMLData":null,"AuthmanFlag":false,"ServiceID":0}"
    bool check = OTPVALBuff.contains("ResponseCode");
    if(check == false)
    {
        miscObj.DisplayMsgBox((char *)"Invalid Data");
        uidmessage::instance()->hide_Message();
        return FUNCTION_FAILED;
    }


    OTPVALBuff.remove("\\");
    OTPVALBuff.remove("{");
    OTPVALBuff.remove("}");
    OTPVALBuff.remove('"');
#if DEBUG
    qDebug()<<"OTPBuff"<<OTPVALBuff;
#endif
    QStringList query = OTPVALBuff.split(",");
#if DEBUG
    for(int i=0 ; i < query.length() ; i++)
        qDebug()<<"Query Response"<<query.at(i);
#endif
    QStringList ResponseCode = query.filter("ResponseCode");
    QStringList ResponseMessage = query.filter("ResponseMessage");
    uidmessage::instance()->hide_Message();
    QString strToFind,str;
    //   qDebug()<<"ResponseCode.at(0)"<<ResponseCode.at(0);
    //   qDebug()<<"ResponseMessage.at(0)"<<ResponseMessage.at(0);

    str = ResponseCode.at(0);
    strToFind = ":";
    QString value1 = str.mid(str.indexOf(strToFind)+strToFind.length());

    str = ResponseMessage.at(0);
    QString value2 = str.mid(str.indexOf(strToFind)+strToFind.length());

    //    qDebug()<<"value1"<<value1;
    //   qDebug()<<"value2"<<value2;


    if(value1 =="0")
    {
        sprintf(ErrorMsg,"%s",(char *)value2.toAscii().data());
        miscObj.DisplayMsgBox(ErrorMsg);
        uidmessage::instance()->hide_Message();
        return FUNCTION_SUCCESS;
    }
    else
    {
        if((strcmp((char *)value2.toAscii().data(),"OTP expired")==0) || (strcmp((char *)value2.toAscii().data(),"OTP Verification failed")==0) || (strcmp((char *)value2.toAscii().data(),"OTP Authentication Failed")==0)) // OTP already Consumed
        {
            uidmessage::instance()->hide_Message();
            memset(ErrorMsg, 0, sizeof(ErrorMsg));
            sprintf(ErrorMsg,"%s",(char *)value2.toAscii().data());
            miscObj.DisplayMsgBox(ErrorMsg);
            uidmessage::instance()->hide_Message();
            ui->OTP_Lineedit->clear();
            ui->OTP_Lineedit->setFocus();
            return OTP_FAILED;
        }

        sprintf(ErrorMsg,"%s",(char *)value2.toAscii().data());
        miscObj.DisplayMsgBox(ErrorMsg);
        uidmessage::instance()->hide_Message();
        return FUNCTION_FAILED;
    }

}


int FPB::iSetNEWPASS(int tagID, unsigned char *value)
{

    switch(tagID)
    {
    case  1:
        strcpy((char *)value,(char *)sndnewpass.ucRequestId_NwPassreq);
        break;
    case  2:
        strcpy((char *)value,(char *)sndnewpass.ucTellerID_NwPassreq);
        break;
    case  3:
        strcpy((char *)value,(char *)sndnewpass.ucIsEncrypt_NwPassreq);
        break;
    case  4:
        strcpy((char *)value,(char *)sndnewpass.ucMethodId_NwPassreq);
        break;
    case  5:
        strcpy((char *)value,(char *)sndnewpass.ucUserid_NwPassreq);
        break;
    case  6:
        strcpy((char *)value,(char *)sndnewpass.ucQstgroup_NwPassreq);
        break;
    case  7:
        strcpy((char *)value,(char *)sndnewpass.ucQstID_NwPassreq);
        break;
    case  8:
        strcpy((char *)value,(char *)sndnewpass.ucAnswer_NwPassreq);
        break;
    case  9:
        strcpy((char *)value,(char *)sndnewpass.ucMobileNo_NwPassreq);
        break;
    case  10:
        strcpy((char *)value,(char *)sndnewpass.ucAuth_id_NwPassreq);
        break;
    case  11:
        strcpy((char *)value,(char *)sndnewpass.ucRDMethodId_NwPassreq);
        break;
    case  12:

        strcpy((char *) value, (char *) sndnewpass.ucOTPRequestId_NwPassreq);
        break;
    case  13:
        strcpy((char *)value,(char *)sndnewpass.ucOTPPIN_NwPassreq);
        break;
    case  14:
        strcpy((char *)value,(char *)sndnewpass.ucMessageID_NwPassreq);
        break;
    case  15:
        strcpy((char *)value,(char *)sndnewpass.ucNewPass_NwPassreq);
        break;
    default:
        return FUNC_ERROR;
    }
    return FUNC_OK;
}


int FPB::makeJSONFINALOTP()//----------------------
{
    char tempbuff[30];
    int iRet=-2,slen;

    memset(gSendData,0,sizeof(gSendData));

    iRet = iMakeRequest(12);//changed by vaishnavi
    if (iRet!=FUNC_OK)
    {
        uidmessage::instance()->hide_Message();
        return FUNC_ERROR;
    }

    //consolprint(Recvdata);
    slen=strlen((char *)Recvdata);
    slen-=2;
    memset(gSendData,0,sizeof(gSendData));

#ifdef UAT_HTTP     // File Download

    strncpy((char *)gSendData,"POST /UIService.svc/ProcessLoginRequest HTTP/1.1\r\n",52);
    strncat((char *)gSendData,"Host: 202.58.125.2:9013\r\n",27);
    strncat((char *)gSendData,"Content-Type: application/json; charset=utf-8\r\n",49);
    sprintf(tempbuff,"Content-Length: %d\r\n",slen);
    strncat((char *)gSendData,tempbuff,strlen(tempbuff));
    strcat((char *)gSendData,(char *)Recvdata);
#else
    strncpy((char *)gSendData,(char *)Recvdata,strlen((char *)Recvdata));
#endif
#if DEBUG
    system("cd /usr/FINO/FPB/REQ/");
    system("rm /usr/FINO/FPB/REQ/FINALOTP_REQ.TXT");
    FILE *fp2 = NULL;
    fp2 =fopen("/usr/FINO/FPB/REQ/FINALOTP_REQ.TXT","a");
    fwrite(gSendData,strlen((char *)gSendData),1,fp2);
    //ttestall(0,100);
    sleep(1);
    fclose(fp2);
#endif
    return FUNC_OK;

}

int FPB::Change_pass_Func()
{
    // printf("*******************NEW PASSWORD 1010 OTP.ucnewpassword == %s\n",OTP.ucnewpassword);
    uidmessage::instance()->hide_Message();
    uidmessage::instance()->Display_BusyMessage("Please Wait...");
    HostComm hComm;
    int encoutlen1=0;
    CMisc miscObj;
    int CHANPASS_len = 0, response_timeout1 = 120, ret = -3, res = -3, iRet = -3;

    char ErrorMsg[100];
    unsigned char out_enc[100],out_enc1[100];
    int out_len = 0,out_len1 =0;
    unsigned char NEWPASSWORD[100];
    unsigned char OLDPASSWORD[100];
    QString OTP_edit;

    OTP_edit = ui->OTP_Lineedit->text();
    if(OTP_edit.isEmpty())
    {
        miscObj.DisplayMsgBox((char *)"Please Enter OTP.  ");
        return FUNC_BACK;
    }
    if(OTP_edit.length()<4)
    {
        miscObj.DisplayMsgBox((char *)"OTP should be minimum 4 digits. ");
        return FUNC_BACK;
    }

    Check_Conditions();

    //    qDebug()<<"*******************NEW PASSWORD 9 = "<<ui->New_password_edit_pg26->text().trimmed().toAscii().data();
    //   printf("*******************NEW PASSWORD 91 OTP.ucnewpassword == %s\n",OTP.ucnewpassword);
    int k = uid_aes_encrypt((unsigned char*)OTP.ucnewpassword,strlen((char*)OTP.ucnewpassword), out_enc, &out_len, (unsigned char *)__DEFAULT_KEY, (unsigned char *)__DEFAULT_IV);
    //    qDebug()<<"*******************NEW PASSWORD 10 == "<<ui->New_password_edit_pg26->text().trimmed().toAscii().data();
    //    qDebug()<<"*******************NEW PASSWORD 101 OTP.ucnewpassword == "<<OTP.ucnewpassword;
    memset(NEWPASSWORD,0,sizeof(NEWPASSWORD));
    uid_encode_b64((unsigned char *)out_enc, out_len, NEWPASSWORD, &encoutlen1);//1bbWOYy0YUtik6tg2BQDsA==
    QString ENC_Data ;
    ENC_Data.clear();
    //    printf("NEWPASSWORD == %s\n",NEWPASSWORD);
    ENC_Data = QString::fromUtf8((const char*)NEWPASSWORD).trimmed();
    memset(NEWPASSWORD,0,sizeof(NEWPASSWORD));
    memcpy(NEWPASSWORD,ENC_Data.toLatin1().data(),strlen(ENC_Data.toLatin1().data()));
#if DEBUG
    printf("OTP.ucoldpasswordj=%s\n",OTP.ucoldpassword);

    printf("NEWPASSWORD after encode =%s\n",NEWPASSWORD);

    printf("OTP.ucoldpasswordj=%s\n",OTP.ucoldpassword);
#endif
    k = uid_aes_encrypt((unsigned char*)OTP.ucoldpassword,strlen((char*)OTP.ucoldpassword), out_enc1, &out_len1, (unsigned char *)__DEFAULT_KEY, (unsigned char *)__DEFAULT_IV);
    memset(OLDPASSWORD,0,sizeof(OLDPASSWORD));
    //  printf("OLDPASSWORD == %s\n",OLDPASSWORD);
    uid_encode_b64((unsigned char *)out_enc1, out_len1, OLDPASSWORD, &encoutlen1);//1bbWOYy0YUtik6tg2BQDsA==
    ENC_Data.clear();
    //    printf("OLDPASSWORD 1== %s\n",OLDPASSWORD);
    ENC_Data = QString::fromUtf8((const char*)OLDPASSWORD).trimmed();
    memset(OLDPASSWORD,0,sizeof(OLDPASSWORD));
    memcpy(OLDPASSWORD,ENC_Data.toLatin1().data(),strlen(ENC_Data.toLatin1().data()));

    //    printf("OLDPASSWORDafter encode == %s\n",OLDPASSWORD);

    iGenerateRequestID((char *)username,(unsigned char *)&sndchange.ucRequestId_CPreq);
    memset(ErrorMsg,0,sizeof(ErrorMsg));
    memset(sndchange.ucTellerID_CPreq,0, sizeof(sndchange.ucTellerID_CPreq));
    memset(sndchange.ucTokenId_CPreq,0, sizeof(sndchange.ucTokenId_CPreq));
    memset(sndchange.ucSessionId_CPreq,0, sizeof(sndchange.ucSessionId_CPreq));
    memset(sndchange.ucIsEncrypt_CPreq,0, sizeof(sndchange.ucIsEncrypt_CPreq));
    memset(sndchange.uccustomerNo_CPreq,0, sizeof(sndchange.uccustomerNo_CPreq));
    memset(sndchange.ucauthid_CPreq,0, sizeof(sndchange.ucauthid_CPreq));
    memset(sndchange.ucRDMethodID_CPreq,0, sizeof(sndchange.ucRDMethodID_CPreq));
    memset(sndchange.ucRDRequestID_CPreq,0, sizeof(sndchange.ucRDRequestID_CPreq));
    memset(sndchange.ucRDOTPPIN_CPreq,0, sizeof(sndchange.ucRDOTPPIN_CPreq));
    memset(sndchange.ucMessageID_CPreq,0, sizeof(sndchange.ucMessageID_CPreq));
    memset(sndchange.ucSessionExpiryTime_CPreq,0, sizeof(sndchange.ucSessionExpiryTime_CPreq));
    memset(sndchange.ucUserID_CPreq,0, sizeof(sndchange.ucUserID_CPreq));
    memset(sndchange.ucOldPassword_CPreq,0, sizeof(sndchange.ucOldPassword_CPreq));
    memset(sndchange.ucNewPassword_CPreq,0, sizeof(sndchange.ucNewPassword_CPreq));
    memset(sndchange.ucMethodId_CPreq,0, sizeof(sndchange.ucMethodId_CPreq));

    iGenerateRequestID((char *)username,(unsigned char *)&sndchange.ucRequestId_CPreq);
    memcpy(sndchange.ucTellerID_CPreq,username,strlen(username));
    memcpy(sndchange.ucIsEncrypt_CPreq,"false",5);
    memcpy(sndchange.ucSessionId_CPreq,configLoginLeg2.ucSessionId, strlen((char *)configLoginLeg2.ucSessionId));
    memcpy(sndchange.ucMethodId_CPreq,"14",2);
    memcpy(sndchange.uccustomerNo_CPreq,configLoginLeg2.ucmobileNumber, strlen((char *)configLoginLeg2.ucmobileNumber));
    memcpy(sndchange.ucauthid_CPreq,"1", 1);
    memcpy(sndchange.ucRDMethodID_CPreq,"2", 1);
    memcpy(sndchange.ucRDRequestID_CPreq,OTP.ucRDRequestId, strlen((char *)OTP.ucRDRequestId));
    memcpy(sndchange.ucRDOTPPIN_CPreq,ui->OTP_Lineedit->text().trimmed().toAscii().data(), strlen((char *)ui->OTP_Lineedit->text().trimmed().toAscii().data()));
    memcpy(sndchange.ucMessageID_CPreq,"42",2);
    memcpy(sndchange.ucSessionExpiryTime_CPreq,configLoginLeg2.ucSessionExpiryTime, strlen((char *)configLoginLeg2.ucSessionExpiryTime));
    memcpy(sndchange.ucUserID_CPreq,configLoginLeg2.ucUser_id_req, strlen((char *)configLoginLeg2.ucUser_id_req));
    sprintf((char *)sndchange.ucOldPassword_CPreq,"%s|%s",OTP.ucencrypkey,OLDPASSWORD);//PtRBW+X4voniatuyPRYnUg== -> Nov@2018
    sprintf((char *)sndchange.ucNewPassword_CPreq,"%s|%s",OTP.ucencrypkey,NEWPASSWORD);//PtRBW+X4voniatuyPRYnUg== -> Nov@2018


    ret	= makeJSONOTP(21);
    if(ret!=FUNC_OK)
    {
        qDebug()<<"JSON Creation failed";
        return FUNC_ERROR;
    }

#ifdef UAT_HTTP     // File Download

    uidmessage::instance()->hide_Message();
    hComm.close_connection();
    res = hComm.open_connect(SERVERIP,SERVERPORT);
    if(res != 0)
    {
        qDebug() << "Failed to connect to the Server";
        uidmessage::instance()->hide_Message();
        uidmessage::instance()->DisplayMessage("Server Not Connected");
        qDebug()<<"FUNCTION_FAILED";
        ui->stackedWidget->setCurrentIndex(0);
        return FUNCTION_FAILED ;
    }


    int len_gSendData = strlen((char *)gSendData);
    memset(CHANPASS_Buff,0,sizeof(CHANPASS_Buff));
    memset(oBuff,0,sizeof(oBuff));
    if((hComm.host_2Sendrecv((unsigned char *)gSendData,len_gSendData,( unsigned char*)oBuff,(int *)&CHANPASS_len,(int)response_timeout1,0))!=SUCCESS)
    {
        printf("No Response From Server\n");
        hComm.close_connection();
        miscObj.DisplayMsgBox((char *)"Response Time-Out");
        uidmessage::instance()->hide_Message();
        qDebug()<<"***   Response Time-Out   ***";
        ui->stackedWidget->setCurrentIndex(1);
    }
    hComm.close_connection();

#else     // File Download full condition

#if DEBUG
    printf("gSendData = %s\n\n",gSendData);
#endif

    memset(str_url,0,sizeof(str_url));
#ifdef PRODUCTION
    strcpy(str_url,URL_CREATION(CHANGEPASSWD_OTP_URL_PROD));
#else
    strcpy(str_url,URL_CREATION(CHANGEPASSWD_OTP_URL));
#endif

    int retk = file_download(str_url, (char *)gSendData,"/usr/FINO/FPB/RESP/OUTPUT.TXT",120);
    if(retk!=0)
    {
        qDebug()<<"retk = "<<retk;
        sleep(1);
        uidmessage::instance()->hide_Message();
        return FUNCTION_FAILED;
    }

#endif

    system("cd /usr/FINO/FPB/RESP/");
    system("rm /usr/FINO/FPB/RESP/OUTPUT.TXT");
    QString CHANGEBuff = QString::fromLocal8Bit(oBuff);
    //   QString ENCBuff = "{RequestId":null,"DisplayMessage":null,"ResponseMessage":"Contract Error","ResponseCode":"1","MessageType":"Exclam","ResponseData":null,"MessageId":null,"SessionExpiryTime":null,"SessionId":null,"RouteID":"YUIService To ProcessLoginRequest","ErrorCode":0,"XMLData":null,"AuthmanFlag":false,"ServiceID":0}"
    bool check = CHANGEBuff.contains("ResponseCode");
    if(check == false)
    {
        miscObj.DisplayMsgBox((char *)"INVALID DATA ");
        uidmessage::instance()->hide_Message();
        return FUNCTION_FAILED;
    }


    CHANGEBuff.remove("\\");
    CHANGEBuff.remove("{");
    CHANGEBuff.remove("}");
    CHANGEBuff.remove('"');
#if DEBUG
    qDebug()<<"OTPBuff"<<CHANGEBuff;
#endif
    QStringList query = CHANGEBuff.split(",");
#if DEBUG

    for(int i=0 ; i < query.length() ; i++)
        qDebug()<<"Query Response"<<query.at(i);
#endif
    QStringList ResponseCode = query.filter("ResponseCode");
    QStringList ResponseMessage = query.filter("ResponseMessage");
    uidmessage::instance()->hide_Message();
    QString strToFind,str;
#if DEBUG
    qDebug()<<"ResponseCode.at(0)"<<ResponseCode.at(0);
    qDebug()<<"ResponseMessage.at(0)"<<ResponseMessage.at(0);
#endif
    str = ResponseCode.at(0);
    strToFind = ":";
    QString value1 = str.mid(str.indexOf(strToFind)+strToFind.length());

    str = ResponseMessage.at(0);
    QString value2 = str.mid(str.indexOf(strToFind)+strToFind.length());
#if DEBUG
    qDebug()<<"value1"<<value1;
    qDebug()<<"value2"<<value2;
#endif

    if(value1 =="0")
    {
        sprintf(ErrorMsg,"%s",(char *)value2.toAscii().data()/*,(char *)value1.toAscii().data()*/);
        miscObj.DisplayMsgBox(ErrorMsg);
        uidmessage::instance()->hide_Message();
        return FUNCTION_SUCCESS;
    }
    else
    {
        if(strcmp((char *)value2.toAscii().data(),"Session Expired.")==0)
        {
            uidmessage::instance()->hide_Message();
            sprintf(ErrorMsg,"%s",(char *)value2.toAscii().data());
            miscObj.DisplayMsgBox(ErrorMsg);
            uidmessage::instance()->hide_Message();
            return SESSION_EXPIRED_FAILED;
        }
        if((strcmp((char *)value2.toAscii().data(),"OTP expired")==0) || (strcmp((char *)value2.toAscii().data(),"OTP Verification failed")==0) || (strcmp((char *)value2.toAscii().data(),"OTP Authentication Failed")==0)) // OTP already Consumed
        {
            uidmessage::instance()->hide_Message();
            sprintf(ErrorMsg,"%s",(char *)value2.toAscii().data());
            miscObj.DisplayMsgBox(ErrorMsg);
            uidmessage::instance()->hide_Message();
            ui->OTP_Lineedit->clear();
            ui->OTP_Lineedit->setFocus();

            return OTP_FAILED;
        }
        sprintf(ErrorMsg,"%s",(char *)value2.toAscii().data()/*,(char *)value1.toAscii().data()*/);
        miscObj.DisplayMsgBox(ErrorMsg);
        uidmessage::instance()->hide_Message();
        //        OTP_page = -1;
        return FUNCTION_FAILED;
    }


}


int FPB::Prepare_encryption_Func()
{
    HostComm hComm;
    CMisc miscObj;
    int ENC_len = 0, response_timeout1 = 120, ret = -3, res = -3, iRet = -3;
    //    char ENC_Buff[1000];
    char ErrorMsg[100];


    uidmessage::instance()->hide_Message();
    uidmessage::instance()->Display_BusyMessage("Please Wait...");

    iGenerateRequestID((char *)username,(unsigned char *)&OTP.ucRequestID_req);
    memset(ErrorMsg,0,sizeof(ErrorMsg));
    memset(OTP.ucMethodID_req,0, sizeof(OTP.ucMethodID_req));
    memset(OTP.ucTellerID_req,0, sizeof(OTP.ucTellerID_req));
    memset(OTP.ucIsEncrypt_req,0, sizeof(OTP.ucIsEncrypt_req));

    memcpy(OTP.ucMethodID_req,"12",2);
    memcpy(OTP.ucTellerID_req,username,strlen((char *)username));
    memcpy(OTP.ucIsEncrypt_req,"false",5);

    ret	= makeJSONOTP(13);
    if(ret!=FUNC_OK)
    {
        qDebug()<<"JSON Creation failed";
        return FUNC_ERROR;
    }

#ifdef UAT_HTTP     // File Download

    uidmessage::instance()->hide_Message();
    hComm.close_connection();
    res = hComm.open_connect(SERVERIP,SERVERPORT);
    if(res != 0)
    {
        qDebug() << "Failed to connect to the Server";
        uidmessage::instance()->hide_Message();
        uidmessage::instance()->DisplayMessage("Server Not Connected");
        qDebug()<<"FUNCTION_FAILED";
        ui->stackedWidget->setCurrentIndex(0);
        return FUNCTION_FAILED;
    }


    int len_gSendData = strlen((char *)gSendData);
    memset(ENC_Buff,0,sizeof(ENC_Buff));
    memset(oBuff, 0, sizeof(oBuff));
    if((hComm.host_2Sendrecv((unsigned char *)gSendData,len_gSendData,( unsigned char*)oBuff,(int *)&ENC_len,(int)response_timeout1,0))!=SUCCESS)
    {
        printf("No Response From Server\n");
        hComm.close_connection();
        miscObj.DisplayMsgBox((char *)"Response Time-Out");
        uidmessage::instance()->hide_Message();
        qDebug()<<"***   Response Time-Out   ***";
        if(Flag_FirstTime_Login == 1)
            ui->stackedWidget->setCurrentIndex(0);
        else
            ui->stackedWidget->setCurrentIndex(1);
    }
    hComm.close_connection();

#else     // File Download full condition

#if DEBUG
    printf("gSendData = %s\n\n",gSendData);
#endif

    memset(str_url,0,sizeof(str_url));
#ifdef PRODUCTION
    strcpy(str_url,URL_CREATION(CHANGEPASSWD_OTP_URL_PROD));
#else
    strcpy(str_url,URL_CREATION((char *)CHANGEPASSWD_OTP_URL));
#endif

    int retk = file_download(str_url, (char *)gSendData,"/usr/FINO/FPB/RESP/OUTPUT.TXT",120);
    if(retk!=0)
    {
        qDebug()<<"retk = "<<retk;
        sleep(1);
        uidmessage::instance()->hide_Message();
        return FUNCTION_FAILED;
    }

#endif

    system("cd /usr/FINO/FPB/RESP/");
    system("rm /usr/FINO/FPB/RESP/OUTPUT.TXT");
    QString ENCBuff = QString::fromLocal8Bit(oBuff);
    //   QString ENCBuff = "{RequestId":null,"DisplayMessage":null,"ResponseMessage":"Contract Error","ResponseCode":"1","MessageType":"Exclam","ResponseData":null,"MessageId":null,"SessionExpiryTime":null,"SessionId":null,"RouteID":"YUIService To ProcessLoginRequest","ErrorCode":0,"XMLData":null,"AuthmanFlag":false,"ServiceID":0}"
    bool check = ENCBuff.contains("ResponseCode");
    if(check == false)
    {
        miscObj.DisplayMsgBox((char *)"Invalid Data");
        uidmessage::instance()->hide_Message();
        return FUNCTION_FAILED;
    }


    ENCBuff.remove("\\");
    ENCBuff.remove("{");
    ENCBuff.remove("}");
    ENCBuff.remove('"');
#if DEBUG
    qDebug()<<"OTPBuff"<<ENCBuff;
#endif
    QStringList query = ENCBuff.split(",");
#if DEBUG
    for(int i=0 ; i < query.length() ; i++)
        qDebug()<<"Query Response"<<query.at(i);
#endif

    QStringList ResponseCode = query.filter("ResponseCode");
    QStringList ResponseMessage = query.filter("ResponseMessage");
    QStringList encryptionKey = query.filter("encryptionKey");
    uidmessage::instance()->hide_Message();
    QString strToFind,str;
#if DEBUG
    qDebug()<<"ResponseCode.at(0)"<<ResponseCode.at(0);
    qDebug()<<"ResponseMessage.at(0)"<<ResponseMessage.at(0);
    qDebug()<<"encryptionKey.at(0)"<<encryptionKey.at(0);
#endif
    str = ResponseCode.at(0);
    strToFind = ":";
    QString value1 = str.mid(str.indexOf(strToFind)+strToFind.length());

    str = ResponseMessage.at(0);
    QString value2 = str.mid(str.indexOf(strToFind)+strToFind.length());

    str = encryptionKey.at(0);
    QString value3 = str.mid(str.indexOf(strToFind)+strToFind.length());
#if DEBUG
    qDebug()<<"value1"<<value1;
    qDebug()<<"value2"<<value2;
    qDebug()<<"value3"<<value3;
#endif
    if(value1 =="0")
    {
        memset(OTP.ucencrypkey,0, sizeof(OTP.ucencrypkey));
        memcpy(OTP.ucencrypkey,value3.trimmed().toAscii().data() ,strlen(value3.trimmed().toAscii().data()));
        //                memcpy(encryption_key,OTP.ucencrypkey, sizeof(OTP.ucencrypkey));
        //                printf("encryption_key789%s\n",encryption_key);
        // ui->stackedWidget->setCurrentIndex(22);
        return FUNCTION_SUCCESS;
    }
    else
    {
        //        qDebug()<<"value2.toAscii().data() == "<<value2;
        if(strcmp((char *)value2.toAscii().data(),"Session Expired.")==0)
        {
            uidmessage::instance()->hide_Message();
            sprintf(ErrorMsg,"%s",/*(char *)LTS.ucResponseCode,*/(char *)value2.toAscii().data());
            miscObj.DisplayMsgBox(ErrorMsg);
            uidmessage::instance()->hide_Message();
            //            ui->stackedWidget->setCurrentIndex(0);
            return SESSION_EXPIRED_FAILED;
        }

        sprintf(ErrorMsg,"%s",(char *)value2.toAscii().data()/*,(char *)value1.toAscii().data()*/);
        miscObj.DisplayMsgBox(ErrorMsg);
        uidmessage::instance()->hide_Message();
        return FUNCTION_FAILED;
    }

    return FUNCTION_SUCCESS;
}


void FPB::on_Forget_password_Back_button_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    return;
}

void FPB::on_Discard_otp_Button_clicked()
{

    if(OTP_page == 1)
    {
        ui->stackedWidget->setCurrentIndex(0);
        return;
    }
    if(OTP_page == 3)
    {
        ui->stackedWidget->setCurrentIndex(34);
        return;
    }
    else if(OTP_page == 4)
    {
        ui->Next_Otp_Button_2->setVisible(true);
        ui->FINISH_PushButton_Pg34->setVisible(false);
        ui->stackedWidget->setCurrentIndex(18);
        return;
    }
    if(OTP_page == 5)
    {
        ui->Transaction_Status_label_pg41->setVisible(false);
        ui->label_TransactionID_pg41->setVisible(false);
        ui->TransactionID_TransStWdRRN_lineEdit_pg41->setVisible(false);
        ui->TransactionID_TransStWdRRN_OK_pushButton_pg41->setVisible(false);
        ui->stackedWidget->setCurrentIndex(41);
        return;
    }
    if(OTP_page == 7)
    {
        ui->stackedWidget->setCurrentIndex(32);
        return;
    }
    if(Flag_FirstTime_Login == 1)
    {
        ui->stackedWidget->setCurrentIndex(0);
        return;
    }
    else
        ui->stackedWidget->setCurrentIndex(12);
    return;
}


//void FPB::on_Refresh_otp_button_clicked()
//{
//    memset(&OTP,0,sizeof(OTP));
//    Check_Conditions();
//    printf("Mobile Number123 %s",configLoginLeg2.ucmobileNumber);
//    ui->OTP_Lineedit->clear();
//    int iRet =-2,iRet1 = -2;



//    iRet = Resend_function();
//    if(iRet != FUNCTION_SUCCESS)
//    {
//        uidmessage::instance()->hide_Message();
//        ui->stackedWidget->setCurrentIndex(22);
//    }
//    else if(iRet == FUNCTION_SUCCESS)
//    {
//        uidmessage::instance()->hide_Message();
//        ui->stackedWidget->setCurrentIndex(22);
//    }
//    return;
//}

int FPB::Resend_function()
{
    uidmessage::instance()->hide_Message();
    uidmessage::instance()->Display_BusyMessage("Please Wait...");

    HostComm hComm;
    CMisc miscObj;
    int OTP_len = 0, response_timeout1 = 120, ret = -3, res = -3, iRet = -3;
    char OTP_Buff[10000];
    char ErrorMsg[100];
    memset(ErrorMsg,0,sizeof(ErrorMsg));
    memset(OTP.ucMethodID_req,0, sizeof(OTP.ucMethodID_req));
    memset(OTP.ucTellerID_req,0, sizeof(OTP.ucTellerID_req));
    memset(OTP.ucSessionID_req,0, sizeof(OTP.ucSessionID_req));
    memset(OTP.ucMessageId_req,0, sizeof(OTP.ucMessageId_req));
    memset(OTP.ucIsEncrypt_req,0, sizeof(OTP.ucIsEncrypt_req));
    memset(OTP.ucCustomerMobileNo_req,0, sizeof(OTP.ucCustomerMobileNo_req));
    memset(OTP.ucChannelID_req,0, sizeof(OTP.ucChannelID_req));

    iGenerateRequestID((char *)username,(unsigned char *)&OTP.ucRequestID_req);
    memcpy(OTP.ucMethodID_req,"1",1);
    memcpy(OTP.ucTellerID_req,username,strlen((char *)username));
    memcpy(OTP.ucChannelID_req,CHANNEL_ID,strlen(CHANNEL_ID));
    memcpy(OTP.ucIsEncrypt_req,"false",5);

    if(OTP_page == 4)
        memcpy(OTP.ucCustomerMobileNo_req,(char *)FC_MobileNumber, strlen((char *)FC_MobileNumber));
    else if(OTP_page == 1)
        memcpy(OTP.ucCustomerMobileNo_req,configLoginLeg1.ucmobileNumber, strlen((char *)configLoginLeg1.ucmobileNumber));
    else
        memcpy(OTP.ucCustomerMobileNo_req,configLoginLeg2.ucmobileNumber, strlen((char *)configLoginLeg2.ucmobileNumber));


    memcpy(OTP.ucMessageId_req,"42",2);
    memcpy(OTP.ucRDFMethodID_req,"4",1);
    memcpy(OTP.ucSessionID_req,configLoginLeg2.ucSessionId,strlen((char *)configLoginLeg2.ucSessionId));
    //    printf("OTP.ucencrypkey456%s\n",OTP.ucencrypkey);
    //   qDebug()<<"*******************NEW PASSWORD 3 == "<<ui->New_password_edit_pg26->text().trimmed().toAscii().data();
    //   printf("*******************NEW PASSWORD 5353 OTP.ucnewpassword == %s\n",OTP.ucnewpassword);



    ret	= makeJSONOTP(11);
    if(ret!=FUNC_OK)
    {
        qDebug()<<"JSON Creation failed";
        return FUNC_ERROR;
    }
    //   qDebug()<<"*******************NEW PASSWORD 4 == "<<ui->New_password_edit_pg26->text().trimmed().toAscii().data();
#ifdef UAT_HTTP     // File Download

    uidmessage::instance()->hide_Message();
    hComm.close_connection();
    res = hComm.open_connect(SERVERIP,SERVERPORT);
    if(res != 0)
    {
        qDebug() << "Failed to connect to the Server";
        uidmessage::instance()->hide_Message();
        uidmessage::instance()->DisplayMessage("Server Not Connected");
        qDebug()<<"FUNCTION_FAILED";
        ui->stackedWidget->setCurrentIndex(0);
        return FUNCTION_FAILED;
    }


    int len_gSendData = strlen((char *)gSendData);
    memset(OTP_Buff,0,sizeof(OTP_Buff));
    memset(oBuff,0,sizeof(oBuff));
    if((hComm.host_2Sendrecv((unsigned char *)gSendData,len_gSendData,( unsigned char*)oBuff,(int *)&OTP_len,(int)response_timeout1,0))!=SUCCESS)
    {
        printf("No Response From Server\n");
        hComm.close_connection();
        miscObj.DisplayMsgBox((char *)"Response Time-Out");
        uidmessage::instance()->hide_Message();
        qDebug()<<"***   Response Time-Out   ***";
        ui->stackedWidget->setCurrentIndex(1);
    }
    hComm.close_connection();


#else     // File Download full condition

#if DEBUG
    printf("gSendData = %s\n\n",gSendData);
#endif

    memset(str_url,0,sizeof(str_url));
#ifdef PRODUCTION
    strcpy(str_url,URL_CREATION(OLDNEW_CHANGE_PASSWORD_URL_PROD));
#else
    if(OTP_page == 1)
    {
        strcpy(str_url,URL_CREATION((char *)FORGET_URL));
    }
    else
    {
        strcpy(str_url,URL_CREATION((char *)OLDNEW_CHANGE_PASSWORD_URL));
    }
#endif

    int retk = file_download(str_url, (char *)gSendData,"/usr/FINO/FPB/RESP/OUTPUT.TXT",120);
    if(retk!=0)
    {
        qDebug()<<"retk = "<<retk;
        sleep(1);
        uidmessage::instance()->hide_Message();
        return FUNCTION_FAILED;
    }

#endif
    //    qDebug()<<"*******************NEW PASSWORD 5 == "<<ui->New_password_edit_pg26->text().trimmed().toAscii().data();
    //   printf("*******************NEW PASSWORD 6565 OTP.ucnewpassword == %s\n",OTP.ucnewpassword);
    system("cd /usr/FINO/FPB/RESP/");
    system("rm /usr/FINO/FPB/RESP/OUTPUT.TXT");
    QString OTPBuff = QString::fromLocal8Bit(oBuff);
    bool check = OTPBuff.contains("ResponseCode");
    if(check == false)
    {
        miscObj.DisplayMsgBox((char *)"Invalid data");
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
    memset(OTP.ucResponseCode, 0, sizeof(OTP.ucResponseCode));
    memcpy(OTP.ucResponseCode,value1.trimmed().toAscii().data(), strlen(value1.trimmed().toAscii().data()));
    //  qDebug()<<"value1"<<value1;
    if(value1 =="0")
    {
        //      qDebug()<<"*******************NEW PASSWORD 6 == "<<ui->New_password_edit_pg26->text().trimmed().toAscii().data();
        //       printf("*******************NEW PASSWORD 7676 OTP.ucnewpassword == %s\n",OTP.ucnewpassword);
        str = DisplayMessage.at(2);
        strToFind = ":";
        value2 = str.mid(str.indexOf(strToFind)+strToFind.length());
        memset(OTP.ucDisplayMessage, 0, sizeof(OTP.ucDisplayMessage));
        memcpy(OTP.ucDisplayMessage,value2.trimmed().toAscii().data(), strlen(value2.trimmed().toAscii().data()));
        //      printf("*******************NEW PASSWORD 767612 OTP.ucnewpassword == %s\n",OTP.ucnewpassword);
        str= RequestId.at(2);
        strToFind= "RequestId:";
        value = str.mid(str.indexOf(strToFind)+strToFind.length());
        //     printf("*******************NEW PASSWORD 767621 OTP.ucnewpassword == %s\n",OTP.ucnewpassword);
        memset(OTP.ucRDRequestId, 0, sizeof(OTP.ucRDRequestId));
        memcpy(OTP.ucRDRequestId,value.trimmed().toAscii().data(), strlen(value.trimmed().toAscii().data()));
        //      qDebug()<<"OTP.ucRDRequestId"<<OTP.ucRDRequestId;
        //        ui->stackedWidget->setCurrentIndex(22);
        return FUNCTION_SUCCESS;
    }
    else
    {
        str = ResponseMessage.at(0);
        strToFind = ":";
        value2 = str.mid(str.indexOf(strToFind)+strToFind.length());
        memset(OTP.ucResponseMessage, 0, sizeof(OTP.ucResponseMessage));
        memcpy(OTP.ucResponseMessage,value2.trimmed().toAscii().data(), strlen(value2.trimmed().toAscii().data()));
        if(strcmp((char *)OTP.ucResponseMessage,"Session Expired.")==0)
        {
            uidmessage::instance()->hide_Message();
            sprintf(ErrorMsg,"%s",/*(char *)LTS.ucResponseCode,*/(char *)OTP.ucResponseMessage);
            miscObj.DisplayMsgBox(ErrorMsg);
            uidmessage::instance()->hide_Message();
            return SESSION_EXPIRED_FAILED;
        }
        sprintf(ErrorMsg,"%s",/*(char *)OTP.ucResponseCode,*/(char *)OTP.ucResponseMessage);
        miscObj.DisplayMsgBox(ErrorMsg);
        uidmessage::instance()->hide_Message();
        return FUNCTION_FAILED;
    }

    return FUNCTION_SUCCESS;
}


void FPB::on_Transaction_Summary_3_clicked()
{
    QDate date = QDate::currentDate();
    QTime time = QTime::currentTime();
    QDateTime startDate(date, time);
    QDateTime endDate(date, time);
    ui->listWidget->clear();
    memset(display,0,sizeof(display));
    //  qDebug() << "Days from startDate to endDate: " << startDate.daysTo(endDate);
    ui->dateEdit_5->setDateTime(startDate);
    ui->dateEdit_6->setDateTime(startDate);
    ui->stackedWidget->setCurrentIndex(32);
    return;
}

int FPB::Merchant_history()
{
    uidmessage::instance()->hide_Message();
    uidmessage::instance()->Display_BusyMessage("Please Wait...");

    CMisc miscObj;
    char to_date[20],from_date[20];
    QString buff_tmp;
    QString todate = ui->dateEdit_5->date().toString("yyyy-MM-dd") ;
    QString fromdate = ui->dateEdit_6->date().toString("yyyy-MM-dd") ;
    //    int today = ui->dateEdit_5->date().day();
    //    int tomonth = ui->dateEdit_5->date().month();
    //    int toyear = ui->dateEdit_5->date().year();

    //    int fromday = ui->dateEdit_6->date().day();
    //    int frommonth = ui->dateEdit_6->date().month();
    //    int fromyear = ui->dateEdit_6->date().year();

    QDate cal_todate = ui->dateEdit_5->date(); // From
    QDate cal_fromdate = ui->dateEdit_6->date(); // To

    int diff_days = getDifference(cal_todate, cal_fromdate); // getDifference(From Date, To Date)
#if DEBUG
    qDebug()<<"diff_days = "<<diff_days;
    qDebug()<<"todate"<<todate;
    qDebug()<<"fromdate"<<fromdate;
#endif
    if((diff_days > 7) || (diff_days < 0))
    {
        uidmessage::instance()->hide_Message();
        miscObj.DisplayMsgBox((char *)"Date difference shouldn't be more than 7 days");
        return FUNC_BACK;
    }



    strcpy(to_date,todate.toAscii().constData());
    strcpy(from_date,fromdate.toAscii().constData());
    char* jsondata;
    char ErrorMsg[100];
    int ret;
    Json1::Value RequestedData,mainreq;
    memset(Merdetail.ucRequestID_req,0, sizeof(Merdetail.ucRequestID_req));

    iGenerateRequestID((char *)username,(unsigned char *)&Merdetail.ucRequestID_req);
    //sprintf(buff1,"1234567890");

    //int kkkk = 102920;
    mainreq["RequestId"] =  Merdetail.ucRequestID_req;
    mainreq["MethodId"] = 91;
    mainreq["ChannelID"] = CHANNEL_ID;
    mainreq["ProductCode"] = "null";
    mainreq["SessionId"] = (char *)configLoginLeg2.ucSessionId;//buff_tmp.toAscii().data();
    //    qDebug()<<"SeSSION id"<<(char *)configLoginLeg2.ucSessionId;
    mainreq["TellerID"]= username;
    mainreq["TokenId"]= "";
    mainreq["X_Auth_Token"] ="";
    mainreq["IsEncrypt"]= false;
    mainreq["Amount"]= "null";
    mainreq["SessionExpiryTime"]= (char *)configLoginLeg2.ucSessionExpiryTime;//OtpImpsNeft.Commdata_OIN.ucSessionExpiryTime_req;;//(char *)configLoginLeg2.ucSessionExpiryTime;//"10/11/2018 11:58:44 PM";
    //qDebug()<<"SeSSION EXPIRY LEG2"<<OtpImpsNeft.Commdata_OIN.ucSessionExpiryTime_req;

    RequestedData["fromDate"] = to_date;
    RequestedData["toDate"] = from_date;
    RequestedData["tranType"] = "";

    RequestedData["accountNo"] = configLoginLeg2.ucBLaccountNo;//(char *)configLoginLeg2.ucBLaccountNo;//"00000100";

    RequestedData["order"] = "DESC";
    RequestedData["sProduct"] = "";
    RequestedData["fromAmount"] = "";
    RequestedData["toAmount"] = "";
    RequestedData["referenceNo"] = "";

    buff_tmp = serialize1(Convertstr(RequestedData.toStyledString()));

    mainreq["RequestData"] = buff_tmp.toAscii().data();

    jsondata = Convertstr(mainreq.toStyledString());
#if DEBUG
    qDebug()<<"------------->"<<jsondata;
#endif
    //-------------------------------------------------------------
    memset(gSendData,0,sizeof(gSendData));
    strncpy((char*)gSendData, jsondata, strlen(jsondata));
#if DEBUG
    printf("gSendData = %s\n\n",gSendData);
#endif
    //-------------------------------------------------------------

#ifdef UAT_HTTP     // File Download

    uidmessage::instance()->hide_Message();
    uidmessage::instance()->Display_BusyMessage("Connecting server");
    sleep(1);
    uidmessage::instance()->hide_Message();
    hComm.close_connection();
    res = hComm.open_connect(SERVERIP,SERVERPORT);
    if(res != 0)
    {
        qDebug() << "Failed to connect to the Server";
        uidmessage::instance()->hide_Message();
        uidmessage::instance()->DisplayMessage("Server Not Connected");
        qDebug()<<"FUNCTION_FAILED";
        return FUNCTION_FAILED ;
    }


    sleep(1);
    uidmessage::instance()->hide_Message();
    uidmessage::instance()->Display_BusyMessage("Transmitting ..");


    int len_gSendData = strlen((char *)gSendData);
    memset(oBuff,0,sizeof(oBuff));
    if((hComm.host_2Sendrecv((unsigned char *)gSendData,len_gSendData,( unsigned char*)oBuff,(int *)&oLen,(int)response_timeout,0))!=SUCCESS)
    {
        printf("No Response From Server\n");
        hComm.close_connection();
        miscObj.DisplayMsgBox((char *)"Response Time-Out");
        sleep(2);
        qDebug()<<"***   Response Time-Out   ***";
        uidmessage::instance()->hide_Message();
        //return SOCKET_RESPONSE_FAILED;
        return FUNCTION_FAILED;
    }

    hComm.close_connection();

#else     // File Download full condition
#if DEBUG
    printf("gSendData = %s\n\n",gSendData);
#endif
    memset(str_url,0,sizeof(str_url));
#ifdef PRODUCTION
    strcpy(str_url,URL_CREATION(LOGIN_URL_PROD));
#else
    strcpy(str_url,URL_CREATION((char *)MER_REFRESH_URL));
#endif

    int retk = file_download(str_url, (char *)gSendData,"/usr/FINO/FPB/RESP/OUTPUT.TXT",120);
    if(retk!=0)
    {
        sleep(1);
        uidmessage::instance()->hide_Message();
        return FUNCTION_FAILED;
    }


#endif


#if DEBUG
    printf("\noBuff = %s\n",oBuff);
#endif
    system("mkdir /usr/FINO/FPB/RESP/");
    system("cd /usr/FINO/FPB/RESP/");
    system("rm /usr/FINO/FPB/RESP/MER_RESP.txt");
    int gRecv_len = strlen((char *)oBuff);
    FILE *fp2 = NULL;
    fp2 = fopen("/usr/FINO/FPB/RESP/MER_RESP.TXT","w");
    if(fp2 == NULL)
    {
        return FUNCTION_FAILED;
    }
    fwrite( oBuff, 1, gRecv_len, fp2 );
    fclose(fp2);
    int iRetparse = Mer_statement();
    system("cd /usr/FINO/FPB/RESP/");
    system("rm /usr/FINO/FPB/RESP/OUTPUT.TXT");
    system("rm /usr/FINO/FPB/RESP/MER_RESP.TXT");

    if(iRetparse==FUNC_OK)
    {
        int j,i,l;
#if DEBUG
        qDebug()<<"Inside the display"<<Merdetail.icount;
#endif
        int vount = Merdetail.icount;
        for(j=0,l=0;j<vount;j=j++,l=l+4)
        {

            QString trnxAmount;
            trnxAmount.clear();
            if(strcmp(Statement[j].debitCredit, "Cr") == 0)
                trnxAmount = "+" + QString::fromUtf8(Statement[j].transactionAmount);
            else if(strcmp(Statement[j].debitCredit, "Dr") == 0)
                trnxAmount = "-" + QString::fromUtf8(Statement[j].transactionAmount);
            else
                trnxAmount = "" + QString::fromUtf8(Statement[j].transactionAmount);;

            sprintf(display[l+1],"%s  %s   %s",Statement[j].transactionDate,Statement[j].time,Statement[j].endingbalance);
            sprintf(display[l+2],"%20s    %s",Statement[j].TransactionTypeDescription,trnxAmount.toAscii().data());
            sprintf(display[l+3],"%s   %s",Statement[j].referenceId,Statement[j].StatusDesc);
            sprintf(display[l+4],"----------------------------------------------- ");
#if DEBUG
            qDebug()<<"Value of j="<<j<<"l="<<l;
            qDebug()<<"Inside the display"<<vount;
#endif
        }
        ui->listWidget->clear();
        for(i=0;i<(4*vount);i++)
        {
#if DEBUG
            printf("display    =====    %s\n",display[i]);
#endif
            ui->listWidget->addItem((char *)display[i]);
        }
        free(Statement);
        uidmessage::instance()->hide_Message();
        return FUNCTION_SUCCESS;

    }
    else
    {

        if(strcmp((char *)Merdetail.ucResponseMessage,"Session Expired.")==0)
        {
            uidmessage::instance()->hide_Message();
            sprintf(ErrorMsg,"%s",(char *)Merdetail.ucResponseMessage);
            miscObj.DisplayMsgBox(ErrorMsg);
            uidmessage::instance()->hide_Message();
            free(Statement);
            return SESSION_EXPIRED_FAILED;
        }
        sprintf(ErrorMsg,"%s",/*(char *)OTP.ucResponseCode,*/(char *)Merdetail.ucResponseMessage);
        miscObj.DisplayMsgBox(ErrorMsg);
        uidmessage::instance()->hide_Message();
        free(Statement);
        return FUNCTION_FAILED;

    }

    return FUNCTION_SUCCESS;


}

void FPB::on_PushButton_Back_pg26_3_clicked()
{
    Check_Conditions();
    int iRet =-2;
    if(TRANS_TYPE == MINISTATEMENT)
    {
        free(MiniStatement);
        iRet = Mini_Statement(1);
        if(iRet != FUNCTION_SUCCESS)
        {
            if(iRet == SESSION_EXPIRED_FAILED)
                ui->stackedWidget->setCurrentIndex(0);
            else if(iRet == FUNC_BACK)
                ui->stackedWidget->setCurrentIndex(32);
            else
                ui->stackedWidget->setCurrentIndex(15);
            return;
        }
        else if(iRet == FUNCTION_SUCCESS)
        {
            ui->stackedWidget->setCurrentIndex(32); // TEMPORARY
            return ;
        }
    }
    else
    {
        iRet = Merchant_history();
        if(iRet != FUNCTION_SUCCESS)
        {

            if(iRet == SESSION_EXPIRED_FAILED)
                ui->stackedWidget->setCurrentIndex(0);
            else if(iRet == FUNC_BACK)
                ui->stackedWidget->setCurrentIndex(32);
            else
                ui->stackedWidget->setCurrentIndex(12);
            return;
        }
        else if(iRet == FUNCTION_SUCCESS)
        {
            ui->stackedWidget->setCurrentIndex(32);; // TEMPORARY
            return ;
        }
    }
    return;
}

int FPB::Mer_statement()
{
    system("sed 's/\"ResponseData\":\"{/\"ResponseData\":{/g' /usr/FINO/FPB/RESP/MER_RESP.TXT > /tmp/1.txt");
    system("sed 's/}\",\"MessageId\"/},\"MessageId\"/g' /tmp/1.txt > /tmp/2.txt  ");
    system(" sed 's/\\\\//g' /tmp/2.txt > /usr/FINO/FPB/RESP/MER_RESP.TXT");

    system("cd /tmp/");
    system("rm /tmp/1.txt");
    system("rm /tmp/2.txt");
    //    system("sed 's/\"ResponseData\":\"{/\"ResponseData\":{/g' /usr/FINO/FPB/RESP/MER_HISTORY.TXT > /tmp/1.txt");
    //   // system("sed 's/\"}\"/\"}/g' /tmp/1.txt > /tmp/2.txt  ");
    //    qDebug()<<"1--------";
    //    system("sed 's/}\",\"MessageId\"/},\"MessageId\"/g' /tmp/1.txt > /tmp/2.txt ");
    ////    qDebug()<<"2-----------";
    ////    system("sed 's/\[//g' /tmp/2.txt > /tmp/3.txt");
    ////    qDebug()<<"3---------";
    ////    system("sed 's/\]//g' /tmp/3.txt > /tmp/4.txt");
    ////    qDebug()<<"4-----------";
    //    system("sed 's/\\//g' /tmp/2.txt > /usr/FINO/FPB/RESP/MER_HISTORY.TXT");
    //    qDebug()<<"5-----------";


    //------------------------THIRUPATHI--------------------------------------------------------------------------------
    //    system("sed 's/\"ResponseData\":\"{/\"ResponseData\":{/g' /usr/FINO/FPB/RESP/MER_HISTORY.TXT > /tmp/1.txt");
    //    system("sed 's/\"}\"/\"}/g' /tmp/1.txt > /tmp/2.txt  ");
    //    system(" sed 's/\\\\//g' /tmp/2.txt > /usr/FINO/FPB/RESP/login.json");
    //------------------------------------------------------------------------------------------------------------------

    Json1::Value root;
    root.clear();
    //QSqlQuery query;
    Json1::Reader reader;
    std::ifstream ReadData("/usr/FINO/FPB/RESP/MER_RESP.TXT");
    bool parsedSuccess = reader.parse(ReadData,root,false);

    ReadData.close();

    if(not parsedSuccess)
    {

        qDebug()<<"FpsMonthlyPdsData File not Found";
        return FUNCAL_FAILED;
    }
    else
    {
        memset(&Merdetail,0,sizeof(Merdetail));
#if DEBUG
        qDebug()<<"RequestId   : "<<Convertstr(root["RequestId"].asString());
        qDebug()<<"DisplayMessage : "<<Convertstr(root["DisplayMessage"].asString());
        qDebug()<<"ResponseMessage    : "<<Convertstr(root["ResponseMessage"].asString());
        qDebug()<<"ResponseCode : "<<Convertstr(root["ResponseCode"].asString());
        qDebug()<<"MessageType : "<<Convertstr(root["MessageType"].asString());
        qDebug()<<"RespocountNo : "<<Convertstr(root["RespocountNo"].asString());
#endif
        strcpy(Merdetail.ucRequestId,Convertstr(root["RequestId"].asString()));
        strcpy(Merdetail.ucDisplayMessage,Convertstr(root["DisplayMessage"].asString()));
        strcpy(Merdetail.ucResponseMessage,Convertstr(root["ResponseMessage"].asString()));
        strcpy(Merdetail.ucResponseCode,Convertstr(root["ResponseCode"].asString()));
        if(strcmp(Merdetail.ucResponseCode,"0")==0)
        {
            Json1::Value ResponseData;
            ResponseData.clear();
            ResponseData = root.get("ResponseData",0);
            qDebug()<<"Size of the ResponseData"<<ResponseData.size();
            printf("\nSize : %d\n",ResponseData.size());
            if(ResponseData.size()==0)
            {
                qDebug("\n1:Key Register Data Not Found\n");
                return FUNCAL_FAILED;
            }
            else
            {
                Json1::Value lstOfDashboardTxnList;
                lstOfDashboardTxnList.clear();
                lstOfDashboardTxnList = ResponseData.get("acctTransaction",0);
                //                qDebug()<<"Size of the lstDashboard"<<lstOfDashboardTxnList.size();
                Merdetail.icount =0;
                Merdetail.icount = lstOfDashboardTxnList.size();
                Statement =(TxnStatement*) malloc(sizeof(TxnStatement)*Merdetail.icount);

                for(int i=0;i<lstOfDashboardTxnList.size();i++)
                {
                    strcpy(Statement[i].product,Convertstr(lstOfDashboardTxnList[i]["product"].asString()));




                    strcpy(Statement[i].transactionType,Convertstr(lstOfDashboardTxnList[i]["transactionType"].asString()));
                    strcpy(Statement[i].channel,Convertstr(lstOfDashboardTxnList[i]["channel"].asString()));
                    strcpy(Statement[i].transactionDate,Convertstr(lstOfDashboardTxnList[i]["transactionDate"].asString()));
                    strcpy(Statement[i].customerNo,Convertstr(lstOfDashboardTxnList[i]["customerNo"].asString()));
                    strcpy(Statement[i].accountNo,Convertstr(lstOfDashboardTxnList[i]["accountNo"].asString()));
                    strcpy(Statement[i].transactionAmount,Convertstr(lstOfDashboardTxnList[i]["transactionAmount"].asString()));
                    strcpy(Statement[i].branchCode,Convertstr(lstOfDashboardTxnList[i]["branchCode"].asString()));
                    strcpy(Statement[i].userId,Convertstr(lstOfDashboardTxnList[i]["userId"].asString()));
                    strcpy(Statement[i].sequenceNumber,Convertstr(lstOfDashboardTxnList[i]["sequenceNumber"].asString()));
                    strcpy(Statement[i].referenceId,Convertstr(lstOfDashboardTxnList[i]["referenceId"].asString()));
                    strcpy(Statement[i].status,Convertstr(lstOfDashboardTxnList[i]["status"].asString()));
                    strcpy(Statement[i].reversalFlag,Convertstr(lstOfDashboardTxnList[i]["reversalFlag"].asString()));
                    strcpy(Statement[i].transactionDesc,Convertstr(lstOfDashboardTxnList[i]["transactionDesc"].asString()));
                    strcpy(Statement[i].time,Convertstr(lstOfDashboardTxnList[i]["time"].asString()));
                    strcpy(Statement[i].customerName,Convertstr(lstOfDashboardTxnList[i]["customerName"].asString()));
                    strcpy(Statement[i].customerMobile,Convertstr(lstOfDashboardTxnList[i]["customerMobile"].asString()));
                    strcpy(Statement[i].splitSequence,Convertstr(lstOfDashboardTxnList[i]["splitSequence"].asString()));
                    strcpy(Statement[i].debitCredit,Convertstr(lstOfDashboardTxnList[i]["debitCredit"].asString()));
                    strcpy(Statement[i].chargeAmount,Convertstr(lstOfDashboardTxnList[i]["chargeAmount"].asString()));
                    strcpy(Statement[i].payoutAmount,Convertstr(lstOfDashboardTxnList[i]["payoutAmount"].asString()));
                    strcpy(Statement[i].commissionAmount,Convertstr(lstOfDashboardTxnList[i]["commissionAmount"].asString()));
                    strcpy(Statement[i].tdsAmount,Convertstr(lstOfDashboardTxnList[i]["tdsAmount"].asString()));
                    strcpy(Statement[i].originalTxnSequence,Convertstr(lstOfDashboardTxnList[i]["originalTxnSequence"].asString()));
                    strcpy(Statement[i].parentTxnId,Convertstr(lstOfDashboardTxnList[i]["parentTxnId"].asString()));
                    strcpy(Statement[i].parentTxnAmount,Convertstr(lstOfDashboardTxnList[i]["parentTxnAmount"].asString()));
                    strcpy(Statement[i].parentTxnAmountCurrent,Convertstr(lstOfDashboardTxnList[i]["parentTxnAmountCurrent"].asString()));
                    strcpy(Statement[i].chargeAmountCurrent,Convertstr(lstOfDashboardTxnList[i]["chargeAmountCurrent"].asString()));
                    strcpy(Statement[i].transactionTypeName,Convertstr(lstOfDashboardTxnList[i]["transactionTypeName"].asString()));
                    strcpy(Statement[i].TransactionTypeProduct,Convertstr(lstOfDashboardTxnList[i]["TransactionTypeProduct"].asString()));
                    strcpy(Statement[i].TransactionTypeDescription,Convertstr(lstOfDashboardTxnList[i]["TransactionTypeDescription"].asString()));
                    strcpy(Statement[i].Status,Convertstr(lstOfDashboardTxnList[i]["Status"].asString()));
                    strcpy(Statement[i].StatusDesc,Convertstr(lstOfDashboardTxnList[i]["statusDesc"].asString()));

                    strcpy(Statement[i].endingbalance,Convertstr(lstOfDashboardTxnList[i]["endingBalance"].asString()));
                    strcpy(Statement[i].rfuDateDesc,Convertstr(lstOfDashboardTxnList[i]["rfuDateDesc"].asString()));
                    strcpy(Statement[i].rfuDateVal,Convertstr(lstOfDashboardTxnList[i]["rfuDateVal"].asString()));
                    strcpy(Statement[i].rfuNumeric1Desc,Convertstr(lstOfDashboardTxnList[i]["rfuNumeric1Desc"].asString()));
                    strcpy(Statement[i].rfuNumeric1Val,Convertstr(lstOfDashboardTxnList[i]["rfuNumeric1Val"].asString()));
                    strcpy(Statement[i].rfuNumeric2Desc,Convertstr(lstOfDashboardTxnList[i]["rfuNumeric2Desc"].asString()));
                    strcpy(Statement[i].rfuNumeric2Val,Convertstr(lstOfDashboardTxnList[i]["rfuNumeric2Val"].asString()));
                    strcpy(Statement[i].rfuNumeric3Desc,Convertstr(lstOfDashboardTxnList[i]["rfuNumeric3Desc"].asString()));
                    strcpy(Statement[i].rfuNumeric3Val,Convertstr(lstOfDashboardTxnList[i]["rfuNumeric3Val"].asString()));
                    strcpy(Statement[i].rfuNumeric4Desc,Convertstr(lstOfDashboardTxnList[i]["rfuNumeric4Desc"].asString()));
                    strcpy(Statement[i].rfuNumeric4Val,Convertstr(lstOfDashboardTxnList[i]["rfuNumeric4Val"].asString()));
                    strcpy(Statement[i].rfuText1Desc,Convertstr(lstOfDashboardTxnList[i]["rfuText1Desc"].asString()));
                    strcpy(Statement[i].rfuText1Val,Convertstr(lstOfDashboardTxnList[i]["rfuText1Val"].asString()));
                    strcpy(Statement[i].rfuText2Desc,Convertstr(lstOfDashboardTxnList[i]["rfuText2Desc"].asString()));
                    strcpy(Statement[i].rfuText2Val,Convertstr(lstOfDashboardTxnList[i]["rfuText2Val"].asString()));

                    strcpy(Statement[i].rfuText3Desc,Convertstr(lstOfDashboardTxnList[i]["rfuText3Desc"].asString()));

                    strcpy(Statement[i].rfuText3Val,Convertstr(lstOfDashboardTxnList[i]["rfuText3Val"].asString()));

                    strcpy(Statement[i].rfuText4Desc,Convertstr(lstOfDashboardTxnList[i]["rfuText4Desc"].asString()));

                    strcpy(Statement[i].rfuText4Val,Convertstr(lstOfDashboardTxnList[i]["rfuText4Val"].asString()));

                    strcpy(Statement[i].rfuText5Desc,Convertstr(lstOfDashboardTxnList[i]["rfuText5Desc"].asString()));

                    strcpy(Statement[i].rfuText5Val,Convertstr(lstOfDashboardTxnList[i]["rfuText5Val"].asString()));

                    strcpy(Statement[i].rfuText6Desc,Convertstr(lstOfDashboardTxnList[i]["rfuText6Desc"].asString()));


                    strcpy(Statement[i].rfuText6Val,Convertstr(lstOfDashboardTxnList[i]["rfuText6Val"].asString()));
                    strcpy(Statement[i].rfuText7Desc,Convertstr(lstOfDashboardTxnList[i]["rfuText7Desc"].asString()));

                    strcpy(Statement[i].rfuText7Val,Convertstr(lstOfDashboardTxnList[i]["rfuText7Val"].asString()));

                    strcpy(Statement[i].rfuText8Desc,Convertstr(lstOfDashboardTxnList[i]["rfuText8Desc"].asString()));

                    strcpy(Statement[i].rfuText8Val,Convertstr(lstOfDashboardTxnList[i]["rfuText8Val"].asString()));

                    strcpy(Statement[i].rfuText9Desc,Convertstr(lstOfDashboardTxnList[i]["rfuText9Desc"].asString()));

                    strcpy(Statement[i].rfuText9Val,Convertstr(lstOfDashboardTxnList[i]["rfuText9Val"].asString()));

                    strcpy(Statement[i].rfuText10Desc,Convertstr(lstOfDashboardTxnList[i]["rfuText10Desc"].asString()));

                    strcpy(Statement[i].rfuText10Val,Convertstr(lstOfDashboardTxnList[i]["rfuText10Val"].asString()));

                    strcpy(Statement[i].debitAmount,Convertstr(lstOfDashboardTxnList[i]["debitAmount"].asString()));

                    strcpy(Statement[i].creditAmount,Convertstr(lstOfDashboardTxnList[i]["creditAmount"].asString()));

                    strcpy(Statement[i].cGST,Convertstr(lstOfDashboardTxnList[i]["cGST"].asString()));

                    strcpy(Statement[i].sGST,Convertstr(lstOfDashboardTxnList[i]["sGST"].asString()));

                    strcpy(Statement[i].iGST,Convertstr(lstOfDashboardTxnList[i]["iGST"].asString()));


                    strcpy(Statement[i].uGST,Convertstr(lstOfDashboardTxnList[i]["uGST"].asString()));
#if DEBUG

                    qDebug()<<"Statement[i].product"<<Statement[i].product;
                    qDebug()<<"Statement[i].transactionType"<<Statement[i].transactionType;
                    qDebug()<<"Statement[i].channel"<<Statement[i].channel;
                    qDebug()<<"Statement[i].transactionDate"<<Statement[i].transactionDate;
                    qDebug()<<"Statement[i].customerNo"<<Statement[i].customerNo;
                    qDebug()<<"Statement[i].accountNo"<<Statement[i].accountNo;
                    qDebug()<<"Statement[i].transactionAmount"<<Statement[i].transactionAmount;
                    qDebug()<<"Statement[i].branchCode"<<Statement[i].branchCode;
                    qDebug()<<"Statement[i].userId"<<Statement[i].userId;
                    qDebug()<<"Statement[i].sequenceNumber"<<Statement[i].sequenceNumber;
                    qDebug()<<"Statement[i].referenceId"<<Statement[i].referenceId;
                    qDebug()<<"Statement[i].status"<<Statement[i].status;
                    qDebug()<<"Statement[i].reversalFlag"<<Statement[i].reversalFlag;
                    qDebug()<<"Statement[i].transactionDesc"<<Statement[i].transactionDesc;
                    qDebug()<<"Statement[i].time"<<Statement[i].time;
                    qDebug()<<"Statement[i].customerName"<<Statement[i].customerName;
                    qDebug()<<"Statement[i].customerMobile"<<Statement[i].customerMobile;
                    qDebug()<<"Statement[i].splitSequence"<<Statement[i].splitSequence;
                    qDebug()<<"Statement[i].debitCredit"<<Statement[i].debitCredit;
                    qDebug()<<"Statement[i].chargeAmount"<<Statement[i].chargeAmount;
                    qDebug()<<"Statement[i].payoutAmount"<<Statement[i].payoutAmount;
                    qDebug()<<"Statement[i].commissionAmount"<<Statement[i].commissionAmount;
                    qDebug()<<"Statement[i].tdsAmount"<<Statement[i].tdsAmount;
                    qDebug()<<"Statement[i].originalTxnSequence"<<Statement[i].originalTxnSequence;
                    qDebug()<<"Statement[i].parentTxnId"<<Statement[i].parentTxnId;
                    qDebug()<<"Statement[i].parentTxnAmount"<<Statement[i].parentTxnAmount;
                    qDebug()<<"Statement[i].parentTxnAmountCurrent"<<Statement[i].parentTxnAmountCurrent;
                    qDebug()<<"Statement[i].chargeAmountCurrent"<<Statement[i].chargeAmountCurrent;
                    qDebug()<<"Statement[i].transactionTypeName"<<Statement[i].transactionTypeName;
                    qDebug()<<"Statement[i].TransactionTypeProduct"<<Statement[i].TransactionTypeProduct;
                    qDebug()<<"Statement[i].TransactionTypeDescription"<<Statement[i].TransactionTypeDescription;
                    qDebug()<<"Statement[i].Status"<<Statement[i].Status;
                    qDebug()<<"Statement[i].rfuDateDesc"<<Statement[i].endingbalance;
                    qDebug()<<"Statement[i].rfuDateDesc"<<Statement[i].rfuDateDesc;
                    qDebug()<<"Statement[i].rfuDateVal"<<Statement[i].rfuDateVal;
                    qDebug()<<"Statement[i].rfuNumeric1Desc"<<Statement[i].rfuNumeric1Desc;
                    qDebug()<<"Statement[i].rfuNumeric1Val"<<Statement[i].rfuNumeric1Val;
                    qDebug()<<"Statement[i].rfuNumeric2Desc"<<Statement[i].rfuNumeric2Desc;
                    qDebug()<<"Statement[i].rfuNumeric2Val"<<Statement[i].rfuNumeric2Val;
                    qDebug()<<"Statement[i].rfuNumeric3Desc"<<Statement[i].rfuNumeric3Desc;
                    qDebug()<<"Statement[i].rfuNumeric3Val"<<Statement[i].rfuNumeric3Val;
                    qDebug()<<"Statement[i].rfuNumeric4Desc"<<Statement[i].rfuNumeric4Desc;
                    qDebug()<<"Statement[i].rfuNumeric4Val"<<Statement[i].rfuNumeric4Val;
                    qDebug()<<"Statement[i].rfuText1Desc"<<Statement[i].rfuText1Desc;

                    qDebug()<<"Statement[i].rfuText1Val"<<Statement[i].rfuText1Val;
                    qDebug()<<"Statement[i].rfuText2Desc"<<Statement[i].rfuText2Desc;

                    qDebug()<<"Statement[i].rfuText2Val"<<Statement[i].rfuText2Val;
                    qDebug()<<"Statement[i].rfuText3Desc"<<Statement[i].rfuText3Desc;
                    qDebug()<<"Statement[i].rfuText3Val"<<Statement[i].rfuText3Val;
                    qDebug()<<"Statement[i].rfuText4Desc"<<Statement[i].rfuText4Desc;
                    qDebug()<<"Statement[i].rfuText4Val"<<Statement[i].rfuText4Val;
                    qDebug()<<"Statement[i].rfuText5Desc"<<Statement[i].rfuText5Desc;
                    qDebug()<<"Statement[i].rfuText5Val"<<Statement[i].rfuText5Val;
                    qDebug()<<"Statement[i].rfuText6Desc"<<Statement[i].rfuText6Desc;
                    qDebug()<<"Statement[i].rfuText6Val"<<Statement[i].rfuText6Val;
                    qDebug()<<"Statement[i].rfuText7Desc"<<Statement[i].rfuText7Desc;
                    qDebug()<<"Statement[i].rfuText7Val"<<Statement[i].rfuText7Val;
                    qDebug()<<"Statement[i].rfuText8Desc"<<Statement[i].rfuText8Desc;
                    qDebug()<<"Statement[i].rfuText8Val"<<Statement[i].rfuText8Val;
                    qDebug()<<"Statement[i].rfuText9Desc"<<Statement[i].rfuText9Desc;
                    qDebug()<<"Statement[i].rfuText9Val"<<Statement[i].rfuText9Val;
                    qDebug()<<"Statement[i].rfuText10Desc"<<Statement[i].rfuText10Desc;
                    qDebug()<<"Statement[i].rfuText10Val"<<Statement[i].rfuText10Val;
                    qDebug()<<"Statement[i].debitAmount"<<Statement[i].debitAmount;
                    qDebug()<<"Statement[i].creditAmount"<<Statement[i].creditAmount;
                    qDebug()<<"Statement[i].cGST"<<Statement[i].cGST;
                    qDebug()<<"Statement[i].sGST"<<Statement[i].sGST;
                    qDebug()<<"Statement[i].iGST"<<Statement[i].iGST;
                    qDebug()<<"Statement[i].uGST"<<Statement[i].uGST;

#endif


                }

            }
            return FUNC_OK;
        }
        else
        {

            return FUNC_ERROR;
        }
    }


}

void FPB::on_PushButton_Back_pg26_2_clicked()
{
    if(TRANS_TYPE == MINISTATEMENT)
    {
        free(MiniStatement);
        TRANS_TYPE = -1;
        ui->stackedWidget->setCurrentIndex(15);
        return;
    }
    else
    {
        ui->stackedWidget->setCurrentIndex(12);
        return;
    }
}





int FPB::iReq_SecretQuestion()
{
    char* jsondata;
    char buff1[100];
    char Message[50 + 1];
    memset(Message,0,sizeof(Message));
    int iRet = 0;
    char ErrorMsg[100];
    CMisc miscObj;
    Json1::Value mainreq;
    uidmessage::instance()->hide_Message();
    uidmessage::instance()->Display_BusyMessage("Please Wait...");
    iGenerateRequestID((char *)username,(unsigned char *)buff1);

    mainreq["RequestId"] =  buff1;//"TLR1032_10112018202736391";
    mainreq["MethodId"] = 15;
    mainreq["TellerID"]= username;
    mainreq["SessionId"] =  (char *)configLoginLeg2.ucSessionId ;//buff_tmp.toAscii().data();//"9d044213-09d9-400d-aa96-5e9a473ba494";
    mainreq["RequestData"] = "";
    mainreq["TokenId"]= "";
    mainreq["IsEncrypt"]= false;
    mainreq["X_Auth_Token"] ="";

    jsondata = Convertstr(mainreq.toStyledString());
#if DEBUG
    qDebug()<<"------------->"<<jsondata;
#endif
    memset(gSendData,0,sizeof(gSendData));
    strncpy((char*)gSendData, jsondata, strlen(jsondata));
#if DEBUG
    printf("gSendData = %s\n\n",gSendData);
#endif
    //-------------------------------------------------------------

#ifdef UAT_HTTP     // File Download

    uidmessage::instance()->hide_Message();
    uidmessage::instance()->Display_BusyMessage("Connecting server");
    sleep(1);
    uidmessage::instance()->hide_Message();
    hComm.close_connection();
    res = hComm.open_connect(SERVERIP,SERVERPORT);
    if(res != 0)
    {
        qDebug() << "Failed to connect to the Server";
        uidmessage::instance()->hide_Message();
        uidmessage::instance()->DisplayMessage("Server Not Connected");
        qDebug()<<"FUNCTION_FAILED";
        return FUNCTION_FAILED ;
    }


    sleep(1);
    uidmessage::instance()->hide_Message();
    uidmessage::instance()->Display_BusyMessage("Transmitting ..");


    int len_gSendData = strlen((char *)gSendData);
    memset(oBuff,0,sizeof(oBuff));
    if((hComm.host_2Sendrecv((unsigned char *)gSendData,len_gSendData,( unsigned char*)oBuff,(int *)&oLen,(int)response_timeout,0))!=SUCCESS)
    {
        printf("No Response From Server\n");
        hComm.close_connection();
        miscObj.DisplayMsgBox((char *)"Response Time-Out");
        sleep(2);
        qDebug()<<"***   Response Time-Out   ***";
        uidmessage::instance()->hide_Message();
        //return SOCKET_RESPONSE_FAILED;
        return FUNCTION_FAILED;
    }

    hComm.close_connection();

#else     // File Download full condition
#if DEBUG
    printf("gSendData = %s\n\n",gSendData);
#endif
    memset(str_url,0,sizeof(str_url));
#ifdef PRODUCTION
    strcpy(str_url,URL_CREATION(LOGIN_URL_PROD));
#else
    strcpy(str_url,URL_CREATION((char *)LOGIN_URL));
#endif

    int retk = file_download(str_url, (char *)gSendData,"/usr/FINO/FPB/RESP/OUTPUT.TXT",120);
    if(retk!=0)
    {
        sleep(1);
        uidmessage::instance()->hide_Message();
        return FUNCTION_FAILED;
    }

#endif

#if DEBUG
    printf("\noBuff = %s\n",oBuff);
#endif


    iRet = iReq_SecretQuestion_parsing();
    if(iRet == FUNC_OK)
    {
        uidmessage::instance()->hide_Message();
        for(int i =0 ; i<20;i++)
        {
            char Display[100];
            memset(Display,0,strlen(Display));
            if(strlen(Secret_question[i].question)>36)
            {
                memcpy(Display,Secret_question[i].question,35);
                strcat(Display," \n");
                strcat(Display,&Secret_question[i].question[35]);
            }
            else
            {
                strcpy(Display,Secret_question[i].question);
            }
            ui->secretQuestion_combo->addItem(Display);
        }
        SecretQue_count = 1;
        sprintf(Message,"Secret Question %d/3",SecretQue_count);
        ui->lbl_SecretQuestion_attempt->setText(Message);
        ui->secretQuestion_combo->setCurrentIndex(0);
        return FUNC_OK;
    }
    else
    {

        if(strcmp((char *)Sec_Resp.cDResponseMessage,"Session Expired.")==0)
        {
            uidmessage::instance()->hide_Message();
            sprintf(ErrorMsg,"%s",(char *)Sec_Resp.cDResponseMessage);
            miscObj.DisplayMsgBox(ErrorMsg);
            uidmessage::instance()->hide_Message();
            free(Statement);
            return SESSION_EXPIRED_FAILED;
        }
        sprintf(ErrorMsg,"%s",/*(char *)OTP.ucResponseCode,*/(char *)Sec_Resp.cDResponseMessage);
        miscObj.DisplayMsgBox(ErrorMsg);
        uidmessage::instance()->hide_Message();
        free(Statement);
        return FUNCTION_FAILED;

    }
}

int FPB::iReq_SecretQuestion_parsing()
{
    char cResCode[10+1];
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
        return FUNCAL_FAILED;
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
        strcpy(Sec_Resp.cDResponse_code,Convertstr(root["ResponseCode"].asString()));

        strcpy(Sec_Resp.cDResponseMessage,Convertstr(root["ResponseMessage"].asString()));
        if(memcmp(Sec_Resp.cDResponse_code,"0",1)==0)
        {
            Json1::Value ResponseData;
            ResponseData.clear();
            ResponseData = root.get("ResponseData",0);
            printf("\nSize : %d\n",ResponseData.size());
            if(ResponseData.size()==0)
            {
                qDebug("\n1:Key Register Data Not Found\n");
                return FUNCAL_FAILED;
            }
            else
            {
                Json1::Value ResponseData1;
                ResponseData1 = ResponseData.get("secretQuestionList",0);
                Secret_question =(Secret_Quest*) malloc(sizeof(Secret_Quest)*200);
                //qDebug()<<"ResponseData1 "<< ResponseData1;
                for(int j=0;j<ResponseData1.size();j++)
                {
                    strcpy(Secret_question[j].cQuestion_id,Convertstr(ResponseData1[j]["questionId"].asString()));
                    strcpy(Secret_question[j].question,Convertstr(ResponseData1[j]["questionDesc"].asString()));
                    strcpy(Secret_question[j].cQuestion_group,Convertstr(ResponseData1[j]["questionGroup"].asString()));
#if DEBUG
                    qDebug()<<"j"<<j;
                    qDebug()<<"Question_id:"<<Secret_question[j].cQuestion_id;
                    qDebug()<<"question:"<<Secret_question[j].question;
                    qDebug()<<"question_group:"<<Secret_question[j].cQuestion_group;
#endif
                }
            }
        }
        else
        {
            return FUNC_ERROR;
        }
    }
    return FUNC_OK;
}

void FPB::on_btn_SecretQuestion_submit_clicked()
{
    char Message[50 + 1];
    memset(Message,0,sizeof(Message));
    int cb;
    CMisc miscObj;
    int iRet = 0;
    if(SecretQue_count == 1)
    {
        memset(cDispalyQue,0,sizeof(cDispalyQue));
        cb = ui->secretQuestion_combo->currentIndex();
        strcpy(selected_que[0].cSel_Question_id,Secret_question[cb].cQuestion_id);
        strcpy(selected_que[0].cSel_question,Secret_question[cb].question);
        strcpy(selected_que[0].cSel_Question_group,Secret_question[cb].cQuestion_group);
        strcpy(selected_que[0].cSel_Answer,ui->SecretAnswer->text().trimmed().toAscii().data());
        secretanswer_encryption((char *)OTP.ucencrypkey,selected_que[0].cSel_Answer,selected_que[0].cenc_Answer);

#if DEBUG
        qDebug()<<"selected_que[0].cSel_question:"<<selected_que[0].cSel_question;
        qDebug()<<"selected_que[0].cSel_question:"<<OTP.ucencrypkey;
        qDebug()<<"selected_que[0].cSel_question:"<<selected_que[0].cSel_question;
        qDebug()<<"selected_que[0].cSel_question:"<<selected_que[0].cenc_Answer;
#endif
        if(ui->SecretAnswer->text().trimmed().isEmpty())
        {
            miscObj.DisplayMsgBox((char *)"Answer is mandatory");
            memset(&selected_que[0],0,sizeof(selected_que[0]));
            SecretQue_count = 1;
            ui->stackedWidget->setCurrentIndex(44);
            return;
        }
        SecretQue_count = 2;
        sprintf(Message,"Secret Question %d/3",SecretQue_count);
        ui->lbl_SecretQuestion_attempt->setText(Message);
        ui->SecretAnswer->clear();
        sprintf(cDispalyQue,"1.");
        if(strlen(selected_que[0].cSel_question)>37)
        {
            strncat(cDispalyQue,Secret_question[cb].question,36);
            strcat(cDispalyQue,"\n");
            strcat(cDispalyQue,&Secret_question[cb].question[36]);
        }
        else
        {
            strcpy(cDispalyQue,Secret_question[cb].question);
        }
        ui->secret_question_display->setText(cDispalyQue);
        ui->stackedWidget->setCurrentIndex(44);
        return;
    }
    else if(SecretQue_count == 2)
    {

        cb = ui->secretQuestion_combo->currentIndex();
        strcpy(selected_que[1].cSel_Question_id,Secret_question[cb].cQuestion_id);
        strcpy(selected_que[1].cSel_question,Secret_question[cb].question);
        strcpy(selected_que[1].cSel_Question_group,Secret_question[cb].cQuestion_group);
        strcpy(selected_que[1].cSel_Answer,ui->SecretAnswer->text().trimmed().toAscii().data());
        secretanswer_encryption((char *)OTP.ucencrypkey,selected_que[1].cSel_Answer,selected_que[1].cenc_Answer);
#if DEBUG
        qDebug()<<"selected_que[1].cSel_question:"<<selected_que[1].cSel_question;
#endif
        if(ui->SecretAnswer->text().trimmed().isEmpty())
        {
            miscObj.DisplayMsgBox((char *)"Answer is mandatory");
            memset(&selected_que[1],0,sizeof(selected_que[1]));
            SecretQue_count = 2;
            ui->stackedWidget->setCurrentIndex(44);
            return;
        }
        if(strcmp(selected_que[1].cSel_Question_id,selected_que[0].cSel_Question_id)==0)
        {
            miscObj.DisplayMsgBox((char *)"Question already selected");
            memset(&selected_que[1],0,sizeof(selected_que[1]));
            SecretQue_count = 2;
            ui->stackedWidget->setCurrentIndex(44);
            return;
        }
        SecretQue_count = 3;
        sprintf(Message,"Secret Question %d/3",SecretQue_count);
        ui->lbl_SecretQuestion_attempt->setText(Message);
        ui->SecretAnswer->clear();
        strcat(cDispalyQue,"\n2.");
        if(strlen(selected_que[1].cSel_question)>37)
        {
            strncat(cDispalyQue,Secret_question[cb].question,36);
            strcat(cDispalyQue,"\n");
            strcat(cDispalyQue,&Secret_question[cb].question[36]);
        }
        else
        {
            strcat(cDispalyQue,Secret_question[cb].question);
        }
        ui->secret_question_display->setText(cDispalyQue);
        ui->stackedWidget->setCurrentIndex(44);
        return;
    }
    else if(SecretQue_count == 3)
    {
        cb = ui->secretQuestion_combo->currentIndex();
        strcpy(selected_que[2].cSel_Question_id,Secret_question[cb].cQuestion_id);
        strcpy(selected_que[2].cSel_question,Secret_question[cb].question);
        strcpy(selected_que[2].cSel_Question_group,Secret_question[cb].cQuestion_group);
        strcpy(selected_que[2].cSel_Answer,ui->SecretAnswer->text().trimmed().toAscii().data());
        secretanswer_encryption((char *)OTP.ucencrypkey,selected_que[2].cSel_Answer,selected_que[2].cenc_Answer);

        if(ui->SecretAnswer->text().trimmed().isEmpty())
        {
            miscObj.DisplayMsgBox((char *)"Answer is mandatory");
            memset(&selected_que[2],0,sizeof(selected_que[2]));
            SecretQue_count = 3;
            ui->stackedWidget->setCurrentIndex(44);
            return;
        }


        if(strcmp(selected_que[1].cSel_Question_id,selected_que[2].cSel_Question_id)==0  || strcmp(selected_que[2].cSel_Question_id,selected_que[0].cSel_Question_id)==0)
        {
            miscObj.DisplayMsgBox((char *)"Question already selected");
            memset(&selected_que[2],0,sizeof(selected_que[2]));
            SecretQue_count = 3;
            ui->stackedWidget->setCurrentIndex(44);
            return;
        }
        sprintf(Message,"Secret Question %d/3",SecretQue_count);
        strcat(cDispalyQue,"\n3.");
        if(strlen(selected_que[2].cSel_question)>37)
        {
            strncat(cDispalyQue,Secret_question[cb].question,36);
            strcat(cDispalyQue,"\n");
            strcat(cDispalyQue,&Secret_question[cb].question[36]);
        }
        else
        {
            strcat(cDispalyQue,Secret_question[cb].question);
        }
#if DEBUG
        qDebug()<<"selected_que[2].cSel_question:"<<selected_que[2].cSel_question;
#endif


        free(Secret_question);
        // sprintf(cDispalyQue,"1. %s\n2. %s\n3. %s",selected_que[0].cSel_question,selected_que[1].cSel_question,selected_que[2].cSel_question);
        ui->Submit_diplsy_question->setText(cDispalyQue);
        ui->Submit_questrion_number->setText(Message);
        ui->stackedWidget->setCurrentIndex(45);
        return;
    }
}



int FPB::iReq_SecretQuestion_Submit()
{
    char* jsondata;
    char buff1[100];
    char Message[50 + 1];
    QString buff_tmp;
    memset(Message,0,sizeof(Message));
    int iRet = 0;
    Json1::Value mainreq;
    Json1::Value ArrayList(Json1::arrayValue);
    Json1::Value lstOfUserSecretQues,RequestedData;
    uidmessage::instance()->hide_Message();
    uidmessage::instance()->Display_BusyMessage("Please Wait ...");
    iGenerateRequestID((char *)username,(unsigned char *)buff1);
    char ErrorMsg[100];
    CMisc miscObj;
    mainreq["RequestId"] =  buff1;//"TLR1032_10112018202736391";
    mainreq["MethodId"] = 9;
    mainreq["TellerID"]= username;
    mainreq["SessionId"] =  (char *)configLoginLeg2.ucSessionId ;//buff_tmp.toAscii().data();//"9d044213-09d9-400d-aa96-5e9a473ba494";
    mainreq["TokenId"]= "";
    mainreq["IsEncrypt"]= false;
    ArrayList.clear();
    for(int i = 0;i<3;i++)
    {
        lstOfUserSecretQues["userId"] = username;
        lstOfUserSecretQues["questionGroup"] = selected_que[i].cSel_Question_group;
        lstOfUserSecretQues["questionId"] = selected_que[i].cSel_Question_id;
        lstOfUserSecretQues["answer"] = selected_que[i].cenc_Answer;
        ArrayList.append(lstOfUserSecretQues);
    }

    RequestedData["lstOfUserSecretQues"] =ArrayList;
    buff_tmp = serialize1(Convertstr(RequestedData.toStyledString()));
    mainreq["RequestData"] = buff_tmp.toAscii().data();
    mainreq["SessionExpiryTime"] ="";
    mainreq["X_Auth_Token"] ="";

    jsondata = Convertstr(mainreq.toStyledString());

#if DEBUG
    qDebug()<<"------------->"<<jsondata;
#endif
    memset(gSendData,0,sizeof(gSendData));
    strncpy((char*)gSendData, jsondata, strlen(jsondata));
#if DEBUG
    printf("gSendData = %s\n\n",gSendData);
#endif
    //-------------------------------------------------------------

#ifdef UAT_HTTP     // File Download

    uidmessage::instance()->hide_Message();
    uidmessage::instance()->Display_BusyMessage("Connecting server");
    sleep(1);
    uidmessage::instance()->hide_Message();
    hComm.close_connection();
    res = hComm.open_connect(SERVERIP,SERVERPORT);
    if(res != 0)
    {
        qDebug() << "Failed to connect to the Server";
        uidmessage::instance()->hide_Message();
        uidmessage::instance()->DisplayMessage("Server Not Connected");
        qDebug()<<"FUNCTION_FAILED";
        return FUNCTION_FAILED ;
    }


    sleep(1);
    uidmessage::instance()->hide_Message();
    uidmessage::instance()->Display_BusyMessage("Transmitting ..");


    int len_gSendData = strlen((char *)gSendData);
    memset(oBuff,0,sizeof(oBuff));
    if((hComm.host_2Sendrecv((unsigned char *)gSendData,len_gSendData,( unsigned char*)oBuff,(int *)&oLen,(int)response_timeout,0))!=SUCCESS)
    {
        printf("No Response From Server\n");
        hComm.close_connection();
        miscObj.DisplayMsgBox((char *)"Response Time-Out");
        sleep(2);
        qDebug()<<"***   Response Time-Out   ***";
        uidmessage::instance()->hide_Message();
        //return SOCKET_RESPONSE_FAILED;
        return FUNCTION_FAILED;
    }

    hComm.close_connection();

#else     // File Download full condition
#if DEBUG
    printf("gSendData = %s\n\n",gSendData);
#endif
    memset(str_url,0,sizeof(str_url));
#ifdef PRODUCTION
    strcpy(str_url,URL_CREATION(LOGIN_URL_PROD));
#else
    strcpy(str_url,URL_CREATION((char *)LOGIN_URL));
#endif

    int retk = file_download(str_url, (char *)gSendData,"/usr/FINO/FPB/RESP/OUTPUT.TXT",120);
    if(retk!=0)
    {
        sleep(1);
        uidmessage::instance()->hide_Message();
        return FUNCTION_FAILED;
    }

#endif

#if DEBUG
    printf("\noBuff = %s\n",oBuff);
#endif

    iRet = iReq_SecretQuestion_Submit_Parse();
    if(iRet == FUNC_OK)
    {
        uidmessage::instance()->hide_Message();
        sprintf(ErrorMsg,"%s",/*(char *)OTP.ucResponseCode,*/(char *)Sec_Resp.cDResponseMessage);
        miscObj.DisplayMsgBox(ErrorMsg);
        uidmessage::instance()->hide_Message();
        return FUNCTION_SUCCESS;
    }
    else
    {

        if(strcmp((char *)Sec_Resp.cDResponseMessage,"Session Expired.")==0)
        {
            uidmessage::instance()->hide_Message();
            sprintf(ErrorMsg,"%s",(char *)Sec_Resp.cDResponseMessage);
            miscObj.DisplayMsgBox(ErrorMsg);
            uidmessage::instance()->hide_Message();
            free(Secret_question);
            return SESSION_EXPIRED_FAILED;
        }
        sprintf(ErrorMsg,"%s",/*(char *)OTP.ucResponseCode,*/(char *)Sec_Resp.cDResponseMessage);
        miscObj.DisplayMsgBox(ErrorMsg);
        uidmessage::instance()->hide_Message();
        free(Secret_question);
        return FUNCTION_FAILED;

    }

}

int FPB::iReq_SecretQuestion_Submit_Parse()
{
    system("sed 's/\"ResponseData\":\"{/\"ResponseData\":{/g' /usr/FINO/FPB/RESP/OUTPUT.TXT > /tmp/1.txt");
    system("sed 's/}\",\"MessageId\"/},\"MessageId\"/g' /tmp/1.txt > /tmp/2.txt  ");
    system(" sed 's/\\\\//g' /tmp/2.txt > /usr/FINO/FPB/RESP/OUTPUT.TXT");
    memset(&Sec_Resp,0,sizeof(Sec_Resp));
    Json1::Value root;
    root.clear();
    Json1::Reader reader;
    std::ifstream ReadData("/usr/FINO/FPB/RESP/OUTPUT.TXT");
    bool parsedSuccess = reader.parse(ReadData,root,false);

    ReadData.close();

    if(not parsedSuccess)
    {

        qDebug()<<"FpsMonthlyPdsData File not Found";
        return FUNCAL_FAILED;
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
        strcpy(Sec_Resp.cDResponse_code,Convertstr(root["ResponseCode"].asString()));
        strcpy(Sec_Resp.cDResponseMessage,Convertstr(root["ResponseMessage"].asString()));
        if(memcmp(Sec_Resp.cDResponse_code,"0",1)==0)
        {
            return FUNC_OK;
        }
        else
        {
            return FUNC_ERROR;
        }
    }

}


void FPB::on_btn_SecretQuestion_submit_2_clicked()
{
    int iRet =-2;
    iRet = iReq_SecretQuestion_Submit();
    if(iRet == FUNCTION_SUCCESS)
    {
        Flag_FirstTime_Login = 1;
        memset(&OTP,0,sizeof(OTP));
        memset(&sndchange,0,sizeof(sndchange));
        uidmessage::instance()->hide_Message();
        OTP_page = 6;
        iRet =-2;
        iRet = Prepare_encryption_Func();
        uidmessage::instance()->hide_Message();
        if(iRet != FUNCTION_SUCCESS)
        {
            ui->stackedWidget->setCurrentIndex(0);
            return;
        }
        else if(iRet == FUNCTION_SUCCESS)
        {
            uidmessage::instance()->hide_Message();
            ui->Old_password_edit_pg26->clear();
            ui->New_password_edit_pg26->clear();
            ui->Confirm_password_edit_pg26->clear();
            ui->stackedWidget->setCurrentIndex(26);
            return;
        }

    }
    else
    {
        ui->stackedWidget->setCurrentIndex(0);
        return;
    }

}


void FPB::on_Addquest_backbutton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    return;
}



void FPB::on_Submit_backbutton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    return;
}



int FPB::Forget_question_parsing()
{
    system("sed 's/\"ResponseData\":\"{/\"ResponseData\":{/g' /usr/FINO/FPB/RESP/OUTPUT.TXT > /tmp/1.txt");
    system("sed 's/}\",\"MessageId\"/},\"MessageId\"/g' /tmp/1.txt > /tmp/2.txt  ");
    system(" sed 's/\\\\//g' /tmp/2.txt > /usr/FINO/FPB/RESP/OUTPUT.TXT");
    memset(&secret_data,0,sizeof(secret_data));
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
        return FUNCAL_FAILED;
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
        strcpy((char *)FORPASS_State.ucResponseCode,Convertstr(root["ResponseCode"].asString()));
        strcpy((char *)FORPASS_State.ucResponseMessage,Convertstr(root["ResponseMessage"].asString()));



        if((char *)memcmp(FORPASS_State.ucResponseCode,"0",1)==0)
        {
            Json1::Value ResponseData;
            ResponseData.clear();
            ResponseData = root.get("ResponseData",0);
            printf("\nSize : %d\n",ResponseData.size());
            if(ResponseData.size()==0)
            {
                qDebug("\n1:Key Register Data Not Found\n");
                return FUNCAL_FAILED;
            }
            else
            {
                Json1::Value ResponseData1;
                ResponseData1 = ResponseData.get("userSecretQuestionsDtls",0);

                CountSQ = ResponseData1.size();
                //qDebug()<<"ResponseData1 "<< ResponseData1;
                for(int j=0;j<ResponseData1.size();j++)
                {
                    strcpy((char *)secret_data[j].questionId,Convertstr(ResponseData1[j]["questionId"].asString()));
                    strcpy((char *)secret_data[j].questionDesc,Convertstr(ResponseData1[j]["questionDesc"].asString()));
                    strcpy((char *)secret_data[j].questionGroup,Convertstr(ResponseData1[j]["questionGroup"].asString()));
#if DEBUG
                    qDebug()<<"j"<<j;
                    qDebug()<<"Question"<<Convertstr(ResponseData1[j]["questionDesc"].asString());
                    qDebug()<<"Question_id:"<<secret_data[j].questionId;
                    qDebug()<<"question:"<<secret_data[j].questionDesc;
                    qDebug()<<"question_group:"<<secret_data[j].questionGroup;
#endif
                }
            }
            return FUNC_OK;
        }
        else
        {
            return FUNC_ERROR;
        }
    }
    return FUNC_OK;
}
