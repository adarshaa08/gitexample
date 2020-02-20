#include "impscommon.h"
#include "ui_FPB.h"
#include "json.h"
#include "Login.h"
#include "displaymessage.h"
#include "print.h"
#include <QSqlDatabase>

walkinCust walkinCustLimit;
extern Login_leg2 configLoginLeg2;
unsigned char Cust_CurrMonthAmount[15];
extern int Flag_Display_WLMsg;


int FPB::Walkin_Cust_Limit(void)
{
    int iRet = 0;
    CMisc miscObj;
    Json1::Value RequestedData,SubData;
    RequestedData.clear();
    SubData.clear();
    char nullbuff[5];
    memset(nullbuff, 0, sizeof(nullbuff));
    strcpy(nullbuff,"");
    char buff1[100];
    memset(buff1,0,sizeof(buff1));
    uidmessage::instance()->hide_Message();
    uidmessage::instance()->Display_BusyMessage("Please Wait...");

    iGenerateRequestID((char *)username,(unsigned char *)buff1);

    RequestedData["RequestId"] = buff1;
    RequestedData["MethodId"] = 103;
    RequestedData["ChannelID"]= CHANNEL_ID;
    RequestedData["SessionId"] = (char *)configLoginLeg2.ucSessionId;
    RequestedData["TellerID"]= (char *)username; // Dhiral
    RequestedData["TokenId"]= "";
    RequestedData["X_Auth_Token"] ="";
    RequestedData["IsEncrypt"]= ISENCEYPT;
    RequestedData["SessionExpiryTime"]= (char *)configLoginLeg2.ucSessionExpiryTime;
    RequestedData["ProductCode"] =nullbuff;
    RequestedData["Amount"] =nullbuff;
    RequestedData["MerchantContribution"] =nullbuff;
    RequestedData["Charges"] =nullbuff;


    SubData["mobileno"] = (char *) FC_MobileNumber;
    SubData["trangroup"] = "DMTREMIT";
    buff_tmp.clear();
    serialize1(Convertstr1(SubData.toStyledString()));
    RequestedData["RequestData"] = buff_tmp.toAscii().data();

    char *Ret;
    Ret = Convertstr1(RequestedData.toStyledString());

    memset(gSendData, 0, sizeof(gSendData));
    memcpy(gSendData, (char*)Convertstr1(RequestedData.toStyledString()),strlen(Convertstr1(RequestedData.toStyledString())));

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
    strcpy(str_url,URL_CREATION(COMMON_URL_PROD));
#else
    strcpy(str_url,URL_CREATION((char *)COMMON_URL));
#endif

    int ret = NetworkConnect(0,str_url, 2);
    if(ret != FUNCTION_SUCCESS)
    {
        uidmessage::instance()->hide_Message();
        if(ret == RESPONSE_TIMEOUT)
        {
            qDebug()<<"Response Timeout";
        }
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
    }
    /**************************************************************************/

    QString WALKBuff = QString::fromLocal8Bit(oBuff);
    bool check = WALKBuff.contains("ResponseCode");
    if(check == false)
    {
        miscObj.DisplayMsgBox((char *)"Invalid Data");
        uidmessage::instance()->hide_Message();
        return FUNCTION_FAILED;
    }


    WALKBuff.remove("\\");
    WALKBuff.remove("{");
    WALKBuff.remove("}");
    WALKBuff.remove('"');
    WALKBuff.remove("[");
    WALKBuff.remove("]");
#if DEBUG
    qDebug()<<"WALKBuff"<<WALKBuff;
#endif
    QStringList query = WALKBuff.split(",");
#if DEBUG
    for(int i=0 ; i < query.length() ; i++)
    {
        qDebug()<<"Query Response"<<query.at(i);
    }
#endif
    QStringList RequestId = query.filter("RequestId");
    QStringList DisplayMessage = query.filter("DisplayMessage");
    QStringList ResponseMessage = query.filter("ResponseMessage");
    QStringList ResponseCode = query.filter("ResponseCode");


    uidmessage::instance()->hide_Message();

    QString str = RequestId.at(0);

    QString strToFind = "RequestId:";
    QString value1 = str.mid(str.indexOf(strToFind)+strToFind.length());

    str = ResponseCode.at(0);
    strToFind = ":";
    QString value2 = str.mid(str.indexOf(strToFind)+strToFind.length());

    str = DisplayMessage.at(0);
    QString value3 = str.mid(str.indexOf(strToFind)+strToFind.length());

    str = ResponseMessage.at(0);
    QString value4 = str.mid(str.indexOf(strToFind)+strToFind.length());




#if DEBUG
    qDebug()<<"value"<<value1;
    qDebug()<<"value2"<<value2;
    qDebug()<<"value3"<<value3;
    qDebug()<<"value4"<<value4;
#endif

    memset(walkinCustLimit.Commdata_WCL.ucRequestID_res, 0, sizeof(walkinCustLimit.Commdata_WCL.ucRequestID_res));
    memset(walkinCustLimit.ucWCL_RDResponseMessage_resp, 0, sizeof(walkinCustLimit.ucWCL_RDResponseMessage_resp));
    memset(walkinCustLimit.ucWCL_RDResponseCode_resp, 0, sizeof(walkinCustLimit.ucWCL_RDResponseCode_resp));

    memcpy(walkinCustLimit.Commdata_WCL.ucRequestID_res,value1.trimmed().toAscii().data(), strlen(value1.trimmed().toAscii().data()));
    memcpy(walkinCustLimit.ucWCL_RDResponseMessage_resp, value4.trimmed().toAscii().data(), strlen(value4.trimmed().toAscii().data()));
    memcpy(walkinCustLimit.ucWCL_RDResponseCode_resp,value2.trimmed().toAscii().data(), strlen(value2.trimmed().toAscii().data()));
#if DEBUG
    qDebug()<<"WALKin RequestId RESP = "<<walkinCustLimit.Commdata_WCL.ucRequestID_res;
    qDebug()<<"WALKin ResponseMessage RESP = "<<walkinCustLimit.ucWCL_RDResponseMessage_resp;
    qDebug()<<"WALKin ResponseCode RESP = "<<walkinCustLimit.ucWCL_RDResponseCode_resp;
#endif
    if(value2 =="0")
    {
        QStringList maxDailyLimit = query.filter("maxDailyLimit");
        QStringList maxMonthlyLimit = query.filter("maxMonthlyLimit");
        QStringList currDayAmount = query.filter("currDayAmount");
        QStringList currMonthAmount = query.filter("currMonthAmount");

        str = maxDailyLimit.at(0);
        strToFind = "maxDailyLimit:";
        QString value_maxDailyLimit = str.mid(str.indexOf(strToFind)+strToFind.length());

        str = maxMonthlyLimit.at(0);
        strToFind = "maxMonthlyLimit:";
        QString value_maxMonthlyLimit = str.mid(str.indexOf(strToFind)+strToFind.length());
        QString value_currMonthAmount,value_currDayAmount;
        check = WALKBuff.contains("currDayAmount");
        if(check == true)
        {
            str = currDayAmount.at(0);
            strToFind = "currDayAmount:";
            value_currDayAmount = str.mid(str.indexOf(strToFind)+strToFind.length());
        }
        else
        {
            value_currDayAmount = "0.00";

        }
        check = WALKBuff.contains("currMonthAmount");
        if(check == true)
        {
            str = currMonthAmount.at(0);
            strToFind = "currMonthAmount:";
            value_currMonthAmount= str.mid(str.indexOf(strToFind)+strToFind.length());
        }
#if DEBUG
        qDebug()<<"value_maxDailyLimit"<<value_maxDailyLimit;
        qDebug()<<"value_maxMonthlyLimit"<<value_maxMonthlyLimit;
        qDebug()<<"value_currDayAmount"<<value_currDayAmount;
        qDebug()<<"value_currMonthAmount"<<value_currMonthAmount;

#endif
        memset(walkinCustLimit.ucWCL_RDWCLmaxDailyLimit_resp, 0, sizeof(walkinCustLimit.ucWCL_RDWCLmaxDailyLimit_resp));
        memset(walkinCustLimit.ucWCL_RDWCLmaxMonthlyLimit_resp, 0, sizeof(walkinCustLimit.ucWCL_RDWCLmaxMonthlyLimit_resp));
        memset(walkinCustLimit.ucWCL_RDWCLcurrDayAmount_resp, 0, sizeof(walkinCustLimit.ucWCL_RDWCLcurrDayAmount_resp));
        memset(walkinCustLimit.ucWCL_RDWCLcurrMonthAmount_resp, 0, sizeof(walkinCustLimit.ucWCL_RDWCLcurrMonthAmount_resp));


        memcpy(walkinCustLimit.ucWCL_RDWCLmaxDailyLimit_resp, value_maxDailyLimit.trimmed().toAscii().data(), strlen(value_maxDailyLimit.trimmed().toAscii().data()));


        memcpy(walkinCustLimit.ucWCL_RDWCLmaxMonthlyLimit_resp, value_maxMonthlyLimit.trimmed().toAscii().data(), strlen(value_maxMonthlyLimit.trimmed().toAscii().data()));

        memcpy(walkinCustLimit.ucWCL_RDWCLcurrDayAmount_resp, value_currDayAmount.trimmed().toAscii().data(), strlen(value_currDayAmount.trimmed().toAscii().data()));

        memcpy(walkinCustLimit.ucWCL_RDWCLcurrMonthAmount_resp, value_currMonthAmount.trimmed().toAscii().data(), strlen(value_currMonthAmount.trimmed().toAscii().data()));
        memset(Cust_CurrMonthAmount, 0, sizeof(Cust_CurrMonthAmount));
        memcpy(Cust_CurrMonthAmount, value_currMonthAmount.trimmed().toAscii().data(), strlen(value_currMonthAmount.trimmed().toAscii().data()));
#if DEBUG
        printf("maxDailyLimit = %s\n",walkinCustLimit.ucWCL_RDWCLmaxDailyLimit_resp);
        printf("maxMonthlyLimit = %s\n",walkinCustLimit.ucWCL_RDWCLmaxMonthlyLimit_resp);
        printf("\nCust_CurrMonthAmount = %s\n", Cust_CurrMonthAmount);
#endif

        //        qDebug()<<"maxDailyLimit :"<<Convertstr1(ResponseData["walkinCustomerLimit"]["maxDailyLimit"].asString());
        //        memcpy(walkinCustLimit.ucWCL_RDWCLmaxDailyLimit_resp, Convertstr1(ResponseData["walkinCustomerLimit"]["maxDailyLimit"].asString()), strlen(Convertstr1(ResponseData["walkinCustomerLimit"]["maxDailyLimit"].asString())));
        //        printf("maxDailyLimit = %s\n",walkinCustLimit.ucWCL_RDWCLmaxDailyLimit_resp);

        //        qDebug()<<"maxMonthlyLimit :"<<Convertstr1(ResponseData["walkinCustomerLimit"]["maxMonthlyLimit"].asString());
        //        memcpy(walkinCustLimit.ucWCL_RDWCLmaxMonthlyLimit_resp, Convertstr1(ResponseData["walkinCustomerLimit"]["maxMonthlyLimit"].asString()), strlen(Convertstr1(ResponseData["walkinCustomerLimit"]["maxMonthlyLimit"].asString())));
        //        printf("maxMonthlyLimit = %s\n",walkinCustLimit.ucWCL_RDWCLmaxMonthlyLimit_resp);

        //        qDebug()<<"currDayAmount :"<<Convertstr1(ResponseData["walkinCustomerLimit"]["currDayAmount"].asString());
        //        memcpy(walkinCustLimit.ucWCL_RDWCLcurrDayAmount_resp, Convertstr1(ResponseData["walkinCustomerLimit"]["currDayAmount"].asString()), strlen(Convertstr1(ResponseData["walkinCustomerLimit"]["currDayAmount"].asString())));

        //        qDebug()<<"currMonthAmount  :"<<Convertstr1(ResponseData["walkinCustomerLimit"]["currMonthAmount"].asString());
        //        memcpy(walkinCustLimit.ucWCL_RDWCLcurrMonthAmount_resp, Convertstr1(ResponseData["walkinCustomerLimit"]["currMonthAmount"].asString()), strlen(Convertstr1(ResponseData["walkinCustomerLimit"]["currMonthAmount"].asString())));

        WALKBuff.clear(); // 17072019
        memset(oBuff, 0, sizeof(oBuff)); // 17072019

        return FUNCTION_SUCCESS;
    }
    else
    {
        uidmessage::instance()->hide_Message();
        char ErrorMsg[250];

        WALKBuff.clear(); // 17072019
        memset(oBuff, 0, sizeof(oBuff)); // 17072019

        if(Flag_Display_WLMsg != 1)
        {
            if(strcmp((const char*)walkinCustLimit.ucWCL_RDResponseMessage_resp,"")!=0)
            {
                memset(ErrorMsg, 0, sizeof(ErrorMsg));
                sprintf(ErrorMsg,"%s",(char *)walkinCustLimit.ucWCL_RDResponseMessage_resp);
                miscObj.DisplayMsgBox(ErrorMsg);
                if(strcmp((char *)walkinCustLimit.ucWCL_RDResponseMessage_resp,"Session Expired.")==0)
                {
                    uidmessage::instance()->hide_Message();
                    return SESSION_EXPIRED_FAILED;
                }
            }
        }
        uidmessage::instance()->hide_Message();
        memset(ErrorMsg, 0, sizeof(ErrorMsg));
        return FUNCTION_FAILED;
    }

    return FUNCTION_SUCCESS;

}

//int FPB::WalkinLmt()
//{

//    HostComm hComm;
//    CMisc miscObj;
//    int WALK_len = 0, response_timeout1 = 120, ret = -3, res = -3, iRet = -3;
//    char WALK_Buff[10000];
//    char ErrorMsg[100];

//    memset(ErrorMsg,0,sizeof(ErrorMsg));
//    memset(walkinLimit.Commdata_WCL.ucMethodID_req,0, sizeof(walkinLimit.Commdata_WCL.ucMethodID_req));
//    memset(walkinLimit.Commdata_WCL.ucTellerID_req,0, sizeof(walkinLimit.Commdata_WCL.ucTellerID_req));
//    memset(walkinLimit.Commdata_WCL.ucSessionID_req,0, sizeof(walkinLimit.Commdata_WCL.ucSessionID_req));
//    memset(walkinLimit.Commdata_WCL.ucIsEncrypt_req,0, sizeof(walkinLimit.Commdata_WCL.ucIsEncrypt_req));
//    memset(walkinLimit.Commdata_WCL.ucChannelID_req,0, sizeof(walkinLimit.Commdata_WCL.ucChannelID_req));
//    memset(walkinLimit.ucCustomerMob,0, sizeof(walkinLimit.ucCustomerMob));
//    memset(walkinLimit.uctransgroup,0, sizeof(walkinLimit.uctransgroup));
//    memset(walkinLimit.Commdata_WCL.ucSessionExpiryTime_req,0, sizeof(walkinLimit.Commdata_WCL.ucSessionExpiryTime_req));


//    iGenerateRequestID((char *)username,(unsigned char *)&walkinLimit.Commdata_WCL.ucRequestID_req);
//    memcpy(walkinLimit.Commdata_WCL.ucMethodID_req,"103",3);
//    memcpy((char *)walkinLimit.Commdata_WCL.ucSessionID_req,configLoginLeg2.ucSessionId,strlen((char *)configLoginLeg2.ucSessionId));
//    memcpy(walkinLimit.Commdata_WCL.ucTellerID_req,username,strlen((char *)username));
//    memcpy(walkinLimit.ucCustomerMob,"8055988351",10);//hardcoded
//    memcpy(walkinLimit.uctransgroup,"DMTREMIT",8);//hardcoded
//    // memcpy(walkinLimit.ucCustomerMob,"100020442",9);//hardcoded
//    memcpy(walkinLimit.Commdata_WCL.ucIsEncrypt_req,"false",5);
//    memcpy((char *)walkinLimit.Commdata_WCL.ucSessionExpiryTime_req,(char *)configLoginLeg2.ucSessionExpiryTime,strlen((char *)configLoginLeg2.ucSessionExpiryTime));

//    ret	= makeJSON(16);
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
//    memset(WALK_Buff,0,sizeof(WALK_Buff));
//    if((hComm.host_2Sendrecv((unsigned char *)gSendData,len_gSendData,( unsigned char*)WALK_Buff,(int *)&WALK_len,(int)response_timeout1,0))!=SUCCESS)
//    {
//        printf("No Response From Server\n");
//        hComm.close_connection();
//        miscObj.DisplayMsgBox((char *)"Response Time-Out");
//        uidmessage::instance()->hide_Message();
//        qDebug()<<"***   Response Time-Out   ***";
//        ui->stackedWidget->setCurrentIndex(1);
//    }
//    hComm.close_connection();


//    QString WALKBuff = QString::fromLocal8Bit(WALK_Buff);
//    bool check = WALKBuff.contains("ResponseCode");
//    if(check == false)
//    {
//        miscObj.DisplayMsgBox("Invalid Data");
//        uidmessage::instance()->hide_Message();
//        return FUNCTION_FAILED;
//    }


//    WALKBuff.remove("\\");
//    WALKBuff.remove("{");
//    WALKBuff.remove("}");
//    WALKBuff.remove('"');
//    WALKBuff.remove("[");
//    WALKBuff.remove("]");
//    qDebug()<<"WALKBuff"<<WALKBuff;
//    QStringList query = WALKBuff.split(",");
//    for(int i=0 ; i < query.length() ; i++)
//        qDebug()<<"Query Response"<<query.at(i);
//    //      QStringList RequestId = query.filter("RequestId");
//    //      QStringList DisplayMessage = query.filter("DisplayMessage");
//    //      QStringList ResponseCode = query.filter("ResponseCode");
//    //      uidmessage::instance()->hide_Message();

//    //      QString str = RequestId.at(2);

//    //      QString strToFind = "RequestId:";
//    //      QString value = str.mid(str.indexOf(strToFind)+strToFind.length());

//    //      str = ResponseCode.at(0);
//    //      strToFind = ":";
//    //      QString value1 = str.mid(str.indexOf(strToFind)+strToFind.length());

//    //      str = DisplayMessage.at(2);
//    //      QString value2 = str.mid(str.indexOf(strToFind)+strToFind.length());

//    //      qDebug()<<"value"<<value;
//    //      qDebug()<<"value1"<<value1;
//    //      qDebug()<<"value2"<<value2;
//    //       memcpy(WALK.ucRDRequestId,value.trimmed().toAscii().data(), strlen(value.trimmed().toAscii().data()));
//    //       qDebug()<<"WALK.ucRDRequestId"<<WALK.ucRDRequestId;
//    //           if(value1 =="0")
//    //           {

//    //                ui->stackedWidget->setCurrentIndex(22);
//    //                return FUNCTION_SUCCESS;
//    //           }
//    //           else
//    //           {

//    //               sprintf(ErrorMsg,"Response Code:%s \n Message:%s",(char *)FORPASS_State.ucResponseCode,(char *)FORPASS_State.ucResponseMessage);
//    //               miscObj.DisplayMsgBox(ErrorMsg);
//    //               uidmessage::instance()->hide_Message();
//    //               return FUNCTION_FAILED;
//    //           }


//    //            return FUNCTION_SUCCESS;



//}



//int FPB::iSetWALKLMT(int tagID, unsigned char *value)
//{
//    switch(tagID)
//    {
//    case  1:
//        strcpy((char *)value,(char *)walkinLimit.Commdata_WCL.ucRequestID_req);
//        break;
//    case  2:
//        strcpy((char *)value,(char *)walkinLimit.Commdata_WCL.ucMethodID_req);//Method id
//        break;
//    case  3:
//        strcpy((char *)value,(char *)walkinLimit.Commdata_WCL.ucSessionID_req);//teller id
//        break;
//    case  4:
//        strcpy((char *)value,(char *)walkinLimit.Commdata_WCL.ucTellerID_req);//session key
//        break;
//    case  5:
//        strcpy((char *)value,walkinLimit.Commdata_WCL.ucIsEncrypt_req);//token id
//        break;
//    case  6:
//        strcpy((char *)value,(char *)walkinLimit.Commdata_WCL.ucSessionExpiryTime_req);//is encrypt
//        break;
//    case  8:
//        strcpy((char *)value,(char *)walkinLimit.ucCustomerMob);//start date
//        break;
//    case  9:
//        strcpy((char *)value,(char *)walkinLimit.uctransgroup);//start date
//        break;
//    default:
//        return FUNC_ERROR;
//    }
//    return FUNC_OK;
//}



