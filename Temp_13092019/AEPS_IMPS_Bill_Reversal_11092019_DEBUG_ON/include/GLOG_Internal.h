// The present software is not subject to the US Export Administration Regulations (no exportation license required), May 2012
/************************************************************/
/**
 * @file GLOG_Internal.h
 * @brief Internal definitions, variables and functions.
 *
 * Copyright &copy; 2014, Morpho
 *
 * - PROJECT     : MorphoSmart&tm;
 * - MODULE      : GLOG
 *
 */
#ifndef GLOG_INTERNAL_H_
#define GLOG_INTERNAL_H_

#include "GLOG_API.h"
#include "PAL.h"

/* Structure containing the General configuration */
typedef struct
{
	char *       m_pc_LogFileName; /* Name of the common log file */
	char *       m_pc_LocationName; /* Name of the log files location */
	T_Int	     m_i_MaximumSize;  /*  Maximum size (in ko) of a log file */
	T_Int        m_i_NbBackups;  /*  Maximum number of backup files */
	E_GLOG_Mode  m_x_Mode; /* The logging mode (ENABLE / DISABLE) */
	E_GLOG_Level m_x_DefaultLevel; /* Default logging level */
	T_UInt       m_ui_NbGroups;  /* Number of groups of log */
} T_GLOG_Configuration;

/* Structure containing the parameters of a group */
typedef struct
{
	char *       m_pc_Name; /* Name of the group; GROUP_<X> if not defined in the configuration file */
	E_GLOG_Level m_x_level; /* Specific logging level for the group*/
	char *       m_pc_LogFileName; /* Name of the specific log file; NULL if not defined in the configuration file. */
} T_GLOG_GroupParam;

/* Global variable containing the general configuration */
extern T_GLOG_Configuration g_x_GLOG_Configuration;

/* Global variable containing the configuration of the groups */
extern T_GLOG_GroupParam*   g_px_GLOG_GroupParam;

/* Array of strings containing the log levels */
extern const char* const g_pc_LvlName[GLOG_NB_LVL];

/*!
*	\fn E_GLOG_Error GLOG_CFG_Init(char* i_pc_CfgFile)
*	\brief Load a configuration file
*	\param[in] i_pc_CfgFile : path of the configuration file
*	\return GLOG_NO_ERROR : load succeeded
*	\return GLOG_BAD_PARAMETERS : i_pc_CfgFile is NULL or the configuration file is not found
*	\return GLOG_MEMORY_ERROR : memory error
**/
E_GLOG_Error GLOG_CFG_Init(char* i_pc_CfgFile);

/*!
*	\fn void GLOG_CFG_Free(void)
*	\brief Release the memory used for the configuration.
**/
void GLOG_CFG_Free(void);



#endif /* GLOG_INTERNAL_H_ */
