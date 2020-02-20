#include "FPB.h"
#include "ui_FPB.h"
#include "bankmaster.h"
#include "Login.h"
#include "json.h"
#include "displaymessage.h"
extern unsigned char iFirstBMTag[5];
extern unsigned char iLastBMTag[10];
//extern unsigned char  BM_Recvdata[1024*45];
extern unsigned char  BM_Recvdata[1024*45]; // Temp BM
char iFinalBank[50]={0};
extern Login_leg2 configLoginLeg2;
AEPS_BANKMAST AEPSBM;
extern char * Convertstr(string str);



int FPB::iBankMaster()
{
    char* jsondata;
    int ret = 0, iRet = 0;
    Json1::Value mainreq;
    char buff1[100];
    memset(buff1,0,sizeof(buff1));
    iGenerateRequestID((char *)username,(unsigned char *)buff1);

    mainreq["RequestId"] =  buff1;
    mainreq["MethodId"] = MID_BANKMASTER;
    mainreq["TellerID"]= username;
    mainreq["SessionId"] = (char *)configLoginLeg2.ucSessionId;
    mainreq["TokenId"]= "";
    mainreq["IsEncrypt"]= false;
    mainreq["X_Auth_Token"] ="";
    mainreq["SessionExpiryTime"]= (char *)configLoginLeg2.ucSessionExpiryTime;//"10/11/2018 11:58:44 PM";
    // mainreq["ChannelID"]= CHANNEL_ID;
    mainreq["RequestData"]= "IFSCMappingData";

    jsondata = Convertstr(mainreq.toStyledString());

#if DEBUG
    qDebug()<<"------------->"<<jsondata;
#endif
    memset(gSendData,0,sizeof(gSendData));
    strncpy((char*)gSendData, jsondata, strlen(jsondata));

    memset(str_url,0,sizeof(str_url));
#ifdef PRODUCTION
    strcpy(str_url,URL_CREATION(AEPS_BANKMASTER_URL_PROD));
#else
    strcpy(str_url,URL_CREATION((char *)AEPS_BANKMASTER_URL));
#endif


    ret = NetworkConnect(0,str_url, 2);
    if(ret != FUNCTION_SUCCESS)
    {
        uidmessage::instance()->hide_Message();
        if(ret == RESPONSE_TIMEOUT)
        {
            qDebug()<<"Response Timeout";
        }
        return FUNCTION_FAILED;
    }

    iRet = Bank_Master_Parsing();
    return iRet;

}

int FPB::Bank_Master_Parsing()
{
    char cResCode[10+1];
    int isize = 0;
    CMisc miscObj;
    QSqlQuery query;
    system("sed 's/\"ResponseData\":\"{/\"ResponseData\":{/g' /usr/FINO/FPB/RESP/OUTPUT.TXT > /tmp/1.txt");
    system("sed 's/}\",\"MessageId\"/},\"MessageId\"/g' /tmp/1.txt > /tmp/2.txt  ");
    system(" sed 's/\\\\//g' /tmp/2.txt > /usr/FINO/FPB/RESP/OUTPUT.TXT");

    Json1::Value root;
    root.clear();
    Json1::Reader reader;
    std::ifstream ReadData("/usr/FINO/FPB/RESP/OUTPUT.TXT");
    bool parsedSuccess = reader.parse(ReadData,root,false);

    ReadData.close();

    if(not parsedSuccess)
    {
        qDebug()<<"File not Found";
        return FUNCTION_FAILED;
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
        memset(cResCode, 0, sizeof(cResCode));
        strcpy(cResCode,Convertstr(root["ResponseCode"].asString()));
        if(memcmp(cResCode,"0",1)==0)
        {
            Json1::Value ResponseData;
            ResponseData.clear();
            ResponseData = root.get("ResponseData",0);
            isize = ResponseData.size();
            qDebug()<<"isize"<<isize;
            printf("\nSize : %d\n",isize);
            if(isize==0)
            {
                qDebug("\n1:Key Register Data Not Found\n");
                return FUNCTION_FAILED;        }
            else
            {
                Json1::Value iCacheMaster;
                iCacheMaster.clear();
                iCacheMaster = ResponseData.get("CacheMaster",0);

                //********************************************      DELETE QUERY     ************************************************

                query.clear();
                query.exec("DELETE from imps_mapping");

                //*******************************************************************************************************************

                query.clear();

                for(int j=0;j<iCacheMaster.size();j++)
                {
#if DEBUG
                    qDebug()<<"IFSC             : "<<Convertstr(iCacheMaster[j]["IFSC"].asString());
                    qDebug()<<"ImpsStatus       : "<<Convertstr(iCacheMaster[j]["ImpsStatus"].asString());
                    qDebug()<<"BankName         : "<<Convertstr(iCacheMaster[j]["BankName"].asString());
                    qDebug()<<"IsActive         : "<<Convertstr(iCacheMaster[j]["IsActive"].asString());
                    qDebug()<<"VerifyStatus     : "<<Convertstr(iCacheMaster[j]["VerifyStatus"].asString());
                    qDebug()<<"DefaultIIFSCCode : "<<Convertstr(iCacheMaster[j]["DefaultIIFSCCode"].asString());
                    qDebug()<<"AEPSStatus       : "<<Convertstr(iCacheMaster[j]["AEPSStatus"].asString());
                    qDebug()<<"NBIN             : "<<Convertstr(iCacheMaster[j]["NBIN"].asString());
                    qDebug()<<"NBINStatus       : "<<Convertstr(iCacheMaster[j]["NBINStatus"].asString());
                    qDebug()<<"DefaultIFSCStatus: "<<Convertstr(iCacheMaster[j]["DefaultIFSCStatus"].asString());
                    qDebug()<<"IMPSNBIN         : "<<Convertstr(iCacheMaster[j]["IMPSNBIN"].asString());
                    qDebug()<<"AEPSbankOrder    : "<<Convertstr(iCacheMaster[j]["AEPSbankOrder"].asString());
                    qDebug()<<"AEPSIcon         : "<<Convertstr(iCacheMaster[j]["AEPSIcon"].asString());
                    qDebug()<<"-------------------------------------------------------------------------------";

#endif
                    query.clear();
                    query.prepare("INSERT INTO imps_mapping (IFSC, IMPS_STATUS, BANK_NAME, IS_ACTIVE, VERIFY_STATUS, DEFAULT_IIFSCCODE, AEPSSTATUS, NBIN, NBINSTATUS, DEFAULT_IFSCSTATUS, AEPSBANK_ORDER, AEPSICON) values (:IFSC,:ImpsStatus,:BankName,:IsActive,:VerifyStatus,:DefaultIIFSCCode,:AEPSStatus,:NBIN,:NBINStatus,:DefaultIFSCStatus,:AEPSBank_Order,:AEPSIcon)");
                    query.bindValue(":IFSC",Convertstr(iCacheMaster[j]["IFSC"].asString()));
                    query.bindValue(":ImpsStatus",Convertstr(iCacheMaster[j]["ImpsStatus"].asString()));
                    query.bindValue(":BankName",Convertstr(iCacheMaster[j]["BankName"].asString()));
                    query.bindValue(":IsActive",Convertstr(iCacheMaster[j]["IsActive"].asString()));
                    query.bindValue(":VerifyStatus",Convertstr(iCacheMaster[j]["VerifyStatus"].asString()));
                    query.bindValue(":DefaultIIFSCCode",Convertstr(iCacheMaster[j]["DefaultIIFSCCode"].asString()));
                    query.bindValue(":AEPSStatus",Convertstr(iCacheMaster[j]["AEPSStatus"].asString()));
                    query.bindValue(":NBIN",Convertstr(iCacheMaster[j]["NBIN"].asString()));
                    query.bindValue(":NBINStatus",Convertstr(iCacheMaster[j]["NBINStatus"].asString()));
                    query.bindValue(":DefaultIFSCStatus",Convertstr(iCacheMaster[j]["DefaultIFSCStatus"].asString()));
                    query.bindValue(":AEPSBank_Order",Convertstr(iCacheMaster[j]["AEPSbankOrder"].asString()));
                    query.bindValue(":AEPSIcon",Convertstr(iCacheMaster[j]["AEPSIcon"].asString()));


                    if(!query.exec())
                    {
                        qDebug() << "In Insert_BM_Txn_DB = " << query.lastError();
                        query.clear();
                        query.finish();
                        return FUNC_ERROR;
                    }
                    query.clear();
                }
                query.finish();
            }
        }
        else
        {
            CMisc miscObj;
            uidmessage::instance()->hide_Message();

            if(strcmp(Convertstr(root["ResponseMessage"].asString()),"")!=0)
            {
                char ErrorMsg[250];
                memset(ErrorMsg, 0, sizeof(ErrorMsg));
                sprintf(ErrorMsg,"%s",(char *)Convertstr(root["ResponseMessage"].asString()));
                miscObj.DisplayMsgBox(ErrorMsg);
                if(strcmp((char *)Convertstr(root["ResponseMessage"].asString()),"Session Expired.")==0)
                {
                    uidmessage::instance()->hide_Message();
                    return SESSION_EXPIRED_FAILED;
                }
            }
            uidmessage::instance()->hide_Message();
            return FUNCTION_FAILED;
        }
    }

    return FUNC_OK;
}



void FPB::on_BankMaster_clicked()
{
    int ibret = iBankMaster();
    if((ibret != FUNC_OK) && (ibret != FUNCTION_SUCCESS))
    {
        if(ibret == SESSION_EXPIRED_FAILED)
        {
            ui->stackedWidget->setCurrentIndex(0);
            return;
        }
        ui->stackedWidget->setCurrentIndex(15);
        return;
    }
    return;
}


int FPB::makeJSONAEPSBANKMAST()
{
    char tempbuff[30];
    memset(gSendData,0,sizeof(gSendData));
    memset(tempbuff,0,sizeof(tempbuff));
    int slen=-2,iRet=-2;
    int ipbm = 7;
    iRet = iMakeRequest(ipbm);
    if (iRet!=FUNC_OK)
    {
        qDebug()<<"iMakeRequest Func error";
        return FUNC_ERROR;
    }

    slen=strlen((char *)Recvdata); // Temp BM
    ////   slen=strlen((char *)BMRecvdata); // Temp BM
#if DEBUG
    qDebug()<<"slen = "<<slen;
#endif
    // slen=strlen((char *)BM_Recvdata);

    slen-=2;
    memset(gSendData,0,sizeof(gSendData));

#ifdef UAT_HTTP     // File Download
    strncpy((char *)gSendData,"POST /UIService.svc/GetMastersData HTTP/1.1\r\n",52);
    strncat((char *)gSendData,"Host: 202.58.125.20\r\n",27);//9013
    strncat((char *)gSendData,"Content-Type: application/json; charset=utf-8\r\n",49);
    sprintf(tempbuff,"Content-Length: %d\r\n",slen);
    strncat((char *)gSendData,tempbuff,strlen(tempbuff));
    strcat((char *)gSendData,(char *)BMRecvdata); // Temp BM
#else
    strncpy((char *)gSendData,(char *)Recvdata,strlen((char *)Recvdata));

    ////    strncpy((char *)gSendData,(char *)BMRecvdata,strlen((char *)BMRecvdata));
#endif
    //strcat((char *)gSendData,(char *)Recvdata); // Temp BM
#if DEBUG
    qDebug()<<"out of iMake ----------------2222222222222222222";
    system("cd /usr/FINO/FPB/REQ/");
    system("chmod 777 *");
    system("rm /usr/FINO/FPB/REQ/MASTER_req.TXT");
    FILE *fp = NULL;
    fp =fopen("/usr/FINO/FPB/REQ/MASTER_req.TXT","a");
    fwrite(gSendData,strlen((char *)gSendData),1,fp);
    //ttestall(0,100);
    sleep(1);
    fclose(fp);
    qDebug()<<"out of iMake ----------------333333333333333";
#endif
    return FUNC_OK;
}


int  FPB::iSetIMPSBANKMAST(int tagID, unsigned char *value)
{
    switch(tagID)
    {
    case  1:
        strcpy((char *)value,(char *)AEPSBM.ucRequestId_ABMreq);
        break;
    case  2:
        strcpy((char *)value,(char *)AEPSBM.ucMethodId_ABMreq);//Method id
        break;
    case  3:
        strcpy((char *)value,(char *)AEPSBM.ucTellerID_ABMreq);//teller id
        break;
    case  4:
        strcpy((char *)value,(char *)AEPSBM.ucSessionId_ABMreq);
        break;
    case  5:
        strcpy((char *)value,(char *)AEPSBM.ucIsEncrypt_ABMreq);
        break;
    case  6:
        strcpy((char *)value,(char *)AEPSBM.ucRequestData_ABMreq);
        break;
    case  7:
        strcpy((char *)value,(char *)AEPSBM.ucSessionExpiryTime_ABMreq);
        break;
    default:
        return FUNC_ERROR;
    }
    return FUNC_OK;
}

int  FPB::iSetAEPSBANKMAST(int tagID, unsigned char *value)
{
    switch(tagID)
    {
    case  1:
        strcpy((char *)value,(char *)AEPSBM.ucRequestId_ABMreq);
        break;
    case  2:
        strcpy((char *)value,(char *)AEPSBM.ucMethodId_ABMreq);//Method id
        break;
    case  3:
        strcpy((char *)value,(char *)AEPSBM.ucTellerID_ABMreq);//teller id
        break;
    case  4:
        strcpy((char *)value,(char *)AEPSBM.ucIsEncrypt_ABMreq);
        break;
    case  5:
        strcpy((char *)value,(char *)AEPSBM.ucRequestData_ABMreq);
        break;
    case  6:
        strcpy((char *)value,(char *)AEPSBM.ucChannelID_ABMreq);
        break;
    case  7:
        strcpy((char *)value,(char *)AEPSBM.ucServiceID_ABMreq);
        break;
    default:
        return FUNC_ERROR;
    }
    return FUNC_OK;
}


int FPB::iParseBank_Master( char *Data)
{

    int len,i=0,slen,len2=-1;
    //int j=0;
    char *RecvJsonData;
    char *CheckData;
    char buff[1024];
    QSqlQuery query;
    memset(&AEPSBM,'\0',sizeof(AEPSBM));
    char *cBankMasterTag[]={
        "RequestId","DisplayMessage","ResponseMessage","ResponseCode","MessageType","MessageId","SessionExpiryTime",
        "SessionId","RouteID","ErrorCode","XMLData","AuthmanFlag","ServiceID"
    };

    int MandTag[]={1,1,1,1,0,0,1,1,1,1,0,0,0,0,0};//1 for mandatory and  0 for non-mandatory by saurabh
    int ifOffset[]={3,2,3,3,3,3,5,2,2,2,3,2,2,2,2};
    int ibOffset[]=  {1,0,1,0,1,0,4,0,0,0,1,0,0,0,0};

    len=sizeof(cBankMasterTag)/sizeof(cBankMasterTag[0]);
    len2=sizeof(MandTag)/sizeof(MandTag[0]);
    for (i = 0; i < len; ++i)
    {
        RecvJsonData=strstr((char *)Data,cBankMasterTag[i]);
        slen=strlen(cBankMasterTag[i]);

        if (RecvJsonData==NULL && MandTag[i]==1)//If Tag not found and its mandatory.
        {
            //DrawString("TAG Missing",15,25,0);
            // ttestall(0,200);
            return FUNC_ERROR;
        }
        else if(RecvJsonData == NULL) //Else proceed to further parsing.
            continue;

        memset(buff,0,sizeof(buff));
        /*		RecvJsonData=&RecvJsonData[slen+ifOffset[i]];
        CheckData=RecvJsonData;
        do {
            CheckData++;
        } while ((*CheckData!=',') && (*CheckData!='}'));

        CheckData-=ibOffset[i];*/
        RecvJsonData=strchr(RecvJsonData,':');
        RecvJsonData=RecvJsonData+1;
        if (*RecvJsonData == '\"')
            RecvJsonData++;
        else
            ;//RecvJsonData--;

        CheckData=RecvJsonData;
        do {
            CheckData++;
        } while ((*CheckData!=',') && (*CheckData!='}') && (*CheckData!='\"'));

        memset(buff,0,sizeof(buff));
        strncpy(buff,RecvJsonData,CheckData-RecvJsonData);

        switch (i)
        {
        case 0:
            strcpy((char *)AEPSBM.ucRequestId,buff);
            break;

        case 1:
            strcpy((char *)AEPSBM.ucDisplayMessage,buff);
            break;

        case 2:
            strcpy((char *)AEPSBM.ucResponseMessage,buff);
            break;

        case 3:
            strcpy((char *)AEPSBM.ucResponseCode,buff);
            break;

        case 4:
            strcpy((char *)AEPSBM.ucMessageType,buff);
            break;

        case 5:
            strcpy((char *)AEPSBM.ucMessageId,buff);
            break;

        case 6:
            strcpy((char *)AEPSBM.ucSessionExpiryTime,buff);
            break;

        case 7:
            strcpy((char *)AEPSBM.ucSessionId,buff);
            break;

        case 8:
            strcpy((char *)AEPSBM.ucRouteID,buff);
            break;

        case 9:
            strcpy((char *)AEPSBM.ucErrorCode,buff);
            break;

        case 10:
            strcpy((char *)AEPSBM.ucXMLData,buff);
            break;

        case 11:
            strcpy((char *)AEPSBM.ucAuthmanFlag,buff);
            break;

        case 12:
            strcpy((char *)AEPSBM.ucServiceID,buff);
            break;

        default:
            break;
        }
    }
    query.clear();
    query.exec("delete from AEPSBankMaster");

    return FUNC_OK;
}


