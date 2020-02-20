/*JSON Tiny parser for FPB Requirement : N L Swami*/



#include "Login.h"

#include "CHeader.h"
//#include "Global.h"
//#include "AEPSBNKMAST.h"
//#define TagsInFile

#define FILE_NAME_JSON			"/HOST/AUTH.JSON"
//#define FILE_NAME_JSON		"/HOST/SESSION.JSON"
//#define FILE_NAME_JSON		"/HOST/PROFILE.JSON"
//#define FILE_NAME_JSON 	"/HOST/BK_MASTR.JSON"
//#define FILE_NAME_JSON		"/HOST/AEPS_WDR.JSON"
//#define FILE_NAME_JSON		"/HOST/BAL_ENQ.JSON"
//#define FILE_NAME_JSON		"/HOST/LTS.JSON"

//#define FILE_NAME_TXT		"/HOST/AUTH.TXT"
//#define FILE_NAME_TXT		"/HOST/SESSION.TXT"
#define FILE_NAME_TXT		"/HOST/PROFILE.TXT"
//#define FILE_NAME_TXT 	"/HOST/BK_MASTR.TXT"
//#define FILE_NAME_TXT		"/HOST/AEPS_WDR.TXT"
//#define FILE_NAME_TXT		"/HOST/BAL_ENQ.TXT"
//#define FILE_NAME_TXT		"/HOST/AEPS_LTS.TXT"

int iParsLeg1(unsigned char *Data);
int iParsLeg2(unsigned char *Data);
int iIsJsonValid(char *cPacket,char *RespC, char *RespMes,int *type);
int iTagCount(unsigned char *iPBuff, char *tagToFind);
void iParseFun(char *arr, int count,char *iFirstTag, char *iLastTag,int Type);
int iParseGP_ABM(char *ifilename,char *iFirstParseData,char *iLastParseData,int *txntype);
int iParsLeg2(unsigned char *Data);
int iGetTagResponse(char *Data,char *tag,char *buff,int offset);

extern int iParseBank_Master(unsigned char *Data);
extern int iParseGet_Profile(unsigned char *Data);


extern unsigned char TagBuffer[4000]; //Used to store intermediate JSON data
extern unsigned char gSendData[1024*30]; //Used to send data through network
extern unsigned char Recvdata[1024*50]; //Used to receive
extern unsigned char JSONTagBuff[5000];
char buff[1024];

typedef struct
{
	char cTagName[100];
	char cTagValue[100];
	char cDepth;
	char cPtagName[6][100];

}TAGS;



char arr_GETProf[][25] = {"ProfileTypeID","TransactionTypeID", "TransactionType", "TransactionTypeName", "PerTransactionLimit",
		"MinTransLimit", "MaxTransLimit", "AuthTypeID", "AuthTypeName", "ProductTypeID", "Status",
		"IsFinancial", "Denomination", "IsSplit", "NoofRetry", "FallBackAuth", "DMSId", "PageUrl", "RFU",
		"UserGrossLimit", "NoofFallBack", "CashContributionStatus", "IsOnlyWalkin"};

unsigned char arr_AEPSBankMaster[][25] = {"IFSC","ImpsStatus", "BankName", "IsActive", "VerifyStatus",
		"DefaultIIFSCCode", "AEPSStatus", "NBIN", "NBINStatus", "DefaultIFSCStatus", "IMPSNBIN"};

char iLastBMTag[10] = "IMPSNBIN";
char iFirstBMTag[5] = "IFSC";

void vWriteTag(TAGS *tags,  int (*callBackFunction)(char *))
{

	int iIndex = 0;
	//memset(TagBuffer,0,sizeof(TagBuffer));
//#ifdef TagsInFile // FPB Dhiral

//	S_FS_FILE *fp = FS_open(FILE_NAME_TXT,"a");
//	while(iIndex  < 6)
//	{
//		if(strlen(tags->cPtagName[iIndex])>0)
//		{
//			FS_write(tags->cPtagName[iIndex],strlen(tags->cPtagName[iIndex]),1,fp);
//			FS_write(":",1,1,fp);
//		}
//		iIndex++;
//	}

//	FS_write(tags->cTagName,strlen(tags->cTagName),1,fp);
//	FS_write(":",1,1,fp);
//	FS_write(tags->cTagValue,strlen(tags->cTagValue),1,fp);
//	FS_write("\r",1,1,fp);
//	FS_write("\n",1,1,fp);
//	FS_close(fp);

//#else
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
	/*if(strlen(TagBuffer)>0 && callBackFunction != NULL)
		callBackFunction(TagBuffer);*/
	//S_FS_FILE *fp2 = NULL;
	//FS_unlink("/HOST/JSOS.TXT");

	/*
	fp2 =FS_open("/HOST/JSOS.TXT","a");
	FS_write(TagBuffer,strlen(TagBuffer),1,fp2);
	ttestall(0,100);
	FS_close(fp2);
	 */
//#endif

	return;
}




int iParsJSON(unsigned char *cPacket, int (*callBackFunction)(char *), char *RespC, char *RespMes)
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
	iIsJsonValid((char *)cPacket,RespC,RespMes,0);
	if (memcmp(RespC,"0",1)!=0)
	{
		return FUNC_ERROR;
	}

	PtrWlk=NULL;
	PtrWlk=JSONTagBuff;

	int iPacketLen = strlen((char *)JSONTagBuff);

	/*S_FS_FILE *fp = NULL;
	FS_unlink("/HOST/JSONTAG.TXT");
	fp =FS_open("/HOST/JSONTAG.TXT","a");
	FS_write(JSONTagBuff,strlen(JSONTagBuff),1,fp);
	ttestall(0,100);
	FS_close(fp);*/

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
			vWriteTag(&JsonTags,callBackFunction);

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
	vWriteTag(&JsonTags,callBackFunction);

    system("cd /usr/FINO/FPB/REQ/");
    system("rm /usr/FINO/FPB/REQ/IntJSON.TXT");
    FILE *fp = NULL;
    fp = fopen("/usr/FINO/FPB/REQ/IntJSON.TXT","a");

    fwrite( TagBuffer, 1, strlen(TagBuffer), fp );
    fclose(fp);

	iRet = callBackFunction(TagBuffer);
	if (iRet != FUNC_OK)
		return FUNC_ERROR;
	memset(gSendData,0,sizeof(gSendData));
	memset(&JsonTags,0,sizeof(JsonTags));
	memset(TagBuffer,0,sizeof(TagBuffer)); //added by saurabh
	PtrWlk=NULL;
	return FUNC_OK;
}


int iTagCount(unsigned char *iPBuff, char *tagToFind)
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

void iParseFun(char *arr, int count,char *iFirstTag, char *iLastTag,int Type)
{
	int j=1, cnt_data=1;
	char *iRecvData1=NULL, *iStrt_Buff=NULL, *idbbuff=NULL, *idbstartbuff=NULL;

	char temp_arr[1000];
	char tempdb_buff[15][100];
	int numberofrows = 0;
	if(Type == BANKMASTER)
		numberofrows = sizeof(arr_AEPSBankMaster) /sizeof(arr_AEPSBankMaster[0]);
	if(Type == GETPROFILE)
		numberofrows = sizeof(arr_GETProf) /sizeof(arr_GETProf[0]);

	iStrt_Buff=strstr(arr,iFirstTag);
	iRecvData1 = strstr(arr,iLastTag);

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

		/*		FILE *stream=NULL;
		stream = FS_open("TEMP.TXT","a");
		fwrite(temp_arr,1,iRecvData1-iStrt_Buff,stream);
		fclose(stream);*/
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
						return;
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

			}

    //FPB Dhiral    //SqliteApp_Execute(BANKMASTER, "INSERT INTO AEPSBankMaster (IFSC, ImpsStatus, BankName, IsActive, VerifyStatus, DefaultIIFSCCode, AEPSStatus, NBIN, NBINStatus, DefaultIFSCStatus, IMPSNBIN) VALUES('%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s')", tempdb_buff[0], tempdb_buff[1], tempdb_buff[2], tempdb_buff[3], tempdb_buff[4], tempdb_buff[5], tempdb_buff[6], tempdb_buff[7], tempdb_buff[8], tempdb_buff[9], tempdb_buff[10]);

			count--;
			cnt_data++;
			memset(tempdb_buff,0,sizeof(tempdb_buff));

			break;
		case GETPROFILE:
			for(j=0;j<numberofrows;j++,icnt=0)
			{
				idbbuff=NULL;
				do{
					if(icnt==0)
						idbbuff = strstr(temp_arr,(char *)arr_GETProf[j]);
					else
						idbbuff = strstr(idbbuff,(char *)arr_GETProf[j]); // For Repeated data in other names

					if(idbbuff=='\0')
						return;

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

			}

    //FPB Dhiral    //SqliteApp_Execute(GETPROFILE, "INSERT INTO GetProfile (ProfileTypeID, TransactionTypeID,  TransactionType,  TransactionTypeName,  PerTransactionLimit, MinTransLimit,  MaxTransLimit,  AuthTypeID,  AuthTypeName,  ProductTypeID,  Status, IsFinancial,  Denomination,  IsSplit,  NoofRetry,  FallBackAuth,  DMSId,  PageUrl,  RFU, UserGrossLimit,  NoofFallBack,  CashContributionStatus,  IsOnlyWalkin) VALUES('%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s')", tempdb_buff[0], tempdb_buff[1], tempdb_buff[2], tempdb_buff[3], tempdb_buff[4], tempdb_buff[5], tempdb_buff[6], tempdb_buff[7], tempdb_buff[8], tempdb_buff[9], tempdb_buff[10], tempdb_buff[11], tempdb_buff[12], tempdb_buff[13], tempdb_buff[14], tempdb_buff[15], tempdb_buff[16], tempdb_buff[17], tempdb_buff[18], tempdb_buff[19], tempdb_buff[20], tempdb_buff[21]);

			count--;
			cnt_data++;
			memset(tempdb_buff,0,sizeof(tempdb_buff));
			break;
		default:
			break;
		}
	}while(count!=0);
}

//int iParseGP_ABM(char *ifilename,char *iFirstParseData,char *iLastParseData,int *txntype)
//{
//	//************************  TO READ FILE  ****************************
//	size_t size;
//	S_FS_FILE *stream;
//	char *contents;
//	int fileSize = 0;
//	int iRet;

//	stream = FS_open(ifilename,"r");
//	if (stream == NULL)
//		return FUNC_ERROR;
//	FS_seek(stream, 0L, SEEK_END);
//	fileSize = FS_tell(stream);
//	FS_seek(stream, 0L, SEEK_SET);
//	// Allocate space for the entire fi/e content
//	contents = (char*)malloc(fileSize + 1);

//	size = FS_read(contents, 1, fileSize, stream);
//	//printf("size == %d\n",size);
//	FS_close(stream);
//	contents[size] = 0;

//	iRet = iIsJsonValid(contents,(char *)AEPSBM.ucResponseCode,(char *)AEPSBM.ucResponseMessage,(int *)BANKMASTER);
//	memcpy(AEPSBM.ucResponseCode,"0",1);
//	if ((memcmp(AEPSBM.ucResponseCode,"0",1)) || (iRet != FUNC_OK))
//	{
//		free(contents);
//		return FUNC_ERROR;
//	}
//	memset(JSONTagBuff,0,sizeof(JSONTagBuff));

//	int count_gp = iTagCount((unsigned char *)contents, iLastParseData);

//	if (txntype == (int *)BANKMASTER)
//	{
//		iRet = iParseBank_Master(contents);
//	}
//	else if(txntype == (int *)GETPROFILE)
//	{
//		iRet = iParseGet_Profile(contents);
//	}

//	if (iRet != FUNC_OK)
//	{
//		free(contents);
//		return FUNC_ERROR;
//	}
//	iParseFun(contents,count_gp,iFirstParseData,iLastParseData,(int)txntype);

//	free(contents);
//	return FUNC_OK;
//}

int iIsJsonValid(char *cPacket,char *RespC, char *RespMes,int *type)
{
	char *StrPtr=NULL,*EndPtr=NULL,*PtrWlk=NULL;
	memset(JSONTagBuff,0,sizeof(JSONTagBuff));

	StrPtr=strstr(cPacket,"ResponseCode");
	if (StrPtr==NULL)
		return FUNC_ERROR;
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
		if (EndPtr == NULL){
			strcpy(RespC,0);
			return FUNC_ERROR;}
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




int testFunc(char *Str) //Remove it
{
	int i = 0;
	i = memcmp(Str,"test",4);

	return i;
}

//char cPacket[2048*2];




