#ifndef PACK_H
#define PACK_H

#include "dl_iso8583.h"
#include "dl_iso8583_defs_1993.h"
#include "dl_output.h"
#include <iostream>
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
#include<string.h>
#include<termios.h>
#include<errno.h>
#include <sys/signal.h>
#include <sys/ioctl.h>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <stdio.h>
#include <QFile>

#include "iso8583.h"

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
#define SERVERIP        "202.58.125.12"
#define SERVERPORT      "895"//"707"
#define READ_TIME_OUT   0x0021E

#define MEMORY_ALLOC_FAIL 	0x00216


#define UNPACK_FAILED -11

class IsoPackUnpack{
    public:
        IsoPackUnpack();
        ~IsoPackUnpack();
        int IsoPack_Unpack(ISO *isoBuf, int tranType);
        char * packField63(void);
        char * packField63Fusion(void);
        char * packField63Exception(void);
        char * packField126(void);
        char * packField127(void);
        int get_ip(char *gprsip);
        void Manipulate_DateTime(char *,char *,char *);
        int conv(char *str,int n, int pos);
        int getServerIpPortFromParamDownload(char *, char *);
        void vBCD2Ascii( unsigned char *pucSrc, unsigned char *pucData, int nSize);
        int hexToDec(char *);
        int Check_GPRS_Available();

};


class HostComm{

    public :
        HostComm(void);
        ~HostComm();
        INT open_connect(CHAR *,CHAR *);
        VOID close_connection();
        INT host_2Sendrecv(CHAR *,INT,CHAR *,INT *,INT );
        INT Write_Comm(UINT,CHAR *,INT );
        INT Read_Comm(UINT fd,INT *,CHAR *,INT);
};



#endif // PACK_H
