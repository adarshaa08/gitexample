#include "json.h"
#include "displaymessage.h"



/*JSON Tiny parser for FPB Requirement : N L Swami*/


#include "Login.h"

#include "CHeader.h"
//#include "Global.h"
//#include "AEPSBNKMAST.h"
//#define TagsInFile

extern unsigned char TagBuffer[4000]; //Used to store intermediate JSON data
extern unsigned char gSendData[1024*30]; //Used to send data through network
extern unsigned char Recvdata[1024*50]; //Used to receive
extern unsigned char JSONTagBuff[5000];
extern unsigned char BM_Recvdata[1024*45]; // Temp BM
char buff[1024];
extern FPB fpb;

char arr_GETProf[][25] = {"ProfileTypeID","TransactionTypeID", "TransactionType", "TransactionTypeName", "PerTransactionLimit",
        "MinTransLimit", "MaxTransLimit", "AuthTypeID", "AuthTypeName", "ProductTypeID", "Status",
        "IsFinancial", "Denomination", "IsSplit", "NoofRetry", "FallBackAuth", "DMSId", "PageUrl", "RFU",
        "UserGrossLimit", "NoofFallBack", "CashContributionStatus", "IsOnlyWalkin"};

//unsigned char arr_AEPSBankMaster[][25] = {"IFSC","ImpsStatus", "BankName", "IsActive", "VerifyStatus",
//        "DefaultIIFSCCode", "AEPSStatus", "NBIN", "NBINStatus", "DefaultIFSCStatus", "IMPSNBIN"}; // IMPS

unsigned char arr_AEPSBankMaster[][25] = {"code","description", "countryCode", "GSTStateCode", "Lom_State",
        "Lom_StateNumericID"}; // IMPS


char iLastBMTag[10] = "IMPSNBIN";
char iFirstBMTag[5] = "IFSC";


Json::Json(FPB *objwid)
{
   wid_obj = objwid;
}

void Json::vWriteTag(TAGS *tags)
{

    int iIndex = 0;

    while(iIndex  < 6)
    {
        if(strlen(tags->cPtagName[iIndex])>0)
        {
            strcat((char *)TagBuffer,tags->cPtagName[iIndex]);
            strcat((char *)TagBuffer,":");
        }
        iIndex++;
    }
    strcat((char *)TagBuffer,tags->cTagName);
    strcat((char *)TagBuffer,":");
    strcat((char *)TagBuffer,tags->cTagValue);
    strcat((char *)TagBuffer,"\n");
    return;
}




int Json::iParsJSON(unsigned char *cPacket, char *RespC, char *RespMes)
{
    char cStartValue = 0;
    char cStartTagName = 0;
    int index = 0;
    int iTagLevel = 0;
    TAGS JsonTags;
    int iRemovPtag = 0,iRet=-2;
    int iArrayStart = 0;
    int iCurlyBraceIngored = 0;
    memset(&JsonTags,0,sizeof(JsonTags));
    memset(TagBuffer,0,sizeof(TagBuffer)); //added by saurabh

    //char *PtrWlk=NULL;
    unsigned char *PtrWlk = cPacket;
    //return FUNC_ERROR;
    int ret_JV = iIsJsonValid((char *)cPacket,RespC,RespMes,0);
    if ((memcmp(RespC,"0",1)!=0) || (ret_JV == FUNC_ERROR))
    {
        qDebug()<<"RespC"<<RespC;
        return FUNC_ERROR;
    }
    PtrWlk=NULL;
    PtrWlk=JSONTagBuff;

    int iPacketLen = strlen((char *)JSONTagBuff);
  //  qDebug()<<"iPacketLen"<<iPacketLen;

    while(iPacketLen--)
    {
        switch(*PtrWlk)
        {
        case '{':
        {
            cStartTagName =1; cStartValue = 0;
            if(strlen(JsonTags.cTagName)>0 )
                strcpy(JsonTags.cPtagName[iTagLevel],JsonTags.cTagName);
            if(iArrayStart == 0) // added @ 14:49 on 17th Oct 18
                iTagLevel++;

            memset(JsonTags.cTagName,0,sizeof(JsonTags.cTagName));

        }break;
        case '}':

            if(iArrayStart == 0)
            {
                iTagLevel--; // put in condn this @14:25 on 17th Oct 18
                iRemovPtag = 1;
            }
            else
                iCurlyBraceIngored = 1;
            break;
        case ':':
            index = 0;
            cStartTagName =0; cStartValue = 1;
            break;
        case ',':
            index = 0;
            cStartTagName =1; cStartValue = 0;
           // vWriteTag(&JsonTags,callBackFunction);
             vWriteTag(&JsonTags);

            if(iRemovPtag == 1)
            {int i=0;
            i = iTagLevel;
            while(i < 6) //@15:37
            {
                memset(JsonTags.cPtagName[i],0,sizeof(JsonTags.cPtagName[i]));
                i++;
            }
            iRemovPtag = 0;
            }
            memset(&JsonTags.cTagName,0,sizeof(JsonTags.cTagName));
            memset(&JsonTags.cTagValue,0,sizeof(JsonTags.cTagValue));
            break;
        case '[':
            iArrayStart = 1;
            break;
        case ']':
            if(iCurlyBraceIngored == 1)
            {
                iRemovPtag = 1; // @18:38 17/10/2018
                iCurlyBraceIngored = 0;
            }
            iArrayStart = 0;
            break;
        case '\r':	case '\n':	case '\\':	case '\"' : case '\t':
            break;

        case ' ': // adde this on 16th @ 18:54
            if(cStartTagName == 1 && cStartValue == 0)
                break;
        default:
            if(cStartTagName  == 1)
                JsonTags.cTagName[index++] = *PtrWlk;
            if(cStartValue == 1)
            {
                switch(*(PtrWlk-1))
                {
                case '\"':
                    while(*PtrWlk != '\"')
                    {
                        if(*PtrWlk != '\\')
                            JsonTags.cTagValue[index++] = *PtrWlk++;
                        if(*PtrWlk == '\\') PtrWlk++;
                    }
                    break;
                case '[':
                    while(*PtrWlk != ']')
                        JsonTags.cTagValue[index++] = *PtrWlk++;
                    if(*PtrWlk == ']') continue; //@21"20 on 17th Oct 18
                    if(*PtrWlk == '\\') PtrWlk++;
                    break;
                default:
                    switch(*PtrWlk)
                    {
                    case ' ': PtrWlk++; continue;
                    case '{': case '\n': case '\r': case ':': case ',': case '}': case '\"' : case '\t':	// added @12:13 on 17th OCT 18
                        continue;
                    default:
                        JsonTags.cTagValue[index++] = *PtrWlk;
                    }
                    break;
                }
            }
        }
        PtrWlk++;
    }
    vWriteTag(&JsonTags);

    system("cd /usr/FINO/FPB/REQ/");
    system("rm /usr/FINO/FPB/REQ/IntJSON.TXT");
    FILE *fp = NULL;
    fp = fopen("/usr/FINO/FPB/REQ/IntJSON.TXT","a");

    fwrite((char*)TagBuffer, 1, strlen((char*)TagBuffer), fp );
    fclose(fp);


    memset(gSendData,0,sizeof(gSendData));
    memset(&JsonTags,0,sizeof(JsonTags));
    //memset(TagBuffer,0,sizeof(TagBuffer)); //added by saurabh
    PtrWlk=NULL;
    return FUNC_OK;
}


int Json::iTagCount(unsigned char *iPBuff, char *tagToFind)
{
    int j=1, cnt_data=0;
    char *iRecvData=NULL;

    iRecvData = strstr((char *)iPBuff,tagToFind);
    if(iRecvData=='\0')
        return -2;
    iRecvData++;
    cnt_data++;
    while(j)
    {
        iRecvData = strstr(iRecvData,tagToFind);
        if(iRecvData == '\0')
            j=0;
        else
            cnt_data++;
        iRecvData++;
    }
    iRecvData=NULL;
    return cnt_data;
}

int Json::iParseFun(char *arr, int count,char *iFirstTag, char *iLastTag,int Type)
{
    qDebug()<<"Inside the iParseFun "<<Type;
    uidmessage::instance()->hide_Message();
    uidmessage::instance()->Display_BusyMessage("Updating Data Please Wait...");
    int j=1, cnt_data=1;
    char *iRecvData1=NULL, *iStrt_Buff=NULL, *idbbuff=NULL, *idbstartbuff=NULL;
    QSqlQuery query;
    char temp_arr[1000];
    char tempdb_buff[25][100];
    int numberofrows = 0;
    memset(temp_arr,0,sizeof(temp_arr));

    if(Type == BANKMASTER)
        numberofrows = sizeof(arr_AEPSBankMaster) /sizeof(arr_AEPSBankMaster[0]);
    if(Type == GETPROFILE)
        numberofrows = sizeof(arr_GETProf) /sizeof(arr_GETProf[0]);
    qDebug()<<"numberofrows"<<numberofrows;
    qDebug()<<"count"<<count;
    iStrt_Buff=strstr(arr,iFirstTag);
    iRecvData1 = strstr(arr,iLastTag);
#if DEBUG
    qDebug()<<"iStrt_Buff"<<iStrt_Buff;
     qDebug()<<"iRecvData1"<<iRecvData1;
#endif
    do{
        if(cnt_data!=1)
        {
            iStrt_Buff = NULL;
            iStrt_Buff = iRecvData1;
            iRecvData1 = strstr(iRecvData1,iLastTag);
            while((*iRecvData1!=','))
            {
                if(*iRecvData1=='\n')
                    break;
                iRecvData1+=1;
            }
        }
        if(cnt_data==1)
        {
            while((*iRecvData1!=','))
            {
                if(*iRecvData1=='\n')
                    break;
                iRecvData1+=1;
            }
        }
        iRecvData1+=2; //by Saurabh
        memset(temp_arr,0,sizeof(temp_arr));
        strncpy(temp_arr,iStrt_Buff,iRecvData1-iStrt_Buff);
        //temp_arr[iRecvData1-iStrt_Buff]=0;

        int ilen=0,icnt=0,iflag=0;

        switch (Type) {
        case BANKMASTER:
            for(j=0;j<numberofrows;j++,icnt=0)
            {
                idbbuff=NULL;
                do{
                    if(icnt==0)
                        idbbuff = strstr(temp_arr,(char *)arr_AEPSBankMaster[j]);
                    else
                        idbbuff = strstr(idbbuff,(char *)arr_AEPSBankMaster[j]); // For Repeated data in other names

                    if(idbbuff=='\0')
                        return FUNC_ERROR;
                    ilen = strlen((char *)arr_AEPSBankMaster[j]);
                    idbbuff+=ilen;
                    icnt=1;
                }while(*idbbuff!='\\');

                while(*idbbuff!=':')
                    idbbuff++;
                idbbuff++;
                if(*idbbuff=='\\')
                {
                    idbbuff+=2;
                    iflag=1;
                }
                else
                    iflag=0;
                idbstartbuff=NULL;
                idbstartbuff=idbbuff;
                while(*idbbuff!='\\') // To fetch data value
                {
                    if(*idbbuff=='}')
                        break;
                    idbbuff++;
                }
                if(iflag==0 && (strcmp((char *)arr_AEPSBankMaster[j],(char *)iLastTag))) idbbuff--;
                memset(&tempdb_buff[j],0,sizeof(tempdb_buff[j]));
                strncpy(tempdb_buff[j],idbstartbuff,idbbuff-idbstartbuff);
                qDebug()<<"BM_tempdb_buff[j]"<<tempdb_buff[j];

            }

            query.prepare("INSERT INTO AEPSBankMaster (IFSC, ImpsStatus, BankName, IsActive, VerifyStatus, DefaultIIFSCCode, AEPSStatus, NBIN, NBINStatus, DefaultIFSCStatus, IMPSNBIN) values (:IFSC,:ImpsStatus,:BankName,:IsActive,:VerifyStatus,:DefaultIIFSCCode,:AEPSStatus,:NBIN,:NBINStatus,:DefaultIFSCStatus,:IMPSNBIN)");
            query.bindValue(":IFSC",tempdb_buff[0]);
            query.bindValue(":ImpsStatus",tempdb_buff[1]);
            query.bindValue(":BankName",tempdb_buff[2]);
            query.bindValue(":IsActive",tempdb_buff[3]);
            query.bindValue(":VerifyStatus",tempdb_buff[4]);
            query.bindValue(":DefaultIIFSCCode",tempdb_buff[5]);
            query.bindValue(":AEPSStatus",tempdb_buff[6]);
            query.bindValue(":NBIN",tempdb_buff[7]);
            query.bindValue(":NBINStatus",tempdb_buff[8]);
            query.bindValue(":DefaultIFSCStatus",tempdb_buff[9]);
            query.bindValue(":IMPSNBIN",tempdb_buff[10]);



            if(!query.exec())
            {
                qDebug() << "In Insert_BM_Txn_DB = " << query.lastError();
                query.clear();
                query.finish();
                return FUNC_ERROR;
            }
            query.clear();
//********************************************      DELETE QUERY     ************************************************

            query.clear();
            query.exec("DELETE from AEPSBankMaster where AEPSStatus = 'false'");

//*******************************************************************************************************************
            query.finish();
            count--;
            cnt_data++;
            memset(tempdb_buff,0,sizeof(tempdb_buff));

            break;
        case GETPROFILE:
            for(j=0;j<numberofrows;j++,icnt=0)
            {
#if DEBUG
                qDebug()<<"Inside GETPROFILE j="<<j<<"temp_arr========"<<temp_arr;
#endif
                idbbuff=NULL;
                do{
                    if(icnt==0)
                        idbbuff = strstr(temp_arr,(char *)arr_GETProf[j]);
                    else
                        idbbuff = strstr(idbbuff,(char *)arr_GETProf[j]); // For Repeated data in other names

                    if(idbbuff=='\0')
                        return FUNC_ERROR;

                    ilen = strlen((char *)arr_GETProf[j]);
                    idbbuff+=ilen;
                    icnt=1;
                }while(*idbbuff!='\\');

                while(*idbbuff!=':')
                    idbbuff++;
                idbbuff++;
                if(*idbbuff=='\\')
                {
                    idbbuff+=2;
                    iflag=1;
                }
                else
                    iflag=0;
                idbstartbuff=NULL;
                idbstartbuff=idbbuff;
                while(*idbbuff!='\\') // To fetch data value
                {
                    if(*idbbuff=='}')
                        break;
                    idbbuff++;
                }
                if(iflag==0 && (strcmp((char *)arr_GETProf[j],(char *)iLastTag))) idbbuff--;
                memset(&tempdb_buff[j],0,sizeof(tempdb_buff[j]));
                strncpy(tempdb_buff[j],idbstartbuff,idbbuff-idbstartbuff);
#if DEBUG
                qDebug()<<"tempdb_buff[j]"<<tempdb_buff[j];
#endif
            }
            query.clear();
//            if(query.exec("INSERT INTO GetProfile  VALUES('%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s') VALUES( tempdb_buff[0], tempdb_buff[1], tempdb_buff[2], tempdb_buff[3], tempdb_buff[4], tempdb_buff[5], tempdb_buff[6], tempdb_buff[7], tempdb_buff[8], tempdb_buff[9], tempdb_buff[10], tempdb_buff[11], tempdb_buff[12], tempdb_buff[13], tempdb_buff[14], tempdb_buff[15], tempdb_buff[16], tempdb_buff[17], tempdb_buff[18], tempdb_buff[19], tempdb_buff[20], tempdb_buff[21])"))
            query.prepare("INSERT INTO GetProfile (ProfileTypeID, TransactionTypeID,  TransactionType,  TransactionTypeName,  PerTransactionLimit, MinTransLimit,  MaxTransLimit,  AuthTypeID,  AuthTypeName,  ProductTypeID,  Status, IsFinancial,  Denomination,  IsSplit,  NoofRetry,  FallBackAuth,  DMSId,  PageUrl,  RFU, UserGrossLimit,  NoofFallBack,  CashContributionStatus,  IsOnlyWalkin) values (:ProfileTypeID, :TransactionTypeID,  :TransactionType,  :TransactionTypeName,  :PerTransactionLimit, :MinTransLimit, :MaxTransLimit,  :AuthTypeID,  :AuthTypeName,  :ProductTypeID,  :Status, :IsFinancial, :Denomination, :IsSplit, :NoofRetry, :FallBackAuth, :DMSId, :PageUrl, :RFU, :UserGrossLimit, :NoofFallBack, :CashContributionStatus, :IsOnlyWalkin)");
            query.bindValue(":ProfileTypeID",tempdb_buff[0]);
            query.bindValue(":TransactionTypeID",tempdb_buff[1]);
            query.bindValue(":TransactionType",tempdb_buff[2]);
            query.bindValue(":TransactionTypeName",tempdb_buff[3]);
            query.bindValue(":PerTransactionLimit",tempdb_buff[4]);
            query.bindValue(":MinTransLimit",tempdb_buff[5]);
            query.bindValue(":MaxTransLimit",tempdb_buff[6]);
            query.bindValue(":AuthTypeID",tempdb_buff[7]);
            query.bindValue(":AuthTypeName",tempdb_buff[8]);
            query.bindValue(":ProductTypeID",tempdb_buff[9]);
            query.bindValue(":Status",tempdb_buff[10]);
            query.bindValue(":IsFinancial",tempdb_buff[11]);
            query.bindValue(":Denomination",tempdb_buff[12]);
            query.bindValue(":IsSplit",tempdb_buff[13]);
            query.bindValue(":NoofRetry",tempdb_buff[14]);
            query.bindValue(":FallBackAuth",tempdb_buff[15]);
            query.bindValue(":DMSId",tempdb_buff[16]);
            query.bindValue(":PageUrl",tempdb_buff[17]);
            query.bindValue(":RFU",tempdb_buff[18]);
            query.bindValue(":UserGrossLimit",tempdb_buff[19]);
            query.bindValue(":NoofFallBack",tempdb_buff[20]);
            query.bindValue(":CashContributionStatus",tempdb_buff[21]);
            query.bindValue(":IsOnlyWalkin",tempdb_buff[22]);


            if(!query.exec())
            {
                qDebug() << "In Insert_GP_Txn_DB = " << query.lastError();
                query.clear();
                query.finish();
                return FUNC_ERROR;
            }
            query.clear();
            query.finish();

            //FPB Dhiral    //SqliteApp_Execute(GETPROFILE, "INSERT INTO GetProfile (ProfileTypeID, TransactionTypeID,  TransactionType,  TransactionTypeName,  PerTransactionLimit, MinTransLimit,  MaxTransLimit,  AuthTypeID,  AuthTypeName,  ProductTypeID,  Status, IsFinancial,  Denomination,  IsSplit,  NoofRetry,  FallBackAuth,  DMSId,  PageUrl,  RFU, UserGrossLimit,  NoofFallBack,  CashContributionStatus,  IsOnlyWalkin) VALUES('%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s')", tempdb_buff[0], tempdb_buff[1], tempdb_buff[2], tempdb_buff[3], tempdb_buff[4], tempdb_buff[5], tempdb_buff[6], tempdb_buff[7], tempdb_buff[8], tempdb_buff[9], tempdb_buff[10], tempdb_buff[11], tempdb_buff[12], tempdb_buff[13], tempdb_buff[14], tempdb_buff[15], tempdb_buff[16], tempdb_buff[17], tempdb_buff[18], tempdb_buff[19], tempdb_buff[20], tempdb_buff[21]);

            count--;
            cnt_data++;
            memset(tempdb_buff,0,sizeof(tempdb_buff));
            break;
        default:
            break;
        }

    }while(count!=0);

    uidmessage::instance()->hide_Message();
}


int Json:: iParseGP_ABM(char *ifilename,char *iFirstParseData,char *iLastParseData,int *txntype)
{
    //************************  TO READ FILE  ****************************

    int iRet;
    FILE *fp1;
    int ret=-3, contents_len=0;
    unsigned char  *contents;

    char ucResCode[4],ucResMessage[50];
    memset(ucResCode,    0, sizeof(ucResCode));
    memset(ucResMessage, 0, sizeof(ucResMessage));
    system("cd /usr/FINO/FPB/REQ/");
    fp1 = fopen(ifilename, "rb"); // open
    if (fp1 == NULL)
    {
        qDebug()<<"Can't OpenARBP";
        //return FUNCTION_FAILED;
        return FUNC_ERROR;
    }
    fseek(fp1, 0, SEEK_END);
    contents_len = ftell(fp1);
    fseek(fp1, 0, SEEK_SET);
    contents = (unsigned char *)malloc(contents_len);
    if (contents == NULL)
    {
        free(contents);
        fclose(fp1);
        printf("\n in testbio memeory alloc failed");
        return FUNC_ERROR;
    }
    ret=fread(contents,sizeof(unsigned char ),contents_len,fp1);
    if(ret==0)
        fclose(fp1);
    qDebug()<<"contents"<<contents;
     qDebug()<<"contents_len"<<contents_len;

    iRet = iIsJsonValid((char *)contents,(char *)ucResCode,(char *)ucResMessage,(int *)txntype);
   qDebug()<<"After Json valid";
    if ((memcmp(ucResCode,"0",1)!=0) || (iRet != FUNC_OK))
    {
        free(contents);
        return FUNC_ERROR;
    }
    memset(JSONTagBuff,0,sizeof(JSONTagBuff));

    int count_gp = iTagCount((unsigned char *)contents, iLastParseData);

    if (txntype == (int *)BANKMASTER)
    {
        iRet =FPB::iParseBank_Master((char*)contents);
    }
    else if(txntype == (int *)GETPROFILE)
    {
        qDebug()<<"Before iParseGet_Profile";
        iRet = FPB::iParseGet_Profile((char*)contents);
      //  int k = param->Check_Conditions();

    }
    if (iRet != FUNC_OK)
    {
        free(contents);
        return FUNC_ERROR;
    }
    iRet = -3;
    iRet = iParseFun((char *)contents,count_gp,iFirstParseData,iLastParseData,(int)txntype);
    if(iRet == FUNC_ERROR)
    {
        free(contents);
        return FUNC_ERROR;
    }

    free(contents);
    return FUNC_OK;
}

int Json::iIsJsonValid(char *cPacket,char *RespC, char *RespMes,int *type)
{
    char *StrPtr=NULL,*EndPtr=NULL,*PtrWlk=NULL;
    memset(JSONTagBuff,0,sizeof(JSONTagBuff));
    StrPtr=strstr(cPacket,"ResponseCode");
    if (StrPtr==NULL)
    {
        qDebug()<<"StrPtr==NULL FUNC_ERROR";
        return FUNC_ERROR;
    }
    StrPtr=strchr(StrPtr,':');
    //StrPtr=StrPtr+2;
    do
        StrPtr++;
    while(*StrPtr != '\"');
    StrPtr++;
    EndPtr=StrPtr;
    do {
        EndPtr++;
    } while (*EndPtr!=',');
    EndPtr--;
    if (*EndPtr == '\"')
        ;
    else
        EndPtr++;
    strncpy(RespC,StrPtr,EndPtr-StrPtr);
    StrPtr=strstr(cPacket,"ResponseMessage");
    StrPtr+=18;
    EndPtr=strchr(StrPtr,',');EndPtr-=1;
    strncpy(RespMes,StrPtr,EndPtr-StrPtr);

    if (memcmp(RespC,"0",1) == 0)
    {
        StrPtr = strstr(cPacket,"{");
        PtrWlk = strstr(cPacket,"SessionId");
        EndPtr = strstr(PtrWlk,"}");
        if (EndPtr == NULL)
        {
            strcpy(RespC,0);
            printf("RespC == %s\n",RespC);
            return FUNC_ERROR;
        }
        EndPtr++;
        if (type != (int *)BANKMASTER)
            strncpy((char *)JSONTagBuff,StrPtr,EndPtr-StrPtr);
    }
    return FUNC_OK;
}



//int vReadFile(char *cPath, char *buffer)
//{
//	int iRet = 0;
//	int index = 0;
//	S_FS_FILE *fp = FS_open(cPath,"r");
//	do
//	{
//		iRet = FS_read(buffer+index++,1,1,fp);
//	}
//	while(iRet == 1);
//	FS_close(fp);


//    system("cd /usr/FINO/FPB/REQ/");
//    system("rm -rf LEG1_REQ.TXT");
//    int gsend_len = strlen((char *)gSendData);
//    FILE *fp = NULL;
//    fp = fopen("/usr/FINO/FPB/LEG1_REQ.TXT","a");
//    fwrite( gSendData, 1, gsend_len, fp );

//    fclose(fp);



//	return index;
//}




//int testFunc(char *Str) //Remove it
//{
//	int i = 0;
//	i = memcmp(Str,"test",4);

//	return i;
//}

////char cPacket[2048*2];





