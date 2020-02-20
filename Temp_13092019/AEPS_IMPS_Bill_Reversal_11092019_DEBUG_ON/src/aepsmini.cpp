#include "aepsmini.h"
#include "ui_FPB.h"
#include "json.h"
#include "FPB.h"
#include "Login.h"
#include "displaymessage.h"
#include "print.h"
extern Login_leg2 configLoginLeg2;
extern Login_leg1 configLoginLeg1;
extern RD_SERVICE_STRUCTURE RD_Service_data;
extern Transaction Txn;
extern Printfpb ConfigPrint;
AEPS_MINISTATEMENT AEPSMini_State;
StrAEPS_MINI dsply_data[15];

void FPB::on_MINIST_pushButton_5_clicked()
{
    memset(&AEPSMini_State,0,sizeof(AEPSMini_State));
    Check_Conditions();
    memset(&Txn,0,sizeof(Txn)); // clearing Transaction Structure
    Txn.Txntype = AEPS_MINI;


    ui->lineEditUID1->setReadOnly(false);
    ui->lineEditUID1->setFocus();
    ui->lineEditUID2->setReadOnly(false);
    ui->lineEditUID3->setReadOnly(false);
    ui->PAGE4_OK_pushButton_8->setEnabled(false);
    ui->PAGE4_OK_pushButton_8->setStyleSheet("background-color: rgbrgb(85, 85, 127); color: rgb(107, 107, 107)");

    ui->stackedWidget->setCurrentIndex(7);
}


int FPB::Prepare_AEPS_MINISTAT_Func()
{

    HostComm hComm;
    CMisc miscObj;
    int MINI_len = 0, response_timeout1 = 120, ret = -3, res = -3, iRet = -3;
    char MINI_Buff[10000];
    char ErrorMsg[100];
    int template_len=0;
    FILE *fp1;
    unsigned char  *template_data;
    memset(ErrorMsg,0,sizeof(ErrorMsg));
    memset(AEPSMini_State.ucSessionId_AMinireq,0, sizeof(AEPSMini_State.ucSessionId_AMinireq));
    memset(AEPSMini_State.ucSessionExpiryTime_AMinireq, 0, sizeof(AEPSMini_State.ucSessionExpiryTime_AMinireq));
    memset(AEPSMini_State.ucMethodId_AMinireq, 0, sizeof(AEPSMini_State.ucMethodId_AMinireq));
    memset(AEPSMini_State.ucTellerID_AMinireq, 0, sizeof(AEPSMini_State.ucTellerID_AMinireq));
    memset(AEPSMini_State.ucIsEncrypt_AMinireq,0, sizeof(AEPSMini_State.ucIsEncrypt_AMinireq));
    memset(AEPSMini_State.ucChannelID_AMinireq,0, sizeof(AEPSMini_State.ucChannelID_AMinireq));
    memset(AEPSMini_State.ucRouteID_AMinireq, 0, sizeof(AEPSMini_State.ucRouteID_AMinireq));
    memset(AEPSMini_State.ucServiceID_AMinireq, 0, sizeof(AEPSMini_State.ucServiceID_AMinireq));
    memset(AEPSMini_State.uccard_acceptor_namelocation_AMinireq, 0, sizeof(AEPSMini_State.uccard_acceptor_namelocation_AMinireq));

    memcpy((char *)AEPSMini_State.ucUID_AMinireq , Txn.uid,12);
    memcpy((char *)AEPSMini_State.ucAadharNo_AMinireq,Txn.bin,strlen((char *)Txn.bin));
    strcat((char *)AEPSMini_State.ucAadharNo_AMinireq,"0");
    strcat((char *)AEPSMini_State.ucAadharNo_AMinireq,(char *)AEPSMini_State.ucUID_AMinireq);
    memcpy((char *)AEPSMini_State.ucSessionId_AMinireq,configLoginLeg2.ucSessionId,strlen((char *)configLoginLeg2.ucSessionId));
    memcpy((char *)AEPSMini_State.ucSessionExpiryTime_AMinireq,configLoginLeg2.ucSessionExpiryTime,strlen((char *)configLoginLeg2.ucSessionExpiryTime));
    memcpy((char *)AEPSMini_State.ucmerMobileNo_AMinireq,configLoginLeg2.ucmobileNumber,10);
//    sprintf((char *)AEPSMini_State.uccard_acceptor_namelocation_AMinireq,"%s %s %s%s",configLoginLeg2.ucaddressLine1,configLoginLeg2.ucaddressLine2,configLoginLeg2.uccity,configLoginLeg2.uccountry);
//    sprintf((char *)AEPSMini_State.uccard_acceptor_namelocation_AMinireq,"%s","Ground Floor, Shop No. 8/9/10");
    sprintf((char *)AEPSMini_State.uccard_acceptor_namelocation_AMinireq,"%s %s%s",configLoginLeg2.uccity,configLoginLeg2.ucstate,configLoginLeg2.uccountry);
    printf("AEPSMini_State.uccard_acceptor_namelocation_AMinireq = %s\n",AEPSMini_State.uccard_acceptor_namelocation_AMinireq);
    memcpy(AEPSMini_State.ucMethodId_AMinireq,"520",3);
    memcpy(AEPSMini_State.ucTellerID_AMinireq,configLoginLeg2.ucUsername_req,strlen((char *)configLoginLeg2.ucUsername_req));
    memcpy(AEPSMini_State.ucIsEncrypt_AMinireq,"false",5);
    memcpy(AEPSMini_State.ucChannelID_AMinireq,"2",1);
    memcpy(AEPSMini_State.ucRouteID_AMinireq,"Request From: UIService To PBSAEPSProcessRequest",48);
    memcpy(AEPSMini_State.ucServiceID_AMinireq,"18",2);
    iGenerateRequestID((char *)configLoginLeg2.ucUsername_req,(unsigned char *)&AEPSMini_State.ucRequestId_AMinireq);


//    uidmessage::instance()->hide_Message();
//    uidmessage::instance()->DisplayMessage("Please Wait...");

        ret	= makeJSONAEPSMini();/*-------------Make REQUEST--------------*/
        if(ret!=FUNC_OK)
        {
            qDebug()<<"Ministatement JSON Creation failed";
            uidmessage::instance()->hide_Message();
            return FUNC_ERROR;
        }

        hComm.close_connection();
        res = hComm.open_connect((char *)SERVERIP,(char *)SERVERPORT);

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
        memset(MINI_Buff,0,sizeof(MINI_Buff));
        if((hComm.host_2Sendrecv((unsigned char *)gSendData,len_gSendData,( unsigned char*)MINI_Buff,(int *)&MINI_len,(int)response_timeout1,0))!=SUCCESS)
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

        ret=json->iParsJSON((unsigned char*)MINI_Buff,(char *)&AEPSMini_State.ucResponseCode,(char *)&AEPSMini_State.ucResponseMessage);

        if(ret!=FUNC_OK)
        {
            sprintf(ErrorMsg,"%s",(char *)AEPSMini_State.ucResponseMessage);
            miscObj.DisplayMsgBox(ErrorMsg);
            uidmessage::instance()->hide_Message();
            return FUNCTION_FAILED;
        }


        ////////////////////////////////////////////
        if (!memcmp(AEPSMini_State.ucResponseCode,"0",1))
        {

            system("cd /usr/FINO/FPB/REQ/");
            fp1 = fopen("/usr/FINO/FPB/REQ/IntJSON.TXT", "rb"); // open
            if (fp1 == NULL)
            {
                qDebug()<<"Can't Open IntJSON.TXT File";
                uidmessage::instance()->hide_Message();
                //return FUNCTION_FAILED;
                return FUNC_ERROR;
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

            AEPSMini_State.iCount = json->iTagCount((unsigned char *)Recvdata, "ModeOfTxn");
            qDebug()<<"Before Mini-Statement Parsing";
            iRet = iParseAEPS_MINIStatement((unsigned char*)template_data,(char *)&AEPSMini_State.ucResponseCode,(char *)&AEPSMini_State.ucResponseMessage,(int *)AEPSMini_State.iCount);

            free(template_data);
            if (iRet == FUNC_OK)
            {
                uidmessage::instance()->hide_Message();
                uidmessage::instance()->Display_BusyMessage("Mini Statement Successful.");
                sleep(2);
                uidmessage::instance()->hide_Message();

            }
            else
            {
                qDebug()<<"Inside Pardse  error";
                miscObj.DisplayMsgBox((char *)"Mini Statement Failed");
                uidmessage::instance()->hide_Message();
                return FUNCTION_FAILED;
            }

            //memcpy((char *)configLoginLeg2.ucMerchantLimit,(char *)AEPSMini_State.ucavailableBalance,strlen((char *)AEPSMini_State.ucavailableBalance));
            memcpy((char *)ConfigPrint.ucSTAN,&AEPSMini_State.ucRRN[6],6);
            memcpy((char *)ConfigPrint.ucTranStatus,"Success(0)",10);
            memcpy((char *)ConfigPrint.ucBCName,configLoginLeg2.ucname,15);
            memcpy((char *)ConfigPrint.ucBCLocation,AEPSMini_State.ucnameLocation,strlen((char *)AEPSMini_State.ucnameLocation));
            memcpy((char *)ConfigPrint.ucAgentID,configLoginLeg2.ucUsername_req,6);
            memcpy((char *)ConfigPrint.ucTerminalID,AEPSMini_State.ucTerminalId,strlen((char *)AEPSMini_State.ucTerminalId));
            memcpy((char *)ConfigPrint.ucAUTHCode,AEPSMini_State.ucAuthCode,strlen((char *)AEPSMini_State.ucAuthCode));
            memcpy((char *)ConfigPrint.ucRRN,AEPSMini_State.ucRRN,strlen((char *)AEPSMini_State.ucRRN));
            sprintf((char *)ConfigPrint.ucCustAadharNo,"********%s",&AEPSMini_State.ucUID_AMinireq[8]);
            sprintf((char *)ConfigPrint.ucACBalance,"Rs. %s",AEPSMini_State.ucBalance);

            if(print_aes_transaction()== FUNC_OK)
            {
                qDebug()<<" Print SUCCESS";
            }
            else
            {
                qDebug()<<" Print FAILED";
                miscObj.DisplayMsgBox((char *)"Print Slip Failed");
                sleep(2);
                uidmessage::instance()->hide_Message();
                //  return FUNCTION_FAILED;
            }
            //            return (FUNC_ERROR);
        }
        else
        {
            uidmessage::instance()->hide_Message();
            uidmessage::instance()->Display_BusyMessage("MIni Statement Failed");
            sleep(2);
            uidmessage::instance()->hide_Message();
            Txn.Txntype = 0;
            ui->stackedWidget->setCurrentIndex(2);
            return FUNCTION_FAILED;
        }


        memset(&AEPSMini_State,0,sizeof(AEPSMini_State));
        return FUNC_OK;
}

int FPB::makeJSONAEPSMini()
{
    memset(gSendData,0,sizeof(gSendData));
    char tempbuff[30];
    int slen=-2,iRet=-2;

    iRet = iMakeRequest(8);
    if (iRet!=FUNC_OK)
    {
        uidmessage::instance()->hide_Message();
        return FUNC_ERROR;
    }

    slen=strlen((char *)Recvdata);
    slen-=2;
    memset(gSendData,0,sizeof(gSendData));//PBSAEPSProcessRequest
    strncpy((char *)gSendData,"POST /UIService.svc/AEPSProcessRequest HTTP/1.1\r\n",52);
    strncat((char *)gSendData,"Host: 202.58.125.20\r\n",27);//9013
    strncat((char *)gSendData,"Content-Type: application/json; charset=utf-8\r\n",49);
    sprintf(tempbuff,"Content-Length: %d\r\n",slen);
    strncat((char *)gSendData,tempbuff,strlen(tempbuff));

    strcat((char *)gSendData,(char *)Recvdata);
#if DEBUG
    system("cd /usr/FINO/FPB/REQ/");
    system("rm /usr/FINO/FPB/REQ/MINI_req.TXT");
    FILE *fp = NULL;
    fp =fopen("/usr/FINO/FPB/REQ/MINI_req.TXT","a");
    fwrite(gSendData,strlen((char *)gSendData),1,fp);
    //ttestall(0,100);
    sleep(1);
    fclose(fp);
#endif
    return FUNC_OK;
}


int FPB::iSetAEPSMiniState(int tagID, unsigned char *value)
{
    switch(tagID)
    {
    case  1:
        strcpy((char *)value,(char *)AEPSMini_State.ucRequestId_AMinireq);
        break;
    case  2:
        strcpy((char *)value,"519");//Method id
        break;
    case  3:
        strcpy((char *)value,(char *)AEPSMini_State.ucTellerID_AMinireq);//teller id
        break;
    case  4:
        strcpy((char *)value,(char *)AEPSMini_State.ucSessionId_AMinireq);//session key
        break;
    case  5:
        strcpy((char *)value,"");//token id
        break;
    case  6:
        strcpy((char *)value,"false"/*(char *)AEPSMini_State.ucIsEncrypt_BEreq*/);//is encrypt
        break;
    case  7:
        strcpy((char *)value,"0"/*(char *)AEPSMini_State.ucAuth_id_BEreq*/); //Auth Id
        break;
    case  8:
        strcpy((char *)value,(char *)AEPSMini_State.ucAadharNo_AMinireq); //BankBin+adhaar no
        break;
    case  9:
        strcpy((char *)value,(char *)AEPSMini_State.ucmerMobileNo_AMinireq);
        break;
    case  10:
        strcpy((char *)value,(char *)AEPSMini_State.ucUID_AMinireq);
        break;
    case  11:
        strcpy((char *)value,"AUTH"/*(char *)AEPSMini_State.ucAuthType_BEreq*/);
        break;
    case  12:
        strcpy((char *)value,(char *)RD_Service_data.PID_DATA.toAscii().constData());
        break;
    case  13:
        strcpy((char *)value,(char *)RD_Service_data.hmac.toAscii().constData());
        break;
    case  14:
        strcpy((char *)value,(char *)EncPIDData);
        break;
    case  15:
        strcpy((char *)value,"Y"/*(char *)RD_fun.ucRC*/);
        break;
    case  16:
        strcpy((char *)value,"851204"/*(char *)AEPSMini_State.ucMeta_lov_BEreq*/);
        break;
    case  17:
        strcpy((char *)value,"X"/*(char *)AEPSMini_State.ucData_type_BEreq*/);
        break;
    case  18:
        strcpy((char *)value,"0000"/*(char *)AEPSMini_State.ucamount_BEreq*/);
        break;
    case  19:
        strcpy((char *)value,"OBU60029078"/*(char *)AEPSMini_State.uccard_acceptor_ID_BEreq*/);
        break;
    case  20:
        strcpy((char *)value,(char *)AEPSMini_State.uccard_acceptor_namelocation_AMinireq);
        break;
    case  21:
        strcpy((char *)value,"80000001"/*(char *)AEPSMini_State.ucTerminal_Id_BEreq*/);
        break;
    case  22:
        strcpy((char *)value,""/*(char *)AEPSMini_State.ucTransactionTime_BEreq*/);
        break;
    case  23:
        strcpy((char *)value,""/*(char *)AEPSMini_State.ucTransactionDate_BEreq*/);
        break;
    case  24:
        strcpy((char *)value,"FINOMER"/*(char *)AEPSMini_State.ucAppId_BEreq*/);
        break;
    case  25:
        strcpy((char *)value,"Y"/*(char *)AEPSMini_State.ucNewAPI_Flag_BEreq*/);
        break;
    case  26:
        strcpy((char *)value,"2.0"/*(char *)AEPSMini_State.ucAPI_Version_BEreq*/);
        break;
    case  27:
        strcpy((char *)value,(char *)AEPSMini_State.ucSessionExpiryTime_AMinireq);
        break;
    case  28:
        strcpy((char *)value,"2"/*(char *)AEPSMini_State.ucChannelID_BEreq*/);
        break;
    case  29:
        strcpy((char *)value,"18"/*(char *)AEPSMini_State.ucServiceID_BEreq*/);
        break;
    case  30:
        strcpy((char *)value,(char *)RD_Service_data.skey.toAscii().constData());
        break;
    case  31:
        strcpy((char *)value,"");
        break;
    case  32:
       // strcpy((char *)value,(char *)RD_Service_data.ucudc);//check this
        break;
    case  33:
        strcpy((char *)value,(char *)RD_Service_data.ci.toAscii().constData());
        break;
    case  34:
        strcpy((char *)value,(char *)RD_Service_data.skey.toAscii().constData());
        break;
    case  35:
        strcpy((char *)value,(char *)RD_Service_data.PID_DATA.toAscii().constData());
        break;
    case  36:
      //  strcpy((char *)value,(char *)RD_Service_data.mc);//check this
        break;
    case  37:
        strcpy((char *)value,(char *)RD_Service_data.dpld.toAscii().constData());
        break;
    case  38:
        strcpy((char *)value,(char *)RD_Service_data.rdsld.toAscii().constData());
        break;
    case  39:
        strcpy((char *)value,(char *)RD_Service_data.rdsver.toAscii().constData());
        break;
    case  40:
        strcpy((char *)value,(char *)RD_Service_data.dc.toAscii().constData());
        break;
    case  41:
        strcpy((char *)value,(char *)RD_Service_data.mi.toAscii().constData());
        break;
    case  42:
        strcpy((char *)value,"");
        break;
    default:
        return FUNC_ERROR;
    }
    return FUNC_OK;
}


int FPB::iParseAEPS_MINIStatement(unsigned char *Data,char *RespC,char *RespMes,int *iTagCount)
{
    int len,i=0,slen,len2=-1;
    int j=0;
    char *RecvJsonData;
    char *CheckData;

    //char gSendData[1024];

    char *AEPSMINI_Tags[]={"RequestId","DisplayMessage","ResponseMessage","ResponseCode","MessageType","MessageId","SessionExpiryTime","SessionId","RouteID",
            "ErrorCode","XMLData","AuthmanFlag","ServiceID","returnCode","localDate","localTime","RRN","UIDAuthCode",
            "nameLocation","TerminalId","Balance"	};

    char *AEPSMINITags[] = {
            "Date","ModeOfTxn","Type","RefNo","DebitCredit","Amount"};

    int MandTag[]={1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1};//1 for mandatory and  0 for non-mandatory by saurabh

    json->iIsJsonValid((char *)Data,RespC,RespMes,0);
    if (memcmp(RespC,"0",1)!=0)
    {
        return FUNC_ERROR;
    }

    len=sizeof(AEPSMINI_Tags)/sizeof(AEPSMINI_Tags[0]);
    len2=sizeof(MandTag)/sizeof(MandTag[0]);
    for (i = 0; i < len; ++i)
    {
        RecvJsonData=strstr((char *)Data,AEPSMINI_Tags[i]);
        slen=strlen(AEPSMINI_Tags[i]);

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

        RecvJsonData=strchr(RecvJsonData,':');
        RecvJsonData=RecvJsonData+2;
        if (*RecvJsonData == '\"')
            RecvJsonData++;

        CheckData=RecvJsonData;
        do {
            CheckData++;
        } while ((*CheckData!=',') && (*CheckData!='}') && (*CheckData!='\\') && (*CheckData!='\"'));

        memset(gSendData,0,sizeof(gSendData));
        strncpy((char *)gSendData,RecvJsonData,CheckData-RecvJsonData);

        switch (i)
        {
        case 0:
            strcpy((char *)AEPSMini_State.ucRequestId,(char *)gSendData);
            break;

        case 1:
            strcpy((char *)AEPSMini_State.ucDisplayMessage,(char *)gSendData);
            break;

        case 2:
            strcpy((char *)AEPSMini_State.ucResponseMessage,(char *)gSendData);
            break;

        case 3:
            strcpy((char *)AEPSMini_State.ucResponseCode,(char *)gSendData);
            break;

        case 4:
            strcpy((char *)AEPSMini_State.ucMessageType,(char *)gSendData);
            break;

        case 5:
            strcpy((char *)AEPSMini_State.ucMessageId,(char *)gSendData);
            break;

        case 6:
            strcpy((char *)AEPSMini_State.ucSessionExpiryTime,(char *)gSendData);
            break;

        case 7:
            strcpy((char *)AEPSMini_State.ucSessionId,(char *)gSendData);
            break;

        case 8:
            strcpy((char *)AEPSMini_State.ucRouteID,(char *)gSendData);
            break;

        case 9:
            strcpy((char *)AEPSMini_State.ucErrorCode,(char *)gSendData);
            break;

        case 10:
            strcpy((char *)AEPSMini_State.ucXMLData,(char *)gSendData);
            break;

        case 11:
            strcpy((char *)AEPSMini_State.ucAuthmanFlag,(char *)gSendData);
            break;

        case 12:
            strcpy((char *)AEPSMini_State.ucServiceID,(char *)gSendData);
            break;

        case 13:
            strcpy((char *)AEPSMini_State.ucreturnCode,(char *)gSendData);
            break;

        case 14:
            strcpy((char *)AEPSMini_State.uclocalDate,(char *)gSendData);
            break;

        case 15:
            strcpy((char *)AEPSMini_State.uclocalTime,(char *)gSendData);
            break;

        case 16:
            strcpy((char *)AEPSMini_State.ucRRN,(char *)gSendData);
            break;

        case 17:
            strcpy((char *)AEPSMini_State.ucAuthCode,(char *)gSendData);

            break;

        case 18:
            strcpy((char *)AEPSMini_State.ucnameLocation,(char *)gSendData);
            break;

        case 19:
            strcpy((char *)AEPSMini_State.ucTerminalId,(char *)gSendData);
            break;

        case 20:
            strcpy((char *)AEPSMini_State.ucBalance,(char *)gSendData);
            break;


        default:
            break;
        }
    }

    RecvJsonData=strstr((char *)Data,"TransactionList");
    for (i = 0; (int *)i < iTagCount; ++i)
    {
        for (j = 0; j < 6; ++j)
        {
            RecvJsonData=strstr((char *)RecvJsonData,AEPSMINITags[j]);
            slen=strlen((char *)AEPSMINITags[j]);

            if (RecvJsonData==NULL)
            {
               // DrawString("TAG Missing",15,25,0);
               // ttestall(0,200);
                memset(gSendData,0,sizeof(gSendData));
                return FUNC_ERROR;
            }
            memset(gSendData,0,sizeof(gSendData));

            RecvJsonData=strchr(RecvJsonData,':');
            RecvJsonData=RecvJsonData+2;
            if (*RecvJsonData == '\"')
                RecvJsonData++;
            else
                RecvJsonData--;

            CheckData=RecvJsonData;
            do {
                CheckData++;
            } while ((*CheckData!=',') && (*CheckData!='}'));

            CheckData--;
            if (*CheckData == '\"')
                CheckData--;
            else
                CheckData++;

            memset(gSendData,0,sizeof(gSendData));
            strncpy((char *)gSendData,RecvJsonData,CheckData-RecvJsonData);

            switch (j)
            {
            case 0:
                strcpy((char *)dsply_data[i].AEPS_MINI_STAT.Date,(char *)gSendData);
                break;

            case 1:
                strcpy((char *)dsply_data[i].AEPS_MINI_STAT.ModeOfTxn,(char *)gSendData);
                break;

            case 2:
                strcpy((char *)dsply_data[i].AEPS_MINI_STAT.Type,(char *)gSendData);
                break;

            case 3:
                strcpy((char *)dsply_data[i].AEPS_MINI_STAT.RefNo,(char *)gSendData);
                break;

            case 4:
                strcpy((char *)dsply_data[i].AEPS_MINI_STAT.DebitCredit,(char *)gSendData);
                break;

            case 5:
                strcpy((char *)dsply_data[i].AEPS_MINI_STAT.Amount,(char *)gSendData);
                break;

            default:
                break;
            }
        }
    }
    dsply_data[i].next  = &dsply_data[i+1];
    i++;
    dsply_data[i].next = NULL;
    memset(gSendData,0,sizeof(gSendData));
    return FUNC_OK;
}



