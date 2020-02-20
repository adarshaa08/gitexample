#ifndef HOSTCOMM_H
#define HOSTCOMM_H

#include <QDebug>
#include <iostream>
#include <stdio.h>
#include<string.h>
#include <stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<unistd.h>
#include<time.h>
#include<sys/time.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<dlfcn.h>
#include<termios.h>
#include<errno.h>
#include<netdb.h>
#include <sys/signal.h>
#include <sys/ioctl.h>
//#include <QSqlQuery>
//#include <QSqlError>
#include <QVariant>
#include <stdio.h>
#include <QFile>

#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include<QHostInfo>


#define INT	int
#define VOID	void
#define CHAR	char
#define BYTE	unsigned char
#define BOOL	bool
#define LONG	long
#define DOUBLE	double
#define CONST	const
#define UINT	unsigned int
#define STRUCT	struct

#define SUCCESS         0
//#define SERVERIP        "202.58.125.2"
//#define SERVERPORT      "8048"//"707"
#define SERVERIP        "103.1.112.205"
#define SERVERPORT      "4444" //"9012"
#define READ_TIME_OUT   0x0021E

#define MEMORY_ALLOC_FAIL 	0x00216


#define UNPACK_FAILED -11


class HostComm{

    public :
        HostComm(void);
        ~HostComm();
        INT open_connect(CHAR *,CHAR *);
        VOID close_connection();
//        INT host_2Sendrecv(CHAR *,INT,CHAR *,INT *,INT );
        INT host_2Sendrecv(unsigned char *,INT,unsigned char *,INT *,INT ,INT );
        INT host_2Sendrecv_param(unsigned char *,INT,unsigned char *,INT *,INT );
        INT Write_Comm(UINT,unsigned char *,INT );
        INT Read_Comm(UINT fd,INT *,unsigned char *,INT,INT);

        INT Read_Comm_bkp(UINT fd,INT *len,unsigned char *O_buff,INT TO);
        INT iReadResponse(UINT fd,INT *len,unsigned char *O_buff,INT TO);
        INT inSendReceive(INT);

};


#endif // HOSTCOMM_H
