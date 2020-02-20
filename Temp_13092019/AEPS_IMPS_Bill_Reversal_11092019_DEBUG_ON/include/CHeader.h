
/*********************************************************************************
********************************************************************************** 
**
** Copyright (C) 2011 VISIONTEK/Linkwell Telesystem Pvt. Ltd.
** 1-11-252/1/A, Gowra Klassic, Begumpet
** Hyderabad 500016 INDIA
**
**
** This program is free software; you can redistribute it and/or modify it under
** the terms of the GNU General Public License as published by the Free Software
** Foundation; either version 2 of the License, or (at your option) any later
** version.
**
** This program is distributed in the hope that it will be useful, but WITHOUT 
** ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
** FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
**
**
** Author : SANJEET K. KARN
** Dept: Research & Development
**
** File : CHeader.h 
** Description : This file is a part of SelfDiag Application Project for GL-11.
**		 It includes all the C Header files required in this application
**
** Modified By :
** Modified Date :
**
***********************************************************************************
***********************************************************************************/

 
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/signal.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/ioctl.h>
#include <string.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <time.h>
#include <net/if.h>
#include <dirent.h>
#include<math.h>

#define SUCCESS 0
#define ERROR -1
#define CANCEL 10
//#define FUNC_ERROR		      -1
//#define FUNC_CANCEL		      -2
//#define FUNC_OK                1

#define RESPONSE_TIMEOUT      -8
#define FUNCAL_FAILED         -7
#define FUNC_ERROR		   	  -111
#define FUNC_CANCEL		      -222
#define FUNC_OK                1
#define MD_BLANK_LINE					10136
#define ENTERTID				10150
#define DISP_CENTER			   2
#define LINE_THREE			   4
#define DELAY_TIME_OUT		100
#define MAX_GRAPHIC_DISP_SIZE			23
#define CLR_LINE						16
#define LINE_ONE                1
#define LINE_TWO				2
#define DISP_LEFT						1
#define DISP_RIGHT				4
#define LINE_FOUR				8
#define CANCEL_KEY            -12
#define _HOST					0
#define _SWAP					1
#define TIME_OUT			  -11
#define FARMER_APPL						0
#define GETPROFILE      1001
#define BANKMASTER		1002
#define MAX_REPEATED_LEN 32
#define MAX_STRING_LEN 32
#define MAX_BYTES_LEN 32

#define __DEFAULT_KEY "0123456789abcdef"
#define __DEFAULT_IV "fedcba9876543210"


