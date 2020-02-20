#include "FPB.h"
#include "ui_FPB.h"
#include "finomisc.h"
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
//#include "Login.h"

//Login_leg1 configLoginLeg1;
//Login_leg2 configLoginLeg2;

typedef struct WriteThis_Struct {
    const char *readptr;
    long sizeleft;
} WriteThis;

//extern char oBuff[5010];

static size_t write_to_FIle(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t written;
//#if DEBUG
//    printf("ptr = %s",ptr);
//#endif
    written = fwrite(ptr, size, nmemb, stream);
//    printf("written = %d\n", written);
    return written;
}
static size_t read_callback(void *ptr, size_t size, size_t nmemb, void *userp)
{
    WriteThis *pooh = (WriteThis *)userp;
    if(size*nmemb < 1)
        return 0;
    if(pooh->sizeleft)
    {
        *(char *)ptr = pooh->readptr[0]; /* copy one single byte */
        pooh->readptr++;                 /* advance pointer */
        pooh->sizeleft--;                /* less data left */
        return 1;                        /* we return 1 byte at a time! */
    }
    return 0;                          /* no more data left to deliver */
}

//**********************************************************************************************************
//-------------------------------    FILE DOWNLOAD FUNCTION VISIONTEK    -----------------------------------
//**********************************************************************************************************

//int FPB::file_download(char *URL, char *data,char *filename,int TimeOut)
//{


//    qDebug()<<"data---------->"<<data;
//    qDebug()<<"url------------>"<<URL;

//    CURL *curl;
//    CURLcode res;

//    WriteThis pooh;
//    FILE *fp;
//    char *Ret;

//    pooh.readptr = data;
//    pooh.sizeleft = (long)strlen(data);

//    struct curl_slist *headers=NULL; // init to NULL is important
//    headers = curl_slist_append(headers, "Accept: application/json");
//    headers = curl_slist_append(headers, "Content-Type: application/json");

//    res = curl_global_init(CURL_GLOBAL_DEFAULT);
//    fp = fopen(filename,"w");
//    curl = curl_easy_init();


//    if (curl)
//    {
//        curl_easy_setopt(curl, CURLOPT_URL, URL);
//        curl_easy_setopt(curl, CURLOPT_POST, 1);
//        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
//        curl_easy_setopt(curl, CURLOPT_POST, 1L);
//        curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);
//        curl_easy_setopt(curl, CURLOPT_READDATA, &pooh);
//        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_to_FIle);
//        curl_easy_setopt(curl, CURLOPT_WRITEDATA,fp);
//        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
//        curl_easy_setopt(curl, CURLOPT_TIMEOUT, TimeOut);
//        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, pooh.sizeleft);
//        res = curl_easy_perform(curl);
//        curl_easy_cleanup(curl);
//        curl_global_cleanup();
//        fclose(fp);
//        qDebug()<<"value-------->"<<res;
//        if(CURLE_OK == res)
//        {
//            res = curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &Ret);
//            return 0;
//        }

//        else
//        {

//            char err[100];
//            memset(err,0x00,sizeof(err));
//            sprintf(err,"%s",curl_easy_strerror(res));
//            fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(res));
//            return 1;
//        }
//    }
//}

//**********************************************************************************************************

int FPB::file_download(char *URL, char *data,char *filename,int TimeOut)
{
#if DEBUG
    qDebug()<<"data---------->"<<data;
    qDebug()<<"url------------>"<<URL;
    qDebug()<<"data---------->"<<data;
#endif
    int ireturn = -5;
    system("rm /usr/FINO/FPB/RESP/OUTPUT.TXT");
    CURL *curl;
    CURLcode res;
    CMisc miscObj;

    WriteThis pooh;
    memset(&pooh,0,sizeof(pooh));
    FILE *fp;
    char *Ret=NULL;

    pooh.readptr = data;
    pooh.sizeleft = (long)strlen(data);

    struct curl_slist *headers=NULL; // init to NULL is important
    headers = curl_slist_append(headers, "Accept: application/json");
    headers = curl_slist_append(headers, "Content-Type: application/json");

    res = curl_global_init(CURL_GLOBAL_DEFAULT);
#if DEBUG
    printf("filename = %s\n",filename);
#endif
    fp = fopen(filename,"w");

    if(fp == NULL)
    {
        qDebug()<<"file empty";
        ireturn = FUNCTION_FAILED;
        return ireturn;
    }
    //------------------------------------------------------------------------------------------
    curl = curl_easy_init();
    if(curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, URL);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POST, 1L);

#ifndef UAT_HTTP     // File Download
        curl_easy_setopt(curl, CURLOPT_CAPATH, "/etc/ssl/certs/"); // HTTPS
        curl_easy_setopt(curl, CURLOPT_CAINFO, "/etc/ssl/certs/DigiCertGlobalRootCA.crt"); // Working 0703
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L); // HTTPS
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 1L); // HTTPS
#endif
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);
        curl_easy_setopt(curl, CURLOPT_READDATA, &pooh);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_to_FIle);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA,fp);
 #if DEBUG
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
#endif
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, TimeOut);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, pooh.sizeleft);

        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        fclose(fp);
    }
#if DEBUG
    qDebug()<<"value-------->"<<res;
#endif
    if(CURLE_OK == res)
    {
        unsigned char *tempBuff1=NULL;
        int iRet = 1;
        memset(oBuff, 0, sizeof(oBuff));
        tempBuff1=(unsigned char*)oBuff;
        fp=fopen(filename,"r");
        do {
            iRet=fread(tempBuff1++,1,1,fp);
        } while (iRet==1);
        fclose(fp);
#if DEBUG
        printf("CURLE_OK\n");
#endif
        ireturn = 0;
        return ireturn;
    }
    else
    {
        char err[100];
        memset(err,0x00,sizeof(err));
        sprintf(err,"%s",curl_easy_strerror(res));
        fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(res));
        uidmessage::instance()->hide_Message();
        miscObj.DisplayMsgBox(err);
        if(res == CURLE_OPERATION_TIMEOUTED)
        {
            ireturn = RESPONSE_TIMEOUT;
            return ireturn;
        }
        ireturn = FUNC_ERROR;
        return ireturn;
    }

    return ireturn;
}



int FPB::NetworkConnect(int iSmallBigfile, char *IP_URL, int iSOC_CURL)
{
    HostComm hComm;
    CMisc miscObj;
    int len_gSendData;
    int response_timeout = 0;
    int iNetCon = -1, iRet = -2;
   // qDebug()<<"iSmallBigfile == "<<iSmallBigfile<<"IP_URL == "<<IP_URL<<"iSOC_CURL == "<<iSOC_CURL;
//#ifdef UAT_HTTP     // File Download
//    iNetCon = 1;
//#else
//    iNetCon = 2;
//#endif

    if(/*iNetCon == 1 || */iSOC_CURL == 1)
    {
        qDebug()<<"iNetCon == 1";
        int oLen2=0;
        int res = 0;
        uidmessage::instance()->hide_Message();
        hComm.close_connection();
        res = hComm.open_connect(SERVERIP,SERVERPORT);
        if(res != 0)
        {
            qDebug() << "Failed to connect to the Server";
//            memset(&configLoginLeg1,0,sizeof(configLoginLeg1));
//            memset(&configLoginLeg2,0,sizeof(configLoginLeg2));
            memset(gSendData,0,sizeof(gSendData));
            memset(Recvdata,0,sizeof(Recvdata));
            system("rm /usr/FINO/FPB/REQ/LEG2_REQ.TXT");
            uidmessage::instance()->hide_Message();
            uidmessage::instance()->DisplayMessage("Server Not Connected");
            qDebug()<<"FUNCTION_FAILED";
            return FUNCTION_FAILED;
        }
        uidmessage::instance()->hide_Message();
        len_gSendData = strlen((char *)gSendData);

        if((hComm.host_2Sendrecv((unsigned char *)gSendData,len_gSendData,( unsigned char*)oBuff,(int *)&oLen2,(int)response_timeout,iSmallBigfile))!=SUCCESS)
        {
            printf("No Response From Server\n");
            hComm.close_connection();
            miscObj.DisplayMsgBox((char *)"Response Time-Out");
            sleep(2);
            qDebug()<<"***   Response Time-Out   ***";
            uidmessage::instance()->hide_Message();
            //return SOCKET_RESPONSE_FAILED;
            return RESPONSE_TIMEOUT;
        }

        hComm.close_connection();
        system("mkdir /usr/FINO/FPB/RESP/");
        system("cd /usr/FINO/FPB/RESP/");
        system("rm /usr/FINO/FPB/RESP/OUTPUT.TXT");

        int gRecv_len = strlen((char *)oBuff);
        FILE *fp2 = NULL;
        fp2 = fopen("/usr/FINO/FPB/RESP/OUTPUT.TXT","a");
        if(fp2 == NULL)
        {
            return FUNCTION_FAILED;
        }
        qDebug()<<"File"<<fp2;
        fwrite( oBuff, 1, gRecv_len, fp2 );
        fclose(fp2);

    }

    else if(/*iNetCon == 2 || */iSOC_CURL == 2)
    {
        qDebug()<<"iNetCon == 2";
#if DEBUG
    printf("gSendData = %s\n\n",gSendData);

        printf("IP_URL = %s\n\n",IP_URL);
#endif
//        memset(str_url,0,sizeof(str_url));
//        #ifdef PRODUCTION
//            strcpy(str_url,URL_CREATION(LOGIN_URL_PROD));
//        #else
//            strcpy(str_url,URL_CREATION(IP_URL));
//        #endif
        system("rm /usr/FINO/FPB/RESP/OUTPUT.TXT");
        int retk = file_download(IP_URL, (char *)gSendData,"/usr/FINO/FPB/RESP/OUTPUT.TXT",150);
        if(retk!=0)
        {
            if(retk == RESPONSE_TIMEOUT)
            {
                return RESPONSE_TIMEOUT;
            }

            sleep(1);
            uidmessage::instance()->hide_Message();
            return FUNCTION_FAILED;
        }
    }

    return FUNCTION_SUCCESS;
}

/*
int FPB::file_download(char *URL, char *data,char *filename,int TimeOut)
{
    qDebug()<<"data---------->"<<data;
    qDebug()<<"url------------>"<<URL;

    system("rm /usr/FINO/FPB/RESP/OUTPUT.TXT");
    CURL *curl;
    CURLcode res;

    WriteThis pooh;
    FILE *fp;
    char *Ret=NULL;

        pooh.readptr = data;
        pooh.sizeleft = (long)strlen(data);

        struct curl_slist *headers=NULL; // init to NULL is important
        headers = curl_slist_append(headers, "Accept: application/json");
        headers = curl_slist_append(headers, "Content-Type: application/json");

        res = curl_global_init(CURL_GLOBAL_DEFAULT);
        printf("filename = %s\n",filename);
        fp = fopen(filename,"w");
        //fp = fopen("/usr/FINO/FPB/REQ/LEG1_REQ.TXT","w");
        if(fp == NULL)
        {
            qDebug()<<"file empty";
            return 1;
        }
    //------------------------------------------------------------------------------------------
    curl = curl_easy_init();
    if(curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, URL);
//        curl_easy_setopt(curl, CURLOPT_POST, 1);

        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_CAPATH, "/etc/ssl/certs/"); // HTTPS
        // curl_easy_setopt(curl, CURLOPT_SSLCERTTYPE, "PEM");curl-ca-bundle.crt
        // curl_easy_setopt(curl, CURLOPT_CAINFO, "/etc/ssl/certs/DigiCertGlobalRootCA.crt");
        // curl_easy_setopt(curl, CURLOPT_CAINFO, "/usr/share/curl/curl-ca-bundle.crt");
        curl_easy_setopt(curl, CURLOPT_CAINFO, "/etc/ssl/certs/DigiCertGlobalRootCA.crt"); // Working 0703
        //curl_easy_setopt(curl, CURLOPT_CAINFO, "/etc/ssl/certs/cafino.pem");

        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L); // HTTPS
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 1L); // HTTPS
//        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1); ////
//        curl_easy_setopt(curl, CURLOPT_TIMEOUT, TimeOut); ////
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);
        curl_easy_setopt(curl, CURLOPT_READDATA, &pooh);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_to_FIle);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA,fp);
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, TimeOut);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, pooh.sizeleft);

//        curl_easy_setopt(curl, CURLOPT_URL, URL); // HTTPS
        //      curl_easy_setopt(curl, CURLOPT_CAPATH, "/usr/FINO/FPB/FPB.cer"); // HTTPS
        //      curl_easy_setopt(curl, CURLOPT_TIMEOUT, 50); // HTTPS
//        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); // HTTPS
//        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L); // HTTPS

        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        fclose(fp);
    }
    qDebug()<<"value-------->"<<res;
    if(CURLE_OK == res)
    {
        qDebug()<<"CURLE_OK";
        unsigned char *tempBuff1=NULL;
        int iRet = 1;
        memset(oBuff, 0, sizeof(oBuff));
        tempBuff1=(unsigned char*)oBuff;
        fp=fopen(filename,"r");
        do {
            iRet=fread(tempBuff1++,1,1,fp);
        } while (iRet==1);
        fclose(fp);

//            double dl;
//        res = curl_easy_getinfo(curl, CURLINFO_SIZE_DOWNLOAD, &dl); // HTTPS
//        if(!res)
//        {
//             printf("Downloaded %.0f bytes\n", dl);
//        }

printf("CURLE_OK == %s\n",Ret);
return 0;
}
else
{
char err[100];
memset(err,0x00,sizeof(err));
sprintf(err,"%s",curl_easy_strerror(res));
fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(res));
return 1;
}
qDebug()<<"curl_easy_cleanup";

// curl_easy_cleanup(curl);
//------------------------------------------------------------------------------------------
//    if (curl)
//    {
//        curl_easy_setopt(curl, CURLOPT_URL, URL);
//        curl_easy_setopt(curl, CURLOPT_POST, 1);
//        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
//        curl_easy_setopt(curl, CURLOPT_POST, 1L);
//        curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);
//        curl_easy_setopt(curl, CURLOPT_READDATA, &pooh);
//        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_to_FIle);
//        curl_easy_setopt(curl, CURLOPT_WRITEDATA,fp);
//        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
//        curl_easy_setopt(curl, CURLOPT_TIMEOUT, TimeOut);
//        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, pooh.sizeleft);
//        res = curl_easy_perform(curl);
//        curl_easy_cleanup(curl);
//        curl_global_cleanup();
//        fclose(fp);
//        qDebug()<<"value-------->"<<res;
//        if(CURLE_OK == res)
//        {
//            res = curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &Ret);
//            return 0;
//        }

//        else
//        {

//            char err[100];
//            memset(err,0x00,sizeof(err));
//            sprintf(err,"%s",curl_easy_strerror(res));
//            fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(res));
//            return 1;
//        }

//    }


}
*/





