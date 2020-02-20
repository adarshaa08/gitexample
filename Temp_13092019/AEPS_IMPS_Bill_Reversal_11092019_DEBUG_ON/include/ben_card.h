#ifndef BEN_CARD_H
#define BEN_CARD_H

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#include <ctype.h>


#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


struct BEN_3F00
{
	char CARDNO[32];
	char NAME[64];
	char MOTHER_NAME[64];
	char SEX[4];
	char DOB[16];
	char LP[4];
	char PI[4];
	char PID[32];
	char CID[16];
	char UID[32];
};

struct BEN_3F04
{
	char ADDRESS[256];
	char LCN[32];
};

struct BEN_F100
{
	char ACCNO[32];
	char OPEN_DATE[16];
};

struct BEN_F104
{
	char ACSTATUS[4];
	char MODEOP[4];
	char CBALANCE[16];
	char ABALANCE[16];
	char DPOWER[16];
    char IRAC[4];
	char IS[8];
	char MOCOUNTER[4];
	char TRANSVOL[16];
	char CARDPROSTA[4];

};

struct BEN_F105
{
	char TXNID[32];
	char TYP[4];
	char TXAMT[16];
	char CASHIND[4];
	char TRCONTRA[32];
    char TXNAR[64];
	char TXDATE[16];
	char TXTIME[16];
	char TID[32];
	char MO[4];
	char STX[4];
	char CBALANCE[16];

};

struct BEN_3F06
{
    char CardIdNo[32];
    char CardExpDate[8];
    char MFI_Code[8];
    char AgentCode[8];
    char PersoStatus[4];
    char CardBlockedStaus[4];
    char CardMaxDailyLimit[16];
    char DailyWithDrawnAmt[16];
    char MaxWD_Limit[4];
    char WD_Counter[4];
    char CardMaxDailyDP_Lmt[16];
    char DailyDP_Amt[16];
    char MaxDP_Lmt[4];
    char DP_Counter[4];
    char TxnDate[16];
    char SyncTxnCounterLmt[4];
    char SyncTxnCounter[4];
    char UniquePostIssueTxnId[32];
};


#endif // BEN_CARD.H

