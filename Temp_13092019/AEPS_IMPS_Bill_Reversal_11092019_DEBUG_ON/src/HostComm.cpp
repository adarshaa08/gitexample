#include "HostComm.h"
#include "displaymessage.h"
#include "FPB.h"
//extern int copy_file (char *dest,char *src);
INT sock_fd;
#include <string.h>
//extern char txntype1;

HostComm::HostComm()
{

}

HostComm::~HostComm()
{

}

///////////////////////////////////////////////////////////////////
//
// This below function will open the port for connection
//
//
//  Input : IP, Port
//  Type  : char *, char *
//  Output:
//
//  Date  :                           Author:
///////////////////////////////////////////////////////////////////

INT HostComm::open_connect(CHAR *IP,CHAR *Port)
{
    system("ln -s /etc/ppp/resolv.conf /etc/resolv.conf"); // Dhiral Softlink 5023
    //copy_file ("/etc/resolv.conf","/etc/ppp/resolv.conf");
    sleep(1);
    struct sockaddr_in serv_addr;
    INT res=0;
    char IP_address[50];
    memset(IP_address,0,sizeof(IP_address));
    QSqlQuery query;
    query.clear();
    sock_fd = -5; // Dhiral
    qDebug()<<"IP = "<<IP<<" PORT: "<<Port;
    uidmessage::instance()->hide_Message();
    uidmessage::instance()->Display_BusyMessage("Connecting server");
    if((sock_fd=socket(AF_INET,SOCK_STREAM,0))<0)  // this function is usde to open socket connection for connecting Server
    {
        qDebug()<<"Socket Cannot Opened";
        return -1;
    }


//        strcpy(IP_address,IP);
//        qDebug()<<"IP_address : "<<IP_address;
//        //QHostInfo info = QHostInfo::fromName("FIFINO.CANARABANK.IN");
//        QHostInfo info = QHostInfo::fromName(IP_address);
//        qDebug()<<"info : "<<info.addresses();

//        QHostAddress address;
//        if (!info.addresses().isEmpty())
//        {
//             address = info.addresses().first();
//        }
//        qDebug()<<"QHostAddress address : "<<address.toString();
//        IP = address.toString().toAscii().data();
//        qDebug()<<"Converted DNS address : "<< IP;



    //**************************************************************//

    bzero((CHAR *)&serv_addr,sizeof(serv_addr));
    serv_addr.sin_family     = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(IP);
    serv_addr.sin_port	= htons(atoi(Port));

    res=connect(sock_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));  // this function is used to connect to the server

    //printf("res = %d\n",res);
    qDebug()<<"res = "<<res;
    if(res == 0)
    {
        qDebug()<<"Connected res = "<<res;
        //printf("connected res =%d\n",res);
        uidmessage::instance()->hide_Message();
        uidmessage::instance()->Display_BusyMessage("Connected to Server");
    }
    else
    {
#if DEBUG
        printf("cannot connect\n");
#endif
        return -1;
    }

    qDebug()<<"End of Open Connect";
    qDebug()<<"Sockfd "<<sock_fd;
    qDebug()<<"res "<<res;
    return res;
}


///////////////////////////////////////////////////////////////////
//
// This below function will close the connection
//
//
//  Input :
//  Type  :
//  Output:
//
//  Date  :                           Author:
///////////////////////////////////////////////////////////////////

VOID HostComm::close_connection()
{
    if(sock_fd != 0)
        close(sock_fd);
    sleep(1);
    sock_fd = 0;
}

INT HostComm::host_2Sendrecv(unsigned char *I_buff,INT inLen,unsigned char *O_buff,INT *ouLen,INT TO,INT flag)
{
    INT res,i=0;
    unsigned char buf[1024*40]={0};
    memset(buf, 0X00, sizeof(buf));
    qDebug()<<"Sockfd"<<sock_fd;

    //    qDebug()<<"host_2Sendrecv Buff ::: ";
    //    for(i=0;i<=inLen;i++)
    //      cout<<I_buff[i];
    //    printf("\n\n");

    //    qDebug()<<"IBuff : ";
    //    for(i=0;i<inLen;i++)
    //        printf("%02X", I_buff[i]);
    //    printf("\n");

    if((res = Write_Comm(sock_fd, I_buff,inLen)) != SUCCESS)  // this function is used wite in the socket
    {
#if DEBUG
        printf("writing to sockfd failed \n");
#endif
        return -1;
    }

    sleep(2);  // database insertion delay in seconds

    uidmessage::instance()->hide_Message();
#if DEBUG
    qDebug()<<"Write Success"<<res;
#endif
    if((res = Read_Comm(sock_fd,ouLen,buf,TO,flag)) != SUCCESS)  // this function is used to read from socket
    {
        uidmessage::instance()->hide_Message();
        if(res == SOCKFUNC_FAILED)
        {
#if DEBUG
            qDebug()<<"SOCK Timeout failed";
#endif
            //uidmessage::instance()->DisplayMessage("SOCK Timeout failed\nPlease try agian!!");
        }
        else if(res == FUNCAL_FAILED)
        {
#if DEBUG
            qDebug()<<"AUTO LTS Condition Generated";
#endif
        }
        //else
            uidmessage::instance()->DisplayMessage("Cannot Receive From Server");
#if DEBUG
        printf("Reading From Socket Failed\n");
#endif
        //return -2;
        return FUNCAL_FAILED;
    }

#if DEBUG
    qDebug()<<"Read Success"<<res;
    qDebug()<<"host-oLen ="<<*ouLen;
    //	printf("host-oLen = %d\n", *ouLen);
    //	printf("host-oBuff = ");
    qDebug()<<"host-oBuff";
#endif
    for(i=0; i<*ouLen; i++)
    {
        O_buff[i] = buf[i];
#if DEBUG
        printf("%02X", O_buff[i]);
#endif
    }
    printf("\n==============\n\n\n");

    return res;
}



INT HostComm::host_2Sendrecv_param(unsigned char *I_buff,INT inLen,unsigned char *O_buff,INT *ouLen,INT TO)
{
    INT res,i=0;
    unsigned char buf[4096];
    memset(buf, 0X00, sizeof(buf));
#if DEBUG
    qDebug()<<"Sockfd"<<sock_fd;
    qDebug()<<"IBuff : ";

    for(i=0;i<inLen;i++)
        printf("%02X", I_buff[i]);
    printf("\n");
#endif

    if((res = Write_Comm(sock_fd, I_buff,inLen)) != SUCCESS)
    {
#if DEBUG
        printf("writing to sockfd failed \n");
#endif
        return -1;
    }

#if DEBUG
    qDebug()<<"Write Success"<<res;
#endif

    if((res = Read_Comm(sock_fd,ouLen,buf,TO,i)) != SUCCESS)
    {
#if DEBUG
        printf("Reading From Socket Failed\n");
#endif
        return -2;
    }

#if DEBUG
    qDebug()<<"Read Success"<<res;
    qDebug()<<"host-oLen ="<<*ouLen;
    //	printf("host-oLen = %d\n", *ouLen);
    //	printf("host-oBuff = ");
    qDebug()<<"host-oBuff";
#endif

    for(i=0; i<*ouLen; i++)
    {
        O_buff[i] = buf[i];
#if DEBUG
        printf("%02X ", O_buff[i]);
#endif
    }

    printf("\n==============\n\n\n");

    return res;
}

///////////////////////////////////////////////////////////////////
//
// This below function will write request to the socket
//
//
//  Input : file descriptor, I/P Buff, Length
//  Type  :
//  Output:
//
//  Date  :                           Author:
///////////////////////////////////////////////////////////////////

INT HostComm::Write_Comm(UINT fd,unsigned char *bufin,INT Len)
{
    INT len;
    printf("Len = %d\n\n",Len);
    uidmessage::instance()->hide_Message();
    uidmessage::instance()->Display_BusyMessage("Transmitting ...");
    len = write(fd,bufin,Len);		// Write data into socket
#if DEBUG
    printf("write len = %d\n", len);
#endif

    if(len != Len)
    {
#if DEBUG
        printf("Writing Error\n");
#endif
        return -1;
    }
    return SUCCESS;
}

///////////////////////////////////////////////////////////////////
//
// This below function will read raw data from the socket
//
//
//  Input :
//  Type  :
//  Output:
//
//  Date  :                           Author:
///////////////////////////////////////////////////////////////////


//INT HostComm:: iReadResponse(UINT fd,INT *len,unsigned char *O_buff,INT TO)
//{
//        char CommBuff[255];
//       int iBytesReceived = 0;
//        char cRespPacket[1024*10];
//       char *TmpPtr = CommBuff;
//       char *TmpPtr1 ;
//       char cContelenBuff[5];
//       char *PtrToContentLen;
//       int iContentLen = 0;
//       int iIndex = 0;
//       int iRespIndex = 0;

//       int iByteCnt = 0;


//       qDebug()<<"Inside read";
//       memset(CommBuff,0,sizeof(CommBuff));

//       iBytesReceived = read(fd,TmpPtr,250);

//       PtrToContentLen = strstr(TmpPtr,"Content-Length");

//       while(*PtrToContentLen++ != ':');

//       while(*PtrToContentLen++ == ' ');

//       memset(cContelenBuff,0,sizeof(cContelenBuff));
//         qDebug()<<"Inside read12345";
//       while(isdigit(*PtrToContentLen))
//       {
//             cContelenBuff[iIndex++] = *PtrToContentLen;
//              PtrToContentLen++;
//       }
//       iContentLen = atoi(cContelenBuff);
//       qDebug()<<"Inside r1";
//       TmpPtr1 = strstr(TmpPtr,"{");
//       qDebug()<<"Inside r2";
//       iBytesReceived = strlen(TmpPtr1);
//       qDebug()<<"Inside r3";
//       memcpy(cRespPacket,TmpPtr1,iBytesReceived);
//       qDebug()<<"Inside r4";
//       while(iBytesReceived < iContentLen)
//       {
//              iByteCnt = read(fd,(char *)(cRespPacket+iBytesReceived),1024);
//              iBytesReceived += iByteCnt;
//            //  TmpPtr+=iByteCnt;
//       }


//        qDebug()<<"Inside read1345";
//       if(iBytesReceived<iContentLen)

//              return -1;


//        *len =iBytesReceived;
//       printf("\n*len = %d\t bytes = %d\n", *len, iBytesReceived);


//       return 0;

//}s


INT HostComm::  Read_Comm(UINT fd,INT *len,unsigned char *O_buff,INT TO,INT flag)  //Tarapathi
{
    INT ret=0,res=0;
    char buff[1024 * 200];
    memset(buff,0,sizeof(buff));
    int X = 0;
    int j = 0;
    int ret1 =0;
    uidmessage::instance()->hide_Message();
    uidmessage::instance()->Display_BusyMessage("Receiving from Server...");

    struct timeval tv;
    if(flag == 0)
        tv.tv_sec = 150;        // 120 Secs Timeout
    else
        tv.tv_sec = 180;        // 180 Secs Timeout
    tv.tv_usec = 0;        // Not init'ing this can cause strange errors
    int iSock  = setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv,sizeof(struct timeval));
    if(iSock!=0)
    {
#if DEBUG
        qDebug()<<"SOCKFUNC_FAILED";
#endif
        return SOCKFUNC_FAILED;
    }

#if DEBUG
    qDebug()<<"iSock == "<<iSock<<"FLag == "<<flag;
    printf("Inside while loop\n");
#endif

    int nDataLength =1,loop=1,value=0;
    char * ret2;
    QString capture,desired;
    ret = 0;
    QStringList lines;
      QString arr[50];
      if(flag==0)
      {
          do
          {
              memset(buff,0,sizeof(buff));
              nDataLength = recv(fd,buff,10000,0);
              //   nDataLength = read(fd,buff,10000);
              if(nDataLength<=0)
                  break;
              if(loop==1)
              {
                  ret2 = strstr(buff,"Content-Length");
                  if(ret1!=NULL)
                  break;
                  capture = buff;
                  desired = capture.mid(capture.indexOf(":")+1);
                  qDebug()<<"Desired"<<desired;
                  lines = desired.split( "\n", QString::SkipEmptyParts );
                  //value = desired.toInt();

                  int i=0;
                  for (i = 0; i < lines.size(); ++i)
                  {
                      arr[i] = lines.at(i);
                      qDebug()<<"LINE :"<<arr[i];
                      qDebug()<<"i == "<<i;
                  }
                  value =arr[0].simplified().remove('"').toInt();
              }
              printf("Datalength = %d\n",nDataLength);


              for(X=0;X<nDataLength;X++,ret++)
              {
                  O_buff[ret]=buff[X];
                  printf("%02X",buff[X]);
                  // printf("Xi = %d\t reti = %d\n", X,ret);
              }
              *len =*len + X;
              ret = *len;
              X=0;
              printf("\n*len = %d\t X = %d\t ret = %d\n", *len, X,ret);
              printf("\n****************value********%d",value);
              if((value+362)<= (*len))
                  break;
              loop++;
              qDebug()<<"12345";
          }while (nDataLength>0);
          printf("\nlen = %d\n", *len);
          O_buff[*len] = '\0';

      }
      else if(flag == 1)
      {
          system("cd /usr/FINO/FPB/REQ/");
          system("rm /usr/FINO/FPB/REQ/GPABM.TXT");
          FILE *fp2 = NULL;
          fp2 = fopen("/usr/FINO/FPB/REQ/GPABM.TXT","a");
          qDebug()<<"fILE"<<fp2;


          do
          {
              memset(buff,0,sizeof(buff));
              nDataLength = recv(fd,buff,10000,0);
              if(nDataLength<=0)
                  break;
              //   nDataLength = read(fd,buff,10000);
              if(loop==1)
              {
                  ret2 = strstr(buff,"Content-Length");
                  if(ret1!=NULL)
                    break;
                  capture = buff;
                  desired = capture.mid(capture.indexOf(":")+1);
                  qDebug()<<"Desired"<<desired;
                  lines = desired.split( "\n", QString::SkipEmptyParts );
                  //value = desired.toInt();

                  int i=0;
                  for (i = 0; i < lines.size(); ++i)
                  {
                      arr[i] = lines.at(i);
                      qDebug()<<"LINE :"<<arr[i];
                      qDebug()<<"i == "<<i;
                  }
                  value =arr[0].simplified().remove('"').toInt();
              }
              printf("Datalength = %d\n",nDataLength);
              fwrite( buff, 1, nDataLength, fp2 );
              qDebug()<<"After write function";

              for(X=0;X<nDataLength;X++,ret++)
              {
                //  O_buff[ret]=buff[X];
                  printf("%02X",buff[X]);
                  // printf("Xi = %d\t reti = %d\n", X,ret);
              }
              *len =*len + X;
              ret = *len;
              X=0;
              printf("\n*len = %d\t X = %d\t ret = %d\n", *len, X,ret);
              printf("\n****************value********%d",value);
              if((value+300)<= (*len))
                  break;
              loop++;
              qDebug()<<"12345";

          }
          while (nDataLength>0);
          fclose(fp2);
      }

      if(nDataLength<=0)
      {
          qDebug()<<"nDataLength <=0   : "<<nDataLength;
          return FUNCAL_FAILED;
      }

      if(*len<=0)
          return res;

      qDebug()<<"DOWNLOAD SUCCESSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS";
      return SUCCESS;

}

//INT HostComm::Read_Comm(UINT fd,INT *len,unsigned char *O_buff,INT TO,INT flag)  //Tarapathi
//{
//    INT ret=0,bytes=0,time_out=0,res=0;  //alloc_bytes=1024*30
//    char buff[1024 * 200];  // akshay j changes 150 KB allocation 7 march 2016
//    memset(buff,0,sizeof(buff));
//    int X = 0;
//    unsigned int iExpLen = 0;
//    int j = 0;
//    int ret1 =0;
//    uidmessage::instance()->hide_Message();
//    uidmessage::instance()->Display_BusyMessage("Receiving from Server...");


//    printf("Inside while loop\n");
//    int nDataLength =1;
//    ret = 0;
//    if(flag==0)
//    {
//        do
//        {
//            memset(buff,0,sizeof(buff));
//            nDataLength = recv(fd,buff,10000,0);
//            //   nDataLength = read(fd,buff,10000);
//            if(nDataLength <= 0)
//                break;
//            printf("Datalength = %d\n",nDataLength);

//            for(X=0;X<nDataLength;X++,ret++)
//            {
//                O_buff[ret]=buff[X];
//                printf("%02X",buff[X]);
//                // printf("Xi = %d\t reti = %d\n", X,ret);
//            }
//            *len =*len + X;
//            ret = *len;
//            X=0;
//            printf("\n*len = %d\t X = %d\t ret = %d\n", *len, X,ret);
//            if(nDataLength <= 262)
//                break;
//            qDebug()<<"12345";
//        }while (1);
//        printf("\nlen = %d\n", *len);
//        O_buff[*len] = '\0';
//    }
//    else if(flag == 1)
//    {
//        system("cd /usr/FINO/FPB/REQ/");
//        system("rm /usr/FINO/FPB/REQ/GPABM.TXT");
//        FILE *fp2 = NULL;
//        fp2 = fopen("/usr/FINO/FPB/REQ/GPABM.TXT","a");
//        qDebug()<<"fILE"<<fp2;


//        do
//        {
//            memset(buff,0,sizeof(buff));
//            nDataLength = recv(fd,buff,10000,0);
//            //   nDataLength = read(fd,buff,10000);
//            if(nDataLength <= 0)
//                break;
//            printf("Datalength = %d\n",nDataLength);
//            fwrite( buff, 1, nDataLength, fp2 );
//            qDebug()<<"After write function";

//            for(X=0;X<nDataLength;X++,ret++)
//            {
//              //  O_buff[ret]=buff[X];
//                printf("%02X",buff[X]);
//                // printf("Xi = %d\t reti = %d\n", X,ret);
//            }
//            *len =*len + X;
//            ret = *len;
//            X=0;
//            printf("\n*len = %d\t X = %d\t ret = %d\n", *len, X,ret);
//            if(nDataLength <=262)
//                break;
//            qDebug()<<"12345";
//        }
//        while (1);
//        fclose(fp2);
//    }


//                ret1 = 0;
//                for(ret1=0;ret1<*len;ret1++)
//                {
//                    printf("outside");
//                        printf("%02X",buff[ret1]);
//                }


//    while(1)
//    {
//        printf("Ioctl return value = %d\n", ioctl(fd,FIONREAD,&bytes));

//        bytes = bytes*2;
//        printf("bytes after ioctl = %d\n", bytes);
//        if(bytes<=0)
//        {
//            sleep(1);
//            //time_out++;
//            if(time_out==TO)
//            {
//                printf("Break Timeout\n");
//                res=READ_TIME_OUT;
//                qDebug()<<"Break Timeout";
//                break;
//            }
//            qDebug()<<"time outtt";
//            time_out++; // added by vaibhav for time out after 0 sec.
//            continue;
//        }
//        printf("\n*len = %d\t bytes = %d\n", *len, bytes);

//        if((*len)+bytes>sizeof(buff))
//           printf("##############********* READ BUFFER EXCEDDED********######## = %d\n", bytes);

//        printf("buff len to be added     %d\n", *len);
//        ret = recv(fd, buff , 10000 , 0);
//       // ret=read(fd,buff,bytes);

//        printf("ret from read is %d\n", ret);
//        if(ret<0)
//        {
//            if(errno==EINTR)
//                continue;
//            printf("break 3\n");
//            break;
//        }
//        else
//        {
//            *len=(*len)+ret;
//            printf("Read len 1 ret =%d  len = %d\n", ret,  *len);

//            for(X=ret1;X<ret;X++)
//            {
//                O_buff[X]=buff[X];
//                printf("%02X",buff[X]);
//            }
//            printf("\n\nret = %d\n\n", ret);

//            printf("\nX = %d\n", X);


////                iExpLen = buff[0]<<8 | buff[1];
////                iExpLen += 2;

////                printf("Expected Length = %d\n", iExpLen);
//                qDebug()<<"Label123 label12 ";
////                if(X < 1320) // Full Packet Size = 1320
////                {
////                     qDebug()<<"Label label ";
////                    memset(buff, 0, sizeof(buff));
////                    ret=0;
////                    *len=0;
////                    X=0;

////                   // goto label;
////                }

////            while(X < iExpLen)
////            {

////                ///****MODIFICATION***// 13-04-16


////                ///*******CLOSE************//

////                memset(buff, 0, sizeof(buff));
////                ret = read(fd,buff,bytes);
////                printf("Read len 2 = %d\n", ret);
////                if(ret<0)
////                {
////                    qDebug()<<"ret<0 break";
////                    printf("break 4\n");
////                    break;
////                }

////                for(j=0; j<ret; j++,X++)
////                {
////                    qDebug()<<"for loop";
////                    if(j==0)
////                        printf("\nX = %d\n", X);
////                    O_buff[X]=buff[j];
////                    printf("%02X",O_buff[X]);
////                }
////                if(X >= iExpLen)
////                {
////                    qDebug()<<"X >= iExpLen";
////                    printf("\n--> X = %d\n", X);
////                    printf("\nEnding Break\n");
////                    break;
////                }
////            }


//            *len = X;



//            /***********/

//            printf("\nX = %d\n", X);
//            O_buff[X] = '\0';


//            printf("\n");
//            break;
//        }
//    }



INT HostComm::Read_Comm_bkp(UINT fd,INT *len,unsigned char *O_buff,INT TO) //bkp on 01052014
{
    INT ret=0,bytes=0,time_out=0,alloc_bytes=2048,res=0;
    CHAR *buff=NULL;
    int X = 0;

    buff=(CHAR*)malloc(alloc_bytes);
    if(buff==NULL)
    {
        printf("Memory Alloication Fail\n");
        return MEMORY_ALLOC_FAIL;
    }

    while(1)
    {
        printf("Inside while loop\n");
        bytes=0;
        printf("Ioctl return value = %d\n", ioctl(fd,FIONREAD,&bytes));
        //        printf("%d ", ioctl(fd,FIONREAD,&bytes));

        printf("bytes after ioctl = %d\n", bytes);
        if(bytes<=0)
        {
            sleep(1);
            time_out++;
            if(time_out==TO)
            {
                printf("Break Timeout\n");
                res=READ_TIME_OUT;
                break;
            }
            continue;
        }

        printf("*len = %d\t bytes = %d\n", *len, bytes);
        if((*len)+bytes>alloc_bytes)
        {
            alloc_bytes+=bytes;
            buff=(CHAR*)malloc(alloc_bytes);
            if(buff==NULL)
            {
                res=MEMORY_ALLOC_FAIL;
                printf("Break2\n");
                break;
            }
        }
        printf("buff len to be added     %d\n", *len);

        ret=read(fd,buff,bytes);
        printf("ret from read is %d\n", ret);
        if(ret<0)
        {
            if(errno==EINTR)
                continue;
            printf("break 3\n");
            break;
        }
        else
        {
            *len=(*len)+ret;
            printf("Read len = %d\n", *len);
            //			printf("Response : ");
            for(X=0;X<(*len);X++)
            {
                O_buff[X]=buff[X];
                //printf("%02X %02x    ",buff[X],O_buff[X]);
            }
            O_buff[ret] = '\0';
            //			printf("\n");
            break;
        }
    }

    //	printf("O_buff === ");
    //	for(X=0; X<(*len); X++)
    //        printf("%02X ", O_buff[X]);

    if(*len<=0)
        return res;

    return SUCCESS;
}

