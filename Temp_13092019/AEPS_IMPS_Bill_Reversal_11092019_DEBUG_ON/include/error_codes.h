
/********************************************************
 *		     Integra Micro Software Services (P) Ltd
 *			#4, 12th KM, Bellary Road, 
 *			Jakkur, Bangalore 560064
 *		
 *		© Copyright 2006, IMSSPL, Bangalore, Karnataka
 *			All Rights Reserved
 *
 * @file	<error_codes.h>
 * @ingroup Error Codes 
 *
 * @author	kiran            (kiranp@integramicro.com)
 * @date		1-12-2006
 * @brief   Error Codes for various Modules.

 ********************************************************
 * Revision History
 *------------------
 * Revision  1-Dec-2006 <Time> <modified by>
 * Brief Revision Description
 *
 *******************************************************/

/*******File Guard *******/
#ifndef  ERRORCODES_H
#define  ERRORCODES_H
#include "ipd_base.h"

/******** Header file includes*******************************/

/******** Macros ****************************************/
//Module Definitions


/******** Constants **************************************/
#define IC_OK			0
#define IC_ERR			-5
#define	IC_ERR_DEV_INIT		61
#define	IC_ERR_DEV_DEINIT	62

/******** External Globals ********************************/

/******** Typedefs, Structs and Unions **********************/
typedef IC_INT32		IC_RETCODE ;

/******** Enums ****************************************/

/******** External Function Prototypes **********************/

#endif //ERRORCODES_H
/* End of File */
