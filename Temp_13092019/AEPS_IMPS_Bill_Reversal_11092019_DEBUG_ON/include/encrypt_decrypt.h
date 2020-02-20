#ifndef _DES_H_
#define _DES_H_
#include<string.h>
#include<stdio.h>
#define DES_ENCRYPT     1
#define DES_DECRYPT     0

//#define        DES3_PLAINTEXT_SIZE            16
#define	 DES3_PLAINTEXT_SIZE	16
//#define        DES3_PLAINTEXT_SIZE            256 //earlier we were using this
//#define        DES3_KEY_SIZE                          24              //168bit or 24 bytes
//#define        DES3_KEY_SIZE                          16              //168bit or 24 bytes
#define	 DES3_IV_SIZE				8


/** 32-bit integer manipulation macros (big endian)	 */
#ifndef GET_ULONG_BE
#define GET_ULONG_BE(n,b,i)                             \
{                                                       \
    (n) = ( (unsigned long) (b)[(i)    ] << 24 )        \
        | ( (unsigned long) (b)[(i) + 1] << 16 )        \
        | ( (unsigned long) (b)[(i) + 2] <<  8 )        \
        | ( (unsigned long) (b)[(i) + 3]       );       \
}
#endif	/*  */

#ifndef PUT_ULONG_BE
#define PUT_ULONG_BE(n,b,i)                             \
{                                                       \
    (b)[(i)    ] = (unsigned char) ( (n) >> 24 );       \
    (b)[(i) + 1] = (unsigned char) ( (n) >> 16 );       \
    (b)[(i) + 2] = (unsigned char) ( (n) >>  8 );       \
    (b)[(i) + 3] = (unsigned char) ( (n)       );       \
}
#endif
  typedef struct
{
  int mode;			/*!<  encrypt/decrypt   */
  unsigned long sk[32];		/*!<  DES subkeys       */
} des_context;

/**           Triple-DES context structure	 */
typedef struct
{
  int mode;			/*!<  encrypt/decrypt   */
  unsigned long sk[96];		/*!<  3DES subkeys      */
} des3_context;




void DES_setkey_enc (des_context * ctx, unsigned char key[8]);
void DES_setkey_dec (des_context * ctx, unsigned char key[8]);
void DES3_set2key_enc (des3_context * ctx, unsigned char key[16]);
void DES3_set2key_dec (des3_context * ctx, unsigned char key[16]);
void DES3_set3key_enc (des3_context * ctx, unsigned char key[24]);
void DES3_set3key_dec (des3_context * ctx, unsigned char key[24]);
void DES_crypt_ecb (des_context * ctx, unsigned char input[8],
		    unsigned char output[8]);
void DES_crypt_cbc (des_context * ctx, int mode, int length,
		    unsigned char iv[8], unsigned char *input,
		    unsigned char *output);
void DES3_crypt_ecb (des3_context * ctx, unsigned char input[8],
		     unsigned char output[8]);
void DES3_crypt_cbc (des3_context * ctx, int mode, int length,
		     unsigned char iv[8], unsigned char *input,
		     unsigned char *output);
int DES3_bufferDecryption (unsigned char *, unsigned char *input, int inLen,
			   unsigned char *output);
int DES3_bufferEncryption_iv (unsigned char *MKEY, unsigned char *input,
			      int inLen, unsigned char *output,
			      unsigned char *iv1);
int DES3_fileEncryption (char *freadpath, char *fwritepath);
int DES3_fileDecryption (char *freadpath, char *fwritepath);

int DES3_self_test (void);	//Testing purpose

int DES3_bufferEncryption_24 (unsigned char *HSK, unsigned char *D_RAND,int input_len, unsigned char *D_RAND_response);

int
DES_bufferEncryption (unsigned char DKEY[8], unsigned char *input, int inLen,
                      unsigned char *output);
int
DES3_bufferEncryption_16 (unsigned char MKEY[16], unsigned char *input,
                          int inLen, unsigned char *output);
#endif //_DES_H_
