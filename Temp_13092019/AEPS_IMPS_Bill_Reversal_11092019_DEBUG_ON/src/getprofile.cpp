#include "FPB.h"
#include "ui_FPB.h"
#include "displaymessage.h"
#include"getprofile.h"
#include "Login.h"
#include "json.h"
#include "displaymessage.h"
char oGPBuff[5010];
int response_timeout1 = 0;
//#include"CHeader.h"
//unsigned char gSendData[1024*30]; //Used to send data through network
//unsigned char Recvdata[1024*50]; //Used to receive
//char oBuff[5010];
//int response_timeout = 0;
char tempbuff[1000];
GProfile GETProfile;
//unsigned char Recvdata[1024*50];

extern Login_leg1 configLoginLeg1;
extern Login_leg2 configLoginLeg2;
extern unsigned char arr_GETProf[][25];

QT_BEGIN_INCLUDE_NAMESPACE
class FPB;
class Json;
QT_END_INCLUDE_NAMESPACE


//void FPB::on_GetProfile_pushbutton_clicked()
int FPB::DownloadAppSettings() // GET PROFILE TRANSACTION
{
    int ret = -2;//, key, bcountinue = 1;

    uidmessage::instance()->hide_Message();
    uidmessage::instance()->Display_BusyMessage("Downloading Application Setting,\nPlease Wait....");

    //****************************************************************************************************
    //memcpy(configLoginLeg2.ucUsername,"1414T9",6);
    iGenerateRequestID((char *)configLoginLeg2.ucUsername_req,(unsigned char *)&GETProfile.ucRequestID_GPReq);

    memcpy(GETProfile.ucSessionID_GPReq,configLoginLeg2.ucSessionId,sizeof(configLoginLeg2.ucSessionId));
    memcpy(GETProfile.ucMethodID_GPReq,"9",1);
    memcpy(GETProfile.ucTellerID_GPReq,"1",1);
    memcpy(GETProfile.ucIsEncrypt_GPReq,"false",5);
    memcpy(GETProfile.ucUserTypeID_GPReq,"190",3);


  //  memcpy(GETProfile.ucRDChannelID_GPReq,"1",1);
    memcpy(GETProfile.ucRDChannelID_GPReq,CHANNEL_ID,strlen(CHANNEL_ID));
    memcpy(GETProfile.ucChannelID_GPReq,"0",1);
  //  memcpy(GETProfile.ucChannelID_GPReq,CHANNEL_ID,strlen(CHANNEL_ID));
    memcpy(GETProfile.ucServiceID_GPReq,"4",1);

    ret = makeJSONReqGetProfile();
    if (ret != FUNC_OK)
    {
        qDebug()<<"Get Profile JSON Creation failed";
        return FUNC_ERROR;
        //return;
    }

//    /****************************Encryption function*********************************/
//    if(!strcmp((char *)GETProfile.ucIsEncrypt_GPReq,"true"))
//    {
//       int gSendData_len =strlen((char *)gSendData);
//        RequestDetachEncryptAttach((char *)gSendData,1,gSendData_len);
//        qDebug()<<"After encrypt function";
//    }
//    /***************************************************************/

#ifdef UAT_HTTP     // File Download

    hComm.close_connection();
    int res;
    res = hComm.open_connect(SERVERIP,SERVERPORT);
    if(res != 0)
    {
        qDebug() << "Failed to connect to the Server";
        uidmessage::instance()->hide_Message();
        uidmessage::instance()->DisplayMessage("Server Not Connected");
        return FUNC_ERROR;
        //return;
    }

    int oGPLen=0;
    int len_gSendData = strlen((char *)gSendData);
    memset(oGPBuff,0,sizeof(oGPBuff));
    if((hComm.host_2Sendrecv((unsigned char *)gSendData,len_gSendData,( unsigned char*)oGPBuff,(int *)&oGPLen,(int)response_timeout1,1))!=SUCCESS)
    {

        printf("No Response From Server\n");
        hComm.close_connection();

        miscObj.DisplayMsgBox((char *)"Response Time-Out");
        sleep(2);
        //            objComm.close_connection();
        qDebug()<<"***   Response Time-Out   ***";
        uidmessage::instance()->hide_Message();
        //return SOCKET_RESPONSE_FAILED;
        return FUNCTION_FAILED;

    }
    hComm.close_connection();
    qDebug()<<"******************************** GP Receive Success.. *****************************************";

//    if(iRet!=FUNC_OK)
//    {
//        qDebug()<<"Connection failed.";
//        //return FUNC_ERROR;
//        return;
//    }
//    system("cd /usr/FINO/FPB/REQ/");
//    system("rm /usr/FINO/FPB/REQ/GPABM.TXT");
//    int gRecv_len = strlen((char *)oGPBuff);
//    FILE *fp2 = NULL;
//    fp2 = fopen("/usr/FINO/FPB/REQ/GPABM.TXT","a");
//    qDebug()<<"fILE"<<fp2;
//    fwrite( oGPBuff, 1, gRecv_len, fp2 );
//    fclose(fp2);
    //************************  TO READ FILE  ****************************

    /*unsigned char *tempBuff1;
    tempBuff1=Recvdata;
    //fp=FS_open("/HOST/NEW_GETREQ.TXT","r");
    fp=FS_open("/HOST/PROFILE.TXT","r");
    ttestall(0,100);
    do {
        iRet=FS_read(tempBuff1++,1,1,fp);
    } while (iRet==1);
    FS_close(fp);*/

    //********************************************************************
    ret = json->iParseGP_ABM("/usr/FINO/FPB/REQ/GPABM.TXT",(char *)arr_GETProf[0],(char *)arr_GETProf[22],(int *)GETPROFILE);
    if (ret != FUNC_OK) {
      //  GL_Dialog_Message(graphicLib, "", "Invalid Data", GL_ICON_NONE,GL_BUTTON_NONE, 500);
      //  return FUNC_ERROR;
        return FUNCTION_FAILED;
    }

#else     // File Download full condition

#if DEBUG
    printf("gSendData = %s\n\n",gSendData);
#endif
    memset(str_url,0,sizeof(str_url));
    #ifdef PRODUCTION
        qDebug()<<"#ifdef PRODUCTION URL";
        strcpy(str_url,URL_CREATION(GETPROFILE_URL_PROD));
    #else
        qDebug()<<"#ifdef UAT URL";
        strcpy(str_url,URL_CREATION((char *)GETPROFILE_URL));
    #endif

        system("rm /usr/FINO/FPB/RESP/GPOUTPUT.TXT"); // Need to uncomment after testing 0606019

    int retk = file_download(str_url, (char *)gSendData,"/usr/FINO/FPB/RESP/GPOUTPUT.TXT",120); // Need to change file name 06062019
    if(retk!=0)
    {
        qDebug()<<"retk Password Page = "<<retk;
        sleep(1);
        uidmessage::instance()->hide_Message();
        return FUNCTION_FAILED;
    }

//    /****************************Decryption function*********************************/
//    if(!strcmp((char *)GETProfile.ucIsEncrypt_GPReq,"true"))
//    {
//        int oBuff_len = strlen(oBuff);
//#if DEBUG
//        qDebug()<<"Length of buffer"<<oBuff_len;
//        printf("Buffer%s\n",oBuff);
//#endif
//        ResponseDetachEncryptAttach(oBuff,2,oBuff_len);//TEMPORARY

//        system("rm /usr/FINO/FPB/RESP/GPOUTPUT.TXT");
//        FILE *fp2 = NULL;
//        fp2 =fopen("/usr/FINO/FPB/RESP/GPOUTPUT.TXT","a");
//        fwrite(oBuff,strlen((char *)oBuff),1,fp2);
//        sleep(1);
//        fclose(fp2);
//    }
//    /*******************************************************************************/

    ret = json->iParseGP_ABM("/usr/FINO/FPB/RESP/GPOUTPUT.TXT",(char *)arr_GETProf[0],(char *)arr_GETProf[22],(int *)GETPROFILE); // Need to change file name 06062019
    system("cd /usr/FINO/FPB/RESP/");
    system("rm /usr/FINO/FPB/RESP/OUTPUT.TXT");
    if (ret != FUNC_OK)
    {
        qDebug()<<"iParseGP_ABM Failed";
        return FUNCTION_FAILED;
    }

#endif        // File Download

    //return FUNC_OK;
    return FUNC_OK;
}


int FPB::makeJSONReqGetProfile(void)
{
    int iRet = -2, slen = -2;
    int c = 6;
    iRet = iMakeRequest(c);
    if (iRet!=FUNC_OK)
    {
        qDebug()<<"makeJSONReqGetProfile Error";
        return FUNC_ERROR;
    }

    slen=strlen((char *)Recvdata);
    slen-=2;
    memset(gSendData,0,sizeof(gSendData));
    memset(tempbuff,0,sizeof(tempbuff));

#ifdef UAT_HTTP
    strncpy((char *)gSendData,"POST /UIService.svc/GetMastersData HTTP/1.1\r\n",48);
    strncat((char *)gSendData,"Host: 202.58.125.2:4015\r\n",27); // 9013 or 4015
    strncat((char *)gSendData,"Content-Type: application/json; charset=utf-8\r\n",49);
    sprintf(tempbuff,"Content-Length: %d\r\n",slen);
    strncat((char *)gSendData,tempbuff,strlen(tempbuff));
    strcat((char *)gSendData,(char *)Recvdata);
#else
    strncpy((char *)gSendData,(char *)Recvdata,strlen((char *)Recvdata));
#endif

#if DEBUG
    qDebug()<<"out";
    system("cd /usr/FINO/FPB/REQ/");
    system("chmod 777 *");
    system("rm /usr/FINO/FPB/REQ/GPfile_REQ.TXT");
    FILE *fp = NULL;
    fp =fopen("/usr/FINO/FPB/REQ/GPfile_REQ.TXT","a");
    fwrite(gSendData,strlen((char *)gSendData),1,fp);
    //ttestall(0,100);
    sleep(1);
    fclose(fp);
#endif

    return FUNC_OK;
}





int FPB::iSetGet_Profile(int tagID, unsigned char *value)
{
   // qDebug()<<"tagID = "<<tagID;
    switch(tagID)
    {
    case  1:
        strcpy((char *)value,(char *)GETProfile.ucRequestID_GPReq);//"FINOTLR1024_10302018154841667");//Request id
        //memcpy((char *)value,configLoginLeg1.ucRequestID_req,strlen(configLoginLeg1.ucRequestID_req));
        break;
    case  2:
        strcpy((char *)value,(char *)GETProfile.ucMethodID_GPReq);//Method id //999-Zip data will received in response
        break;
    case  3:
        strcpy((char *)value,(char *)GETProfile.ucTellerID_GPReq);//teller id
        break;
    case  4:
        strcpy((char *)value,(char *)GETProfile.ucSessionID_GPReq);//session key
        break;
    case  5:
        strcpy((char *)value,(char *)GETProfile.ucIsEncrypt_GPReq);//Is encrypt
        break;
    case  6:
        strcpy((char *)value,(char *)GETProfile.ucUserTypeID_GPReq);//Usertype Id
        break;
    case  7:
        strcpy((char *)value,(char *)GETProfile.ucRDChannelID_GPReq);// RD Channel id
        break;
    case  8:
        strcpy((char *)value,(char *)GETProfile.ucChannelID_GPReq);// channel id
        break;
    case  9:
        strcpy((char *)value,(char *)GETProfile.ucServiceID_GPReq);// service id
        break;
    default:
        return FUNC_ERROR;
    }

    return FUNC_OK;
}

int FPB::iParseGet_Profile(char *Data)
{
    char buff1[1024];
    int len,i=0,slen,len2=-1;
      QSqlQuery query;
    int j=0;
    char *RecvJsonData;
    char *CheckData;

    memset(&GETProfile, '\0', sizeof(GETProfile));
    char *cGetProfileTag[] = { "RequestId", "DisplayMessage", "ResponseMessage",
            "ResponseCode", "MessageType", "MessageId", "SessionExpiryTime",
            "SessionId", "RouteID", "ErrorCode", "XMLData", "AuthmanFlag" };

    int MandTag[]={1,1,1,1,1,1,1,1,1,0,0,0};//1 for mandatory and  0 for non-mandatory by saurabh
    int ifOffset[]={3,2,3,3,3,2,2,2,3,2,2,2};//forward
    int ibOffset[]={1,0,1,0,0,0,0,0,0,0,0,0};//backward

    len=sizeof(cGetProfileTag)/sizeof(cGetProfileTag[0]);
    len2=sizeof(MandTag)/sizeof(MandTag[0]);
    for (i = 0; i < len; ++i)
    {
        RecvJsonData=strstr((char *)Data,cGetProfileTag[i]);
        slen=strlen(cGetProfileTag[i]);

        if (RecvJsonData==NULL && MandTag[i]==1)//If Tag not found and its mandatory.
        {
            //DrawString("TAG Missing",15,25,0);
            //ttestall(0,200);
            return FUNC_ERROR;
        }
        else if(RecvJsonData == NULL) //Else proceed to further parsing.
            continue;

        memset(buff1,0,sizeof(buff1));
        RecvJsonData=&RecvJsonData[slen+ifOffset[i]];

        CheckData=RecvJsonData;
        do {
            CheckData++;
        } while ((*CheckData!=',') && (*CheckData!='}'));

        CheckData-=ibOffset[i];
        memset(buff1,0,sizeof(buff1));
        strncpy(buff1,RecvJsonData,CheckData-RecvJsonData);

        switch (i)
        {
        case 0:
            strcpy((char *)GETProfile.ucRequestId_GPRes,buff1);
            break;

        case 1:
            strcpy((char *)GETProfile.ucDisplayMessage_GPRes,buff1);
            break;

        case 2:
            strcpy((char *)GETProfile.ucResponseMessage_GPRes,buff1);
            break;

        case 3:
            strcpy((char *)GETProfile.ucResponseCode_GPRes,buff1);
            break;

        case 4:
            strcpy((char *)GETProfile.ucMessageType_GPRes,buff1);
            break;

        case 5:
            strcpy((char *)GETProfile.ucMessageId_GPRes,buff1);//message id
            break;

        case 6:
            strcpy((char *)GETProfile.ucSessionExpiryTime_GPRes,buff1);//session exp
            break;

        case 7:
            strcpy((char *)GETProfile.ucSessionId_GPRes,buff1);//sessoinid
            break;

        case 8:
            strcpy((char *)GETProfile.ucRouteID_GPRes,buff1);//route id
            break;

        case 9:
            strcpy((char *)GETProfile.ucErrorCode_GPRes,buff1);//master service
            break;

        case 10:
            strcpy((char *)GETProfile.ucErrorCode_GPRes,buff1);//error
            break;

        case 11:
            strcpy((char *)GETProfile.ucXMLData_GPRes,buff1);//xml data
            break;

        case 12:
            strcpy((char *)GETProfile.ucAuthmanFlag_GPRes,buff1);//authman flag
            break;

        default:
            break;
        }
    }
    query.clear();
    query.exec("delete from GetProfile");
    return FUNC_OK;
}

//int FPB::iParseGet_Profile(char *Data)
//{
//    qDebug()<<"iParseGet_Profile Function";
//    char buff1[1024];
//    int len,i=0,slen,len2=-1;
//    //int j=0;
//    char *RecvJsonData;
//    char *CheckData;

//    memset(&GETProfile, '\0', sizeof(GETProfile));
//    char *cGetProfileTag[] = { "RequestId", "DisplayMessage", "ResponseMessage",
//            "ResponseCode", "MessageType", "MessageId", "SessionExpiryTime",
//            "SessionId", "RouteID", "ErrorCode", "XMLData", "AuthmanFlag" };

//    int MandTag[]={1,1,1,1,1,1,1,1,1,0,0,0};//1 for mandatory and  0 for non-mandatory by saurabh
//    int ifOffset[]={3,2,3,3,3,2,2,2,3,2,2,2};//forward
//    int ibOffset[]={1,0,1,0,0,0,0,0,0,0,0,0};//backward

//    len=sizeof(cGetProfileTag)/sizeof(cGetProfileTag[0]);
//    len2=sizeof(MandTag)/sizeof(MandTag[0]);
//    for (i = 0; i < len; ++i)
//    {
//        RecvJsonData=strstr((char *)Data,cGetProfileTag[i]);
//        slen=strlen(cGetProfileTag[i]);

//        if (RecvJsonData==NULL && MandTag[i]==1)//If Tag not found and its mandatory.
//        {
//            //DrawString("TAG Missing",15,25,0);
//            //ttestall(0,200);
//            return FUNC_ERROR;
//        }
//        else if(RecvJsonData == NULL) //Else proceed to further parsing.
//            continue;

//        memset(buff1,0,sizeof(buff1));
//        RecvJsonData=&RecvJsonData[slen+ifOffset[i]];

//        CheckData=RecvJsonData;
//        do {
//            CheckData++;
//        } while ((*CheckData!=',') && (*CheckData!='}'));

//        CheckData-=ibOffset[i];
//        memset(buff1,0,sizeof(buff1));
//        strncpy(buff1,RecvJsonData,CheckData-RecvJsonData);

//        switch (i)
//        {
//        case 0:
//            strcpy((char *)GETProfile.ucRequestId_GPRes,buff1);
//            break;

//        case 1:
//            strcpy((char *)GETProfile.ucDisplayMessage_GPRes,buff1);
//            break;

//        case 2:
//            strcpy((char *)GETProfile.ucResponseMessage_GPRes,buff1);
//            break;

//        case 3:
//            strcpy((char *)GETProfile.ucResponseCode_GPRes,buff1);
//            break;

//        case 4:
//            strcpy((char *)GETProfile.ucMessageType_GPRes,buff1);
//            break;

//        case 5:
//            strcpy((char *)GETProfile.ucMessageId_GPRes,buff1);//message id
//            break;

//        case 6:
//            strcpy((char *)GETProfile.ucSessionExpiryTime_GPRes,buff1);//session exp
//            break;

//        case 7:
//            strcpy((char *)GETProfile.ucSessionId_GPRes,buff1);//sessoinid
//            break;

//        case 8:
//            strcpy((char *)GETProfile.ucRouteID_GPRes,buff1);//route id
//            break;

//        case 9:
//            strcpy((char *)GETProfile.ucErrorCode_GPRes,buff1);//master service
//            break;

//        case 10:
//            strcpy((char *)GETProfile.ucErrorCode_GPRes,buff1);//error
//            break;

//        case 11:
//            strcpy((char *)GETProfile.ucXMLData_GPRes,buff1);//xml data
//            break;

//        case 12:
//            strcpy((char *)GETProfile.ucAuthmanFlag_GPRes,buff1);//authman flag
//            break;

//        default:
//            break;
//        }
//    }
//    return FUNC_OK;
//}
