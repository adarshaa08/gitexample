#ifndef GETPROFILE_H
#define GETPROFILE_H

typedef struct
{
    //***********************************     REQUEST DATA     ***************************************//

    unsigned char ucRequestID_GPReq[30];
    unsigned char ucMethodID_GPReq[3];
    unsigned char ucTellerID_GPReq[5];
    unsigned char ucSessionID_GPReq[40];
    unsigned char ucTokenID_GPReq[10];
    unsigned char ucIsEncrypt_GPReq[8];
    unsigned char ucUserTypeID_GPReq[10];
//	unsigned char ucPassword_GPReq[50];
//	unsigned char ucClient_id_GPReq[25];
//	unsigned char ucIP_GPReq[25]; // 10
    unsigned char ucRDChannelID_GPReq[10]; // Request Data Channel ID
    unsigned char ucproductTypeID_GPReq[10];
    unsigned char ucTransactionTypeID_GPReq[10];
//	unsigned char ucISP_GPReq[25];
//	unsigned char ucBrowser_GPReq[50];
//	unsigned char ucVersion_GPReq[25];
//	unsigned char ucPostalCode_GPReq[25];
//	unsigned char ucMAC_DeviceID_GPReq[25];
//	unsigned char ucCellID_GPReq[25];
//	unsigned char ucDeviceModel_GPReq[50];
//	unsigned char ucDeviceOS_GPReq[25];
//	unsigned char ucMCC_GPReq[25]; //20
//	unsigned char ucMNC_GPReq[25];
//	unsigned char ucLanguageSupported_GPReq[25];
//	unsigned char ucgeoLocation_GPReq[25];
//	unsigned char ucdeviceId_GPReq[50];
//	unsigned char ucxauthToken_GPReq[25];
//	unsigned char ucBiometric_fp_GPReq[25];
//	unsigned char ucOtp_GPReq[25];
//	unsigned char ucAadhaar_GPReq[25];
    unsigned char ucSessionExpiryTime_GPReq[25];
    unsigned char ucX_Auth_Token_GPReq[25]; //30
    unsigned char ucChannelID_GPReq[25];
    unsigned char ucProductCode_GPReq[25];
    unsigned char ucAmount_GPReq[25];
    unsigned char ucMerchantContribution_GPReq[25];
    unsigned char ucCharges_GPReq[25];
    unsigned char ucRouteID_GPReq[25];
    unsigned char ucSplitReferenceNo_GPReq[25];
    unsigned char ucServiceID_GPReq[25];
    unsigned char ucIdentifier_GPReq[25]; //40
    unsigned char ucTxnHash_GPReq[25]; //41


    //***********************************     RESPONSE DATA     ***************************************//

    unsigned char ucRequestId_GPRes[30];
    unsigned char ucDisplayMessage_GPRes[25];
    unsigned char ucResponseMessage_GPRes[30];
    unsigned char ucResponseCode_GPRes[4];
    unsigned char ucMessageType_GPRes[10];
    unsigned char ucCacheMaster_GPRes[4];
//	unsigned char ucid_token_GPRes[10];
//	unsigned char uctoken_type_GPRes[15];
//	unsigned char ucidentifier_GPRes[10];
//	unsigned char ucname_GPRes[15];
//	unsigned char ucUCcode_GPRes[5]; 			// user class code 10
//	unsigned char ucdescription_GPRes[10];
//	unsigned char ucmaxDaysBackdatedAllowed_GPRes[10];
//	unsigned char ucmaxDaysFutureDatedAllowed_GPRes[10];
//	unsigned char ucBAcode_GPRes[8]; 			// branch associated code
//	unsigned char ucBAdescription_GPRes[10]; 	// branch associated description
//	unsigned char ucifsc_GPRes[12];
//	unsigned char ucmicr_GPRes[10];
//	unsigned char ucgrid_GPRes[10];
//	unsigned char ucdefaultCostCenter_GPRes[5];
//	unsigned char uctype_GPRes[4];
//	unsigned char ucBTdescription_GPRes[10]; 	// branch Type description
//	unsigned char ucsubType_GPRes[5];
//	unsigned char ucsubtypedescription_GPRes[15];
//	unsigned char ucaddressLine1_GPRes[40];
//	unsigned char ucaddressLine2_GPRes[40];
//	unsigned char uccity_GPRes[20];
//	unsigned char ucstate_GPRes[15];
//	unsigned char ucstateDescription_GPRes[15];
//	unsigned char ucpinCode_GPRes[6];
//	unsigned char uccountry_GPRes[15];
//	unsigned char uccashOpen_GPRes[5];
//	unsigned char uccts_GPRes[5];
//	unsigned char ucgridBranch_GPRes[5];
//	unsigned char ucallowedToChangePostingDate_GPRes[5];
//	unsigned char uccurrencyEnvironment_GPRes[10];
//	unsigned char ucprimaryCashLimit_GPRes[10];
//	unsigned char uctellerIdentifier_GPRes[10];
//	unsigned char ucpasswordExpired_GPRes[5];
//	unsigned char ucfPIndex_GPRes[5];
//	unsigned char ucmobileNumber_GPRes[10];
//	unsigned char ucstatus_GPRes[5];
//	unsigned char ucrevoked_GPRes[5];
//	unsigned char ucsucceeded_schemes_GPRes[10];
//	unsigned char ucaccess_token_GPRes[8];
//	unsigned char ucreturnCode_GPRes[40];
//	unsigned char ucfirstLoginKilled_GPRes[5];
//	unsigned char ucrefresh_expires_in_GPRes[5];
//	unsigned char ucaccountNo_GPRes[10];
//	unsigned char ucledgerBalance_GPRes[10];
//	unsigned char ucavailableBalance_GPRes[10];
//	unsigned char uccurrency_GPRes[10];
//	unsigned char ucLDresponseMessage_GPRes[15]; // Login Data Response Message
//	unsigned char ucexpires_in_GPRes[15];
//	unsigned char ucerrors_GPRes[10];
//	unsigned char ucforce_change_password_GPRes[5];
//	unsigned char uctellerProofList_GPRes[10];
//	unsigned char ucLastLogin_GPRes[25];
//	unsigned char ucRDSessionID_GPRes[10]; 		  // Response Data Session ID
//	unsigned char ucNoOfFinger_GPRes[5];
//	unsigned char ucThreshold_GPRes[5];
//	unsigned char ucusertypeID_GPRes[5];
//	unsigned char ucuserRole_GPRes[5];
//	unsigned char uceAgreement_GPRes[5];
//	unsigned char uceSurvey_GPRes[5];
//	unsigned char ucchannelID_GPRes[5];
//	unsigned char ucEkyc_GPRes[5];
//	unsigned char ucRoles_GPRes[10];
//	unsigned char ucMaxMinLimit_GPRes[20];
    unsigned char ucMessageId_GPRes[10];
    unsigned char ucSessionExpiryTime_GPRes[20];
    unsigned char ucSessionId_GPRes[40];
    unsigned char ucRouteID_GPRes[40];
    unsigned char ucErrorCode_GPRes[5];
    unsigned char ucXMLData_GPRes[20];
    unsigned char ucAuthmanFlag_GPRes[5];
    unsigned char ucServiceID_GPRes[5];

}GProfile;





#endif // GETPROFILE_H
