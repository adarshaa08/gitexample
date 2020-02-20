// The present software is not subject to the US Export Administration Regulations (no exportation license required), May 2012
#ifndef __SPILV_H
#define __SPILV_H


/* include du fichier de définition des identifiants */
#include "Ilv_definitions.h"

#define I_L_MAX_SIZE	9	// (I)1 + (L)2 + (I étendu)2 + (L étendu)4

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

UL SizeOfI(PUC i_puc_ILV);
UL SizeOfL(PUC i_puc_ILV);
UL SizeOfIandL(PUC i_puc_ILV);
I ILV_Init(PUC i_puc_ILV, PUL io_ul_ILVSize, US i_us_I);
I ILV_Check(PUC i_puc_ILV, UL i_ul_ILVSize);
I ILV_AddValue(PUC i_puc_ILV, PUL io_ul_ILVSize, PUC i_puc_Value, UL i_ul_ValueSize);
I ILV_GetValue(PUC i_puc_ILV, PUL io_ul_Pos, PUC i_puc_Value, UL i_ul_ValueSize);
US ILV_GetI(PUC i_puc_ILV);
UL ILV_GetL(PUC i_puc_ILV);
UL ILV_GetSize(PUC i_puc_ILV);
PUC ILV_GetV(PUC i_puc_ILV);
void ILV_SetL(PUC i_puc_ILV, PUL io_ul_ILVSize, UL i_ul_LValue);
I ILV_GetILV( PUC i_puc_ILV, PUS o_pus_I, PUL	o_pul_L, PUC* o_ppuc_V);
I ILV_SetILV(PT_BUF io_px_BufIlv, US i_us_I, PT_BUF i_px_Data, PUL o_pul_IlvSize);
I ILV_AddILV(PT_BUF io_px_BufIlv, UL i_ul_SizeData, PUC i_puc_Data, PUL o_pul_IlvSize);
I ILV_GetErrorILV(PUC i_puc_V, PC o_pc_ILV_Status, PI o_pi_EmbeddedError);
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /*__SPILV_H*/

