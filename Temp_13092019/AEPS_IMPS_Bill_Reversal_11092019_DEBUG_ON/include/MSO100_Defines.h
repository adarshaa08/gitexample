// The present software is not subject to the US Export Administration Regulations (no exportation license required), May 2012
#ifndef __MSO100_DEFINES_H
#define __MSO100_DEFINES_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/////////////////////////////////////////////////////////////////////
//global variables
/////////////////////////////////////////////////////////////////////
// enregistrement de callbacks sur les ILVs asynchrones

#define TEMPO_RECEIVE	100	// 100 ms

#define LogTrace(a, ...) do{}while(0)

#define LOCK(i_h_Mutex)		if(i_h_Mutex != NULL)							\
						WaitForSingleObject(i_h_Mutex,INFINITE);

#define UNLOCK(i_h_Mutex)	if(i_h_Mutex != NULL)							\
						ReleaseMutex(i_h_Mutex);


I MSO_SpUsb_LoadLibrary(void);
I MSO_SpRs232_LoadLibrary(void);

typedef struct
{
	B				m_b_Initialised;
	C				m_str_Interface[50];

	MORPHO_HANDLE	m_h_Handle;	

	HANDLE			g_h_MutexFrame;
	HANDLE			g_h_MutexILV;
	I				g_i_Stop;
	I				g_i_EmbeddedError;
#ifndef MA2G
	PT_SECU_BUF		g_px_MsoCertificate;
	UL				g_ul_AntiReplayCmpt;		// anti-replay counter
#endif

	T_MORPHO_CALLBACK_COM_OPEN					g_pf_CallbackComOpen;		
	T_MORPHO_CALLBACK_COM_SEND					g_pf_CallbackComSend;	
	T_MORPHO_CALLBACK_COM_RECEIVE				g_pf_CallbackComReceive;	
	T_MORPHO_CALLBACK_COM_RECEIVE_FREE			g_pf_CallbackComReceiveFree;
	T_MORPHO_CALLBACK_COM_CLOSE					g_pf_CallbackComClose;

#ifndef MA2G
	T_MORPHO_CALLBACK_TRIPLE_DES_SIGN			g_pf_CallbackTripleDesSign;
	T_MORPHO_CALLBACK_TRIPLE_DES_CRYPT			g_pf_CallbackTripleDesCrypt;
	T_MORPHO_CALLBACK_TRIPLE_DES_VERIF_SIGN		g_pf_CallbackTripleDesVerifSign;
	T_MORPHO_CALLBACK_TRIPLE_DES_DECRYPT		g_pf_CallbackTripleDesDecrypt;

	T_MORPHO_CALLBACK_RAND						g_pf_CallbackGenRandom;
	T_MORPHO_CALLBACK_VERIF_OFFERED_SECU_SIGN	g_pf_CallbackVerifOfferedSecuritySignature;

	UC				g_auc_hash[SHS_DIGEST_SIZE];
#endif
	I				g_i_nb_RecordedILV;
	T_ILV_BUFFER	g_px_IlvTable[NB_RECORDED_ILV];

} T_MSO100_STRUCT, *PT_MSO100_STRUCT;


typedef struct
{
	HMODULE m_h_SpInstance;

	T_MORPHO_CALLBACK_COM_OPEN			m_pf_Sp_OpenEx;
	T_MORPHO_CALLBACK_COM_SEND			m_pf_Sp_WriteFrame;
	T_MORPHO_CALLBACK_COM_RECEIVE		m_pf_Sp_ReadFrame;
	T_MORPHO_CALLBACK_COM_RECEIVE_FREE	m_pf_Sp_ReadFrameReleaseEx;
	T_MORPHO_CALLBACK_COM_CLOSE			m_pf_Sp_Close;

} T_MSO100_SP_MODULE_STRUCT, *PT_MSO100_SP_MODULE_STRUCT;

typedef int (*MyPROC)();


typedef struct
{
	T_MSO100_SP_MODULE_STRUCT	m_x_Sp_Struct;

#ifndef MA2G
	T_SPUSB_ENUMDEVICES m_pf_SpUsb_EnumDevices;
	T_SPUSB_RELEASEENUMDEVICES m_pf_SpUsb_ReleaseEnumDevices;
	T_SPUSB_SERVERINFOS m_pf_SpUsb_ServerInfos;
	T_SPUSB_SERVERINFOSRELEASE m_pf_SpUsb_ServerInfosRelease;
	T_SPUSB_LOCK m_pf_SpUsb_Lock;
	T_SPUSB_UNLOCK m_pf_SpUsb_UnLock;
	T_SPUSB_WAITFORREBOOT m_pf_SpUsb_WaitForReboot;
#endif
	
} T_MSO100_SPUSB_MODULE_STRUCT, *PT_MSO100_SPUSB_MODULE_STRUCT;

extern PT_MSO100_SPUSB_MODULE_STRUCT	g_px_SpUsb_Module_Struct;


typedef struct
{
	T_MSO100_SP_MODULE_STRUCT	m_x_Sp_Struct;

	MyPROC m_pf_SPRS232_NegociateBaudRate;
	MyPROC m_pf_SPRS232_Break;
	MyPROC m_pf_SPRS232_Write;
	MyPROC m_pf_SPRS232_Read;
	MyPROC m_pf_SPRS232_GetConfig;
	MyPROC m_pf_SPRS232_Initialize;
	MyPROC m_pf_SPRS232_Close;
	MyPROC m_pf_SPRS232_SetBaudRate;
	MyPROC m_pf_SPRS232_Send;
	MyPROC m_pf_SPRS232_Receive;
	MyPROC m_pf_SPRS232_ConfigUart;

} T_MSO100_SPRS232_MODULE_STRUCT, *PT_MSO100_SPRS232_MODULE_STRUCT;

extern PT_MSO100_SPRS232_MODULE_STRUCT	g_px_SpRs232_Module_Struct;


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
