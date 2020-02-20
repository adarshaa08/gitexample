// The present software is not subject to the US Export Administration Regulations (no exportation license required), May 2012
#ifndef __Crc_h
#define __Crc_h

/*******************************************************************************
-- ComputeCrc16 implementation
*******************************************************************************/
#ifdef __cplusplus
extern "C"
{
#endif

void ComputeCrc16 (PUC i_puc_Buffer, UL i_ul_Len, PUC io_puc_CrcH, PUC io_puc_CrcL);
void ComputeCrc8(PUC i_puc_data, I i_i_len, PUC o_puc_crc, UC i_uc_crc_init);

US ComputeCrc16_(unsigned char *buf, int len);

#ifdef __cplusplus
}
#endif

#endif /*__Crc_h*/
