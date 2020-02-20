#ifndef KF311_P90_H
#define KF311_P90_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define MKSK 0
#define DUKPT 1
//#define SUCCESS 0
#define MODEM_OPEN_FAIL -1

/********************************** KEY  FUNCTIONS **********************************/

enum error { READ_FAIL = -2, WRITE_FAIL, SUCCESS, ILLEGAL_INDEX,
             INPUT_ILLEGAL_INDEX, INPUT_ILLEGAL_VARIBLE_LENGTH,
             CANCEL_INPUT, OVERTIME_TO_INPUT, KEYS_MISMATCHED, OPEN_FAIL,
             KEY_EXIST, KEY_LEN_FAIL, MASTERKEY_LEN_LESS_THAN_DERIVEKEY,
             ERROR, KEY_NOT_EXIT, KEY_WRONG
           };

/* This function used for opening the port for the communication */

int DES3_bufferEncryption_16(unsigned char*, unsigned char*, int, unsigned char*);

int Device_Open(char *port);

int  PINPAD_ModemOpen(char*);

int PINPAD_Modemclose ();

/* This function used for mutual authentication.Once mutual authentication success we can perform other operation otherwise not */

int  DoHandShake(char *port);

int handshake(void);
/* This function used save Pin_MK key in PINPAD with the provided index which is used to encrypt PINK*/
int
Save_Pin_MK (unsigned char key_to_send[],int key_length,
        unsigned char pin_mk_index, unsigned char Key_Block_Protection_Key[],unsigned char *random_padding,int mode);



/* This function used save PIN_KEY key in PINPAD with the provided index which is used to encrypt the PIN*/

/*
 * Enc Mode 
  0: plain text work key.
 
  1: DES or 3DES ECB encrypt  
 
  2: DES or 3DES ECB decrypt
 
  3: DES or 3DES CBC encrypt IV are all zero
 
  4: DES or 3DES CBC decrypt IV are all zero
 */

int
Save_PINK (unsigned char PIN_KEY[], int des_key_len,
           unsigned char desk_index,unsigned char des_mk_index,int enc_mode);



/* This function used save Des_MK key in PINPAD with the provided index which is used to encrypt Track Data*/
int
Save_Des_MK (unsigned char key_to_send[],int key_length,
        unsigned char des_mk_index, unsigned char Key_Block_Protection_Key[],unsigned char *random_padding,int mode);



int
Save_DESK (unsigned char DES_KEY[], int des_key_len,
           unsigned char desk_index,unsigned char des_mk_index,int enc_mode);

int Get_Encrypted_Track_Data(unsigned char *track_data,int len, unsigned char desk_index, unsigned char *enc_track_data);
/* This function used to get the encrypted PINBLOCK from the PINPAD */
int
Get_MK_SK_Encrypted_PIN (unsigned char card_number[], int card_len,
               unsigned char pinkey_index,unsigned char enc_pin_blk[],char *amount,int pin_blk_format);
int 
Save_Dukpt_Key (unsigned char *Dukpt_Derivation_Key,int dukpt_key_len,unsigned char dukpt_key_index,
                        unsigned char *KSN, unsigned char *Key_Block_Protection_Key,unsigned char *tr_31_dukpt_padding, int mode);

int Get_DUKPT_Encrypted_PIN (unsigned char *card_number, int card_len, unsigned char *enc_pin_blk,unsigned char *dukpt_ksn);
/* This function used to print the hexdata with the message */
void Print_Data (const char msg[], unsigned char data[], int size);

/* This function used to print the error code description */
int
print_error_resp (int error);

int set_currency_symbol(unsigned char *currency); 
int
Get_Des_EncDec_Data (unsigned char *data, int len,
                         unsigned char deskey_index,
                         unsigned char *encrypted_data,int enc_mode);


int Get_Serial_No(unsigned char serialno[16]);


int Get_Firmware_Ver(unsigned char firmware_ver[16]);
/*************************************************************************************/

#endif //KF311_P90_H
