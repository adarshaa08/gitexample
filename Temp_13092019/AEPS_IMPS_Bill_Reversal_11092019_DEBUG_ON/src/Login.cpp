#include "FPB.h"
#include "ui_FPB.h"
#include "displaymessage.h"
#include "Login.h"
#include "json.h"
#include "HostComm.h"
#include "userprofile.h"
#include "findcust.h"
#include <QMovie>
#include<fstream>
//#include "aepsbalance.h"
//#include "aepsmini.h"
//#include "aepswthdraw.h"
//#include "CHeader.h"
//unsigned char gSendData[1024*30]; //Used to send data through network
//unsigned char Recvdata[1024*50]; //Used to receive
char oBuff[25*1024];  // File Download

char oBuff2[10000]; // File Download
char str_url[500+1];
int response_timeout = 0;
int Flag_FirstTime_Login = 0;
int Flag_eAgreement_Login = 0;
int Flag_UpdateApp = 0;
unsigned char static_key[50];
//Login_leg1 configLoginLeg1;
//Login_leg2 configLoginLeg2;
unsigned char TagBuffer[4000]; //Used to store intermediate JSON data
unsigned char gSendData[1024*30]; //Used to send data through network
unsigned char Recvdata[1024*50]; //Used to receive
unsigned char JSONTagBuff[5000];
unsigned char  BMRecvdata[1024*50]; // Temp BM
char Mer_Latest_Balance[30];
char Balance_Display[25];
char cTer_Sl_No[20+1];
extern char * Convertstr(string str);
extern "C" char genRandom();
Login_leg1 configLoginLeg1;
Login_leg2 configLoginLeg2;
extern FINDCUSTOMER FINDCUST;
extern FOR_OTP OTP;
extern NewPass sndnewpass;
extern ChangePass sndchange;
extern int OTP_page;
//extern AEPS_MINISTATEMENT AEPSMini_State;
//extern AEPS_BALENQ AEPSBALENQ;
//extern AEPS_WITHDRAWAL AEPSWITH;
extern Transaction Txn;
Change_Device_OTP snddevice;

extern "C" int uid_aes_encrypt(unsigned char *in, int inlen, unsigned char *out, int *outlen, unsigned char *key, unsigned char *iv);

void FPB::clear_fun(void)
{
    memset(username,0,sizeof(username));
    memset(&configLoginLeg1,0,sizeof(configLoginLeg1));
    memset(&configLoginLeg2,0,sizeof(configLoginLeg2));
    //    memset(&AEPSBALENQ,0,sizeof(AEPSBALENQ));
    //    memset(&AEPSMini_State,0,sizeof(AEPSMini_State));
    //    memset(&AEPSWITH,0,sizeof(AEPSWITH));
    memset(gSendData,0,sizeof(gSendData));
    memset(Recvdata,0,sizeof(Recvdata));
    memset(&Txn,0,sizeof(Txn));
    ui->lineEdit_userid->clear();
    ui->lineEdit_password->clear();
    system("rm /usr/FINO/FPB/REQ/LEG2_REQ.TXT");
    system("rm /usr/FINO/FPB/REQ/LEG1_REQ.TXT");

}

//void FPB::on_pushButton_4_clicked()
//{
//    //    ();
//    //    int iret = 0;
//    //    clear_fun();
//    //    iret = Check_Conditions();
//    //    CMisc miscObj;

//    //    if((check_RD_connection() == FUNCTION_FAILED) && (miscObj.GPRS_isAvailable() == true))
//    //    {
//    //        sleep(1);
//    //        uidmessage::instance()->hide_Message();
//    //    }
//    //    else
//    //    {
//    //        //        if(miscObj.GPRS_isAvailable() == true)
//    //        //            uidmessage::instance()->Display_BusyMessage("Connected to RD service");
//    //        //        sleep(1);
//    //        //        uidmessage::instance()->hide_Message();
//    //    }
//    //    ui->lineEdit_userid->setFocus();
//    //    ui->stackedWidget->setCurrentIndex(0);

//    //    return;
//}

void FPB::on_pushButton_clicked()
{
    // ui->Eagreement_IAccept_Button_pg27->setEnabled(false);
    //  ui->Eagreement_IAccept_Button_pg27->setStyleSheet("background-color: rgb(85, 85, 127); color: rgb(107, 107, 107)");
    int iRet = -2;
    ui->lineEdit_password->clear();
    //ui->Forget_button->setHidden(true);
    char ver[20];
    memset(ver, 0, sizeof(ver));
    sprintf(ver, "Ver : %s",APP_VERSION);
    ui->AppVersion_Label_2->setText(ver);
    //ui->AppVersion_Label_2->setText(APP_VERSION);
    memset(username,0,sizeof(username));
    iRet = iLoginpage();
    uidmessage::instance()->hide_Message();
    if(iRet == OTP_PAGE)
    {
        ui->OTP_editline->clear();
        ui->stackedWidget->setCurrentIndex(31);
    }
    else if(iRet != FUNCTION_SUCCESS)
    {
        memset(username,0,sizeof(username));
        ui->lineEdit_userid->text().clear();
        ui->stackedWidget->setCurrentIndex(0);
    }
    else if(iRet == FUNCTION_SUCCESS)
    {
        ui->lineEdit_userid_2->setText(ui->lineEdit_userid->text().toAscii().data());
        ui->lineEdit_userid_2->setEnabled(false);
        ui->pushButton_2->setFocus();
        ui->lineEdit_password->setFocus();
        ui->stackedWidget->setCurrentIndex(1);
    }
    return;
}

void FPB::on_pushButton_2_clicked()
{
    int iRet = -2;
    iRet = iPasswordpage();
    if(iRet != FUNCTION_SUCCESS && iRet != FUNC_OK)
    {
        uidmessage::instance()->hide_Message();
        ui->lineEdit_userid->clear();
        ui->lineEdit_userid->clear();
        if(iRet == FUNC_BACK)
            ui->stackedWidget->setCurrentIndex(1);
        else
            ui->stackedWidget->setCurrentIndex(0);
    }
    else if(iRet == FUNC_OK)
    {
        ui->stackedWidget->setCurrentIndex(44);
    }
    else if(iRet == FUNCTION_SUCCESS)
    {
        ui->Search_lineEdit->clear();
        if(Flag_FirstTime_Login == 1)
        {
            int iRet =-2;
            //            Flag_FirstTime_Login = 1;
            memset(&OTP,0,sizeof(OTP));
            memset(&sndchange,0,sizeof(sndchange));
            //            qDebug()<<"first time login flag";
            uidmessage::instance()->hide_Message();
            //            ui->stackedWidget->setCurrentIndex(30);
            //            return;



            OTP_page = 2;
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
        if(Flag_eAgreement_Login == 1)
        {
            uidmessage::instance()->hide_Message();
            ui->Eagreement_Merchant_Name_pg27->setText((char *)configLoginLeg2.ucname);
            ui->stackedWidget->setCurrentIndex(27);
            return;
        }

        int i_ldf = login_download_func();
        if(i_ldf == FUNCTION_SUCCESS)
        {
            uidmessage::instance()->hide_Message();
            uidmessage::instance()->Display_BusyMessage("Please Wait...");
            int ilRet = LoginCardAcceptorLocation();
            uidmessage::instance()->hide_Message();
            ui->Search_lineEdit->setFocus(); // 02052019

            if(ilRet != FUNCTION_SUCCESS)
                ui->stackedWidget->setCurrentIndex(0);
            else
                ui->stackedWidget->setCurrentIndex(12);
            return;
        }
        else
        {
            ui->stackedWidget->setCurrentIndex(0);
            return;
        }

    }

    uidmessage::instance()->hide_Message();
    return;
}

int FPB::iLoginpage()
{
    uidmessage::instance()->hide_Message();
    uidmessage::instance()->Display_BusyMessage("Authenticating, please wait...");

    int iRet=-2;
    int gSendData_len =0;
    int usr_len;
    CMisc miscObj;
    FILE *fp1 = NULL;
    int ret=-3, template_len=0;
    unsigned char  *template_data;
    char ErrorMsg[100];
    memset(username, 0, sizeof(username));
    memset(&configLoginLeg1,0,sizeof(configLoginLeg1));

    memcpy(username, (char *)ui->lineEdit_userid->text().trimmed().toAscii().data(), strlen((const char *)ui->lineEdit_userid->text().trimmed().toAscii().data()));
    if(strcmp((char *)username,"") == 0)
    {
        uidmessage::instance()->hide_Message();
        miscObj.DisplayMsgBox((char *)"Please enter the Username");
        ui->lineEdit_userid->clear();
        uidmessage::instance()->hide_Message();
        return FUNCTION_FAILED;

    }
    else if((Stringtrim((char *)username)== -1) || (Stringtrim((char *)username)== -2) || (Stringtrim((char *)username)== -3))//Stringtrim special char condition commented
    {
        uidmessage::instance()->hide_Message();
        miscObj.DisplayMsgBox((char *)"Please Enter Valid Account Number");
        ui->lineEdit_userid->clear();
        uidmessage::instance()->hide_Message();
        return FUNCTION_FAILED;
    }
    else if(strlen((char *)username) < 9)
    {
        uidmessage::instance()->hide_Message();
        miscObj.DisplayMsgBox((char *)"Username must be 9 digit");
        ui->lineEdit_userid->clear();
        uidmessage::instance()->hide_Message();
        return FUNCTION_FAILED;
    }

#if DEBUG
    qDebug()<<"User Name"<<username;
#endif
    usr_len = strlen((char *)username);

    //  uid_aes_encrypt((unsigned char *)"101068318",9,(unsigned char *)encrypt_user,(int *)encrypt_len,(unsigned char *)__DEFAULT_KEY,(unsigned char *)__DEFAULT_IV);
    //    qDebug()<<"encrypt_user"<<encrypt_user<<"encrypt_len"<<encrypt_len;

    unsigned char out_enc[100];
    int out_len = 0;
    memset(out_enc,0,sizeof(out_enc));
    int k = uid_aes_encrypt((unsigned char*)username,strlen((char*)username), out_enc, &out_len, (unsigned char *)__DEFAULT_KEY, (unsigned char *)__DEFAULT_IV);

    int encoutlen1=0;
    unsigned char FINALUSERID[100];
    memset(FINALUSERID,0,sizeof(FINALUSERID));

    uid_encode_b64((unsigned char *)out_enc, out_len, FINALUSERID, &encoutlen1);
    QString ENC_Data ;
    ENC_Data.clear();


    QSqlQuery query;
    QString old_user;
    query.clear();

    // CASH WITHDRAWAL AEPS ONUS

    //    printf("FINALUSERID == %s\n",FINALUSERID);
    ENC_Data = QString::fromUtf8((const char*)FINALUSERID).trimmed();
    memset(FINALUSERID,0,sizeof(FINALUSERID));
    memcpy(FINALUSERID,ENC_Data.toLatin1().data(),strlen(ENC_Data.toLatin1().data()));
    memcpy(configLoginLeg1.ucUser_id_req,(char *)FINALUSERID,strlen((char *)FINALUSERID));//3USgowGll1VaTyufW0i3dg==
    iGenerateRequestID((char *)username,(unsigned char *)&configLoginLeg1.ucRequestID_req);
    memcpy(configLoginLeg1.ucMethodID_req,"20",2);
    memcpy(configLoginLeg1.ucTellerID_req,username,sizeof(username));
    memcpy(configLoginLeg1.ucIsEncrypt_req,"false",5);
    memcpy(configLoginLeg1.ucUser_id_req,configLoginLeg1.ucUser_id_req,24);
    memcpy(configLoginLeg1.ucClient_id_req,CLIENT_ID,strlen(CLIENT_ID));
    memcpy(configLoginLeg1.ucChannelID_req,"null",4); // IMPS
    memcpy(configLoginLeg1.ucProductCode_req,"null",4);
    memcpy(configLoginLeg1.ucAmount_req,"null",4);
    memcpy(configLoginLeg1.ucMerchantContribution_req,"null",4);
    memcpy(configLoginLeg1.ucCharges_req,"null",4);
    memcpy(configLoginLeg1.ucRouteID_req,"null",4);
    memcpy(configLoginLeg1.ucSplitReferenceNo_req,"null",4);
    memcpy(configLoginLeg1.ucServiceID_req,"null",4);
    memcpy(configLoginLeg1.ucIdentifier_req,"null",4);
    memcpy(configLoginLeg1.ucTxnHash_req,"null",4);
    query.clear();
    query.prepare("SELECT LAST_USER_ID from LastData ");
    if(!query.exec())
    {
        qDebug() << query.lastError()<<"data base entry error";
        return FUNC_ERROR;
    }
    else
    {
        while(query.next())
        {
            old_user = query.value(0).toString().trimmed();
        }
    }
    query.finish();
    if(old_user.isEmpty())
    {
        memcpy(configLoginLeg1.ucold_user_id,"null",4);
    }
    else
    {
        strncpy((char *)configLoginLeg1.ucold_user_id,old_user.toAscii().data(),strlen(old_user.toAscii().data()));
        //memcpy(configLoginLeg1.ucold_user_id,old_user.toAscii().data(),sizeof(old_user.toAscii().data()));
    }
    iRet = makeJSONReqLEG1();
    if(iRet!=TRUE)
    {
        qDebug()<<"JSON Creation failed";
        uidmessage::instance()->hide_Message();
        return FUNCTION_FAILED;
    }
    if(!strcmp(ISENCEYPT,"true"))
    {
        memset((char*)static_key,0,sizeof(static_key));
        srand(time(0));
        // std::string Str;
        for(unsigned int i = 0; i < 36; ++i)
        {
            static_key[i] += genRandom();
        }
#if DEBUG
        printf("static_key%s",static_key);
  #endif
        gSendData_len = strlen((char *)gSendData);
        Login_encryption((char *)gSendData,gSendData_len,static_key);
    }

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
    if(!strcmp(ISENCEYPT,"true"))
    {
        strcpy(str_url,URL_CREATION((char *)LOGINAUTH_URL));
    }
    else
    {
        strcpy(str_url,URL_CREATION((char *)LOGIN_URL));

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

    /*    file_download( "https://103.1.112.204/UIService.svc/ProcessLoginRequest", (char *)gSendData,"/usr/FINO/FPB/RESP/LoginCurl1.TXT",50);
    file_download( "https://partner.finopaymentbank.in/UIService.svc/ProcessLoginRequest", (char *)gSendData,"/usr/FINO/FPB/RESP/OUTPUT.TXT",90);*/

#endif

    /****************************Decryption function*********************************/
    if(!strcmp(ISENCEYPT,"true"))
    {
        int oBuff_len = strlen(oBuff);
#if DEBUG
        qDebug()<<"Length of buffer"<<oBuff_len;
        printf("Buffer%s\n",oBuff);
#endif
        LoginDetachEncryptAttach(oBuff,oBuff_len,static_key,1);//TEMPORARY
    }
    /**************************************************************************/
    iRet=json->iParsJSON((unsigned char*)oBuff,(char*)&configLoginLeg1.ucResponseCode,(char*)&configLoginLeg1.ucResponseMessage);
    system("cd /usr/FINO/FPB/RESP/");
    system("rm /usr/FINO/FPB/RESP/OUTPUT.TXT");
    if(iRet!=FUNC_OK)
    {
        // sprintf(ErrorMsg,"Response Code:%s \n Message:%s",(char *)configLoginLeg1.ucResponseCode,(char *)configLoginLeg1.ucResponseMessage);
        memset(ErrorMsg, 0, sizeof(ErrorMsg));
        if((memcmp((char *)configLoginLeg1.ucResponseMessage, "null", 4) == 0) || (memcmp((char *)configLoginLeg1.ucResponseMessage, "", 1) == 0))
        {
            sprintf(ErrorMsg,"%s",(char *)"Service is currently unavailable");
        }
        else
        {
            sprintf(ErrorMsg,"%s",(char *)configLoginLeg1.ucResponseMessage);
        }
        uidmessage::instance()->hide_Message();
        miscObj.DisplayMsgBox(ErrorMsg);
        uidmessage::instance()->hide_Message();
        return FUNCTION_FAILED;
    }

    system("cd /usr/FINO/FPB/REQ/");
    fp1 = fopen("/usr/FINO/FPB/REQ/IntJSON.TXT", "rb"); // open
    if (fp1 == NULL)
    {
        qDebug()<<"Can't Open IntJSON.TXT File";
        uidmessage::instance()->hide_Message();
        //return FUNCTION_FAILED;
        return FUNCTION_FAILED;
    }

    fseek(fp1, 0, SEEK_END);
    template_len = ftell(fp1);
    template_data = (unsigned char *)malloc(template_len);

    if (template_data == NULL)
    {
        fclose(fp1);
        printf("\n in testbio memeory alloc failed");
        uidmessage::instance()->hide_Message();
        //return FUNCTION_FAILED;
        return FUNCTION_FAILED;
    }

    fseek(fp1, 0, SEEK_SET);
    ret=fread(template_data,sizeof(unsigned char ),template_len,fp1);
    if(ret==0)
        fclose(fp1);

    QString temp;
    temp.clear();
    temp = QString::fromUtf8((const char*)template_data);

    if(temp.contains("AuthmanFlag:false",Qt::CaseInsensitive))
    {
        int iotpret;
        temp.clear();
        temp = QString::fromUtf8((const char*)oBuff);
        iotpret = OTP_change_parse(temp);
        system("cd /usr/FINO/FPB/REQ/");
        system("rm /usr/FINO/FPB/REQ/IntJSON.TXT");
        if(iotpret == FUNCTION_SUCCESS)
        {
            return OTP_PAGE;
        }
        else
        {
            return FUNCTION_FAILED;
        }
    }
    else
    {
        iRet = iParsLeg1((unsigned char*)template_data);
        free(template_data);
        system("cd /usr/FINO/FPB/REQ/");
        system("rm /usr/FINO/FPB/REQ/IntJSON.TXT");
        if (iRet != FUNC_OK)
        {
            qDebug()<<"Inside Pardse  error";
            miscObj.DisplayMsgBox("Login Failed .....");
            uidmessage::instance()->hide_Message();
            return FUNCTION_FAILED;
        }
        else if(iRet == FUNC_OK)
        {
            uidmessage::instance()->hide_Message();
            return FUNCTION_SUCCESS;
        }
    }
}

int FPB::iPasswordpage()
{
    QString Potversion,Currentversion,MandatoryVersion;
    int iRet=-2;
    char password[25];
    FILE *fp = NULL;
    char ErrorMsg[100];
    CMisc miscObj;
    int encoutlen1=0;
    unsigned char out_enc[100];
    int out_len = 0;
    int pswd_len;
    char ilatlong[100];
    memset(password,'\0',sizeof(password));
    memset(&configLoginLeg2,0,sizeof(configLoginLeg2));
    int ret = 0;

    memcpy(password,ui->lineEdit_password->text().trimmed().toAscii().data(),strlen(ui->lineEdit_password->text().trimmed().toAscii().data()));
    if(strcmp((char *)password,"") == 0)
    {
        miscObj.DisplayMsgBox((char *)"Please enter the password");
        ui->lineEdit_password->clear();
        return FUNC_BACK;

    }
    else if((Stringtrim((char *)password)== -1) || (Stringtrim((char *)password)== -2) || (Stringtrim((char *)password)== -3))//Stringtrim special char condition commented
    {
        miscObj.DisplayMsgBox((char *)"Please Enter Valid Account Number");
        ui->lineEdit_password->clear();
        return FUNC_BACK;
    }

    uidmessage::instance()->hide_Message();
    uidmessage::instance()->Display_BusyMessage("Login in process,please wait...");
#if DEBUG
    qDebug()<<"User Name"<<password;
#endif
    pswd_len = strlen((char *)password);


    int k = uid_aes_encrypt((unsigned char*)password,strlen((char*)password), out_enc, &out_len, (unsigned char *)__DEFAULT_KEY, (unsigned char *)__DEFAULT_IV);
    unsigned char FINALPASSWORD[100];
    memset(FINALPASSWORD,0,sizeof(FINALPASSWORD));
    uid_encode_b64((unsigned char *)out_enc, out_len, FINALPASSWORD, &encoutlen1);//1bbWOYy0YUtik6tg2BQDsA==
    QString ENC_Data ;
    ENC_Data.clear();
    //   printf("FINALPASSWORD == %s\n",FINALPASSWORD);
    ENC_Data = QString::fromUtf8((const char*)FINALPASSWORD).trimmed();
    memset(FINALPASSWORD,0,sizeof(FINALPASSWORD));
    memcpy(FINALPASSWORD,ENC_Data.toLatin1().data(),strlen(ENC_Data.toLatin1().data()));
#if DEBUG
    printf("username  = %s\n",username);
    printf("Password configLoginLeg1.ucencryption_key  = %s\n",configLoginLeg1.ucencryption_key);
#endif
    iGenerateRequestID((char *)username,(unsigned char *)&configLoginLeg2.ucRequestID_req);

    char arr[20], arr_ip[16];
    memset(arr, 0, sizeof(arr));
    memset(arr_ip, 0, sizeof(arr_ip));
    int imac = get_mac_id(arr, arr_ip);
#if DEBUG
    printf("MAC ID == %s\n",arr);
    printf("IP Address == %s\n",arr_ip);
#endif

    /******************Latitude and longitude**************************///Aadarsha
    memset(ilatlong, 0, sizeof(ilatlong));
    strcpy(ilatlong,get_Lat_long());
    printf("ilatlong == %s\n",ilatlong);
    QString strToFind;
    QString Latitude = QString::fromLocal8Bit(ilatlong);
    QStringList ilatquery = Latitude.split(",");
#if DEBUG
    for(int i=0 ; i < ilatquery.length() ; i++)
        qDebug()<<"Query Response"<<ilatquery.at(i);
#endif
    strToFind.clear();
    QString str = ilatquery.at(0);
    strToFind = ":";
    QString value_latitude = str.mid(str.indexOf(strToFind)+strToFind.length());
    str.clear();

    strToFind.clear();
    str = ilatquery.at(1);
    strToFind = ":";
    QString value_longitude = str.mid(str.indexOf(strToFind)+strToFind.length());

    /***************************************************************************/


    memcpy(configLoginLeg2.ucDeviceID_req,cTer_Sl_No,8);
    memcpy(configLoginLeg2.ucUsername_req,username,strlen((char *)username));
    memcpy(configLoginLeg2.ucMethodID_req,"2",1);
    memcpy(configLoginLeg2.ucTellerID_req,configLoginLeg2.ucUsername_req,strlen((char *)configLoginLeg2.ucUsername_req));
    memcpy(configLoginLeg2.ucIsEncrypt_req,"false",5);
    memcpy(configLoginLeg2.ucUser_id_req,configLoginLeg1.ucUser_id_req,strlen((char *)configLoginLeg1.ucUser_id_req));
    memcpy(configLoginLeg2.ucfingerid_req,"1",1);
    sprintf((char *)configLoginLeg2.ucPassword_req,"%s|%s",configLoginLeg1.ucencryption_key,FINALPASSWORD);//PtRBW+X4voniatuyPRYnUg== -> Nov@2018
    memcpy(configLoginLeg2.ucClient_id_req,CLIENT_ID,strlen(CLIENT_ID));	//1bbWOYy0YUtik6tg2BQDsA==  Querty@11    pVX1vV4YRzlC6KaKSQydwA==  Bank@1234
    memcpy(configLoginLeg2.ucChannel_req,CHANNEL_ID,strlen(CHANNEL_ID)); 			//1bbWOYy0YUtik6tg2BQDsA== Qwerty@11

    memcpy(configLoginLeg2.ucChannelID_req,CHANNEL_ID,strlen(CHANNEL_ID)); //18012019 Dhiral // IMPS
    memcpy(configLoginLeg2.ucServiceID_req,"1",1); //18012019 Dhiral

    // if(!memcmp((char *)configLoginLeg2.ucGeoLattitude_req,"1",1))//if file is not received
    if(!memcmp((char *)value_latitude.trimmed().toAscii().data(),"00.00",5))
    {
        memcpy(configLoginLeg2.ucGeoLattitude_req,"null",4);//20032019 Aaadarsha
        memcpy(configLoginLeg2.ucGeoLongitude_req,"null",4); //20032019 Aadarsha
    }
    else
    {
        memcpy((char *)configLoginLeg2.ucGeoLattitude_req, value_latitude.trimmed().toAscii().data(), strlen(value_latitude.trimmed().toAscii().data()));   //20032019 Aaadarsha
        memcpy((char *)configLoginLeg2.ucGeoLongitude_req, value_longitude.trimmed().toAscii().data(), strlen(value_longitude.trimmed().toAscii().data()));   //20032019 Aaadarsha
    }
    memcpy(configLoginLeg2.ucVersion_req,APP_VERSION,7);
    memcpy(configLoginLeg2.ucMAC_DeviceID_req,arr,strlen(arr));
    memcpy(configLoginLeg2.ucCellID_req,"0",1);
    memcpy(configLoginLeg2.ucDeviceModel_req,"Visiontek",9);
    memcpy(configLoginLeg2.ucDeviceOS_req,"Linux",5);
    memcpy(configLoginLeg2.ucMCC_req,"404",3);
    memcpy(configLoginLeg2.ucMNC_req,"20",2);
    memcpy(configLoginLeg2.ucipAddress_req,arr_ip,strlen(arr_ip));
    //    memcpy(configLoginLeg2.ucClient_id_req,"1",1);




    //	memcpy(configLoginLeg2.ucProductCode_req,"null",4);
    //	memcpy(configLoginLeg2.ucAmount_req,"null",4);
    //	memcpy(configLoginLeg2.ucMerchantContribution_req,"null",4);
    //	memcpy(configLoginLeg2.ucCharges_req,"null",4);
    //	memcpy(configLoginLeg2.ucRouteID_req,"null",4);
    //	memcpy(configLoginLeg2.ucSplitReferenceNo_req,"null",4);

    //	memcpy(configLoginLeg2.ucIdentifier_req,"null",4);
    //	memcpy(configLoginLeg2.ucTxnHash_req,"null",4);


    iGenerateRequestID((char *)username,(unsigned char *)&configLoginLeg2.ucRRN_req);

    iRet=makeJSONReqLEG2();
    if(iRet!=FUNC_OK)
    {
        qDebug()<<"JSON Creation failed";
        uidmessage::instance()->hide_Message();
        return FUNCTION_FAILED;;

    }
    Flag_FirstTime_Login=0;
    Flag_eAgreement_Login=0;
    if(!strcmp(ISENCEYPT,"true"))
    {
#if DEBUG
        printf("static_key%s",static_key);
 #endif
        int gSendData_len = strlen((char *)gSendData);
        Login_encryption((char *)gSendData,gSendData_len,static_key);
    }

#ifdef UAT_HTTP     // File Download


    int oLen2=0;
    uidmessage::instance()->hide_Message();
    hComm.close_connection();
    res = hComm.open_connect(SERVERIP,SERVERPORT);
    if(res != 0)
    {
        qDebug() << "Failed to connect to the Server";
        //        memset(&configLoginLeg1,0,sizeof(configLoginLeg1));
        //        memset(&configLoginLeg2,0,sizeof(configLoginLeg2));
        memset(gSendData,0,sizeof(gSendData));
        memset(Recvdata,0,sizeof(Recvdata));
        system("rm /usr/FINO/FPB/REQ/LEG2_REQ.TXT");
        uidmessage::instance()->hide_Message();
        uidmessage::instance()->DisplayMessage("Server Not Connected");
        qDebug()<<"FUNCTION_FAILED";
        return FUNCTION_FAILED;
    }
    uidmessage::instance()->hide_Message();
    len_gSendData = strlen((char *)gSendData);

    if((hComm.host_2Sendrecv((unsigned char *)gSendData,len_gSendData,( unsigned char*)oBuff,(int *)&oLen2,(int)response_timeout,0))!=SUCCESS)
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

    //    if(iRet!=FUNC_OK)configLoginLeg2.ucforce_change_password
    //    {
    //        qDebug()<<"Connection failed.";
    //        //return FUNC_ERROR;
    //        uidmessage::instance()->hide_Message();
    //        return FUNCTION_FAILED;
    //    }


#else
#if DEBUG
    printf("gSendData = %s\n\n",gSendData);
#endif
    memset(str_url,0,sizeof(str_url));
#ifdef PRODUCTION
    strcpy(str_url,URL_CREATION(LOGIN_URL_PROD));
#else
    if(!strcmp(ISENCEYPT,"true"))
    {
        strcpy(str_url,URL_CREATION((char *)LOGINAUTH_URL));
    }
    else
    {
        strcpy(str_url,URL_CREATION((char *)LOGIN_URL));

    }

#endif

    int retk = file_download(str_url, (char *)gSendData,"/usr/FINO/FPB/RESP/OUTPUT.TXT",120);
    if(retk!=0)
    {
        qDebug()<<"retk Password Page = "<<retk;
        sleep(1);
        uidmessage::instance()->hide_Message();
        return FUNCTION_FAILED;
    }

#endif


    /****************************Decryption function*********************************/
    if(!strcmp(ISENCEYPT,"true"))
    {
        int oBuff_len = strlen(oBuff);
#if DEBUG
        qDebug()<<"Length of buffer"<<oBuff_len;
        printf("Buffer%s\n",oBuff);
#endif
        LoginDetachEncryptAttach(oBuff,oBuff_len,static_key,2);//TEMPORARY
    }
    /**************************************************************************/

#if DEBUG
    printf("\noBuff = %s\n",oBuff);
#endif
    system("mkdir /usr/FINO/FPB/RESP/");
    system("cd /usr/FINO/FPB/RESP/");
    system("rm /usr/FINO/FPB/RESP/LEG2_RESP.txt");
    //int gRecv_len = strlen((char *)oBuff2); //// ORIGINAL
    int gRecv_len = strlen((char *)oBuff);
    FILE *fp2 = NULL;
    fp2 = fopen("/usr/FINO/FPB/RESP/LEG2_RESP.TXT","w");
    if(fp2 == NULL)
    {
        return FUNCTION_FAILED;
    }
    qDebug()<<"File"<<fp2;
    //fwrite( oBuff2, 1, gRecv_len, fp2 ); //// ORIGINAL
    fwrite( oBuff, 1, gRecv_len, fp2 );
    fclose(fp2);


    int iRetparse = Login_Data();

    system("cd /usr/FINO/FPB/RESP/");
    //system("rm /usr/FINO/FPB/RESP/LEG2_RESP.TXT");
    system("rm /usr/FINO/FPB/RESP/OUTPUT.TXT");

    strncpy(Mer_Latest_Balance, configLoginLeg2.ucavailableBalance, strlen(configLoginLeg2.ucavailableBalance));
    //  printf("Mer_Latest_Balance logindata = %s\n",Mer_Latest_Balance);
    QString temp1 = QString :: fromUtf8(Mer_Latest_Balance);
    QString BL = "Balance : " + temp1;
    ui->Dash_Latest_Balance_Label_pg12->setText(BL);

    ui->lineEdit_userid_2->setText(ui->lineEdit_userid->text().toAscii().data());

    uidmessage::instance()->hide_Message();
    if(iRetparse!=FUNC_OK)
    {
        Flag_FirstTime_Login=0;
        Flag_eAgreement_Login=0;
        sprintf(ErrorMsg,"%s",/*(char *)configLoginLeg2.ucResponseCode,*/(char *)configLoginLeg2.ucResponseMessage);
        miscObj.DisplayMsgBox(ErrorMsg);
        uidmessage::instance()->hide_Message();
        return FUNCTION_FAILED;
    }

    if (!memcmp(configLoginLeg2.ucResponseCode,"0",1))
    {
        qDebug()<<"Login Successful";
        QSqlQuery query;
        query.exec("delete from LastData");
        char iquery[500];
        memset(iquery, 0, sizeof(iquery));
        sprintf(iquery, "INSERT INTO LastData (LAST_USER_ID) VALUES('%s')",configLoginLeg2.ucUser_id_req);
#if DEBUG
        printf("QUERY === %s\n",iquery);
#endif
        if(!query.exec(iquery))
        {
            qDebug() << "In Insert_LST_DB = " << query.lastError();
            query.clear();
            query.finish();
            uidmessage::instance()->hide_Message();
            //return FUNC_ERROR;
        }
        query.clear();
        query.finish();

        memcpy(configLoginLeg2.ucMerchantLimit,configLoginLeg2.ucprimaryCashLimit,strlen((char *)configLoginLeg2.ucprimaryCashLimit));
        ui->Eagreement_Merchant_Name_pg27->clear();

        QString  Version = APP_VERSION;
        Potversion = Version.remove('.');
        // strcpy((char *)configLoginLeg2.ucCurrentVersion,"770071001");
        MandatoryVersion = QString::fromUtf8((const char*)configLoginLeg2.ucMandatoryVersion);
        Currentversion = QString::fromUtf8((const char*)configLoginLeg2.ucCurrentVersion);
#if DEBUG
        qDebug()<<"Potversion"<<Potversion;
        qDebug()<<"MandatoryVersion"<<MandatoryVersion;
        qDebug()<<"Currentversion"<<Currentversion;
#endif
        if(Potversion.toInt() < MandatoryVersion.toInt())
        {
            Flag_UpdateApp = 1;
            if(iDisplayConsent ("This Version of POS is expired \nPlease update the application") == false)
            {
                ui->stackedWidget->setCurrentIndex(0);
                return FUNCTION_FAILED;
            }
            // strcpy((char *)configLoginLeg2.ucMandatoryVersion,"550061002");
            uidmessage::instance()->hide_Message();
            int iret_AU = FTP_download_function((char *)configLoginLeg2.ucMandatoryVersion);
            if(iret_AU == FUNCTION_FAILED)
            {
                ui->stackedWidget->setCurrentIndex(0);
                return FUNCTION_FAILED;
            }
            return FUNCTION_SUCCESS;
        }

        if(Potversion.toInt() < Currentversion.toInt() && Potversion.toInt() >=MandatoryVersion.toInt())
        {
            Flag_UpdateApp = 2;
            if(iDisplayConsent ("New version is available \nWould you like update") == false)
            {
                Flag_UpdateApp = 0;
                return FUNCTION_SUCCESS;
            }
            uidmessage::instance()->hide_Message();
            int iret_AU = FTP_download_function((char *)configLoginLeg2.ucCurrentVersion);
            if(iret_AU == FUNCTION_FAILED)
            {
                Flag_UpdateApp = 0;
                return FUNCTION_SUCCESS;
            }

        }

#if DEBUG
        printf("configLoginLeg2.ucADagentStatus = %s\n",configLoginLeg2.ucADagentStatus);
#endif
        if((strcmp(configLoginLeg2.uceAgreement, "1")==0) && (strcmp(configLoginLeg2.ucADagentStatus, "1")==0))
        {
#if DEBUG
            qDebug()<<"uceAgreement == "<<configLoginLeg2.uceAgreement;
#endif
            Flag_eAgreement_Login = 1;
            uidmessage::instance()->hide_Message();
            return FUNCTION_SUCCESS;
            //ui->stackedWidget->setCurrentIndex(0);

        }
#if DEBUG
        qDebug()<<"strcpy first time login == "<<configLoginLeg2.ucforce_change_password;
#endif
        if(strcmp(configLoginLeg2.ucforce_change_password, "true")==0)
        {
#if DEBUG
            qDebug()<<"first time login == "<<configLoginLeg2.ucforce_change_password;
#endif
            Flag_FirstTime_Login = 1;
            ui->secret_question_display->clear();
            ui->SecretAnswer->clear();
            //_----------------vaibhav---------------
            ret = Prepare_encryption_Func();
            if(ret == FUNCTION_SUCCESS)
            {
                ret = iReq_SecretQuestion();
                return ret ;
            }
            else
            {
               return ret ;
            }


            return ret;


        }


        if((strcmp(configLoginLeg2.uceAgreement, "2")==0) && (strcmp(configLoginLeg2.ucADagentStatus, "1")==0)) // Under Dev 17022019
        {
            ;
        }


        uidmessage::instance()->hide_Message();
        //memcpy(configLoginLeg2.ucMerchantLimit,configLoginLeg2.ucprimaryCashLimit,strlen((char *)configLoginLeg2.ucprimaryCashLimit));
        if(Flag_FirstTime_Login!=1 && Flag_eAgreement_Login!=1)
        {
            uidmessage::instance()->Display_BusyMessage("Login Successful...");
        }
        //        sleep(1);
        //        uidmessage::instance()->hide_Message();
        return FUNCTION_SUCCESS;

        //GL_Dialog_Message(graphicLib,"","Login Successful.",GL_ICON_NONE,GL_BUTTON_NONE,2000);
    }
    else
    {
        Flag_FirstTime_Login=0;
        Flag_eAgreement_Login=0;
        uidmessage::instance()->Display_BusyMessage("Login Failed...");
        uidmessage::instance()->hide_Message();
        qDebug()<<"Login FAILED";
        return FUNCTION_FAILED;
    }

    return FUNCTION_SUCCESS;
}




void FPB::on_PAGE32_exit_2_clicked()
{
    ui->lineEdit_userid->text().clear();
    ui->stackedWidget->setCurrentIndex(0);
    return;
}


void FPB::on_PAGE32_exit_clicked()    // File Download
{
    if(iDisplayConsent ("Do you want to Exit ?") == false)
        return;
    uidmessage::instance()->DisplayMessage("Closing Application, Please Wait..");
    gl11_optcap_pwroff();
    gl11_iodev_close();
    exit(1);
    /*    // File Download
    ui->lineEdit_userid->text().clear();
    ui->lineEdit_password->text().clear();
    ui->stackedWidget->setCurrentIndex(9);
    return;
*/
}





int FPB::makeJSONReqLEG1()
{
    qDebug()<<"makeJSONReqLEG1";
    int iRet=-2,slen;
    // memset(&configLoginLeg1,0,sizeof(configLoginLeg1));
    memset(gSendData,0,sizeof(gSendData));


    iRet = iMakeRequest(0);//changed by vaishnavi
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
    system("rm /usr/FINO/FPB/REQ/LEG1_REQ.TXT");
    FILE *fp2 = NULL;
    fp2 =fopen("/usr/FINO/FPB/REQ/LEG1_REQ.TXT","a");
    fwrite(gSendData,strlen((char *)gSendData),1,fp2);
    //ttestall(0,100);
    sleep(1);
    fclose(fp2);
#endif
    return FUNC_OK;
}


int FPB::makeJSONReqLEG2()
{
    int iRet=-2;
    memset(&configLoginLeg1,0,sizeof(configLoginLeg1));
    memset(gSendData,0,sizeof(gSendData));


    iRet = iMakeRequest(1);//changed by vaishnavi
    if (iRet!=FUNC_OK)
    {
        uidmessage::instance()->hide_Message();
        return FUNC_ERROR;
    }

    /*    //consolprint(Recvdata); // For File_Download all fun commented
    slen=strlen((char *)Recvdata);
    slen-=2;
    memset(gSendData,0,sizeof(gSendData));
    strncpy((char *)gSendData,"POST /UIService.svc/ProcessLoginRequest HTTP/1.1\r\n",52);
    strncat((char *)gSendData,"Host: 202.58.125.2:9013\r\n",27);
    strncat((char *)gSendData,"Content-Type: application/json; charset=utf-8\r\n",49);
    sprintf(tempbuff,"Content-Length: %d\r\n",slen);
    strncat((char *)gSendData,tempbuff,strlen(tempbuff));

    strcat((char *)gSendData,(char *)Recvdata);
*/

    strncpy((char *)gSendData,(char *)Recvdata,strlen((char*)Recvdata)); // For File_Download
    //	memcpy(gSendData,Recvdata,strlen(Recvdata));
    //	consolprint(gSendData);
    //FS_unlink("/HOST/LEG2_REQ.TXT");
#if DEBUG
    system("cd /usr/FINO/FPB/REQ/");
    system("rm /usr/FINO/FPB/REQ/LEG2_REQ.TXT");
    FILE *fp2 = NULL;
    fp2 =fopen("/usr/FINO/FPB/REQ/LEG2_REQ.TXT","a");
    fwrite(gSendData,strlen((char *)gSendData),1,fp2);
    //ttestall(0,100);
    sleep(1);
    fclose(fp2);
#endif

    return FUNC_OK;
}


int FPB::LoginCardAcceptorLocation()
{


    memset(&FINDCUST,0,sizeof(FINDCUST));

    //    uidmessage::instance()->hide_Message();
    //    uidmessage::instance()->Display_BusyMessage("Please Wait....");

    memset(FINDCUST.ucSessionId_FCreq,            0, sizeof(FINDCUST.ucSessionId_FCreq));
    memset(FINDCUST.ucRequestID_FCreq,            0, sizeof(FINDCUST.ucRequestID_FCreq));
    memset(FINDCUST.ucSessionExpiryTime_FCreq,    0, sizeof(FINDCUST.ucSessionExpiryTime_FCreq));
    memset(FINDCUST.ucTellerID_FCreq,             0, sizeof(FINDCUST.ucTellerID_FCreq));
    memset(FINDCUST.ucIsEncrypt_FCreq,            0, sizeof(FINDCUST.ucIsEncrypt_FCreq));
    memset(FINDCUST.ucRDmobileNumber_FCreq,       0, sizeof(FINDCUST.ucRDmobileNumber_FCreq));
    memset(FINDCUST.ucRDchannelId_FCreq,          0, sizeof(FINDCUST.ucRDchannelId_FCreq));
    memset(FINDCUST.ucRDmobileNumber_FCreq,       0, sizeof(FINDCUST.ucRDmobileNumber_FCreq));
    memset(FINDCUST.ucMethodID_FCreq,             0, sizeof(FINDCUST.ucMethodID_FCreq));


    QString buff_tmp;
    Json1::Value RequestedData,SubData;
    char buff1[100];
    iGenerateRequestID((char *)username,(unsigned char *)buff1);

    RequestedData["RequestId"] = buff1;//"TLR1032_2620191138138";
    RequestedData["MethodId"] = 8;
    RequestedData["ChannelID"]= 1;
    RequestedData["SessionId"] = (char *)configLoginLeg2.ucSessionId; //buff_tmp.toAscii().data();//"9d044213-09d9-400d-aa96-5e9a473ba494";
    RequestedData["TellerID"]= (char *)username; // Dhiral
    RequestedData["TokenId"]= "";
    RequestedData["X_Auth_Token"] ="";
    RequestedData["IsEncrypt"]= ISENCEYPT;
    RequestedData["Amount"]= "";
    RequestedData["MerchantContribution"]= ""; // Dhiral
    RequestedData["ProductCode"]= ""; //""; // Dhiral
    RequestedData["SessionExpiryTime"]= (char *)configLoginLeg2.ucSessionExpiryTime;//"10/11/2018 11:58:44 PM";

    SubData["customerName"] = "";
    SubData["mobileNo"] = "";
    SubData["aadharNo"] = "";
    SubData["panNo"] = "";
    SubData["DOB"] = "";
    SubData["customerNo"] =  "";
    SubData["idproofNo"] = "";
    SubData["addressproofNo"] = "";
    SubData["channelId"] = "";
    SubData["accountNo"] = (char*)configLoginLeg2.ucmerchantAccount;

    buff_tmp = serialize1(Convertstr(SubData.toStyledString()));
    //SubData["RequestData"]["CustomerAuth"]["Otp"] = buff_tmp.toAscii().data();
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
        RequestDetachEncryptAttach((char *)gSendData,1,gSendData_len);
        qDebug()<<"After encrypt function";
    }
    /***************************************************************/

    system("rm /usr/FINO/FPB/RESP/FINDCUST.TXT");
    memset(str_url,0,sizeof(str_url));
#ifdef PRODUCTION
    strcpy(str_url,URL_CREATION(LOGIN_URL_PROD));
#else
    strcpy(str_url,URL_CREATION((char *)FIND_CUST));
#endif

    system("rm /usr/FINO/FPB/RESP/FINDCUST.TXT");
    int retk = file_download(str_url, (char *)gSendData,(char *)"/usr/FINO/FPB/RESP/FINDCUST.TXT",120);
    if(retk!=0)
    {
        sleep(1);
        uidmessage::instance()->hide_Message();
        return FUNCTION_FAILED;
    }



//    file_download( "http://103.1.112.205:4444//UIService.svc/ProcessRegistrationRequest", (char *)gSendData,"/usr/FINO/FPB/RESP/FINDCUST.TXT",120);
    /****************************Decryption function*********************************/
    if(!strcmp((char *)ISENCEYPT,"true"))
    {
        int oBuff_len = strlen(oBuff);
#if DEBUG
        qDebug()<<"Length of buffer"<<oBuff_len;
        printf("Buffer%s\n",oBuff);
#endif
        ResponseDetachEncryptAttach(oBuff,2,oBuff_len);//TEMPORARY

        system("rm /usr/FINO/FPB/RESP/FINDCUST.TXT");
        FILE *fp2 = NULL;
        fp2 =fopen("/usr/FINO/FPB/RESP/FINDCUST.TXT","a");
        fwrite(oBuff,strlen((char *)oBuff),1,fp2);
        sleep(1);
        fclose(fp2);
    }
    /*******************************************************************************/

    int ret_fc = findcust(1);
    //    uidmessage::instance()->hide_Message();
    if(ret_fc != FUNCTION_SUCCESS)
    {
        qDebug()<<"Find Cust Fun Failed";
        // ui->stackedWidget->setCurrentIndex(0);
        return FUNCTION_FAILED;
    }

    return FUNCTION_SUCCESS;
}

void FPB::on_OTP_submit_button_clicked()
{
    Check_Conditions();
    int iRet =-2,iRet1 = -2;
    iRet = Device_change_OTP();
    if(iRet != FUNCTION_SUCCESS)
    {
        if(iRet == FUNC_BACK)
        {
            ui->OTP_editline->clear();
            uidmessage::instance()->hide_Message();
            ui->stackedWidget->setCurrentIndex(31);
        }
        else
        {
            ui->OTP_editline->clear();
            uidmessage::instance()->hide_Message();
            ui->stackedWidget->setCurrentIndex(0);
        }
    }
    else if(iRet == FUNCTION_SUCCESS)
    {
        ui->lineEdit_userid_2->setText(ui->lineEdit_userid->text().toAscii().data());
        ui->lineEdit_userid_2->setEnabled(false);
        uidmessage::instance()->hide_Message();
        ui->stackedWidget->setCurrentIndex(1);
    }
    return;
}

int FPB::Device_change_OTP()
{
    uidmessage::instance()->hide_Message();
    uidmessage::instance()->Display_BusyMessage("Please wait...");
    QString Buff ;
    CMisc miscObj;
    int ret = -3, iRet=-2;
    char ErrorMsg[100];
    char* jsondata;
    char OTP[10];
    QString OTP_edit;
    FILE *fp1 = NULL;
    memset(ErrorMsg,0,sizeof(ErrorMsg));
    memset((char *)snddevice.ucRequestID_req,0,strlen((char *)snddevice.ucRequestID_req));
    iGenerateRequestID((char *)username,(unsigned char *)&snddevice.ucRequestID_req);
    //   qDebug()<<"ui->OTP_editline->text()123345"<<ui->OTP_editline->text();
    memset(OTP,0,sizeof(OTP));
    OTP_edit = ui->OTP_editline->text();
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


    memcpy(OTP,ui->OTP_editline->text().trimmed().toAscii().data(),strlen(ui->OTP_editline->text().trimmed().toAscii().data()));

    //    Json1::Value ArrayList(Json1::arrayValue);
    Json1::Value mainreq,RequestData,Request;
    mainreq["RequestId"]=(char *)snddevice.ucRequestID_req;
    mainreq["MethodId"]=21;
    mainreq["SessionId"]="";
    mainreq["TellerID"]=username;
    mainreq["TokenId"]="";
    mainreq["X_Auth_Token"]="";
    mainreq["IsEncrypt"]=false;
    RequestData["requestbitmap"] ="11";
    RequestData["CustomerAuthentication"]["Auth_id"] = 1;
    RequestData["CustomerAuthentication"]["MobileNo"] ="";
    RequestData["CustomerAuthentication"]["AadharNo"] ="";
    RequestData["CustomerAuthentication"]["Otp"]["RequestId"] =(char *)snddevice.ucRequestID_req;
    RequestData["CustomerAuthentication"]["Otp"]["MethodId"] = 2;
    RequestData["CustomerAuthentication"]["Otp"]["SessionId"] = "";
    RequestData["CustomerAuthentication"]["Otp"]["TellerID"] =username;
    RequestData["CustomerAuthentication"]["Otp"]["TokenId"]  = "";
    RequestData["CustomerAuthentication"]["Otp"]["X_Auth_Token"] = "";
    RequestData["CustomerAuthentication"]["Otp"]["IsEncrypt"] = false;
    RequestData["CustomerAuthentication"]["Otp"]["SessionExpiryTime"] ="";
    Request["MethodId"]= 2;
    Request["RequestId"]=(char *)configLoginLeg1.ucrequestId;
    Request["CustomerMobileNo"]="";
    Request["OtpPin"]=OTP;
    Request["MessageId"]=15;
    Request["OtpParam"]="";
    Buff = serialize1(Convertstr(Request.toStyledString()));
    RequestData["CustomerAuthentication"]["Otp"]["RequestData"] = Buff.toAscii().data();;
    RequestData["CustomerAuthentication"]["Aadhaar"] ="";
    RequestData["CustomerAuthentication"]["Fp_Auth"] ="";
    RequestData["AuthProfile"]["user_id"] =(char *) configLoginLeg1.ucUser_id_req;
    RequestData["AuthProfile"]["client_id"] =CLIENT_ID;
    Buff = serialize1(Convertstr(RequestData.toStyledString()));
    mainreq["RequestData"] =Convertstr(RequestData.toStyledString());
    jsondata = Convertstr(mainreq.toStyledString());
#if DEBUG
    qDebug()<<"------------->"<<jsondata;
#endif
    //-------------------------------------------------------------
    memset(gSendData,0,sizeof(gSendData));
    strncpy((char*)gSendData, jsondata, strlen(jsondata));
#if DEBUG
    printf("gSendData = %s\n",gSendData);
#endif
    //-------------------------------------------------------------
    if(!strcmp(ISENCEYPT,"true"))
    {
#if DEBUG
        printf("static_key%s",static_key);
#endif
        int gSendData_len = strlen((char *)gSendData);
        Login_encryption((char *)gSendData,gSendData_len,static_key);
    }

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
    if(!strcmp(ISENCEYPT,"true"))
    {
        strcpy(str_url,URL_CREATION((char *)LOGINAUTH_URL));
    }
    else
    {
        strcpy(str_url,URL_CREATION((char *)LOGIN_URL));

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

    /*    file_download( "https://103.1.112.204/UIService.svc/ProcessLoginRequest", (char *)gSendData,"/usr/FINO/FPB/RESP/LoginCurl1.TXT",50);
    file_download( "https://partner.finopaymentbank.in/UIService.svc/ProcessLoginRequest", (char *)gSendData,"/usr/FINO/FPB/RESP/OUTPUT.TXT",90);*/

#endif
#if DEBUG
    printf("\noBuff = %s\n",oBuff);
#endif

    /****************************Decryption function*********************************/
    if(!strcmp(ISENCEYPT,"true"))
    {
        int oBuff_len = strlen(oBuff);
#if DEBUG
        qDebug()<<"Length of buffer"<<oBuff_len;
        printf("Buffer%s\n",oBuff);
#endif
        LoginDetachEncryptAttach(oBuff,oBuff_len,static_key,1);//TEMPORARY
    }
    /**************************************************************************/


    system("mkdir /usr/FINO/FPB/RESP/");
    system("cd /usr/FINO/FPB/RESP/");
    system("rm /usr/FINO/FPB/RESP/Login_otp.txt");
    int gRecv_len = strlen((char *)oBuff);
    FILE *fp2 = NULL;
    fp2 = fopen("/usr/FINO/FPB/RESP/Login_otp.txt","w");
    if(fp2 == NULL)
    {
        return FUNCTION_FAILED;
    }
    fwrite( oBuff, 1, gRecv_len, fp2 );
    fclose(fp2);

    iRet = Login1_data();
    system("cd /usr/FINO/FPB/RESP/");
    system("rm /usr/FINO/FPB/RESP/OUTPUT.TXT");
    system("rm /usr/FINO/FPB/RESP/Login_otp.txt");

    if(iRet!=FUNC_OK)
    {
        sprintf(ErrorMsg,"%s",(char *)configLoginLeg1.ucResponseMessage);
        miscObj.DisplayMsgBox(ErrorMsg);
        uidmessage::instance()->hide_Message();
        return FUNCTION_FAILED;
    }
    else if(iRet == FUNC_OK)
    {

        uidmessage::instance()->hide_Message();
        return FUNCTION_SUCCESS;
    }


}

int FPB::Login1_data()
{
    system("sed 's/\"ResponseData\":\"{/\"ResponseData\":{/g' /usr/FINO/FPB/RESP/Login_otp.txt > /tmp/1.txt");
    system("sed 's/}\",\"MessageId\"/},\"MessageId\"/g' /tmp/1.txt > /tmp/2.txt  ");
    system(" sed 's/\\\\//g' /tmp/2.txt > /usr/FINO/FPB/RESP/Login_otp.txt");

    system("cd /tmp/");
    system("rm /tmp/1.txt");
    system("rm /tmp/2.txt");


    Json1::Value root;
    root.clear();
    //QSqlQuery query;
    Json1::Reader reader;
    std::ifstream ReadData("/usr/FINO/FPB/RESP/Login_otp.txt");
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
#endif
        strcpy((char *)configLoginLeg1.ucRequestId,Convertstr(root["RequestId"].asString()));
        strcpy((char *)configLoginLeg1.ucDisplayMessage,Convertstr(root["DisplayMessage"].asString()));
        strcpy((char *)configLoginLeg1.ucResponseMessage,Convertstr(root["ResponseMessage"].asString()));
        strcpy((char *)configLoginLeg1.ucResponseCode,Convertstr(root["ResponseCode"].asString()));
        if(strcmp((char *)configLoginLeg1.ucResponseCode,"0")==0)
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
                strcpy((char *)configLoginLeg1.ucreturnCode,Convertstr(ResponseData["returnCode"].asString()));
                strcpy((char *)configLoginLeg1.ucresponseBitmap,Convertstr(ResponseData["responseBitmap"].asString()));

                strcpy((char *)configLoginLeg1.ucCAreturnCode,Convertstr(ResponseData["CustomerAuthResponse"]["returnCode"].asString()));
                strcpy((char *)configLoginLeg1.ucCAresponseMessage,Convertstr(ResponseData["CustomerAuthResponse"]["responseMessage"].asString()));

                strcpy((char *)configLoginLeg1.ucUAreturnCode,Convertstr(ResponseData["UserAuthProfileResponse"]["returnCode"].asString()));
                strcpy((char *)configLoginLeg1.ucUAresponseMessage,Convertstr(ResponseData["UserAuthProfileResponse"]["responseMessage"].asString()));
                strcpy((char *)configLoginLeg1.ucuserId,Convertstr(ResponseData["UserAuthProfileResponse"]["userId"].asString()));
#if DEBUG
                qDebug()<<"configLoginLeg1.ucuserId"<<Convertstr(ResponseData["UserAuthProfileResponse"]["userId"].asString());
#endif
                strcpy((char *)configLoginLeg1.ucsubUserClass,Convertstr(ResponseData["UserAuthProfileResponse"]["subUserClass"].asString()));
                strcpy((char *)configLoginLeg1.ucfPIndex,Convertstr(ResponseData["UserAuthProfileResponse"]["fpIndex"].asString()));
                strcpy((char *)configLoginLeg1.ucbranchCode,Convertstr(ResponseData["UserAuthProfileResponse"]["branchCode"].asString()));
#if DEBUG
                qDebug()<<"configLoginLeg1.branchCode"<<Convertstr(ResponseData["UserAuthProfileResponse"]["branchCode"].asString());
#endif

                Json1::Value ResponseData1;
                ResponseData1.clear();
                ResponseData1 = ResponseData.get("UserAuthProfileResponse",0);

                Json1::Value policies;
                policies.clear();
                policies = ResponseData1.get("policies",0);

                for(int i=0;i<policies.size();i++)
                {
                    strcpy(configLoginLeg2.ucpolicy,Convertstr(policies[i]["policy"].asString()));
                    strcpy(configLoginLeg2.ucpriority,Convertstr(policies[i]["priority"].asString()));
                    strcpy(configLoginLeg2.ucabort_on_failure,Convertstr(policies[i]["abort_on_failure"].asString()));
                }
#if DEBUG
                qDebug()<<"configLoginLeg2.ucabort_on_failure"<<configLoginLeg2.ucabort_on_failure;
                qDebug()<<"configLoginLeg2.ucpriority"<<configLoginLeg2.ucpriority;
                qDebug()<<"configLoginLeg2.ucpolicy"<<configLoginLeg2.ucpolicy;
#endif
                strcpy((char *)configLoginLeg1.ucclient_id,Convertstr(ResponseData["UserAuthProfileResponse"]["client_id"].asString()));

                Json1::Value user_roles;
                user_roles.clear();
                user_roles = ResponseData1.get("user_roles",0);

                for(int i=0;i<user_roles.size();i++)
                {
                    strcpy((char *)configLoginLeg1.ucuser_roles,Convertstr(user_roles[i].asString()));
#if DEBUG
                    qDebug()<<"configLoginLeg1.branchCode"<<Convertstr(user_roles[i].asString());
#endif

                }
                strcpy((char *)configLoginLeg1.ucmobileNumber,Convertstr(ResponseData["UserAuthProfileResponse"]["mobile_no"].asString()));
                strcpy((char *)configLoginLeg1.ucencryption_key,Convertstr(ResponseData["UserAuthProfileResponse"]["encryption_key"].asString()));
#if DEBUG
                qDebug()<<"configLoginLeg2.ucencryption_key"<<Convertstr(ResponseData["UserAuthProfileResponse"]["encryption_key"].asString());
#endif
                //        strcpy((char *)configLoginLeg1.uclast_login_time,Convertstr(ResponseData["last_login_time"].asString()));
                strcpy((char *)configLoginLeg1.ucis_last_login_failed,Convertstr(ResponseData["UserAuthProfileResponse"]["security_profile"]["is_last_login_failed"].asString()));
                strcpy((char *)configLoginLeg1.ucis_user_locked,Convertstr(ResponseData["UserAuthProfileResponse"]["security_profile"]["is_user_locked"].asString()));
                //            strcpy((char *)configLoginLeg1.uclast_login_date,Convertstr(ResponseData["UserAuthProfileResponse"]["security_profile"]["last_login_date"].asString()));
                strcpy((char *)configLoginLeg1.uclast_login_ip,Convertstr(ResponseData["UserAuthProfileResponse"]["security_profile"]["last_login_ip"].asString()));
                strcpy((char *)configLoginLeg1.ucfailure_count,Convertstr(ResponseData["UserAuthProfileResponse"]["security_profile"]["failure_count"].asString()));
                strcpy((char *)configLoginLeg1.ucfailure_sec_question_count,Convertstr(ResponseData["UserAuthProfileResponse"]["security_profile"]["failure_sec_question_count"].asString()));
                //       strcpy((char *)configLoginLeg1.ucfirst_login_time,Convertstr(ResponseData["UserAuthProfileResponse"]["security_profile"]["first_login_time"].asString()));
                //         strcpy((char *)configLoginLeg1.ucfirst_login_date,Convertstr(ResponseData["UserAuthProfileResponse"]["security_profile"]["first_login_date"].asString()));
                return FUNC_OK;
            }
        }
        else
        {
            return FUNC_ERROR;
        }
    }

}

int FPB::OTP_change_parse(QString temp)
{
    CMisc miscObj;
    char ErrorMsg[100];
    temp.remove("\\");
    temp.remove("{");
    temp.remove("}");
    temp.remove('"');
#if DEBUG
    qDebug()<<"OTPBuff"<<temp;
#endif
    QStringList query = temp.split(",");
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
    memset(configLoginLeg1.ucResponseCode, 0, sizeof(configLoginLeg1.ucResponseCode));
    memcpy(configLoginLeg1.ucResponseCode,value1.trimmed().toAscii().data(), strlen(value1.trimmed().toAscii().data()));
    if(value1 =="0")
    {
        str = DisplayMessage.at(0);
        strToFind = ":";
        value2 = str.mid(str.indexOf(strToFind)+strToFind.length());
        memset(configLoginLeg1.ucDisplayMessage, 0, sizeof(configLoginLeg1.ucDisplayMessage));
        memcpy(configLoginLeg1.ucDisplayMessage,value2.trimmed().toAscii().data(), strlen(value2.trimmed().toAscii().data()));
        str= RequestId.at(1);
        strToFind= "RequestId:";
        value = str.mid(str.indexOf(strToFind)+strToFind.length());
        memset(configLoginLeg1.ucrequestId, 0, sizeof(configLoginLeg1.ucrequestId));
        memcpy(configLoginLeg1.ucrequestId,value.trimmed().toAscii().data(), strlen(value.trimmed().toAscii().data()));
#if DEBUG
        qDebug()<<"configLoginLeg1.ucrequestId"<<configLoginLeg1.ucrequestId;
#endif
        //        ui->stackedWidget->setCurrentIndex(22);
        return FUNCTION_SUCCESS;
    }
    else
    {
        str = ResponseMessage.at(0);
        strToFind = ":";
        value2 = str.mid(str.indexOf(strToFind)+strToFind.length());
        memset(configLoginLeg1.ucResponseMessage, 0, sizeof(configLoginLeg1.ucResponseMessage));
        memcpy(configLoginLeg1.ucResponseMessage, value2.trimmed().toAscii().data(), strlen(value2.trimmed().toAscii().data()));
        sprintf(ErrorMsg,"%s",/*(char *)OTP.ucResponseCode,*/(char *)configLoginLeg1.ucResponseMessage);
        miscObj.DisplayMsgBox(ErrorMsg);
        uidmessage::instance()->hide_Message();
        return FUNCTION_FAILED;
    }
}


void FPB::on_OTP_back_button_clicked()
{
    ui->lineEdit_userid->text().clear();
    ui->stackedWidget->setCurrentIndex(0);
    return;
}


