// The present software is not subject to the US Export Administration Regulations (no exportation license required), May 2012
#ifndef __ILV_DEFINITIONS_H__
#define __ILV_DEFINITIONS_H__


/* ************************************************************
 * ILV Commands
 * ************************************************************/
#define ILV_REBOOT										0x04
#define ILV_GET_DESCRIPTOR								0x05

#define ILV_GET_BASE_CONFIG								0x07

#define ILV_VERIFY										0x20
#define ILV_ENROLL										0x21
#define ILV_IDENTIFY									0x22
#define ILV_VERIFY_MATCH								0x23
#define ILV_IDENTIFY_MATCH								0x24
#define ILV_CONVERT_PK_FVP								0x25
#define ILV_BUILD_PK_FVP								0x26

#define ILV_CREATE_DB									0x30

#define ILV_ERASE_BASE									0x32

#define ILV_ADD_RECORD									0x35
#define ILV_REMOVE_RECORD								0x36

#define ILV_FIND_USER_DB								0x38

#define ILV_DESTROY_DB									0x3B
#define ILV_UPDATE_PUBLIC_DATA							0x3C
#define ILV_UPDATE_PRIVATE_DATA							0x3D
#define ILV_LIST_PUBLIC_FIELDS							0x3E
#define ILV_GET_DATA_DB									0x3F

#define ILV_GET_USER_TEMPLATE_QUALITY					0x46

#define ILV_INVALID										0x50

#define ILV_MSO_GET_FFD_LOG								0x65

#define ILV_CANCEL										0x70
#define ILV_ASYNC_MESSAGE								0x71

#define ILV_SECU_GET_CONFIG								0x80

#define ILV_GET_UNLOCK_SEED								0x8B
#define ILV_UNLOCK										0x8C

#define ILV_GET_MSO_CONFIG								0x90
#define ILV_MODIFY_MSO_CONFIG							0x91

#define	ILV_OTP_ENROLL_USER								0xB0
#define	ILV_OTP_GENERATE								0xB1
#define	ILV_OTP_SET_PARAMETERS							0xB2
#define	ILV_OTP_GET_STATUS								0xB3

#define ILV_LOAD_MASTER_KEYS							0xC3
#define ILV_LOAD_MOC_KEYS								0xC4
#define ILV_SET_MOC_APDU								0xC5
#define ILV_KEY_CHECK_VALUE								0xC6
#define ILV_LOAD_KS										0xC7

#define ILV_GET_IMAGE_INDEX								0xD0

#define ILV_CONFIG_UART									0xEE



/* ************************************************************
 * ILV Command Errors
 * ************************************************************/
#define ILV_OK											0x00	//  0	Successful result
#define ILVERR_ERROR									0xFF	// -1	An error occurred
#define ILVERR_BADPARAMETER								0xFE	// -2	Input parameters are not valid
#define ILVERR_BAD_PARAMETER							ILVERR_BADPARAMETER
#define ILVERR_INVALID_MINUTIAE							0xFD	// -3	The minutiae is not valid
#define ILVERR_INVALID_USER_ID							0xFC	// -4	The record identifier does not exist in the database
#define ILVERR_INVALID_USER_DATA						0xFB	// -5	The user data are not valid
#define ILVERR_TIMEOUT									0xFA	// -6	No response after defined time.
#define ILVERR_INVALID_ID_PROTOCOL						0xF9	// -7	The protocole used is not valid
#define ILVERR_ALREADY_ENROLLED							0xF8	// -8	The person is already in the base
#define ILVERR_BASE_NOT_FOUND							0xF7	// -9	The specified base does not exist
#define ILVERR_BASE_ALREADY_EXISTS						0xF6	//-10	The specified base already exist
#define ILVERR_BIO_IN_PROGRESS							0xF5	//-11	Command received during biometric processing
#define ILVERR_BIO_ENCOURS								ILVERR_BIO_IN_PROGRESS
#define ILVERR_CMD_INPROGRESS							0xF4	//-12	Command received while another command is running
#define ILVERR_CMD_ENCOURS								ILVERR_CMD_INPROGRESS
#define ILVERR_FLASH_INVALID							0xF3	//-13	Flash type invalid
#define ILVERR_NO_SPACE_LEFT							0xF2	//-14	Not Enough memory for the creation of a database

#define ILVERR_BAD_SIGNATURE							0xF0	//-16	Invalid digital signature

#define ILVERR_OUT_OF_FIELD								0xEB	//-21
#define ILVERR_INVALID_ACCESS							0xEA	//-22
#define	ILVERR_FIELD_NOT_FOUND							0xE9	//-23	Field does not exist.
#define ILVERR_FIELD_INVALID							0xE8	//-24	Field size or field name is invalid.
#define ILVERR_SECURITY_MODE							0xE7	//-25	The request is not compatible with security mode.
#define ILVERR_USER_NOT_FOUND							0xE6	//-26	The searched user is not found.
#define ILVERR_CMDE_ABORTED								0xE5	//-27	Commanded has been aborted by the user.
#define ILVERR_SAME_FINGER								0xE4	//-28	There are two templates of the same finger
#define ILVERR_NO_HIT									0xE3	//-29	Presented finger does not match
#define ILVERR_SECU_CERTIF_NOT_EXIST					0xE2	//-30	The required certificate does not exist
#define ILVERR_SECU_BAD_STATE							0xE1	//-31	Invalid security state
#define ILVERR_SECU_ANTIREPLAY							0xE0	//-32	An antireplay error occured
#define ILVERR_SECU_ASN1								0xDF	//-33	Error while decoding an ASN1 object
#define ILVERR_SECU										0xDE	//-34	Cryptographic error
#define ILVERR_SECU_AUTHENTICATION						0xDD	//-35	Mutual authentication error
#define ILVERR_NO_LIC_MKIT								0xDC	//-36	No licence MorphoKit
#define ILVERR_FFD										0xDB	//-37
#define ILVERR_MOIST_FINGER								0xDA	//-38
#define ILVERR_OTP_NOT_INITIALIZED						0xD9	//-39	All OTP paramaters are not initialized
#define ILVERR_NO_MORE_OTP								0xD8	//-40	No more OTP (sequence number = 0)
#define ILVERR_OTP_ENROLL_NEEDED						0xD7	//-41	OTP database is empty : enroll is needed
#define ILVERR_OTP_NO_HIT								0xD6	//-42	No Hit (or latent ou FFD)
#define ILVERR_OTP_REENROLL_NOT_ALLOWED					0xD5	//-43	Re-enrolment not allowed
#define ILVERR_OTP_ENROLL_FAILED						0xD4	//-44	Enrolment failed (status != ILV_OK)
#define ILVERR_OTP_IDENT_FAILED							0xD3	//-45	Ident failed (status != ILV_OK except DB errors)
#define ILVERR_OTP_PIN_NEEDED							0xD2	//-46	Pin needed for first enrolment

#define ILVERR_UEPS_SECU_ERROR							0xCF	//-49	Error Secu UEPS
#define ILVERR_NO_UEPS_KEYS								0xCE	//-50	No UEPS Keys injected
#define ILVERR_UEPS_ERROR_FILE							0xCD	//-51	Error in UEPS file Keys
#define ILVERR_UEPS_BAD_KEY_ID							0xCC	//-52	Bad Key ID
#define ILVERR_UEPS_SECURE_SESSION_NOT_CREATED			0xCB	//-53	Secure Session not created
#define ILVERR_UEPS_KEY_ID_NOT_FOUND					0xCA	//-54	KeyID not found
#define ILVERR_UEPS_DEFAULT_SESSION_NOT_CREATED			0xC9	//-55	default load key Session not created
#define ILVERR_FVP_MINUTIAE_SECURITY_MISMATCH			0xC8	//-56
#define ILVERR_OPERATION_NOT_SUPPORTED					0xC7	//-57
#define ILVERR_FINGER_MISPLACED_OR_WITHDRAWN			0xC5	//-59

#define ILVERR_NO_LICENSE								0xC3	//-61	Missing required license
#define ILVERR_ADVANCED_SECURITY_LEVEL_MISMATCH			0xC2	//-62
#define ILVERR_INCONSISTENT_PARAMETERS					0xC1	//-63

#define ILVERR_OTP_LOCK_SET_PARAM						0xBF	//-65	OTP Set Param ILV locked
#define ILVERR_OTP_LOCK_ENROLL							0xBE	//-66	OTP Enroll ILV locked
#define ILVERR_OTP_LOCK_GEN_OTP							0xBD	//-67	OTP Generate ILV locked
#define ILVERR_DEVICE_LOCKED							0xBC	//-68	The Application is locked
#define ILVERR_DEVICE_NOT_LOCK							0xBB	//-69	The Application is locked

#define ILVERR_FFD_FINGER_MISPLACED						0xB9	//-71	Misplaced finger detected by FFD sensor
#define ILVERR_KEY_NOT_FOUND							0xB8	//-72	Cryptographic key not found
#define ILVERR_ADVANCED_SECURITY_LEVEL_NOT_AVAILABLE	0xB7  	//-73

#define ILVERR_NOT_IMPLEMENTED							0x9D	//-99	The command is not yet implemented



/* ************************************************************
 * Biometic ILV Command Status
 * ************************************************************/
#define ILVSTS_OK										0x00	// 0	Success
#define ILVSTS_HIT										0x01	// 1	Authentication or identification succeeded
#define ILVSTS_NO_HIT									0x02	// 2	Authentication or identification failed
#define ILVSTS_LATENT									0x03	// 3	Latent fingerprint detected
#define ILVSTS_DB_FULL									0x04	// 4	The database is full
#define ILVSTS_DB_EMPTY									0x05	// 5	The database is empty
#define ILVSTS_BAD_QUALITY								0x06	// 6	Bad finger and/or enrollment quality

#define ILVSTS_FFD										0x22	//34	Fake finger detected
#define ILVSTS_FAKE_FINGER_DETECTED						ILVSTS_FFD
#define ILVSTS_MOIST_FINGER								0x23	//35	Finger is too moist



/* ************************************************************
 * Identifiers of ILV formatted data
 * ************************************************************/
#define ID_PKCOMP										2	//0x02
#define ID_PKMAT										3	//0x03
#define ID_USER_ID										4	//0x04
#define ID_USER_DATA									5	//0x05
#define ID_COM1											6	//0x06

#define ID_BIO_MODE										11	//0x0B
#define ID_MODE_MATCHER									ID_BIO_MODE
#define ID_DB_ENCRYPTION								14  //0x0E  //Determine whether the database is encrypted
#define ID_FIELD										15	//0x0F
#define ID_FIELD_NAME									ID_FIELD
#define ID_FIELD_SIZE									16	//0x10

#define ID_PUC_DATA										20	//0x14

#define ID_DESC_PRODUCT									41	//0x29
#define ID_DESC_SOFTWARE								42	//0x2A
#define ID_DESC_SENSOR									43	//0x2B
#define ID_COMPRESSION_NULL							    44	//0x2C

#define ID_FORMAT_TEXT									47	//0x2F
#define ID_FORMAT_BIN									48	//0x30
#define	ID_PRIVATE_FIELD								49	//0x31
#define	ID_FIELD_CONTENT								50	//0x32
#define ID_COMPRESSION_HUFFMAN							51	//0x33
#define ID_ASYNCHRONOUS_EVENT							52	//0x34
#define	ID_PKMAT_NORM									53	//0x35
#define	ID_USER_INDEX									54  //0x36
#define	ID_PKCOMP_NORM									55	//0x37
#define ID_BIO_ALGO_PARAM								56	//0x38
#define ID_LATENT_SETTING								57	//0x39
#define ID_PKBASE										58	//0x3A
#define ID_PKMOC										59	//0x3B
#define ID_COMPRESSION_V1								60	//0x3C
#define ID_EXPORT_IMAGE									61	//0x3D
#define	ID_COMPRESSION									62	//0x3E
#define ID_ISO_PK										63	//0x3F
#define ID_ISO_PK_PARAM									64	//0x40
#define ID_ISO_PK_DATA_ANSI_378							65	//0x41
#define	ID_FFD_LEVEL									66	//0X42
#define	ID_CODER_CHOICE									67	//0X43
#define	ID_MODE_DETECT									68	//0X44

#define ID_UPDATE_EXISTING_PK							75	//0x4B

#define ID_ISO_PK_DATA_ISO_FMR_2011						76 //0x4C
#define ID_ISO_PK_DATA_ANSI_378_2009					77 //0x4D

#define ID_X509_CERTIFICATE								80	//0x50
#define ID_PKCS12_TOKEN									81	//0x51
#define ID_CRYPTOGRAM_MUTUAL_AUTH						82	//0x52
#define ID_TKB_SETTINGS									83	//0x53
#define	ID_X984_PARAM									84	//0x54
#define	ID_X984											85	//0x55
#define ID_MATCHING_SCORE								86	//0x56
#define	ID_TKB											87	//0x57

#define ID_NO_CHECK_ON_TEMPLATE							96	//0x60

#define ID_EXPORT_TIMES									97	//0x61

#define ID_EXPORT_NUM_PK								99	//0x63
#define ID_OTP_SEQUENCE_NUMBER_MAX						100	//0x64
#define ID_OTP_PASSWORD									101	//0x65
#define ID_OTP_PIN										102	//0x66
#define ID_OTP_PARAM									103	//0x67
#define ID_OTP_ALGO_HASH								104	//0x68
#define ID_OTP_SEQUENCE_NUMBER							105	//0x69
#define ID_OTP_SEED										106	//0x6A
#define ID_ISO_PK_DATA_ILO_FMR							107	//0x6B
#define ID_ISO_PK_DATA_ISO_FMC_CS						108	//0x6C
#define ID_ISO_PK_DATA_ISO_FMC_NS						109	//0x6D
#define ID_ISO_PK_DATA_ISO_FMR							110	//0x6E
#define ID_ISO_PK_DATA_MINEX_A							111	//0x6F

#define ID_OTP_USER_DATA								113	//0x71

#define ID_FORMAT_BIN_VERSION							116	//0x74
#define ID_FORMAT_BIN_MAX_USER							117	//0x75
#define ID_FORMAT_BIN_MAX_DB							118	//0x76
#define ID_PK_FROM_SMARTCARD							118	//0x76
#define ID_PKV10										120	//0x78

#define ID_PK_MOC										123	//0x7B

#define ID_PK_DIN_V66400_CS								125	//0x7D
#define ID_PK_DIN_V66400_CS_AA							126	//0x7E
#define ID_ISO_PK_DATA_ISO_FMC_CS_AA					127	//0x7F
#define ID_FVP_PK_FORMAT								128	//0x80
#define ID_PK_FVP										129	//0x81

#define ILV_BOX_SECU									135	//0x87

#define ID_SAVE_IMAGE_INDEX								137	//0x89

#define ID_MODE											147	//0x93
#define ID_TYPE											148	//0x94

#define ID_ALIVE_MESSAGE_TIME							153	//0x99
#define ID_BIO_WINDOW_MODE								154	//0x9A

#define ID_COMPRESSION_WSQ								156	//0x9C

#define ID_NO_FVP										164	//0xA4
#define ID_FINGERPRINT_QUALITY							165	//0xA5
#define ID_ADVANCED_SECURITY_LEVELS_COMPATIBILITY		166	//0xA6
#define	ID_BIO_DATA_ENCRYPTION							167	//0xA7
#define ID_EXPORT_IMAGE_FFD								168	//0xA8

#define ID_MOC_PARAMETER								170	//0xAA
#define ID_PUC_APDU										171	//0xAB
#define ID_PUC_GENERIC_DATA								172	//0xAC

//FVP 1.10
#define ID_ACQUISITION_STRATEGY_MODE					174 //0xAE
#define ID_EXPORT_BEST_FP_IMAGE							175 //0xAF
#define ID_FORMAT_BIN_SOFT								182	//0xB6
#define ID_FORMAT_BIN_FLASH_SIZE						183	//0xB7
#define ID_FORMAT_BIN_PRODUCT_NAME						184	//0xB8
#define ID_FORMAT_BIN_PID								185	//0xB9
#define ID_FORMAT_BIN_SN								186	//0xBA
#define ID_FORMAT_BIN_OEM_PID							187	//0xBB
#define ID_FORMAT_BIN_OEM_SN							188	//0xBC
#define ID_FORMAT_BIN_SENSOR_ID							189	//0xBD
#define ID_FORMAT_BIN_SENSOR_SN							190	//0xBE
#define ID_FORMAT_BIN_LICENSES							191	//0xBF
#define ID_MIN_SECU_LEVEL								192	//0xC0

#define ID_FORMAT_BIN_CUSTOM_DESCRIPTOR					195	//0xC3

#define ID_PK_CFV										196	//  0xC4
#define ID_PK_BIOSCRYPT									197	//  0xC5

#define ID_PK_FVP_MATCH									198	//0xC6

// ..
#define ID_PKLITE							204				//  0xCC
#define ID_NOPK											255	//0xFF



/* ************************************************************
 * Extra Definitions
 * ************************************************************/
#define ID_WORFLOW_SMARTCARD_SAGEM_DEMO					1
#define ID_WORFLOW_SMARTCARD_CLIENT_GB					2

#define ID_BIO_ALGO_PARAM_PKCOMP						0
#define ID_BIO_ALGO_PARAM_PKCOMP_NORM					1
#define ID_BIO_ALGO_PARAM_PKMAT							2
#define ID_BIO_ALGO_PARAM_PKMAT_NORM					3

#define MSO_CODER_DEFAULT								0
#define	MSO_CODER_V9									3
#define MSO_CODER_V9_JUV								7
#define MSO_CODER_V9_THIN_FINGER						8

#define LATENT_DETECT_DISABLE							0
#define LATENT_DETECT_ENABLE							1

#define EXPORT_MATCHING_SCORE_DISABLE					0
#define EXPORT_MATCHING_SCORE_ENABLE					1

#define LOADMOCKEYS_KENC_CHIFFERED_BY_KENCTRANS			0
#define LOADMOCKEYS_KENC_CHIFFERED_BY_CERTIFICATE		1


#endif /*__ILV_DEFINITIONS_H__*/

