#include<stdio.h>
#include<stdlib.h>
#include<string.h>

//#define DEBUG 1
#define MODEM_OPEN_FAIL -8

enum error
{ READ_FAIL = -2, WRITE_FAIL, SUCCESS, ILLEGAL_INDEX,
  INPUT_ILLEGAL_INDEX, INPUT_ILLEGAL_VARIBLE_LENGTH,
  CANCEL_INPUT, OVERTIME_TO_INPUT, KEYS_MISMATCHED, OPEN_FAIL,
  KEY_EXIST, KEY_LEN_FAIL, MASTERKEY_LEN_LESS_THAN_DERIVEKEY,
  ERROR, KEY_NOT_EXIT, KEY_WRONG
};

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

void Write_Key_Bloclk_In_File (const key_block_t * KeyBlk);

/********************************** COMMON FUNCTIONS **********************************/
void
Crc16CCITT (unsigned char *pbyDataIn, unsigned int dwDataLen,
            unsigned char abyCrcOut[2]);

void shift_onebit (unsigned char *input, unsigned char *output);

void xor_128 (unsigned char *a, unsigned char *b, unsigned char *out);

void leftshift_onebit (unsigned char *input, unsigned char *output);

void
Convert_Hex_To_Ascii (const unsigned char *data, unsigned char *hex_ascii,
		      int input_data_size);
void
Convert_Ascii_To_Hex (unsigned char chiperdata[], unsigned char chiper[],
		      int input_data_len);

void Derive_K1_K2 (unsigned char L[], keys_t * key);

void Derive_KM1_KM2 (keys_t * key);

void
Encrypt_Header (const keys_t * key, unsigned char header[],
		unsigned char Encrypted_Header[]);
void
Generate_Mac (const keys_t * key, const conf_key_t * Secure_Data,
	      unsigned char header[], unsigned char MAC[]);
void
Derive_Key_Block_Encryption_Key (keys_t * key,
				 unsigned char kbek_random_data[16]);
void
Derive_Key_Block_Mac_Key (keys_t * key,
			  unsigned char mac_key_random_data[16]);

void Key_Generation (keys_t * key);

/********************************** DECRYPTION FUNCTIONS **********************************/

void Read_Data_From_File (key_block_t * KeyBlk);

void
Get_Plain_Key (keys_t * key, unsigned char mac[8],
	       unsigned char enc_key[], unsigned char plain_key_data[]);
void
Cal_Chiperdata_To_Normaldata (keys_t * key, key_block_t * KeyBlk,
			      conf_key_t * Secure_Data);

int Verfiying_Mac (const key_block_t * KeyBlk, unsigned char mac[]);

/*************************************************************************************/
void Print_Data (const char msg[], unsigned char data[], int size);
int
print_error_resp (int error);
