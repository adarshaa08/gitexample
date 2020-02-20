
/********************************************************
 *		     Integra Micro Software Services (P) Ltd
 *			#4, 12th KM, Bellary Road, 
 *			Jakkur, Bangalore 560064
 *		
 *		© Copyright 2006, IMSSPL, Bangalore, Karnataka
 *			All Rights Reserved
 *
 * @file	<ipd_base.h>
 * @ingroup Basic Datatypes
 *
 * @author	kiran            (kiranp@integramicro.com)
 * @date		1-12-2006
 * @brief   Basic Datatypes.

 ********************************************************
 * Revision History
 *------------------
 * Revision  1-Dec-2006 <Time> <modified by>
 * Brief Revision Description
 *
 *******************************************************/

/*******File Guard *******/
#ifndef  BASE_H
#define  BASE_H

/******** Header file includes*******************************/

/******** Macros ****************************************/

#define IC_NULL		NULL

#define BASE_APP_DIR			"/mnt/userfs/apps"
#define BASE_CONF_DIR			"/mnt/userfs/conf"


/******** Constants **************************************/

/******** External Globals ********************************/

/******** Typedefs, Structs and Unions **********************/
typedef char              IC_INT8 ;
typedef short             IC_INT16 ;
typedef int               IC_INT32 ;
typedef long              IC_INT64 ;
typedef unsigned char     IC_UINT8 ;
typedef unsigned short    IC_UINT16 ;
typedef unsigned int      IC_UINT32 ;
typedef unsigned long     IC_UINT64 ;
typedef double            IC_DOUBLE ;
typedef float             IC_FLOAT ;
typedef void              IC_VOID ;
/******** Enums ****************************************/

typedef enum
{
	E_EN_NOR,	/* Normal Enrollment */
	E_EN_WOC,	/* Exec WithOut Card Enrollment */
	E_EN_WPC,	/* Exec Physical With Card Enrollment */
	E_EN_WVC	/* Exec Virtual With Card Enrollment */
}E_EXEC_ENROLL_TYPE;


/******** External Function Prototypes **********************/

#endif //IPDBASE_H
/* End of File */
