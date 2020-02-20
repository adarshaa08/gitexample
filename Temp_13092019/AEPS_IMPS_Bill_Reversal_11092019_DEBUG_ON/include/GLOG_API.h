// The present software is not subject to the US Export Administration Regulations (no exportation license required), May 2012
/************************************************************/
/**
 * @file GLOG_API.h
 * @brief Log API
 *
 * Copyright &copy; 2014, Morpho
 *
 * - PROJECT     : MorphoSmart&tm;
 * - MODULE      : GLOG
 *
 */
#ifndef GLOG_API_H_
#define GLOG_API_H_

#ifdef WIN32
	#ifdef GLOG_EXPORTS
	#define GLOG_API __declspec(dllexport)
	#else
	#define GLOG_API __declspec(dllimport)
	#endif
#else
	#if __GNUC__ >= 4
		#ifdef GLOG_EXPORTS
		#define GLOG_API __attribute__ ((visibility ("default")))
		#else
		#define GLOG_API
		#endif
	#else
		#define GLOG_API
	#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif


/** @brief Enumeration containing the logging mode. */
typedef enum
{
	GLOG_MODE_ENABLE = 0, /**< The logging is enabled */
	GLOG_MODE_DISABLE = 1 /**< The logging is disabled whatever the configuration of the groups */
} E_GLOG_Mode;

/* Number of modes; a valid mode is < GLOG_NB_MODE. */
#define GLOG_NB_MODE  2

/** @brief Enumeration containing the logging level (in increasing order). */
typedef enum
{
	GLOG_NOLOG = 0,         /**< Logging disabled */
	GLOG_LEVEL_ERROR = 1,   /**< For recording the traces with the level ERROR */
	GLOG_LEVEL_WARNING = 2, /**< for recording the traces with the level ERROR or WARNING */
	GLOG_LEVEL_INFO = 3,    /**< For recording the traces with the level ERROR or WARNING or INFO */
	GLOG_LEVEL_DEBUG = 4    /**< For recording the traces with the level ERROR or WARNING or INFO or DEBUG */
} E_GLOG_Level;

/* Number of levels; a valid level is < GLOG_NB_LVL. */
#define GLOG_NB_LVL	5

/** @brief Enumeration containing the error codes. */
typedef enum
{
	GLOG_NO_ERROR,
	GLOG_BAD_PARAMETERS,
	GLOG_ALREADY_INITIALIZED,
	GLOG_FILTERED_EVENT,
	GLOG_UNAVAILABLE,
	GLOG_WRITE_ERROR,
	GLOG_MEMORY_ERROR
} E_GLOG_Error;

/** @brief Type of a group identifier. */
#ifdef WIN32
typedef unsigned __int32 T_GLOG_Id;
#else
typedef unsigned int     T_GLOG_Id;
#endif


/*!
*	\fn E_GLOG_Error GLOG_Init(char* i_pc_CfgFile)
*	\brief Function initializing the module
*	\param[in] i_pc_CfgFile : path of the configuration file
*	\return GLOG_NO_ERROR : initialization succeeded
*	\return GLOG_BAD_PARAMETERS : i_pc_CfgFile is NULL or the configuration file is not found
*	\return GLOG_ALREADY_INITIALIZED : the module is already initialized, it must be closed before for an initialisation with a new configuration file.
*	\return GLOG_MEMORY_ERROR : memory error
**/
E_GLOG_Error GLOG_API GLOG_Init(char* i_pc_CfgFile);

/*!
*	\fn E_GLOG_Error GLOG_WriteLog(T_GLOG_Id i_ui_GroupId, E_GLOG_Level i_x_Level, char* i_pc_Format, ...)
*	\brief Function initializing the module
*	\param[in] i_ui_GroupId : identifier of the group
*	\param[in] i_x_Level : message level
*	\param[in] i_pc_Format : the formatted message to trace
*	\return GLOG_NO_ERROR : the message is traced
*	\return GLOG_UNAVAILABLE : log unavailable
*	\return GLOG_BAD_PARAMETERS : invalid identifier of the group 
*	\return GLOG_FILTERED_EVENT  : the message is not traced because of the current logging level of the group or the current logging mode. 
*	\return GLOG_WRITE_ERROR : error when writing
*	\return GLOG_MEMORY_ERROR : memory error
**/
E_GLOG_Error GLOG_API GLOG_WriteLog(T_GLOG_Id i_ui_GroupId, E_GLOG_Level i_x_Level, char* i_pc_Format, ...);

/*!
*	\fn E_GLOG_Error GLOG_SetGroupLevel(T_GLOG_Id i_ui_GroupId, E_GLOG_Level i_x_Level)
*	\brief Modify the logging level of a group (the modification is effective only in RAM memory and it is not reported in the configuration file).
*	\param[in] i_ui_GroupId : identifier of the group.
*	\param[in] i_x_Level : new logging level of the group.
*	\return GLOG_NO_ERROR : the modification succeeded
*	\return GLOG_UNAVAILABLE : log unavailable
*	\return GLOG_BAD_PARAMETERS : invalid identifier of the group 
**/
E_GLOG_Error GLOG_API GLOG_SetGroupLevel(T_GLOG_Id i_ui_GroupId, E_GLOG_Level i_x_Level);

/*!
*	\fn E_GLOG_Error GLOG_SetLoggingMode(E_GLOG_Mode i_x_Mode)
*	\brief Modify the logging mode (the modification is effective only in RAM memory and it is not reported in the configuration file).
*	\param[in] i_x_Mode : the new logging mode.
*	\return GLOG_NO_ERROR : the modification succeeded
*	\return GLOG_UNAVAILABLE : log unavailable
*	\return GLOG_BAD_PARAMETERS : invalid identifier of the group 
**/
E_GLOG_Error GLOG_API GLOG_SetLoggingMode(E_GLOG_Mode i_x_Mode);

/*!
*	\fn void GLOG_Close(void)
*	\brief Close the module.
**/
void GLOG_API GLOG_Close(void);


#ifdef __cplusplus
}
#endif

#endif /* GLOG_API_H_ */
