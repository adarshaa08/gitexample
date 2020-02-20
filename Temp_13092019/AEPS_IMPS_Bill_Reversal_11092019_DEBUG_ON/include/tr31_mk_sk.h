#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "common.h"
//#define MODEM_OPEN_FAIL -8
//#define MKSK 0
//#define DUKPT 1


struct Confidential_Key_Data
{

  unsigned char key_length[2];	//2bytes 
  unsigned char key[24];	//16bytes
  unsigned char padded_data[6];	//6bytes
};

typedef struct Confidential_Key_Data conf_key_t;


struct Key_Block
{

  unsigned char header[16];		/** Header to send **/
  unsigned char enc_key_ascii[64];	/** Ascii of the encrypted sensitive data need to send **/
  unsigned char mac_ascii[8];		/** Ascii of the mac **/

};

typedef struct Key_Block key_block_t;

struct Key
{

  unsigned char KBPK[16];	/** key_block_protection_key **/
  unsigned char KBEK[16];	/** key_block_encryption_key **/
  unsigned char MACKEY[16];	/** mac_key **/
  unsigned char KM1[8];		/** Sub Keys **/
  unsigned char KM2[8];		/** Sub Keys **/
  unsigned char K1[8];		/** Sub Keys **/
  unsigned char K2[8];		/** Sub Keys **/
};

typedef struct Key keys_t;



/********************************** ENCRYPTION FUNCTIONS **********************************/

void
Encrypt_Confidencial_Data (keys_t * key, conf_key_t * Secure_Data,
			   unsigned char header[], unsigned char Enc_Key[],
			   int len);


/********************************** COMMON FUNCTIONS **********************************/

int
TR31 (unsigned char header[], unsigned char key_to_send[], int len,
      unsigned char Key_Block_Protection_Key[], key_block_t * KeyBlk,unsigned char *padded_data,
      int mode);

void Derive_K1_K2 (unsigned char L[], keys_t * key);

void Derive_KM1_KM2 (keys_t * key);

void
Encrypt_Header (keys_t * key, unsigned char header[],
		unsigned char Encrypted_Header[]);
void
Generate_Mac (keys_t * key, conf_key_t * Secure_Data,
	      unsigned char header[], unsigned char MAC[]);
void
Derive_Key_Block_Encryption_Key (keys_t * key,
				 unsigned char kbek_random_data[16]);
void
Derive_Key_Block_Mac_Key (keys_t * key,
			  unsigned char mac_key_random_data[16]);

void Key_Generation (keys_t * key);

/********************************** DECRYPTION FUNCTIONS **********************************/

void
Get_Plain_Key (keys_t * key, unsigned char mac[8],
	       unsigned char enc_key[], unsigned char plain_key_data[]);
void
Cal_Chiperdata_To_Normaldata (keys_t * key, key_block_t * KeyBlk,
			      conf_key_t * Secure_Data);

/*************************************************************************************/
