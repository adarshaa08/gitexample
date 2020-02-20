#ifndef DEFINES_H
#define DEFINES_H


#define WITHDRAWAL 1
#define DEPOSIT    2

#define BC_AUTH 0
#define BC_READ 1
#define BC_WRIT 2
#define BC_DREAD 3
#define BC_DWRIT 4
#define BC_CARD 5


#define BEN_AUTH 0
#define BEN_READ 1
#define BEN_WRIT 2
#define BEN_RAIF 3	// Account Info Read	
#define BEN_WAIF 4	// Account Info Write
#define BEN_PERS 5
#define BEN_PREAD 11

#define BEN_UPT 6
#define BEN_WRIT_FIRST 7
#define BEN_WRIT_NOUSE 8


#define CASH_AUTH 0
#define CASH_READ 1
#define CASH_WRIT 2

#define FUNC_OK 0
#define FUNC_KO -1
#define FUNC_ERROR -2
//#define PRINT_CONSOLE
#define BEN_MINIREAD 9


//#define MF      1
//#define BC00    2
//#define BC04    3
//#define BC05    4
//#define FP      5
//#define F100    6
//#define F104    7
//#define F105    8
//#define f3F04   9
//#define ACT_F105 10
////#define F303    11
////#define F305    12
#define FP_BC   313
#define FP_CUST 314

#define BC      1
#define CUST    2

#define AF105   1
#define AF100   2




//#define CSP_NAME        2
//#define CSP_MOTHER_NAME 3
//#define CSP_ADDR        4
//#define CSP_SEX         5
//#define CSP_PI          6
//#define CSP_PID         7
//#define CSP_UID         8
//#define CSP_OPCODE      9
//#define CSP_OPSTATUS    10
//#define CSP_CARDEXP     11

//#define BEN_CN          111
//#define BEN_NAME        112
//#define BEN_MOTHER_NAME 113
//#define BEN_ADDR        114
//#define BEN_SEX         115
//#define BEN_PI          116
//#define BEN_PID         117
//#define BEN_UID         118
//#define BEN_OPCODE      119
//#define BEN_OPSTATUS    120
//#define BEN_CARDEXP     121
//#define BEN_ACCNO       122

#define BEN_ACCINFO     123
//#define BEN_DOB         124
//#define CSP_DOB         125
//#define CSP_CID         126
//#define CSP_DYNAMIC     127

#define BC_TXN     	128
#define BEN_TXN     	129
#define BEN_DYNAMIC	130

#define NO_CONV 1
#define A2H     2
#define BCD2ASC 3






//#define TRUE 1;
//#define FALSE 0;
/**/
//typedef unsigned char BYTE;
typedef unsigned short int WORD;
typedef unsigned int DWORD;
//unsigned char SW1,SW2;
//unsigned short int SW1SW2,CStatus;
//unsigned char statusword1,statusword2;
//unsigned char CARD_TYPE;

//struct TEMPLATES
//{
//        unsigned char ID[4];
//        unsigned char Rating[6];
//        unsigned char FPData[1024];
//        int Len;
//}finger[4];

//unsigned char FINGER_DATA[7000];

#endif
