#include "FPB.h"
#include "ui_FPB.h"
#include "displaymessage.h"
#include "Login.h"
//#include  "CHeader.h"
#include"getprofile.h"
//extern GProfile GETProfile;

extern Login_leg1 configLoginLeg1;
extern Login_leg2 configLoginLeg2;
//unsigned char BM_Recvdata[1024*45];


int FPB::iMakeRequest(int TYPE)
{
    FILE *fp=NULL;
    int iRet = -1;
    char cTagId[5];
    int iTagId = 0;
    char cFileBuuf[2];
    int index = 0;
    //unsigned char *tempBuff1=NULL;
    memset(Recvdata,0,sizeof(Recvdata));
    memset(cFileBuuf,0,sizeof(cFileBuuf));
    //tempBuff1=Recvdata;
    qDebug()<<"TYPE = "<<TYPE;
    switch (TYPE)
    {
    case 0:
        fp=fopen("/usr/FINO/FPB/REQ_Dollar/Leg1.txt","r");//Log in Leg1
        break;
    case 1:
        fp=fopen("/usr/FINO/FPB/REQ_Dollar/Leg2N.txt","r");//Log in Leg1
        break;
    case 2:
        fp=fopen("/usr/FINO/FPB/REQ_Dollar/Leg2.txt","r"); //Login Leg2
        break;
    case 3:
        fp=fopen("/usr/FINO/FPB/REQ_Dollar/AEPSWITH.txt","r");// AEPS Withdrawal
        break;
    case 4:
        fp=fopen("/usr/FINO/FPB/REQ_Dollar/AEPS_BALENQ.txt","r");
        break;
    case 5:
        fp = fopen("/usr/FINO/FPB/REQ_Dollar/LTS.txt","r"); // Last Transaction Status
        break;
    case 6:
        fp = fopen("/usr/FINO/FPB/REQ_Dollar/GETPROF.txt","r");
        break;
    case 7:
        //           fp = fopen("/usr/FINO/FPB/REQ_Dollar/BANKMAST.txt","r");
        fp = fopen("/usr/FINO/FPB/REQ_Dollar/IMPS_BANKMAST.txt","r");
        break;
    case 8:
        fp = fopen("/usr/FINO/FPB/REQ_Dollar/AEPSMINI.txt","r");
        break;
    case 9:
        fp = fopen("/usr/FINO/FPB/REQ_Dollar/AEPSLTS.txt","r"); // LTTS-Last Ten Transaction Status
        break;
    case 10:
        qDebug()<<"Inside the make request";
        fp = fopen("/usr/FINO/FPB/REQ_Dollar/VTGSQ.txt","r");
        break;
    case 11:
        fp = fopen("/usr/FINO/FPB/REQ_Dollar/VTGenOTP.txt","r");
        break;
    case 12:
        fp = fopen("/usr/FINO/FPB/REQ_Dollar/VTOTPVAL.txt","r");
        break;
    case 13:
        fp = fopen("/usr/FINO/FPB/REQ_Dollar/VTGSK.txt","r");
        break;
    case 14:
        fp = fopen("/usr/FINO/FPB/REQ_Dollar/VTFEL.txt","r");
        break;
    case 15:
        fp = fopen("/usr/FINO/FPB/REQ_Dollar/VTSWC.txt","r");
        break;
    case 19:
        fp = fopen("/usr/FINO/FPB/REQ_Dollar/VTOIN.txt","r");
        break;
    case 21:
        fp = fopen("/usr/FINO/FPB/REQ_Dollar/VTCP.txt","r");
        break;
    case 22:
        fp = fopen("/usr/FINO/FPB/REQ_Dollar/VTFC.txt","r");
        break;
    case 23:
        qDebug()<<"23";
        fp = fopen("/usr/FINO/FPB/REQ_Dollar/REFIRE.txt","r");
        break;
    case 24:
        qDebug()<<"24";
        fp = fopen("/usr/FINO/FPB/REQ_Dollar/REFUND.txt","r");
        break;

    case 25:
        qDebug()<<"25";
        fp = fopen("/usr/FINO/FPB/REQ_Dollar/BILLREFIRE.txt","r");
        break;

    default:
        break;
    }

    if (fp==NULL)
    {
        qDebug()<<"fp == NULL";
        //GL_Dialog_Message(graphicLib,"ERROR","Please load Request Files",GL_ICON_NONE,GL_BUTTON_NONE,3000);
        return FUNC_ERROR;
    }

    do
    {
        while(*cFileBuuf != '$')
        {
            memset(cFileBuuf,0,sizeof(cFileBuuf));
            iRet=fread(cFileBuuf,1,1,fp);
            if(iRet != 1 )
            {
                fclose(fp);
                if(strlen((char*)Recvdata) <=0)
                {
                    qDebug()<<"fread failed";
                    return FUNC_ERROR;
                }
                else
                    return FUNC_OK;
            }
            if(*cFileBuuf != '$')
                Recvdata[index++] = *cFileBuuf;
        }
        memset(cTagId,0,sizeof(cTagId));
        fread(cTagId,3,1,fp);
        iTagId = atoi(cTagId);
        // call the db fuction

        switch (TYPE) {
        case 0:

            iLogInLeg1(iTagId,&Recvdata[index]); // Log in LEG1
            break;
        case 1:
            iLogInLeg2(iTagId,&Recvdata[index]); // Log in LEG2
            break;
        case 2:
            //	iLogInLeg2(iTagId,&Recvdata[index]); //Log in LEG2
            break;
        case 3:
            iSetAEPS_With(iTagId,&Recvdata[index]); //AEPS Withdrawal
            break;
        case 4:
            iSetAEPS_BalEnq(iTagId,&Recvdata[index]);//AEPS Balance Enquiry
            break;
        case 5:
            iSetLTS(iTagId,&Recvdata[index]);//AEPS LTS last one transaction
            break;
        case 6:
            iSetGet_Profile(iTagId,&Recvdata[index]);//Get Profile Enquiry
            break;
        case 7:
            //iSetAEPSBANKMAST(iTagId,&Recvdata[index]);//Get AEPS Bank master  // Temp BM
            iSetIMPSBANKMAST(iTagId,&Recvdata[index]);//Get AEPS Bank master  // Temp BM
            break;
        case 8:
            iSetAEPSMiniState(iTagId,&Recvdata[index]);//Get AEPS Ministatement
            break;
        case 9:
            iSetAEPSLTS(iTagId,&Recvdata[index]);//AEPS Last 10 Transactions
            break;

        case 10:
            iSetFORGET(iTagId,&Recvdata[index]);
            break;
        case 11:
            iSetOTP(iTagId,&Recvdata[index]);
            break;
        case 12:
            iSetNEWPASS(iTagId,&Recvdata[index]);
            break;
        case 13:
            iSetOTP(iTagId,&Recvdata[index]);
            break;
            //        case 14:
            //            iSetFETCH(iTagId,&Recvdata[index]);
            //            break;
        case 15:
            iSetSEARCHWALK(iTagId,&Recvdata[index]);
            break;
        case 19:
          //  qDebug()<<"isetCustlimit";
            iSetOTPIMPS(iTagId,&Recvdata[index]);
            break;
        case 21:
            //   qDebug()<<"isetCustlimit";
            iSetCHANGE(iTagId,&Recvdata[index]);
            break;
        case 22:
            iSetFINDCUSTOMER(iTagId,&Recvdata[index]);
            break;
        case 23:
            iSetREFIRE(iTagId,&Recvdata[index]);
            break;

        case 24:
            iSetREFUND(iTagId,&Recvdata[index]);
            break;

        case 25:
            iSetBILLREFIRE(iTagId,&Recvdata[index]);
            break;

        default:
            break;
        }
//        if(TYPE == 7)  // Temp BM
//        {
//            printf("index : %d\n",index);
//            index += strlen((char *)&BMRecvdata[index]);  // Temp BM
//        }
//        else  // Temp BM
            index += strlen((char *)&Recvdata[index]);
        memset(cFileBuuf,0,sizeof(cFileBuuf));
    } while (iRet==1);
    fclose(fp);

    /*//consolprint(gSendData);
    //S_FS_FILE *fp=NULL;
    FS_unlink("/HOST/WITHF.TXT");
    fp =FS_open("/HOST/WITHF.TXT","a");
    FS_write(gSendData,strlen(gSendData),1,fp);
    ttestall(0,100);
    FS_close(fp);*/

    return FUNC_OK;
}





//int FPB::iLogInLeg1(int tagID, unsigned char *value)
//{
//    switch(tagID)
//    {
//    case  1:
//        strcpy((char *)value,(char *)configLoginLeg2.ucRequestID_req);
//        break;
//    case  2:
//        strcpy((char *)value,(char *)configLoginLeg2.ucMethodID_req);//Method id
//        break;
//    case  3:
//        strcpy((char *)value,(char *)configLoginLeg2.ucTellerID_req);//teller id
//        break;
//    case  4:
//        strcpy((char *)value,(char *)configLoginLeg2.ucIsEncrypt_req);//Is encrypt
//        break;
//    case  5:
//        strcpy((char *)value,(char *)configLoginLeg2.ucUser_id_req);//User ID
//        break;
//    case  6:
//        strcpy((char *)value,(char *)configLoginLeg2.ucPassword_req);//Password
//        break;
//    case  7:
//        strcpy((char *)value,(char *)configLoginLeg2.ucClient_id_req);//Client Id
//        break;
//    case  8:
//        strcpy((char *)value,(char *)configLoginLeg2.ucChannelID_req);//Channel ID //18012019 Dhiral
//        break;
//    case  9:
//        strcpy((char *)value,(char *)configLoginLeg2.ucServiceID_req);//Service Id
//        break;
//    case  10:
//        strcpy((char *)value,(char *)configLoginLeg2.ucChannel_req);//Service Id
//        break;
//    default:
//        return FUNC_ERROR;
//    }
//    return FUNC_OK;
//}


int FPB::iLogInLeg1(int tagID, unsigned char *value)
{
    switch(tagID)
    {
    case  1:
        strcpy((char *)value,(char *)configLoginLeg1.ucRequestID_req);
        break;
    case  2:
        strcpy((char *)value,(char *)configLoginLeg1.ucMethodID_req);//Method id
        break;
    case  3:
        strcpy((char *)value,(char *)configLoginLeg1.ucTellerID_req);//teller id
        break;
    case  4:
        strcpy((char *)value,(char *)configLoginLeg1.ucIsEncrypt_req);//Is encrypt
        break;
    case  5:
        strcpy((char *)value,(char *)configLoginLeg1.ucUser_id_req);//User ID
        break;
    case  6:
        strcpy((char *)value,(char *)configLoginLeg1.ucold_user_id);//Password
        break;
    case  7:
        strcpy((char *)value,(char *)configLoginLeg1.ucClient_id_req);//Password
        break;
        return FUNC_ERROR;
    }
    return FUNC_OK;
}


int FPB::iLogInLeg2(int tagID, unsigned char *value)
{
    switch(tagID)
    {
    case  1:
        strcpy((char *)value,(char *)configLoginLeg2.ucRequestID_req);
        break;
    case  2:
        strcpy((char *)value,(char *)configLoginLeg2.ucMethodID_req);//Method id
        break;
    case  3:
        strcpy((char *)value,(char *)configLoginLeg2.ucTellerID_req);//teller id
        break;
    case  4:
        strcpy((char *)value,(char *)configLoginLeg2.ucIsEncrypt_req);//Is encrypt
        break;
    case  5:
        strcpy((char *)value,(char *)configLoginLeg2.ucUser_id_req);//User ID
        break;
    case  6:
        strcpy((char *)value,(char *)configLoginLeg2.ucPassword_req);//Password
        break;
    case  7:
        strcpy((char *)value,(char *)configLoginLeg2.ucClient_id_req);//Client Id
        break;
    case  8:
        strcpy((char *)value,(char *)configLoginLeg2.ucChannelID_req);//Channel ID //18012019 Dhiral
        break;
    case  9:
        strcpy((char *)value,(char *)configLoginLeg2.ucServiceID_req);//Service Id
        break;
    case  10:
        strcpy((char *)value,(char *)configLoginLeg2.ucChannel_req);//Service Id
        break;
    case  11:
        strcpy((char *)value,(char *)configLoginLeg2.ucGeoLattitude_req);
        //       printf("configLoginLeg2.ucGeoLattitude_req = %s\n",configLoginLeg2.ucGeoLattitude_req);
        break;
    case  12:
        strcpy((char *)value,(char *)configLoginLeg2.ucGeoLongitude_req);//Method id
        //       printf("configLoginLeg2.ucGeoLongitude_req = %s\n",configLoginLeg2.ucGeoLongitude_req);
        break;
    case  13:
        strcpy((char *)value,(char *)configLoginLeg2.ucVersion_req);//teller id
        //      printf("configLoginLeg2.ucVersion_req = %s\n",configLoginLeg2.ucVersion_req);
        break;
    case  14:
        strcpy((char *)value,(char *)configLoginLeg2.ucMAC_DeviceID_req);//Is encrypt
        //      printf("configLoginLeg2.ucMAC_DeviceID_req = %s\n",configLoginLeg2.ucMAC_DeviceID_req);
        break;
    case  15:
        strcpy((char *)value,(char *)configLoginLeg2.ucCellID_req);//User ID
        //      printf("configLoginLeg2.ucCellID_req = %s\n",configLoginLeg2.ucCellID_req);
        break;
    case  16:
        strcpy((char *)value,(char *)configLoginLeg2.ucDeviceModel_req);//Password
        //      printf("configLoginLeg2.ucDeviceModel_req = %s\n",configLoginLeg2.ucDeviceModel_req);
        break;
    case  17:
        strcpy((char *)value,(char *)configLoginLeg2.ucDeviceOS_req);//Client Id
        //      printf("configLoginLeg2.ucDeviceOS_req = %s\n",configLoginLeg2.ucDeviceOS_req);////
        break;
    case  18:
        strcpy((char *)value,(char *)configLoginLeg2.ucMCC_req);//Channel ID //18012019 Dhiral
        //     printf("configLoginLeg2.ucMCC_req = %s\n",configLoginLeg2.ucMCC_req);
        break;
    case  19:
        strcpy((char *)value,(char *)configLoginLeg2.ucMNC_req);//Service Id
        //    printf("configLoginLeg2.ucMNC_req = %s\n",configLoginLeg2.ucMNC_req);
        break;
    case  20:
        strcpy((char *)value,(char *)configLoginLeg2.ucipAddress_req);//Service Id
        //     printf("configLoginLeg2.ucipAddress_req = %s\n",configLoginLeg2.ucipAddress_req);
        break;
    case  21:
        strcpy((char *)value,(char *)configLoginLeg2.ucClient_id_req);//Service Id
        //     printf("configLoginLeg2.ucClient_id_req = %s\n",configLoginLeg2.ucClient_id_req);
        break;
    default:
        return FUNC_ERROR;
    }
    return FUNC_OK;
}
