// The present software is not subject to the US Export Administration Regulations (no exportation license required), May 2012
#ifndef __MSO100_H
#define __MSO100_H

#ifdef MSO100_EXPORTS
#define MSO100_API __declspec(dllexport)
#else
#ifndef _MSO_LIB
#define MSO100_API __declspec(dllimport)
#else
#define MSO100_API
#endif
#endif

#include "MORPHO_Types.h"
#include "MORPHO_Types_Ext.h"
#include "MSO_types.h"
#include "MORPHO_RegisterKeyDef.h"
#include "Crypto.h"
#include "BioDB_Types.h"
#include "Ilv_Struct.h"
#ifdef _ANDROID_USB
#include "jni.h"
#include <android/log.h>
#endif


typedef I	(*T_pFuncILV_Buffer)(PVOID,I,PVOID);

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

//CampaignFVP
#define FP_ROI_IMAGE_HEADER_SIZE 208
#define MAX_FP_ROI_IMAGE_COUNT 25

#define TEMPLATE_BUFFER_MAX_SIZE    5120
#define ILV_SENDBUFFER_SIZE			8192

#define READ_TIMEOUT				15000	//augmentation du timeout (read/write) pour attente de la lecture EEPROM sur soft outillage CBI lors d'un download soft
#define WRITE_TIMEOUT				15000
#define IDENT_TIMEOUT				10000	//wait timeout for identification process
											//50 ms * 200 matching

#ifndef MA2G
#define NB_RECORDED_ILV				30
#else
#define NB_RECORDED_ILV				0		// We can ignore asynchronous ILVs in the QuickLoader
#endif
#define SPUSB_READ_TIMEOUT			15000
#define SPUSB_WRITE_TIMEOUT			15000

//////////////////////////////////////////////////////////

#ifdef __linux__
typedef struct
{
#ifdef _ANDROID_USB
		jobject* m_device_object;
#else
		struct usb_device   *m_px_device;
#endif
        PUC                 m_puc_SerialNumber;
        PUC                 m_puc_FriendlyName;
        UC                  m_uc_Index;
        PUC                 m_puc_DevicePath;
}T_MSO_USB_DEVICE_PROPERTIES, *PT_MSO_USB_DEVICE_PROPERTIES;

#else
typedef struct _T_MSO_USB_DEVICE_PROPERTIES
{
	PUC		m_puc_InstanceId;
	PUC		m_puc_SerialNumber;
	PUC		m_puc_FriendlyName;
	PUC		m_puc_DevicePath;
}T_MSO_USB_DEVICE_PROPERTIES, *PT_MSO_USB_DEVICE_PROPERTIES;
#endif

typedef struct
{
    US					m_us_Ident;			/* Identifier that will trigger callback function */
	T_pFuncILV_Buffer	m_x_Callback;		/* callback function */
	PVOID				m_pv_Context;
}T_ILV_BUFFER;


typedef struct
{
	PT_BUF		io_px_BiometricData;
	UC			i_uc_ExportMinutiae;
	UC			pad[3];
}T_EXPORT_PK, *PT_EXPORT_PK;

typedef struct
{
	PT_BUF		l_px_CaptureFingerImages;
	PT_BUF		l_px_CaptureVeinImages;
	UC			l_uc_nbFingerImages;
	UC			l_uc_nbVeinImages;
	UC			pad[2];
}T_SIMU_CAMERA_MODALITY, *PT_SIMU_CAMERA_MODALITY;

typedef struct
{
	PT_BUF		io_px_BufImage;
	UC			i_uc_ExportImageType;
	UC			i_uc_CompressionType;
	UC			i_uc_CompressionRate;
	UC          pad[1];
}T_EXPORT_IMAGE, *PT_EXPORT_IMAGE;

typedef struct
{
	PUC			o_puc_DiversificationData;
	UL			o_ul_DiversificationDataSize;
	UC			i_uc_IsBioDataEncryption;
	UC          pad[3];
}T_BIO_DATA_ENCRYPTION, *PT_BIO_DATA_ENCRYPTION;

typedef struct
{
	I			i_i_Token[2];	
	PT_BUF		o_px_ScrambledFFDImage;
	I			o_i_Seed[2];
	UC			i_uc_IsEnableCS;
	UC          pad[3];
}T_EXPORT_IMAGE_FFD, *PT_EXPORT_IMAGE_FFD;

typedef struct
{
	I						m_i_ffd_security_level;
	I						m_i_Algo;
	UL						m_ul_DetectModeChoice;
	UL						m_ul_DetectModeDefault;
	UL						m_ul_MatcherMode;
	UL						m_ul_asynchronous_evt;
	PUL						m_pul_score;
	PT_EXPORT_IDENT_TIMERS	m_px_Timers;
	UC						m_uc_LatentDetection;
	UC						m_uc_StrategyAcquisitionMode; // FVP 1.10
	UC                      m_uc_ExportBestFPImage; //CampaignFVP
	UC						m_auc_Pad;
}T_BIO_PARAM_PC,*PT_BIO_PARAM_PC;

//CampaignFVP
typedef struct
{
	UL						m_ul_TimeStamp;
	T_BUF					m_x_Image_Data;
	UC                      m_auc_Image_Header[FP_ROI_IMAGE_HEADER_SIZE];
}T_FP_ROI_IMAGE,*PT_FP_ROI_IMAGE;

//CampaignFVP
typedef struct
{
	I						m_i_Trailing_Count;
	I						m_i_Best_Image_Ind; // 0 to n
	I						m_i_Nb_Images;
	PT_FP_ROI_IMAGE			m_px_FP_ROI_Image[MAX_FP_ROI_IMAGE_COUNT];
}T_FP_ROI_IMAGE_ARR,*PT_FP_ROI_IMAGE_ARR;

/////////////////////////////////////////////////////////////////////
MSO100_API
I
MSO_InitCom
(
	MORPHO_HANDLE	*i_ph_Mso100Handle,
	PC				i_str_Interface,
	PC				i_str_comName,
	I				i_i_baudRate
 );

/////////////////////////////////////////////////////////////////////
MSO100_API
I
MSO_NegociateBaudRate
(
	MORPHO_HANDLE	*i_h_Mso100Handle,
	PC		i_pc_String,
	UL		i_ul_Param,
	PVOID	io_pv_Param
);

/////////////////////////////////////////////////////////////////////
MSO100_API I MSO_Free(PUC *i_ppuc_Buf);

MSO100_API void *MSO_Malloc( size_t size );

/////////////////////////////////////////////////////////////////////
typedef I (F_MSO_COM_OPEN) (
	MORPHO_HANDLE						*o_ph_Mso100Handle,
	T_MORPHO_CALLBACK_COM_OPEN			i_pf_CallbackComOpen,
	T_MORPHO_CALLBACK_COM_SEND			i_pf_CallbackComSend,
	T_MORPHO_CALLBACK_COM_RECEIVE		i_pf_CallbackComReceive,
	T_MORPHO_CALLBACK_COM_RECEIVE_FREE	i_pf_CallbackComReceiveFree,
	T_MORPHO_CALLBACK_COM_CLOSE			i_pf_CallbackComClose,
	PC									i_pc_String,
	UL									i_ul_Param,
	PVOID								io_pv_param
);
typedef F_MSO_COM_OPEN *PF_MSO_COM_OPEN;


MSO100_API
F_MSO_COM_OPEN MSO_ComOpen;

/////////////////////////////////////////////////////////////////////
MSO100_API
I
MSO_ComSend
(
	MORPHO_HANDLE	i_h_Mso100Handle,
	UL		i_ul_Timeout,
	PUC		i_puc_Data,
	UL		i_ul_DataSize
);

/////////////////////////////////////////////////////////////////////
MSO100_API
I
MSO_ComReceive
(
	MORPHO_HANDLE	i_h_Mso100Handle,
	UL		i_ul_Timeout,
	PUC*	io_ppuc_Data,
	PUL		o_pul_DataSize
);

/////////////////////////////////////////////////////////////////////
MSO100_API
I
MSO_ComReceiveFree
(
	MORPHO_HANDLE	i_h_Mso100Handle,
	PUC*	io_ppuc_Data
);

/////////////////////////////////////////////////////////////////////
typedef I (F_MSO_COM_CLOSE) ( MORPHO_HANDLE	*o_ph_Mso100Handle);
typedef F_MSO_COM_CLOSE *PF_MSO_COM_CLOSE;

MSO100_API F_MSO_COM_CLOSE MSO_CloseCom;

/////////////////////////////////////////////////////////////////////
MSO100_API
I
MSO_SendReceive
(
	MORPHO_HANDLE	i_h_Mso100Handle,
	US		i_us_I,			// Identifier
	PUC		i_puc_Frame,	// data to send
	UL		i_ul_len,		// number of bytes to write
	PUC		*o_puc_dataOut,	// response buffer
	PUL		o_pul_len,		// response length
	PUC		*o_ppuc_Data,	// ILV data
	PUC		o_puc_ILV_Status,
	PI		o_pi_EmbeddedError
);

/////////////////////////////////////////////////////////////////////
MSO100_API
I
MSO_SendReceiveTimeout
(
	MORPHO_HANDLE	i_h_Mso100Handle,
	UL		i_ul_TransmitTimeout,	// reception timeout, only for USB (ms)
	UL		i_ul_ReceiveTimeout,	// reception timeout (ms)
	US		i_us_I,					// Identifier
	PUC		i_puc_Frame,			// data to send
	UL		i_ul_len,				// number of bytes to write
	PUC		*o_ppuc_dataOut,		// response buffer
	PUL		o_pul_len,				// response length
	PUC		*o_ppuc_Data,			// ILV data
	PUC		o_puc_ILV_Status,
	PI		o_pi_EmbeddedError
);

/////////////////////////////////////////////////////////////////////
MSO100_API
void
MSO_FrameRelease
(
	MORPHO_HANDLE	i_h_Mso100Handle,
	PUC *l_puc_Buffer
);


/////////////////////////////////////////////////////////////////////
MSO100_API
I
MSO_GetLastEmbeddedError
(
	MORPHO_HANDLE	i_h_Mso100Handle
);

/////////////////////////////////////////////////////////////////////
typedef I (F_MSO_CHANGERUNLEVELANDWAITFORREBOOT) (
	MORPHO_HANDLE			i_h_Mso100Handle,
	UC						i_uc_RunLevel,
	PC						i_str_Interface,
	PC						i_str_comName,
	I						i_i_TimeOut
);
typedef F_MSO_CHANGERUNLEVELANDWAITFORREBOOT *PF_MSO_CHANGERUNLEVELANDWAITFORREBOOT;

MSO100_API F_MSO_CHANGERUNLEVELANDWAITFORREBOOT MSO_ChangeRunLevelAndWaitForReboot;


/////////////////////////////////////////////////////////////////////
MSO100_API
I
MSO_Usb_EnumDevices
(
	PT_MSO_USB_DEVICE_PROPERTIES	*o_ppx_DeviceProperties,
	UL								*o_pul_NbreDevices
);

/////////////////////////////////////////////////////////////////////
MSO100_API
I
MSO_Usb_ReleaseEnumDevices
(
	PT_MSO_USB_DEVICE_PROPERTIES	*o_ppx_DeviceProperties,
	UL								i_ul_NbreDevices
);

/////////////////////////////////////////////////////////////////////
MSO100_API
I
MSO_Usb_ServerInfos
(
	UL	i_ul_InfoType,
	UL	i_ul_InfoParam,
	UC 	**io_ppuc_Data,
	UL	*o_pul_DataSize
);
#define USB_SERVERINFOS_INFO_TYPE_VERSION	0

/////////////////////////////////////////////////////////////////////
MSO100_API
I
MSO_Usb_ServerInfosRelease
(
	UC 	**io_ppuc_Data
);

/////////////////////////////////////////////////////////////////////
MSO100_API
I
MSO_Usb_Lock
(
	MORPHO_HANDLE		i_h_Mso100Handle,
	UL					i_ul_Timeout
);

/////////////////////////////////////////////////////////////////////
MSO100_API
I
MSO_Usb_UnLock
(
	MORPHO_HANDLE		i_h_Mso100Handle
);

/////////////////////////////////////////////////////////////////////
MSO100_API
I
MSO_RebootSoft
(
	MORPHO_HANDLE	i_h_Mso100Handle,
	PUC	o_puc_ILV_Status
);

/////////////////////////////////////////////////////////////////////
MSO100_API
I
MSO_WaitForReboot
(
	MORPHO_HANDLE	i_h_Mso100Handle,
	UL				i_ul_WriteTimeout,
	PUC				i_puc_Data,
	UL				i_ul_DataSize,
	UL				i_ul_WaitTimeout
);

/////////////////////////////////////////////////////////////////////
MSO100_API
I
MSO_ChangeRunLevel
(
	MORPHO_HANDLE	i_h_Mso100Handle,
	UC		i_uc_RunLevel
);
/////////////////////////////////////////////////////////////////////
MSO100_API
I
MSO_SmartChangeRunLevel
(
	MORPHO_HANDLE			i_h_Mso100Handle,
	UC						i_uc_RunLevel,
	PC						i_str_Interface,
	PC						i_str_comName,
	I						i_i_TimeOut
);

/////////////////////////////////////////////////////////////////////
MSO100_API
I
MSO_GetRunLevel
(
	MORPHO_HANDLE	i_h_Mso100Handle,
	PUC	i_puc_RunLevel,
	PUC	o_puc_ILV_Status
);

/////////////////////////////////////////////////////////////////////
MSO100_API
I
MSO_GetDescriptor
(
	MORPHO_HANDLE	i_h_Mso100Handle,
	UC				i_uc_DescFormat,
	PUC				o_puc_ILV_Status,
	VOID*			i_pv_DescProduct,
	UL				i_ul_SizeOfDescProduct,
	VOID*			i_pv_DescSensor,
	UL				i_ul_SizeOfDescSensor,
	VOID*			i_pv_DescSoftware,
	UL				i_ul_SizeOfDescSoftware
);
//////////////////////////////////////////////////////////////////////////////////
MSO100_API
I
MSO_GetMaxUser
(
	MORPHO_HANDLE	i_h_Mso100Handle,
	PUC		o_puc_ILV_Status,
	PUS		o_pus_MaxUser);
//////////////////////////////////////////////////////////////////
MSO100_API
I
MSO_GetMaxDataBase
(
	MORPHO_HANDLE	i_h_Mso100Handle,
	PUC		o_puc_ILV_Status,
	PUS		o_pus_MaxDataBase);
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
MSO100_API
I
MSO_GetDescriptorBin
(
	MORPHO_HANDLE			i_h_Mso100Handle,
	UC						i_uc_DescFormat,
	PUC						o_puc_ILV_Status,
	PC						o_pc_BinDesc,
	PUL						io_pul_SizeOfBinDesc
);
/////////////////////////////////////////////////////////////////////
MSO100_API
I
MSO_GetDescriptorBinValue
(
	MORPHO_HANDLE			i_h_Mso100Handle,
	UC						i_uc_DescFormat,
	PUC						o_puc_ILV_Status,
	PUL						o_pul_BinDesc
);
/////////////////////////////////////////////////////////////////////
MSO100_API
I
MSO_GetLOG
(
	MORPHO_HANDLE	i_h_Mso100Handle,
	UC				i_uc_Type,	//0=RAM - 1=FLASH
	PUC				o_puc_Log,
	UL				i_ul_MaxLogSize,
	PUL				o_pul_LogSize,
	PUC				o_puc_ILV_Status
);

/////////////////////////////////////////////////////////////////////
MSO100_API
I
MSO_RegisterAsyncILV
(
	MORPHO_HANDLE		i_h_Mso100Handle,
	I					i_us_I,
	T_pFuncILV_Buffer	i_p_Callback,
	PVOID				i_pv_context
);

/////////////////////////////////////////////////////////////////////
MSO100_API
I
MSO_UnregisterAsyncILV
(
	MORPHO_HANDLE		i_h_Mso100Handle,
	I					i_us_I
);

/////////////////////////////////////////////////////////////////////
MSO100_API
I
MSO_UnregisterAllAsyncILV
(
	MORPHO_HANDLE		i_h_Mso100Handle
);

/////////////////////////////////////////////////////////////////////
MSO100_API
I
MSO_RunAsyncILV
(
	MORPHO_HANDLE		i_h_Mso100Handle,
	US					i_us_Ident,
	PUC					i_puc_dataOut
);

/////////////////////////////////////////////////////////////////////
MSO100_API
I
MSO_Sensor_Light (
	MORPHO_HANDLE	i_h_Mso100Handle,
	US				i_us_Timeout,
	UL				i_ul_AsynchronousEvent,
	PUC				o_puc_ILV_Status
);

/////////////////////////////////////////////////////////////////////
// DataBase
/////////////////////////////////////////////////////////////////////
MSO100_API
I
MSO_BioDB_CreateDb (
	MORPHO_HANDLE			i_h_Mso100Handle,
	UC 		i_uc_IndexDB,
	US		i_us_UserMax,
	UC		i_uc_NbFinger,
	UC		i_uc_NormalizedPK_Type,
	UC		i_uc_NbAddField,
	UC		i_uc_encryptedDB,
	PT_FIELD i_px_AddField,
	PUC		o_puc_ILV_Status
);

/////////////////////////////////////////////////////////////////////
MSO100_API
I
MSO_BioDB_DestroyDb (
	MORPHO_HANDLE			i_h_Mso100Handle,
	UC 		i_uc_IndexDB,
	PUC		o_puc_ILV_Status,
	PUL		o_pul_EmbeddedError
);

/////////////////////////////////////////////////////////////////////
MSO100_API
I
MSO_BioDbAddBaseRecord_FingerVein(
	MORPHO_HANDLE	i_h_Mso100Handle,
	UC	 			i_uc_IndexDB,
	UC				i_uc_NbPk,
	PT_BUF_PK		i_px_Pk,
	PT_BUF			i_px_UserId,
	UC				i_uc_NbAddField,
	PT_BUF			i_px_AddField,
	UL				i_ul_pkUpdateMask,
	PUL				o_pul_IndexUser,
	PUC				o_puc_ILV_Status,
	PUC				o_puc_Base_Status,
	BOOL			i_b_NoCheckOnTemplate
);

/////////////////////////////////////////////////////////////////////
MSO100_API
I
MSO_BioDB_DeleteUser (
	MORPHO_HANDLE			i_h_Mso100Handle,
	UC 		i_uc_IndexDB,
	UL		i_ul_IndexUser,
	PUC		o_puc_ILV_Status,
	PUL		o_pul_EmbeddedError
);

/////////////////////////////////////////////////////////////////////
MSO100_API
I
MSO_BioDB_GetData (
	MORPHO_HANDLE			i_h_Mso100Handle,
	UC 		i_uc_IndexDB,
	UL		i_ul_IndexUser,
	UL		i_ul_UidData,
	PUL		io_pul_SizeData,
	PUC		o_puc_Data,
	PUC		o_puc_ILV_Status,
	PUL		o_pul_EmbeddedError
);

/////////////////////////////////////////////////////////////////////
/*MSO100_API
I
MSO_BioDB_GetBaseConfig (
	MORPHO_HANDLE			i_h_Mso100Handle,
	UC 		i_uc_IndexDB,
	PUC		o_puc_FingerNb,
	PUL		o_pul_MaxRecord,
	PUL		o_pul_CurrentRecord,
	PUC		o_puc_EncrytedDB,
	PUL		io_pul_AddFieldNb,
	PT_FIELD o_px_AddField,
	PUC		o_puc_NormalizedPK_Type,
	PUC		o_puc_ILV_Status,
	PUL		o_pul_EmbeddedError
);
*/
/////////////////////////////////////////////////////////////////////
MSO100_API
I
MSO_BioDB_EraseDb (
	MORPHO_HANDLE			i_h_Mso100Handle,
	UC 		i_uc_IndexDB,
	PUC		o_puc_ILV_Status,
	PUL		o_pul_EmbeddedError
);

/////////////////////////////////////////////////////////////////////
MSO100_API
I
MSO_BioDB_FindUser (
	MORPHO_HANDLE			i_h_Mso100Handle,
	UC 		i_uc_IndexDB,
	UL		i_ul_UidData,
	UL		i_ul_SizeData,
	PUC		i_puc_Data,
	PUL		io_pul_IndexUser,
	PUC		o_puc_ILV_Status,
	PUL		o_pul_EmbeddedError
);

/////////////////////////////////////////////////////////////////////
// Security
/////////////////////////////////////////////////////////////////////

#ifdef ENABLE_SECU_ILV

MSO100_API
I
MSO_OfferedSecurityOpen
(
	MORPHO_HANDLE								i_h_Mso100Handle,
	T_MORPHO_CALLBACK_RAND						i_pf_CallbackGenRandom,
	T_MORPHO_CALLBACK_VERIF_OFFERED_SECU_SIGN	i_pf_CallbackVerifOfferedSecuritySignature,
	T_MORPHO_CALLBACK_VERIF_CERTIF				i_pf_CallbackVerifCertif
);

/////////////////////////////////////////////////////////////////////
MSO100_API
I
MSO_OfferedSecurityClose(
	MORPHO_HANDLE								i_h_Mso100Handle
);

/////////////////////////////////////////////////////////////////////
MSO100_API
I
MSO_TunnelingOpen(
	MORPHO_HANDLE							i_h_Mso100Handle,
	T_MORPHO_CALLBACK_TRIPLE_DES_SIGN		i_pf_CallbackTripleDesSign,
	T_MORPHO_CALLBACK_TRIPLE_DES_CRYPT		i_pf_CallbackTripleDesCrypt,
	T_MORPHO_CALLBACK_TRIPLE_DES_VERIF_SIGN	i_pf_CallbackTripleDesVerifSign,
	T_MORPHO_CALLBACK_TRIPLE_DES_DECRYPT	i_pf_CallbackTripleDesDecrypt,
	T_MORPHO_CALLBACK_TUNNELING_INIT1		i_pf_CallbackTunnelingInit1,
	T_MORPHO_CALLBACK_TUNNELING_INIT2		i_pf_CallbackTunnelingInit2,
	UL										i_ul_HostCertificateSize,
	PUC										i_puc_HostCertificate,
	PUC										o_puc_ILV_Status,
	PUL										o_pul_EmbeddedError
);

/////////////////////////////////////////////////////////////////////
MSO100_API
I
MSO_TunnelingClose(
	MORPHO_HANDLE			i_h_Mso100Handle
);

/////////////////////////////////////////////////////////////////////
MSO100_API
I
MSO_SecuReadCertificate
(
	MORPHO_HANDLE	i_h_Mso100Handle,
	UC				i_uc_index,
	PT_SECU_BUF		o_ps_Certif,
	PUC				o_puc_ILV_Status,
	PUL				o_pul_EmbeddedError
);

MSO100_API
I
MSO_SecuReadCertificateFree(PT_SECU_BUF	io_ps_Certif);

/////////////////////////////////////////////////////////////////////
MSO100_API
I
MSO_SECU_StoPkcs12(
	MORPHO_HANDLE	i_h_Mso100Handle,
	T_SECU_BUF		i_ps_clePub,
	I				i_i_AntiReplayCnt,
	PUC				o_puc_ILV_Status,
	PUL				o_pul_EmbeddedError
);

/////////////////////////////////////////////////////////////////////
MSO100_API
I
MSO_SECU_StoCertif(
	MORPHO_HANDLE	i_h_Mso100Handle,
	PT_SECU_BUF		i_ps_certif,
	PUC				o_puc_ILV_Status,
	PUL				o_pul_EmbeddedError
);

/////////////////////////////////////////////////////////////////////
MSO100_API
I
MSO_SECU_MutualAuthInit1
(
	MORPHO_HANDLE			i_h_Mso100Handle,
	T_SECU_BUF				l_x_certifHost,
	PT_TokenInitAuthOut1	o_ps_tokenInitAuthOut1,
	PUC						o_puc_ILV_Status,
	PUL						o_pul_EmbeddedError
);

/////////////////////////////////////////////////////////////////////
MSO100_API
I
MSO_SECU_MutualAuthInit2
(
	MORPHO_HANDLE			i_h_Mso100Handle,
	PT_TokenInitAuthIn2		i_ps_tokenInitAuthIn2,
	PT_TokenInitAuthOut2	o_ps_tokenInitAuthOut2,
	PUC						o_puc_ILV_Status,
	PUL						o_pul_EmbeddedError
);


MSO100_API
I
MSO_SECU_PresPWD
(
	MORPHO_HANDLE	i_h_Mso100Handle,
	UC		i_puc_pwd[DES_TRIPLE_KEY_LEN],
	PUC		o_puc_ILV_Status,
	PUL		o_pul_EmbeddedError
);


/////////////////////////////////////////////////////////////////////
MSO100_API
I
MSO_SECU_LoadOptions
(
	MORPHO_HANDLE	i_h_Mso100Handle,
	UC		i_uc_option,
	PUC		o_puc_ILV_Status,
	PUL		o_pul_EmbeddedError
);

/////////////////////////////////////////////////////////////////////
MSO100_API
I
MSO_SECU_PresKT1
(
	MORPHO_HANDLE	i_h_Mso100Handle,
	UC		o_puc_Random[LEN_RAND_TRANSP],
	UC		o_puc_SerialNumber[MSO_SERIAL_NUMBER_LEN],
	PUC		o_puc_ILV_Status,
	PUL		o_pul_EmbeddedError
);

/////////////////////////////////////////////////////////////////////
MSO100_API
I
MSO_SECU_PresKT2
(
	MORPHO_HANDLE	i_h_Mso100Handle,
	UC		i_puc_CryptogrammeIn[2*LEN_RAND_TRANSP+MSO_SERIAL_NUMBER_LEN],
	UC		o_puc_CryptogrammeOut[2*LEN_RAND_TRANSP],
	PUC		o_puc_ILV_Status,
	PUL		o_pul_EmbeddedError
);

/////////////////////////////////////////////////////////////////////
MSO100_API
I
MSO_SECU_ChangeKT
(
	MORPHO_HANDLE	i_h_Mso100Handle,
	UC		i_puc_KT[DES_TRIPLE_KEY_LEN],
	PUC		o_puc_ILV_Status,
	PUL		o_pul_EmbeddedError
);

/////////////////////////////////////////////////////////////////////
MSO100_API
I
MSO_SetMinFar
(
	MORPHO_HANDLE	i_h_Mso100Handle,
	US		i_us_MinFar,
	PUC		o_puc_ILV_Status
);

/////////////////////////////////////////////////////////////////////
MSO100_API
I
MSO_SetMinSL
(
	MORPHO_HANDLE	i_h_Mso100Handle,
	UL				i_ul_MinSL,
	PUC				o_puc_ILV_Status
);

/////////////////////////////////////////////////////////////////////
MSO100_API
I
MSO_SECU_ResetPkcs12Counter
(
	MORPHO_HANDLE	i_h_Mso100Handle,
	PUC		o_puc_ILV_Status,
	PUL		o_pul_EmbeddedError
);
#endif

/////////////////////////////////////////////////////////////////////
MSO100_API
I
MSO_SECU_GetSerialNumber(
	MORPHO_HANDLE	i_h_Mso100Handle,
	UC		o_auc_SerialNumber[MSO_SERIAL_NUMBER_LEN],
	PUC		o_puc_SecuConfig,
	PUS		o_pus_SecuMaxFAR,
	PUL		o_pul_SecuMinMSL,
	PUC		o_puc_ILV_Status
);

/////////////////////////////////////////////////////////////////////
MSO100_API
I
MSO_SECU_GetConfig(
	MORPHO_HANDLE	i_h_Mso100Handle,
	UC		o_auc_SerialNumber[MSO_SERIAL_NUMBER_LEN],
	PUC		o_puc_SecuConfig,
	PUS		o_pus_SecuMaxFAR,
	PUL		o_pul_SecuMinMSL,
	PUC		o_puc_ILV_Status
);

/////////////////////////////////////////////////////////////////////
// Enroll
/////////////////////////////////////////////////////////////////////
MSO100_API
I
MSO_Bio_GetImageIndex(
			MORPHO_HANDLE		i_h_Mso100Handle,
			UC 					i_uc_ImageIndex,
			PT_EXPORT_IMAGE		io_px_ExportImage,
			PUC					o_puc_ILV_Status
);


/////////////////////////////////////////////////////////////////////
// MSO_Convert_PkFVP
/////////////////////////////////////////////////////////////////////
MSO100_API
I
MSO_Convert_PkFVP(
		MORPHO_HANDLE			i_h_Mso100Handle,
		PT_BUF_PK				i_px_PkFVP,
		PT_EXPORT_PK		    o_px_Convert_BufPkFVP,
		PUC						o_puc_ILV_Status
		);


/////////////////////////////////////////////////////////////////////
// MSO_Build_PkFVP
/////////////////////////////////////////////////////////////////////
MSO100_API
I
MSO_Build_PkFVP(
		MORPHO_HANDLE			i_h_Mso100Handle,
		PT_BUF_PK				i_px_BufPkFVP,
		UC					i_uc_PkFVP_PkQuality,
		UC					i_uc_PkFVP_SecurityLevelCompatibility,
		PT_BUF_PK				i_px_BufPkComp,
		UC					i_uc_PkComp_PkQuality,
		PT_EXPORT_PK		    i_px_Build_BufPkFVP,
		PUC					o_puc_Build_pkFVP_PkQuality,
		PUC					o_puc_Build_pkFVP_SecurityLevelCompatibility,
		PUC					o_puc_ILV_Status
		);

	/////////////////////////////////////////////////////////////////////
	// MSO_GetDescriptor_FormatBinSoft
	/////////////////////////////////////////////////////////////////////

	MSO100_API
	I MSO_GetDescriptor_FormatBinSoft (
				   MORPHO_HANDLE			i_h_Mso100Handle,
				   I						io_i_Status
				   );

	/////////////////////////////////////////////////////////////////////
	// MSO_Bio_Enroll_FingerVein
	/////////////////////////////////////////////////////////////////////

	MSO100_API
		I MSO_Bio_Enroll_FingerVein (
		MORPHO_HANDLE					i_h_Mso100Handle,
		UC 								i_uc_IndexDB,
		US								i_us_Timeout,
		UC								i_uc_Acquisition_Threshold,
		UC								i_uc_EnrollmentType,
		UC								i_uc_NbFinger,
		UC								i_uc_SaveRecord,
		UC								i_uc_NormalizedPK_Type,
		UC								i_uc_NormalizedFVP_Type,
		UC								i_uc_NbAddField,
		PT_BUF							i_px_AddField,
		UL								i_ul_pkUpdateMask,
		PT_EXPORT_PK					io_px_ExportPk,
		PT_EXPORT_PK					io_px_ExportFVP,
		PT_EXPORT_IMAGE					io_px_ExportImage,
		PUC								o_puc_EnrollStatus,
		PUL								o_pul_UserDBIndex,
		PUC								o_puc_ILV_Status,
		T_MORPHO_TEMPLATE_ENVELOP		i_x_typEnvelop,
		I								i_i_ApplicationDataLen,
		PUC								i_puc_ApplicationData,
		UC								i_uc_ExportCoderResult,
		PT_BUF							io_px_ExportResultCoder,
		PT_BUF							io_px_ExportMatrixDirections,
		PT_BUF							io_px_ExportMatrixQualities,
		PT_BIO_PARAM_PC					io_px_BioParamPC,
		UC								i_uc_SaveImageIndex,
		PT_MORPHO_MOC_PARAMETERS		i_px_MocParameters,
		PUC								io_puc_PkFpQuality,
		PUC								io_puc_AdvancedSecurityLevelCompatibility,
		const PT_BIO_DATA_ENCRYPTION	i_px_BioDataEncryption,
		PT_EXPORT_IMAGE_FFD				io_px_ExportFFDImage);


/////////////////////////////////////////////////////////////////////
// MSO_Bio_Free_FP_ROI_Images
/////////////////////////////////////////////////////////////////////

	MSO100_API
void MSO_Bio_Free_FP_ROI_Images(PT_FP_ROI_IMAGE_ARR io_px_ExportImageArr);
/////////////////////////////////////////////////////////////////////
// MSO_Bio_Export_FP_ROI_Images
/////////////////////////////////////////////////////////////////////

	MSO100_API
I MSO_Bio_Export_FP_ROI_Images (      
							 MORPHO_HANDLE					i_h_Mso100Handle,
							 PUC							o_puc_ILV_Status,
							 PT_FP_ROI_IMAGE_ARR			io_px_ExportImageArr
							 );
/////////////////////////////////////////////////////////////////////
// Identify
/////////////////////////////////////////////////////////////////////
MSO100_API
I
MSO_Bio_Identify_FingerVein (
							 MORPHO_HANDLE			i_h_Mso100Handle,
							 UC 					i_uc_UidDB,
							 US						i_us_Timeout,
							 US						i_us_MatchingTreshold,
							 PUC					o_puc_MatchingResult,
							 PUL					o_pul_UserDBIndex,
							 PT_BUF					o_px_UserID,
							 PUL					io_pul_AddFieldNb,
							 PT_BUF					o_px_AddFieldValue,
							 PUC					o_puc_ILV_Status,
							 PT_BIO_PARAM_PC		io_px_BioParamPC);

/////////////////////////////////////////////////////////////////////
// Identify Match
/////////////////////////////////////////////////////////////////////
MSO100_API
I
MSO_Bio_IdentifyMatch_FingerVein (
								  MORPHO_HANDLE			i_h_Mso100Handle,
								  UC 					i_uc_UidDB,
								  US					i_us_MatchingTreshold,
								  I 			        i_us_SecutityLevel,
								  UC					i_uc_NbPk,
								  PT_BUF_PK				i_px_Pk,
								  PUC					o_puc_MatchingResult,
								  PUL					o_pul_UserDBIndex,
								  PT_BUF				o_px_UserID,
								  PUL					o_pul_score,
								  PUC					o_puc_ILV_Status);


/////////////////////////////////////////////////////////////////////
// Verify
/////////////////////////////////////////////////////////////////////
MSO100_API
I
MSO_Bio_Verify_FingerVein(MORPHO_HANDLE			i_h_Mso100Handle,
							US					i_us_VerificationType, // Values : 0 or ID_USER_ID
							US					i_us_Timeout,
							US					i_us_MatchingTreshold,
							UC					i_uc_IndexDb,
							UC					i_uc_NbFinger,
							PT_BUF_PK			i_px_Pk,
							PUC					o_puc_MatchingResult,
							PUC					o_puc_ILV_Status,
							PUC					o_puc_num_pk,
							PUL					i_pul_UEPS_Verification_type,
							PUC					o_puc_TemplateSignature,
							PT_BIO_PARAM_PC		io_px_BioParamPC);


/////////////////////////////////////////////////////////////////////
// Identify Match
/////////////////////////////////////////////////////////////////////
MSO100_API
I
MSO_Bio_IdentifyMatch (
	MORPHO_HANDLE			i_h_Mso100Handle,
	UC 			i_uc_UidDB,
	US			i_us_MatchingTreshold,
	UC			i_uc_NbPk,
	PT_BUF_PK	i_px_Pk,
	PUC			o_puc_MatchingResult,
	PUL			o_pul_UserDBIndex,
	PT_BUF		o_px_UserID,
	PUL			o_pul_score,
	PUC			o_puc_ILV_Status
);


/////////////////////////////////////////////////////////////////////
// Verify Match
/////////////////////////////////////////////////////////////////////
MSO100_API
I
MSO_Bio_VerifyMatch_FingerVein (
		MORPHO_HANDLE		 i_h_Mso100Handle,
		UC					 i_uc_NbPkSrc,
		PT_BUF_PK	         i_px_PkSrc,
		UC			         i_uc_NbPkRef,
		PT_BUF_PK	         i_px_PkRef,
		US			         i_us_MatchingTreshold,
		US					 i_us_SecutityLevel,
		PUC		             o_puc_MatchingResult,
		PUC		             o_puc_ListRefIndex,
		PUL		             o_pul_score,
		PUC		             o_puc_ILV_Status
);

/////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////
MSO100_API
I
MSO_Cancel (
	MORPHO_HANDLE			i_h_Mso100Handle
);

/////////////////////////////////////////////////////////////////////
MSO100_API
I
BIODB_UpdatePublicData (
	MORPHO_HANDLE			i_h_Mso100Handle,
	UC 						i_uc_IndexDB,
	UL						i_ul_IndexUser,
	PT_BIODB_DATA_TRANSPORT	i_px_BioDbDataTransport,
	UC						i_uc_NbData,
	PUC						o_puc_ILV_Status
);

/////////////////////////////////////////////////////////////////////
MSO100_API
I
BIODB_UpdatePrivateData_BioParam
(
	MORPHO_HANDLE			i_h_Mso100Handle,
	UC 						i_uc_IndexDB,
	UL						i_ul_IndexUser,
	PT_BIODB_DATA_TRANSPORT	i_px_BioDbDataTransport,
	UC						i_uc_NbData,
	US						i_us_Timeout,
	US						i_us_MatchingTreshold,
	PUC						o_puc_ILV_Status,
	PT_BIO_PARAM_PC			io_px_BioParamPC
);

/////////////////////////////////////////////////////////////////////
MSO100_API
I
BIODB_UpdatePrivateDataFFDCoder (MORPHO_HANDLE			i_h_Mso100Handle,
					   UC 						i_uc_IndexDB,
					   UL						i_ul_IndexUser,
					   PT_BIODB_DATA_TRANSPORT	i_px_BioDbDataTransport,
					   UC						i_uc_NbData,
					   US						i_us_Timeout,
					   US						i_us_MatchingTreshold,
					   UL						i_ul_AsynchronousEvent,
					   PUC						o_puc_ILV_Status,
					   I						i_i_security_level,
					   I						i_i_Algo
					   );

/////////////////////////////////////////////////////////////////////
MSO100_API
I
BIODB_UpdatePrivateDataFFD (MORPHO_HANDLE			i_h_Mso100Handle,
					   UC 						i_uc_IndexDB,
					   UL						i_ul_IndexUser,
					   PT_BIODB_DATA_TRANSPORT	i_px_BioDbDataTransport,
					   UC						i_uc_NbData,
					   US						i_us_Timeout,
					   US						i_us_MatchingTreshold,
					   UL						i_ul_AsynchronousEvent,
					   PUC						o_puc_ILV_Status,
					   I						i_i_security_level
					   );

/////////////////////////////////////////////////////////////////////
MSO100_API
I
BIODB_UpdatePrivateData (
	MORPHO_HANDLE			i_h_Mso100Handle,
	UC 						i_uc_IndexDB,
	UL						i_ul_IndexUser,
	PT_BIODB_DATA_TRANSPORT	i_px_BioDbDataTransport,
	UC						i_uc_NbData,
	US						i_us_Timeout,
	US						i_us_MatchingTreshold,
	UL						i_ul_AsynchronousEvent,
	PUC						o_puc_ILV_Status
);

/////////////////////////////////////////////////////////////////////
MSO100_API
I
MSO_BioDB_GetPublicListData (
	MORPHO_HANDLE					i_h_Mso100Handle,
	UC 								i_uc_IndexDB,
	UL								i_ul_UidData,
	PUL								io_pul_NbTranport,
	PT_TRANSPORT_PUBLIC_LIST_FIELD	io_ax_TransportPublicField,
	PUC								o_puc_ILV_Status
);

/////////////////////////////////////////////////////////////////////
// RS232
/////////////////////////////////////////////////////////////////////
MSO100_API
I
MSO_ConfigUart
(
	MORPHO_HANDLE	i_h_Mso100Handle,
	UL	i_ul_BaudRate,
	UC	i_uc_ByteSize,
	UC	i_uc_StopBits,
	UC	i_uc_Parity,
	UC	i_uc_FlowCtrl,
	UC	i_uc_Send,
	UC	i_uc_Format,
	PUC	o_puc_ILV_Status
);

/////////////////////////////////////////////////////////////////////
MSO100_API
I
MSO_Break
(
	MORPHO_HANDLE	i_h_Mso100Handle,
	UL		i_ul_Time
);

/////////////////////////////////////////////////////////////////////
MSO100_API
I
MSO_RS232_Write
(
	MORPHO_HANDLE	i_h_Mso100Handle,
	UC *i_puc_Buf,
	UL i_ul_Size,
	UL *o_pul_BytesWritten
);

/////////////////////////////////////////////////////////////////////
MSO100_API
I
MSO_RS232_Read
(
	MORPHO_HANDLE	i_h_Mso100Handle,
	PUC	i_puc_Buf,
	UL	i_ul_Size,
	UL	i_ul_ReadTotalTimeoutConstant
);

/////////////////////////////////////////////////////////////////////
MSO100_API
I
MSO_RS232_GetConfig
(
	MORPHO_HANDLE	i_h_Mso100Handle
);

/////////////////////////////////////////////////////////////////////
MSO100_API
I
MSO_SPRS232_Initialize
(
	MORPHO_HANDLE	i_h_Mso100Handle,
	PC				i_strName,
	UL				i_ul_BaudRate
);

/////////////////////////////////////////////////////////////////////
MSO100_API
I
MSO_SPRS232_Close
(
	MORPHO_HANDLE	i_h_Mso100Handle
);

/////////////////////////////////////////////////////////////////////
MSO100_API
I
MSO_SPRS232_SetBaudRate
(
	MORPHO_HANDLE	i_h_Mso100Handle,
	I				i_i_baudRate
);

/////////////////////////////////////////////////////////////////////
MSO100_API
I
MSO_SPRS232_Send
(
	MORPHO_HANDLE	i_h_Mso100Handle,
	PUC				i_puc_Data,
	UL				i_ul_DataSize
);

/////////////////////////////////////////////////////////////////////
MSO100_API
I
MSO_SPRS232_Receive(
	MORPHO_HANDLE	i_h_Mso100Handle,
	PUC				i_puc_Buffer,
	UL				i_ul_BufferSize,
	PUL				o_pul_ILVSize,
	UL				i_ul_ReadTotalTimeoutConstant
);


/////////////////////////////////////////////////////////////////////
//Prototypes OTP
/////////////////////////////////////////////////////////////////////
MSO100_API
I
MSO_OTP_SetParameters (
		MORPHO_HANDLE				i_h_Mso100Handle,
		PT_OTP_PARAMETERS_STRUCT	i_px_Otp_Parameters,
		US							i_us_option_mask,
		PUC							o_puc_ILV_Status
);

/////////////////////////////////////////////////////////////////////
MSO100_API
I
MSO_OTP_Generate (
	MORPHO_HANDLE				i_h_Mso100Handle,
	US							i_us_Timeout,
	PT_OTP_GENERATE_STRUCT		i_px_Otp_generate,
	US							i_us_option_mask,
	PUC*						o_ppuc_OTP,
	PUL							o_pul_NbUserData,
	PT_BUF*						o_ppx_UserData,
	PT_BUF						o_px_OTP_UserData,
	PUC							o_puc_ILV_Status,
	B*							o_pb_UserId
);
/////////////////////////////////////////////////////////////////////

MSO100_API
I
MSO_OTP_GetStatus (
	MORPHO_HANDLE		i_h_Mso100Handle,
	PUC					o_puc_ParamState,
	PUS					o_pus_OTP_Param,
	PUC					o_puc_UserEnrolled,
	PUL					o_pul_NbUserData,
	PT_BUF*				o_ppx_UserData,
	PT_BUF				o_px_OTP_UserData,
	PUC					o_puc_ILV_Status,
	B*					o_pb_UserId
);
/////////////////////////////////////////////////////////////////////
MSO100_API
I
MSO_OTP_EnrollUserBioParam (
		MORPHO_HANDLE			i_h_Mso100Handle,
		US						i_us_Timeout,
		PT_BUF					i_px_Pin,
		UC						i_uc_lenUserID,						//	<opt> the UserID length
		PUC						i_puc_UserID,						//	<opt> the UserID
		UC						i_uc_lenAddField1,					//	<opt> the additionnal field1 length
		PUC						i_puc_AddField1,					//	<opt> the additionnal field1
		UC						i_uc_lenAddField2,					//	<opt> the additionnal field2 length
		PUC						i_puc_AddField2,
		UC						i_uc_NormalizedPK_Type,
		PT_EXPORT_PK			io_px_ExportPk,
		T_MORPHO_TEMPLATE_ENVELOP	i_x_typEnvelop,
		I						i_i_ApplicationDataLen,
		PUC						i_puc_ApplicationData,
		US						i_us_option_mask,
		PUC						o_puc_ILV_Status,
		PT_BIO_PARAM_PC			io_px_BioParamPC
);
/////////////////////////////////////////////////////////////////////
MSO100_API
I
MSO_OTP_EnrollUser (
		MORPHO_HANDLE		i_h_Mso100Handle,
		US					i_us_Timeout,
		PT_BUF				i_px_Pin,
		UC					i_uc_lenUserID,						//	<opt> the UserID length
		PUC					i_puc_UserID,						//	<opt> the UserID
		UC					i_uc_lenAddField1,					//	<opt> the additionnal field1 length
		PUC					i_puc_AddField1,					//	<opt> the additionnal field1
		UC					i_uc_lenAddField2,					//	<opt> the additionnal field2 length
		PUC					i_puc_AddField2,
		UC					i_uc_NormalizedPK_Type,
		PT_EXPORT_PK		io_px_ExportPk,
		T_MORPHO_TEMPLATE_ENVELOP	i_x_typEnvelop,
		I					i_i_ApplicationDataLen,
		PUC					i_puc_ApplicationData,
		US					i_us_option_mask,
		UL					i_ul_AsynchronousEvent,
		PUC					o_puc_ILV_Status
);

/////////////////////////////////////////////////////////////////////
MSO100_API
I
MSO_GetUnlockSeed
(
	MORPHO_HANDLE	i_h_Mso100Handle,
	PUC				o_puc_Seed,
	UL				i_ul_SeedSizeMax,
	PUL				o_pul_SeedSize,
	PUC				o_puc_ILV_Status
);

/////////////////////////////////////////////////////////////////////
MSO100_API
I
MSO_Unlock
(
	MORPHO_HANDLE	i_h_Mso100Handle,
	PUC				i_puc_SecretID,
	UL				i_ul_SecretIDSize,
	PUC				i_puc_CipheredSeed,
	UL				i_ul_CipheredSeedSize,
	PUC				o_puc_ILV_Status
);

/////////////////////////////////////////////////////////////////////
MSO100_API
I
MSO_LoadMasterKeys (
	MORPHO_HANDLE			i_h_Mso100Handle,
	PUC 	i_puc_MasterKey1,
	PUC		i_puc_MasterKey3,
	PUC		o_puc_ILV_Status
);

/////////////////////////////////////////////////////////////////////
MSO100_API
I
MSO_LoadMocKey_by_KencTrans (
	MORPHO_HANDLE			i_h_Mso100Handle,
	PUC		i_puc_Kenc_Ciffered_by_KencTrans,
	UL		i_ul_Kenc_Ciffered_by_KencTrans_Len,
	PUC		o_puc_ILV_Status
);

/////////////////////////////////////////////////////////////////////
MSO100_API
I
MSO_LoadMocKey_by_Certificate (
	MORPHO_HANDLE			i_h_Mso100Handle,
	PUC		i_puc_Kenc_Ciffered_by_Certificate,
	UL		i_ul_Kenc_Ciffered_by_Certificate_Len,
	PUC		i_puc_Kenc_Ciffered_by_Certificate_Signature,
	UL		i_ul_Kenc_Ciffered_by_Certificate_Signature_Len,
	PUC		i_puc_HostCertificate,
	UL		i_ul_HostCertificateSize,
	PUC		o_puc_ILV_Status
);

/////////////////////////////////////////////////////////////////////
MSO100_API
I
MSO_SetMocApdu (
	MORPHO_HANDLE			i_h_Mso100Handle,
	PT_MORPHO_MOC_APDU_PARAMETERS i_px_MocApduParameters,
	PUC		o_puc_ILV_Status
);


/////////////////////////////////////////////////////////////////////
MSO100_API
I
MSO_GetKCV
(
	MORPHO_HANDLE	i_h_Mso100Handle,
	UC				i_uc_ID_Key,
	PUC				i_puc_SecretID,
	UC				o_auc_KCV[KCV_LEN],
	PUC				o_puc_ILV_status
);

/////////////////////////////////////////////////////////////////////
MSO100_API
I
MSO_SetConfigValue
(
	MORPHO_HANDLE					i_h_Mso100Handle,
	PUC								o_puc_ILV_Status,
	US								i_us_Tag,
	UL								i_ul_SizeOfSettings,
	PUC								i_puc_Settings
);

/////////////////////////////////////////////////////////////////////
MSO100_API
I
MSO_GetConfigValue
(
	MORPHO_HANDLE					i_h_Mso100Handle,
	PUC								o_puc_ILV_Status,
	US								i_us_Tag,
	PUL								io_pul_SizeOfConfigValue,
	PUC								o_puc_ConfigValue
);
//////////////////////////////////////////////////////////////////////
MSO100_API
I
MSO_GET_UserTemplateQuality
(
                             MORPHO_HANDLE i_h_Mso100Handle,
                             UC i_uc_IndexDB,
                             PUC i_puc_UserID,
                             UC i_uc_UserIDSize,
                             UL i_ul_UserIndex,
                             PUC o_puc_TemplateQuality, // Pointeur sur un tableau de tail o_uc_NBPK
                             PUC o_puc_NBPK,
                             PUC o_puc_ILV_Status
);
///////////////////////////////////////////////////////////////////
MSO100_API
I
MSO_GetFFDLogs
(
 MORPHO_HANDLE		i_h_Mso100Handle,
 PUL				o_pul_NbLogs,
 T_ILV_LOG_LINE**	o_px_LogLines,
 PUC				o_puc_ILV_Status
);

///////////////////////////////////////////////////////////////////
MSO100_API int MSO_LoadKs
(
	MORPHO_HANDLE		i_h_mso100,
	const PUC			i_puc_key,
	unsigned int		i_ui_key_size,
	PUC					o_puc_status
);

///////////////////////////////////////////////////////////////////
MSO100_API int MSO_LoadKsSymSecure
(
	MORPHO_HANDLE		i_h_mso100,
	const PUC			i_puc_key,
	unsigned int		i_ui_key_size,
	PUC					o_puc_status
);

///////////////////////////////////////////////////////////////////
MSO100_API int MSO_LoadKsAsymSecure
(
	MORPHO_HANDLE		i_h_mso100,
	const PUC			i_puc_key,
	unsigned int		i_ui_key_size,
	const PUC			i_puc_signature,
	unsigned int		i_ui_signature_size,
	const PUC			i_puc_host_certificate,
	T__UINT32			i_ui_host_certificate_size,
	PUC					o_puc_status
);

#ifdef _ANDROID_USB
MSO100_API I SetJNIEnv(JNIEnv* i_env);
#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

