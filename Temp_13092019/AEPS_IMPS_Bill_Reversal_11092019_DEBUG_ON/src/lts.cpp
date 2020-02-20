#include "lts.h"
#include "ui_FPB.h"
#include "json.h"
#include "FPB.h"
#include "Login.h"
#include "displaymessage.h"
#include "print.h"
#include <QSqlDatabase>

extern Login_leg2 configLoginLeg2;
extern Login_leg1 configLoginLeg1;
extern RD_SERVICE_STRUCTURE RD_Service_data;
extern Transaction Txn;
extern Printfpb ConfigPrint;

StrAEPS_LTS display_LTS[2];
//StrAEPS_LTS display_data[1];
char display_lts[10][50];
char fromdatabase[10][50];
int count_ALTS = 1;


void FPB::on_LTS_pushButton_TxnMenu_clicked()
{
    int iret=-3;
    //unsigned char tmp[30];
    memset(&ConfigPrint,0,sizeof(ConfigPrint));
    //memcpy(tmp,AEPSLTS.ucX_CORRELATION_ID_LTSreq,sizeof(AEPSLTS.ucX_CORRELATION_ID_LTSreq));
    memset(&LTS,0,sizeof(LTS));
    //memcpy(AEPSLTS.ucX_CORRELATION_ID_LTSreq,tmp,sizeof(tmp));
    unsigned char BankBin[10];
    memset(BankBin,0,sizeof(BankBin));
    Check_Conditions();

    ui->LTSNLTTS_LabelName ->setText((char *)configLoginLeg2.ucname);
    ui->LTSNLTTS_LabelName ->setFont(QFont("SansSerif",10));
    ui->LTSNLTTS_LabelName ->setAlignment(Qt::AlignRight | Qt::AlignBottom);

    ui->LTSNLTTS_LabelBal->setText(QString::fromUtf8((const char*)Balance_Display));
    ui->LTSNLTTS_LabelBal->setFont(QFont("SansSerif",9));
    ui->LTSNLTTS_LabelBal->setAlignment(Qt::AlignRight | Qt::AlignBottom);
    count_ALTS = 1;
    Txn.Txntype = AEPS_LTS;
    iret = LTS_FUNC();
    if(iret == FUNCTION_FAILED || iret == FUNC_ERROR || iret == SESSION_EXPIRED_FAILED)
    {
        //printf("*************  iret LTS = %s *****\n", iret);
        count_ALTS = 1;
        if(iret == SESSION_EXPIRED_FAILED)
            ui->stackedWidget->setCurrentIndex(0);
        else
        {
            ui->SubTransaction_Label1 ->setText((char *)configLoginLeg2.ucname);
            ui->SubTransaction_Label1 ->setFont(QFont("SansSerif",10));
            ui->SubTransaction_Label1 ->setAlignment(Qt::AlignRight | Qt::AlignBottom);

            ui->SubTransaction_Label2->setText(QString::fromUtf8((const char*)Balance_Display));
            ui->SubTransaction_Label2->setFont(QFont("SansSerif",9));
            ui->SubTransaction_Label2->setAlignment(Qt::AlignRight | Qt::AlignBottom);
            ui->stackedWidget->setCurrentIndex(2);
        }
    }
    return;
}

int FPB::AutoLTS()
{
    uidmessage::instance()->hide_Message();
    uidmessage::instance()->Display_BusyMessage("Please Wait...");
    //    int iret=-3;
    //    memset(&ConfigPrint,0,sizeof(ConfigPrint));
    //    memset(&LTS,0,sizeof(LTS));
    //    unsigned char BankBin[10];
    //    memset(BankBin,0,sizeof(BankBin));
    //    Check_Conditions();
    //    Txn.Txntype = AEPS_LTS;
    //    iret = LTS_FUNC();
    //    if(iret == FUNCTION_FAILED || iret == FUNC_ERROR)
    //        ui->stackedWidget->setCurrentIndex(2);
    //    return iret;

    int ret_al = -1;
    count_ALTS = 1;
    QSqlQuery query;
    QLTSflag.clear();
    query.clear();
    query.prepare("select ALTSYesNo from AUTOLTS");
    if(!query.exec())
    {
        qDebug()<<"select flag for LTS failed "<<query.lastError();
    }
    else
    {
        while(query.next())
        {
            QLTSflag = query.value(0).toString().trimmed();
        }
    }

    if(QLTSflag == "Y")
    {
        qDebug()<<"AEPS BL QLTSflag"<<QLTSflag;

AGAIN:  ret_al = LTS_FUNC();
        uidmessage::instance()->hide_Message();
        if(ret_al == FUNCAL_FAILED || (ret_al == FUNCTION_FAILED || ret_al == FUNC_ERROR))
        {
            qDebug()<<"count_ALTS out = "<<count_ALTS;
            if(count_ALTS<=3) // 3 Times Auto LTS will fired
            {
                ret_al=-9;
                qDebug()<<"count_ALTS = "<<count_ALTS;
                memset(&LTS,0,sizeof(LTS));
                goto AGAIN;
            }
            else
            {
                count_ALTS = 1;
                qDebug()<<"AUTO LTS FUNCION FAILED WITH TRANSACTION";
                query.clear();
                query.exec("update AUTOLTS set ALTSYesNo='N'");
                query.clear();
            }
        }
        else if(ret_al == FUNCTION_SUCCESS || ret_al == FUNC_OK)
        {
            qDebug()<<"Auto LTS Success";
            query.clear();
            query.exec("update AUTOLTS set ALTSYesNo='N'");
            query.clear();
        }
    }
    else
    {
      //  qDebug()<<"Auto LTS flag is N";
        ret_al = FUNC_OK;
    }
    //uidmessage::instance()->hide_Message();
    count_ALTS = 1;
    return ret_al;

}


int FPB::LTS_FUNC()
{

    uidmessage::instance()->hide_Message();
    uidmessage::instance()->Display_BusyMessage("Please Wait...");
    memset(&ConfigPrint,0,sizeof(ConfigPrint));
    memset(&LTS,0,sizeof(LTS));
    unsigned char BankBin[10];
    memset(BankBin,0,sizeof(BankBin));
    //    Check_Conditions();
    //    uidmessage::instance()->hide_Message();
    //    uidmessage::instance()->Display_BusyMessage("Please Wait...");
    Txn.Txntype = AEPS_LTS;
    int gSendData_len = 0;
    int oBuff_len =0;
    char ErrorMsg[150];
    int ret,res;
    int iRet = -1;
    HostComm hComm;
    CMisc miscObj;
    int template_len=0;
    unsigned char  *template_data;
    FILE *fp1;
    memset(ErrorMsg,0,sizeof(ErrorMsg));
    memset(LTS.ucSessionId_LTSreq,0,sizeof(LTS.ucSessionId_LTSreq));
    memset(LTS.ucSessionExpiryTime_LTSreq,0,sizeof(LTS.ucSessionExpiryTime_LTSreq));
    memset(LTS.ucMethodId_LTSreq,0,sizeof(LTS.ucMethodId_LTSreq));
    memset(LTS.ucTellerID_LTSreq,0,sizeof(LTS.ucTellerID_LTSreq));
    memset(LTS.ucIsEncrypt_LTSreq,0,sizeof(LTS.ucIsEncrypt_LTSreq));
    memset(LTS.ucX_CORRELATION_ID_LTSreq,0,sizeof(LTS.ucX_CORRELATION_ID_LTSreq));
    memset(LTS.ucChannelID_LTSreq,0,sizeof(LTS.ucChannelID_LTSreq));
    memset(LTS.ucRouteID_LTSreq,0,sizeof(LTS.ucRouteID_LTSreq));
    memset(LTS.ucServiceID_LTSreq,0,sizeof(LTS.ucServiceID_LTSreq));

    memcpy((char *)LTS.ucSessionId_LTSreq,configLoginLeg2.ucSessionId,strlen((char *)configLoginLeg2.ucSessionId));
    memcpy((char *)LTS.ucSessionExpiryTime_LTSreq,configLoginLeg2.ucSessionExpiryTime,strlen((char *)configLoginLeg2.ucSessionExpiryTime));
    memcpy(LTS.ucMethodId_LTSreq,"520",3);
    //memcpy(AEPSLTS.ucTellerID_LTSreq,configLoginLeg2.ucUsername_req,strlen((char *)configLoginLeg2.ucUsername_req));
    memcpy((char *)LTS.ucTellerID_LTSreq,(char *)configLoginLeg2.ucUsername_req,strlen((char *)configLoginLeg2.ucUsername_req));
  //  printf("\n\nLTS.ucTellerID_LTSreq == %s\n\n",LTS.ucTellerID_LTSreq);
    // memcpy(LTS.ucIsEncrypt_LTSreq,"true",5);
    memcpy((char *)LTS.ucIsEncrypt_LTSreq,ISENCEYPT,strlen(ISENCEYPT));
    //strcpy((char *)LTS.ucX_CORRELATION_ID_LTSreq,"101038618_05022019155227");
    // memcpy(LTS.ucX_CORRELATION_ID_LTSreq,gstPFRData.ucLastCorelationid,strlen(gstPFRData.ucLastCorelationid));
    //  memcpy(LTS.ucChannelID_LTSreq,"2",1); // comment as per discusion with MPOS team
    memcpy(LTS.ucChannelID_LTSreq,CHANNEL_ID,strlen(CHANNEL_ID));
    //  memcpy(LTS.ucRouteID_LTSreq,"Request From: UIService To PBSAEPSProcessRequest",48); // comment as per discusion with MPOS team
    memcpy(LTS.ucRouteID_LTSreq,"",48);
    memcpy(LTS.ucServiceID_LTSreq,"18",2);

    iGenerateRequestID((char *)configLoginLeg2.ucUsername_req,(unsigned char *)&LTS.ucRequestId_LTSreq);
    // strcpy((char*)LTS.ucX_CORRELATION_ID_LTSreq, "101068318_10022019153159");

    count_ALTS++;

    QSqlQuery query;

    query.prepare("SELECT X_CORRELATION_ID from LST");
    if(!query.exec())
    {
        qDebug() << query.lastError();
        qDebug() << "LST database query selection failed";
        uidmessage::instance()->hide_Message();
        uidmessage::instance()->DisplayMessage("Record Not Found..");
        qDebug()<<"FUNCTION_FAILED";
        query.clear();
        query.finish();
        ui->stackedWidget->setCurrentIndex(2);
        return FUNCTION_FAILED;
    }
    else
    {
        while(query.next())
        {
            memset(LTS.ucX_CORRELATION_ID_LTSreq,0,sizeof(LTS.ucX_CORRELATION_ID_LTSreq));
            strcpy((char*)LTS.ucX_CORRELATION_ID_LTSreq, query.value(0).toString().trimmed().toAscii().data());
 //           printf("LTS.ucX_CORRELATION_ID_LTSreq = %s\n",LTS.ucX_CORRELATION_ID_LTSreq);
        }
    }


    //    uidmessage::instance()->hide_Message();
    //    uidmessage::instance()->DisplayMessage("Please Wait...");

    ret	= makeJSONLTS();
    if(ret!=FUNC_OK)
    {
        qDebug()<<"JSON Creation failed";
        qDebug() << "Failed to connect to the Server";
        uidmessage::instance()->hide_Message();
        return FUNC_ERROR;
    }

    /****************************Encryption function*********************************/
    if(!strcmp((char *)LTS.ucIsEncrypt_LTSreq,"true"))
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
        return FUNC_ERROR;
    }

    //    sleep(1);
    //    uidmessage::instance()->hide_Message();
    //    uidmessage::instance()->Display_BusyMessage("Transmitting .....");

    int len_gSendData = strlen((char *)gSendData);
    memset(LTS_Buff,0,sizeof(LTS_Buff));
    if((hComm.host_2Sendrecv((unsigned char *)gSendData,len_gSendData,( unsigned char*)oBuff,(int *)&LTS_len,(int)response_timeout1,0))!=SUCCESS)
    {
        printf("No Response From Server\n");
        hComm.close_connection();
        miscObj.DisplayMsgBox((char *)"Response Time-Out");
        uidmessage::instance()->hide_Message();
        qDebug()<<"***   Response Time-Out   ***";
        return FUNCAL_FAILED;
        ////ui->stackedWidget->setCurrentIndex(1);
    }
    hComm.close_connection();

#else     // File Download full condition
#if DEBUG
    printf("gSendData = %s\n\n",gSendData);
#endif

    memset(str_url,0,sizeof(str_url));
#ifdef PRODUCTION
    strcpy(str_url,URL_CREATION(AEPS_LTS_URL_PROD));
#else
    strcpy(str_url,URL_CREATION((char *)AEPS_LTS_URL));
#endif

    int retk = file_download(str_url, (char *)gSendData,"/usr/FINO/FPB/RESP/OUTPUT.TXT",150);
    if(retk!=0)
    {
        qDebug()<<"retk 121212 = "<<retk;
        //        sleep(1);
        uidmessage::instance()->hide_Message();
        return FUNCTION_FAILED;
    }

#endif

    /****************************Decryption function*********************************/
    if(!strcmp((char *)LTS.ucIsEncrypt_LTSreq,"true"))
    {
        oBuff_len = strlen(oBuff);
#if DEBUG
        qDebug()<<"Length of buffer"<<oBuff_len;
        printf("Buffer%s\n",oBuff);
#endif
        ResponseDetachEncryptAttach(oBuff,2,oBuff_len);//TEMPORARY
    }
    /**************************************************************************/

    ret=json->iParsJSON((unsigned char *)oBuff,(char *)&LTS.ucResponseCode,(char *)&LTS.ucResponseMessage);
    system("cd /usr/FINO/FPB/RESP/");
    system("rm /usr/FINO/FPB/RESP/OUTPUT.TXT");
    if(ret!=FUNC_OK)
    {
#if DEBUG
        printf("LTS.ucResponseCode = %s\n",LTS.ucResponseCode);
        printf("************  LTS.ucResponseMessage = %s  ************\n", LTS.ucResponseMessage);
 #endif
        if(strcmp((char *)LTS.ucResponseMessage,"Session Expired.")==0)
        {
            uidmessage::instance()->hide_Message();
            sprintf(ErrorMsg,"%s",/*(char *)LTS.ucResponseCode,*/(char *)LTS.ucResponseMessage);
            miscObj.DisplayMsgBox(ErrorMsg);
            uidmessage::instance()->hide_Message();
            //            ui->stackedWidget->setCurrentIndex(0);
            return SESSION_EXPIRED_FAILED;
        }
        //        if((strcmp((char *)LTS.ucResponseMessage,"SessionExpired")==0) || (strcmp((char *)LTS.ucResponseMessage,"Session Expired.")==0))
        //        {
        //            miscObj.DisplayMsgBox("Session Is Expired. Please Login");
        //            uidmessage::instance()->hide_Message();
        //            ui->stackedWidget->setCurrentIndex(0);
        //            return FUNCTION_FAILED;
        //        }
        sprintf(ErrorMsg,"%s",/*(char *)LTS.ucResponseCode,*/(char *)LTS.ucResponseMessage);
        miscObj.DisplayMsgBox(ErrorMsg);

        uidmessage::instance()->hide_Message();
        if (memcmp(LTS.ucResponseCode,"1",1))
        {
            qDebug()<<"LTS Check";
            //uidmessage::instance()->hide_Message();
            return FUNCTION_SUCCESS;
        }
        if ((memcmp(LTS.ucResponseCode,"1",1) == 0) && (strcmp((const char*)LTS.ucResponseMessage,"Records not found from BLL Database.") == 0))
        {
            //uidmessage::instance()->hide_Message();
            return FUNCTION_SUCCESS;
        }
        //uidmessage::instance()->hide_Message();
        ////return FUNCTION_FAILED;
        return FUNCAL_FAILED;
    }



    if (!memcmp(LTS.ucResponseCode,"0",1))
    {
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

        iRet = iParse_LTS((unsigned char*)template_data);
        free(template_data);
        //        if (iRet != FUNC_OK)
        //        {
        //            qDebug()<<"Inside Pardse  error";
        //            miscObj.DisplayMsgBox("LTS Failed");
        //            uidmessage::instance()->hide_Message();
        //            return FUNCTION_FAILED;
        //        }
        system("cd /usr/FINO/FPB/RESP/");
        system("rm /usr/FINO/FPB/RESP/OUTPUT.TXT");
        system("rm /usr/FINO/FPB/REQ/IntJSON.TXT");
        if (iRet == FUNC_OK)
        {
            uidmessage::instance()->hide_Message();
            uidmessage::instance()->Display_BusyMessage("LTS Successful");
            sleep(5);
            uidmessage::instance()->hide_Message();

        }
        else
        {
            qDebug()<<"Inside Pardse  error";
            miscObj.DisplayMsgBox((char *)"LTS Failed");
            uidmessage::instance()->hide_Message();
            ////return FUNCTION_FAILED;
            return FUNCAL_FAILED;
        }
        query.prepare("SELECT TransactionTypeName,Date,Time,TxnAmt,TxnStatus,RRN,AadharNo,Bankname from AUTOLTS");
        if(!query.exec())
        {
            qDebug() << query.lastError();
            qDebug() << "LST database query selection failed";
            uidmessage::instance()->hide_Message();
            uidmessage::instance()->DisplayMessage("Record Not Found..");
            qDebug()<<"FUNCTION_FAILED";
            query.clear();
            query.finish();
            ui->stackedWidget->setCurrentIndex(2);
            return FUNCTION_FAILED;
        }
        else
        {
            while(query.next())
            {
                strcpy(fromdatabase[0],query.value(0).toString().toAscii().data());
                strcpy(fromdatabase[1],query.value(1).toString().toAscii().data());
                strcpy(fromdatabase[2],query.value(2).toString().toAscii().data());
                strcpy(fromdatabase[3],query.value(3).toString().toAscii().data());
                strcpy(fromdatabase[4],query.value(4).toString().toAscii().data());
                strcpy(fromdatabase[5],query.value(5).toString().toAscii().data());
                strcpy(fromdatabase[6],query.value(6).toString().toAscii().data());
                strcpy(fromdatabase[7],query.value(7).toString().toAscii().data());
            }
        }
        char Aadhar[20];
        strcpy(Aadhar,fromdatabase[6]);
        int j=0,l=0;
        for(j=0,l=0;j<1;j=j++,l=l+7)
        {
            sprintf(display_lts[l+1],"Date Time  :%s  %s",fromdatabase[1],fromdatabase[2]);
            sprintf(display_lts[l+2],"TranType   :%s",fromdatabase[0]) ;
            sprintf(display_lts[l+3],"Amount     :%s",fromdatabase[3]);
            sprintf(display_lts[l+4],"RRN        :%s",fromdatabase[5]);
            sprintf(display_lts[l+5],"Aadhaar No :********%s", &(Aadhar[strlen (Aadhar) - 4]));
            sprintf(display_lts[l+6],"Bank Name  :%s",fromdatabase[7]);
            if(strcmp((char *)LTS.ucRec1RESPCODE,"00") == 0)
            {
                sprintf(display_lts[l+7],"     LTS SUCCESSFUL");
            }
            else
            {
                sprintf(display_lts[l+7],"      LTS FAILED");
            }

            //                sprintf(display[l+6],"Status     :%s",display_data[j].AEPS_LTS_data.iStatus);
            //                sprintf(display[l+8],"          ");
            //  sleep(1);
            //printf("inside the for loop%d/n",j);
   //         qDebug()<<"Value of j="<<j<<"l="<<l;

        }
        idisplayTxnDetailes(display_lts,1);//icount);
    }
    else
    {

        hComm.close_connection();
        miscObj.DisplayMsgBox((char *)"NO RECORD FOUND");
        uidmessage::instance()->hide_Message();
        return FUNCTION_FAILED;
    }

    memset(&LTS,0,sizeof(LTS));
    return FUNC_OK;
}

int FPB::makeJSONLTS()
{
    memset(gSendData,0,sizeof(gSendData));
    //    char tempbuff[30];
    int slen=-2,iRet=-2;


    iRet = iMakeRequest(5);
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
    system("rm /usr/FINO/FPB/REQ/LTS_req.TXT");
    FILE *fp = NULL;
    fp =fopen("/usr/FINO/FPB/REQ/LTS_req.TXT","a");
    fwrite(gSendData,strlen((char *)gSendData),1,fp);
    sleep(1);
    fclose(fp);
#endif
    return FUNC_OK;
}


int FPB::iSetLTS(int tagID, unsigned char *value)
{
    switch(tagID)
    {
    case  1:
        strcpy((char *)value,(char *)LTS.ucRequestId_LTSreq);
        break;
    case  2:
        strcpy((char *)value,(char *)LTS.ucMethodId_LTSreq);//Method id
        break;
    case  3:
        strcpy((char *)value,(char *)LTS.ucTellerID_LTSreq);//teller id
        break;
    case  4:
        strcpy((char *)value,(char *)LTS.ucSessionId_LTSreq);//session key
        break;
    case  5:
        strcpy((char *)value,"");//token id
        break;
    case  6:
        strcpy((char *)value,(char *)LTS.ucIsEncrypt_LTSreq);//is encrypt
        break;
    case  7:
        strcpy((char *)value,(char *)LTS.ucSessionExpiryTime_LTSreq);
        break;
    case  8:
        strcpy((char *)value,(char *)LTS.ucChannelID_LTSreq);
        break;
    case  9:
        strcpy((char *)value,(char *)LTS.ucRouteID_LTSreq);
        break;
        //    case  10:
        //        strcpy((char *)value,(char *)LTS.ucServiceID_LTSreq);
        //        break;
    case  11:
        strcpy((char *)value,(char *)LTS.ucX_CORRELATION_ID_LTSreq);
        break;
    default:
        return FUNC_ERROR;
    }
    return FUNC_OK;
}

int FPB::iParse_LTS(unsigned char *Data)
{
    qDebug()<<"iParse LTS";
    int len,i=0,slen,len2=-1;
    //int j=0;
    char *RecvJsonData;
    char *CheckData;
    //char gSendData[1024];

    char *LoginLeg1[]={"RequestId:",
                       "DisplayMessage:",
                       "ResponseMessage:",
                       "ResponseCode:",
                       "MessageType:",
                       "ResponseData:returnCode:",
                       "ResponseData:X_CORRELATION_ID:",
                       "ResponseData:Trace:",
                       "ResponseData:ResponseData:Record1:PCODE:",
                       "ResponseData:ResponseData:Record1:MSGTYPE:",
                       "ResponseData:ResponseData:Record1:RESPCODE:",//10
                       "ResponseData:ResponseData:Record1:Response_Msg:",
                       "ResponseData:ResponseData:Record2:PCODE:",
                       "ResponseData:ResponseData:Record2:RESPCODE:",
                       "ResponseData:ResponseData:Record1:PCODE:",
                       "MessageId:",
                       "SessionExpiryTime:",
                       "SessionId:",
                       "RouteID:",
                       "ErrorCode:",
                       "XMLData:",
                       "AuthmanFlag:",
                       "ServiceID:",//28
                      };

    int MandTag[]={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0};//1 for mandatory and  0 for non-mandatory by saurabh

    len=sizeof(LoginLeg1)/sizeof(LoginLeg1[0]);
    len2=sizeof(MandTag)/sizeof(MandTag[0]);
    for (i = 0; i < len; ++i)
    {
        RecvJsonData=strstr((char *)Data,LoginLeg1[i]);
        slen=strlen(LoginLeg1[i]);

        if (RecvJsonData==NULL && MandTag[i]==1)//If Tag not found and its mandatory.  //Test Later
        {
            //DrawString("TAG Missing",15,25,0);
            // ttestall(0,200);
            qDebug()<<"iParse LTS FUNC_ERROR";
            memset(gSendData,0,sizeof(gSendData));
            return FUNC_ERROR;
        }
        else if(RecvJsonData == NULL) //Else proceed to further parsing.
            continue;

        memset(gSendData,0,sizeof(gSendData));

        RecvJsonData=&RecvJsonData[slen];

        CheckData=strchr(RecvJsonData,'\n');
        //CheckData--;
        memset(gSendData,0,sizeof(gSendData));
        strncpy((char *)gSendData,RecvJsonData,CheckData-RecvJsonData);

        switch (i)
        {
        case 0:
            strcpy((char *)LTS.ucRequestId,(char *)gSendData);
      //      printf("LTS.ucRequestId == %s\n",LTS.ucRequestId);
            break;

        case 1:
            strcpy((char *)LTS.ucDisplayMessage,(char *)gSendData);
      //      printf("LTS.ucDisplayMessage == %s\n",LTS.ucDisplayMessage);
            break;

        case 2:
            strcpy((char *)LTS.ucResponseMessage,(char *)gSendData);
   //         printf("LTS.ucResponseMessage == %s\n",LTS.ucResponseMessage);
            break;

        case 3:
            strcpy((char *)LTS.ucResponseCode,(char *)gSendData);
    //        printf("LTS.ucResponseCode == %s\n",LTS.ucResponseCode);
            break;

        case 4:
            strcpy((char *)LTS.ucMessageType,(char *)gSendData);
   //         printf("LTS.ucMessageType == %s\n",LTS.ucMessageType);
            break;

        case 5:
            strcpy((char *)LTS.ucreturnCode,(char *)gSendData);
   //         printf("LTS.ucreturnCode == %s\n",LTS.ucreturnCode);
            break;

        case 6:
            strcpy((char *)LTS.ucX_CORRELATION_ID,(char *)gSendData);
    //        printf("LTS.ucX_CORRELATION_ID == %s\n",LTS.ucX_CORRELATION_ID);
            break;

        case 7:
            strcpy((char *)LTS.ucTrace,(char *)gSendData);
    //        printf("LTS.ucTrace == %s\n",LTS.ucTrace);
            break;

        case 8:
            strcpy((char *)LTS.ucRec1PCODE1,(char *)gSendData);
    //        printf("LTS.ucRec1PCODE1 == %s\n",LTS.ucRec1PCODE1);
            break;

        case 9:
            strcpy((char *)LTS.ucRec1MSGTYPE,(char *)gSendData);
   //         printf("LTS.ucRec1MSGTYPE == %s\n",LTS.ucRec1MSGTYPE);
            break;

        case 10:
            strcpy((char *)LTS.ucRec1RESPCODE,(char *)gSendData);
   //         printf("LTS.ucRec1RESPCODE == %s\n",LTS.ucRec1RESPCODE);
            break;

        case 11:
            strcpy((char *)LTS.ucRec1ResponseMsg,(char *)gSendData);
   //         printf("LTS.ucRec1ResponseMsg == %s\n",LTS.ucRec1ResponseMsg);
            break;

        case 12:
            strcpy((char *)LTS.ucRec2PCODE2,(char *)gSendData);
 //           printf("LTS.ucRec2PCODE2 == %s\n",LTS.ucRec2PCODE2);
            break;

        case 13:
            strcpy((char *)LTS.ucRec2RESPCODE,(char *)gSendData);
  //          printf("LTS.ucRec2RESPCODE == %s\n",LTS.ucRec2RESPCODE);
            break;

        case 14:
            strcpy((char *)LTS.ucMessageId,(char *)gSendData);
   //         printf("LTS.ucMessageId == %s\n",LTS.ucMessageId);
            break;

        case 15:
            strcpy((char *)LTS.ucSessionExpiryTime,(char *)gSendData);
 //           printf("LTS.ucSessionExpiryTime == %s\n",LTS.ucSessionExpiryTime);
            break;

        case 16:
            strcpy((char *)LTS.ucSessionId,(char *)gSendData);
  //          printf("LTS.ucSessionId == %s\n",LTS.ucSessionId);
            break;

        case 17:
            strcpy((char *)LTS.ucRouteID,(char *)gSendData);
 //           printf("LTS.ucRouteID == %s\n",LTS.ucRouteID);
            break;

        case 18:
            strcpy((char *)LTS.ucErrorCode,(char *)gSendData);
 //           printf("LTS.ucErrorCode == %s\n",LTS.ucErrorCode);
            break;

        case 19:
            strcpy((char *)LTS.ucXMLData,(char *)gSendData);
  //          printf("LTS.ucXMLData == %s\n",LTS.ucXMLData);
            break;

        case 20:
            strcpy((char *)LTS.ucAuthmanFlag,(char *)gSendData);
 //           printf("LTS.ucAuthmanFlag == %s\n",LTS.ucAuthmanFlag);
            break;

        case 21:
            strcpy((char *)LTS.ucServiceID,(char *)gSendData);
  //          printf("LTS.ucServiceID == %s\n",LTS.ucServiceID);
            break;

        default:
            break;
        }
    }
    // ttestall(0,200);
    qDebug()<<"iParse LTS End";
    memset(gSendData,0,sizeof(gSendData));
    return FUNC_OK;
}
