// The present software is not subject to the US Export Administration Regulations (no exportation license required), May 2012
#ifndef ILV_STRUCT_H
#define ILV_STRUCT_H

///////////////////////////////////////////////////////////////////////
// DEBUT de section PACKED
///////////////////////////////////////////////////////////////////////

/*******************************************************************
ILV_DESC_SOFT_BIN

********************************************************************/

#if defined(WIN32)
	#pragma pack(1)
	typedef struct
#elif defined(__GNUC__)
	typedef struct __attribute__((packed))
#else
	typedef __packed struct
#endif
{
	UL			m_ul_SoftwareReferenceSize;
	UC			m_uc_Version;
	UC			m_uc_Release;
}T_ILV_DESC_SOFT_BIN, *PT_ILV_DESC_SOFT_BIN;

/*******************************************************************
ILV_UART_CONFIG

********************************************************************/
#if defined(WIN32)
	typedef struct
#elif defined(__GNUC__)
	typedef struct __attribute__((packed))
#else
	typedef __packed struct
#endif
{
	UL			m_ul_BaudRate;
	UC			m_uc_ByteSize;
	UC			m_uc_StopBits;
	UC			m_uc_Parity;
	UC			m_uc_FlowCtrl;
	UC			m_uc_Send;
	UC			m_uc_Format;
}T_ILV_UART_CONFIG, *PT_ILV_UART_CONFIG;

#define BAUDRATE_1200			0			//MA	MSO
#define BAUDRATE_2400			1			//MA	MSO
#define BAUDRATE_4800			2			//MA	MSO
#define BAUDRATE_9600			3			//MA	MSO
#define BAUDRATE_19200			4			//MA	MSO
#define BAUDRATE_28800			5			//MA
#define BAUDRATE_38400			6			//MA	MSO
#define BAUDRATE_57600			7			//MA	MSO
#define BAUDRATE_115200			8			//MA	MSO
#define BAUDRATE_110			9			//MSO
#define BAUDRATE_14400			10			//MSO
#define BAUDRATE_76800			11			//MSO
#define BAUDRATE_230400			12			//MSO

#define FlOW_CTRL_NO			0			//MA	MSO
#define FlOW_CTRL_RTS_CTS		1			//MA	MSO		
#define FlOW_CTRL_XON_XOFF		2			//MA	MSO	

#define	BYTE_SIZE_5				5			//MA
#define	BYTE_SIZE_6				6			//MA
#define	BYTE_SIZE_7				7			//MA	MSO
#define	BYTE_SIZE_8				8			//MA	MSO

#define	NO_PARITY				0			//MA	MSO
#define	ODD_PARITY				1			//MA	MSO
#define	EVEN_PARITY				2			//MA	MSO

#define STOP_BITS_1				1			//MA	MSO
#define STOP_BITS_2				2			//MA	MSO

/*******************************************************************
ILV_UPDATE_PRIVATE_DATA

********************************************************************/
#if defined(WIN32)
	typedef struct
#elif defined(__GNUC__)
	typedef struct __attribute__((packed))
#else
	typedef __packed struct
#endif
{
	UC			m_uc_IndexDB;
	UC			m_uc_NbUpdateData;
	US			m_us_Timeout;
	US			m_us_MatchingThreshold;
}T_ILV_UPDATE_PRIVATE_DATA, *PT_ILV_UPDATE_PRIVATE_DATA;

/*******************************************************************
ILV_UPDATE_PUBLIC_DATA

********************************************************************/
#if defined(WIN32)
	typedef struct
#elif defined(__GNUC__)
	typedef struct __attribute__((packed))
#else
	typedef __packed struct
#endif
{
	UC			m_uc_IndexDB;
	UC			m_uc_NbUpdateData;
}T_ILV_UPDATE_PUBLIC_DATA, *PT_ILV_UPDATE_PUBLIC_DATA;

/*******************************************************************
ID_FIELD_CONTENT

********************************************************************/
#if defined(WIN32)
	typedef struct
#elif defined(__GNUC__)
	typedef struct __attribute__((packed))
#else
	typedef __packed struct
#endif
{
	UL			m_ul_IdData;
	UL			m_ul_DataLenght;
}T_ID_FIELD_CONTENT, *PT_ID_FIELD_CONTENT;

/*******************************************************************
ILVEXT_TEST_COMPRESS

********************************************************************/
#if defined(WIN32)
	typedef struct
#elif defined(__GNUC__)
	typedef struct __attribute__((packed))
#else
	typedef __packed struct
#endif
{
	UL			m_ul_NbRow;
	UL			m_ul_NbCol;
	UC			m_uc_CompressType;
	UC			m_uc_CompressParam;
	UC			pad[2];
}T_ILVEXT_TEST_COMPRESS, *PT_ILVEXT_TEST_COMPRESS;


/*******************************************************************
	ID_TKB_SETTINGS
********************************************************************/
#if defined(WIN32)
	typedef struct
#elif defined(__GNUC__)
	typedef struct __attribute__((packed))
#else
	typedef __packed struct
#endif
{
	UC			m_uc_EnvelopTyp;
	I			m_i_DataSize;
}T_ID_TKB_SETTINGS, *PT_ID_TKB_SETTINGS;


/*******************************************************************
ILVEXT_PROD_TEST

********************************************************************/
#if defined(WIN32)
	typedef struct
#elif defined(__GNUC__)
	typedef struct __attribute__((packed))
#else
	typedef __packed struct
#endif
{
	UC			m_uc_Light_Level_LowLed_Min;
	UC			m_uc_Light_Level_LowLed_Max;
	UC			m_uc_Light_Level_HighLed_Min;
	UC			m_uc_Light_Level_HighLed_Max;
	UC			m_uc_Light_Level_Difference;
	UC			padding[3];
}T_ID_LIGHT_LEVEL_THRES, *PT_ID_LIGHT_LEVEL_THRES;


/*******************************************************************
ILV_GET_DESCRIPTOR

********************************************************************/
#if defined(WIN32)
	typedef struct
#elif defined(__GNUC__)
	typedef struct __attribute__((packed))
#else
	typedef __packed struct
#endif
{
	UC			m_uc_StructVersion;
	UC			m_uc_ID_CameraFP;
	UC			m_uc_ID_CameraFV;
	UC			m_uc_BoardID;
	UL			m_ul_ram_size;
	UL			m_ul_ram_model;
	UL			m_ul_nor_flash_size;
	US			m_ul_nor_flash_manufacturer_id;
	US			m_ul_nor_flash_device_id;
	UL			m_ul_nand_flash_size;
	US			m_ul_nand_flash_manufacturer_id;
	US			m_ul_nand_flash_device_id;
	UL			m_ul_sdcard_size;
	UL			m_ul_sdcard_model;
	UL			m_ul_ProcessorID;
	UL			m_ul_MicrobootVersion;
	C			m_ac_FWVersion[16];
	C			m_ac_SN_OEM[12];
	C			m_ac_PN_OEM[12];
	C			m_ac_SN_BOARD[12];
	C			m_ac_PN_BOARD[12];
	C			m_ac_SN_PACK[12];
	C			m_ac_PN_PACK[12];
	UL			m_ul_SecuConfig;
	UC			m_uc_NbrBioDB;
	UC			m_auc_BioDBVer[16];
	UL			m_ul_NorFileSystemVersion;
	UL			m_ul_NandFileSystemVersion;
	UL			m_ul_SmartCardVersion;
	UL			m_ul_FPEepromVersion;
	UL			m_ul_FVEepromVersion;
	UC			m_auc_ExtraInfo[16];
}T_HARDWARE_INFO_V1, *PT_HARDWARE_INFO_V1;

#if defined(WIN32)
	typedef struct
#elif defined(__GNUC__)
	typedef struct __attribute__((packed))
#else
	typedef __packed struct
#endif
{
	UC			m_uc_StructVersion;
	UC			m_uc_ID_CameraFP;
	UC			m_uc_ID_CameraFV;
	UC			m_uc_BoardID;
	UL			m_ul_ram_size;
	UL			m_ul_ram_model;
	UL			m_ul_nor_flash_size;
	US			m_ul_nor_flash_manufacturer_id;
	US			m_ul_nor_flash_device_id;
	UL			m_ul_nand_flash_size;
	US			m_ul_nand_flash_manufacturer_id;
	US			m_ul_nand_flash_device_id;
	UL			m_ul_sdcard_size;
	UL			m_ul_sdcard_model;
	UL			m_ul_ProcessorID;
	UL			m_ul_MicrobootVersion;
	C			m_ac_FWVersion[16];
	C			m_ac_SN_OEM[12];
	C			m_ac_PN_OEM[12];
	C			m_ac_SN_BOARD[12];
	C			m_ac_PN_BOARD[12];
	C			m_ac_SN_PACK[12];
	C			m_ac_PN_PACK[12];
	UL			m_ul_SecuConfig;
	UC			m_uc_NbrBioDB;
	UC			m_auc_BioDBVer[5];
	UC			m_uc_BioDbEncrypted[5];
	UC			m_uc_RFU[6];
	UL			m_ul_NorFileSystemVersion;
	UL			m_ul_NandFileSystemVersion;
	UL			m_ul_SmartCardVersion;
	UL			m_ul_FPEepromVersion;
	UL			m_ul_FVEepromVersion;
	UC			m_auc_ExtraInfo[16];
}T_HARDWARE_INFO_V3, *PT_HARDWARE_INFO_V3;

#if defined(WIN32)
	typedef struct
#elif defined(__GNUC__)
	typedef struct __attribute__((packed))
#else
	typedef __packed struct
#endif
{
	UC			m_uc_StructVersion;
	UC			m_uc_ID_CameraFP;
	UC			m_uc_ID_CameraFV;
	UC			m_uc_BoardID;
	UL			m_ul_ram_size;
	UL			m_ul_ram_model;
	UL			m_ul_nor_flash_size;
	US			m_ul_nor_flash_manufacturer_id;
	US			m_ul_nor_flash_device_id;
	UL			m_ul_nand_flash_size;
	US			m_ul_nand_flash_manufacturer_id;
	US			m_ul_nand_flash_device_id;
	UL			m_ul_sdcard_size;
	UL			m_ul_sdcard_model;
	UL			m_ul_ProcessorID;
	UL			m_ul_MicrobootVersion;
	C			m_ac_FWVersion[16];
	C			m_ac_SN_OEM[12];
	C			m_ac_PN_OEM[12];
	C			m_ac_SN_BOARD[12];
	C			m_ac_PN_BOARD[12];
	C			m_ac_SN_PACK[12];
	C			m_ac_PN_PACK[12];
	UL			m_ul_SecuConfig;
	UC			m_uc_NbrBioDB;
	UC			m_auc_BioDBVer[5];
	UC			m_uc_BioDbEncrypted[5];
	UC			m_uc_RFU[6];
	UL			m_ul_NorFileSystemVersion;
	UL			m_ul_NandFileSystemVersion;
	UL			m_ul_SmartCardVersion;
	/* EEPROM version */
	UL			m_ul_FPEepromVersion;
	UL			m_ul_FVEepromVersion;
	/* Uniformite version. */
	UC          m_uc_UniformiteVersion;
	UC          m_uc_UniformiteSubVersion;
	UC          m_uc_UniformiteAlgoType;
	/* Deconvolution version. */
	UC          m_uc_DeconvolVersion;
	UC          m_uc_DeconvolSubVersion;
	UC          m_uc_DeconvolAlgoType;
	/* CMOS version. */
	UC          m_uc_CmosVersion;
	UC          m_uc_CmosSubVersion;
	UC          m_uc_CmosType;
	/* Defaut version. */
	UC          m_uc_DefautVersion;
	UC          m_uc_DefautSubVersion;
	UC          m_uc_DefautAlgoType;
	/* */
	UC			m_auc_ExtraInfo[16];
}T_HARDWARE_INFO_V4, *PT_HARDWARE_INFO_V4;

typedef T_HARDWARE_INFO_V4 T_HARDWARE_INFO;
typedef PT_HARDWARE_INFO_V4 PT_HARDWARE_INFO;

#define T_HARDWARE_INFO_VERSION 4

/*******************************************************************
ID_FFD_LOG_LINE
********************************************************************/
#if defined(WIN32)
	typedef struct
#elif defined(__GNUC__)
	typedef struct __attribute__((packed))
#else
	typedef __packed struct
#endif
{
	UL			m_ul_TimeStamp;
	C			m_c_EventString[64];
}T_ILV_LOG_LINE;

#if defined(WIN32)
#	pragma pack()
#endif
///////////////////////////////////////////////////////////////////////
// FIN de section PACKED
///////////////////////////////////////////////////////////////////////
#endif
