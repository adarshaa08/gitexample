#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<signal.h>
#include <sys/signal.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include <sys/ioctl.h>
#include<fcntl.h>
#include<termios.h>
#include <errno.h>
#include <sys/types.h>
#include<sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include<sys/time.h>
#include<time.h>
#include <net/if.h>
#include <dirent.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

//#define ENTER 15
//#define CANCEL 12
 
//#define RES32 4
//#define UprOffset 1
//#define MAX_MENU_ENTRIES 200
//#define MAX_ENTRY_LENGTH 30

#define FALSE 0
#define TRUE  1

////#define SAM  3
////#define CUST 2
////#define CSP  1


//#define KERNEL      1
//#define RAMDISK     2
//#define APPLICATION 3
                                                                                                                             
//#define ETHERNET    1
//#define RALINKWLAN  2
//#define CNETWLAN    3
//#define CDMA        4
//#define GSM_GPRS    5
//#define PSTN        6
//#define SERIAL      7

//#define SIGNOUT     0
//#define SIGNIN      1

#define  SUCCESS   0
#define  ERROR    -1
#define FAILED  -1
//#define DB_ERROR -2

//#define MODE_UNDEFINED 0
//#define MODE_ACQUIRE   1
//#define MODE_VERIFY    2
//#define MAX_USER       32
//#define MAX_PATH       256
#define RECORDLENGTH   200//107
                                                                                                                             

