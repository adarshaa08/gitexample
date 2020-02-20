// The present software is not subject to the US Export Administration Regulations (no exportation license required), May 2012
#ifndef _SPUSB_LIB_
#define _SPUSB_LIB_

#include "MSO100.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

//*****************************************************************
/* F_SPUSB_ENUMDEVICES */
typedef I	(F_SPUSB_ENUMDEVICES)
( 
	PT_MSO_USB_DEVICE_PROPERTIES	*o_ppx_DeviceProperties,
	UL								*o_pul_NbreDevices
);

typedef F_SPUSB_ENUMDEVICES *T_SPUSB_ENUMDEVICES;

F_SPUSB_ENUMDEVICES SpUsb_EnumDevices;


//*****************************************************************
/* F_SPUSB_RELEASEENUMDEVICES */
typedef I	(F_SPUSB_RELEASEENUMDEVICES)
( 
	PT_MSO_USB_DEVICE_PROPERTIES	*o_ppx_DeviceProperties,
	UL								i_ul_NbreDevices
);

typedef F_SPUSB_RELEASEENUMDEVICES *T_SPUSB_RELEASEENUMDEVICES;

F_SPUSB_RELEASEENUMDEVICES SpUsb_ReleaseEnumDevices;


//*****************************************************************
/* F_SPUSB_SERVERINFOS */
typedef I	(F_SPUSB_SERVERINFOS)
( 
	UL	i_ul_InfoType,
	UL	i_ul_InfoParam,
	UC 	**io_ppuc_Data,
	UL	*o_pul_DataSize
);

typedef F_SPUSB_SERVERINFOS *T_SPUSB_SERVERINFOS;

F_SPUSB_SERVERINFOS SpUsb_ServerInfos;

//*****************************************************************
/* F_SPUSB_SERVERINFOSRELEASE */
typedef I	(F_SPUSB_SERVERINFOSRELEASE)
( 
	UC 	**io_ppuc_Data
);

typedef F_SPUSB_SERVERINFOSRELEASE *T_SPUSB_SERVERINFOSRELEASE;

F_SPUSB_SERVERINFOSRELEASE SpUsb_ServerInfosRelease;

//*****************************************************************
/* F_SPUSB_LOCK */
typedef I	(F_SPUSB_LOCK)
( 
	MORPHO_HANDLE	i_h_Handle,
	UL				i_ul_Timeout
);

typedef F_SPUSB_LOCK *T_SPUSB_LOCK;

F_SPUSB_LOCK SpUsb_Lock;

//*****************************************************************
/* F_SPUSB_UNLOCK */
typedef I	(F_SPUSB_UNLOCK )
( 
	MORPHO_HANDLE	i_h_Handle
);

typedef F_SPUSB_UNLOCK  *T_SPUSB_UNLOCK ;

F_SPUSB_UNLOCK SpUsb_UnLock;


//*****************************************************************
/* F_SPUSB_WAITFORREBOOT */
typedef I	(F_SPUSB_WAITFORREBOOT )
( 
	MORPHO_HANDLE	i_h_Handle,
	UL				i_ul_WriteTimeout,
	PUC				i_puc_Data,
	UL				i_ul_DataSize,
	UL				i_ul_WaitTimeout
);

typedef F_SPUSB_WAITFORREBOOT  *T_SPUSB_WAITFORREBOOT ;

F_SPUSB_WAITFORREBOOT SpUsb_WaitForReboot;


//////////////////////////////////////////////////////////
//		fonctions génériques
//		utilisées en callback
//////////////////////////////////////////////////////////
F_MORPHO_CALLBACK_COM_OPEN SpUsb_OpenEx;

//////////////////////////////////////////////////////////
F_MORPHO_CALLBACK_COM_SEND SpUsb_WriteFrame;

//////////////////////////////////////////////////////////
F_MORPHO_CALLBACK_COM_RECEIVE SpUsb_ReadFrame;

//////////////////////////////////////////////////////////
F_MORPHO_CALLBACK_COM_RECEIVE_FREE SpUsb_ReadFrameReleaseEx;

//////////////////////////////////////////////////////////
F_MORPHO_CALLBACK_COM_CLOSE SpUsb_Close;


#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif

