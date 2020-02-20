// The present software is not subject to the US Export Administration Regulations (no exportation license required), May 2012
#ifndef MSO_SETCOMMONBIOPARAM_H
#define MSO_SETCOMMONBIOPARAM_H

#include "MORPHO_Types_Ext.h"

#ifdef MSO100_EXPORTS
#define MSO100_API __declspec(dllexport)
#else
#ifndef _MSO_LIB
#define MSO100_API __declspec(dllimport)
#else
#define MSO100_API
#endif
#endif


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define NO_COMMAND_LATENT	0

MSO100_API
I
MSO_BioParamDefaultPC
(
	PT_BIO_PARAM_PC			i_px_BioParamPC,
	I						i_i_security_level,
	I						i_i_Algo,
	UL						i_ul_DetectModeChoice,
	UL						i_ul_MatcherModeChoice,
	UL						i_ul_AsynchronousEvent,
	PUL						io_pul_score,
	PT_EXPORT_IDENT_TIMERS	o_px_Timers,
	UC						i_uc_LatentDetection
);

I MSO_SetCommonBioParamPC(	PT_BUF				io_px_MainIlv,
							PUL					io_pul_MainIlvSize,
							PT_BIO_PARAM_PC		o_px_BioParamPC);

I MSO_SetFFDSecuLevel(	PT_BUF	io_px_MainIlv,
						PUL		io_pul_MainIlvSize,
						I		i_i_ffd_security_level);
I MSO_SetCoderChoice(	PT_BUF	io_px_MainIlv,
						PUL		io_pul_MainIlvSize,
						I		i_i_Algo);

I MSO_SetDetectModeChoice(	PT_BUF		io_px_MainIlv,
							PUL			io_pul_MainIlvSize,
							UL			i_ul_DetectMode,
							UL			i_ul_DefaultMode);

I MSO_SetAsynchronousEvent(	PT_BUF		io_px_MainIlv,
							PUL			io_pul_MainIlvSize,
							UL			i_ul_AsynchronousEvent);

I MSO_SetMatcherModeChoice(	PT_BUF		io_px_MainIlv,
							PUL			io_pul_MainIlvSize,
							UL			i_ul_MatcherMode);

I MSO_SetLatent(PT_BUF	io_px_MainIlv,
				PUL		io_pul_MainIlvSize,
				UC		i_uc_LatentDetection);

I MSO_SetExportTimer(	PT_BUF					io_px_MainIlv,
						PUL						io_pul_MainIlvSize,
						PT_EXPORT_IDENT_TIMERS	o_px_Timers);

I MSO_Get_IdentTimer(	PUC i_puc_ILV_ReceiveBuffer,
						PUL	io_pul_Pos,
						PT_EXPORT_IDENT_TIMERS	o_px_IdentTimers);

I MSO_SetExportMatchingScore(	PT_BUF		io_px_MainIlv,
								PUL			io_pul_MainIlvSize,
								PUL			io_pul_score);

I SetExportCoderResults(PT_BUF	io_px_MainIlv,
						PUL		io_pul_MainIlvSize,
						UC		i_uc_ExportCoderResult);

I GetCoderResults(	UC		i_uc_ExportCoderResult,
					PT_BUF	io_px_ExportResultCoder,
					PT_BUF	io_px_ExportMatrixDirections,
					PT_BUF	io_px_ExportMatrixQualities,
					UC		i_uc_NbFinger,
					PUC		i_puc_ILV_ReceiveBuffer,
					PUL		io_pul_Pos);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

