//#include "finomisc.h"
#include "FPB.h"
//#include "CHeader.h"
#include "Login.h"
//#include "Global.h"
#include "json.h"

extern Login_leg1 configLoginLeg1;
extern Login_leg2 configLoginLeg2;

int FPB::iParsLeg1(unsigned char *Data)
{
    int len,i=0,slen,len2=-1;
    //int j=0;
    char *RecvJsonData;
    char *CheckData;
     char buff[1024];
    //	char buff[1024];

    char *LoginLeg1[]={"RequestId:",
                       "DisplayMessage:",
                       "ResponseMessage:",
                       "ResponseCode:",
                       "MessageType:",
                       "ResponseData:returnCode:",
                       "ResponseData:responseMessage:",
                       "ResponseData:userId:",
                       "ResponseData:subUserClass:",
                       "ResponseData:fpIndex:",//10
                       "ResponseData:aadharNumber:",
                       "ResponseData:branchCode:",
                       "ResponseData:policies:policy:",
                       "ResponseData:policies:priority:",
                       "ResponseData:policies:abort_on_failure:",
                       "ResponseData:client_id:",
                       "ResponseData:user_roles:",
                       "ResponseData:mobile_no:",
                       "ResponseData:encryption_key:",
                       "ResponseData:security_profile:last_login_time:",//20
                       "ResponseData:security_profile:is_last_login_failed:",
                       "ResponseData:security_profile:is_user_locked:",
                       "ResponseData:security_profile:last_login_date:",
                       "ResponseData:security_profile:failure_count:",
                       "ResponseData:security_profile:failure_sec_question_count:",//25
                       "ResponseData:security_profile:first_login_time:",
                       "ResponseData:security_profile:first_login_date:",
                       "MessageId:",
                       "SessionExpiryTime:",
                       "SessionId:",
                       "RouteID:",
                       "ErrorCode:",
                       "XMLData:",
                       "AuthmanFlag:",
                       "ServiceID:"//34
                      };

   // int MandTag[]={1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,0,1,1,0,1,1,0,0,1,1,1,1,0,0,0,0};//1 for mandatory and  0 for non-mandatory by saurabh
  //  int MandTag[]={1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,0,1,1,0,1,1,0,0,1,1,1,1,0,0,1,0}; // Working in Mobile channel 1
    int MandTag[]={1,1,1,1,1,1,1,1,1,1,0,1,0,0,0,1,1,1,1,0,1,1,0,1,1,0,0,1,1,1,1,0,0,1,0}; // Visiontek PDS channel 3
    len=sizeof(LoginLeg1)/sizeof(LoginLeg1[0]);
    len2=sizeof(MandTag)/sizeof(MandTag[0]);
    for (i = 0; i < len; ++i)
    {
     //   printf("ii ==== %d\n",i);
        RecvJsonData=strstr((char *)Data,LoginLeg1[i]);
        slen=strlen(LoginLeg1[i]);
        if (RecvJsonData==NULL && MandTag[i]==1)//If Tag not found and its mandatory.
        {
            qDebug()<<"Parse Error compare fun";
            return FUNC_ERROR;
            //continue;
        }
        else if(RecvJsonData == NULL) //Else proceed to further parsing.
            continue;

        memset(buff,0,sizeof(buff));

        RecvJsonData=&RecvJsonData[slen];

        CheckData=strchr(RecvJsonData,'\n');
        //CheckData--;
        memset(buff,0,sizeof(buff));
        strncpy(buff,RecvJsonData,CheckData-RecvJsonData);

        switch (i)
        {
        case 0:
            strcpy((char *)configLoginLeg1.ucRequestId,buff);
     //      printf("configLoginLeg1.ucRequestId == %s\n", configLoginLeg1.ucRequestId,buff);
            break;

        case 1:
            strcpy((char *)configLoginLeg1.ucDisplayMessage,buff);
     //       printf("configLoginLeg1.ucDisplayMessage == %s\n", configLoginLeg1.ucDisplayMessage,buff);
            break;

        case 2:
            strcpy((char *)configLoginLeg1.ucResponseMessage,buff);
    //       printf("configLoginLeg1.ucResponseMessage == %s\n", configLoginLeg1.ucResponseMessage,buff);
            break;

        case 3:
            strcpy((char *)configLoginLeg1.ucResponseCode,buff);
  //          printf("configLoginLeg1.ucResponseCode == %s\n", configLoginLeg1.ucResponseCode,buff);
            break;

        case 4:
            strcpy((char *)configLoginLeg1.ucMessageType,buff);
   //         printf("configLoginLeg1.ucMessageType == %s\n", configLoginLeg1.ucMessageType,buff);
            break;

        case 5:
            strcpy((char *)configLoginLeg1.ucreturnCode,buff);
     //       printf("configLoginLeg1.ucreturnCode == %s\n", configLoginLeg1.ucreturnCode,buff);
            break;

        case 6:
            strcpy((char *)configLoginLeg1.ucRDresponseMessage,buff);
   //         printf("configLoginLeg1.ucRDresponseMessage == %s\n", configLoginLeg1.ucRDresponseMessage,buff);
            break;

        case 7:
            strcpy((char *)configLoginLeg1.ucuserId,buff);
     //      printf("configLoginLeg1.ucuserId == %s\n", configLoginLeg1.ucuserId,buff);
            break;

        case 8:
            strcpy((char *)configLoginLeg1.ucsubUserClass,buff);
     //       printf("configLoginLeg1.ucsubUserClass == %s\n", configLoginLeg1.ucsubUserClass,buff);
            break;

        case 9:
            strcpy((char *)configLoginLeg1.ucfPIndex,buff);
    //        printf("configLoginLeg1.ucfPIndex == %s\n", configLoginLeg1.ucfPIndex,buff);
            break;

        case 10:
            strcpy((char *)configLoginLeg1.ucaadharNumber,buff);
   //         printf("configLoginLeg1.ucaadharNumber == %s\n", configLoginLeg1.ucaadharNumber,buff);
            break;

        case 11:
            strcpy((char *)configLoginLeg1.ucbranchCode,buff);
   //         printf("configLoginLeg1.ucbranchCode == %s\n", configLoginLeg1.ucbranchCode,buff);
            break;

        case 12:
            strcpy((char *)configLoginLeg1.ucpolicy,buff);
   //         printf("configLoginLeg1.ucpolicy == %s\n", configLoginLeg1.ucpolicy,buff);
            break;

        case 13:
            strcpy((char *)configLoginLeg1.ucpriority,buff);
    //        printf("configLoginLeg1.ucpriority == %s\n", configLoginLeg1.ucpriority,buff);
            break;

        case 14:
            strcpy((char *)configLoginLeg1.ucabort_on_failure,buff);
   //         printf("configLoginLeg1.ucabort_on_failure == %s\n", configLoginLeg1.ucabort_on_failure,buff);
            break;

        case 15:
            strcpy((char *)configLoginLeg1.ucclient_id,buff);
  //          printf("configLoginLeg1.ucclient_id == %s\n", configLoginLeg1.ucclient_id,buff);
            break;

        case 16:
            strcpy((char *)configLoginLeg1.ucuser_roles,buff);
   //         printf("configLoginLeg1.ucuser_roles == %s\n", configLoginLeg1.ucuser_roles,buff);
            break;

        case 17:
            strcpy((char *)configLoginLeg1.ucmobileNumber,buff);
   //         printf("configLoginLeg1.ucmobileNumber == %s\n", configLoginLeg1.ucmobileNumber,buff);
            break;

        case 18:
            strcpy((char *)configLoginLeg1.ucencryption_key,buff);
 //           printf("configLoginLeg1.ucencryption_key ekey== %s\n", configLoginLeg1.ucencryption_key,buff);
            break;

        case 19:
            strcpy((char *)configLoginLeg1.uclast_login_time,buff);
   //         printf("configLoginLeg1.uclast_login_time efe== %s", configLoginLeg1.uclast_login_time,buff);
            break;

        case 20:
            strcpy((char *)configLoginLeg1.ucis_last_login_failed,buff);
   //         printf("configLoginLeg1.ucis_last_login_failed == %s\n", configLoginLeg1.ucis_last_login_failed,buff);
            break;

        case 21:
            strcpy((char *)configLoginLeg1.ucis_user_locked,buff);
    //        printf("configLoginLeg1.ucis_user_locked == %s\n", configLoginLeg1.ucis_user_locked,buff);
            break;

        case 22:
            strcpy((char *)configLoginLeg1.uclast_login_date,buff);
    //        printf("configLoginLeg1.uclast_login_date == %s\n", configLoginLeg1.uclast_login_date,buff);
            break;

        case 23:
            strcpy((char *)configLoginLeg1.ucfailure_count,buff);
   //         printf("configLoginLeg1.ucfailure_count == %s\n", configLoginLeg1.ucfailure_count,buff);
            break;

        case 24:
            strcpy((char *)configLoginLeg1.ucfailure_sec_question_count,buff);
  //          printf("configLoginLeg1.ucfailure_sec_question_count == %s\n", configLoginLeg1.ucfailure_sec_question_count,buff);
            break;

        case 25:
            strcpy((char *)configLoginLeg1.ucfirst_login_time,buff);
  //          printf("configLoginLeg1.ucfirst_login_time == %s\n", configLoginLeg1.ucfirst_login_time,buff);
            break;

        case 26:
            strcpy((char *)configLoginLeg1.ucfirst_login_date,buff);
  //          printf("configLoginLeg1.ucfirst_login_date == %s\n", configLoginLeg1.ucfirst_login_date,buff);
            break;

        case 27:
            strcpy((char *)configLoginLeg1.ucMessageId,buff);
 //           printf("configLoginLeg1.ucMessageId == %s\n", configLoginLeg1.ucMessageId,buff);
            break;

        case 28:
            strcpy((char *)configLoginLeg1.ucSessionExpiryTime,buff);
  //          printf("configLoginLeg1.ucSessionExpiryTime == %s\n", configLoginLeg1.ucSessionExpiryTime,buff);
            break;

        case 29:
            strcpy((char *)configLoginLeg1.ucSessionId,buff);
  //          printf("configLoginLeg1.ucSessionId == %s\n", configLoginLeg1.ucSessionId,buff);
            break;

        case 30:
            strcpy((char *)configLoginLeg1.ucRouteID,buff);
  //          printf("configLoginLeg1.ucRouteID == %s\n", configLoginLeg1.ucRouteID,buff);
            break;

        case 31:
            strcpy((char *)configLoginLeg1.ucErrorCode,buff);
//            printf("configLoginLeg1.ucErrorCode == %s\n", configLoginLeg1.ucErrorCode,buff);
            break;

        case 32:
            strcpy((char *)configLoginLeg1.ucXMLData,buff);
 //           printf("configLoginLeg1.ucXMLData == %s\n", configLoginLeg1.ucXMLData,buff);
            break;

        case 33:
            strcpy((char *)configLoginLeg1.ucAuthmanFlag,buff);
  //          printf("configLoginLeg1.ucAuthmanFlag == %s\n", configLoginLeg1.ucAuthmanFlag,buff);
            break;

        case 34:
            strcpy((char *)configLoginLeg1.ucServiceID,buff);
   //         printf("configLoginLeg1.ucServiceID == %s\n", configLoginLeg1.ucServiceID,buff);
            break;

        default:
            break;
        }
    }
    return FUNC_OK;
}




int FPB::iParsLeg2(unsigned char *Data)
{
    int len,i=0,slen;
    char *RecvJsonData;
    char *CheckData;
    char buff[1024];
    //CMisc miscObj;
    char *LoginLeg2[]={
        "RequestId:",
        "DisplayMessage:",
        "ResponseMessage:",
        "ResponseCode:",
        "MessageType:",
        "ResponseData:Login_Data:id_token:",
        "ResponseData:Login_Data:date2:",
        "ResponseData:Login_Data:token_type:",
        "ResponseData:Login_Data:userDetails:identifier:",
        "ResponseData:Login_Data:userDetails:name:",
        "ResponseData:Login_Data:userDetails:userClass:code:",//10
        "ResponseData:Login_Data:userDetails:userClass:description:",
        "ResponseData:Login_Data:userDetails:userClass:maxDaysBackdatedAllowed:",
        "ResponseData:Login_Data:userDetails:userClass:maxDaysFutureDatedAllowed:",
        "ResponseData:Login_Data:userDetails:branchAssociated:code:",
        "ResponseData:Login_Data:userDetails:branchAssociated:description:",
        "ResponseData:Login_Data:userDetails:branchAssociated:ifsc:",
        "ResponseData:Login_Data:userDetails:branchAssociated:micr:",
        "ResponseData:Login_Data:userDetails:branchAssociated:grid:",
        "ResponseData:Login_Data:userDetails:branchAssociated:defaultCostCenter:",
        "ResponseData:Login_Data:userDetails:branchAssociated:branchType:type:",//20
        "ResponseData:Login_Data:userDetails:branchAssociated:branchType:description:",
        "ResponseData:Login_Data:userDetails:branchAssociated:branchType:subType:",
        "ResponseData:Login_Data:userDetails:branchAssociated:branchType:subtypedescription:",
        "ResponseData:Login_Data:userDetails:branchAssociated:address:addressLine1:",
        "ResponseData:Login_Data:userDetails:branchAssociated:address:addressLine2:",
        "ResponseData:Login_Data:userDetails:branchAssociated:address:city:",
        "ResponseData:Login_Data:userDetails:branchAssociated:address:state:",
        "ResponseData:Login_Data:userDetails:branchAssociated:address:stateDescription:",
        "ResponseData:Login_Data:userDetails:branchAssociated:address:pinCode:",
        "ResponseData:Login_Data:userDetails:branchAssociated:address:country:",//30
        "ResponseData:Login_Data:userDetails:branchAssociated:cashOpen:",
        "ResponseData:Login_Data:userDetails:branchAssociated:cts:",
        "ResponseData:Login_Data:userDetails:branchAssociated:gridBranch:",
        "ResponseData:Login_Data:userDetails:allowedToChangePostingDate:",
        "ResponseData:Login_Data:userDetails:currencyEnvironment:",
        "ResponseData:Login_Data:userDetails:primaryCashLimit:",//miss
        "ResponseData:Login_Data:userDetails:tellerIdentifier:",
        "ResponseData:Login_Data:userDetails:passwordExpired:",
        "ResponseData:Login_Data:userDetails:fPIndex:",
        "ResponseData:Login_Data:userDetails:mobileNumber:",//40
        "ResponseData:Login_Data:userDetails:status:",
        "ResponseData:Login_Data:userDetails:revoked:",
        "ResponseData:Login_Data:succeeded_schemes:",
        "ResponseData:Login_Data:access_token:",
        "ResponseData:Login_Data:returnCode:",
        "ResponseData:Login_Data:firstLoginKilled:",
        "ResponseData:Login_Data:refresh_expires_in:",
        "ResponseData:Login_Data:balancesList:accountNo:",//miss
        "ResponseData:Login_Data:balancesList:ledgerBalance:",
        "ResponseData:Login_Data:balancesList:availableBalance:",//50 miss
        "ResponseData:Login_Data:balancesList:currency:",//miss
        "ResponseData:Login_Data:responseMessage:",
        "ResponseData:Login_Data:expires_in:",
        "ResponseData:Login_Data:errors:",
        "ResponseData:Login_Data:force_change_password:",
        "ResponseData:Login_Data:tellerProofList:",//56
        "ResponseData:LastLogin:","ResponseData:SessionID:","ResponseData:NoOfFinger:","ResponseData:Threshold:",//60
        "ResponseData:usertypeID:","ResponseData:userRole:","ResponseData:eAgreement:","ResponseData:eSurvey:",//65
        "ResponseData:channelID:","ResponseData:Ekyc:","ResponseData:Roles:","ResponseData:MaxMinLimit:",
        "MessageId:","SessionExpiryTime:","SessionId:","RouteID:","ErrorCode:","XMLData:","AuthmanFlag:","ServiceID:",
        "ResponseData:Login_Data:userDetails:branchAssociated:address:addressLine3:",//77
    };

    int MandTag[]={1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,//40
                   1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,0,0,0,1,1,0,1,0,0,0,0};
    len=sizeof(LoginLeg2)/sizeof(LoginLeg2[0]);

    for (i = 0; i < len; ++i)
    {
        RecvJsonData=strstr((char *)Data,LoginLeg2[i]);
        slen=strlen(LoginLeg2[i]);
        if (RecvJsonData==NULL && MandTag[i]==1)//If Tag not found and its mandatory.
        {
            //			DrawString("TAG Missing",15,25,0);
            //			ttestall(0,200);
            //miscObj.DisplayMsgBox((char *)"TAG Missing");
            sleep(2);
            return FUNC_ERROR;
        }
        else if(RecvJsonData == NULL) //Else proceed to further parsing.
            continue;
        memset(buff,0,sizeof(buff));

        RecvJsonData=&RecvJsonData[slen];

        CheckData=strchr(RecvJsonData,'\n');
        memset(buff,0,sizeof(buff));
        //CheckData--;
        strncpy(buff,RecvJsonData,CheckData-RecvJsonData);

        switch (i)
        {
        case 0:
            strcpy((char *)configLoginLeg2.ucRequestId,buff);
            break;

        case 1:
            strcpy((char *)configLoginLeg2.ucDisplayMessage,buff);
            break;

        case 2:
            strcpy((char *)configLoginLeg2.ucResponseMessage,buff);
            break;

        case 3:
            strcpy((char *)configLoginLeg2.ucResponseCode,buff);
            break;

        case 4:
            strcpy((char *)configLoginLeg2.ucMessageType,buff);
            break;

        case 5:
            strcpy((char *)configLoginLeg2.ucid_token,buff);
            break;

        case 6:
            strcpy((char *)configLoginLeg2.ucdate2,buff);
            break;

        case 7:
            strcpy((char *)configLoginLeg2.uctoken_type,buff);
            break;

        case 8:
            strcpy((char *)configLoginLeg2.ucidentifier,buff);
            break;

        case 9:
            strcpy((char *)configLoginLeg2.ucname,buff);
            break;

        case 10:
            strcpy((char *)configLoginLeg2.ucUCcode,buff);
            break;

        case 11:
            strcpy((char *)configLoginLeg2.ucUCdescription,buff);
            break;

        case 12:
            strcpy((char *)configLoginLeg2.ucmaxDaysBackdatedAllowed,buff);
            break;

        case 13:
            strcpy((char *)configLoginLeg2.ucmaxDaysFutureDatedAllowed,buff);
            break;

        case 14:
            strcpy((char *)configLoginLeg2.ucBAcode,buff);
            break;

        case 15:
            strcpy((char *)configLoginLeg2.ucBAdescription,buff);
            break;

        case 16:
            strcpy((char *)configLoginLeg2.ucifsc,buff);
            break;

        case 17:
            strcpy((char *)configLoginLeg2.ucmicr,buff);
            break;

        case 18:
            strcpy((char *)configLoginLeg2.ucgrid,buff);
            break;

        case 19:
            strcpy((char *)configLoginLeg2.ucdefaultCostCenter,buff);
            break;

        case 20:
            strcpy((char *)configLoginLeg2.ucBranchtype,buff);
            break;

        case 21:
            strcpy((char *)configLoginLeg2.ucBTdescription,buff);//Branch Type
            break;

        case 22:
            strcpy((char *)configLoginLeg2.ucsubType,buff);
            break;

        case 23:
            strcpy((char *)configLoginLeg2.ucsubtypedescription,buff);
            break;

        case 24:
            strcpy((char *)configLoginLeg2.ucaddressLine1,buff);
            break;

        case 25:
            strcpy((char *)configLoginLeg2.ucaddressLine2,buff);
            break;

        case 26:
            strcpy((char *)configLoginLeg2.uccity,buff);
            break;

        case 27:
            strcpy((char *)configLoginLeg2.ucstate,buff);
            break;

        case 28:
            strcpy((char *)configLoginLeg2.ucstateDescription,buff);
            break;

        case 29:
            strcpy((char *)configLoginLeg2.ucpinCode,buff);
            break;

        case 30:
            strcpy((char *)configLoginLeg2.uccountry,buff);
            break;

        case 31:
            strcpy((char *)configLoginLeg2.uccashOpen,buff);
            break;

        case 32:
            strcpy((char *)configLoginLeg2.uccts,buff);
            break;

        case 33:
            strcpy((char *)configLoginLeg2.ucgridBranch,buff);
            break;

        case 34:
            strcpy((char *)configLoginLeg2.ucallowedToChangePostingDate,buff);
            break;

        case 35:
            strcpy((char *)configLoginLeg2.uccurrencyEnvironment,buff);
            break;

        case 36:
            strcpy((char *)configLoginLeg2.ucprimaryCashLimit,buff);
            break;

        case 37:
            strcpy((char *)configLoginLeg2.uctellerIdentifier,buff);
            break;

        case 38:
            strcpy((char *)configLoginLeg2.ucpasswordExpired,buff);
            break;

        case 39:
            strcpy((char *)configLoginLeg2.ucfPIndex,buff);
            break;

        case 40:
            strcpy((char *)configLoginLeg2.ucmobileNumber,buff);
            break;

        case 41:
            strcpy((char *)configLoginLeg2.ucstatus,buff);
            break;

        case 42:
            strcpy((char *)configLoginLeg2.ucrevoked,buff);
            break;

        case 43:
            strcpy((char *)configLoginLeg2.ucsucceeded_schemes,buff);
            break;

        case 44:
            strcpy((char *)configLoginLeg2.ucaccess_token,buff);
            break;

        case 45:
            strcpy((char *)configLoginLeg2.ucreturnCode,buff);
            break;

        case 46:
            strcpy((char *)configLoginLeg2.ucfirstLoginKilled,buff);
            break;

        case 47:
            strcpy((char *)configLoginLeg2.ucrefresh_expires_in,buff);
            break;

        case 48:
            strcpy((char *)configLoginLeg2.ucaccess_token,buff);
            break;

        case 49:
            strcpy((char *)configLoginLeg2.ucledgerBalance,buff);
            break;

        case 50:
            strcpy((char *)configLoginLeg2.ucavailableBalance,buff);
            break;

        case 51:
            strcpy((char *)configLoginLeg2.uccurrency,buff);
            break;

        case 52:
            strcpy((char *)configLoginLeg2.ucLDresponseMessage,buff);
            break;

        case 53:
            strcpy((char *)configLoginLeg2.ucexpires_in,buff);
            break;

        case 54:
            strcpy((char *)configLoginLeg2.ucerrors,buff);
            break;

        case 55:
            strcpy((char *)configLoginLeg2.ucforce_change_password,buff);
            break;

        case 56:
            strcpy((char *)configLoginLeg2.uctellerProofList,buff);
            break;

        case 57:
            strcpy((char *)configLoginLeg2.ucLastLogin,buff);
            break;

        case 58:
            strcpy((char *)configLoginLeg2.ucRESPSession_ID,buff);
            break;

        case 59:
            strcpy((char *)configLoginLeg2.ucNoOfFinger,buff);
            break;

        case 60:
            strcpy((char *)configLoginLeg2.ucThreshold,buff);
            break;

        case 61:
            strcpy((char *)configLoginLeg2.ucusertypeID,buff);
            break;

        case 62:
            strcpy((char *)configLoginLeg2.ucuserRole,buff);
            break;

        case 63:
            strcpy((char *)configLoginLeg2.uceAgreement,buff);
            break;

        case 64:
            strcpy((char *)configLoginLeg2.uceSurvey,buff);
            break;

        case 65:
            strcpy((char *)configLoginLeg2.ucchannelID,buff);
            break;

        case 66:
            strcpy((char *)configLoginLeg2.ucEkyc,buff);
            break;

        case 67:
            strcpy((char *)configLoginLeg2.ucRoles,buff);
            break;

        case 68:
            strcpy((char *)configLoginLeg2.ucMaxMinLimit,buff);
            break;

        case 69:
            strcpy((char *)configLoginLeg2.ucMessageId,buff);
            break;

        case 70:
            strcpy((char *)configLoginLeg2.ucSessionExpiryTime,buff);
            break;

        case 71:
            strcpy((char *)configLoginLeg2.ucSessionId,buff);
            break;

        case 72:
            strcpy((char *)configLoginLeg2.ucRouteID,buff);
            break;

        case 73:
            strcpy((char *)configLoginLeg2.ucErrorCode,buff);
            break;

        case 74:
            strcpy((char *)configLoginLeg2.ucXMLData,buff);
            break;

        case 75:
            strcpy((char *)configLoginLeg2.ucAuthmanFlag,buff);
            break;

        case 76:
            strcpy((char *)configLoginLeg2.ucServiceID,buff);
            break;
        case 77:
            strcpy((char *)configLoginLeg2.ucaddressLine3,buff);
            break;

        default:
            break;
        }
    }
    return FUNC_OK;
}




/*


int iLoginUserclass(char *Data)
{
    int fval=-1,bval=-1,slen,i=0,len=-1;
    char *RecvJsonData;
    char *CheckData;
    char *Userclass[]={"code","description","maxDaysBackdatedAllowed","maxDaysFutureDatedAllowed"};
    unsigned char buff[25];

    len=sizeof(Userclass)/sizeof(Userclass[0]);

    for (i = 0; i < len; ++i)
    {
        RecvJsonData=strstr(Data,Userclass[i]);
        slen=strlen(Userclass[i]);
        if (RecvJsonData==NULL)
        {
            DrawString("TAG Missing",15,25,0);
            ttestall(0,200);
            return FUNC_ERROR;
        }
        fval=ifetchfOffset(Userclass[i],fval);
        bval=ifetchbOffset(Userclass[i],bval);

        if (fval==FUNC_ERROR || bval==FUNC_ERROR)
        {
            DrawString("Offset Error",10,25,0);
            ttestall(0,200);
            return FUNC_ERROR;
        }

        RecvJsonData=&RecvJsonData[slen+fval];

        CheckData=strchr(RecvJsonData,',');
        CheckData-=bval;
        memset(buff,0,sizeof(buff));
        strncpy(buff,RecvJsonData,CheckData-RecvJsonData);

        switch (i)
        {
        case 0:
            strcpy(configLogin.ucUCcode,buff);
            break;

        case 1:
            strcpy(configLogin.ucUCdescription,buff);
            break;

        case 2:
            strcpy(configLogin.ucmaxDaysBackdatedAllowed,buff);
            break;

        case 3:
            strcpy(configLogin.ucmaxDaysFutureDatedAllowed,buff);
            break;

        default:
            return FUNC_ERROR;
            break;
        }
        return FUNC_OK;
    }
}

*/

//int FPB::ifetchfOffset(char *tag,int found)
//{
//    int len = -2,i=0;

//    char *ArrayOffset3[]={"RequestId","ResponseMessage","ResponseCode","MessageType","versionID","RouteID"};

//    char *ArrayOffset2[]={"DisplayMessage","MessageId","SessionExpiryTime","SessionId","ErrorCode","XMLData","AuthmanFlag","ServiceID"};

//    char *ArrayOffset5[]={"MastersVersion"};

//    len=sizeof(ArrayOffset3)/sizeof(ArrayOffset3[0]);

//    for(i=0; i<len; i++)
//    {
//        if(memcmp(ArrayOffset3[i], tag,sizeof(ArrayOffset3[i])))
//        {
//            found = 3;
//            return found;
//        }
//    }

//    len=sizeof(ArrayOffset2)/sizeof(ArrayOffset2[0]);

//    for(i=0; i<len; i++)
//    {
//        if(ArrayOffset2[i] == tag)
//        {
//            found = 2;
//            return found;
//        }
//    }

//    len=sizeof(ArrayOffset5)/sizeof(ArrayOffset5[0]);

//    for(i=0; i<len; i++)
//    {
//        if(ArrayOffset5[i] == tag)
//        {
//            found = 5;
//            return found;
//        }
//    }

//    return FUNC_ERROR;
//}

//int FPB::ifetchbOffset(char *tag,int found)
//{

//    int len = -2,i=0;

//    char *ArrayOffset0[]={"DisplayMessage","MessageId","SessionId","ErrorCode","XMLData",
//            "AuthmanFlag","ServiceID","subUserClass","fpIndex","priority","is_last_login_failed","is_user_locked","failure_count",
//            "failure_sec_question_count","maxDaysBackdatedAllowed"};

//    char *ArrayOffset1[]={"RequestId","ResponseMessage","RouteID","SessionExpiryTime","MessageType","ResponseCode",
//            "last_login_time","last_login_date","first_login_time","first_login_date","maxDaysFutureDatedAllowed"};

//    char *ArrayOffset2[]={"returnCode","userId","aadharNumber","branchCode","policy","client_id","mobile_no","encryption_key","responseMessage","abort_on_failure",
//            "code","description"};

//    char *ArrayOffset3[]={"user_roles"};

//    len=sizeof(ArrayOffset1)/sizeof(ArrayOffset1[0]);

//    for(i=0; i<len; i++)
//    {
//        if(ArrayOffset1[i] == tag)
//        {
//            found = 1;
//            return found;
//        }
//    }

//    len=sizeof(ArrayOffset0)/sizeof(ArrayOffset0[0]);

//    for(i=0; i<len; i++)
//    {
//        if(ArrayOffset0[i] == tag)
//        {
//            found = 0;
//            return found;
//        }
//    }

//    len=sizeof(ArrayOffset2)/sizeof(ArrayOffset2[0]);

//    for(i=0; i<len; i++)
//    {
//        if(ArrayOffset2[i] == tag)
//        {
//            found = 2;
//            return found;
//        }
//    }

//    len=sizeof(ArrayOffset3)/sizeof(ArrayOffset3[0]);

//    for(i=0; i<len; i++)
//    {
//        if(ArrayOffset3[i] == tag)
//        {
//            found = 3;
//            return found;
//        }
//    }

//    return FUNC_ERROR;

//}

/*
int iGetTagResponse(char *Data,char *tag,char *buff,int offset)
{
    char *CheckData;
    char *RecvJsonData;
    int i=1;
    RecvJsonData=strstr(Data,tag);

    for (i = 1; i <= offset; ++i)
    {
        if (i==1)
        {
            CheckData=strchr(RecvJsonData,'}');
        }
        else
        {
            CheckData=strchr(CheckData+i,'},');
        }
    }
    CheckData+=3;  // to get , in buffer
    memset(buff,0,sizeof(buff));
    strncpy(buff,RecvJsonData,CheckData-RecvJsonData);
    return FUNC_OK;
}

*/





