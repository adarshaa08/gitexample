// The present software is not subject to the US Export Administration Regulations (no exportation license required), May 2012
#ifndef COMP_H
#define COMP_H

typedef struct {
	enum {ONE_IMAGE=0, THREE_IMAGES=1, FOUR_IMAGES=2} m_i_image_number;
	UC	m_auc_Pad[3];
	int m_i_image0_bpp;		/* 0 à 8 */
	int m_i_image12_bpp;	/* 0 à 8 */
	int m_i_image3_bpp;		/* 0 à 8 */
}	T_CompressionParameters;

/* **********************************
 * Compression usage :
 * - ONE_IMAGE option implies subsampling
 * - {FOUR_IMAGES,8,8,8} = looseless compression
 * - (THREE_IMAGES,7,5,0) = recommanded values
 * - (THREE_IMAGES,6,4,0) = more compression
 * - (FOUR_IMAGES,8,7,5) = better quality
 */



/* compression function
 * buffers should be allocated :
 * io_puc_Image size is i_i_NCol*i_i_NLin
 * o_puc_CompressedImage size is set in *io_pi_CompressedImageSize
 * is should be at least i_i_NCol*i_i_NLin*3/4
 * io_puc_Image is modified by this function
 * io_puc_Image and o_puc_CompressedImage can be equal */
I	COMP_compress_image(
	I							i_i_Mode,					/* parameter. should be 0 */
	PUC							i_puc_Image,				/* input image. is modified !! */
	I							i_i_NCol,					/* number of columns */
	I							i_i_NLin,					/* number of lines */
	PUC							o_puc_CompressedImage,		/* compressed image */
	PUL							io_pul_CompressedImageSize,	/* compressed image size. is initialised to compressed image buffer size */
	T_CompressionParameters *	i_x_Parameters);

/* compression function
 * buffers should be allocated :
 * o_puc_Image size is i_i_NCol*i_i_NLin
 * o_puc_Image and i_puc_CompressedImage can be equal */
I	COMP_uncompress_image(
			I	i_i_Mode,					/* parameter. should be 0 */
			PUC o_puc_Image,				/* output image */
			I	i_i_NCol,					/* number of columns */
			I	i_i_NLin,					/* number of lines */
			PUC i_puc_CompressedImage,		/* compressed input image */
			I	i_i_CompressedImageSize		/* input image size */
			);

/* loseless compression
 * buffers should be allocated :
 * i_puc_Image size is i_i_NCol*i_i_NLin
 * o_puc_CompressedImage size is set in io_pi_CompressedImageSize.
 * It should be at least i_i_NCol*i_i_NLin
 * i_puc_Image and o_puc_CompressedImage cannot be equal */
I	COMP_loseless_compress_image(
	I	i_i_Mode,						/* parameter. should be 0 */
	PUC i_puc_Image,					/* input image */
	I	i_i_NCol,						/* number of columns */
	I	i_i_NLin,						/* number of lines */
	PUC o_puc_CompressedImage,			/* compressed image */
	PUL io_pul_CompressedImageSize);	/* compressed image size. is initialised to compressed image buffer size */


/* loseless uncompression
 * buffers should be allocated :
 * o_puc_Image size is i_i_NCol*i_i_NLin
 * o_puc_Image and i_puc_CompressedImage cannot be equal */
I	COMP_loseless_uncompress_image(
	I i_i_Mode,						/* parameter. should be 0 */
	PUC o_puc_Image,				/* output image */
	I i_i_NCol,						/* number of columns */
	I i_i_NLin,						/* number of lines */
	PUC i_puc_CompressedImage,		/* compressed input image */
	I i_i_CompressedImageSize);		/* input image size */

#endif
