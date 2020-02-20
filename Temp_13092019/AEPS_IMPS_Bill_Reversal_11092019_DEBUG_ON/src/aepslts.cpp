#include "aepslts.h"
#include "ui_FPB.h"
#include "json.h"
#include "FPB.h"
#include "Login.h"
#include "displaymessage.h"
#include "print.h"
extern Login_leg2 configLoginLeg2;
extern Login_leg1 configLoginLeg1;
extern Transaction Txn;
extern Printfpb ConfigPrint;
StrAEPS_LTS display_data[10];

int CountLTS = 0;
char display[100][50];


void FPB::on_LTTS_pushButton_4_clicked()
{

    memset(&AEPSLTS,0,sizeof(AEPSLTS));
    unsigned char BankBin[10];

    memset(BankBin,0,sizeof(BankBin));
    Check_Conditions();

    int ret = AutoLTS();
    if(ret!=FUNC_OK && ret!=FUNCTION_SUCCESS)
    {
        ui->SubTransaction_Label1 ->setText((char *)configLoginLeg2.ucname);
        ui->SubTransaction_Label1 ->setFont(QFont("SansSerif",10));
        ui->SubTransaction_Label1 ->setAlignment(Qt::AlignRight | Qt::AlignBottom);

        ui->SubTransaction_Label2->setText(QString::fromUtf8((const char*)Balance_Display));
        ui->SubTransaction_Label2->setFont(QFont("SansSerif",9));
        ui->SubTransaction_Label2->setAlignment(Qt::AlignRight | Qt::AlignBottom);

        ui->stackedWidget->setCurrentIndex(2);
        return;
    }

    memset(&Txn,0,sizeof(Txn));
    Txn.Txntype = AEPS_LTTS;


    QDate date = QDate::currentDate();
    QTime time = QTime::currentTime();
    QDateTime startDate(date, time);
    QDateTime endDate(date, time);

    qDebug() << "Days from startDate to endDate: " << startDate.daysTo(endDate);
    uidmessage::instance()->hide_Message();
    ui->dateEdit->setDateTime(startDate);
    ui->dateEdit_2->setDateTime(startDate);
    ui->FromToDateLabelName ->setText((char *)configLoginLeg2.ucname);
    ui->FromToDateLabelName ->setFont(QFont("SansSerif",10));
    ui->FromToDateLabelName ->setAlignment(Qt::AlignRight | Qt::AlignBottom);

    ui->FromToDate_LabelBal->setText(QString::fromUtf8((const char*)Balance_Display));
    ui->FromToDate_LabelBal->setFont(QFont("SansSerif",9));
    ui->FromToDate_LabelBal->setAlignment(Qt::AlignRight | Qt::AlignBottom);
    ui->stackedWidget->setCurrentIndex(5);
    // Prepare_AEPS_LTTS_Func();
    return;
}

int FPB::Prepare_AEPS_LTTS_Func()
{
    CMisc miscObj;
    int  ret = -3;
    char ErrorMsg[150];
    int gSendData_len = 0;
    int oBuff_len =0;
    memset(&ConfigPrint,0,sizeof(ConfigPrint));
    memset(&AEPSLTS,0,sizeof(AEPSLTS));
    memset(ErrorMsg,0,sizeof(ErrorMsg));

    uidmessage::instance()->hide_Message();
    uidmessage::instance()->Display_BusyMessage("Please Wait....");

    iGenerateRequestID((char *)configLoginLeg2.ucUsername_req,(unsigned char *)&AEPSLTS.ucRequestId_ALTSreq);
    memcpy(AEPSLTS.ucMethodId_ALTSreq,"521",3);
    memcpy(AEPSLTS.ucTellerID_ALTSreq,configLoginLeg2.ucUsername_req,strlen((char *)configLoginLeg2.ucUsername_req));
    memcpy((char *)AEPSLTS.ucSessionId_ALTSreq,configLoginLeg2.ucSessionId,strlen((char *)configLoginLeg2.ucSessionId));
    //memcpy(AEPSLTS.ucTokenId_ALTSreq,"",5);

    memcpy(AEPSLTS.ucIsEncrypt_ALTSreq,ISENCEYPT,strlen(ISENCEYPT));
    memcpy(AEPSLTS.ucStartDate_ALTSreq,Txn.todate,10);
    memcpy(AEPSLTS.ucEndDate_ALTSreq,Txn.fromdate,10);
    memcpy((char *)AEPSLTS.ucSessionExpiryTime_ALTSreq,configLoginLeg2.ucSessionExpiryTime,strlen((char *)configLoginLeg2.ucSessionExpiryTime));
    memcpy(AEPSLTS.ucChannelID_ALTSreq,CHANNEL_ID,strlen(CHANNEL_ID));
    memcpy(AEPSLTS.ucRouteID_ALTSreq,"Request From: UIService To PBSAEPSProcessRequest",48);
    memcpy(AEPSLTS.ucServiceID_ALTSreq,"18",2);

    //iRet = IacCallCaptureAndSign(&RD_fun);

    ret	= makeJSONAEPSLTS();
    if(ret!=FUNC_OK)
    {
        uidmessage::instance()->hide_Message();
        qDebug()<<"JSON Creation failed";
        return FUNC_ERROR;
    }
    /****************************Encryption function*********************************/
    if(!strcmp((char *)AEPSLTS.ucIsEncrypt_ALTSreq,"true"))
    {
       gSendData_len =strlen((char *)gSendData);
       RequestDetachEncryptAttach((char *)gSendData,1,gSendData_len);
    }
    /***************************************************************/
    uidmessage::instance()->hide_Message();
    uidmessage::instance()->Display_BusyMessage("Transaction Request in Progress");


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
    }


    //    sleep(1);
    //    uidmessage::instance()->hide_Message();
    //    uidmessage::instance()->Display_BusyMessage("Transmitting .....");

    int len_gSendData = strlen((char *)gSendData);
    memset(AEPSLTS_Buff,0,sizeof(AEPSLTS_Buff));
    if((hComm.host_2Sendrecv((unsigned char *)gSendData,len_gSendData,( unsigned char*)oBuff,(int *)&AEPSLTS_len,(int)response_timeout1,0))!=SUCCESS)
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
    strcpy(str_url,URL_CREATION(AEPS_LTTS_URL_PROD));
#else
    strcpy(str_url,URL_CREATION((char *)AEPS_LTTS_URL));
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

    //    uidmessage::instance()->hide_Message();


    //    system("cd /usr/FINO/FPB/REQ/");
    //    system("rm /usr/FINO/FPB/REQ/AEPSLTS_RESP.TXT");
    //    FILE *fp2 = NULL;
    //    fp2 = fopen("/usr/FINO/FPB/REQ/AEPSLTS_RESP.TXT","wb");
    //      fwrite( AEPSLTS_Buff, 1, AEPSLTS_len, fp2 );
    //      fclose(fp2);

    //   // ret=json->iParsJSON(Recvdata,(char *)&AEPSLTS.ucResponseCode,(char *)&AEPSLTS.ucResponseMessage);
    //    system("cd /usr/FINO/FPB/REQ/");
    //    fp1 = fopen("/usr/FINO/FPB/REQ/AEPSLTS_RESP.TXT", "rb"); // open
    //    if (fp1 == NULL)
    //    {
    //        qDebug()<<"Can't Open IntJSON.TXT File";
    //        uidmessage::instance()->hide_Message();
    //        //return FUNCTION_FAILED;
    //        return FUNCTION_FAILED;
    //    }

    //    fseek(fp1, 0, SEEK_END);
    //    template_len = ftell(fp1);
    //    template_data = (unsigned char *)malloc(template_len);

    //    if (template_data == NULL)
    //    {
    //        fclose(fp1);
    //        printf("\n in testbio memeory alloc failed");
    //        uidmessage::instance()->hide_Message();
    //        //return FUNCTION_FAILED;
    //        return FUNCTION_FAILED;
    //    }

    //    fseek(fp1, 0, SEEK_SET);
    //    ret=fread(template_data,sizeof(unsigned char ),template_len,fp1);
    //    if(ret==0)
    //        fclose(fp1);

    /****************************Decryption function*********************************/
    if(!strcmp((char *)AEPSLTS.ucIsEncrypt_ALTSreq,"true"))
    {
        oBuff_len = strlen(oBuff);
 #if DEBUG
        qDebug()<<"Length of buffer"<<oBuff_len;
        printf("Buffer%s\n",oBuff);
 #endif
        ResponseDetachEncryptAttach(oBuff,2,oBuff_len);//TEMPORARY
    }
    /**************************************************************************/


    memset(&display_data,0,sizeof(display_data));
    ret=iParse_AEPSLTS((unsigned char *)oBuff,(char *)&AEPSLTS.ucResponseCode,(char *)&AEPSLTS.ucResponseMessage);
    qDebug()<<"retuyrn value"<<ret;
    system("cd /usr/FINO/FPB/RESP/");
    system("rm /usr/FINO/FPB/RESP/OUTPUT.TXT");
 //   printf("AEPSLTS.ucResponseCode%s/nAEPSLTS.ucResponseMessage%s/n",AEPSLTS.ucResponseCode,AEPSLTS.ucResponseMessage);
    if((ret!=FUNC_OK) && (CountLTS!=-1))
    {
        sprintf(ErrorMsg,"%s",/*(char *)AEPSLTS.ucResponseCode,*/(char *)AEPSLTS.ucResponseMessage);
        miscObj.DisplayMsgBox(ErrorMsg);
        sleep(1);
        uidmessage::instance()->hide_Message();

        if(strcmp((char *)AEPSLTS.ucResponseMessage,"Session Expired.")==0)
        {
            uidmessage::instance()->hide_Message();
            sprintf(ErrorMsg,"%s",/*(char *)LTS.ucResponseCode,*/(char *)AEPSLTS.ucResponseMessage);
            miscObj.DisplayMsgBox(ErrorMsg);
            uidmessage::instance()->hide_Message();
            return SESSION_EXPIRED_FAILED;
        }

        return FUNCTION_FAILED;
    }
    uidmessage::instance()->hide_Message();
    //    uidmessage::instance()->hide_Message();
//    qDebug()<<"After condition";
    int j=0,l=0,icount=10;
    for(j=0,l=0;j<CountLTS;j=j++,l=l+9)
    {
#if DEBUG
        //            // qDebug()<<"display_data[j].AEPS_LTS_data.iDate"<<l++;
        printf("display_data[j].AEPS_LTS_data.iDatej=%d%s\n",j,display_data[j].AEPS_LTS_data.iDate);
        printf("display_data[j].AEPS_LTS_data.iDatej=%d%s\n",j,display_data[j].AEPS_LTS_data.Trantype);
        printf("display_data[j].AEPS_LTS_data.iDatej=%d%s\n",j,display_data[j].AEPS_LTS_data.Amount);
        printf("display_data[j].AEPS_LTS_data.iDatej=%d%s\n",j,display_data[j].AEPS_LTS_data.RRN);
        printf("display_data[j].AEPS_LTS_data.iDatej=%d%s\n",j,display_data[j].AEPS_LTS_data.AadharNo);
        printf("display_data[j].AEPS_LTS_data.iDatej=%d%s\n",j,display_data[j].AEPS_LTS_data.iStatus);
        printf("display_data[j].AEPS_LTS_data.iDatej=%d%s\n",j,display_data[j].AEPS_LTS_data.Bank_Name);
#endif


        sprintf(display[l],"TRANSACTION NO:%d",j+1);
        sprintf(display[l+1],"Date NO    :%s",display_data[j].AEPS_LTS_data.iDate);
        sprintf(display[l+2],"TranType   :%s",display_data[j].AEPS_LTS_data.Trantype);
        sprintf(display[l+3],"Amount     :%s",display_data[j].AEPS_LTS_data.Amount);
        sprintf(display[l+4],"RRN        :%s",display_data[j].AEPS_LTS_data.RRN);
        sprintf(display[l+5],"Aadhaar No :%s",display_data[j].AEPS_LTS_data.AadharNo);
        sprintf(display[l+6],"Status     :%s",display_data[j].AEPS_LTS_data.LTS_Status);
        sprintf(display[l+7],"Bank Name  :%s",display_data[j].AEPS_LTS_data.TransactionComments);
        sprintf(display[l+8],"          ");
        //  sleep(1);
        //printf("inside the for loop%d/n",j);
      //  qDebug()<<"Value of j="<<j<<"l="<<l;

    }


 //  qDebug()<<"Before Display";
    uidmessage::instance()->hide_Message();

    idisplayTxnDetailes(display,CountLTS);//icount);


    memset(&AEPSLTS,0,sizeof(AEPSLTS));
    return FUNC_OK;

}



void FPB::on_OK_pushButton_9_clicked()
{

//    uidmessage::instance()->hide_Message();
//    uidmessage::instance()->Display_BusyMessage("Please Wait...");
    QString todate = ui->dateEdit->date().toString("yyyy-MM-dd") ;
    QString fromdate = ui->dateEdit_2->date().toString("yyyy-MM-dd") ;
    qDebug()<<"todate"<<todate;
    qDebug()<<"fromdate"<<fromdate;
    strcpy(Txn.todate,todate.toAscii().constData());
    strcpy(Txn.fromdate,fromdate.toAscii().constData());
 #if DEBUG
    qDebug()<<" Txn.todate"<< Txn.todate;
    qDebug()<<"Txn.fromdate"<<Txn.fromdate;
#endif
    int iRet_txn;
    memset(gSendData,0,sizeof(gSendData));
    memset(Recvdata,0,sizeof(Recvdata));
    iRet_txn = Prepare_AEPS_LTTS_Func();
    if(iRet_txn == FUNCTION_FAILED || iRet_txn == FUNC_ERROR || iRet_txn == SESSION_EXPIRED_FAILED)
    {
        uidmessage::instance()->hide_Message();
        qDebug()<<"Prepare_AEPS_BAL_Func Failed";
        if(iRet_txn == SESSION_EXPIRED_FAILED)
            ui->stackedWidget->setCurrentIndex(0);
        else
            ui->stackedWidget->setCurrentIndex(2);
        //return FUNCTION_FAILED;
    }
    uidmessage::instance()->hide_Message();
    return;
}

void FPB::on_BACK_pushButton_8_clicked()
{
    ui->dateEdit->clear();
    ui->dateEdit_2->clear();

    ui->stackedWidget->setCurrentIndex(2);  // go to page Main Menu
    return;
}


int FPB::makeJSONAEPSLTS()//----------------------
{
    qDebug()<<"Inside makeJSONAEPSLTS";
    memset(gSendData,0,sizeof(gSendData));
    char tempbuff[30];
    int slen=-2,iRet=-2;

//    uidmessage::instance()->DisplayMessage("Please Wait....");
    iRet = iMakeRequest(9);
    if (iRet!=FUNC_OK)
    {
        return FUNC_ERROR;
    }

    slen=strlen((char *)Recvdata);
    slen-=2;
    memset(gSendData,0,sizeof(gSendData));

#ifdef UAT_HTTP     // File Download

    strncpy((char *)gSendData,"POST /UIService.svc/AEPSProcessRequest HTTP/1.1\r\n",52);
    strncat((char *)gSendData,"Host: 202.58.125.20\r\n",27);//9013
    strncat((char *)gSendData,"Content-Type: application/json; charset=utf-8\r\n",49);
    sprintf(tempbuff,"Content-Length: %d\r\n",slen);
    strncat((char *)gSendData,tempbuff,strlen(tempbuff));
    strcat((char *)gSendData,(char *)Recvdata);

#else
    strncpy((char *)gSendData,(char *)Recvdata,strlen((char *)Recvdata));
#endif

    //memcpy(gSendData,Recvdata,sizeof(Recvdata));
#if DEBUG
    system("cd /usr/FINO/FPB/REQ/");
    system("rm /usr/FINO/FPB/REQ/AEPSLTS_req.TXT");
    FILE *fp = NULL;
    fp =fopen("/usr/FINO/FPB/REQ/AEPSLTS_req.TXT","a");
    fwrite(gSendData,strlen((char *)gSendData),1,fp);
    sleep(1);
    fclose(fp);
#endif
    return FUNC_OK;

}



int FPB::iSetAEPSLTS(int tagID, unsigned char *value)
{
    switch(tagID)
    {
    case  1:
        strcpy((char *)value,(char *)AEPSLTS.ucRequestId_ALTSreq);
        break;
    case  2:
        strcpy((char *)value,(char *)AEPSLTS.ucMethodId_ALTSreq);//Method id
        break;
    case  3:
        strcpy((char *)value,(char *)AEPSLTS.ucTellerID_ALTSreq);//teller id
        break;
    case  4:
        strcpy((char *)value,(char *)AEPSLTS.ucSessionId_ALTSreq);//session key
        break;
    case  5:
        strcpy((char *)value,"");//token id
        break;
    case  6:
        strcpy((char *)value,(char *)AEPSLTS.ucIsEncrypt_ALTSreq);//is encrypt
        break;
    case  7:
        strcpy((char *)value,(char *)AEPSLTS.ucStartDate_ALTSreq);//start date
        break;
    case  8:
        strcpy((char *)value,(char *)AEPSLTS.ucEndDate_ALTSreq);// End date
        break;
    case  9:
        strcpy((char *)value,(char *)AEPSLTS.ucSessionExpiryTime_ALTSreq);//session exp time
        break;
    case  10:
        strcpy((char *)value,(char *)AEPSLTS.ucChannelID_ALTSreq);//channel id
        break;
    case  11:
        strcpy((char *)value,(char *)AEPSLTS.ucRouteID_ALTSreq);//route id
        break;
    case  12:
        strcpy((char *)value,(char *)AEPSLTS.ucServiceID_ALTSreq);//service id
        break;
    default:
        return FUNC_ERROR;
    }
    return FUNC_OK;
}

int FPB::iParse_AEPSLTS(unsigned char *Data,char *RespC,char *RespMes)
{
    int len,i=0,slen,len2=-1;
    int j=0;
    char *RecvJsonData;
    char *CheckData;
    char ErrorMsg[100];
    CMisc miscObj;
    memset(ErrorMsg,0,sizeof(ErrorMsg));

    //char gSendData[1024];

    char *AEPSLTS_Tags[]={"RequestId","DisplayMessage","ResponseMessage","ResponseCode","MessageType","MessageId","SessionExpiryTime","SessionId",
                          "RouteID","ErrorCode","XMLData","AuthmanFlag","ServiceID"};

    char *AEPSLTSTags[] = {
        "Datetime","TransactionType","ReferenceNo","Amount","AadharNo","CustomerMobileNo","TransactionComments","Status","LTS_Status"};

    int MandTag[]={1,1,1,1,1,1,1,1,0,0,0,0,0};//1 for mandatory and  0 for non-mandatory by saurabh
    int ifOffset[]={3,2,3,3,3,2,2,2,3,2,2,2,2};//forward
    int ibOffset[]={1,0,1,1,1,0,0,0,1,0,0,0,0};//backward
    int ret_JV = json->iIsJsonValid((char *)Data,RespC,RespMes,0);
    qDebug()<<"Response code"<<RespC;
    if ((memcmp(RespC,"0",1)!=0) || (ret_JV == FUNC_ERROR))
    {
        qDebug()<<"RespC"<<RespC;
        qDebug()<<"iParseJSON 3";
        return FUNC_ERROR;
    }

    len=sizeof(AEPSLTS_Tags)/sizeof(AEPSLTS_Tags[0]);
    len2=sizeof(MandTag)/sizeof(MandTag[0]);
    qDebug()<<"len"<<len;
    qDebug()<<"len2"<<len2;
    for (i = 0; i < len; ++i)
    {
        RecvJsonData=strstr((char *)Data,AEPSLTS_Tags[i]);
        slen=strlen(AEPSLTS_Tags[i]);

        if (RecvJsonData==NULL && MandTag[i]==1)//If Tag not found and its mandatory.  //Test Later
        {
            //DrawString("TAG Missing",15,25,0);
            // ttestall(0,200);
            memset(gSendData,0,sizeof(gSendData));
            return FUNC_ERROR;
        }
        else if(RecvJsonData == NULL) //Else proceed to further parsing.
            continue;

        memset(gSendData,0,sizeof(gSendData));

        RecvJsonData=&RecvJsonData[slen+ifOffset[i]];

        CheckData=RecvJsonData;
        do {
            CheckData++;
        } while ((*CheckData!=',') && (*CheckData!='}'));

        CheckData-=ibOffset[i];

        memset(gSendData,0,sizeof(gSendData));
        strncpy((char *)gSendData,RecvJsonData,CheckData-RecvJsonData);

        switch (i)
        {
        case 0:
            strcpy((char *)AEPSLTS.ucRequestId,(char *)gSendData);
            break;

        case 1:
            strcpy((char *)AEPSLTS.ucDisplayMessage,(char *)gSendData);
            break;

        case 2:
            strcpy((char *)AEPSLTS.ucResponseMessage,(char *)gSendData);
            break;

        case 3:
            strcpy((char *)AEPSLTS.ucResponseCode,(char *)gSendData);
            break;

        case 4:
            strcpy((char *)AEPSLTS.ucMessageType,(char *)gSendData);
            break;

        case 5:
            strcpy((char *)AEPSLTS.ucMessageId,(char *)gSendData);
            break;

        case 6:
            strcpy((char *)AEPSLTS.ucSessionExpiryTime,(char *)gSendData);
            break;

        case 7:
            strcpy((char *)AEPSLTS.ucSessionId,(char *)gSendData);
            break;

        case 8:
            strcpy((char *)AEPSLTS.ucRouteID,(char *)gSendData);
            break;

        case 9:
            strcpy((char *)AEPSLTS.ucErrorCode,(char *)gSendData);
            break;

        case 10:
            strcpy((char *)AEPSLTS.ucXMLData,(char *)gSendData);
            break;

        case 11:
            strcpy((char *)AEPSLTS.ucAuthmanFlag,(char *)gSendData);
            break;

        case 12:
            strcpy((char *)AEPSLTS.ucServiceID,(char *)gSendData);
            break;
        default:
            break;
        }
    }

    CountLTS = json->iTagCount((unsigned char *)Data, "Datetime");
    if(CountLTS == -2)
    {
        qDebug()<<"Transaction Data Not Found";
        uidmessage::instance()->hide_Message();
        if(!(memcmp((char *)AEPSLTS.ucResponseCode,"0",1)))
        {
            //            memset(ErrorMsg,0,sizeof(ErrorMsg));
            //            sprintf(ErrorMsg,"Response Code:%s \n Message:%s",(char *)AEPSLTS.ucResponseCode,(char *)AEPSLTS.ucResponseMessage);
            //            miscObj.DisplayMsgBox(ErrorMsg);
        }
        else
        {
            CountLTS=-1;
            miscObj.DisplayMsgBox((char *)"Transaction data not found");
        }
        uidmessage::instance()->hide_Message();
        memset(gSendData,0,sizeof(gSendData));
        return FUNCTION_FAILED;
    }

    RecvJsonData=(char *)Data;
//    qDebug()<<"before Final Data Calculations (CountLTS) "<<CountLTS;
    for (i = 0; i < CountLTS; ++i)
    {
        if(CountLTS <= 10)
        {
            for (j = 0; j < 9; ++j)
            {
                RecvJsonData=strstr((char *)RecvJsonData,AEPSLTSTags[j]);
                slen=strlen((char *)AEPSLTSTags[j]);

                if (RecvJsonData==NULL)
                {
                    //DrawString("TAG Missing",15,25,0);
                    //ttestall(0,200);
                    memset(gSendData,0,sizeof(gSendData));
                    return FUNC_ERROR;
                }

                memset(gSendData,0,sizeof(gSendData));
                    if(!strcmp((char *)AEPSLTS.ucIsEncrypt_ALTSreq,"true"))
                    {
                        RecvJsonData=&RecvJsonData[slen+3];//Change 20032019 Aadarsha
                        CheckData=strchr(RecvJsonData,'\"');//Change 20032019 Aadarsha
                    }
                    else
                    {
                           RecvJsonData=&RecvJsonData[slen+5];//change for Encryption Aadarsha
                           CheckData=strchr(RecvJsonData,'\\');//change for Encryption Aadarsha
                    }



                //CheckData--;
                memset(gSendData,0,sizeof(gSendData));
                strncpy((char *)gSendData,RecvJsonData,CheckData-RecvJsonData);
 //               qDebug()<< "inside the data parssefield j ="<<j;
                switch (j)
                {
                case 0:
                    strcpy((char *)display_data[i].AEPS_LTS_data.iDate,(char *)gSendData);
  //                  printf("Parse value%s",display_data[i].AEPS_LTS_data.iDate);
                    break;

                case 1:
                    strcpy((char *)display_data[i].AEPS_LTS_data.Trantype,(char *)gSendData);
   //                 printf("Parse value%s",display_data[i].AEPS_LTS_data.Trantype);
                    break;

                case 2:
                    strcpy((char *)display_data[i].AEPS_LTS_data.RRN,(char *)gSendData);
    //                printf("Parse value%s",display_data[i].AEPS_LTS_data.RRN);
                    break;

                case 3:
                    strcpy((char *)display_data[i].AEPS_LTS_data.Amount,(char *)gSendData);
   //                 printf("Parse value%s",display_data[i].AEPS_LTS_data.Amount);
                    break;

                case 4:
                    strcpy((char *)display_data[i].AEPS_LTS_data.AadharNo,(char *)gSendData);
    //                printf("Parse value%s",display_data[i].AEPS_LTS_data.AadharNo);
                    break;

                case 5:
                    strcpy((char *)display_data[i].AEPS_LTS_data.CustomerMobileNo,(char *)gSendData);
     //               printf("Parse value%s",display_data[i].AEPS_LTS_data.CustomerMobileNo);
                    break;

                case 6:
                    strcpy((char *)display_data[i].AEPS_LTS_data.TransactionComments,(char *)gSendData);
      //              printf("Parse value%s",display_data[i].AEPS_LTS_data.TransactionComments);
                    break;

                case 7:
                    strcpy((char *)display_data[i].AEPS_LTS_data.iStatus,(char *)gSendData);
     //               printf("Parse value%s",display_data[i].AEPS_LTS_data.iStatus);
                    break;

                case 8:
                    strcpy((char *)display_data[i].AEPS_LTS_data.LTS_Status,(char *)gSendData);
     //               printf("Parse value%s",display_data[i].AEPS_LTS_data.LTS_Status);
                    break;

                default:
                    break;

                }
            }
        }
        else
        {
            qDebug()<<"else countLTS < 10 ";
            i = CountLTS+1;
            break;
        }
    }

    display_data[i].next  = &display_data[i+1];
    i++;
    display_data[i].next = NULL;
    qDebug()<<"After switch";
    memset(gSendData,0,sizeof(gSendData));
    return FUNC_OK;
}
