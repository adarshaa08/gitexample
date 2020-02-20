// The present software is not subject to the US Export Administration Regulations (no exportation license required), May 2012
#ifndef __BIODB_TYPES_H
#define __BIODB_TYPES_H


/*****************************************************************************/
/* DataBase struct */
/*****************************************************************************/

#define	MAX_FIELD_NAME_LEN		6	///< Field Name Max length (set for compatibilty whith the MA)
#define PUBLIC					0	///< Public field ID
#define	PRIVATE					1	///< Private field ID
#define FILTER					2	///< Filter field ID
#define STATISTIC				3	///< Statistics field ID

/**
* Bio data structure.
*/
typedef struct
{
	PUC			m_puc_Data;			///< Data buffer
	US			m_us_SizeData;		///< Size of Buffer
	UC			m_uc_IdData;		///< Data ID: ID_USER_ID or ID_USER_DATA
	UC			m_uc_RFU;			///< Reserved
}T_BIODB_DATA_TRANSPORT, *PT_BIODB_DATA_TRANSPORT;


///////////////////////////////////////////////////////////////////////
// Start Packed struct
///////////////////////////////////////////////////////////////////////

/**
* Field structure.
*/
#if defined(WIN32)
	#pragma pack(1)
	typedef struct
#elif defined(__GNUC__)
	typedef struct __attribute__((packed))
#else
	typedef __packed struct
#endif
{
	US			m_us_FieldMaxSize;						///< Field Max size
	UC			m_auc_FieldName[MAX_FIELD_NAME_LEN+1];	///< Field Name
	UC			m_uc_Right;								///< Field type: PUBLIC or PRIVATE
}T_FIELD, *PT_FIELD;


/**
* Public list field structure.
*/
#if defined(WIN32)
	typedef struct
#elif defined(__GNUC__)
	typedef struct __attribute__((packed))
#else
	typedef __packed struct
#endif
{
	UL			m_ul_UserIndex;						///< User index in database
	UL			m_ul_DataLenght;					///< Data length
	PUC			m_puc_Data;							///< Buffer Data
}T_TRANSPORT_PUBLIC_LIST_FIELD, *PT_TRANSPORT_PUBLIC_LIST_FIELD;


/**
* ILV database structure. Use in MSO_BioDB_CreateDb.c
*/
#if defined(WIN32)
	typedef struct
#elif defined(__GNUC__)
	typedef struct __attribute__((packed))
#else
	typedef __packed struct
#endif
{
	UC			m_uc_IndexDB;				///< Database Index
	UC			m_uc_FlashType;				///< Not used: set to 0
	US			m_us_UserMax;				///< Max number of user
	UC			m_uc_FingerMax;				///< Max number of PK
}T_ILV_DB, *PT_ILV_DB;


/**
* Additionnal field structure. Use in MSO_BioDB_GetBaseConfig.c
*/
#if defined(WIN32)
	typedef struct
#elif defined(__GNUC__)
	typedef struct __attribute__((packed))
#else
	typedef __packed struct
#endif
{
	US			m_us_FieldMaxSize;						///< Field Max size
	UC			m_auc_FieldName[MAX_FIELD_NAME_LEN];	///< field name
}T_ILV_ADD_FIELD, *PT_ILV_ADD_FIELD;


/**
* Base configuration structure. Use in MSO_BioDB_GetBaseConfig.c
*/
#if defined(WIN32)
	typedef struct
#elif defined(__GNUC__)
	typedef struct __attribute__((packed))
#else
	typedef __packed struct
#endif
{
	UC			m_uc_RequestStatus;				///< Request status
	UC			m_uc_Finger;					///< Number of finger per user		
	UL			m_ul_MaxRecord;					///< Max number of database record
	UL			m_ul_CurrentRecord;				///< Current number of databse record
	UL			m_ul_FreeRecord;				///< Number of free database record
	UL			m_ul_AddField;					///< Number of additionnal field
}T_ILV_BASE_CONFIG, *PT_ILV_BASE_CONFIG;


/**
* ILV User Find structure. Use in MSO_BioDB_FindUser.c
*/
#if defined(WIN32)
	typedef struct
#elif defined(__GNUC__)
	typedef struct __attribute__((packed))
#else
	typedef __packed struct
#endif
{
	UC			m_uc_IndexDB;					///< Database Index
	UL			m_ul_IdData;					///< UID Data 
	UL			m_ul_IndexUser;					///< Index User
}T_ILV_FIND_USER, *PT_ILV_FIND_USER;

/**
* Database Header
*/
#if defined(WIN32)
	typedef struct
#elif defined(__GNUC__)
	typedef struct __attribute__((packed))
#else
	typedef __packed struct
#endif
{
	UC			m_uc_SizeBioDbHdr;		// taille de la structure T_BIODB_DB_HDR au moment de la creation
	UC			m_uc_Version;			// Version du fichier "entete de la base"
	UC			m_uc_SizeFile;			// taille du fichier "entete de la base"
	UC			m_uc_IndexDB;			// Database index (0 to 15)
	US			m_us_UserMax;			// maximum number of user in base
	UC			m_uc_FingerMax;			// finger number
	UC			m_uc_DataMax;			// number of additional fields (including UID)
	UC			m_uc_NbData;			// 3*m_uc_FingerMax + m_uc_DataMax (number of fields)	(x3: templates FP, FV & PM)
	UC			m_uc_MaxFieldName;		// max length of field name
	UC			m_uc_BioDbEncrypted;	// indique si les données PK sont stockées sur le FS de manière cryptées
}T_BIODB_DB_HDR, *PT_BIODB_DB_HDR;

#if defined(WIN32)
#	pragma pack()
#endif

///////////////////////////////////////////////////////////////////////
// End Packed struct
///////////////////////////////////////////////////////////////////////

typedef struct
{
	PT_BIODB_DATA_TRANSPORT	m_px_DataPkFp;
	PT_BIODB_DATA_TRANSPORT	m_px_DataPkFv;
	PT_BIODB_DATA_TRANSPORT	m_px_DataPkPm;
	PT_BIODB_DATA_TRANSPORT	m_px_DataData;
	UC						m_uc_NbPk;
	UC						m_uc_NbData;
	UC						m_uc_RFU;
	UC						m_uc_RFU2;
}T_BIODB_DATA_TRAIN, *PT_BIODB_DATA_TRAIN;

typedef struct
{
	PUC			m_puc_FieldName;	// field name
	US			m_us_MaxSize;		// maximum length of a record
	UC			m_uc_FieldNameLen;	// length of m_puc_FieldName
	UC			m_uc_Rights;		// rights of this field
}T_BIODB_FIELD_INFO, *PT_BIODB_FIELD_INFO;

typedef struct
{
	UL		m_ul_SizeMaxAllDb;
	UL		m_ul_SizeMaxDb;
	UL		m_ul_MaxUser;
	UC		m_uc_NbData;
	UC		m_uc_MaxFieldName;
	UC      m_auc_Padding[2];
}T_BIODB_MAX_USER, *PT_BIODB_MAX_USER;

typedef struct
{
	PT_BIODB_DATA_TRAIN		m_px_DataTrain;
	PUS						m_pus_IndexUser;
	UC						m_uc_IndexDB;
	B						m_b_no_check_on_template;
	UC						m_uc_Pad[2];
}T_BIODB_ADDUSER, *PT_BIODB_ADDUSER;

#endif		//__BIODB_TYPES_H

