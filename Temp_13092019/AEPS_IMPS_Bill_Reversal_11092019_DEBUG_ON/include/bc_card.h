
struct BC_3F00
{
	unsigned char CN[32];
    unsigned char AGENT_CODE[16];
    unsigned char MFI_CODE[8];
	unsigned char NAME[64];
	unsigned char MOTHER_NAME[64];
	unsigned char ADDRESS[128];
	unsigned char SEX[4];
	unsigned char DOB[16];
	unsigned char PI[4];
	unsigned char PID[32];
	unsigned char CID[16];
	unsigned char UID[32];
	unsigned char OPCODE[32];
	unsigned char OPSTATUS[4];
	unsigned char CARDEXP[16];
};

/*
struct PERINFO
{
    unsigned char CN[32];
    unsigned char NAME[64];
    unsigned char MOTHER_NAME[64];
    unsigned char ADDRESS[128];
    unsigned char SEX[4];
    unsigned char DOB[16];
    unsigned char PI[4];
    unsigned char PID[32];
    unsigned char CID[16];
    unsigned char UID[32];
    unsigned char OPCODE[32];
    unsigned char OPSTATUS[4];
    unsigned char CARDEXP[16];
};
*/
struct BC_BC04
{
    unsigned char CASHINH[32];
    unsigned char CASHINHP[32];
    unsigned char MAXMOC[8];
    unsigned char MAXMOP[8];
    unsigned char MAXMO[8];
    unsigned char MAXSYNC[8];
    unsigned char LASTSYNC[32];
    unsigned char MAXT[8];
    unsigned char TRANSYNC[8];
    unsigned char TURNSYNC[32];
    unsigned char MAXTO[32];
    unsigned char CARDNO[32];
};
/*struct DYNAMIC_DATA
{
    unsigned char CASHINH[16];
    unsigned char CASHINHP[16];
    unsigned char MAXMOC[4];
    unsigned char MAXMOP[4];
    unsigned char MAXMO[4];
    unsigned char MAXSYNC[4];//2
    unsigned char LASTSYNC[16];
    unsigned char MAXT[4];
    unsigned char TRANSYNC[4];
    unsigned char TURNSYNC[16];
    unsigned char MAXTO[16];
    unsigned char CARDNO[32];
};
*/

struct BC_BC05
{
    unsigned char TXNID[32];
	unsigned char CCARDNO[32];
	unsigned char ACCNO[32];
	unsigned char TYPE[4];
	unsigned char CASHIND[4];
	unsigned char TRCONTRA[32];
	unsigned char TXAMT[16];
	unsigned char TXNAR[32];
	unsigned char CLOBAL[16];
	unsigned char TXDATE[16];
	unsigned char TXTIME[16];
	unsigned char TID[16];
	unsigned char MO[4];
	unsigned char STX[4];
};
