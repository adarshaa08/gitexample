#include "FPB.h"
#include "ui_FPB.h"
#include "Login.h"
#include "impscommon.h"
#include "json.h"
#include "json_VT.h"
#include<fstream>
#include "Reversal.h"
#include "displaymessage.h"
Reversaldata ReversalDetails[10];
Reversaldata reve_data;
extern char * Convertstr(string str);
int currentIndex;
QList<QString> refNumList;
//extern char * Convertstr1(string str);
extern "C"
{
#include "gl11.h"
#include "http_client.h"
#include "uid_auth.h"
}
OTPIMPSNEFT OtpImpsNeft1;
extern Login_leg2 configLoginLeg2;
extern OTPIMPSNEFT OtpImpsNeft;
extern sCustDetails CustDet;
extern int OTP_page;
int Reversal_type;
int arrsize;
Reversal revesal;
RevRefire firefund;
int Resend_flag =0;
QString Ref_amount,Ref_charges,Total_amount;
int rev_display =0;
extern SUPPORTDATA_DEVICEINFO sd_deviceinfo;

void FPB::on_Reversal_Button_clicked()
{
    int iRet = 0;
    ui->Reversal_conf_button->setVisible(true);
    ui->Reversal_next_button->setText("Next");

    iRet = GetReversalRequest();
    if(iRet == FUNC_OK)
    {
        ui->stackedWidget->setCurrentIndex(41);
        return;

    }
    else
    {
        qDebug()<<"Failure";
        ui->stackedWidget->setCurrentIndex(15);
        return;
    }
}


int FPB:: GetReversalRequest()
{
    uidmessage::instance()->hide_Message();
    uidmessage::instance()->Display_BusyMessage("Please wait...");
    QSqlQuery query;
    int iRet = 0;
    char* jsondata;
    Json1::Value RequestedData,mainreq;
    char buff1[100];
    char ErrorMsg[100];
    char DspMsg[400];
    memset(buff1,0,sizeof(buff1));
    QString reference;
    CMisc miscObj;


    query.clear();
    query.exec("delete from REVERSAL_TXN");
    query.clear();

    iGenerateRequestID((char *)username,(unsigned char *)buff1);
    mainreq["RequestId"] =  buff1;
    mainreq["MethodId"] = 105;
    mainreq["TellerID"]= username;
    mainreq["SessionId"] = (char *)configLoginLeg2.ucSessionId;//buff_tmp.toAscii().data();
    qDebug()<<"SeSSION id"<<(char *)configLoginLeg2.ucSessionId;
    mainreq["TokenId"]= "";
    mainreq["IsEncrypt"]= false;
    mainreq["X_Auth_Token"] ="";
    mainreq["SessionExpiryTime"]= (char *)configLoginLeg2.ucSessionExpiryTime;//"10/11/2018 11:58:44 PM";
    mainreq["ChannelID"]= 1;
    mainreq["MerchantContribution"]= "";
    mainreq["Charges"]= "";
    mainreq["Amount"]= "";

    RequestedData["accountNumber"] ="";
    RequestedData["mobileNumber"] = (char *)FC_MobileNumber;

    serialize1(Convertstr1(RequestedData.toStyledString()));
    mainreq["RequestData"] = buff_tmp.toAscii().data();

    jsondata = Convertstr1(mainreq.toStyledString());

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

#else
    memset(str_url,0,sizeof(str_url));
#ifdef PRODUCTION
    strcpy(str_url,URL_CREATION(LOGOUT_URL_PROD));
#else
    strcpy(str_url,URL_CREATION(REVERSAL_URL));
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
        system("cd /usr/FINO/FPB/RESP/");
        system("rm /usr/FINO/FPB/RESP/GetReversal.TXT");
        int gRecv_len = strlen((char *)oBuff);
        FILE *fp2 = NULL;
        fp2 = fopen("/usr/FINO/FPB/RESP/GetReversal.TXT","w");
        if(fp2 == NULL)
        {
            return FUNCTION_FAILED;
        }
        qDebug()<<"File"<<fp2;
        fwrite( oBuff, 1, gRecv_len, fp2 );
        fclose(fp2);
        iRet = GetReversal_Parsing();
        if(iRet==FUNC_OK)
        {
            arrsize =0;
            ui->listWidget_Reversal_data->clear();
            query.clear();
            query.prepare("SELECT DISTINCT PARENT_TXN_ID FROM REVERSAL_TXN;");
            if(!query.exec())
            {
                qDebug() << query.lastError();
            }
            else
            {
                while(query.next())
                {
                    strcpy(ReversalDetails[arrsize].parentTxnId,query.value(0).toString().trimmed().toAscii().data());
                    arrsize++;
                }
            }


            qDebug()<<arrsize;
            qDebug()<<"reve_data[arrsize].parentTxnId"<<ReversalDetails[0].parentTxnId;

            for(int i = 0;i<arrsize;i++)
            {
                QString ref;
                ref.clear();
                char charges[20];
                qDebug()<<"reve_data[arrsize].parentTxnId"<<ReversalDetails[i].parentTxnId;
                ref = QString::fromUtf8((const char*)ReversalDetails[i].parentTxnId);
                qDebug()<<ref;
                getreversaldata(ref);
                query.clear();
                query.prepare("SELECT TXN_TIME,TXN_DATE,TRANSACTION_TYPE,PARENT_TXN_AMOUNT,CHARGE_AMOUNT FROM REVERSAL_TXN WHERE PARENT_TXN_ID = ? limit 1 ");
                query.addBindValue(ref);
                if(!query.exec())
                {
                    qDebug() << query.lastError();
                }
                else
                {

                    while(query.next())
                    {
                        memset(charges,0,strlen(charges));
                        memset(DspMsg,0,sizeof(DspMsg));
                        strcpy(charges,query.value(4).toString().trimmed().toAscii().data());
                        sprintf(DspMsg,"%s %s   %s\nAmount:%-8s          %s\nCharge:%s\n---------------------------------------------------",query.value(0).toString().trimmed().toAscii().data(),query.value(1).toString().trimmed().toAscii().data(),query.value(2).toString().trimmed().toAscii().data(),query.value(3).toString().trimmed().toAscii().data(),ReversalDetails[i].parentTxnId,&charges[1]);
                        ui->listWidget_Reversal_data->addItem(DspMsg);
                    }

                }
            }
                ui->listWidget_Reversal_data->setCurrentRow(0);
                uidmessage::instance()->hide_Message();
                return FUNC_OK;


        }
        else
        {

            if(strcmp((char *)revesal.Revdetail_response_msg,"Session Expired.")==0)
            {
                uidmessage::instance()->hide_Message();
                sprintf(ErrorMsg,"%s",(char *)revesal.Revdetail_response_msg);
                miscObj.DisplayMsgBox(ErrorMsg);
                uidmessage::instance()->hide_Message();
                return SESSION_EXPIRED_FAILED;
            }
            sprintf(ErrorMsg,"%s",(char *)revesal.Revdetail_response_msg);
            miscObj.DisplayMsgBox(ErrorMsg);
            uidmessage::instance()->hide_Message();
            return FUNCTION_FAILED;

        }

        return FUNCTION_SUCCESS;
}

int FPB::GetReversal_Parsing()
{
    system("sed 's/\"ResponseData\":\"{/\"ResponseData\":{/g' /usr/FINO/FPB/RESP/GetReversal.TXT > /tmp/1.txt");
    system("sed 's/}\",\"MessageId\"/},\"MessageId\"/g' /tmp/1.txt > /tmp/2.txt  ");
    system(" sed 's/\\\\//g' /tmp/2.txt > /usr/FINO/FPB/RESP/GetReversal.TXT");
    QSqlQuery query;
    Json1::Value root;
    root.clear();
    //QSqlQuery query;
    Json1::Reader reader;
    std::ifstream ReadData("/usr/FINO/FPB/RESP/GetReversal.TXT");
    bool parsedSuccess = reader.parse(ReadData,root,false);
    ReadData.close();
    if(not parsedSuccess)
    {
        qDebug()<<"FpsMonthlyPdsData File not Found";
        return FUNCAL_FAILED;
    }
    else
    {
        qDebug()<<"RequestId   : "<<Convertstr1(root["RequestId"].asString());
        qDebug()<<"DisplayMessage : "<<Convertstr1(root["DisplayMessage"].asString());
        qDebug()<<"ResponseMessage    : "<<Convertstr1(root["ResponseMessage"].asString());
        strcpy(revesal.Revdetail_responsecode,Convertstr1(root["ResponseCode"].asString()));
        strcpy(revesal.Revdetail_response_msg,Convertstr1(root["ResponseMessage"].asString()));

        if(strcmp((char *)revesal.Revdetail_responsecode,"0")==0)
        {
            qDebug()<<"After Condition Success";
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
                Json1::Value reversalTxnDetails;
                reversalTxnDetails.clear();
                reversalTxnDetails = ResponseData.get("reversalTxnDetails",0);
                qDebug()<<"Getting successfull PostTransactionResponse";
                printf("\nSize : %d\n",reversalTxnDetails.size());
                //  arrsize = reversalTxnDetails.size();
                if(reversalTxnDetails.size()==0)
                {
                    qDebug("\n1:Key Register Data Not Found\n");
                    return FUNC_ERROR;
                }
                else
                {
                    arrsize =0;
                    int j=0;

                    // ReversalDetails = (Reversaldata*) malloc(sizeof(Reversaldata)*arrsize);
                    for(int i=0;i<reversalTxnDetails.size();i++)
                    {
                        memset(&reve_data,0,sizeof(reve_data));
                        qDebug()<<"in  reversalTxnDetails success";
                        qDebug()<<"product:"<<Convertstr1(reversalTxnDetails[i]["product"].asString());
                        memcpy(reve_data.product,Convertstr1(reversalTxnDetails[i]["product"].asString()),strlen(Convertstr1(reversalTxnDetails[i]["product"].asString())));
                        qDebug()<<"trantype:"<<Convertstr1(reversalTxnDetails[i]["trantype"].asString());
                        memcpy(reve_data.trantype,Convertstr1(reversalTxnDetails[i]["trantype"].asString()),strlen(Convertstr1(reversalTxnDetails[i]["trantype"].asString())));
                        qDebug()<<"channel:"<<Convertstr1(reversalTxnDetails[i]["channel"].asString());
                        memcpy(reve_data.channel,Convertstr1(reversalTxnDetails[i]["channel"].asString()),strlen(Convertstr1(reversalTxnDetails[i]["channel"].asString())));
                        qDebug()<<"date:"<<Convertstr1(reversalTxnDetails[i]["date"].asString());
                        memcpy(reve_data.date,Convertstr1(reversalTxnDetails[i]["date"].asString()),strlen(Convertstr1(reversalTxnDetails[i]["date"].asString())));
                        qDebug()<<"customerNo:"<<Convertstr1(reversalTxnDetails[i]["customerNo"].asString());
                        memcpy(reve_data.customerNo,Convertstr1(reversalTxnDetails[i]["customerNo"].asString()),strlen(Convertstr1(reversalTxnDetails[i]["customerNo"].asString())));
                        qDebug()<<"accountNo:"<<Convertstr1(reversalTxnDetails[i]["accountNo"].asString());
                        memcpy(reve_data.accountNo,Convertstr1(reversalTxnDetails[i]["accountNo"].asString()),strlen(Convertstr1(reversalTxnDetails[i]["accountNo"].asString())));
                        qDebug()<<"amount:"<<Convertstr1(reversalTxnDetails[i]["amount"].asString());
                        memcpy(reve_data.amount,Convertstr1(reversalTxnDetails[i]["amount"].asString()),strlen(Convertstr1(reversalTxnDetails[i]["amount"].asString())));
                        qDebug()<<"branch:"<<Convertstr1(reversalTxnDetails[i]["branch"].asString());
                        memcpy(reve_data.branch,Convertstr1(reversalTxnDetails[i]["branch"].asString()),strlen(Convertstr1(reversalTxnDetails[i]["branch"].asString())));
                        qDebug()<<"userId:"<<Convertstr1(reversalTxnDetails[i]["userId"].asString());
                        memcpy(reve_data.userId,Convertstr1(reversalTxnDetails[i]["userId"].asString()),strlen(Convertstr1(reversalTxnDetails[i]["userId"].asString())));
                        qDebug()<<"sequenceNo:"<<Convertstr1(reversalTxnDetails[i]["sequenceNo"].asString());
                        memcpy(reve_data.sequenceNo,Convertstr1(reversalTxnDetails[i]["sequenceNo"].asString()),strlen(Convertstr1(reversalTxnDetails[i]["sequenceNo"].asString())));
                        qDebug()<<"referenceNo:"<<Convertstr1(reversalTxnDetails[i]["referenceNo"].asString());
                        memcpy(reve_data.referenceNo,Convertstr1(reversalTxnDetails[i]["referenceNo"].asString()),strlen(Convertstr1(reversalTxnDetails[i]["referenceNo"].asString())));
                        qDebug()<<"status:"<<Convertstr1(reversalTxnDetails[i]["status"].asString());
                        memcpy(reve_data.status,Convertstr1(reversalTxnDetails[i]["status"].asString()),strlen(Convertstr1(reversalTxnDetails[i]["status"].asString())));
                        qDebug()<<"reversalFlag:"<<Convertstr1(reversalTxnDetails[i]["reversalFlag"].asString());
                        memcpy(reve_data.reversalFlag,Convertstr1(reversalTxnDetails[i]["reversalFlag"].asString()),strlen(Convertstr1(reversalTxnDetails[i]["reversalFlag"].asString())));
                        qDebug()<<"narrative:"<<Convertstr1(reversalTxnDetails[i]["narrative"].asString());
                        memcpy(reve_data.narrative,Convertstr1(reversalTxnDetails[i]["narrative"].asString()),strlen(Convertstr1(reversalTxnDetails[i]["narrative"].asString())));
                        qDebug()<<"time:"<<Convertstr1(reversalTxnDetails[i]["time"].asString());
                        memcpy(reve_data.time,Convertstr1(reversalTxnDetails[i]["time"].asString()),strlen(Convertstr1(reversalTxnDetails[i]["time"].asString())));
                        qDebug()<<"customerName:"<<Convertstr1(reversalTxnDetails[i]["customerName"].asString());
                        memcpy(reve_data.customerName,Convertstr1(reversalTxnDetails[i]["customerName"].asString()),strlen(Convertstr1(reversalTxnDetails[i]["customerName"].asString())));
                        qDebug()<<"customerMobile:"<<Convertstr1(reversalTxnDetails[i]["customerMobile"].asString());
                        memcpy(reve_data.customerMobile,Convertstr1(reversalTxnDetails[i]["customerMobile"].asString()),strlen(Convertstr1(reversalTxnDetails[i]["customerMobile"].asString())));
                        qDebug()<<"splitSequence:"<<Convertstr1(reversalTxnDetails[i]["splitSequence"].asString());
                        memcpy(reve_data.splitSequence,Convertstr1(reversalTxnDetails[i]["splitSequence"].asString()),strlen(Convertstr1(reversalTxnDetails[i]["splitSequence"].asString())));
                        qDebug()<<"debitCredit:"<<Convertstr1(reversalTxnDetails[i]["debitCredit"].asString());
                        memcpy(reve_data.debitCredit,Convertstr1(reversalTxnDetails[i]["debitCredit"].asString()),strlen(Convertstr1(reversalTxnDetails[i]["debitCredit"].asString())));
                        qDebug()<<"chargeAmount:"<<Convertstr1(reversalTxnDetails[i]["chargeAmount"].asString());
                        memcpy(reve_data.chargeAmount,Convertstr1(reversalTxnDetails[i]["chargeAmount"].asString()),strlen(Convertstr1(reversalTxnDetails[i]["chargeAmount"].asString())));
                        qDebug()<<"reverseCommissionAmount:"<<Convertstr1(reversalTxnDetails[i]["reverseCommissionAmount"].asString());
                        memcpy(reve_data.reverseCommissionAmount,Convertstr1(reversalTxnDetails[i]["reverseCommissionAmount"].asString()),strlen(Convertstr1(reversalTxnDetails[i]["reverseCommissionAmount"].asString())));
                        qDebug()<<"reverseTargetAmount:"<<Convertstr1(reversalTxnDetails[i]["reverseTargetAmount"].asString());
                        memcpy(reve_data.reverseTargetAmount,Convertstr1(reversalTxnDetails[i]["reverseTargetAmount"].asString()),strlen(Convertstr1(reversalTxnDetails[i]["reverseTargetAmount"].asString())));
                        qDebug()<<"rfuNumeric1Desc:"<<Convertstr1(reversalTxnDetails[i]["rfuNumeric1Desc"].asString());
                        memcpy(reve_data.rfuNumeric1Desc,Convertstr1(reversalTxnDetails[i]["rfuNumeric1Desc"].asString()),strlen(Convertstr1(reversalTxnDetails[i]["rfuNumeric1Desc"].asString())));
                        qDebug()<<"rfuNumeric1Val:"<<Convertstr1(reversalTxnDetails[i]["rfuNumeric1Val"].asString());
                        memcpy(reve_data.rfuNumeric1Val,Convertstr1(reversalTxnDetails[i]["rfuNumeric1Val"].asString()),strlen(Convertstr1(reversalTxnDetails[i]["rfuNumeric1Val"].asString())));
                        qDebug()<<"rfuNumeric3Desc:"<<Convertstr1(reversalTxnDetails[i]["rfuNumeric3Desc"].asString());
                        memcpy(reve_data.rfuNumeric3Desc,Convertstr1(reversalTxnDetails[i]["rfuNumeric3Desc"].asString()),strlen(Convertstr1(reversalTxnDetails[i]["rfuNumeric3Desc"].asString())));
                        qDebug()<<"rfuNumeric3Val:"<<Convertstr1(reversalTxnDetails[i]["rfuNumeric3Val"].asString());
                        memcpy(reve_data.rfuNumeric3Val,Convertstr1(reversalTxnDetails[i]["rfuNumeric3Val"].asString()),strlen(Convertstr1(reversalTxnDetails[i]["rfuNumeric3Val"].asString())));
                        qDebug()<<"rfuNumeric4Desc:"<<Convertstr1(reversalTxnDetails[i]["rfuNumeric4Desc"].asString());
                        memcpy(reve_data.rfuNumeric4Desc,Convertstr1(reversalTxnDetails[i]["rfuNumeric4Desc"].asString()),strlen(Convertstr1(reversalTxnDetails[i]["rfuNumeric4Desc"].asString())));
                        qDebug()<<"rfuNumeric4Val:"<<Convertstr1(reversalTxnDetails[i]["rfuNumeric4Val"].asString());
                        memcpy(reve_data.rfuNumeric4Val,Convertstr1(reversalTxnDetails[i]["rfuNumeric4Val"].asString()),strlen(Convertstr1(reversalTxnDetails[i]["rfuNumeric4Val"].asString())));
                        qDebug()<<"rfuText1Desc:"<<Convertstr1(reversalTxnDetails[i]["rfuText1Desc"].asString());
                        memcpy(reve_data.rfuText1Desc,Convertstr1(reversalTxnDetails[i]["rfuText1Desc"].asString()),strlen(Convertstr1(reversalTxnDetails[i]["rfuText1Desc"].asString())));
                        printf("reve_data.rfuText1Desc====%s",reve_data.rfuText1Desc);
                        printf("reve_data.rfuText1Desc====%s",reve_data.rfuText1Desc);
                        printf("reve_data.rfuText1Desc====%s",reve_data.rfuText1Desc);
                        printf("reve_data.rfuText1Desc====%s",reve_data.rfuText1Desc);
                        qDebug()<<"rfuText1Desc:============"<<reve_data.rfuText1Desc;
                        qDebug()<<"rfuText1Desc:============"<<reve_data.rfuText1Desc;

                        qDebug()<<"rfuText1Val:"<<Convertstr1(reversalTxnDetails[i]["rfuText1Val"].asString());
                        memcpy(reve_data.rfuText1Val,Convertstr1(reversalTxnDetails[i]["rfuText1Val"].asString()),strlen(Convertstr1(reversalTxnDetails[i]["rfuText1Val"].asString())));
                        qDebug()<<"rfuText2Desc:"<<Convertstr1(reversalTxnDetails[i]["rfuText2Desc"].asString());
                        memcpy(reve_data.rfuText2Desc,Convertstr1(reversalTxnDetails[i]["rfuText2Desc"].asString()),strlen(Convertstr1(reversalTxnDetails[i]["rfuText2Desc"].asString())));
                        qDebug()<<"rfuText2Val:"<<Convertstr1(reversalTxnDetails[i]["rfuText2Val"].asString());
                        memcpy(reve_data.rfuText2Val,Convertstr1(reversalTxnDetails[i]["rfuText2Val"].asString()),strlen(Convertstr1(reversalTxnDetails[i]["rfuText2Val"].asString())));
                        qDebug()<<"rfuText3Desc:"<<Convertstr1(reversalTxnDetails[i]["rfuText3Desc"].asString());
                        memcpy(reve_data.rfuText3Desc,Convertstr1(reversalTxnDetails[i]["rfuText3Desc"].asString()),strlen(Convertstr1(reversalTxnDetails[i]["rfuText3Desc"].asString())));
                        qDebug()<<"rfuText3Val:"<<Convertstr1(reversalTxnDetails[i]["rfuText3Val"].asString());
                        memcpy(reve_data.rfuText3Val,Convertstr1(reversalTxnDetails[i]["rfuText3Val"].asString()),strlen(Convertstr1(reversalTxnDetails[i]["rfuText3Val"].asString())));
                        qDebug()<<"rfuText5Desc:"<<Convertstr1(reversalTxnDetails[i]["rfuText5Desc"].asString());
                        memcpy(reve_data.rfuText5Desc,Convertstr1(reversalTxnDetails[i]["rfuText5Desc"].asString()),strlen(Convertstr1(reversalTxnDetails[i]["rfuText5Desc"].asString())));
                        qDebug()<<"rfuText5Val:"<<Convertstr1(reversalTxnDetails[i]["rfuText5Val"].asString());
                        memcpy(reve_data.rfuText5Val,Convertstr1(reversalTxnDetails[i]["rfuText5Val"].asString()),strlen(Convertstr1(reversalTxnDetails[i]["rfuText5Val"].asString())));
                        qDebug()<<"rfuText8Desc:"<<Convertstr1(reversalTxnDetails[i]["rfuText8Desc"].asString());
                        memcpy(reve_data.rfuText8Desc,Convertstr1(reversalTxnDetails[i]["rfuText8Desc"].asString()),strlen(Convertstr1(reversalTxnDetails[i]["rfuText8Desc"].asString())));
                        qDebug()<<"rfuText8Val:"<<Convertstr1(reversalTxnDetails[i]["rfuText8Val"].asString());
                        memcpy(reve_data.rfuText8Val,Convertstr1(reversalTxnDetails[i]["rfuText8Val"].asString()),strlen(Convertstr1(reversalTxnDetails[i]["rfuText8Val"].asString())));
                        qDebug()<<"rfuText9Desc:"<<Convertstr1(reversalTxnDetails[i]["rfuText9Desc"].asString());
                        memcpy(reve_data.rfuText9Desc,Convertstr1(reversalTxnDetails[i]["rfuText9Desc"].asString()),strlen(Convertstr1(reversalTxnDetails[i]["rfuText9Desc"].asString())));
                        qDebug()<<"rfuText9Val:"<<Convertstr1(reversalTxnDetails[i]["rfuText9Val"].asString());
                        memcpy(reve_data.rfuText9Val,Convertstr1(reversalTxnDetails[i]["rfuText9Val"].asString()),strlen(Convertstr1(reversalTxnDetails[i]["rfuText9Val"].asString())));
                        qDebug()<<"originalSequenceNo:"<<Convertstr1(reversalTxnDetails[i]["originalSequenceNo"].asString());
                        memcpy(reve_data.originalSequenceNo,Convertstr1(reversalTxnDetails[i]["originalSequenceNo"].asString()),strlen(Convertstr1(reversalTxnDetails[i]["originalSequenceNo"].asString())));
                        qDebug()<<"parentTxnId:"<<Convertstr1(reversalTxnDetails[i]["parentTxnId"].asString());
                        memcpy(reve_data.parentTxnId,Convertstr1(reversalTxnDetails[i]["parentTxnId"].asString()),strlen(Convertstr1(reversalTxnDetails[i]["parentTxnId"].asString())));
                        qDebug()<<"parentTxnAmount:"<<Convertstr1(reversalTxnDetails[i]["parentTxnAmount"].asString());
                        memcpy(reve_data.parentTxnAmount,Convertstr1(reversalTxnDetails[i]["parentTxnAmount"].asString()),strlen(Convertstr1(reversalTxnDetails[i]["parentTxnAmount"].asString())));

                        query.clear();
                        query.prepare("INSERT INTO REVERSAL_TXN (SEQUENCE_NO, CHANNEL_ID,  REFERENCE_NO,  TXN_DATE,  AMOUNT, TRANSACTION_TYPE, PARENT_TXN_ID,  CHARGE_AMOUNT,  ORIGINAL_SEQ_NO,PRODUCT,  CUSTOMER_NO, ACCOUNT_NO, BRANCH, USER_ID, STATUS, REVERSAL_FLAG, NARRATIVE, TXN_TIME, CUSTOMER_NAME, CUSTOMER_MOBILE,SPLIT_SEQUENCE, DEBIT_CREDIT, PAYOUT_AMOUNT,COMMISSION_AMOUNT, TDSAMOUNT, RFU_DATE_DESC,RFU_DATE_VAL,RFU_NUMERIC1_DESC, RFU_NUMERIC1_VAL,RFU_NUMERIC2_DESC, RFU_NUMERIC2_VAL,RFU_NUMERIC3_DESC, RFU_NUMERIC3_VAL,RFU_NUMERIC4_DESC, RFU_NUMERIC4_VAL,RFU_TEXT1_DESC, RFU_TEXT1_VAL, RFU_TEXT2_DESC, RFU_TEXT2_VAL, RFU_TEXT3_DESC, RFU_TEXT3_VAL, RFU_TEXT4_DESC, RFU_TEXT4_VAL, RFU_TEXT5_DESC, RFU_TEXT5_VAL, RFU_TEXT6_DESC, RFU_TEXT6_VAL, RFU_TEXT7_DESC, RFU_TEXT7_VAL, RFU_TEXT8_DESC, RFU_TEXT8_VAL, RFU_TEXT9_DESC, RFU_TEXT9_VAL, RFU_TEXT10_DESC,RFU_TEXT10_VAL,PARENT_TXN_AMOUNT_CUR,CHARGE_AMOUNT_CUR,PARENT_TXN_AMOUNT) values (:SEQUENCE_NO, :CHANNEL_ID,  :REFERENCE_NO,  :TXN_DATE,  :AMOUNT, :TRANSACTION_TYPE, :PARENT_TXN_ID,  :CHARGE_AMOUNT,  :ORIGINAL_SEQ_NO,:PRODUCT,  :CUSTOMER_NO, :ACCOUNT_NO, :BRANCH, :USER_ID, :STATUS, :REVERSAL_FLAG, :NARRATIVE, :TXN_TIME, :CUSTOMER_NAME, :CUSTOMER_MOBILE,:SPLIT_SEQUENCE, :DEBIT_CREDIT, :PAYOUT_AMOUNT,:COMMISSION_AMOUNT, :TDSAMOUNT, :RFU_DATE_DESC,:RFU_DATE_VAL,:RFU_NUMERIC1_DESC, :RFU_NUMERIC1_VAL,:RFU_NUMERIC2_DESC, :RFU_NUMERIC2_VAL,:RFU_NUMERIC3_DESC, :RFU_NUMERIC3_VAL,:RFU_NUMERIC4_DESC, :RFU_NUMERIC4_VAL,:RFU_TEXT1_DESC, :RFU_TEXT1_VAL, :RFU_TEXT2_DESC, :RFU_TEXT2_VAL, :RFU_TEXT3_DESC, :RFU_TEXT3_VAL, :RFU_TEXT4_DESC, :RFU_TEXT4_VAL, :RFU_TEXT5_DESC, :RFU_TEXT5_VAL, :RFU_TEXT6_DESC, :RFU_TEXT6_VAL, :RFU_TEXT7_DESC, :RFU_TEXT7_VAL, :RFU_TEXT8_DESC, :RFU_TEXT8_VAL, :RFU_TEXT9_DESC, :RFU_TEXT9_VAL, :RFU_TEXT10_DESC,:RFU_TEXT10_VAL,:PARENT_TXN_AMOUNT_CUR,:CHARGE_AMOUNT_CUR,:PARENT_TXN_AMOUNT)");
                        query.bindValue(":SEQUENCE_NO",reve_data.sequenceNo);
                        query.bindValue(":CHANNEL_ID",reve_data.channel);
                        query.bindValue(":REFERENCE_NO",reve_data.referenceNo);
                        query.bindValue(":TXN_DATE",reve_data.date);
                        query.bindValue(":AMOUNT",reve_data.amount);
                        query.bindValue(":TRANSACTION_TYPE",reve_data.trantype);
                        query.bindValue(":PARENT_TXN_ID",reve_data.parentTxnId);
                        query.bindValue(":CHARGE_AMOUNT",reve_data.chargeAmount);
                        query.bindValue(":ORIGINAL_SEQ_NO",reve_data.originalSequenceNo);
                        query.bindValue(":PRODUCT",reve_data.product);
                        query.bindValue(":CUSTOMER_NO",reve_data.customerNo);
                        query.bindValue(":ACCOUNT_NO",reve_data.accountNo);
                        query.bindValue(":BRANCH",reve_data.branch);
                        query.bindValue(":USER_ID",reve_data.userId);
                        query.bindValue(":STATUS",reve_data.status);
                        query.bindValue(":REVERSAL_FLAG",reve_data.reversalFlag);
                        query.bindValue(":NARRATIVE",reve_data.narrative);
                        query.bindValue(":TXN_TIME",reve_data.time);
                        query.bindValue(":CUSTOMER_NAME",reve_data.customerName);
                        query.bindValue(":CUSTOMER_MOBILE",reve_data.customerMobile);
                        query.bindValue(":SPLIT_SEQUENCE",reve_data.splitSequence);
                        query.bindValue(":DEBIT_CREDIT",reve_data.debitCredit);
                        query.bindValue(":PAYOUT_AMOUNT",reve_data.payoutamt);
                        query.bindValue(":COMMISSION_AMOUNT",reve_data.reverseCommissionAmount);
                        query.bindValue(":TDSAMOUNT",reve_data.reverseTargetAmount);
                        query.bindValue(":RFU_DATE_DESC",reve_data.rfudateDesc);
                        query.bindValue(":RFU_DATE_VAL",reve_data.rfudateval);
                        query.bindValue(":RFU_NUMERIC1_DESC",reve_data.rfuNumeric1Desc);
                        query.bindValue(":RFU_NUMERIC1_VAL",reve_data.rfuNumeric1Val);
                        query.bindValue(":RFU_NUMERIC2_DESC",reve_data.rfuNumeric2Desc);
                        query.bindValue(":RFU_NUMERIC2_VAL",reve_data.rfuNumeric2Val);
                        query.bindValue(":RFU_NUMERIC3_DESC",reve_data.rfuNumeric3Desc);
                        query.bindValue(":RFU_NUMERIC3_VAL",reve_data.rfuNumeric3Val);
                        query.bindValue(":RFU_NUMERIC4_DESC",reve_data.rfuNumeric4Desc);
                        query.bindValue(":RFU_NUMERIC4_VAL",reve_data.rfuNumeric4Val);
                        query.bindValue(":RFU_TEXT1_DESC",reve_data.rfuText1Desc);
                        query.bindValue(":RFU_TEXT1_VAL",reve_data.rfuText1Val);
                        query.bindValue(":RFU_TEXT2_DESC",reve_data.rfuText2Desc);
                        query.bindValue(":RFU_TEXT2_VAL",reve_data.rfuText2Val);
                        query.bindValue(":RFU_TEXT3_DESC",reve_data.rfuText3Desc);
                        query.bindValue(":RFU_TEXT3_VAL",reve_data.rfuText3Val);
                        query.bindValue(":RFU_TEXT4_DESC",reve_data.rfuText4Desc);
                        query.bindValue(":RFU_TEXT4_VAL",reve_data.rfuText4Val);
                        query.bindValue(":RFU_TEXT5_DESC",reve_data.rfuText5Desc);
                        query.bindValue(":RFU_TEXT5_VAL",reve_data.rfuText5Val);
                        query.bindValue(":RFU_TEXT6_DESC",reve_data.rfuText6Desc);
                        query.bindValue(":RFU_TEXT6_VAL",reve_data.rfuText6Val);
                        query.bindValue(":RFU_TEXT7_DESC",reve_data.rfuText7Desc);
                        query.bindValue(":RFU_TEXT7_VAL",reve_data.rfuText7Val);
                        query.bindValue(":RFU_TEXT8_DESC",reve_data.rfuText8Desc);
                        query.bindValue(":RFU_TEXT8_VAL",reve_data.rfuText8Val);
                        query.bindValue(":RFU_TEXT9_DESC",reve_data.rfuText9Desc);
                        query.bindValue(":RFU_TEXT9_VAL",reve_data.rfuText9Val);
                        query.bindValue(":RFU_TEXT10_DESC",reve_data.rfuText10Desc);
                        query.bindValue(":RFU_TEXT10_VAL",reve_data.rfuText10Val);
                        query.bindValue(":PARENT_TXN_AMOUNT_CUR",reve_data.parentcurr);
                        query.bindValue(":CHARGE_AMOUNT_CUR",reve_data.chargecurr);
                        query.bindValue(":PARENT_TXN_AMOUNT",reve_data.parentTxnAmount);
                        if(!query.exec())
                            qDebug() << "get reversal" << ":" << query.lastError();


                        arrsize= j;
                        qDebug()<<"arrsize"<<arrsize;

                        j++;
                    }
                }
            }

        }
        else
        {

            return FUNCAL_FAILED;
        }
    }
    return FUNC_OK;
}



void FPB::on_listWidget_Reversal_data_clicked(const QModelIndex &index)
{

    qDebug()<<"success";
    qDebug()<<"current row"<<ui->listWidget_Reversal_data->currentRow();
    char cDspMsg[400];
    ui->btnRefund->setVisible(true);
    ui->btnRefire->setVisible(true);
    currentIndex = 0;
    currentIndex = ui->listWidget_Reversal_data->currentRow();
    ui->Rev_display_list->clear();
    getreferenceno(QString::fromUtf8((const char*)ReversalDetails[currentIndex].parentTxnId));
    for(int i =0;i<refNumList.size();i++)
    {
        qDebug()<<"refNumList[i]=========="<<refNumList[i];
    }
    memset(cDspMsg,0,sizeof(cDspMsg));

    if(strlen(ReversalDetails[currentIndex].rfuText5Desc)==0)
    {
        sprintf(cDspMsg,"%-24s:%s\n%-24s:%s\n%-24s:%s\n%-24s:%s\n%-24s:%s\n%-24s:%s\n%-24s:%s\n",ReversalDetails[currentIndex].rfuNumeric3Desc,ReversalDetails[currentIndex].rfuNumeric3Val,ReversalDetails[currentIndex].rfuNumeric4Desc,ReversalDetails[currentIndex].rfuNumeric4Val,ReversalDetails[currentIndex].rfuText1Desc,ReversalDetails[currentIndex].rfuText1Val,ReversalDetails[currentIndex].rfuText2Desc,ReversalDetails[currentIndex].rfuText2Val,ReversalDetails[currentIndex].rfuText3Desc,ReversalDetails[currentIndex].rfuText3Val,ReversalDetails[currentIndex].rfuText8Desc,ReversalDetails[currentIndex].rfuText8Val,ReversalDetails[currentIndex].rfuText9Desc,ReversalDetails[currentIndex].rfuText9Val);
    }
    else
    {
        sprintf(cDspMsg,"%-24s:%s\n%-24s:%s\n%-24s:%s\n%-24s:%s\n%-24s:%s\n%-24s:%s\n%-24s:%s\n%-24s:%s\n",ReversalDetails[currentIndex].rfuNumeric3Desc,ReversalDetails[currentIndex].rfuNumeric3Val,ReversalDetails[currentIndex].rfuNumeric4Desc,ReversalDetails[currentIndex].rfuNumeric4Val,ReversalDetails[currentIndex].rfuText1Desc,ReversalDetails[currentIndex].rfuText1Val,ReversalDetails[currentIndex].rfuText2Desc,ReversalDetails[currentIndex].rfuText2Val,ReversalDetails[currentIndex].rfuText5Desc,ReversalDetails[currentIndex].rfuText5Val,ReversalDetails[currentIndex].rfuText3Desc,ReversalDetails[currentIndex].rfuText3Val,ReversalDetails[currentIndex].rfuText8Desc,ReversalDetails[currentIndex].rfuText8Val,ReversalDetails[currentIndex].rfuText9Desc,ReversalDetails[currentIndex].rfuText9Val);
    }

    qDebug()<<"cDspMsg"<<cDspMsg;
    ui->Rev_display_list->addItem(cDspMsg);
    printf("ReversalDetails[currentIndex].userId%s\n",ReversalDetails[currentIndex].userId);



    if(strcmp(ReversalDetails[currentIndex].userId,username)!=0)
    {
        ui->btnRefund->hide();
        ui->btnRefire->hide();
        qDebug()<<"Inside the compare fiels";

    }
    ui->stackedWidget->setCurrentIndex(42);
}

void FPB::on_btnRefund_clicked()
{
    QString remove_amount = QString::fromUtf8((const char*)ReversalDetails[currentIndex].parentTxnAmount).remove("-");
    Ref_amount = remove_amount;
    qDebug()<<"Amount"<<Ref_amount;
    remove_amount = QString::fromUtf8((const char*)ReversalDetails[currentIndex].chargeAmount).remove("-");
    Ref_charges = remove_amount;
    qDebug()<<"Amount"<<Ref_charges;
    remove_amount.clear();
    Total_amount.clear();
    float temp = Ref_charges.toFloat() + Ref_amount.toFloat();
    qDebug()<<"temp"<<temp;
    Total_amount = QString::number(temp);
    qDebug()<<"Total_amount"<<Total_amount;
//    Ref_amount = Ref_amount.append(".00");
//    Ref_charges = Ref_charges.append(".00");
//    Total_amount = Total_amount.append(".00");
    QString narrative = QString::fromLocal8Bit(ReversalDetails[currentIndex].narrative);
    QStringList query = narrative.split("/");
    narrative.clear();
    narrative = query.at(1);

    ui->Reversal_label->setText("    CASH WITHDRAWAL TXN");

    rev_display = 0;
    char display_confirmpage[10][120];
    sprintf(display_confirmpage[0],"Amount               : %s",Ref_amount.toAscii().data());
    sprintf(display_confirmpage[1],"Charges              : %s",Ref_charges.toAscii().data());
    sprintf(display_confirmpage[2],"Total Amount         : %s",Total_amount.toAscii().data());


    int i=0;
    qDebug()<<"Display";
    ui->Confirm_rev_list->clear(); // clear list
    ui->Confirm_rev_list->setFont(QFont("Courier",12));

    for(i=0;i<3;i++)
    {
#if DEBUG
        printf("display_confirmpage    =====    %s\n",display_confirmpage[i]);
#endif
        ui->Confirm_rev_list->addItem((char *)display_confirmpage[i]);
    }
    Reversal_type = 0;
    ui->stackedWidget->setCurrentIndex(43);

    return;


}




int FPB::Reversal_otp_request()
{
    int iRet = 0;
    char* jsondata;
    CMisc miscObj;
    Json1::Value RequestedData,mainreq;
    char buff1[100];
    memset(buff1,0,sizeof(buff1));

    uidmessage::instance()->hide_Message();
    uidmessage::instance()->Display_BusyMessage("Please Wait....");

    iGenerateRequestID((char *)username,(unsigned char *)buff1);

    mainreq["RequestId"] =  buff1;
    if(Resend_flag == 1)
    {
        mainreq["MethodId"] = 4;
    }
    else
    {
        mainreq["MethodId"] = 1;
    }
    mainreq["TellerID"]= username;
    mainreq["SessionId"] = (char *)configLoginLeg2.ucSessionId;//buff_tmp.toAscii().data();
    qDebug()<<"SeSSION id"<<(char *)configLoginLeg2.ucSessionId;
    mainreq["TokenId"]= "";
    mainreq["IsEncrypt"]= false;
    mainreq["X_Auth_Token"] ="";
    mainreq["SessionExpiryTime"]= (char *)configLoginLeg2.ucSessionExpiryTime;//"10/11/2018 11:58:44 PM";
    mainreq["ChannelID"]= 1;
    mainreq["ProductCode"]= "";
    mainreq["Amount"]= "";
//    mainreq["JsonFormatter"]= true;


    RequestedData["MethodId"]= "1";
    RequestedData["CustomerMobileNo"] =(char *)FC_MobileNumber;//ReversalDetails[currentIndex].rfuNumeric4Val;
    if(Reversal_type == 1)
    {
        RequestedData["MessageId"]="458";
    }
    else if(Reversal_type == 0)
    {
        RequestedData["MessageId"]="459";
    }
    RequestedData["OtpParam"]["AccountNo"] ="";
    if(Reversal_type == 1)
    {
        RequestedData["OtpParam"]["Amount"] =Ref_amount.toAscii().data();
    }
    else if(Reversal_type == 0)
    {
        RequestedData["OtpParam"]["Amount"] =Total_amount.toAscii().data();
    }
    RequestedData["OtpParam"]["BenefiAccountNo"] =ReversalDetails[currentIndex].rfuText1Val;
    RequestedData["OtpParam"]["Benename"] =ReversalDetails[currentIndex].rfuText5Val;
    RequestedData["OtpParam"]["CustomerCareNo"] =ReversalDetails[currentIndex].customerNo;


    serialize1(Convertstr1(RequestedData.toStyledString()));
    mainreq["RequestData"] = buff_tmp.toAscii().data();

    jsondata = Convertstr1(mainreq.toStyledString());

#if DEBUG
    qDebug()<<"------------->"<<jsondata;
#endif
    //-------------------------------------------------------------
    memset(gSendData,0,sizeof(gSendData));
    strncpy((char*)gSendData, jsondata, strlen(jsondata));
#if DEBUG
    printf("gSendData = %s\n\n",gSendData);
#endif

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
    strcpy(str_url,URL_CREATION(OLDNEW_CHANGE_PASSWORD_URL));
#endif

    int retk = file_download(str_url, (char *)gSendData,"/usr/FINO/FPB/RESP/Reversal_OTP_Refund.TXT",120);
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

      FILE *fp1 = fopen("/usr/FINO/FPB/RESP/Reversal_OTP_Refund.TXT", "rb"); // open
      if (fp1 == NULL)
      {
          qDebug()<<"Can't Open IntJSON.TXT File";
          //uidmessage::instance()->hide_Message();
          return FUNCTION_FAILED;
      }

      fseek(fp1, 0, SEEK_END);
      int template_len = ftell(fp1);

      fseek(fp1, 0, SEEK_SET);
      int ret=fread(oBuff,sizeof(unsigned char ),template_len,fp1);
      if(ret==0)
          fclose(fp1);

      QString OTPBuff = QString::fromLocal8Bit(oBuff);//Get the ouput buffer in QString

      bool check = OTPBuff.contains("ResponseCode");
      if(check == false)
      {
          //miscObj.DisplayMsgBox("Invalid data");
        //  uidmessage::instance()->hide_Message();
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

      //uidmessage::instance()->hide_Message();
      QString str ,strToFind,value,value1,value2;
      str = ResponseCode.at(0);
      strToFind = ":";
      value1 = str.mid(str.indexOf(strToFind)+strToFind.length());
      memset(revesal.Revotp_responsecode, 0, sizeof(revesal.Revotp_responsecode));
      memcpy(revesal.Revotp_responsecode,value1.trimmed().toAscii().data(), strlen(value1.trimmed().toAscii().data()));
   //  qDebug()<<"value1"<<value1;
      if(value1 =="0")
      {
          str = DisplayMessage.at(2);
          strToFind = ":";
          value2 = str.mid(str.indexOf(strToFind)+strToFind.length());
          str= RequestId.at(2);
          strToFind= "RequestId:";
          memset(revesal.Revotp_requestid,0,sizeof(revesal.Revotp_requestid));
          strcpy(revesal.Revotp_requestid,str.mid(str.indexOf(strToFind)+strToFind.length()).toAscii().data());
          qDebug()<<"RequestId"<<revesal.Revotp_requestid;
          char otp_mobileno[20], otp_message[700];
          memset(otp_mobileno, 0, sizeof(otp_mobileno));
          memset(otp_message, 0, sizeof(otp_message));
      //    sprintf(otp_mobileno,"%s",Mobile.toAscii().data());
          sprintf(otp_mobileno,"******%s",&FC_MobileNumber[6]);
          sprintf(otp_message, "One Time Password (OTP) has been\n sent to your registered mobile %s,\n Please enter the same here",otp_mobileno);
          ui->Otp_display_label->setText(otp_message);
          uidmessage::instance()->hide_Message();
          return FUNC_OK;
      }
      else
      {
          str = ResponseMessage.at(0);
          strToFind = ":";
          value2 = str.mid(str.indexOf(strToFind)+strToFind.length());
          uidmessage::instance()->hide_Message();
          return FUNCTION_FAILED;

       }
}

int FPB::Reversal_Refund_otp_Parsing()
{

    system("sed 's/\"ResponseData\":\"{/\"ResponseData\":{/g' /usr/FINO/FPB/RESP/OUTPUT.TXT > /tmp/1.txt");
    system("sed 's/}\",\"MessageId\"/},\"MessageId\"/g' /tmp/1.txt > /tmp/2.txt  ");
    system(" sed 's/\\\\//g' /tmp/2.txt > /usr/FINO/FPB/RESP/OUTPUT.TXT");

    Json1::Value root;
    root.clear();
    //QSqlQuery query;
    Json1::Reader reader1;
    std::ifstream ReadData1("/usr/FINO/FPB/RESP/OUTPUT.TXT");
    bool parsedSuccess1 = reader1.parse(ReadData1,root,false);

    ReadData1.close();

    if(not parsedSuccess1)
    {

        qDebug()<<"FpsMonthlyPdsData File not Found";
        return FUNCAL_FAILED;
    }
    else
    {
        qDebug()<<"RequestId   : "<<Convertstr1(root["RequestId"].asString());
        qDebug()<<"DisplayMessage : "<<Convertstr1(root["DisplayMessage"].asString());
        qDebug()<<"ResponseMessage    : "<<Convertstr1(root["ResponseMessage"].asString());
        strcpy(firefund.ucResponseCode,Convertstr1(root["ResponseCode"].asString()));
        strcpy(firefund.ucResponseMswssage,Convertstr1(root["ResponseMessage"].asString()));
        qDebug()<<"ResponseCode : "<<Convertstr1(root["ResponseCode"].asString());
        if(memcmp(firefund.ucResponseCode,"0",1)==0)
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

                Json1::Value ResponseData1,ResponseData2;
                ResponseData1.clear();
                ResponseData1 = ResponseData.get("PostTransactionResponse",0);

                qDebug()<<"Getting successfull PostTransactionResponse";
                for(int j=0;j<ResponseData1.size();j++)
                {   qDebug()<<"valueof j"<<j;
                    qDebug()<<"returnCode:"<<Convertstr(ResponseData1[j]["returnCode"].asString());
                    qDebug()<<"txnReferenceNo:"<<Convertstr(ResponseData1[j]["txnReferenceNo"].asString());
                    qDebug()<<"cbsTxnReferenceNo:"<<Convertstr(ResponseData1[j]["cbsTxnReferenceNo"].asString());
                    strcpy(firefund.txnReferenceNo,Convertstr(ResponseData1[j]["txnReferenceNo"].asString()));

                    ResponseData2 = ResponseData1[j].get("chargesList",0);
                    printf("\nSize : %d\n",ResponseData2.size());
                    if(ResponseData2.size()==0)
                    {
                        qDebug("\n1:Key Register Data Not Found\n");
                        return FUNC_ERROR;
                    }
                    else
                    {
                        for(int i = 0;i<ResponseData2.size();i++)
                        {

                            qDebug()<<"code:"<<Convertstr(ResponseData2[i]["code"].asString());
                            qDebug()<<"amount:"<<QString::number(ResponseData2[i]["amount"].asFloat());
                            qDebug()<<"description:"<<Convertstr(ResponseData2[i]["description"].asString());
                            qDebug()<<"chargeAcctNum:"<<Convertstr(ResponseData2[i]["chargeAcctNum"].asString());
                            strcpy(firefund.uccode[i],Convertstr(ResponseData2[i]["code"].asString()));
                            strcpy(firefund.ucamount[i],QString::number(ResponseData2[i]["amount"].asFloat()).toAscii().data());
                            strcpy(firefund.ucdescription[i],Convertstr(ResponseData2[i]["description"].asString()));
                            strcpy(firefund.ucchargeAcctNum[i],Convertstr(ResponseData2[i]["chargeAcctNum"].asString()));



                        }
                    }
                    ResponseData2.clear();
                    ResponseData2 = ResponseData1[j].get("balancesList",0);
                    printf("\nSize : %d\n",ResponseData2.size());
                    for(int i=0;i<ResponseData2.size();i++)
                    {
                        qDebug()<<"balancesList ledgerBalance : "<<Convertstr(ResponseData2[i]["ledgerBalance"].asString());         ////
                        qDebug()<<"balancesList availableBalance : "<<QString::number(ResponseData2[i]["availableBalance"].asFloat());   ////
                        qDebug()<<"balancesList currency : "<<Convertstr(ResponseData2[i]["currency"].asString());
                        strcpy(firefund.BAledgerBalance,Convertstr(ResponseData2[i]["ledgerBalance"].asString()));
                        strcpy(firefund.BAavailableBalance,QString::number(ResponseData2[i]["availableBalance"].asFloat()).toAscii().data());
                        strcpy(firefund.BAcurrency,Convertstr(ResponseData2[i]["currency"].asString()));
                    }

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

void FPB::on_btnRefire_clicked()
{
    QString remove_amount = QString::fromUtf8((const char*)ReversalDetails[currentIndex].parentTxnAmount).remove("-");
    Ref_amount = remove_amount;
    qDebug()<<"Amount"<<Ref_amount;
    remove_amount = QString::fromUtf8((const char*)ReversalDetails[currentIndex].chargeAmount).remove("-");
    Ref_charges = remove_amount;
    qDebug()<<"Amount"<<Ref_charges;
    remove_amount.clear();
    Total_amount.clear();
    float temp = Ref_charges.toFloat() + Ref_amount.toFloat();
    qDebug()<<"temp"<<temp;
    Total_amount = QString::number(temp);
    qDebug()<<"Total_amount"<<Total_amount;

    QString narrative = QString::fromLocal8Bit(ReversalDetails[currentIndex].narrative);
    QStringList query = narrative.split("/");
    narrative.clear();
    narrative = query.at(1);
    ui->Reversal_label->setText("   RE-INITIATE TXN");

    rev_display = 0;
    char display_confirmpage[10][120];
    sprintf(display_confirmpage[0],"Amount               : %s",Ref_amount.toAscii().data());
    sprintf(display_confirmpage[1],"Charges              : %s",Ref_charges.toAscii().data());
    sprintf(display_confirmpage[2],"Total Amount         : %s",Total_amount.toAscii().data());
    sprintf(display_confirmpage[3],"Transaction Type     : %s",narrative.toAscii().data());
    sprintf(display_confirmpage[4],"Bene Account Number  : %s",ReversalDetails[currentIndex].rfuText1Val);
    sprintf(display_confirmpage[5],"Bene IFSC            : %s",ReversalDetails[currentIndex].rfuText2Val);
    sprintf(display_confirmpage[6],"Bene Bank Name       : %s",ReversalDetails[currentIndex].rfuText3Val);
    sprintf(display_confirmpage[7],"Bene name            : %s",ReversalDetails[currentIndex].rfuText5Val);
    sprintf(display_confirmpage[8],"External Reference No: %s",ReversalDetails[currentIndex].rfuText8Val);
    sprintf(display_confirmpage[9],"Transaction ID       : %s",ReversalDetails[currentIndex].rfuText9Val);


    int i=0;
    qDebug()<<"Display";
    ui->Confirm_rev_list->clear(); // clear list
    ui->Confirm_rev_list->setFont(QFont("Courier",12));

    for(i=0;i<10;i++)
    {
#if DEBUG
        printf("display_confirmpage    =====    %s\n",display_confirmpage[i]);
#endif
        ui->Confirm_rev_list->addItem((char *)display_confirmpage[i]);
    }
    Reversal_type = 1;
    ui->stackedWidget->setCurrentIndex(43);
    return;
}


int FPB::Reversal_Refund_Request()
{
    char szDate[16], szTime[16];
    CMisc miscObj;
    char ErrorMsg[150];
    memset(&firefund,0,sizeof(firefund));
    char spiltstring[40];
    uidmessage::instance()->hide_Message();
    uidmessage::instance()->Display_BusyMessage("Please Wait....");
    QSqlQuery query;
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

    Clear_DeviceInfo();
    int iretDI = DeviceInfo();

    if(iretDI != FUNCTION_SUCCESS)
        return FUNCTION_FAILED;



    iGenerateRequestID((char *)username,(unsigned char *)&firefund.Ref_RequestId_req);
    memcpy(firefund.Ref_MethodId_req,"110",3);
    memcpy(firefund.Ref_TellerID_req,username,sizeof(username));
    memcpy(firefund.Ref_IsEncrypt_req,"false",5);
    memcpy(firefund.Ref_SessionId_req,(char *)configLoginLeg2.ucSessionId,strlen((char *)(char *)configLoginLeg2.ucSessionId));
    memcpy(firefund.Ref_impsserviceflag_req,"Y",1);
    memcpy(firefund.Ref_mobileNumber_req,(char *)FC_MobileNumber,strlen((char *)FC_MobileNumber));
    memcpy(firefund.Ref_Authid_req,"1",1);
    memcpy(firefund.Ref_otpMethodId_req,"2",1);
    memcpy(firefund.Ref_otpRequestId_req,revesal.Revotp_requestid,strlen(revesal.Revotp_requestid));
    memcpy(firefund.Ref_otppin_req,CustDet.cOtp,strlen(CustDet.cOtp));
    memcpy(firefund.Ref_otpMessageId_req,"18",2);
    memcpy(firefund.Ref_SessionExpiryTime_req,configLoginLeg2.ucSessionExpiryTime,strlen(configLoginLeg2.ucSessionExpiryTime));
    memcpy(firefund.Ref_ABappId_req,"FINOMERNP",9);
   // memcpy(firefund.Ref_amount_req,Total_amount.toAscii().data(),strlen(Total_amount.toAscii().data()));
    memcpy(firefund.Ref_amount_req,Total_amount.toAscii().data(),strlen(Total_amount.toAscii().data()));
   // memcpy(firefund.Ref_amount_req,"175.0",5);
    memcpy(firefund.Ref_amount_req,Total_amount.toAscii().data(),strlen(Total_amount.toAscii().data()));
    memcpy(firefund.Ref_currency_req,"INR",3);
    memcpy(firefund.Ref_DebitFlag_req,"D",1);
    memcpy(firefund.Ref_transactionType_req,"DMTCASHW",10);
    memcpy(firefund.Ref_transactionComment_req,"cashwithdraw",13);
    memcpy(firefund.Ref_costCenter_req,configLoginLeg2.ucBAcode,strlen(configLoginLeg2.ucBAcode));
    memcpy(firefund.Ref_ChannelID_req,CHANNEL_ID,strlen(CHANNEL_ID));
    memcpy(firefund.Ref_ProductCode_req,"9999",4);
    memcpy(firefund.Ref_RemeName_req,ReversalDetails[currentIndex].customerName,strlen(ReversalDetails[currentIndex].customerName));
    Ref_amount =Ref_amount.remove(".00");
    sprintf(spiltstring,"%s|%s|3",Ref_amount.toAscii().data(),ReversalDetails[currentIndex].referenceNo);
    memcpy(firefund.Ref_SplitReferenceNo_req,spiltstring,strlen(spiltstring));
    memcpy(firefund.Ref_RRN,ReversalDetails[currentIndex].referenceNo,strlen(ReversalDetails[currentIndex].referenceNo));
    memcpy(firefund.Ref_totalamount,Ref_amount.toAscii().data(),strlen(Ref_amount.toAscii().data()));


    //memcpy(firefund.Ref_SplitReferenceNo_req,"165|912315025105|3",18);
    memcpy(firefund.Ref_CreditFlag_req,"C",1);
    memcpy(firefund.Ref_merchantMobileNumber_req,(char *)configLoginLeg2.ucmobileNumber,strlen((char *)configLoginLeg2.ucmobileNumber));
    memcpy(firefund.Ref_analysisFlag_req,"0",1);
    memcpy(firefund.Ref_remarks_req,"Test",4);




    memcpy((char *)firefund.RefCustomernumber, FC_CustomerNumber, strlen(FC_CustomerNumber));  // 36 PVTXNID Previous Successful Txn ID ..length




    if(refNumList.size()==1)
    {
      memcpy((char *)firefund.RefsupportData_SPLTSEQ, "0", 1);  //  SPLTSEQ  1 - Txn Amount>PerTransactionLimit (Total) | 0: Txn Amount<=PerTransactionLimit (Total)
    }
    else
    {
        memcpy((char *)firefund.RefsupportData_SPLTSEQ, "1", 1);
    }
    Ref_charges =Ref_charges.remove(".00");
    memcpy((char *)firefund.RefsupportData_CHGAMT,Ref_charges.toAscii().data(),strlen(Ref_charges.toAscii().data()));  // 46 CHGAMT Charges (AEPS - 0)



    memset(szDate, 0, sizeof(szDate));
    memset(szTime, 0, sizeof(szTime));
    GetLocalTime(szDate, szTime);

    int ret	=  makeJSONREFUND();/*-------------Make REQUEST--------------*/
    if(ret!=FUNC_OK)
    {
        return FUNC_ERROR;
    }


    uidmessage::instance()->hide_Message();
    uidmessage::instance()->Display_BusyMessage("Transaction Request in Progress");

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
    strcpy(str_url,URL_CREATION(REVERSAL_URL));
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

        int iRetparse = Reversal_Refund_Parsing();
        if(iRetparse==FUNC_OK)
        {

            char iquery[500];
            query.clear();

            query.exec("delete from LastWithdrawalData");
            query.clear();
            memset(iquery,0,sizeof(iquery));
            sprintf(iquery, "INSERT INTO LastWithdrawalData (LAST_RRN, LAST_DATE) VALUES('%s','%s')", firefund.txnReferenceNo,szDate);
    #if DEBUG
            printf("QUERY 1 === %s\n",iquery);
    #endif
            if(!query.exec(iquery))
            {
                qDebug() << "In Insert_LastWithdrawalData = " << query.lastError();
                query.clear();
                query.finish();
            }
            query.clear();
            query.finish();

            QString bal = QString::fromUtf8((const char*)(const char*)firefund.BAavailableBalance);
            bool decimal;
            decimal = bal.contains('.');
            if(decimal == false)
            {
                bal = bal.append(".00");
            }
            memset(Balance_Display,0,strlen(Balance_Display));
            sprintf(Balance_Display,"Balance : %s\n",bal.toLatin1().data(),strlen(bal.toLatin1().data()));
            strncpy(Mer_Latest_Balance, bal.toLatin1().data(),strlen(bal.toLatin1().data()));
            ui->Dash_Latest_Balance_Label_pg12->setText(Balance_Display);
            ui->SubTransaction_Label1->setText((char *)configLoginLeg2.ucname);
            ui->SubTransaction_Label1->setFont(QFont("SansSerif",10));

            ui->SubTransaction_Label1->setAlignment(Qt::AlignRight | Qt::AlignBottom);
            printf("Balance_Display = %s\n",Balance_Display);
            ui->SubTransaction_Label2->setText(QString::fromUtf8((const char*)Balance_Display));
            ui->SubTransaction_Label2->setFont(QFont("SansSerif",9));
            ui->SubTransaction_Label2->setAlignment(Qt::AlignRight | Qt::AlignBottom);
            uidmessage::instance()->hide_Message();

            char display_confirmpage[5][100];
            sprintf(display_confirmpage[0],"Amount Debited       : %s",Ref_amount.toAscii().data());
            sprintf(display_confirmpage[1],"Charges              : %s",Ref_charges.toAscii().data());
            sprintf(display_confirmpage[2],"Total Amount         : %s",Total_amount.toAscii().data());
            sprintf(display_confirmpage[3],"Transaction No       : %s",firefund.txnReferenceNo);
            sprintf(display_confirmpage[4],"       Fund Transfer Successful");

            int i=0;
            qDebug()<<"Display";
            ui->Confirm_rev_list->clear(); // clear list
            ui->Confirm_rev_list->setFont(QFont("Courier",12));

            for(i=0;i<5;i++)
            {
        #if DEBUG
                printf("display_confirmpage    =====    %s\n",display_confirmpage[i]);
        #endif
                ui->Confirm_rev_list->addItem((char *)display_confirmpage[i]);
            }

            ui->Reversal_conf_button->setVisible(false);
            ui->Reversal_next_button->setText("Finish");
            rev_display = 1;
            return FUNCTION_SUCCESS;

        }
        else
        {

            if(strcmp((char *)firefund.ucResponseMswssage,"Session Expired.")==0)
            {
                uidmessage::instance()->hide_Message();
                sprintf(ErrorMsg,"%s",(char *)firefund.ucResponseMswssage);
                miscObj.DisplayMsgBox(ErrorMsg);
                uidmessage::instance()->hide_Message();
                return SESSION_EXPIRED_FAILED;
            }
            sprintf(ErrorMsg,"%s",(char *)firefund.ucResponseMswssage);
            miscObj.DisplayMsgBox(ErrorMsg);
            uidmessage::instance()->hide_Message();
            return FUNCTION_FAILED;

        }

        return FUNCTION_SUCCESS;

}


int FPB::Reversal_Refund_Parsing()
{

    system("sed 's/\"ResponseData\":\"{/\"ResponseData\":{/g' /usr/FINO/FPB/RESP/OUTPUT.TXT > /tmp/1.txt");
    system("sed 's/}\",\"MessageId\"/},\"MessageId\"/g' /tmp/1.txt > /tmp/2.txt  ");
    system(" sed 's/\\\\//g' /tmp/2.txt > /usr/FINO/FPB/RESP/OUTPUT.TXT");

    Json1::Value root;
    root.clear();
    //QSqlQuery query;
    Json1::Reader reader1;
    std::ifstream ReadData1("/usr/FINO/FPB/RESP/OUTPUT.TXT");
    bool parsedSuccess1 = reader1.parse(ReadData1,root,false);

    ReadData1.close();

    if(not parsedSuccess1)
    {

        qDebug()<<"FpsMonthlyPdsData File not Found";
        return FUNCAL_FAILED;
    }
    else
    {
        qDebug()<<"RequestId   : "<<Convertstr1(root["RequestId"].asString());
        qDebug()<<"DisplayMessage : "<<Convertstr1(root["DisplayMessage"].asString());
        qDebug()<<"ResponseMessage    : "<<Convertstr1(root["ResponseMessage"].asString());
        strcpy(firefund.ucResponseCode,Convertstr1(root["ResponseCode"].asString()));
        strcpy(firefund.ucResponseMswssage,Convertstr1(root["ResponseMessage"].asString()));
        qDebug()<<"ResponseCode : "<<Convertstr1(root["ResponseCode"].asString());
        if(memcmp(firefund.ucResponseCode,"0",1)==0)
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

                Json1::Value ResponseData1,ResponseData2;
                ResponseData1.clear();


                qDebug()<<"Getting successfull TransactionResponse";

                    qDebug()<<"returnCode:"<<Convertstr(ResponseData["returnCode"].asString());
                    ResponseData1 = ResponseData.get("TransactionResponse",0);
                    qDebug()<<"txnReferenceNo:"<<Convertstr(ResponseData1["txnReferenceNo"].asString());
                    qDebug()<<"cbsTxnReferenceNo:"<<Convertstr(ResponseData1["cbsTxnReferenceNo"].asString());
                    strcpy(firefund.txnReferenceNo,Convertstr(ResponseData1["txnReferenceNo"].asString()));

                    ResponseData2.clear();
                    ResponseData2 = ResponseData1.get("balancesList",0);
                    printf("\nSize : %d\n",ResponseData2.size());
                    for(int i=0;i<ResponseData2.size();i++)
                    {
                        qDebug()<<"balancesList ledgerBalance : "<<Convertstr(ResponseData2[i]["ledgerBalance"].asString());         ////
                        qDebug()<<"balancesList availableBalance : "<<QString::number(ResponseData2[i]["availableBalance"].asFloat());   ////
                        qDebug()<<"balancesList currency : "<<Convertstr(ResponseData2[i]["currency"].asString());
                        strcpy(firefund.BAledgerBalance,Convertstr(ResponseData2[i]["ledgerBalance"].asString()));
                        strcpy(firefund.BAavailableBalance,QString::number(ResponseData2[i]["availableBalance"].asFloat()).toAscii().data());
                        strcpy(firefund.BAcurrency,Convertstr(ResponseData2[i]["currency"].asString()));
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



int FPB::Reversal_Refire_Request()
{
   char szDate[16], szTime[16];
    QSqlQuery query;
    CMisc miscObj;
    char ErrorMsg[150];
    char spiltstring[40];
    memset(&firefund,0,sizeof(firefund));

    uidmessage::instance()->hide_Message();
    uidmessage::instance()->Display_BusyMessage("Please Wait....");

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

    Clear_DeviceInfo();
    int iretDI = DeviceInfo();

    if(iretDI != FUNCTION_SUCCESS)
        return FUNCTION_FAILED;


    Ref_amount = Ref_amount.remove(".00");
    iGenerateRequestID((char *)username,(unsigned char *)&firefund.Ref_RequestId_req);
    memcpy(firefund.Ref_MethodId_req,"109",3);
    memcpy(firefund.Ref_TellerID_req,username,sizeof(username));
    memcpy(firefund.Ref_IsEncrypt_req,"false",5);
    memcpy(firefund.Ref_SessionId_req,(char *)configLoginLeg2.ucSessionId,strlen((char *)(char *)configLoginLeg2.ucSessionId));
    memcpy(firefund.Ref_asyncFlag_req,"Y",1);
    memcpy(firefund.Ref_totalAmount_req,Ref_amount.toAscii().data(),strlen(Ref_amount.toAscii().data())); // IMPS
    memcpy(firefund.Ref_txnType_req,"IMPS",4);
    memcpy(firefund.Ref_impsserviceflag_req,"Y",1);
    memcpy(firefund.Ref_cashoraccount_req,"CASH",4);
    memcpy(firefund.Ref_requestbitmap_req,"10000100",8);
    memcpy(firefund.Ref_mobileNumber_req,(char *)FC_MobileNumber,strlen((char *)FC_MobileNumber));
    memcpy(firefund.Ref_Authid_req,"1",1);
    memcpy(firefund.Ref_otpMethodId_req,"2",1);
    memcpy(firefund.Ref_otpRequestId_req,revesal.Revotp_requestid,strlen(revesal.Revotp_requestid));
    memcpy(firefund.Ref_otppin_req,CustDet.cOtp,strlen(CustDet.cOtp));
    memcpy(firefund.Ref_otpMessageId_req,"-1",2);
    memcpy(firefund.Ref_SessionExpiryTime_req,configLoginLeg2.ucSessionExpiryTime,strlen(configLoginLeg2.ucSessionExpiryTime));
    memcpy(firefund.Ref_ABappId_req,CLIENT_ID,strlen(CLIENT_ID));
    memcpy(firefund.Ref_ABcustomerNumber_req,FC_CustomerNumber,strlen(FC_CustomerNumber));
    memcpy(firefund.Ref_ABbeneficiaryType_req,"3",1);
    memcpy(firefund.Ref_ABbeneficiaryAccount_req,ReversalDetails[currentIndex].rfuText1Val,strlen(ReversalDetails[currentIndex].rfuText1Val));
    memcpy(firefund.Ref_ABbeneficiaryAccountType_req,"10",2);
    memcpy(firefund.Ref_ABbeneficiaryName_req,ReversalDetails[currentIndex].rfuText5Val,strlen(ReversalDetails[currentIndex].rfuText5Val));
    memcpy(firefund.Ref_ABbeneficiaryBank_req,ReversalDetails[currentIndex].rfuText3Val,strlen(ReversalDetails[currentIndex].rfuText3Val));
    memcpy(firefund.Ref_ABbeneficiaryBankIfsc_req,ReversalDetails[currentIndex].rfuText2Val,strlen(ReversalDetails[currentIndex].rfuText2Val));
    memcpy(firefund.Ref_PTisInclusive_req,"0",1);
    memcpy(firefund.Ref_PTisClubbed_req,"0",1);
    memcpy(firefund.Ref_PTchargeOverride_req,"1",1);


    qDebug()<<"In the timestamp";
    memcpy(firefund.Ref_amount_req,Ref_amount.toAscii().data(),strlen(Ref_amount.toAscii().data()));
    memcpy(firefund.Ref_currency_req,"INR",3);
    memcpy(firefund.Ref_DebitFlag_req,"D",1);
    memcpy(firefund.Ref_transactionType_req,"DMTIMPSP2A",10);
    memcpy(firefund.Ref_transactionComment_req,"FT DMTIMPSP2A",13);
    memcpy(firefund.Ref_costCenter_req,configLoginLeg2.ucBAcode,strlen(configLoginLeg2.ucBAcode));
    memcpy(firefund.Ref_ChannelID_req,CHANNEL_ID,strlen(CHANNEL_ID));
    memcpy(firefund.Ref_ProductCode_req,"9999|Y",6);
    memcpy(firefund.Ref_RemeName_req,ReversalDetails[currentIndex].customerName,strlen(ReversalDetails[currentIndex].customerName));
    //sprintf(spiltstring,"%s|%s|5|10",Ref_amount.toAscii().data(),ReversalDetails[currentIndex].referenceNo);


    memcpy(firefund.Ref_SplitReferenceNo_req,setSplitSeqNumber().toAscii().data(),strlen(setSplitSeqNumber().toAscii().data()));
    printf("%s\n",firefund.Ref_SplitReferenceNo_req);
 //   memcpy(firefund.Ref_SplitReferenceNo_req,"100|911712028149|5|10",21);
    memcpy(firefund.Ref_CreditFlag_req,"C",1);
    memcpy(firefund.Ref_merchantMobileNumber_req,(char *)configLoginLeg2.ucmobileNumber,strlen((char *)configLoginLeg2.ucmobileNumber));

        memcpy(firefund.Ref_RRN,getsupportdataref().toAscii().data(),strlen(getsupportdataref().toAscii().data()));


        if(refNumList.size()==1)
        {
          memcpy((char *)firefund.RefsupportData_SPLTSEQ, "0", 1);  //  SPLTSEQ  1 - Txn Amount>PerTransactionLimit (Total) | 0: Txn Amount<=PerTransactionLimit (Total)
        }
        else
        {
            memcpy((char *)firefund.RefsupportData_SPLTSEQ, "1", 1);
        }
        Ref_charges =Ref_charges.remove(".00");
        memcpy((char *)firefund.RefsupportData_CHGAMT,Ref_charges.toAscii().data(),strlen(Ref_charges.toAscii().data()));  // 46 CHGAMT Charges (AEPS - 0)


        memset(szDate, 0, sizeof(szDate));
        memset(szTime, 0, sizeof(szTime));
        GetLocalTime(szDate, szTime);


    int ret	=  makeJSONREFIRE();/*-------------Make REQUEST--------------*/
    if(ret!=FUNC_OK)
    {
        return FUNC_ERROR;
    }


    uidmessage::instance()->hide_Message();
    uidmessage::instance()->Display_BusyMessage("Transaction Request in Progress");

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
    strcpy(str_url,URL_CREATION(REVERSAL_URL));
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

        int iRetparse = Reversal_Refund_otp_Parsing();
        qDebug()<<"After the Reversal_Refund_otp_Parsing";
        if(iRetparse==FUNC_OK)
        {
            char iquery[500];
            query.clear();

            query.exec("delete from LastWithdrawalData");
            query.clear();
            memset(iquery,0,sizeof(iquery));
            sprintf(iquery, "INSERT INTO LastWithdrawalData (LAST_RRN, LAST_DATE) VALUES('%s','%s')", firefund.txnReferenceNo,szDate);
    #if DEBUG
            printf("QUERY 1 === %s\n",iquery);
    #endif
            if(!query.exec(iquery))
            {
                qDebug() << "In Insert_LastWithdrawalData = " << query.lastError();
                query.clear();
                query.finish();
            }
            query.clear();
            query.finish();


            QString bal = QString::fromUtf8((const char*)(const char*)firefund.BAavailableBalance);
            bool decimal;
            decimal = bal.contains('.');
            if(decimal == false)
            {
                bal = bal.append(".00");
            }
            memset(Balance_Display,0,strlen(Balance_Display));
            sprintf(Balance_Display,"Balance : %s\n",bal.toLatin1().data(),strlen(bal.toLatin1().data()));
            strncpy(Mer_Latest_Balance, bal.toLatin1().data(),strlen(bal.toLatin1().data()));
            ui->Dash_Latest_Balance_Label_pg12->setText(Balance_Display);
            ui->SubTransaction_Label1->setText((char *)configLoginLeg2.ucname);
            ui->SubTransaction_Label1->setFont(QFont("SansSerif",10));

            ui->SubTransaction_Label1->setAlignment(Qt::AlignRight | Qt::AlignBottom);
            printf("Balance_Display = %s\n",Balance_Display);
            ui->SubTransaction_Label2->setText(QString::fromUtf8((const char*)Balance_Display));
            ui->SubTransaction_Label2->setFont(QFont("SansSerif",9));
            ui->SubTransaction_Label2->setAlignment(Qt::AlignRight | Qt::AlignBottom);
            uidmessage::instance()->hide_Message();

            char display_confirmpage[5][100];
            sprintf(display_confirmpage[0],"Beneficiary Name     : %s",firefund.Ref_ABbeneficiaryName_req);
            sprintf(display_confirmpage[1],"Transaction Amount   : %s",Ref_amount.toAscii().data());
            sprintf(display_confirmpage[2],"Charges              : %s",Ref_charges.toAscii().data());
            sprintf(display_confirmpage[3],"Bank Name            : %s",firefund.Ref_ABbeneficiaryBank_req);
            sprintf(display_confirmpage[4],"Transaction No       : %s",firefund.txnReferenceNo);
            sprintf(display_confirmpage[5],"       Fund Transfer Successful");

            int i=0;
            qDebug()<<"Display";
            ui->Confirm_rev_list->clear(); // clear list
            ui->Confirm_rev_list->setFont(QFont("Courier",12));

            for(i=0;i<6;i++)
            {
        #if DEBUG
                printf("display_confirmpage    =====    %s\n",display_confirmpage[i]);
        #endif
                ui->Confirm_rev_list->addItem((char *)display_confirmpage[i]);
            }

            ui->Reversal_conf_button->setVisible(false);
            ui->Reversal_next_button->setText("Finish");
            rev_display = 1;
            return FUNCTION_SUCCESS;

        }
        else
        {

            if(strcmp((char *)firefund.ucResponseMswssage,"Session Expired.")==0)
            {
                uidmessage::instance()->hide_Message();
                sprintf(ErrorMsg,"%s",(char *)firefund.ucResponseMswssage);
                miscObj.DisplayMsgBox(ErrorMsg);
                uidmessage::instance()->hide_Message();
                return SESSION_EXPIRED_FAILED;
            }
            sprintf(ErrorMsg,"%s",(char *)firefund.ucResponseMswssage);
            miscObj.DisplayMsgBox(ErrorMsg);
            uidmessage::instance()->hide_Message();
            return FUNCTION_FAILED;

        }

        return FUNCTION_SUCCESS;
}






int FPB::makeJSONREFIRE()//----------------------
{
    char tempbuff[30];
    int iRet=-2,slen;

    memset(gSendData,0,sizeof(gSendData));

    iRet = iMakeRequest(23);//changed by vaishnavi
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
    system("rm /usr/FINO/FPB/REQ/REFIRE_REQ.TXT");
    FILE *fp2 = NULL;
    fp2 =fopen("/usr/FINO/FPB/REQ/REFIRE_REQ.TXT","a");
    fwrite(gSendData,strlen((char *)gSendData),1,fp2);
    //ttestall(0,100);
    sleep(1);
    fclose(fp2);
#endif
    return FUNC_OK;

}

int FPB::makeJSONREFUND()//----------------------
{
    char tempbuff[30];
    int iRet=-2,slen;

    memset(gSendData,0,sizeof(gSendData));

    iRet = iMakeRequest(24);//changed by vaishnavi
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
    system("rm /usr/FINO/FPB/REQ/REFUND_REQ.TXT");
    FILE *fp2 = NULL;
    fp2 =fopen("/usr/FINO/FPB/REQ/REFUND_REQ.TXT","a");
    fwrite(gSendData,strlen((char *)gSendData),1,fp2);
    //ttestall(0,100);
    sleep(1);
    fclose(fp2);
#endif
    return FUNC_OK;

}


int FPB::iSetREFIRE(int tagID, unsigned char *value)
{
    switch(tagID)
    {
    case  1:
        strcpy((char *)value,(char *)firefund.Ref_RequestId_req);

        break;
    case  2:
        strcpy((char *)value,firefund.Ref_MethodId_req);//Method id
        break;
    case  3:
        strcpy((char *)value,(char *)firefund.Ref_TellerID_req);//teller id
        break;
    case  4:
        strcpy((char *)value,(char *)firefund.Ref_SessionId_req);//session key
    //    printf("firefund.ucSessionID == %s\n",firefund.ucSessionID);
        break;
    case  5:
        strcpy((char *)value,(char *)firefund.Ref_IsEncrypt_req);//token id
        break;
    case  6:
        strcpy((char *)value,(char *)firefund.Ref_asyncFlag_req);//is encrypt
        break;
    case  7:
        strcpy((char *)value,(char *)firefund.Ref_totalAmount_req);
        break;
    case  8:
        strcpy((char *)value,(char *)firefund.Ref_txnType_req);
   //     printf("firefund.ucmerchantMobileNumber == %s\n",firefund.ucmerchantMobileNumber);
        break;
    case  9:
        strcpy((char *)value,(char *)firefund.Ref_impsserviceflag_req);
        break;
    case  10:
        strcpy((char *)value,(char *)firefund.Ref_cashoraccount_req);
        break;
    case  11:
        strcpy((char *)value,(char *)firefund.Ref_requestbitmap_req);
   //     printf("firefund.ucAadharNo == %s\n",firefund.ucAadharNo);
        break;
    case  12:
        strcpy((char *)value,(char *)firefund.Ref_mobileNumber_req);//free
        break;
    case  13:
        strcpy((char *)value,(char *)firefund.Ref_Authid_req);
        break;
    case  14:
        strcpy((char *)value,(char *)firefund.Ref_otpMethodId_req);
        break;
    case  15:
        strcpy((char *)value,(char *)firefund.Ref_otpRequestId_req/*firefund.ucPID*/);
        break;
    case  16:
        strcpy((char *)value,(char *)firefund.Ref_otppin_req/*firefund.ucHmac*/);
        break;
    case  17:
        strcpy((char *)value,(char *)firefund.Ref_otpMessageId_req/*firefund.ucSkey*/);
        break;
    case  18:
        strcpy((char *)value,(char *)firefund.Ref_SessionExpiryTime_req/*firefund.ucPidData*/);
        break;
    case  19:
        strcpy((char *)value,(char *)firefund.Ref_ABappId_req);
        break;
    case  20:
        strcpy((char *)value,(char *)firefund.Ref_ABcustomerNumber_req);
        break;
    case  21:
        strcpy((char *)value,(char *)firefund.Ref_ABbeneficiaryType_req);
        break;
    case  22:
        strcpy((char *)value,(char *)firefund.Ref_ABbeneficiaryAccount_req);
        break;
    case  23:
        strcpy((char *)value,(char *)firefund.Ref_ABbeneficiaryAccountType_req);
        break;
    case  24:
        strcpy((char *)value,(char *)firefund.Ref_ABbeneficiaryName_req);
        break;
    case  25:
        strcpy((char *)value,(char *)firefund.Ref_ABbeneficiaryBank_req);
        break;
    case  26:
        //strcpy((char *)value,"");//free
        strcpy((char *)value,(char *)firefund.Ref_ABbeneficiaryBankIfsc_req);
        break;
    case  27:
        strcpy((char *)value,(char *)firefund.Ref_PTisInclusive_req);
        break;
    case  28:
        strcpy((char *)value,(char *)firefund.Ref_PTisClubbed_req);
        break;
    case  29:
        strcpy((char *)value,(char *)firefund.Ref_PTchargeOverride_req);
        break;
    case  30:
        strcpy((char *)value,(char *)sd_deviceinfo.ucSupportData_PRTXNID_req);
        break;
    case  31:
        strcpy((char *)value,(char *)firefund.Ref_amount_req);
        break;
    case  32:
        strcpy((char *)value,(char *)firefund.Ref_currency_req);//Terminal ID
        break;
    case  33:
        strcpy((char *)value,(char *)firefund.Ref_DebitFlag_req);
        break;
    case  34:
        strcpy((char *)value,(char *)firefund.Ref_transactionType_req);
        break;
    case  35:
        strcpy((char *)value,(char *)firefund.Ref_transactionComment_req);
        break;

    case  36:
        strcpy((char *)value,(char *)firefund.Ref_costCenter_req);
        break;
    case  37:
        strcpy((char *)value,(char *)firefund.Ref_ChannelID_req);
        break;
    case  38:
        strcpy((char *)value,(char *)firefund.Ref_ProductCode_req);
        break;
    case  39:
        strcpy((char *)value,(char *)firefund.Ref_RemeName_req);
        break;
    case  40:
        strcpy((char *)value,(char *)firefund.Ref_ABbeneficiaryName_req);
        break;
    case  41:
        strcpy((char *)value,(char *)firefund.Ref_SplitReferenceNo_req);
        break;
    case  42:
        strcpy((char *)value,(char *)firefund.Ref_CreditFlag_req);
        break;
    case  43:
        strcpy((char *)value,(char *)firefund.Ref_merchantMobileNumber_req);
        break;
    case  44:
        strcpy((char *)value,(char *)firefund.Ref_remarks_req);
        break;
    case  45:
        strcpy((char *)value,(char *)firefund.Ref_analysisFlag_req);
        break;

    case  46:
        strcpy((char *)value,(char *)sd_deviceinfo.ucSupportData_PVTXNID_req);
        break;

    case  47:
        strcpy((char *)value,(char *)sd_deviceinfo.ucSupportData_PVTXNDT_req);
        break;

    case  48:
        strcpy((char *)value,(char *)sd_deviceinfo.ucSupportData_IPADDR_req);
        break;

    case  49:
        strcpy((char *)value,(char *)sd_deviceinfo.ucSupportData_DEVICEID_req);
        break;

    case  50:
        strcpy((char *)value,(char *)sd_deviceinfo.ucSupportData_CELLIDLatitude_req);
        break;

    case  51:
        strcpy((char *)value,(char *)sd_deviceinfo.ucSupportData_CELLIDLongitude_req);
        break;

    case  52:
        strcpy((char *)value,(char *)sd_deviceinfo.ucSupportData_MCCCellID_req);
        break;

    case  53:
        strcpy((char *)value,(char *)sd_deviceinfo.ucSupportData_MCCLAC_req);
        break;

    case  54:
        strcpy((char *)value,(char *)sd_deviceinfo.ucSupportData_MCCMCC_req);
        break;

    case  55:
        strcpy((char *)value,(char *)sd_deviceinfo.ucSupportData_MCCMNC_req);
        break;

    case  56:
        strcpy((char *)value,(char *)firefund.RefsupportData_SPLTSEQ);
        break;

    case  57:
        strcpy((char *)value,(char *)firefund.RefsupportData_CHGAMT);
        break;

    case  58:
        strcpy((char *)value,(char *)firefund.Ref_RRN);
        break;

    default:
        return FUNC_ERROR;
    }
    return FUNC_OK;
}





int FPB::iSetREFUND(int tagID, unsigned char *value)
{
    switch(tagID)
    {
    case  1:
        strcpy((char *)value,(char *)firefund.Ref_RequestId_req);

        break;
    case  2:
        strcpy((char *)value,firefund.Ref_MethodId_req);//Method id
        break;
    case  3:
        strcpy((char *)value,(char *)firefund.Ref_TellerID_req);//teller id
        break;
    case  4:
        strcpy((char *)value,(char *)firefund.Ref_SessionId_req);//session key
    //    printf("firefund.ucSessionID == %s\n",firefund.ucSessionID);
        break;
    case  5:
        strcpy((char *)value,(char *)firefund.Ref_IsEncrypt_req);//token id
        break;
    case  6:
        strcpy((char *)value,(char *)firefund.Ref_mobileNumber_req);//free
        break;
    case  7:
        strcpy((char *)value,(char *)firefund.Ref_Authid_req);
        break;

    case  8:
        strcpy((char *)value,(char *)firefund.Ref_otpMethodId_req);
        break;

    case  9:
        strcpy((char *)value,(char *)firefund.Ref_otpRequestId_req);
        break;

    case  10:
        strcpy((char *)value,(char *)firefund.Ref_otppin_req/*firefund.ucHmac*/);
        break;

    case  11:
        strcpy((char *)value,(char *)firefund.Ref_otpMessageId_req/*firefund.ucSkey*/);
        break;

    case  12:
        strcpy((char *)value,(char *)firefund.Ref_SessionExpiryTime_req);
        break;

    case  13:
        strcpy((char *)value,(char *)firefund.Ref_amount_req);
        break;

    case  14:
        strcpy((char *)value,(char *)firefund.Ref_transactionType_req);
        break;

    case  15:
        strcpy((char *)value,(char *)firefund.Ref_transactionComment_req);
        break;

    case  16:
        strcpy((char *)value,(char *)firefund.Ref_costCenter_req);
        break;
    case  17:
        strcpy((char *)value,(char *)sd_deviceinfo.ucSupportData_PVTXNID_req);
        break;

    case  18:
        strcpy((char *)value,(char *)sd_deviceinfo.ucSupportData_PVTXNDT_req);
        break;

    case  19:
        strcpy((char *)value,(char *)sd_deviceinfo.ucSupportData_IPADDR_req);
        break;

    case  20:
        strcpy((char *)value,(char *)sd_deviceinfo.ucSupportData_DEVICEID_req);
        break;

    case  21:
        strcpy((char *)value,(char *)firefund.Ref_ABappId_req);
        break;



    case  22:
        strcpy((char *)value,(char *)sd_deviceinfo.ucSupportData_CELLIDLatitude_req);
        break;

    case  23:
        strcpy((char *)value,(char *)sd_deviceinfo.ucSupportData_MCCCellID_req);
        break;


    case  25:
        strcpy((char *)value,(char *)sd_deviceinfo.ucSupportData_PRTXNID_req);
        break;

    case  26:
        strcpy((char *)value,(char *)firefund.RefsupportData_SPLTSEQ);
        break;

    case  27:
        strcpy((char *)value,(char *)firefund.RefsupportData_CHGAMT);
        break;

    case  28:
        strcpy((char *)value,(char *)firefund.RefCustomernumber);
        break;

//    case  29:
//        strcpy((char *)value,(char *)firefund.RefsupportData_LKNST);
//        break;

    case  30:
        strcpy((char *)value,(char *)firefund.Ref_remarks_req);
        break;

    case  31:
        strcpy((char *)value,(char *)firefund.Ref_supportdata2);
        break;

    case  32:
        strcpy((char *)value,(char *)firefund.Ref_analysisFlag_req);
        break;


    case  33:
        strcpy((char *)value,(char *)firefund.Ref_SplitReferenceNo_req);
        break;

    case  34:
        strcpy((char *)value,(char *)firefund.Ref_merchantMobileNumber_req);
        break;

    case  35:
        strcpy((char *)value,(char *)sd_deviceinfo.ucSupportData_CELLIDLongitude_req);
        break;


    case  36:
        strcpy((char *)value,(char *)firefund.Ref_ChannelID_req);
        break;


    case  37:
        strcpy((char *)value,(char *)firefund.Ref_ProductCode_req);
        break;

    case  38:
        strcpy((char *)value,(char *)firefund.Ref_RRN);
        break;

    default:
        return FUNC_ERROR;
    }
    return FUNC_OK;
}






void FPB::on_btnReversal_confirmation_back_clicked()
{
    ui->stackedWidget->setCurrentIndex(41);
}

void FPB::on_Back_Reversal_data_dsp_clicked()
{
    ui->stackedWidget->setCurrentIndex(15);
}


void FPB::on_Reversal_next_button_clicked()
{
    int iRet=0;
    ui->OTP_Lineedit->clear();
    if(rev_display == 1)
    {
        ui->stackedWidget->setCurrentIndex(15);
        return;
    }
    else
    {
        iRet = Reversal_otp_request();

    }
    if(iRet == FUNC_OK)
    {
        OTP_page = 5;
        ui->stackedWidget->setCurrentIndex(22);
        return;

    }
    else if(iRet != FUNCTION_SUCCESS)
    {
        ui->stackedWidget->setCurrentIndex(42);
        return;
    }


}

void FPB::on_Reversal_conf_button_clicked()
{
    CMisc miscObj;
    uidmessage::instance()->hide_Message();
    int ret = miscObj.DisplayMsgBox_SHG("Are you sure, you want to discard the process?");
    uidmessage::instance()->hide_Message();
    if(ret == true)
    {
        ui->stackedWidget->setCurrentIndex(41);
    }
    return;
}


void FPB::getreversaldata(QString referenceno)
{
    QSqlQuery query;

    for(int i = 0;i<arrsize;i++)
    {
        query.clear();
        query.prepare("SELECT * FROM REVERSAL_TXN  WHERE TRANSACTION_TYPE NOT LIKE 'CHG%' AND PARENT_TXN_ID = ? limit 1");
        query.addBindValue(referenceno);
        if(!query.exec())
        {
            qDebug() << query.lastError();
        }
        else
        {

            while(query.next())
            {
                strcpy(ReversalDetails[i].sequenceNo,query.value(0).toString().trimmed().toAscii().data());
                strcpy(ReversalDetails[i].channel,query.value(1).toString().trimmed().toAscii().data());
                strcpy(ReversalDetails[i].referenceNo,query.value(2).toString().trimmed().toAscii().data());
                strcpy(ReversalDetails[i].date,query.value(3).toString().trimmed().toAscii().data());
                strcpy(ReversalDetails[i].amount,query.value(4).toString().trimmed().toAscii().data());
                strcpy(ReversalDetails[i].trantype,query.value(5).toString().trimmed().toAscii().data());
                strcpy(ReversalDetails[i].parentTxnId,query.value(6).toString().trimmed().toAscii().data());
                strcpy(ReversalDetails[i].chargeAmount,query.value(7).toString().trimmed().toAscii().data());
                strcpy(ReversalDetails[i].originalSequenceNo,query.value(8).toString().trimmed().toAscii().data());
                strcpy(ReversalDetails[i].product,query.value(9).toString().trimmed().toAscii().data());
                strcpy(ReversalDetails[i].customerNo,query.value(10).toString().trimmed().toAscii().data());
                strcpy(ReversalDetails[i].accountNo,query.value(11).toString().trimmed().toAscii().data());
                strcpy(ReversalDetails[i].branch,query.value(12).toString().trimmed().toAscii().data());
                strcpy(ReversalDetails[i].userId,query.value(13).toString().trimmed().toAscii().data());
                strcpy(ReversalDetails[i].status,query.value(14).toString().trimmed().toAscii().data());
                strcpy(ReversalDetails[i].reversalFlag,query.value(15).toString().trimmed().toAscii().data());
                strcpy(ReversalDetails[i].narrative,query.value(16).toString().trimmed().toAscii().data());
                strcpy(ReversalDetails[i].time,query.value(17).toString().trimmed().toAscii().data());
                strcpy(ReversalDetails[i].customerName,query.value(18).toString().trimmed().toAscii().data());
                strcpy(ReversalDetails[i].customerMobile,query.value(19).toString().trimmed().toAscii().data());
                strcpy(ReversalDetails[i].splitSequence,query.value(20).toString().trimmed().toAscii().data());
                strcpy(ReversalDetails[i].debitCredit,query.value(21).toString().trimmed().toAscii().data());
                strcpy(ReversalDetails[i].payoutamt,query.value(22).toString().trimmed().toAscii().data());
                strcpy(ReversalDetails[i].reverseCommissionAmount,query.value(23).toString().trimmed().toAscii().data());
                strcpy(ReversalDetails[i].reverseTargetAmount,query.value(24).toString().trimmed().toAscii().data());
                strcpy(ReversalDetails[i].rfudateDesc,query.value(25).toString().trimmed().toAscii().data());
                strcpy(ReversalDetails[i].rfudateval,query.value(26).toString().trimmed().toAscii().data());
                strcpy(ReversalDetails[i].rfuNumeric1Desc,query.value(27).toString().trimmed().toAscii().data());
                strcpy(ReversalDetails[i].rfuNumeric1Val,query.value(28).toString().trimmed().toAscii().data());
                strcpy(ReversalDetails[i].rfuNumeric2Desc,query.value(29).toString().trimmed().toAscii().data());
                strcpy(ReversalDetails[i].rfuNumeric2Val,query.value(30).toString().trimmed().toAscii().data());
                strcpy(ReversalDetails[i].rfuNumeric3Desc,query.value(31).toString().trimmed().toAscii().data());
                strcpy(ReversalDetails[i].rfuNumeric3Val,query.value(32).toString().trimmed().toAscii().data());
                strcpy(ReversalDetails[i].rfuNumeric4Desc,query.value(33).toString().trimmed().toAscii().data());
                strcpy(ReversalDetails[i].rfuNumeric4Val,query.value(34).toString().trimmed().toAscii().data());
                strcpy(ReversalDetails[i].rfuText1Desc,query.value(35).toString().trimmed().toAscii().data());
                strcpy(ReversalDetails[i].rfuText1Val,query.value(36).toString().trimmed().toAscii().data());
                strcpy(ReversalDetails[i].rfuText2Desc,query.value(37).toString().trimmed().toAscii().data());
                strcpy(ReversalDetails[i].rfuText2Val,query.value(38).toString().trimmed().toAscii().data());
                strcpy(ReversalDetails[i].rfuText3Desc,query.value(39).toString().trimmed().toAscii().data());
                strcpy(ReversalDetails[i].rfuText3Val,query.value(40).toString().trimmed().toAscii().data());
                strcpy(ReversalDetails[i].rfuText4Desc,query.value(41).toString().trimmed().toAscii().data());
                strcpy(ReversalDetails[i].rfuText4Val,query.value(42).toString().trimmed().toAscii().data());
                strcpy(ReversalDetails[i].rfuText5Desc,query.value(43).toString().trimmed().toAscii().data());
                strcpy(ReversalDetails[i].rfuText5Val,query.value(44).toString().trimmed().toAscii().data());
                strcpy(ReversalDetails[i].rfuText6Desc,query.value(45).toString().trimmed().toAscii().data());
                strcpy(ReversalDetails[i].rfuText6Val,query.value(46).toString().trimmed().toAscii().data());
                strcpy(ReversalDetails[i].rfuText7Desc,query.value(47).toString().trimmed().toAscii().data());
                strcpy(ReversalDetails[i].rfuText7Val,query.value(48).toString().trimmed().toAscii().data());
                strcpy(ReversalDetails[i].rfuText8Desc,query.value(49).toString().trimmed().toAscii().data());
                strcpy(ReversalDetails[i].rfuText8Val,query.value(50).toString().trimmed().toAscii().data());
                strcpy(ReversalDetails[i].rfuText9Desc,query.value(51).toString().trimmed().toAscii().data());
                strcpy(ReversalDetails[i].rfuText9Val,query.value(52).toString().trimmed().toAscii().data());
                strcpy(ReversalDetails[i].rfuText10Desc,query.value(53).toString().trimmed().toAscii().data());
                strcpy(ReversalDetails[i].rfuText10Val,query.value(54).toString().trimmed().toAscii().data());
                strcpy(ReversalDetails[i].parentcurr,query.value(55).toString().trimmed().toAscii().data());
                strcpy(ReversalDetails[i].chargecurr,query.value(56).toString().trimmed().toAscii().data());
                strcpy(ReversalDetails[i].parentTxnAmount,query.value(57).toString().trimmed().toAscii().data());
            }

        }
    }

}


void FPB::getreferenceno(QString reference)
{

     QSqlQuery query;
     refNumList.clear();
     query.clear();
     query.prepare("SELECT  REFERENCE_NO FROM REVERSAL_TXN  WHERE TRANSACTION_TYPE NOT LIKE 'CHG%' AND PARENT_TXN_ID = ? ");
     query.addBindValue(reference);
     if(!query.exec())
     {
         qDebug() << query.lastError();
     }
     else
    {
         while(query.next())
         {
            refNumList<<query.value(0).toString();
        }
     }
}

int FPB::getChargeAmountFromDB(QString referenceNo)
{
   int ChgAmount = -1;
   QString Amt = getReversalSumOfChargesFromParentTxnId(referenceNo);

     if(!Amt.isEmpty())
     {
       int retChargeAmount = Amt.toInt();
        ChgAmount = abs(retChargeAmount);
        qDebug()<<"ChgAmoun11111111111111111t"<<ChgAmount;


                return  ChgAmount;
      }
            return ChgAmount;
}

QString FPB::getReversalSumOfChargesFromParentTxnId(QString reference)
{
    QSqlQuery query;
    query.clear();
    query.prepare("SELECT  SUM(AMOUNT) FROM REVERSAL_TXN  WHERE TRANSACTION_TYPE  LIKE 'CHG%' AND REFERENCE_NO = ? ");
    query.addBindValue(reference);
    if(!query.exec())
    {
        qDebug() << query.lastError();
    }
    else
   {
        while(query.next())
        {
           return query.value(0).toString();
       }
    }

}

int FPB::getAmount(QString reference)
{
    QSqlQuery query;
    query.clear();
    query.prepare("SELECT  AMOUNT FROM REVERSAL_TXN  WHERE TRANSACTION_TYPE  NOT LIKE 'CHG%' AND REFERENCE_NO = ? ");
    query.addBindValue(reference);
    if(!query.exec())
    {
        qDebug() << query.lastError();
    }
    else
   {
        while(query.next())
        {

            qDebug()<<"query.value(0).toInt()333333333333"<<query.value(0).toInt();


           return query.value(0).toInt();
       }
    }

}

QString FPB::getSplitSequence(QString reference)
{
    QSqlQuery query;
    query.clear();
    query.prepare("SELECT  SEQUENCE_NO FROM REVERSAL_TXN  WHERE TRANSACTION_TYPE  NOT LIKE 'CHG%' AND REFERENCE_NO = ? ");
    query.addBindValue(reference);
    if(!query.exec())
    {
        qDebug() << query.lastError();
    }
    else
   {
        while(query.next())
        {
           return query.value(0).toString();
       }
    }

}


QString FPB::setSplitSeqNumber()
{
       QString result = "";
       QString sb ;

       if(refNumList.size() > 0 && refNumList.size() > 0)
       {    sb.clear();
           qDebug()<<"refNumList.size()-----------"<<refNumList.size();
           for(int i =0;i<refNumList.size();i++)
           {
               if(sb.length() > 0)
               {
                   sb.append(',');
               }
               QString ref = refNumList[i];
               int Amount = abs(getAmount(ref));
               qDebug()<<"Amount==========="<<Amount;


               if(getChargeAmountFromDB(ref) != -1)
               {
                   sb.append(QString::number(Amount));
                   sb.append("|");
                   sb.append(ref);
                   sb.append("|5|");
                   sb.append(QString::number(getChargeAmountFromDB(ref)));
               }
                 //  sb.append((int)Amount+"|"+ref+"|"+ "5"+"|"+getChargeAmountFromDB(ref));
               else {
                   if (! (QString::compare(getSplitSequence(ref), "1", Qt::CaseInsensitive))||(! QString::compare(getSplitSequence(ref), "1", Qt::CaseInsensitive))) {
                      {
                           sb.append(QString::number(Amount));
                           sb.append("|");
                           sb.append(ref);
                           sb.append("|5|");
                           sb.append(QString::fromUtf8(ReversalDetails[i].chargeAmount));

                       }
                           //sb.append( (int)Amount + "|"  + ref  +"|" +"5"  + "|" + QString::fromUtf8(ReversalDetails[i].chargeAmount));

                   }else
                   {
                       sb.append(QString::number(Amount));
                       sb.append("|");
                       sb.append(ref);
                       sb.append("|5|0");


                   }
                      // sb.append( (int)Amount + "|"+ ref + "|" + "5" + "|0" );

               }
           }
           result = sb;
           return result;
       }
       return result;
   }


QString FPB::getsupportdataref()
{
     QString Ref_number;
     Ref_number.clear();
     for(int i =0;i<refNumList.size();i++)
     {
         if(Ref_number.length() > 0)
         {
             Ref_number.append(',');
         }
         Ref_number.append(refNumList[i]);
     }
     return Ref_number;

}
