
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
** File :  glHeaderGSM.h
** Description : This file is a part of SelfDiag Application Project for GL-11.
**		 It contains prototype declaration of functions required By C-
**		 file using in GSM-GPRS module.
**
** Modified By :
** Modified Date :
**
***********************************************************************************
***********************************************************************************/

#ifndef GLHEADERGSM_H
#define GLHEADERGSM_H


/*
 * This header file includes all prototype declaration of C-functions
 * @required by GSM-GPRS Communication Module.
 */

//int ModemOpen(const char *str);
//int Modemclose();
//int setmodemparams(int);
//int Test_modem_response_without_signal(const char*,char*,char**,int);

int  pppdial_cdma(char *dialno);
int  pppdial_gprs(char *dialno);
int  pppondial_pstn(char *dialno);
int  ppp_options(char *userid,char *password);
int  ppp_dial_update(char *dialno,char *gprsdomain);
int  ppp_on_dial(char *dialno);
int  ppp_options_ruim(char *userid,char *password);
int  ppp_pstn(char *dialno,char *userid,char *password,char *localip,
                char *remoteip,char *netmask);
void ppp_close();
int  ppp_open();

//for Gsm
int modem_open(void) ;
int modem_close (void) ;
char * modem_config (char *) ;
int modem_dial (const char *) ;
int modem_tx_data (unsigned char * , int) ;
int modem_rx_data(unsigned char *,int) ;
int modem_disconnect (void) ;
void alarm_handler (int signum);


#endif //GLHEADERGSM_H
