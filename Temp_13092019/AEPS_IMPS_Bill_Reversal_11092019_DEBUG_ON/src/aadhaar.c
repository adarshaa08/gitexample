/*
    Copyright (C) 2011 Geodesic Limited, Mumbai, India

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell 
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
    INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
    PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE X CONSORTIUM BE LIABLE
    FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
    OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
    DEALINGS IN THE SOFTWARE.

    Except as contained in this notice, the name of the Geodesic Limited shall not be used
    in advertising or otherwise to promote the sale, use or other dealings in this Software
    without prior written authorization from the Geodesic Limited.

*/

#include "aadhaar.h"
//#include "bfd.h"
#include "uid_auth.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <libxml/parser.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include "auth_1.6.pb-c.h"

#define GSCB_BIN "999999"

extern unsigned char * bfd_proto(int *na,char *nfiq,char* fdpaths,int n_fingers,int *rbdlen);

UID_AUTH_Data gUID_AUTH_Data;
char Global_TimeStamp[50];

unsigned char *pidb, *encryptedSessKey,*encryptedSessKeyFI, *pload,*ploadFI,*pload1, *hmac, *template1;
int aes_enc_data_len,aes_enc_data_lenFI,enc_sessn_key,enc_sessn_keyFI,hmac_len,aes_enc_data_len1;
unsigned char *enc_ses_key_pack;
unsigned char enc_ses_key_packFI[512];
//extern char * get_expiry_date(char *expiry);
//extern unsigned char *uid_get_skey_data (unsigned char *key);
//extern unsigned char *uid_get_aes_encrypted_data (unsigned char *in, int inlen,
//		 unsigned char *key);
extern int uid_encode_b64(unsigned char *in, int inlen, unsigned char *out, 
			int *outlen);
//extern unsigned char* biometric_proto_details(char *puid, char *template_data, int template_len,int TranType);
//extern unsigned char* biometric_proto_details_FI(char *puid, char *template_data, int template_len,char *cBIN);

//extern unsigned char* biometric_proto_details_BFD(char *template_data_path, int template_len,int TranType , int number_of_attempts,char *NFIQ,int number_of_finger);

extern int FusionBuffLen1;
extern int FusionBuffLen2;
char duid[16];
char fdc[11];

unsigned char sessKey1[32];
unsigned char FusionFPBuff1[1024];
unsigned char FusionFPBuff2[1024];
//char FPArr[10];
extern int g_FusionCounter;
			
extern int FP_Pos;

/**********************************
	Validate Aadhaar ID
**********************************/
int validate_uid( char *uId )
{
	int dMultTable[10][10] = {{0, 1, 2, 3, 4, 5, 6, 7, 8, 9},
		{1, 2, 3, 4, 0, 6, 7, 8, 9, 5},
		{2, 3, 4, 0, 1, 7, 8, 9, 5, 6},
		{3, 4, 0, 1, 2, 8, 9, 5, 6, 7},
		{4, 0, 1, 2, 3, 9, 5, 6, 7, 8},
		{5, 9, 8, 7, 6, 0, 4, 3, 2, 1},
		{6, 5, 9, 8, 7, 1, 0, 4, 3, 2},
		{7, 6, 5, 9, 8, 2, 1, 0, 4, 3},
		{8, 7, 6, 5, 9, 3, 2, 1, 0, 4},
		{9, 8, 7, 6, 5, 4, 3, 2, 1, 0}};
	int permTable[8][10] = {{0, 1, 2, 3, 4, 5, 6, 7, 8, 9},
		{1, 5, 7, 6, 2, 8, 3, 0, 9, 4},
		{5, 8, 0, 3, 7, 9, 6, 1, 4, 2},
		{8, 9, 1, 6, 0, 4, 3, 5, 2, 7},
		{9, 4, 5, 3, 1, 2, 6, 8, 7, 0},
		{4, 2, 8, 6, 5, 7, 3, 9, 0, 1},
		{2, 7, 9, 3, 8, 0, 6, 4, 1, 5},
		{7, 0, 4, 6, 9, 1, 3, 2, 5, 8}};

	short int i=0, c=0;
	printf("validate_uid: %d:", c);
	for (i=0; i < 12; i++) {
		short int ni=0, newC=0;

		if (isdigit(uId[11-i]))
			ni = uId[11-i] - '0';
		else {
			printf("\n");
			return(-1);
		}
		newC = dMultTable[c][permTable[i%8][ni]];
        //printf("%d:", newC);
		c = newC;
	}
	printf("\n");
	if (c == 0)
		return(0);

	return(-1);
}

/**********************************
    Validate Virtual ID
**********************************/
int validate_Vid( char *vId ) // Ekyc Vid
{

    int dMultTable[10][10] = {{0, 1, 2, 3, 4, 5, 6, 7, 8, 9},
        {1, 2, 3, 4, 0, 6, 7, 8, 9, 5},
        {2, 3, 4, 0, 1, 7, 8, 9, 5, 6},
        {3, 4, 0, 1, 2, 8, 9, 5, 6, 7},
        {4, 0, 1, 2, 3, 9, 5, 6, 7, 8},
        {5, 9, 8, 7, 6, 0, 4, 3, 2, 1},
        {6, 5, 9, 8, 7, 1, 0, 4, 3, 2},
        {7, 6, 5, 9, 8, 2, 1, 0, 4, 3},
        {8, 7, 6, 5, 9, 3, 2, 1, 0, 4},
        {9, 8, 7, 6, 5, 4, 3, 2, 1, 0}};
    int permTable[8][10] = {{0, 1, 2, 3, 4, 5, 6, 7, 8, 9},
        {1, 5, 7, 6, 2, 8, 3, 0, 9, 4},
        {5, 8, 0, 3, 7, 9, 6, 1, 4, 2},
        {8, 9, 1, 6, 0, 4, 3, 5, 2, 7},
        {9, 4, 5, 3, 1, 2, 6, 8, 7, 0},
        {4, 2, 8, 6, 5, 7, 3, 9, 0, 1},
        {2, 7, 9, 3, 8, 0, 6, 4, 1, 5},
        {7, 0, 4, 6, 9, 1, 3, 2, 5, 8}};

    short int i=0, c=0;
    printf("validate_vId: %d:", c);
    for (i=0; i < 16; i++) {
        short int ni=0, newC=0;

        if (isdigit(vId[15-i]))
            ni = vId[15-i] - '0';
        else {
            printf("\n");
            return(-1);
        }
        newC = dMultTable[c][permTable[i%8][ni]];
        //printf("%d:", newC);
        c = newC;
    }
    printf("\n");
    if (c == 0)
        return(0);

    return(-1);
}

//char * parse_expiry_data(char *timestamp, char *expiry)
//{
	
//	char *strTimestamp=NULL;
//    int mon=0, year=0, date=0;
//	char m[][4] = {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};

//	strTimestamp = strtok(timestamp," ");
//	if (strTimestamp != NULL) {
//		int j;

//		for (j=0; j < 12; j++) {
//			if (strcmp(m[j],strTimestamp) == 0) {
//				mon = j+1;
//				break;
//			}
//		}
//	}
//	strTimestamp = strtok(NULL," ");
//	if (strTimestamp != NULL)
//		date = atoi(strTimestamp);
//	strTimestamp = strtok(NULL," ");
//	strTimestamp = strtok(NULL," ");
//	if (strTimestamp != NULL)
//		year = atoi(strTimestamp);
//        sprintf(expiry, "%d%02d%02d", year, mon, date);
//#ifdef DEBUG
//	printf("parse_expiry_data: value is :%s:\n", expiry);
//#endif
//	return expiry;
//}

//unsigned char* pidxml_demographic(char *pname)
//{
//	xmlNodePtr root, demo, bios, n;
//	xmlDocPtr doc;
//	xmlChar *xmlbuff;
//	int buffersize;
//	struct tm * curr_tm;
//        time_t curr_time;
//	char buff[50];

//        curr_time = time(NULL);
//        curr_tm = localtime(&curr_time);
//        sprintf(buff, "%04d-%02d-%02dT%02d:%02d:%02d", curr_tm->tm_year+1900,
//		curr_tm->tm_mon+1, curr_tm->tm_mday,
//		curr_tm->tm_hour, curr_tm->tm_min, curr_tm->tm_sec);

//    doc = xmlNewDoc(NULL);
//	root = xmlNewNode(NULL, "Pid");
//	xmlSetProp(root, "ts", buff);
//	xmlSetProp(root, "ver", "1.0");
//	xmlSetProp(root, "xmlns", "http://www.uidai.gov.in/authentication/uid-auth-request-data/1.0");
//	xmlDocSetRootElement(doc, root);

//	demo = xmlNewNode(NULL, "Demo");
//	n = xmlNewNode(NULL, "Pi");
//	xmlSetProp(n, "ms", "E");
//	xmlSetProp(n, "name", pname ? pname : "");
//	xmlAddChild(demo, n);
//	xmlAddChild(root, demo);

//	xmlDocDumpFormatMemory(doc, &xmlbuff, &buffersize, 1);

//	xmlFreeDoc(doc);
//#if 1
//	printf("\n############################################################\n%s\n", xmlbuff);
//#endif
//	return (unsigned char*)xmlbuff;
//}



//unsigned char* pidxml_biometric(char *tmplData)
//{
//	xmlNodePtr root, demo, bios, n;
//	xmlDocPtr doc;
//	xmlChar *xmlbuff;
//	int buffersize;
//	struct tm * curr_tm;
//        time_t curr_time;
//	char buff[50];

//        curr_time = time(NULL);
//        curr_tm = localtime(&curr_time);
//        sprintf(buff, "%04d-%02d-%02dT%02d:%02d:%02d", curr_tm->tm_year+1900,
//		curr_tm->tm_mon+1, curr_tm->tm_mday,
//		curr_tm->tm_hour, curr_tm->tm_min, curr_tm->tm_sec);

//	doc = xmlNewDoc(NULL);
//	root = xmlNewNode(NULL, "Pid");
//	xmlSetProp(root, "ts", buff);
//	xmlSetProp(root, "ver", "1.0");
//	xmlSetProp(root, "xmlns", "http://www.uidai.gov.in/authentication/uid-auth-request-data/1.0");
//	xmlDocSetRootElement(doc, root);
	
//	if( fdc[0] != '\0')
//	{
//	//-----------------------------------------------------------------
//	n = xmlNewNode(NULL, "Meta");
//		xmlSetProp(n, "fdc", fdc);
//		xmlAddChild(root, n);
//	}

//	bios = xmlNewNode(NULL, "Bios");
//	n = xmlNewNode(NULL, "Bio");
//	xmlSetProp(n, "type", "FMR");
//	xmlNodeSetContent(n, tmplData);
//	xmlAddChild(bios, n);
//	xmlAddChild(root, bios);

//	xmlDocDumpFormatMemory(doc, &xmlbuff, &buffersize, 1);

//	xmlFreeDoc(doc);
//	//-----------------------------------------------------------------
//printf("\n############################################################\n%s\n", xmlbuff);
//	return (unsigned char*)xmlbuff;

//}

///***************************************************
//	 Generate Biometric  - Auth Xml
//***************************************************/

//int get_biometric_protobuff_pid(void **buff, int *len, char *template_data, int template_len)
//{
//    Pbuf__Pid pid = PBUF__PID__INIT; // Pid
//    Pbuf__Demo demo = PBUF__DEMO__INIT; // demo
//    Pbuf__Bios bios = PBUF__BIOS__INIT; //bios

//    pid.demo = &demo;
//    pid.bios = &bios;

//     printf("Buffering PID ################################\n");

//     struct tm * curr_tm;
//     time_t curr_time;
//    // char date[50];

//     memset(Global_TimeStamp,0,sizeof(Global_TimeStamp));

//     curr_time = time(NULL);
//     curr_tm = localtime(&curr_time);
//     sprintf(Global_TimeStamp, "%04d-%02d-%02dT%02d:%02d:%02d", curr_tm->tm_year+1900,
//             curr_tm->tm_mon+1, curr_tm->tm_mday,
//             curr_tm->tm_hour, curr_tm->tm_min, curr_tm->tm_sec);

//     pid.ts = strdup(Global_TimeStamp);
//     pid.ver = strdup("1.0");

//     bios.n_bio = 1; //No of finger prints
//     if( bios.n_bio > 0)
//     {
//         bios.bio = (Pbuf__Bio **)malloc((sizeof(Pbuf__Bio **)) * bios.n_bio );
//     }

//     int i;
    
//     for(i=0; i<bios.n_bio; i++)
//     {
//         printf("i = %d\n", i);
//         Pbuf__Bio bio = PBUF__BIO__INIT;
//         bios.bio[i] = &bio;
//         printf("i = %d\n", i);


//         //bio.posh = 7; //TODO The value should be set to UNKNOWN  //Commented By Praveen
//         bio.posh = FP_Pos;
//         bio.type = PBUF__BIO_TYPE__FMR; //Assumed

//         (bio.content).len = template_len;
//         (bio.content).data = template_data;
//     }

//     *len = pbuf__pid__get_packed_size(&pid);
//     *buff = malloc(*len);
//     pbuf__pid__pack(&pid,*buff);

//     //TODO add all the elements
//     free(pid.ver); free(pid.ts);

//     if( bios.n_bio > 0){
//         free(bios.bio);
//     }

//         return 0;
//}

//unsigned char * authxml_biometric(char *puid, char *tmplData)
//{
//	xmlNodePtr root, n;
//	xmlDocPtr doc;
//	xmlChar *preDigSignedXmlBuff, *digSignedXmlBuff;
//	int buffersize;
//	char *pidb, *encryptedSessKey=NULL, *pload, *hmac;
//	unsigned char sessKey[32], shaHash[65];
//	unsigned char txnId[32], devId[16];

//	sprintf(txnId, "%d", rand());
//	strcpy(duid, puid);
//	printf("\n\nUid value is :%s\n",duid);

//	doc = xmlNewDoc("1.0");
//	root = xmlNewNode(NULL, "Auth");
//	xmlSetProp(root, "xmlns", "http://www.uidai.gov.in/authentication/uid-auth-request/1.0");
//	xmlSetProp(root, "ver", "1.5");
//	xmlSetProp(root, "tid", "public");
//	xmlSetProp(root, "ac", "public");
//	xmlSetProp(root, "sa", "public");
//	xmlSetProp(root, "lk", LICENCE_KEY_ONE);
//	xmlSetProp(root, "uid", puid ? puid : "");
//	xmlSetProp(root, "txn", (const xmlChar *)txnId);
//	xmlDocSetRootElement(doc, root);

//	char bufExpiryStr[12];
//	char *expiry=NULL;
//	n = xmlNewNode(NULL, "Skey");
//	xmlAddChild(root, n);
//	{
//		bzero(bufExpiryStr, 12);
//		expiry = get_expiry_date(expiry);
//		parse_expiry_data(expiry, bufExpiryStr);
//		free(expiry);
//		xmlSetProp(n, "ci", bufExpiryStr);
//	}
	
//	encryptedSessKey = uid_get_skey_data(sessKey);
//	xmlNodeSetContent(n, encryptedSessKey);

//	n = xmlNewNode(NULL, "Uses");
//	xmlSetProp(n, "otp", "n"); //dpin as otp
//	xmlSetProp(n, "pin", "n");
//	xmlSetProp(n, "bio", "y");
//	xmlSetProp(n, "pa", "n");
//	xmlSetProp(n, "pfa", "n");
//	xmlSetProp(n, "pi", "n");
//	xmlSetProp(n, "bt", "FMR");
//	xmlAddChild(root, n);

//    //For 1.6
//    n = xmlNewNode(NULL, (const xmlChar *)"Meta");
//    xmlSetProp(n, "udc", txnId); //Unique Device Code
//    xmlSetProp(n, "fdc", "NC"); //Fingerprint device code
//    xmlSetProp(n, "idc", "NC"); //Iris device code
//    xmlSetProp(n, "pip", "NA"); //Public IP address of the device
//    xmlSetProp(n, "lot", "P"); //Location type.
//    xmlSetProp(n, "lov", "560094"); //Location Value
//    xmlAddChild(root, n);

//	pidb = pidxml_biometric(tmplData);
//	n = xmlNewNode(NULL, "Data");
//	xmlAddChild(root, n);
//	pload = uid_get_aes_encrypted_data(pidb, strlen(pidb), sessKey);
//	xmlNodeSetContent(n, pload);

//	int res=hMacSha256(pidb, strlen(pidb), shaHash);

//	hmac = uid_get_aes_encrypted_data(shaHash, SHA256_LENGTH, sessKey);
//	n = xmlNewNode(NULL, "Hmac");
//	xmlAddChild(root, n);
//	xmlNodeSetContent(n, hmac);
//	//free(encryptedSessKey);
//	free(hmac);

//	xmlDocDumpFormatMemory(doc, &preDigSignedXmlBuff, &buffersize, 1);

//	printf("\n\n AuthXML - 1:\n");
//	printf("\n############################################################\n%s\n", preDigSignedXmlBuff);

//#ifdef DEBUG
	
//	char str[7][256];

//        sprintf(str[0], "echo size of base64 plain template buff is %d >> %s", strlen(tmplData), LOG_FILE);//Data in plain XML format
//        system(str[0]);
//        sprintf(str[1], "echo size of base64 template XML buff is %d >> %s", strlen(pidb), LOG_FILE);//Data in plain XML format
//        system(str[1]);
//        sprintf(str[2], "echo size of encrypted template buff is %d >> %s", strlen(pload), LOG_FILE);//Data in encrypted XML format
//        system(str[2]);
//        sprintf(str[3], "echo size of final encrypted xmlbuff is %d >> %s", strlen(preDigSignedXmlBuff), LOG_FILE); //final Data in Encrypted XML
//        system(str[3]);
//        sprintf(str[4], "echo -------------------------------------------------- >> %s", LOG_FILE);
//        system(str[4]);
//#endif
//	free(pidb);
//	free(pload);

//#ifdef XML_SECURITY
//	printf("\n############################################################\n");
//	printf(" Digital Signature using XML Security\n\n");
//	do_digital_signature(preDigSignedXmlBuff,&digSignedXmlBuff);
//	if(preDigSignedXmlBuff)
//		free(preDigSignedXmlBuff);
//	xmlFreeDoc(doc);
//	FILE *fp = fopen("/tmp/out.xml","w");
//	fwrite(digSignedXmlBuff,1,strlen(digSignedXmlBuff),fp);
//	fclose(fp);
//	return ((unsigned char*)digSignedXmlBuff);
//#else

//	xmlFreeDoc(doc);
//	return ((unsigned char*)preDigSignedXmlBuff);
//#endif
//}

///***************************************************
//	 Generate Biometric  - Auth Xml with Fdc
//***************************************************/

//unsigned char * authxml_biometric_with_fdc(char *puid, char *pfdc, char *tmplData)
//{
//	xmlNodePtr root, n;
//	xmlDocPtr doc;
//	xmlChar *preDigSignedXmlBuff, *digSignedXmlBuff;
//	int buffersize;
//    char *pidb, *encryptedSessKey=NULL, *pload, *hmac;
//	unsigned char sessKey[32], shaHash[65];
//	unsigned char txnId[32], devId[16];

//	sprintf(txnId, "%d", rand());
//	strcpy(duid, puid);
//	printf("\n\nUid value is :%s\n",duid);

//	// Fdc attribute value
//	if (( pfdc != NULL ) && (strlen(pfdc) != 0))
//		strcpy(fdc, pfdc);
//	else
//		fdc[0]='\0';

//	doc = xmlNewDoc("1.0");
//	root = xmlNewNode(NULL, "Auth");
//	xmlSetProp(root, "xmlns", "http://www.uidai.gov.in/authentication/uid-auth-request/1.0");
//	xmlSetProp(root, "ver", "1.5");
//	xmlSetProp(root, "tid", "public");
//	xmlSetProp(root, "ac", "public");
//	xmlSetProp(root, "sa", "public");
//	xmlSetProp(root, "lk", LICENCE_KEY_ONE);
//	xmlSetProp(root, "uid", puid ? puid : "");
//	xmlSetProp(root, "txn", (const xmlChar *)txnId);
//	xmlDocSetRootElement(doc, root);

//	char bufExpiryStr[12];
//	char *expiry=NULL;
//	n = xmlNewNode(NULL, "Skey");
//	xmlAddChild(root, n);
//	{
//		bzero(bufExpiryStr, 12);
//		expiry = get_expiry_date(expiry);
//		parse_expiry_data(expiry, bufExpiryStr);
//		free(expiry);
//		xmlSetProp(n, "ci", bufExpiryStr);
//	}
	
//	encryptedSessKey = uid_get_skey_data(sessKey);
//	xmlNodeSetContent(n, encryptedSessKey);

//	n = xmlNewNode(NULL, "Uses");
//	xmlSetProp(n, "otp", "n"); //dpin as otp
//	xmlSetProp(n, "pin", "n");
//	xmlSetProp(n, "bio", "y");
//	xmlSetProp(n, "pa", "n");
//	xmlSetProp(n, "pfa", "n");
//	xmlSetProp(n, "pi", "n");
//	xmlSetProp(n, "bt", "FMR");
//	xmlAddChild(root, n);

//    //For 1.6
//    n = xmlNewNode(NULL, (const xmlChar *)"Meta");
//    xmlSetProp(n, "udc", txnId); //Unique Device Code
//    xmlSetProp(n, "fdc", "NC"); //Fingerprint device code
//    xmlSetProp(n, "idc", "NC"); //Iris device code
//    xmlSetProp(n, "pip", "NA"); //Public IP address of the device
//    xmlSetProp(n, "lot", "P"); //Location type.
//    xmlSetProp(n, "lov", "560094"); //Location Value
//    xmlAddChild(root, n);

//	pidb = pidxml_biometric(tmplData);
//	n = xmlNewNode(NULL, "Data");
//	xmlAddChild(root, n);
//	pload = uid_get_aes_encrypted_data(pidb, strlen(pidb), sessKey);
//	xmlNodeSetContent(n, pload);

//	int res=hMacSha256(pidb, strlen(pidb), shaHash);

//	hmac = uid_get_aes_encrypted_data(shaHash, SHA256_LENGTH, sessKey);
//	n = xmlNewNode(NULL, "Hmac");
//	xmlAddChild(root, n);
//	//free(encryptedSessKey);
//	free(hmac);

//	xmlDocDumpFormatMemory(doc, &preDigSignedXmlBuff, &buffersize, 1);

//	printf("\n\n AuthXML - 1:\n");
//	printf("\n############################################################\n%s\n", preDigSignedXmlBuff);

//#ifdef DEBUG
	
//	char str[7][256];

//        sprintf(str[0], "echo size of base64 plain template buff is %d >> %s", strlen(tmplData), LOG_FILE);//Data in plain XML format
//        system(str[0]);
//        sprintf(str[1], "echo size of base64 template XML buff is %d >> %s", strlen(pidb), LOG_FILE);//Data in plain XML format
//        system(str[1]);
//        sprintf(str[2], "echo size of encrypted template buff is %d >> %s", strlen(pload), LOG_FILE);//Data in encrypted XML format
//        system(str[2]);
//        sprintf(str[3], "echo size of final encrypted xmlbuff is %d >> %s", strlen(preDigSignedXmlBuff), LOG_FILE); //final Data in Encrypted XML
//        system(str[3]);
//        sprintf(str[4], "echo -------------------------------------------------- >> %s", LOG_FILE);
//        system(str[4]);
//#endif
//	free(pidb);
//	free(pload);

//#ifdef XML_SECURITY
//	printf("\n############################################################\n");
//	printf(" Digital Signature using XML Security\n\n");
//	do_digital_signature(preDigSignedXmlBuff,&digSignedXmlBuff);
//	if(preDigSignedXmlBuff)
//		free(preDigSignedXmlBuff);
//	xmlFreeDoc(doc);
//	FILE *fp = fopen("/tmp/out.xml","w");
//	fwrite(digSignedXmlBuff,1,strlen(digSignedXmlBuff),fp);
//	fclose(fp);
//	return ((unsigned char*)digSignedXmlBuff);
//#else

//	xmlFreeDoc(doc);
//	return ((unsigned char*)preDigSignedXmlBuff);
//#endif
//}


///***************************************************
//	 Generate Demographic  - Auth Xml
//***************************************************/

//unsigned char* authxml_demographic_details(char *puid, char *pname)
//{
//	xmlNodePtr root, n;
//	xmlDocPtr doc;
//	xmlChar *preDigSignedXmlBuff, *digSignedXmlBuff;
//	int buffersize;
//	char *pidb, *encryptedSessKey=NULL, *pload, *hmac;
//	unsigned char sessKey[32];
//	unsigned char txnId[32], devId[16];
//	unsigned char shaHash[64];

//	sprintf(txnId, "%d", rand());
//	strcpy(duid, puid);

//	doc = xmlNewDoc("1.0");
//	root = xmlNewNode(NULL, "Auth");
//	xmlSetProp(root, "xmlns", "http://www.uidai.gov.in/authentication/uid-auth-request/1.0");
//	xmlSetProp(root, "ver", "1.6");
//	xmlSetProp(root, "tid", "public");
//	xmlSetProp(root, "ac", "public");
//	xmlSetProp(root, "sa", "public");
//	xmlSetProp(root, "lk", LICENCE_KEY_ONE);
//	xmlSetProp(root, "uid", puid ? puid : "");
//	xmlSetProp(root, "txn", (const xmlChar *)txnId);
//	xmlDocSetRootElement(doc, root);

//	n = xmlNewNode(NULL, "Uses");
//	xmlSetProp(n, "otp", "n");
//	xmlSetProp(n, "pin", "n");
//	xmlSetProp(n, "bio", "n");
//	xmlSetProp(n, "pa", "n");
//	xmlSetProp(n, "pfa", "n");
//	xmlSetProp(n, "pi", "y");
//	xmlAddChild(root, n);


//    //For 1.6
//    n = xmlNewNode(NULL, (const xmlChar *)"Meta");
//    xmlSetProp(n, "udc", txnId); //Unique Device Code
//    xmlSetProp(n, "fdc", "NC"); //Fingerprint device code
//    xmlSetProp(n, "idc", "NC"); //Iris device code
//    xmlSetProp(n, "pip", "NA"); //Public IP address of the device
//    xmlSetProp(n, "lot", "P"); //Location type.
//    xmlSetProp(n, "lov", "560094"); //Location Value
//    xmlAddChild(root, n);

//	char bufExpiryStr[12];
//	char *expiry=NULL;
//	n = xmlNewNode(NULL, "Skey");
//	{
//		bzero(bufExpiryStr, 12);
//		expiry = get_expiry_date(expiry);
//		parse_expiry_data(expiry, bufExpiryStr);
//		free(expiry);
//		xmlSetProp(n, "ci", bufExpiryStr);
//	}
//	xmlAddChild(root, n);
	
//	encryptedSessKey = uid_get_skey_data(sessKey);
//	xmlNodeSetContent(n, encryptedSessKey);

//	pidb = pidxml_demographic(pname);
//	n = xmlNewNode(NULL, "Data");
//	xmlAddChild(root, n);
//	pload =  uid_get_aes_encrypted_data(pidb, strlen(pidb), sessKey);
//	xmlNodeSetContent(n, pload);

//	int res=hMacSha256(pidb, strlen(pidb), shaHash);

//	hmac = uid_get_aes_encrypted_data(shaHash, SHA256_LENGTH, sessKey);
//	n = xmlNewNode(NULL, "Hmac");
//	xmlAddChild(root, n);
//	xmlNodeSetContent(n, hmac);
////	free(encryptedSessKey);
//	free(hmac);

//	xmlDocDumpFormatMemory(doc, &preDigSignedXmlBuff, &buffersize, 1);
//	free(pidb);
//	free(pload);

//#ifdef DEBUG
//	printf("\n\n AuthXML - 1:\n");
//	printf("\n############################################################\n%s\n", preDigSignedXmlBuff);
//#endif

//#ifdef XML_SECURITY
//	printf("\nDigital Signature using XML Security\n\n");
//	do_digital_signature(preDigSignedXmlBuff,&digSignedXmlBuff);
//	free(preDigSignedXmlBuff);
//	xmlFreeDoc(doc);
//#if 1
//   // send_data(digSignedXmlBuff);
//#endif

//	return((unsigned char*)digSignedXmlBuff);
//#endif

//	xmlFreeDoc(doc);
//	return((unsigned char*)preDigSignedXmlBuff);
//}

///***************************************************
//	 Generate Biometric-Protobuff  - Auth Xml
//***************************************************/

/////********************BDF PROTO DETAILS**********************************//

//unsigned char* biometric_proto_details_BFD(char *template_data_path, int template_len,int TranType , int number_of_attempts,char *NFIQ,int number_of_finger)
//{
//    //FI 0
//    //AEPS 2
//    printf("In Biometric Proto Detail function");
//    int pid_len=0,i=0;
//    unsigned char sessKey[32];
//    unsigned char shaHash[64];
//    free(pload); // akshay j commented temp
//    pload=(char*)malloc(2048*4);





//        encryptedSessKey = uid_get_skey_data_fino(sessKey,&enc_sessn_key,TranType);

//        memset(gUID_AUTH_Data.Encoded_skey,0,sizeof(gUID_AUTH_Data.Encoded_skey));
//        gUID_AUTH_Data.Encoded_skeylen = enc_sessn_key;  // used in field 62
//      // memcpy(gUID_AUTH_Data.Encoded_skey,encryptedSessKey,gUID_AUTH_Data.Encoded_skeylen); // used in field 62



//         printf(" \n");

//        printf("The session key is---- \n");
//        for(i=0;i<32;i++)
//        {
//            printf("%02X ",sessKey[i]);
//            sessKey1[i] = sessKey[i];
//        }
//        printf("\n");
//        printf("The encrypted session key is---here- \n");
//        for(i=0;i<enc_sessn_key;i++)
//        {
//            printf("%02X ",encryptedSessKey[i]);
//            enc_ses_key_pack[i]=encryptedSessKey[i];
//            gUID_AUTH_Data.Encoded_skey[i]=encryptedSessKey[i];
//        }

//        printf("\n");
//        printf("The encrypted session key STORED is---here- \n");

//        for(i=0;i<gUID_AUTH_Data.Encoded_skeylen;i++)  // display data
//        {
//            printf("%02X ",gUID_AUTH_Data.Encoded_skey[i]);

//        }


//        pidb = bfd_proto(&number_of_attempts,NFIQ,(char*)template_data_path,number_of_finger,&pid_len);
//        //get_biometric_protobuff_pid(&pidb, &pid_len, template_data, template_len);

//        printf("proto-buffer template length is %d\n",pid_len);
//        for(i=0;i<pid_len;i++)
//            printf("%02X ",pidb[i]);

//        /************** Fusion ***************/
//        for(i=0; i<pid_len; i++)
//        {
//            FusionFPBuff1[i] = pidb[i];
//        }

//        FusionBuffLen1 = pid_len;

//        if(FusionBuffLen2 > 0)
//        {
//            char TotalFPbuff[1024];
//            memset(TotalFPbuff,0,sizeof(TotalFPbuff));
//            for(i=0;i<pid_len;i++)
//                TotalFPbuff[i] = pidb[i];
//            int j=i;
//            for(i=0;j < (FusionBuffLen2+pid_len);j++,i++)
//                TotalFPbuff[j] = FusionFPBuff2[i];

//            pload =  uid_get_aes_encrypted_data_fino(TotalFPbuff, (FusionBuffLen2+pid_len), sessKey,&aes_enc_data_len,TranType);

//            printf("\n\nFusion proto-buffer aes encrypted %d\n",aes_enc_data_len);
//            for(i=0;i<aes_enc_data_len;i++)
//                fprintf(stdout,"%02X ",pload[i]);
//            fprintf(stdout,"\n");

//            int res=hMacSha256(TotalFPbuff, pid_len+FusionBuffLen2, shaHash);
//            hmac = uid_get_aes_encrypted_data_fino(shaHash, SHA256_LENGTH, sessKey,&hmac_len,TranType);

//            fprintf(stdout,"HMac is :\n");
//            for(i=0; i<SHA256_LENGTH; i++)
//                fprintf(stdout, "%02X ", hmac[i]);
//            fprintf(stdout,"\n");

//        }
//        else
//        {
//            pload =  uid_get_aes_encrypted_data_fino(pidb, pid_len, sessKey,&aes_enc_data_len,TranType);
//             memset(gUID_AUTH_Data.Encoded_pid,0,sizeof(gUID_AUTH_Data.Encoded_pid));
//             gUID_AUTH_Data.Encode_pidBlocklen = aes_enc_data_len;
//            // memcpy(gUID_AUTH_Data.Encoded_pid,pload,gUID_AUTH_Data.Encode_pidBlocklen);

//            printf("\n\nproto-buffer aes encrypted %d\n",aes_enc_data_len);
//            for(i=0;i<aes_enc_data_len;i++)
//            {
//                fprintf(stdout,"%02X ",pload[i]);
//                gUID_AUTH_Data.Encoded_pid[i]=pload[i];

//            }

//            printf("\n");
//            printf("The encrypted PID BLOCK key STORED is---here- \n");


//            for(i=0;i<gUID_AUTH_Data.Encode_pidBlocklen;i++)  // display data
//            {
//                printf("%02X ",gUID_AUTH_Data.Encoded_pid[i]);

//            }


//            printf("--------------------------------");
//            printf("in ascii : %s",pload);
//            fprintf(stdout,"\n");

//            hMacSha256(pidb, pid_len, shaHash);
//            hmac = uid_get_aes_encrypted_data_fino(shaHash, SHA256_LENGTH, sessKey,&hmac_len,TranType);

//            fprintf(stdout,"HMac is :\n");

//            memset(gUID_AUTH_Data.Encoded_HMAC,0,sizeof(gUID_AUTH_Data.Encoded_HMAC));
//            gUID_AUTH_Data.Encode_HMAClen = hmac_len;

//            for(i=0; i<hmac_len; i++)
//            {
//                fprintf(stdout, "%02X ", hmac[i]);
//                gUID_AUTH_Data.Encoded_HMAC[i]=hmac[i];
//            }

//            printf("\n");
//            printf("The encrypted HMACK key STORED is---here- \n");

//            for(i=0;i<gUID_AUTH_Data.Encode_HMAClen;i++)  // display data
//            {
//                printf("%02X ",gUID_AUTH_Data.Encoded_HMAC[i]);

//            }

//            fprintf(stdout,"\n");


//           // memcpy(gUID_AUTH_Data.Encoded_HMAC,hmac,SHA256_LENGTH);

//        }
//    return pload;
//}

////************************************************************************//

//unsigned char* biometric_proto_details(char *puid, char *template_data, int template_len,int TranType)
//{
//    //FI 0
//    //AEPS 2
//    printf("In Biometric Proto Detail function");
//    int pid_len=0,i=0;
//    unsigned char sessKey[32];
//    unsigned char shaHash[64];
//    pload=(char*)malloc(2048);




//        encryptedSessKey = uid_get_skey_data_fino(sessKey,&enc_sessn_key,TranType);

//        memset(gUID_AUTH_Data.Encoded_skey,0,sizeof(gUID_AUTH_Data.Encoded_skey));
//        gUID_AUTH_Data.Encoded_skeylen = enc_sessn_key;  // used in field 62
//      // memcpy(gUID_AUTH_Data.Encoded_skey,encryptedSessKey,gUID_AUTH_Data.Encoded_skeylen); // used in field 62



//         printf(" \n");

//        printf("The session key is---- \n");
//        for(i=0;i<32;i++)
//        {
//            printf("%02X ",sessKey[i]);
//            sessKey1[i] = sessKey[i];
//        }
//        printf("\n");
//        printf("The encrypted session key is---here- \n");
//        for(i=0;i<enc_sessn_key;i++)
//        {
//            printf("%02X ",encryptedSessKey[i]);
//            enc_ses_key_pack[i]=encryptedSessKey[i];
//            gUID_AUTH_Data.Encoded_skey[i]=encryptedSessKey[i];
//        }

//        printf("\n");
//        printf("The encrypted session key STORED is---here- \n");

//        for(i=0;i<gUID_AUTH_Data.Encoded_skeylen;i++)  // display data
//        {
//            printf("%02X ",gUID_AUTH_Data.Encoded_skey[i]);

//        }


//        get_biometric_protobuff_pid(&pidb, &pid_len, template_data, template_len);

//        printf("proto-buffer template length is %d\n",pid_len);
//        for(i=0;i<pid_len;i++)
//            printf("%02X ",pidb[i]);

//        /************** Fusion ***************/
//        for(i=0; i<pid_len; i++)
//        {
//            FusionFPBuff1[i] = pidb[i];
//        }

//        FusionBuffLen1 = pid_len;

//        if(FusionBuffLen2 > 0)
//        {
//            char TotalFPbuff[1024];
//            memset(TotalFPbuff,0,sizeof(TotalFPbuff));
//            for(i=0;i<pid_len;i++)
//                TotalFPbuff[i] = pidb[i];
//            int j=i;
//            for(i=0;j < (FusionBuffLen2+pid_len);j++,i++)
//                TotalFPbuff[j] = FusionFPBuff2[i];

//            pload =  uid_get_aes_encrypted_data_fino(TotalFPbuff, (FusionBuffLen2+pid_len), sessKey,&aes_enc_data_len,TranType);

//            printf("\n\nFusion proto-buffer aes encrypted %d\n",aes_enc_data_len);
//            for(i=0;i<aes_enc_data_len;i++)
//                fprintf(stdout,"%02X ",pload[i]);
//            fprintf(stdout,"\n");

//            int res=hMacSha256(TotalFPbuff, pid_len+FusionBuffLen2, shaHash);
//            hmac = uid_get_aes_encrypted_data_fino(shaHash, SHA256_LENGTH, sessKey,&hmac_len,TranType);

//            fprintf(stdout,"HMac is :\n");
//            for(i=0; i<SHA256_LENGTH; i++)
//                fprintf(stdout, "%02X ", hmac[i]);
//            fprintf(stdout,"\n");

//        }
//        else
//        {
//            pload =  uid_get_aes_encrypted_data_fino(pidb, pid_len, sessKey,&aes_enc_data_len,TranType);
//             memset(gUID_AUTH_Data.Encoded_pid,0,sizeof(gUID_AUTH_Data.Encoded_pid));
//             gUID_AUTH_Data.Encode_pidBlocklen = aes_enc_data_len;
//            // memcpy(gUID_AUTH_Data.Encoded_pid,pload,gUID_AUTH_Data.Encode_pidBlocklen);

//            printf("\n\nproto-buffer aes encrypted %d\n",aes_enc_data_len);
//            for(i=0;i<aes_enc_data_len;i++)
//            {
//                fprintf(stdout,"%02X ",pload[i]);
//                gUID_AUTH_Data.Encoded_pid[i]=pload[i];

//            }

//            printf("\n");
//            printf("The encrypted PID BLOCK key STORED is---here- \n");


//            for(i=0;i<gUID_AUTH_Data.Encode_pidBlocklen;i++)  // display data
//            {
//                printf("%02X ",gUID_AUTH_Data.Encoded_pid[i]);

//            }


//            printf("--------------------------------");
//            printf("in ascii : %s",pload);
//            fprintf(stdout,"\n");

//            hMacSha256(pidb, pid_len, shaHash);
//            hmac = uid_get_aes_encrypted_data_fino(shaHash, SHA256_LENGTH, sessKey,&hmac_len,TranType);

//            fprintf(stdout,"HMac is :\n");

//            memset(gUID_AUTH_Data.Encoded_HMAC,0,sizeof(gUID_AUTH_Data.Encoded_HMAC));
//            gUID_AUTH_Data.Encode_HMAClen = hmac_len;

//            for(i=0; i<hmac_len; i++)
//            {
//                fprintf(stdout, "%02X ", hmac[i]);
//                gUID_AUTH_Data.Encoded_HMAC[i]=hmac[i];
//            }

//            printf("\n");
//            printf("The encrypted HMACK key STORED is---here- \n");

//            for(i=0;i<gUID_AUTH_Data.Encode_HMAClen;i++)  // display data
//            {
//                printf("%02X ",gUID_AUTH_Data.Encoded_HMAC[i]);

//            }

//            fprintf(stdout,"\n");


//           // memcpy(gUID_AUTH_Data.Encoded_HMAC,hmac,SHA256_LENGTH);

//        }
//    return pload;
//}

//unsigned char* biometric_proto_details_FI(char *puid, char *template_data, int template_len,char *cBIN)
//{

//    int i=0;
//    unsigned char sessKey[32];
//    char bankbin[10];
//    memset(bankbin,0,sizeof(bankbin));
//    strcpy(bankbin,cBIN);


//    ploadFI=(char*)malloc(2048);


//    encryptedSessKeyFI = uid_get_skey_data_fino_FI(sessKey,&enc_sessn_keyFI);


//    printf("The encrypted session key is %d \n",enc_sessn_keyFI);
//    for(i=0;i<enc_sessn_keyFI;i++)
//    {
//        printf("%02X ",encryptedSessKeyFI[i]);
//        enc_ses_key_packFI[i] = encryptedSessKeyFI[i];
//    }


//    printf("\n template length : %d\n",template_len);

//    if(strcmp(bankbin,GSCB_BIN) == 0) // if GSCB
//    {
//       aes_enc_data_lenFI = template_len; // akshay j added for GSCB unfied 21/03/2017
//       ploadFI = (unsigned char*) template_data; // akshay j added for gscb unified
//    }
//    else // other banks
//    {
//         ploadFI =  uid_get_aes_encrypted_data_fino_FI((unsigned char *)template_data, template_len, sessKey,&aes_enc_data_lenFI); // AKSHAY J CHANGES TEMP for GSCB test 21/03/2017

//    }


//    printf("\n\nproto-buffer aes encrypted %d\n",aes_enc_data_lenFI);
//    for(i=0;i<aes_enc_data_lenFI;i++)
//        fprintf(stdout,"%02X ",ploadFI[i]);
//    fprintf(stdout,"\n");

//    return ploadFI;
//}

//int uid_auth_demographic_details(char *puid, char *name)
//{
//	unsigned char *authb=NULL;
//    int res=0;
	
//	authb = authxml_demographic_details(puid, name);
//	puts(authb);
//	free(authb);
	
//	return res;
//}

#if 1




char getHexChar(unsigned int bt)    //check made unsigned char to unsigned int
{
        bt &= 0x0f;

        if( (bt >= 0 ) && ( bt <= 9 ) ) // wajahath added
                return '0' + bt;
        else
                return 'A' + bt - 10;
}

int HexByte2AsciiStr(unsigned char  *hex_val, int hex_len,char *str)
{
    int j = 0,i;

    if (hex_len <= 0)
        return 0;

    if (!str)
        return 0;
    for (i = 0; i < hex_len; i++)
    {
        str[j++] = getHexChar(hex_val[i] >> 4);
        str[j++] = getHexChar(hex_val[i] & 0x0f);
    }
    str[j] = '\0';

    return strlen(str);
}


#endif

