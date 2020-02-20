// The present software is not subject to the US Export Administration Regulations (no exportation license required), May 2012
// The present software is not subject to the US Export Administration Regulations (no exportation license required), May 2012
#ifndef __USB_DRV_INTERFACE_H
#define __USB_DRV_INTERFACE_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*****************************************************************************/
/*****************************************************************************/
int SpUsb_OpenEx
(
	void	**o_h_Mso100Handle,
	PC		i_pc_String,
	UL		i_ul_TimeOut,
	PVOID	io_pv_Param
);

/*****************************************************************************/
/*****************************************************************************/
int SpUsb_Close
(
	void	**i_h_Mso100Handle
);

/*****************************************************************************/
/*****************************************************************************/
int SpUsb_WriteFrame
(
	void	*i_h_Mso100Handle,
	UL		i_ul_Timeout,
	PUC		i_puc_Data,
	UL		i_ul_DataSize
);

/*****************************************************************************/
/*****************************************************************************/
int SpUsb_ReadFrame
(
	void	*i_h_Mso100Handle,
	UL		i_ul_Timeout,
	PUC*	io_ppuc_DataReceive,
	PUL		o_pul_DataSize
);

/*****************************************************************************/
/*****************************************************************************/
int SpUsb_EnumDevices
(
	PT_MSO_USB_DEVICE_PROPERTIES	*o_ppx_DeviceProperties,
	PUL								o_pul_NbDevices
);

/*****************************************************************************/
/*****************************************************************************/
int SpUsb_ReadFrameReleaseEx
(
	void	*i_h_Mso100Handle,
	PUC*	io_ppuc_Data
);

/*****************************************************************************/
/*****************************************************************************/
int SpUsb_ReleaseEnumDevices
( 
	PT_MSO_USB_DEVICE_PROPERTIES	*o_ppx_DeviceProperties,
	UL								i_ul_NbreDevices
);

/*****************************************************************************/
/*****************************************************************************/
int SpUsb_WaitForReboot
( 
	void	*i_ph_Mso100Handle,
	UL		i_ul_WriteTimeout,
	PUC		i_puc_Data,
	UL		i_ul_DataSize,
	UL		i_ul_WaitTimeOut
);


int SpUsb_ServerInfosRelease(unsigned char **io_ppuc_Data);
int SpUsb_Lock(MORPHO_HANDLE i_h_Handle, unsigned long i_ul_Timeout);
int SpUsb_UnLock(MORPHO_HANDLE	i_h_Handle);
int SpUsb_ServerInfos(  unsigned long i_ul_InfoType,
						unsigned long i_ul_InfoParam,
						unsigned char **io_ppuc_Data,
						unsigned long *o_pul_DataSize);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

