 
 
#ifndef _T1_H_
#define _T1_H_

/*
#ifndef TRUE
#define TRUE	1
#define FALSE 	0
#endif 
*/


#define BUFFMAX     260
#define MAX_ATR     34
#define HBSIZE      15
#define FWSIZE      5
#define ON          0x04
#define OFF         0x05
#define ERR_TIMEOUT -10
#define ERR_EDC     -11   
/* CTAPI - response codes                                                   */
#define OK               0             /* Successfull completion            */
#define ERR_INVALID     -1             /* Invalid parameter or value        */
#define ERR_CT          -8             /* Cardterminal error                */
#define ERR_TRANS       -10            /* Transmission error                */
#define ERR_MEMORY      -11            /* Memory allocate error             */
#define ERR_HOST        -127           /* Function aborted by host os       */
#define ERR_HTSI        -128           /* 'HTSI' error                      */                 


                                                                                                                            
/* Structure for handling one ECO5000                                        */
                                                                                                                            
#define MAX_ATR     34
#define HBSIZE      15
#define FWSIZE      5
#define MAX_READER  8
                                                                                                                            
#define SYNC_ICC    0x0000
#define ASYNC_ICC   0x0001
#define UNKNOWN     0x0002
                                                                                                                  
                                                                                                                              
//--------------------------new added feb_23_10
#define INVALID_PROTO   -200
#define INVAL_LEN       -201
//-------------------------------
                                                                                                                              
   
                                                                                                                 
typedef unsigned char   uchar8;
typedef unsigned int    uint32;
typedef unsigned long   ulong32;
typedef int             int32;
typedef long            long32;
                                                                                                                             
unsigned char rcv_buf_t1[BUFFMAX];      // used to cpy data rcvd by send_ioctl();
unsigned int gRcvd_bytes;               // used to copy data len rcvd by send_ioctl();

                                                                                                                             
# define BUFFMAXSIZE    300
                                                                                                                             
struct commproto_t
{
	uchar8 	Firmware[FWSIZE+1]; /* Holds Reader Firmware Version      */
    	uchar8  ATR[MAX_ATR];       /* Last ATR received from the card    */
    	uchar8  LenOfATR;           /* Length of ATR                      */
    	uchar8  NumOfHB;            /* Number of HB                       */
    	uchar8  HCC[HBSIZE];        /* History Bytes                      */
    	uchar8  Type;               /* Type of inserted Card              */
    	uchar8  Indirect;
    	uchar8  Specific;
    	int32  	AvailProt;          /* Protocols indicated in ATR         */
    	int32  	Protocol;           /* Handler module providing protocol  */
    	int32 	D;                  /* Baud rate conversion factor        */
    	int32  	DI;                 /* Indicated baud rate conversion     */
    	int32  	FI;                 /* Indicated clock conversion rate    */
    	uchar8  T0_WI;              /* Work waiting time index            */
    	uchar8  T1_IFSC;            /* Information field size             */
    	uchar8  T1_CWI;             /* Character waiting time index       */
    	uchar8  T1_BWI;             /* Block waiting time index           */
    	int32   (*CTModFunc)(struct commproto_t *,
                	uint32,    /* length of command  */
                        uchar8 *,  /* command            */
                        uint32 *,  /* length of response */
                        uchar8 *); /* response           */
    	union
    	{
//	        struct BP_T0_t      *t0;
        	struct BP_T1_t      *t1;
    	}Data;
};
                                                                                                                             
                                                                                                                             
typedef int (*CTModFunc_t) (struct commproto_t *,        /* specified ECO Data */
                            uint32,         /* length of command  */
                            uchar8 *,           /* command            */
                            uint32 *,       /* length of response */
                            uchar8 *);
                                                                                                                                                                                                                                                     
struct BP_T1_t
{
		
        uint32  CharWaitTime;           // Character Wait Time Integer
        uint32  BlockWaitTime;          // Block Wait Time Integer
        long32  WorkBWT;                // Working Block Wait Time in ETU
        int32   IFSC;                   // Maximum length of INF field
        int32   RSequenz;               // Receiver sequence number
        int32   SSequenz;               // Transmitter sequence number
        uchar8  Nad;                    // NAD byte of received block
        uchar8  Pcb;                    // PCB byte of received blocks
        int32   InBuffLength;           // Length of received data block
        uchar8  InBuff[BUFFMAXSIZE];    // Buffer for incoming data
};


int T1Init(struct commproto_t *ctx);

int T1Process(struct commproto_t *ctx,
                  uint32   lc, 
                  uchar8     *cmd, 
                  uint32 *lr,
                  uchar8     *rsp); 

int T1Term(struct commproto_t *ctx);
int T1Resync(struct commproto_t *ctx, int SrcNode, int DestNode);
int T1AbortChain(struct commproto_t *ctx, int SrcNode, int DestNode);

int Send_T1Data(struct commproto_t *ctx, int HostMode,int SrcNode, int DestNode, uchar8 *Buffer, int BuffLen, uchar8 *RcvBuff, int *RcvLen);
int Receive_T1Data(struct commproto_t *ctx,int SrcNode,int DestNode, uchar8 *RcvBuffer, int RcvBuffLen);

int Send_T1Block(uchar8 Nad,uchar8 Pcb, uchar8 *Buffer, int BuffLen);
int Receive_T1Block(struct commproto_t *ctx);
int Get_T1Block (struct commproto_t *ctx, int SrcNode, int DestNode);

int T1Transport(struct commproto_t *ctx,
                   int SrcNode,
                   int DestNode,
                   uchar8 *OBuffer,
                   int OBuffLen,
                   uchar8 *IBuffer,
                   int IBuffLen);


int T1Process (struct commproto_t *ctx,
                  unsigned int  lc,
                  unsigned char *cmd,
                  unsigned int  *lr,
                  unsigned char *rsp);



int SCR_SendReceiveT0(struct commproto_t *ctx,unsigned char *CAPDU_Data,unsigned int CLen,unsigned char *RAPDU_Data,int *RLen );
int SCR_SendReceiveT1(struct commproto_t *ctx,unsigned char *CAPDU_Data,unsigned int CLen,unsigned char *RAPDU_Data,int *RLen
);
//int SCR_SendCommand(unsigned char *CAPDU_Data,unsigned int CLen,unsigned char *RAPDU_Data,int *RLen );
//int SCR_SendCommand(struct commproto_t *ctx,unsigned char *CAPDU_Data,unsigned int CLen,unsigned char *RAPDU_Data,int *RLen );
int SCR_GetAtrParam(void);
int SCR_GetAtr(int *length, unsigned char *buffer);
//int SCR_PowerUp(void);
int SCR_PowerUp(unsigned char rst_type);
int SCR_CheckupCardPresence(void);
int SCR_Close(void);
int SCR_Open(void);

int Parse_T1Block(struct commproto_t *ctx,uchar8 nad,uchar8 pcb,int RLen,unsigned char *RDATA);
/**/
int GetATR(struct commproto_t *ctx);	// GetATR - fills structure vars (HCC, ATR, ...)  
                                                                                                                              
int Async_PowerOff(struct commproto_t *ctx);
                                                                                                                              
int ASync_GetATR(struct commproto_t *ctx);
                                                                                                                              
int Sync_GetATR(struct commproto_t *ctx);
/**/





#ifndef MAX
#define MAX(x,y)    ((x) > (y) ? (x) : (y))
#define MIN(x,y)    ((x) < (y) ? (x) : (y))
#endif



/* In chipcard communication protocols, the timing is based on ETUs          */
/* One ETU is the number of us it takes to transfer one bit                  */
#define ETU(baud)       (1000000L / (long)MAX(baud,38400))

/* Convert ETU timing into milliseconds                                      */
#define RSD(baud, time) (((1000000L / (long)MAX(baud,9600)) * time) / 1000L)

#define CWT     1920                    /* Timeout between 2 character 200ms */
#define BWT     9600                    /* Timeout between 2 blocks      1s  */
#define BLEN    32                      /* Initial length of block           */
#define RETRY   2                       /* Number of retries                 */

#define RERR_NONE       0x00            /* No error indicated in R-block     */
#define RERR_EDC        0x01            /* EDC error indicated in  R-block   */
#define RERR_OTHER      0x02            /* Other errors in R-block           */
#define RERR_MASK       0x0F            /* Mask to decode errors             */

#define SADMASK         0x07            /* Mask for source id                */
#define DADMASK         0x70            /* Mask for destination id           */

#define NONIBLOCK       0x80            /* Non I Block indicator             */
#define SBLOCK          0x40            /* S Block indicator                 */
#define NRBIT           0x10            /* N(R) indicator                    */
#define NSBIT           0x40            /* N(S) indicator                    */
#define MOREBIT         0x20            /* More bit indicator                */

#define SBLOCKFUNCMASK  0x3F            /* S-block function mask             */
#define RESYNCHREQ      0x00            /* S-block with RESYNCH request      */
#define RESYNCHRES      0x20            /* S-block with RESYNCH response     */
#define IFSREQ          0x01            /* S-block with IFS request          */
#define IFSRES          0x21            /* S-block with IFS response         */
#define ABORTREQ        0x02            /* S-block with ABORT request        */
#define ABORTRES        0x22            /* S-block with ABORT response       */
#define WTXREQ          0x03            /* S-block with WTX request          */
#define WTXRES          0x23            /* S-block with WTX response         */

#define VPPERRRES       0x24            /* S-block with VPPERR response      */

#define CODENAD(sad,dad)    (uchar8)(((dad & 0x0F) << 4) + (sad & 0x0F))
#define CODESBLOCK(x)       (uchar8)((x)|NONIBLOCK|SBLOCK)
#define CODERBLOCK(nr,rc)   (uchar8)(NONIBLOCK|((nr) << 4)|(rc))
#define CODEIBLOCK(ns,mb)   (uchar8)(((ns) << 6)|((mb) << 5))

#define ISIBLOCK(x)     (!((x) & NONIBLOCK))                    /* I block   */
#define ISRBLOCK(x)     (((x) & NONIBLOCK) && !((x) & SBLOCK))  /* R block   */
#define ISSBLOCK(x)     (((x) & NONIBLOCK) && ((x) & SBLOCK))   /* S block   */
#define NR(x)           (((x) & NRBIT) >> 4)        /* R-Block Sequence bit  */
#define NS(x)           (((x) & NSBIT) >> 6)        /* I-Block Sequence bit  */
#define RERR(x)         ((x) & RERR_MASK)           /* Error field in R-blck */
#define MORE(x)         (((x) & MOREBIT) >> 5)      /* More data bit         */
#define SBLOCKFUNC(x)   ((x) & SBLOCKFUNCMASK)      /* S-block function      */
#define SAD(x)          ((x) & SADMASK)             /* SAD from NAD byte     */
#define DAD(x)          (((x) & DADMASK) >> 4)      /* DAD from NAD byte     */



#endif







