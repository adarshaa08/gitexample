
void shift_onebit (unsigned char *input, unsigned char *output);

void xor_128 (unsigned char *a, unsigned char *b, unsigned char *out);

void leftshift_onebit (unsigned char *input, unsigned char *output);

void
Convert_Hex_To_Ascii (const unsigned char *data, unsigned char *hex_ascii,
                      int input_data_size);
void
Convert_Ascii_To_Hex (unsigned char chiperdata[], unsigned char chiper[],
                      int input_data_len);

void
Crc16CCITT (unsigned char *pbyDataIn, unsigned int dwDataLen,
            unsigned char abyCrcOut[2]);

int
print_error_resp (int error);

int
Modem_Read_Write_Cmd (unsigned char cmd[], unsigned char response[50],
                      int size, int rcv_bytes, int timeout);

void
Print_Data (const char msg[], unsigned char data[], int size);


void convert_byte_string(char *pos,int len,unsigned char *byteString);
