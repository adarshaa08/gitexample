#ifndef LOGIN_H
#define LOGIN_H


typedef struct
{
    /*Request data*/

    unsigned char ucRequestID_req[25];
    unsigned char ucMethodID_req[5];
    unsigned char ucTellerID_req[10];
    unsigned char ucSessionID_req[3];
    unsigned char ucTokenID_req[25];
    unsigned char ucIsEncrypt_req[25];
    unsigned char ucUser_id_req[50];
    unsigned char ucClient_id_req[25];
    unsigned char ucold_user_id[50];
    unsigned char ucSessionExpiryTime_req[25];
    unsigned char ucX_Auth_Token_req[25];
    unsigned char ucChannelID_req[25];
    unsigned char ucProductCode_req[25];
    unsigned char ucAmount_req[25];
    unsigned char ucMerchantContribution_req[25];
    unsigned char ucCharges_req[25];
    unsigned char ucRouteID_req[25];
    unsigned char ucSplitReferenceNo_req[25];
    unsigned char ucServiceID_req[25];
    unsigned char ucIdentifier_req[25];
    unsigned char ucTxnHash_req[25];
   unsigned char ucresponseBitmap[3];
    /*Response data*/
    unsigned char ucRequestId[25];
    unsigned char ucDisplayMessage[25];
    unsigned char ucResponseMessage[100];
    unsigned char ucResponseCode[4];
    unsigned char ucMessageType[10];
    unsigned char ucreturnCode[4];
    unsigned char ucRDresponseMessage[30];//LD login data
    unsigned char ucuserId[10];
    unsigned char ucsubUserClass[2];
    unsigned char ucfPIndex[3];
    unsigned char ucaadharNumber[13];
    unsigned char ucbranchCode[5];
    unsigned char ucpolicy[30];
    unsigned char ucpriority[40];
    unsigned char ucabort_on_failure[40];
    unsigned char ucclient_id[40];
    unsigned char ucuser_roles[40];
    unsigned char ucmobileNumber[12];   //check this mob no
    unsigned char ucencryption_key[40];
    //unsigned char ucsecurity_profile[40];
    unsigned char uclast_login_time[40];
    unsigned char ucis_last_login_failed[40];
    unsigned char ucis_user_locked[40];
    unsigned char uclast_login_date[40];
    unsigned char ucfailure_count[40];
    unsigned char ucfailure_sec_question_count[40];
    unsigned char ucfirst_login_time[40];
    unsigned char ucfirst_login_date[40];
    unsigned char ucMessageId[40];
    unsigned char ucSessionExpiryTime[40];
    unsigned char ucSessionId[40];
    unsigned char ucRouteID[40];
    unsigned char ucErrorCode[40];
    unsigned char ucXMLData[40];
    unsigned char ucAuthmanFlag[40];
    unsigned char ucServiceID[40];
    unsigned char ucrequestId[20];
    unsigned char ucCAreturnCode[25];
    unsigned char ucCAresponseMessage[40];
    unsigned char ucUAreturnCode[5];
    unsigned char ucUAresponseMessage[40];
    unsigned char uclast_login_ip[40];

    /*

    unsigned char ucid_token[15];
    unsigned char ucdate2[30];
    unsigned char uctoken_type[15];
    unsigned char ucidentifier[8];
    unsigned char ucname[8];
    unsigned char ucUCcode[8];//UC for User class
    unsigned char ucUCdescription[40];
    unsigned char ucmaxDaysBackdatedAllowed[2];
    unsigned char ucmaxDaysFutureDatedAllowed[2];
    unsigned char ucBAcode[8]; //BA for BranchAssociated
    unsigned char ucBAdescription[20];
    unsigned char ucifsc[15];
    unsigned char ucmicr[15];
    unsigned char ucgrid[3];
    unsigned char ucdefaultCostCenter[8];
    unsigned char ucBranchtype[2];
    unsigned char ucBTdescription[20]; //BT for branchtype
    unsigned char ucsubType[5];
    unsigned char ucsubtypedescription[20];
    unsigned char ucaddressLine1[40];
    unsigned char ucaddressLine2[40];
    unsigned char uccity[20];
    unsigned char ucstate[10];
    unsigned char ucstateDescription[25];
    unsigned char ucpinCode[8];
    unsigned char uccountry[6];
    unsigned char uccashOpen[7];
    unsigned char uccts[7];
    unsigned char ucgridBranch[7];
    unsigned char ucallowedToChangePostingDate[8];
    unsigned char uccurrencyEnvironment[3];
    unsigned char ucprimaryCashLimit[15];
    unsigned char uctellerIdentifier[5];
    unsigned char ucpasswordExpired[8];
    //unsigned char ucfPIndex[3];
    unsigned char ucstatus[3];
    unsigned char ucrevoked[8];
    unsigned char ucsucceeded_schemes[15];
    unsigned char ucaccess_token[20];
    unsigned char ucfirstLoginKilled[8];
    unsigned char ucrefresh_expires_in[5];
    unsigned char ucBLaccountNo[15];      //BL for Balance List
    unsigned char ucledgerBalance[15];
    unsigned char ucavailableBalance[15];
    unsigned char uccurrency[5];
    unsigned char ucexpires_in[20];
    unsigned char ucerrors[20];
    unsigned char ucforce_change_password[8];
    unsigned char uctellerProofList[10];
    unsigned char ucLastLogin[30];
    unsigned char ucRESPSession_ID[50];
    unsigned char ucNoOfFinger[5];
    unsigned char ucThreshold[3];
    unsigned char ucusertypeID[3];
    unsigned char ucuserRole[3];
    unsigned char uceAgreement[3];
    unsigned char uceSurvey[3];
    unsigned char ucchannelID[3];
    unsigned char ucEkyc[3];
    unsigned char ucRoles[15];
    unsigned char ucMaxMinLimit[20];
*/

}Login_leg1;

typedef struct
{
    /*Request data*/

    unsigned char ucUsername_req[10];
    unsigned char ucRequestID_req[25];
    unsigned char ucMethodID_req[5];
    unsigned char ucTellerID_req[10];
    unsigned char ucSessionID_req[3];
    unsigned char ucTokenID_req[25];
    unsigned char ucIsEncrypt_req[25];
    unsigned char ucRRN_req[25];
    unsigned char ucPassword_req[60];
    unsigned char ucfingerid_req[3];
    unsigned char ucUser_id_req[30];
    unsigned char ucDeviceID_req[20];
    unsigned char ucChannel_req[2];
    //unsigned char ucSessionExpiryTime_req[25];
    //unsigned char ucX_Auth_Token_req[25];

    unsigned char ucChannelID_req[25]; // 18012019 Dhiral
    unsigned char ucGeoLattitude_req[25]; //11
    unsigned char ucGeoLongitude_req[25]; //12
    unsigned char ucVersion_req[10]; //13
    unsigned char ucMAC_DeviceID_req[30]; //14
    unsigned char ucCellID_req[20]; //15
    unsigned char ucDeviceModel_req[30]; //16
    unsigned char ucDeviceOS_req[10]; // 17
    unsigned char ucMCC_req[15]; //18
    unsigned char ucMNC_req[15]; //19
    unsigned char ucipAddress_req[20]; //20
    unsigned char ucClient_id_req[25]; //21
    //unsigned char ucProductCode_req[25];
    //unsigned char ucAmount_req[25];
    // char ucMerchantContribution_req[25];
    //unsigned char ucCharges_req[25];
    //unsigned char ucRouteID_req[25];
    //unsigned char ucSplitReferenceNo_req[25];
    unsigned char ucServiceID_req[25]; //18012019 Dhiral
    //unsigned char ucIdentifier_req[25];
    //unsigned char ucTxnHash_req[25];

    /*Response data*/

            char ucRequestId[25];
            char ucDisplayMessage[25];
            char ucResponseMessage[60];
            char ucResponseCode[4];
            char ucMessageType[10];
            char ucreturnCode[4];
            char ucBalancelist[20];
            char ucLDresponseMessage[30];//LD login data
            char ucuserId[10];
            char ucsubUserClass[2];
            //char ucfpIndex[1];
            char ucaadharNumber[13];
            char ucbranchCode[5];
            char ucpolicy[30];
            char ucpriority[40];
            char ucabort_on_failure[40];
            char ucclient_id[40];
            char ucuser_roles[40];
            //char ucmobile_no[40];
            char ucencryption_key[40];
            char ucsecurity_profile[40];
            char uclast_login_time[40];
            char ucis_last_login_failed[40];
            char ucis_user_locked[40];
            char uclast_login_date[40];
            char ucfailure_count[40];
            char ucfailure_sec_question_count[40];
            char ucfirst_login_time[40];
            char ucfirst_login_date[40];
            char ucMessageId[40];
            char ucSessionExpiryTime[40];
            char ucSessionId[40];
            char ucRouteID[40];
            char ucErrorCode[40];
            char ucXMLData[40];
            char ucAuthmanFlag[40];
            char ucServiceID[40];
             char ucipaddress[50];
            char ucid_token[15];
            char ucdate2[30];
            char uctoken_type[15];
            char ucidentifier[8];
            char ucname[25];
            char ucUCcode[8];//UC for User class
            char ucUCdescription[40];
            char ucmaxDaysBackdatedAllowed[2];
            char ucmaxDaysFutureDatedAllowed[2];
            char ucBAcode[8]; //BA for BranchAssociated
            char ucBAdescription[20];
            char ucifsc[15];
            char ucmicr[15];
            char ucgrid[3];
            char ucdefaultCostCenter[8];
            char ucBranchtype[2];
            char ucBTdescription[20]; //BT for branchtype
            char ucsubType[5];
            char ucsubtypedescription[20];
            char ucaddressLine1[40];
            char ucaddressLine2[40];
            char ucaddressLine3[40];
            char uccity[20];
            char ucstate[10];
            char ucstateDescription[25];
            char ucpinCode[8];
            char uccountry[6];
            char uccashOpen[7];
            char uccts[7];
            char ucgridBranch[7];
            char ucallowedToChangePostingDate[8];
            char uccurrencyEnvironment[3];
            char ucprimaryCashLimit[15];
            char uctellerIdentifier[5];
            char ucpasswordExpired[8];
            char ucADagentStatus[10];
            char ucADdistributorFlag[10];
            char ucfPIndex[3];
            char ucmobileNumber[12];   //Merchant Mob No
            char ucstatus[3];
            char ucrevoked[8];
            char ucsupervisorId[10];
            char ucsucceeded_schemes[15];
            char ucaccess_token[20];
            char ucfirstLoginKilled[8];
            char ucrefresh_expires_in[5];
            char ucBLaccountNo[20];      //BL for Balance List
            char ucledgerBalance[15];
            char ucavailableBalance[15];
            char uccurrency[5];
            char ucexpires_in[20];
            char ucerrors[20];
            char ucforce_change_password[8];
            char uctellerProofList[10];
            char ucLastLogin[30];
            char ucRESPSession_ID[50];
            char ucNoOfFinger[5];
            char ucThreshold[3];
            char ucusertypeID[3];
            char ucuserRole[3];
            char uceAgreement[3];
            char CertificateExpiryDate[20];
            char uceSurvey[3];
            char ucchannelID[3];
            char ucEkyc[3];
            char ucRoles[15];
            char ucMaxMinLimit[20];
            char ucMandatoryVersion[10];
            char ucCurrentVersion[10];
            char ucev_mandat[10];
            char ucev_current[5];
            char ucmorp_mandat[5];
            char ucmorp_current[5];
            char ucMastersVersion[3000];
            char uczeroizationDateTime[20];
            char ucMerchantLimit[30];
            char ucFCCardAcceptorLocation[400];
            char ucmerchantAccount[15];

}Login_leg2;

typedef struct
{
    unsigned char ucRequestID_req[25];
    char cDResponse_code[10];
    char cDResponseMessage[60];
}Change_Device_OTP;


typedef struct
{
    char cQuestion_id[2+1];
    char question [100+1];
    char cQuestion_group[2+1];
}Secret_Quest;
typedef struct
{
    char cSel_Question_id[2+1];
    char cSel_question [100+1];
    char cSel_Question_group[2+1];
    char cSel_Answer[50];
    char cenc_Answer[50];

}Select_secretQue;

typedef struct
{
    char cDResponse_code[10];
    char cDResponseMessage[60];
    char cDreturncode[10];

}Secret_resp;
//typedef struct
//{
//    /*Request data*/

//    unsigned char ucUsername_req[10];
//    unsigned char ucRequestID_req[25];
//    unsigned char ucMethodID_req[5];
//    unsigned char ucTellerID_req[10];
//    unsigned char ucSessionID_req[3];
//    unsigned char ucTokenID_req[25];
//    unsigned char ucIsEncrypt_req[25];
//    unsigned char ucRRN_req[25];
//    unsigned char ucPassword_req[60];
//    unsigned char ucfingerid_req[3];
//    unsigned char ucUser_id_req[30];
//    unsigned char ucClient_id_req[25];
//    unsigned char ucDeviceID_req[20];
//    unsigned char ucChannel_req[2];
//    //unsigned char ucSessionExpiryTime_req[25];
//    //unsigned char ucX_Auth_Token_req[25];
//    unsigned char ucChannelID_req[25]; // 18012019 Dhiral
//    //unsigned char ucProductCode_req[25];
//    //unsigned char ucAmount_req[25];
//    // char ucMerchantContribution_req[25];
//    //unsigned char ucCharges_req[25];
//    //unsigned char ucRouteID_req[25];
//    //unsigned char ucSplitReferenceNo_req[25];
//    unsigned char ucServiceID_req[25]; //18012019 Dhiral
//    //unsigned char ucIdentifier_req[25];
//    //unsigned char ucTxnHash_req[25];

//    /*Response data*/

//    char ucRequestId[25];
//    char ucDisplayMessage[25];
//    char ucResponseMessage[60];
//    char ucResponseCode[4];
//    char ucMessageType[10];
//    char ucreturnCode[4];
//    char ucBalancelist[20];
//    char ucLDresponseMessage[30];//LD login data
//    char ucuserId[10];
//    char ucsubUserClass[2];
//    //char ucfpIndex[1];
//    char ucaadharNumber[13];
//    char ucbranchCode[5];
//    char ucpolicy[30];
//    char ucpriority[40];
//    char ucabort_on_failure[40];
//    char ucclient_id[40];
//    char ucuser_roles[40];
//    //char ucmobile_no[40];
//    char ucencryption_key[40];
//    char ucsecurity_profile[40];
//    char uclast_login_time[40];
//    char ucis_last_login_failed[40];
//    char ucis_user_locked[40];
//    char uclast_login_date[40];
//    char ucfailure_count[40];
//    char ucfailure_sec_question_count[40];
//    char ucfirst_login_time[40];
//    char ucfirst_login_date[40];
//    char ucMessageId[40];
//    char ucSessionExpiryTime[40];
//    char ucSessionId[40];
//    char ucRouteID[40];
//    char ucErrorCode[40];
//    char ucXMLData[40];
//    char ucAuthmanFlag[40];
//    char ucServiceID[40];
//    char ucipaddress[50];
//    char ucid_token[15];
//    char ucdate2[30];
//    char uctoken_type[15];
//    char ucidentifier[8];
//    char ucname[25];
//    char ucUCcode[8];//UC for User class
//    char ucUCdescription[40];
//    char ucmaxDaysBackdatedAllowed[2];
//    char ucmaxDaysFutureDatedAllowed[2];
//    char ucBAcode[8]; //BA for BranchAssociated
//    char ucBAdescription[20];
//    char ucifsc[15];
//    char ucmicr[15];
//    char ucgrid[3];
//    char ucdefaultCostCenter[8];
//    char ucBranchtype[2];
//    char ucBTdescription[20]; //BT for branchtype
//    char ucsubType[5];
//    char ucsubtypedescription[20];
//    char ucaddressLine1[40];
//    char ucaddressLine2[40];
//    char ucaddressLine3[40];
//    char uccity[20];
//    char ucstate[10];
//    char ucstateDescription[25];
//    char ucpinCode[8];
//    char uccountry[6];
//    char uccashOpen[7];
//    char uccts[7];
//    char ucgridBranch[7];
//    char ucallowedToChangePostingDate[8];
//    char uccurrencyEnvironment[3];
//    char ucprimaryCashLimit[15];
//    char uctellerIdentifier[5];
//    char ucpasswordExpired[8];
//    char ucfPIndex[3];
//    char ucmobileNumber[12];   //Merchant Mob No
//    char ucstatus[3];
//    char ucrevoked[8];
//    char ucsupervisorId[10];
//    char ucsucceeded_schemes[15];
//    char ucaccess_token[20];
//    char ucfirstLoginKilled[8];
//    char ucrefresh_expires_in[5];
//    char ucBLaccountNo[15];      //BL for Balance List
//    char ucledgerBalance[15];
//    char ucavailableBalance[15];
//    char uccurrency[5];
//    char ucexpires_in[20];
//    char ucerrors[20];
//    char ucforce_change_password[8];
//    char uctellerProofList[10];
//    char ucLastLogin[30];
//    char ucRESPSession_ID[50];
//    char ucNoOfFinger[5];
//    char ucThreshold[3];
//    char ucusertypeID[3];
//    char ucuserRole[3];
//    char uceAgreement[3];
//    char CertificateExpiryDate[20];
//    char uceSurvey[3];
//    char ucchannelID[3];
//    char ucEkyc[3];
//    char ucRoles[15];
//    char ucMaxMinLimit[20];
//    char ucMandatoryVersion[10];
//    char ucCurrentVersion[3];
//    char ucev_mandat[3];
//    char ucev_current[5];
//    char ucmorp_mandat[5];
//    char ucmorp_current[5];
//    char ucMastersVersion[100];
//    char uczeroizationDateTime[20];
//    char ucMerchantLimit[30];

//}Login_leg2;




/*


    #define  RequestId  				    1
    #define  ResponseMessage        		2
    #define  ResponseCode					3
    #define  MessageType					4
    #define  SessionExpiryTime				5
    #define  RouteID						6
    #define  last_login_time				7
    #define  first_login_time				8
    #define  first_login_date         		9
    #define  last_login_date    			10
    #define  XMLData        				11
    #define  AuthmanFlag      				12
    #define  DisplayMessage   				13
    #define  MessageId         				14
    #define  SessionId      				15
    #define  ErrorCode        				16
    #define  subUserClass     				17
    #define  priority          				18
    #define  abort_on_failure           	19
    #define  is_user_locked   				20
    #define  failure_count  				21
    #define  failure_sec_question_count 	22
    #define  fpIndex        				23
    #define  is_last_login_failed    		24
    #define  ServiceID          			25
    #define  returnCode           			26
    #define  userId     					27
    #define  aadharNumber    				28
    #define  branchCode     				29
    #define  policy        					30
    #define  client_id      				31
    #define  mobile_no        				32
    #define  encryption_key      			33
    #define  user_roles						34



    */



/*


#define  RequestId  				    1
#define  ResponseMessage        		2
#define  ResponseCode					3
#define  MessageType					4
#define  SessionExpiryTime				5
#define  RouteID						6
#define  last_login_time				7
#define  first_login_time				8
#define  first_login_date         		9
#define  last_login_date    			10
#define  XMLData        				11
#define  AuthmanFlag      				12
#define  DisplayMessage   				13
#define  MessageId         				14
#define  SessionId      				15
#define  ErrorCode        				16
#define  subUserClass     				17
#define  priority          				18
#define  abort_on_failure           	19
#define  is_user_locked   				20
#define  failure_count  				21
#define  failure_sec_question_count 	22
#define  fpIndex        				23
#define  is_last_login_failed    		24
#define  ServiceID          			25
#define  returnCode           			26
#define  userId     					27
#define  aadharNumber    				28
#define  branchCode     				29
#define  policy        					30
#define  client_id      				31
#define  mobile_no        				32
#define  encryption_key      			33
#define  user_roles						34



*/








#endif // LOGIN_H
