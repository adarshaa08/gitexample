// The present software is not subject to the US Export Administration Regulations (no exportation license required), May 2012
#ifndef __RS232_ERRORS_H
#define __RS232_ERRORS_H

/***********************************************************************
							Rs232 PC Errors
 520 to 599 reserved

***********************************************************************/

/***********************************************************************
							driver RS232 Errors
 520 to 559 reserved

 file drv_rs232.c
***********************************************************************/

#define RS232_OK										 0
#define RS232ERR_ERROR									-520
#define RS232ERR_BUSY									-521
#define RS232ERR_INIT									-522
#define RS232ERR_INVALID_HANDLE_VALUE					-523
#define RS232ERR_IO_INCOMPLETE							-524
#define RS232ERR_OUTSTANDING_ASYNCHRONOUS_IO_REQUEST	-525
#define RS232ERR_IO_PENDING								-526
#define RS232ERR_WRITE_OPERATION_ABORTED				-527
#define RS232ERR_READ_OPERATION_ABORTED					-528
#define RS232ERR_BADPARAM								-529
#define	RS232ERR_WRITE_CE_BREAK							-530		//break condition.
#define	RS232ERR_WRITE_CE_DNS							-531		//Windows 95/98 : A parallel device is not selected. 
#define	RS232ERR_WRITE_CE_FRAME							-532		//framing error
#define	RS232ERR_WRITE_CE_IOE							-533		//I/O error
#define	RS232ERR_WRITE_CE_MODE 							-534		//parameter error
#define	RS232ERR_WRITE_CE_OOP							-535		//Windows 95/98 : out of paper
#define	RS232ERR_WRITE_CE_OVERRUN						-536		//character-buffer overrun : The next character is lost. 
#define	RS232ERR_WRITE_CE_PTO							-537		//Windows 95/98: A time-out occurred on a parallel device. 
#define	RS232ERR_WRITE_CE_RXOVER						-538		//input buffer overflowor or a character was received after the end-of-file (EOF)
#define	RS232ERR_WRITE_CE_RXPARITY						-539		//parity error
#define	RS232ERR_WRITE_CE_TXFULL						-540		//output buffer was full
#define	RS232ERR_READ_CE_BREAK							-541
#define	RS232ERR_READ_CE_DNS							-542
#define	RS232ERR_READ_CE_FRAME							-543
#define	RS232ERR_READ_CE_IOE							-544
#define	RS232ERR_READ_CE_MODE 							-545
#define	RS232ERR_READ_CE_OOP							-546
#define	RS232ERR_READ_CE_OVERRUN						-547
#define	RS232ERR_READ_CE_PTO							-548
#define	RS232ERR_READ_CE_RXOVER							-549
#define	RS232ERR_READ_CE_RXPARITY						-550
#define	RS232ERR_READ_CE_TXFULL							-551
#define RS232ERR_WRITE_TIMEOUT							-552
#define RS232ERR_READ_TIMEOUT							-553

#define RS232ERR_FIRST									RS232ERR_ERROR
#define RS232ERR_LAST									RS232ERR_READ_TIMEOUT

/***********************************************************************
							Service RS232 Errors
 560 to 599 reserved

 file sp_rs232.c
***********************************************************************/

#define SPRS232_OK_EOP									1		//plus utilisé à partir du 1/02/03
#define SPRS232_OK										0
#define SPRS232_ERR_ERROR								-560
#define SPRS232_ERR_INIT								-561
#define	SPRS232_BAD_STUFFING							-562
#define SPRS232_ERR_OPERATION_ABORTED					-563
#define SPRS232_ERR_BADPARAM							-564
#define SPRS232_ERR_BADSTX								-565
#define SPRS232_ERR_BADDATA								-566
#define SPRS232_ERR_BADID								-567
#define SPRS232_ERR_CRC									-568
#define SPRS232_ERR_NO_EOP								-569	//plus utilisé à partir du 1/02/03
#define SPRS232_ERR_EOP									-570
#define SPRS232_FRAME_NOT_COMPLETE						-571
#define SPRS232_ERR_CONFIG_UART							-572
#define SPRS232_ERR_SMALL_BUF							-573

#define SPRS232_ERR_FIRST								SPRS232_ERR_ERROR
#define SPRS232_ERR_LAST								SPRS232_ERR_SMALL_BUF
				
#endif	//__RS232_ERRORS_H				

