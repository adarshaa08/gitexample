// The present software is not subject to the US Export Administration Regulations (no exportation license required), May 2012
/************************************************************
  Copyright (c) 2003, Morpho 
************************************************************/

#ifdef IMAGECOMPRESS_EXPORTS
#define COMPRESS_API __declspec(dllexport)
#else
#ifndef _MSO_LIB
#define COMPRESS_API __declspec(dllimport)
#else
#define COMPRESS_API
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

COMPRESS_API 
I COMPRESS_GetRawImageSize(
				  PUC	i_puc_CompressedImageWithHeader,
				  UL	i_ul_CompressedImageWithHeaderSize,
				  PUL	o_pul_UnCompressedImageWithHeaderSize
				  );

COMPRESS_API
I COMPRESS_UnCompressImage(
				  PUC	i_puc_CompressedImageWithHeader,
				  UL	i_ul_CompressedImageWithHeaderSize,
				  PUC	o_puc_RawImageWithHeader,
				  PUL	io_pul_RawImageWithHeaderSize
				  );

#ifdef __cplusplus
}
#endif /* __cplusplus */
