// The present software is not subject to the US Export Administration Regulations (no exportation license required), May 2012
#ifndef __errors_h
#define __errors_h

#define RETURN_NO_ERROR							0

#define ERROR_INFO_TYPE_UNKNOW					-4		/* 0xFC */
#define ERROR_NOT_INITIALIZED					-5		/* 0xFB */
#define ERROR_MEMORY							-6		/* 0xFA */


#define	CRYPTO_BAD_PARAM						-217	/* 0x27 */

#define	ERROR_APPLI_BAD_I_VALUE					-341	/*0xEAB*/
#define	ERROR_APPLI_BAD_L_VALUE					-342	/*0xEAA*/
#define	ERROR_APPLI_ILV_BAD_PARAMETERS			-344	/*0xEA8*/
#define	ERROR_APPLI_ID_FIELD					-347

#define	COMPRESS_OVER_SIZE						-376	/*0xE88*/

#define BIO_ERROR_BAD_PARAMETER						-500	/*0xE0C*/
#define ILV_ERROR_INVALID_ILV					-600	/*0xDA8*/
#define ILV_ERROR_BAD_ILV_SIZE					-601	/*0xDA7*/
#define ILV_ERROR_NOT_ENOUGH_MEMORY				-602	/*0xDA6*/

#define COMPRESS_RET_CORRUPTED_IMAGE			-690	/*0xD4E*/
#define COMPRESS_RET_BAD_PARAMETER				-691	/*0xD4D*/
#define COMPRESS_RET_ALLOC_FAILED				ERROR_MEMORY
#define COMPRESS_INVALID_IMAGE					-692	/*0xD4C*/


#define SPILVERR_BAD_PARAMETER					-206	/*0x32*/
#define	ERROR_APPLI_BAD_PARAMETER				-330	/*0xEB6*/

#define CRYPTO_ASN1								-710	/* 0xD3A */
#define	CRYPTO_UNKNOWN_ALGORITHM				-711	/* 0xD39 */
#define	CRYPTO_X984_INVALID						-719	/* 0xD31 */
#define	CRYPTO_ERR_RSA_SIGN						-722	/* 0xD2E */
#define	CRYPTO_ERR_DSA_SIGN						-723	/* 0xD2D */
#define	CRYPTO_NOT_THIS_CERTIF					-724	/* 0xD2C */


#define STATUS_ERR_REF							-2455	/*0x669*/

#endif /*__errors_h*/
