// The present software is not subject to the US Export Administration Regulations (no exportation license required), May 2012
#ifndef MSO_SETPK_H
#define MSO_SETPK_H


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

I MSO_SetPk(UC			i_uc_NbPk,
			PT_BUF_PK	i_px_Pk,
			PT_BUF		io_px_MainIlv,
			PUL			io_pul_MainIlvSize);

I MSO_GetPk(UC				i_uc_NbFinger,
			PUC				i_puc_ReceivedIlv,
			PUL				i_pul_ReceivedIlvSize,
			PT_EXPORT_PK	o_px_ExportPk);

I MSO_GetPkFVP(UC			i_uc_NbFinger,
			   PUC			i_puc_ReceivedIlv,
			   PUL			i_pul_ReceivedIlvSize,
			   PT_EXPORT_PK o_px_ExportPkFVP);


I MSO_SetTkb(PT_BUF_PK	i_px_X984,
			 UC			i_uc_PkIndex,
			 PT_BUF		io_px_MainIlv,
			 PUL		io_pul_MainIlvSize);

I MSO_SetIsoPk(PT_BUF_PK i_px_Pk, 
			   PT_BUF io_px_MainIlv, 
			   PUL	io_pul_MainIlvSize);

I MSO_GetPkFP_Quality(	UC			    i_uc_NbFinger,
						PUC			    i_puc_ReceivedIlv,
						PUL			    i_pul_Pos,
						PUC				io_puc_PkFpQuality );


I MSO_GetPkFVP_ASecuLevel(	UC			    i_uc_NbFinger,
							PUC			    i_puc_ReceivedIlv,
							PUL			    i_pul_Pos,
							PUC				io_puc_PkFvpALQ );


I MSO_GetPkFVPX984(	PUC				i_puc_ReceivedIlv,
					PUL				i_pul_ReceivedIlvSize,
					PT_EXPORT_PK	o_px_ExportPkFVP);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

