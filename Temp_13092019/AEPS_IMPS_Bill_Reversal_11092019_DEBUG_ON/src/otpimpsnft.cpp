#include "impscommon.h"
#include "ui_FPB.h"
#include "json.h"
#include "Login.h"
#include "displaymessage.h"
#include "print.h"
#include <QSqlDatabase>

extern int OTP_page;
OTPIMPSNEFT OtpImpsNeft;
extern Login_leg2 configLoginLeg2;
extern OTPBENEVERIF OtpBeneVer;
extern sCustDetails CustDet;
extern int Resend_flag;
extern char flag_bc_merchant_account;


int FPB::Otpimpsnft()
{

    HostComm hComm;
    CMisc miscObj;
    int ret = -3;
    char ErrorMsg[100];
    memset(ErrorMsg,0,sizeof(ErrorMsg));

    memset(OtpImpsNeft.Commdata_OIN.ucMethodID_req,  0, sizeof(OtpImpsNeft.Commdata_OIN.ucMethodID_req));
    memset(OtpImpsNeft.Commdata_OIN.ucTellerID_req,  0, sizeof(OtpImpsNeft.Commdata_OIN.ucTellerID_req));
    memset(OtpImpsNeft.Commdata_OIN.ucSessionID_req, 0, sizeof(OtpImpsNeft.Commdata_OIN.ucSessionID_req));
    memset(OtpImpsNeft.Commdata_OIN.ucIsEncrypt_req, 0, sizeof(OtpImpsNeft.Commdata_OIN.ucIsEncrypt_req));
    memset(OtpImpsNeft.Commdata_OIN.ucChannelID_req, 0, sizeof(OtpImpsNeft.Commdata_OIN.ucChannelID_req));
    memset(OtpImpsNeft.ucRDMethodID_req,             0, sizeof(OtpImpsNeft.ucRDMethodID_req));
    memset(OtpImpsNeft.ucCustomerCareNo,             0, sizeof(OtpImpsNeft.ucCustomerCareNo));

    memset(OtpImpsNeft.ucCustMobNo_OIN,              0, sizeof(OtpImpsNeft.ucCustMobNo_OIN));
    memset(OtpImpsNeft.ucMessageID_OIN,              0, sizeof(OtpImpsNeft.ucMessageID_OIN));
    memset(OtpImpsNeft.ucBenename_OIN,               0, sizeof(OtpImpsNeft.ucBenename_OIN));
    memset(OtpImpsNeft.ucBeneAccNo_OIN,              0, sizeof(OtpImpsNeft.ucBeneAccNo_OIN));
    memset(OtpImpsNeft.ucAmount_OIN,                 0, sizeof(OtpImpsNeft.ucAmount_OIN));
    memset(OtpImpsNeft.ucAccountNo_OIN,              0, sizeof(OtpImpsNeft.ucAccountNo_OIN));
    memset(OtpImpsNeft.Commdata_OIN.ucSessionExpiryTime_req,0, sizeof(OtpImpsNeft.Commdata_OIN.ucSessionExpiryTime_req));

    iGenerateRequestID((char *)username,(unsigned char *)&OtpImpsNeft.Commdata_OIN.ucRequestID_req);

    memcpy(OtpImpsNeft.Commdata_OIN.ucChannelID_req, CHANNEL_ID, strlen(CHANNEL_ID));
    memcpy(OtpImpsNeft.ucCustomerCareNo, "", 1);

    memcpy(OtpImpsNeft.Commdata_OIN.ucMethodID_req,"1",1);

    if(Resend_flag == 1)
        memcpy(OtpImpsNeft.ucRDMethodID_req,"4",1);
    else
        memcpy(OtpImpsNeft.ucRDMethodID_req,"1",1);

    memcpy((char *)OtpImpsNeft.Commdata_OIN.ucSessionID_req,configLoginLeg2.ucSessionId,strlen((char *)configLoginLeg2.ucSessionId));
    memcpy(OtpImpsNeft.Commdata_OIN.ucTellerID_req,username,strlen((char *)username));
    memcpy(OtpImpsNeft.ucCustMobNo_OIN,FC_MobileNumber,strlen(FC_MobileNumber));

  //  if(flag_bc_merchant_account != 'B' && Resend_flag != 1)
        memcpy(OtpImpsNeft.ucMessageID_OIN,"457",3);
  //  else
  //      memcpy(OtpImpsNeft.ucMessageID_OIN,"455",3);

    memcpy(OtpImpsNeft.ucBenename_OIN,ui->Bene_Name_edit->text().trimmed().toAscii().data(),strlen(ui->Bene_Name_edit->text().trimmed().toAscii().data()));
    memcpy(OtpImpsNeft.ucBeneAccNo_OIN,ui->Bene_acc_no_edit->text().trimmed().toAscii().data(),strlen(ui->Bene_acc_no_edit->text().trimmed().toAscii().data()));//hardcoded
    memcpy(OtpImpsNeft.ucAccountNo_OIN,"",11);
    memcpy(OtpImpsNeft.ucAmount_OIN,ui->Fund_transfer_amount_edit->text().trimmed().toAscii().data(),strlen(ui->Fund_transfer_amount_edit->text().trimmed().toAscii().data()));
    // memcpy(OtpImpsNeft.ucCustomerMob,"100020442",9);//hardcoded
    memcpy(OtpImpsNeft.Commdata_OIN.ucIsEncrypt_req,"false",5);
    memcpy((char *)OtpImpsNeft.Commdata_OIN.ucSessionExpiryTime_req,(char *)configLoginLeg2.ucSessionExpiryTime,strlen((char *)configLoginLeg2.ucSessionExpiryTime));

    ret	= makeJSON(19);
    if(ret!=FUNC_OK)
    {
        qDebug()<<"JSON Creation failed";
        return FUNC_ERROR;
    }


#if DEBUG
    printf("gSendData = %s\n\n",gSendData);
#endif
    //-------------------------------------------------------------

    memset(str_url,0,sizeof(str_url));
#ifdef PRODUCTION
    strcpy(str_url,URL_CREATION(OTP_POST_IMPS_PROD));
#else
    strcpy(str_url,URL_CREATION((char *)OTP_POST_IMPS));
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


    //       uidmessage::instance()->hide_Message();
    //      hComm.close_connection();
    //      res = hComm.open_connect(SERVERIP,SERVERPORT);
    //      if(res != 0)
    //      {
    //          qDebug() << "Failed to connect to the Server";
    //          uidmessage::instance()->hide_Message();
    //          uidmessage::instance()->DisplayMessage("Server Not Connected");
    //          qDebug()<<"FUNCTION_FAILED";
    //          ui->stackedWidget->setCurrentIndex(0);
    //          return FUNCTION_FAILED;
    //      }


    //      int len_gSendData = strlen((char *)gSendData);
    //      memset(OtpIn_Buff,0,sizeof(OtpIn_Buff));
    //      if((hComm.host_2Sendrecv((unsigned char *)gSendData,len_gSendData,( unsigned char*)OtpIn_Buff,(int *)&OtpIn_len,(int)response_timeout1,0))!=SUCCESS)
    //      {
    //          printf("No Response From Server\n");
    //          hComm.close_connection();
    //          miscObj.DisplayMsgBox((char *)"Response Time-Out");
    //          uidmessage::instance()->hide_Message();
    //          qDebug()<<"***   Response Time-Out   ***";
    //          ui->stackedWidget->setCurrentIndex(1);
    //      }
    //      hComm.close_connection();


    QString OtpInBuff = QString::fromLocal8Bit(oBuff);
    bool check = OtpInBuff.contains("ResponseCode");
    if(check == false)
    {
        miscObj.DisplayMsgBox((char *)"Invalid Data");
        uidmessage::instance()->hide_Message();
        return FUNCTION_FAILED;
    }
    uidmessage::instance()->hide_Message();

    OtpInBuff.remove("\\");
    OtpInBuff.remove("{");
    OtpInBuff.remove("}");
    OtpInBuff.remove('"');
    OtpInBuff.remove("[");
    OtpInBuff.remove("]");
#if DEBUG
    qDebug()<<"OtpInBuff"<<OtpInBuff;
#endif
    QStringList query = OtpInBuff.split(",");
#if DEBUG
    for(int i=0 ; i < query.length() ; i++)
        qDebug()<<"Query Response"<<query.at(i);
#endif
    QStringList RequestId = query.filter("RequestId");
    QStringList DisplayMessage = query.filter("DisplayMessage");
    QStringList ResponseCode = query.filter("ResponseCode");
    QStringList ResponseMessage = query.filter("ResponseMessage");

    uidmessage::instance()->hide_Message();

    QString str ,strToFind,value,value1,value2,value3;
    char temp_buff[100];
    memset(temp_buff, 0, sizeof(temp_buff));

    str = ResponseCode.at(0);
    strToFind = ":";
    value1 = str.mid(str.indexOf(strToFind)+strToFind.length());

    str.clear();
    str = ResponseMessage.at(0);
    value3 = str.mid(str.indexOf(strToFind)+strToFind.length());


    memcpy(temp_buff, value3.trimmed().toAscii().data(), strlen(value3.trimmed().toAscii().data()));
#if DEBUG
    qDebug()<<"temp_buff = "<<temp_buff;
#endif


    //memcpy(OTP.ucResponseCode,value1.trimmed().toAscii().data(), strlen(value1.trimmed().toAscii().data()));
    qDebug()<<"value1"<<value1;
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
        if(memcmp(CustDet.cAddBeneWithouTxn,"Y",1)==0)
        {

            memset(OtpImpsNeft.Commdata_OIN.ucRequestID_res , 0, sizeof(OtpImpsNeft.Commdata_OIN.ucRequestID_res));
            memcpy(OtpImpsNeft.Commdata_OIN.ucRequestID_res ,value.trimmed().toAscii().data(), strlen(value.trimmed().toAscii().data()));
        }
        else
        {
            memset(OtpBeneVer.ucRequestID_Resp , 0, sizeof(OtpBeneVer.ucRequestID_Resp));
            memcpy(OtpBeneVer.ucRequestID_Resp ,value.trimmed().toAscii().data(), strlen(value.trimmed().toAscii().data()));
        }


        char otp_mobileno[20], otp_message[400];
        memset(otp_mobileno, 0, sizeof(otp_mobileno));
        memset(otp_message, 0, sizeof(otp_message));
        char custmob[11];
        memset(custmob, 0, sizeof(custmob));
        // memcpy(custmob, ui->Search_lineEdit->text().trimmed().toAscii().data(),strlen(ui->Search_lineEdit->text().trimmed().toAscii().data()));
        memcpy(custmob, FC_MobileNumber,strlen(FC_MobileNumber));

        sprintf(otp_mobileno,"******%s", &custmob[6]);
        sprintf(otp_message, "One Time Password (OTP) has been\n sent to your registered mobile %s,\n Please enter the same here",otp_mobileno);
        ui->Otp_display_label->setText(otp_message);

        OTP_page = 3;
        // ui->stackedWidget->setCurrentIndex(22);
        return FUNCTION_SUCCESS;
    }
    else
    {
        uidmessage::instance()->hide_Message();
        char ErrorMsg[250];

        if(strcmp((const char*)temp_buff,"")!=0)
        {
            memset(ErrorMsg, 0, sizeof(ErrorMsg));
            sprintf(ErrorMsg,"%s",(char *)temp_buff);
            miscObj.DisplayMsgBox(ErrorMsg);
            if(strcmp((char *)temp_buff,"Session Expired.")==0)
            {
                uidmessage::instance()->hide_Message();
                return SESSION_EXPIRED_FAILED;
            }
        }

        uidmessage::instance()->hide_Message();
        memset(ErrorMsg, 0, sizeof(ErrorMsg));
        return FUNCTION_FAILED;
    }

    //           if(value1 =="0")
    //           {

    //                ui->stackedWidget->setCurrentIndex(22);
    //                return FUNCTION_SUCCESS;
    //           }
    //           else
    //           {

    //               sprintf(ErrorMsg,"Response Code:%s \n Message:%s",(char *)FORPASS_State.ucResponseCode,(char *)FORPASS_State.ucResponseMessage);
    //               miscObj.DisplayMsgBox(ErrorMsg);
    //               uidmessage::instance()->hide_Message();
    //               return FUNCTION_FAILED;
    //           }


    return FUNCTION_SUCCESS;



}


int FPB::iSetOTPIMPS(int tagID, unsigned char *value)
{
    switch(tagID)
    {
    case  1:
        strcpy((char *)value,(char *)OtpImpsNeft.Commdata_OIN.ucRequestID_req);
        break;
    case  2:
        strcpy((char *)value,(char *)OtpImpsNeft.Commdata_OIN.ucMethodID_req);//Method id
        break;
    case  3:
        strcpy((char *)value,(char *)OtpImpsNeft.Commdata_OIN.ucSessionID_req);//teller id
        break;
    case  4:
        strcpy((char *)value,(char *)OtpImpsNeft.Commdata_OIN.ucTellerID_req);//session key
        break;
    case  5:
        strcpy((char *)value,OtpImpsNeft.Commdata_OIN.ucIsEncrypt_req);//token id
        break;
    case  6:
        strcpy((char *)value,(char *)OtpImpsNeft.ucCustMobNo_OIN);//is encrypt
        break;
    case  7:
        strcpy((char *)value,(char *)OtpImpsNeft.ucMessageID_OIN);//start date
        break;
    case  8:
        strcpy((char *)value,(char *)OtpImpsNeft.ucBenename_OIN);//start date
        break;
    case  9:
        strcpy((char *)value,(char *)OtpImpsNeft.ucBeneAccNo_OIN);//start date
        break;
    case  10:
        strcpy((char *)value,(char *)OtpImpsNeft.ucAmount_OIN);//start date
        break;
    case  11:
        strcpy((char *)value,(char *)OtpImpsNeft.ucAccountNo_OIN);//start date
        break;
    case  12:
        strcpy((char *)value,(char *)OtpImpsNeft.Commdata_OIN.ucSessionExpiryTime_req);//start date
        break;
    case  13:
        strcpy((char *)value,(char *)OtpImpsNeft.Commdata_OIN.ucChannelID_req);//start date
        break;
    case  14:
        strcpy((char *)value,(char *)OtpImpsNeft.ucRDMethodID_req);//start date
        break;
    case  15:
        strcpy((char *)value,(char *)OtpImpsNeft.ucCustomerCareNo);//start date
        break;
    default:
        return FUNC_ERROR;
    }
    return FUNC_OK;
}



