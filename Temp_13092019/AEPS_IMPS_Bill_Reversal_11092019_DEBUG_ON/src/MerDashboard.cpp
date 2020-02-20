#include "ui_FPB.h"
#include "finomisc.h"
#include "FPB.h"
#include "userprofile.h"
#include "Login.h"
#include<fstream>
extern char * Convertstr(string str);
extern Login_leg2 configLoginLeg2;
extern char MER_FLAG;
Refresh sndrefresh;
QString HideBL;


void FPB::on_MerchanData_clicked()
{


 //   printf("configLoginLeg2.ucBAdescription = %s and configLoginLeg2.ucBAcode = %s\n",configLoginLeg2.ucBAdescription, configLoginLeg2.ucBAcode);
    char DispBranchCode[30];
    memset(DispBranchCode, 0, sizeof(DispBranchCode));
    if(strcmp(configLoginLeg2.ucBAcode,"")!=0)
        sprintf(DispBranchCode,"%s-%s",configLoginLeg2.ucBAdescription,configLoginLeg2.ucBAcode);
    else
        sprintf(DispBranchCode,"%s",configLoginLeg2.ucBAdescription);

    ui->Label_Merchant_Name_pg26->setText((char *)configLoginLeg2.ucname);
    ui->Label_BranchNo_pg26->setText(DispBranchCode);
    char ver[20];
    memset(ver, 0, sizeof(ver));
    sprintf(ver, "Ver : %s",APP_VERSION); //APP_VERSION
    ui->AppVersion_Label_2->setText(ver);
    ui->verUpdate_label->setText(ver);
    ui->stackedWidget->setCurrentIndex(25);
    return;
}


void FPB::on_Refresh_Button_clicked()
{
    int iRet = -2;
//    ui->pushButton_Dash_AEPS_pg12->setDisabled(true);
//    ui->pushButton_Dash_AEPS_pg12->setHidden(true);
    iRet = Refresh_func();
    system("cd /usr/FINO/FPB/RESP/");
    system("rm /usr/FINO/FPB/RESP/OUTPUT.TXT");
//    ui->pushButton_Dash_AEPS_pg12->setHidden(false);
//    ui->PleaseWait__Merchant_DashBoard_label->setHidden(true);
//    ui->PleaseWait__Merchant_DashBoard_label->setEnabled(false);
    if(iRet != FUNCTION_SUCCESS)
    {
        if(iRet == SESSION_EXPIRED_FAILED)
            ui->stackedWidget->setCurrentIndex(0);
        else
            ui->stackedWidget->setCurrentIndex(12);
    }
    else if(iRet == FUNCTION_SUCCESS)
    {
        QString bal = QString::fromUtf8((const char*)(const char*)sndrefresh.uRbalance);
        bool decimal;
        decimal = bal.contains('.');
        if(decimal == false)
        {
            bal = bal.append(".00");
        }
        memset(Balance_Display,0,strlen(Balance_Display));
        sprintf(Balance_Display,"Balance : %s\n",bal.toLatin1().data(),strlen(bal.toLatin1().data()));
//        printf("Balance_Display in Refresh = %s\n", Balance_Display);


        strncpy(Mer_Latest_Balance,bal.toLatin1().data(),strlen(bal.toLatin1().data()));
  //      printf("Refresh Response Balance = %s & %s\n",sndrefresh.uRbalance, Mer_Latest_Balance);
//        QString temp1 = QString :: fromUtf8(Mer_Latest_Balance);
//        QString BL = "Balance : " + temp1;
//        qDebug()<<"BL == "<<BL;
        ui->Dash_Latest_Balance_Label_pg12->setText(Balance_Display);
        ui->stackedWidget->setCurrentIndex(12);
    }
    return;
}


int FPB::Refresh_func()
{
    uidmessage::instance()->hide_Message();
    uidmessage::instance()->Display_BusyMessage("Please Wait...");
//    uidmessage::instance()->hide_Message();
    qDebug()<<" PleaseWait__Merchant_DashBoard_label ";
//    ui->pushButton_Dash_AEPS_pg12->setHidden(true);
//    ui->PleaseWait__Merchant_DashBoard_label->setHidden(false);
//    ui->PleaseWait__Merchant_DashBoard_label->setEnabled(true);
//    ui->PleaseWait__Merchant_DashBoard_label->setText("Please Wait...");
    CMisc miscObj;
    int ret = -3;

    char ErrorMsg[100];
    char* jsondata;
    memset(ErrorMsg,0,sizeof(ErrorMsg));
    iGenerateRequestID((char *)username,(unsigned char *)&sndrefresh.ucRequestId_Rreq);

    Json1::Value ArrayList(Json1::arrayValue);
    Json1::Value RequestedData,mainreq;

    mainreq["RequestId"] =(char *) sndrefresh.ucRequestId_Rreq;
    mainreq["MethodId"] = "72";
    mainreq["TellerID"]= username;
    mainreq["SessionId"] =  (char *)configLoginLeg2.ucSessionId;
    mainreq["SessionExpiryTime"]= (char *)configLoginLeg2.ucSessionExpiryTime;
    mainreq["IsEncrypt"]= ISENCEYPT;
    RequestedData["userId"] =username;
    RequestedData["mobileno"] = "";
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
    printf("gSendData = %s\n",gSendData);
 #endif
    //-------------------------------------------------------------
    system("cd /usr/FINO/FPB/RESP/");
    system("rm /usr/FINO/FPB/RESP/REFRESH.txt");
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
    strcpy(str_url,URL_CREATION(MER_REFRESH_URL_PROD));
#else
    strcpy(str_url,URL_CREATION((char *)MER_REFRESH_URL));
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
//        ui->pushButton_Dash_AEPS_pg12->setHidden(false);
//        ui->PleaseWait__Merchant_DashBoard_label->setHidden(true);
//        ui->PleaseWait__Merchant_DashBoard_label->setEnabled(false);
        return FUNCTION_FAILED;
    }

    //    ret =file_download( "http://103.1.112.205:4444/UIService.svc/NonFinancialProcessRequest", jsondata,"/usr/FINO/FPB/RESP/REFRESH.txt",120);

    /****************************Decryption function*********************************/
    if(!strcmp((char *)ISENCEYPT,"true"))
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

    system("sed 's/\"ResponseData\":\"{/\"ResponseData\":{/g' /usr/FINO/FPB/RESP/OUTPUT.TXT > /tmp/1.txt");
    system("sed 's/}\",\"MessageId\"/},\"MessageId\"/g' /tmp/1.txt > /tmp/2.txt  ");
    system(" sed 's/\\\\//g' /tmp/2.txt > /usr/FINO/FPB/RESP/OUTPUT.TXT");

    system("cd /tmp/");
    system("rm /tmp/1.txt");
    system("rm /tmp/2.txt");

    Json1::Value root;
    root.clear();
    Json1::Reader reader;
    std::ifstream ReadData("/usr/FINO/FPB/RESP/OUTPUT.TXT");
    bool parsedSuccess = reader.parse(ReadData,root,false);
    ReadData.close();
    if(not parsedSuccess)
    {
        uidmessage::instance()->hide_Message();
        qDebug()<<" File not Found";
//        ui->pushButton_Dash_AEPS_pg12->setHidden(false);
//        ui->PleaseWait__Merchant_DashBoard_label->setHidden(true);
//        ui->PleaseWait__Merchant_DashBoard_label->setEnabled(false);
        return FUNCTION_FAILED;
        //return -1;
    }
    else
    {
        strcpy(sndrefresh.cRResponse_code ,Convertstr(root["ResponseCode"].asString()));
        strcpy(sndrefresh.cRResponseMessage ,Convertstr(root["ResponseMessage"].asString()));
        strcpy(sndrefresh.cRRequestId ,Convertstr(root["RequestId"].asString()));
        strcpy(sndrefresh.cRMessageType ,Convertstr(root["MessageType"].asString()));
        strcpy(sndrefresh.cRDisplayMessage ,Convertstr(root["DisplayMessage"].asString()));
#if DEBUG
        printf("sndrefresh.cLResponse_code%s",sndrefresh.cRResponse_code);
        printf("sndrefresh.cLResponseMessage%s",sndrefresh.cRResponseMessage);
#endif
        if(strcmp((char *)sndrefresh.cRResponseMessage,"Session Expired.")==0)
        {
            uidmessage::instance()->hide_Message();
            sprintf(ErrorMsg,"%s",/*(char *)LTS.ucResponseCode,*/(char *)sndrefresh.cRResponseMessage);
            miscObj.DisplayMsgBox(ErrorMsg);
            uidmessage::instance()->hide_Message();
//            ui->stackedWidget->setCurrentIndex(0);
            return SESSION_EXPIRED_FAILED;
        }

        if(strcmp(sndrefresh.cRResponse_code,"0")==0)
        {
            Json1::Value ResponseData;
            ResponseData.clear();
            ResponseData = root.get("ResponseData",0);
            printf("\nSize : %d\n",ResponseData.size());
            if(ResponseData.size()==0)
            {
                qDebug("\n1:Key Register Data Not Found\n");
//                ui->pushButton_Dash_AEPS_pg12->setHidden(false);
//                ui->PleaseWait__Merchant_DashBoard_label->setHidden(true);
//                ui->PleaseWait__Merchant_DashBoard_label->setEnabled(false);
                return FUNCTION_FAILED;
            }
            else
            {
                strcpy(sndrefresh.uRreturnCode,Convertstr(ResponseData["returnCode"].asString()));
                strcpy(sndrefresh.uRresponseMessage,Convertstr(ResponseData["responseMessage"].asString()));
                strcpy(sndrefresh.uRglAccountNo,Convertstr(ResponseData["glAccountBalance"]["glAccountNo"].asString()));
                strcpy(sndrefresh.uRbalance,Convertstr(ResponseData["glAccountBalance"]["balance"].asString()));
            //    printf("sndrefresh.uRglAccountNo%s",sndrefresh.uRglAccountNo);
            //    printf("sndrefresh.uRbalance%s",sndrefresh.uRbalance);
                strcpy(sndrefresh.uRMessageId,Convertstr(root["MessageId"].asString()));
                strcpy(sndrefresh.uRSessionExpiryTime,Convertstr(root["SessionExpiryTime"].asString()));
                strcpy(sndrefresh.uRSessionId,Convertstr(root["SessionId"].asString()));
                strcpy(sndrefresh.uRRouteID,Convertstr(root["RouteID"].asString()));
                strcpy(sndrefresh.uRErrorCode,Convertstr(root["ErrorCode"].asString()));
                strcpy(sndrefresh.uRXMLData,Convertstr(root["XMLData"].asString()));
                strcpy(sndrefresh.uRAuthmanFlag,Convertstr(root["AuthmanFlag"].asString()));
                strcpy(sndrefresh.uRServiceID,Convertstr(root["ServiceID"].asString()));
           //     printf("sndrefresh.uRglAccountNo%s",sndrefresh.uRglAccountNo);
            //    printf("sndrefresh.uRbalance%s",sndrefresh.uRbalance);
            //    qDebug()<<"sndrefresh.uRglAccountNo"<<sndrefresh.uRglAccountNo;
          //      qDebug()<<"sndrefresh.uRbalance"<<sndrefresh.uRbalance;
           //     qDebug()<<"Return success";

                memset(Mer_Latest_Balance,0,sizeof(Mer_Latest_Balance));
                strncpy(Mer_Latest_Balance, sndrefresh.uRbalance, strlen(sndrefresh.uRbalance));
         //       printf("Refresh Mer Balance = %s\n",Mer_Latest_Balance);

                uidmessage::instance()->hide_Message();
//                ui->pushButton_Dash_AEPS_pg12->setHidden(false);
//                ui->PleaseWait__Merchant_DashBoard_label->setHidden(true);
//                ui->PleaseWait__Merchant_DashBoard_label->setEnabled(false);
                return FUNCTION_SUCCESS;
            }
        }
        else
        {
//            ui->pushButton_Dash_AEPS_pg12->setHidden(false);
//            ui->PleaseWait__Merchant_DashBoard_label->setHidden(true);
//            ui->PleaseWait__Merchant_DashBoard_label->setEnabled(false);
            uidmessage::instance()->hide_Message();
            sprintf(ErrorMsg,"%s",sndrefresh.cRResponseMessage);
            miscObj.DisplayMsgBox(ErrorMsg);
            uidmessage::instance()->hide_Message();
            return FUNCTION_FAILED;
        }
    }
    uidmessage::instance()->hide_Message();
//    ui->pushButton_Dash_AEPS_pg12->setHidden(false);
//    ui->PleaseWait__Merchant_DashBoard_label->setHidden(true);
//    ui->PleaseWait__Merchant_DashBoard_label->setEnabled(false);
    return FUNCTION_SUCCESS;

}

void FPB::Balance_Hide_Fun()
{
    HideBL.clear();
#if DEBUG
    printf("Balance Hide Function = %s\n",Mer_Latest_Balance);
#endif
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
        qDebug()<<"Size of star"<<star_size;
        temp1.clear();
        for(i=0;i<star_size;i++)
        {
            temp1 = temp1.append('*');
        }

    //    qDebug()<<"Available Balance"<<bal;
        HideBL = "Balance : " + temp1;
  //      qDebug()<<"show_bal(1) HideBL == "<<HideBL<<"temp1 == "<<temp1;
        show_bal = 1;
        return;
    }

    if(show_bal == 1)
    {
        QString temp1 = QString :: fromUtf8(Mer_Latest_Balance);
        HideBL = "Balance : " + temp1;
  //      qDebug()<<"show_bal(0) HideBL == "<<HideBL<<"temp1 == "<<temp1;
        show_bal = 0;
        return;
    }
    return;
}
void FPB::on_Balance_Show_pushButton_Transaction_page_2_clicked()
{
    Balance_Hide_Fun();
    ui->SubTransaction_Label2->clear();
    ui->SubTransaction_Label2->setText(HideBL);
    return;
}


void FPB::on_Balance_Show_pushButton_UID_page_clicked()
{
    Balance_Hide_Fun();
    ui->UID_LabelBal->clear();
    ui->UID_LabelBal->setText(HideBL);
    return;
}

void FPB::on_Balance_Show_pushButton_FromToDate_page_clicked()
{
    Balance_Hide_Fun();
    ui->FromToDate_LabelBal->clear();
    ui->FromToDate_LabelBal->setText(HideBL);
    return;
}

void FPB::on_Balance_Show_pushButton_BankSelection_page_clicked()
{
    Balance_Hide_Fun();
    ui->BankSelection_LabelBal->clear();
    ui->BankSelection_LabelBal->setText(HideBL);
    return;
}

void FPB::on_Balance_Show_pushButton_BankName_page_clicked()
{

    Balance_Hide_Fun();
    ui->BankName_LabelBal->clear();
    ui->BankName_LabelBal->setText(HideBL);
    return;
}



void FPB::on_Balance_Show_pushButton_Amount_page_3_clicked()
{
    Balance_Hide_Fun();
    ui->EnterAmountLabelBal->clear();
    ui->EnterAmountLabelBal->setText(HideBL);
    return;
}

void FPB::on_Balance_Show_pushButton_Confirm_page_3_clicked()
{
    Balance_Hide_Fun();
     ui->DispConfirmation_LabelBal->clear();
     ui->DispConfirmation_LabelBal->setText(HideBL);
     return;

}


void FPB::on_Balance_Show_pushButton_BankName_page_2_clicked()
{
  Balance_Hide_Fun();
  ui->CustMoNum_LabelBal->clear();
  ui->CustMoNum_LabelBal->setText(HideBL);
  return;
}








