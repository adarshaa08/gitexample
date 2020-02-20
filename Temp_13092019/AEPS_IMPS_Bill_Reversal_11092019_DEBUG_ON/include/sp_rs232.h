// The present software is not subject to the US Export Administration Regulations (no exportation license required), May 2012
#include "MSO_types.h"

/*******************************************************************************
-- definition
*******************************************************************************/
#define MAX_DATA_SIZE				1024

 	#define ACK_TIMEOUT					1000

/* Definitions added by R Picon on 4 12 2001 */

#define	PARTIAL_FRAME				0
#define COMPLETE_FRAME				1

#define SPRS232_RECEIVE_MAX		400 * 1024  //400ko
#define SPRS232_RECEIVE_ALLOC	SPRS232_RECEIVE_MAX +20

#ifdef __cplusplus
extern "C"
{
#endif

//////////////////////////////////////////////////////////
//		fonctions génériques
//		utilisées en callback
//////////////////////////////////////////////////////////
F_MORPHO_CALLBACK_COM_OPEN SPRS232_OpenEx;

F_MORPHO_CALLBACK_COM_SEND SPRS232_SendEx;

F_MORPHO_CALLBACK_COM_RECEIVE SPRS232_ReceiveEx;

F_MORPHO_CALLBACK_COM_RECEIVE_FREE SPRS232_ReceiveFreeEx;

F_MORPHO_CALLBACK_COM_CLOSE SPRS232_Close;

//////////////////////////////////////////////////////////

/*******************************************************************************
// Export functions definition
*******************************************************************************/
I SPRS232_SetBaudRate
(
	MORPHO_HANDLE	i_h_Mso100Handle,
	T__UINT32	i_dw_BaudRate /* PortageLP64: Change DWORD to T__UINT32 */
);

I SPRS232_Break
(
	MORPHO_HANDLE	i_h_Mso100Handle,
	UL i_ul_Time
);

BOOL SPRS232_IsInitialize
(
	MORPHO_HANDLE	i_h_Mso100Handle
);

I SPRS232_Write
(
	MORPHO_HANDLE	i_h_Mso100Handle,
	UC *i_puc_Buf,
	UL i_ul_Size,
	UL *o_pul_BytesWritten
);

I SPRS232_Read
(
	MORPHO_HANDLE	i_h_Mso100Handle,
	PUC	i_puc_Buf,
	UL	i_ul_Size,
	UL	i_ul_ReadTotalTimeoutConstant
);

I SPRS232_GetConfig 
(	
	MORPHO_HANDLE	i_h_Mso100Handle
);

////////////////////////////////////////////////////////////
//	fonctions utilisées seulement 
//	pour compatibilité avec les anciens programmes
////////////////////////////////////////////////////////////

I SPRS232_Initialize
(
	MORPHO_HANDLE	*o_ph_Mso100Handle,
	C * i_strName,
	UL i_ul_BaudRate
);

I SPRS232_Send
(
	MORPHO_HANDLE	i_h_Mso100Handle,
	UC * i_pData,
	UL i_nDataSize
);

I SPRS232_Receive
(
	MORPHO_HANDLE	i_h_Mso100Handle,
	UC * i_puc_Buffer, 
	UL i_ul_BufferSize, 
	UL *o_pul_ILVSize,
	T__UINT32 i_dw_ReadTotalTimeoutConstant /* PortageLP64: Change DWORD to T__UINT32 */
);

I SPRS232_ReceiveOneFrame
(
	MORPHO_HANDLE	i_h_Mso100Handle,
	UC * i_puc_Buffer, 
	UL i_ul_BufferSize, 
	UL *o_pul_ILVSize,
	T__UINT32 i_dw_ReadTotalTimeoutConstant, /* PortageLP64: Change DWORD to T__UINT32 */
	UC*	o_puc_WholeFrameReceived
);

I SPRS232_NegociateBaudRate
(
	MORPHO_HANDLE	*i_ph_Mso100Handle,
	PC		i_pc_String,
	UL		i_ul_Param,
	PVOID	io_pv_Param
);

I
SPRS232_ConfigUart (
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

//---- Fonction de réception pour MA2G -----//
#ifdef _SPRS485
I SPRS485_Receive
(
	MORPHO_HANDLE	i_h_Mso100Handle,
	UC * i_puc_Buffer, 
	UL i_ul_BufferSize, 
	UL *o_pul_ILVSize,
	T__UINT32 i_dw_ReadTotalTimeoutConstant /* PortageLP64: Change DWORD to T__UINT32 */
);
#endif
//------------------------------------------//


#ifdef __cplusplus
}
#endif
