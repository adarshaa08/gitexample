// The present software is not subject to the US Export Administration Regulations (no exportation license required), May 2012
#ifndef __USB_ERRORS_H
#define __USB_ERRORS_H

#include "Rs232_Errors.h"
/* ------------------------------------------------ */
/*				ERROR Codes							*/
/* ------------------------------------------------ */

/****************************************************
	
	390 to 429 reserved

*****************************************************/

#define SPUSB_OK					0		//0x000			/* succesful operation */
#define SPUSB_ERR_CREATE_EVENT		-390	//0xE7A			/* error during an event creation opertaion */
#define SPUSB_ERR_TIMEOUT			RS232ERR_READ_TIMEOUT	//0xE79			/* timeout */
#define	SPUSB_ERR_OVERLAP			-392	//0xE78			/* internal overlap error */
#define SPUSB_ERR_NOT_SUPPORTED		-393	//0xE77			/* Version not supported */
#define SPUSB_NO_MEMORY				-394	//0xE76			/* memory allocation error */
#define SPUSB_CORRUPTED_FRAME		-395	//0xE75			/* received frame is corrupted */
#define SPUSB_OPEN_ERROR			-396	//0xE74	// Obsolete			/* open COM port error */
#define SPUSB_REGISTRY_ERROR		-397	//0xE73	// Obsolete			/* registry not found, installation has not been properly performed */
#define SPUSB_ALREADY_OPENED		-398	//0xE72			/* driver already opened */
#define SPUSB_NOT_OPEN				-399	//0xE71			/* driver is not opened */
#define SPUSB_FRAME_TOO_LONG		-400	//0xE70	// Obsolete			/* received length is too long */
#define SPUSB_ERR_FRAME_LEN			-401	//0xE6F			/* Frame integrity error */
#define SPUSB_ERR					-402	//0xE6E			/* */
#define SPUSB_DEVICE_NOT_PRESENT	-403	//0xE6D			/* The device desire is not plug in the system */
#define SPUSB_ERR_GET_LAST_ERROR	-404	//0xE6C			/* a System problem appears. To get extended error information, call GetLastError */
#define SPUSB_NO_MORE_DEVICES		-405	//0xE6B			/* No more devices to inform */
#define SPUSB_COMMUNICATION_SDK_ERR	-406	//0xE6A			/* GetCommTimeouts SDK function error */
#define SPUSB_MAX_IPC				-407	//0xE69			/* The Max number of IPC is reached
#define SPUSB_BAD_IPC_FORMAT		-408	//0xE68			/* The format of the IPC received is not correct
#define SPUSB_TIMEOUT_END_SYNC		-409	//0xE67 // Obsolete
#define SPUSB_TIMEOUT_DATA			-410	//0xE66 // Obsolete
#define SPUSB_TIMEOUT_END_FRAME		-411	//0xE65 // Obsolete
#define SPUSB_BAD_PARAMETER			-412	//0xE64
#define SPUSB_ERR_WAIT_ABANDONED	-413	//0xE63			/* WaitForSingleObject error */
#define SPUSB_SYNC_SDK_ERR			-414	//0xE62			/* Synchronization Functions SDK function error. To get extended error information, call GetLastError */
#define SPUSB_STOPING				-415	//0xE61			/* En cours d'arret */
#define SPUSB_CANCELIO_ERR			-416	//0xE60			/* CancelIo SDK function error */

/* Errors issue from ToolShare */
#define	NOK_TRANS_BADPARAMS			-417	//0xE5F
#define	NOK_TRANS_CLOSE				-418	//0xE5E
#define	NOK_TRANS_ERROR				-419	//0xE5D
#define	NOK_TRANS_INVALID			-420	//0xE5C
#define	NOK_TRANS_TIMEOUT			-421	//0xE5B
#define	NOK_TRANS_NOSERVER			-422	//0xE5A
#define	NOK_TRANS_SERVERUNKNOWN		-423	//0xE59
#define	NOK_TRANS_ERROR2			SPUSB_ERR_WAIT_ABANDONED
#define	NOK_TRANS_MEMALLOC			-424	//0xE58
#define	NOK_IPC_COMMAND				-425	//0xE57

#define SPUSB_ERR_FORBIDDEN_OP		-426	//0xE56			/* Forbidden operation : attempt to write a command during another command issued with the same handle */
#define SPUSB_ERR_BAD_RL			-427	//0xE55			

#define SPUSB_UNEXPECTED_FRAME		-428	//0XE54

#endif	//__USB_ERRORS_H

