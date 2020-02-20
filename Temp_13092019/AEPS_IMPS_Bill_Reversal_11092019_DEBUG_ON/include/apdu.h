#ifndef apdu_h
#define apdu_h
typedef struct
{
	int CLA; // Index to Class of instruction;
	int INS; // Index to Instruction code
	int P1;  // Index to Instruction parameter 1
	int P2;  // Index to Instruction parameter 2
	int Data;      // Index to Data
	int Lc;        // Index to a Number of bytes present in command data field
	int Le;        // Index to a Maximum number of data bytes expected in data field of response
	int SW1;       // Index to a status byte 1 (valid only in a response header)
	int SW2;       // Index to a status byte 2 (valid only in a response header)
}apdu_struct;

#define byte unsigned char
#define CLA_Indx   0
#define INS_Indx   1
#define P1_Indx    2
#define P2_Indx    3
#define Data_Indx  5

#endif
