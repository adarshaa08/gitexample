#include "aepswthdraw.h"
#include "ui_FPB.h"
#include "json.h"
#include "FPB.h"
#include "Login.h"
#include "displaymessage.h"
#include "print.h"
#include <QSqlDatabase>
extern Login_leg2 configLoginLeg2;
extern Login_leg1 configLoginLeg1;
extern RD_SERVICE_STRUCTURE RD_Service_data;
extern Transaction Txn;
extern Printfpb ConfigPrint;

void FPB::on_WITHDR_pushButton_2_clicked()
{
    HostComm hComm;
    CMisc miscObj;
    memset(&AEPSWITH,0,sizeof(AEPSWITH));
    unsigned char BankBin[10];
    int response_timeout1 =0;
    ui->lineEdit_Bank_Search->clear();
    memset(BankBin,0,sizeof(BankBin));
    Check_Conditions();

    int ret = AutoLTS();
    qDebug()<<"ALTS ret = "<<ret;
    if(ret!=FUNC_OK && ret!=FUNCTION_SUCCESS)
    {
        uidmessage::instance()->hide_Message();

        ui->SubTransaction_Label1->setText((char *)configLoginLeg2.ucname);
        ui->SubTransaction_Label2->setText(Balance_Display);

        ui->stackedWidget->setCurrentIndex(2);
        return;
    }
    //    int i_ret = AEPSProcess();
    //    if(i_ret == FUNCTION_FAILED)
    //    {
    //        qDebug()<<"Return Failed from AEPSProcess";
    //        ui->stackedWidget->setCurrentIndex(2);
    //        return;
    //    }

    memset(&Txn,0,sizeof(Txn)); // clearing Transaction Structure
    Txn.Txntype = AEPS_WITH;


    ui->lineEditUID1->setReadOnly(false);
    ui->lineEditUID1->setFocus();
    ui->lineEditUID2->setReadOnly(false);
    ui->lineEditUID3->setReadOnly(false);

    QRegExp rx("\\d+");
    QRegExpValidator *validator1 = new QRegExpValidator(rx,this);
    ui->lineEditUID1->setValidator(validator1);
    ui->lineEditUID2->setValidator(validator1);
    ui->lineEditUID3->setValidator(validator1);


    ui->PAGE4_OK_pushButton_8->setEnabled(false);
    ui->PAGE4_OK_pushButton_8->setStyleSheet("background-color: rgbrgb(85, 85, 127); color: rgb(107, 107, 107)");

    uidmessage::instance()->hide_Message();

    ui->BankName_LabelName->setText((char *)configLoginLeg2.ucname);
    ui->BankName_LabelName->setFont(QFont("SansSerif",10));
    ui->BankName_LabelName->setAlignment(Qt::AlignRight | Qt::AlignBottom);

    QString bal1 = QString::fromUtf8((const char*)Balance_Display);
    ui->BankName_LabelBal->setText(bal1);
    ui->BankName_LabelBal->setFont(QFont("SansSerif",9));

    QRegExp rx1("\\w+");
    QRegExpValidator *validator2 = new QRegExpValidator(rx1,this);
    ui->lineEdit_Bank_Search->setValidator(validator2);
    //    QRegExp rx("[^A-Za-z]");
    //    QRegExpValidator *validator1 = new QRegExpValidator(rx,this);
    //    ui->lineEdit_Bank_Search->setValidator(validator1);
    //  ui->BankName_LabelBal->setAlignment(Qt::AlignRight | Qt::AlignBottom);
    ui->stackedWidget->setCurrentIndex(7);  // go to page ENTER Bank Search

}

int FPB::Prepare_AEPS_WITHDRWL_Func()
{
    //    HostComm hComm;
    CMisc miscObj;
    char szDate[16], szTime[16];
    int ret = -3, iRet = -3;
    //    char WITHDRWL_Buff[10000];
    char ErrorMsg[150];
    FILE *fp1;
    int template_len=0;
    unsigned char  *template_data;
    int gSendData_len = 0;
    int oBuff_len =0;
    uidmessage::instance()->hide_Message();
    uidmessage::instance()->Display_BusyMessage("Please Wait....");

    memset(ErrorMsg,0,sizeof(ErrorMsg));
    memset(AEPSWITH.ucSessionID_AWreq,            0, sizeof(AEPSWITH.ucSessionID_AWreq));
    memset(AEPSWITH.ucSessionExpiryTime_AWreq,    0, sizeof(AEPSWITH.ucSessionID_AWreq));
    memset(AEPSWITH.ucAadharNo_AWreq,             0, sizeof(AEPSWITH.ucSessionID_AWreq));
    memset(AEPSWITH.ucmerchantMobileNumber_AWreq, 0, sizeof(AEPSWITH.ucSessionID_AWreq));
    memset(AEPSWITH.ucaccountNumber_AWreq,        0, sizeof(AEPSWITH.ucSessionID_AWreq));
    memset(AEPSWITH.ucmobileNumber_AWreq,         0, sizeof(AEPSWITH.ucmobileNumber_AWreq));
    memset(AEPSWITH.uctotalAmount_AWreq,          0, sizeof(AEPSWITH.uctotalAmount_AWreq));
    memset(AEPSWITH.ucRequestID_AWreq,          0, sizeof(AEPSWITH.ucRequestID_AWreq));

    iGenerateRequestID((char *)configLoginLeg2.ucUsername_req,(unsigned char *)&AEPSWITH.ucRequestID_AWreq);

    QSqlQuery query;
    query.exec("delete from LST");
    char iquery[500] = "INSERT INTO LST (X_CORRELATION_ID) VALUES('";
    sprintf(iquery, "INSERT INTO LST (X_CORRELATION_ID) VALUES('%s')",AEPSWITH.ucRequestID_AWreq);
 #if DEBUG
    printf("QUERY === %s\n",iquery);
#endif
    if(!query.exec(iquery))
    {
        qDebug() << "In Insert_LST_DB = " << query.lastError();
        query.clear();
        query.finish();
        uidmessage::instance()->hide_Message();
        uidmessage::instance()->Display_BusyMessage("LST Database Updation Failed..");
        sleep(1);
        //return FUNC_ERROR;
    }
    query.clear();
    query.finish();


    //   memcpy(gstPFRData.ucLastCorelationid,AEPSWITH.ucRequestID_AWreq,strlen((char *)AEPSWITH.ucRequestID_AWreq));
    //   memcpy(LTS.ucX_CORRELATION_ID_LTSreq,AEPSWITH.ucRequestID_AWreq,sizeof(AEPSWITH.ucRequestID_AWreq));
    memcpy((char *)AEPSWITH.ucUID_AWreq, Txn.uid,12);
    memcpy((char *)AEPSWITH.ucAadharNo_AWreq,Txn.bin,strlen((char *)Txn.bin));
    strcat((char *)AEPSWITH.ucAadharNo_AWreq,"0");
    strcat((char *)AEPSWITH.ucAadharNo_AWreq,(char *)AEPSWITH.ucUID_AWreq);
    memcpy((char *)AEPSWITH.ucSessionID_AWreq,        (char *)configLoginLeg2.ucSessionId, strlen((char *)configLoginLeg2.ucSessionId));
    memcpy((char *)AEPSWITH.ucSessionExpiryTime_AWreq,(char *)configLoginLeg2.ucSessionExpiryTime,strlen((char *)configLoginLeg2.ucSessionExpiryTime));
    //sprintf((char *)AEPSWITH.ucAadharNo_AWreq,"%s0%s",(char *)AEPSWITH.ucNBIN_AWreq,AEPSWITH.ucUID_AWreq);
    memcpy((char *)AEPSWITH.ucTellerID_AWreq,         (char *)configLoginLeg2.ucUsername_req,strlen((char *)configLoginLeg2.ucUsername_req));
    memcpy((char *)AEPSWITH.ucmerchantMobileNumber_AWreq, (char *)configLoginLeg2.ucmobileNumber,strlen((char *)configLoginLeg2.ucmobileNumber));
    //// sprintf((char *)AEPSWITH.uccard_acceptor_namelocation_AWreq,"%s %s",(char *)configLoginLeg2.ucaddressLine1,configLoginLeg2.ucaddressLine2);
    //sprintf((char *)AEPSWITH.uccard_acceptor_namelocation_AWreq,"%s %s %s%s",(char *)configLoginLeg2.ucaddressLine2,configLoginLeg2.uccity,configLoginLeg2.ucstate,configLoginLeg2.uccountry);
    memcpy((char *)AEPSWITH.uccard_acceptor_namelocation_AWreq, configLoginLeg2.ucFCCardAcceptorLocation, strlen(configLoginLeg2.ucFCCardAcceptorLocation));
    memcpy((char *)AEPSWITH.ucaccountNumber_AWreq,    (char *)configLoginLeg2.ucBLaccountNo,strlen((char *)configLoginLeg2.ucBLaccountNo));
    memcpy((char *)AEPSWITH.ucmobileNumber_AWreq, Txn.Mobileno, strlen(Txn.Mobileno));
    memcpy((char *)AEPSWITH.uctotalAmount_AWreq, Txn.Amount, strlen(Txn.Amount));
    memcpy((char *)AEPSWITH.ucAppId_AWreq, CLIENT_ID,strlen(CLIENT_ID));
    memcpy((char *)AEPSWITH.ucPTappId_AWreq, CLIENT_ID,strlen(CLIENT_ID));
    memcpy((char *)AEPSWITH.uccostCenter_AWreq, configLoginLeg2.ucBAcode,strlen(configLoginLeg2.ucBAcode));
    memcpy((char *)AEPSWITH.ucChannelID_AWreq, CHANNEL_ID,strlen(CHANNEL_ID));
    memcpy((char *)AEPSWITH.ucIsEncrypt_AWreq,ISENCEYPT,strlen(ISENCEYPT));

    //configLoginLeg2.uccurrency
    char arr[20], arr_ip[16];
    memset(arr, 0, sizeof(arr));
    memset(arr_ip, 0, sizeof(arr_ip));
    int imac = get_mac_id(arr, arr_ip);
    QString qMac = QString::fromUtf8((const char*)arr);
    qMac = qMac.remove(":");


    char itimedate[20], ilatlong[100];
    memset(itimedate, 0, sizeof(itimedate));
    strcpy(itimedate,iTimestamp());
    printf("Time Stamp == %s\n",itimedate);


    memset(ilatlong, 0, sizeof(ilatlong));
    strcpy(ilatlong,get_Lat_long());
    printf("ilatlong == %s\n",ilatlong);

    QString strToFind;

    QString Latitude = QString::fromLocal8Bit(ilatlong);
    QStringList ilatquery = Latitude.split(",");
    for(int i=0 ; i < ilatquery.length() ; i++)
        qDebug()<<"Query Response"<<ilatquery.at(i);

    strToFind.clear();
    QString str = ilatquery.at(0);
    strToFind = ":";
    QString value_latitude = str.mid(str.indexOf(strToFind)+strToFind.length());
    qDebug()<<"value_latitude = "<<value_latitude;
    str.clear();

    strToFind.clear();
    str = ilatquery.at(1);
    strToFind = ":";
    QString value_longitude = str.mid(str.indexOf(strToFind)+strToFind.length());
    qDebug()<<"value_longitude = "<<value_longitude;

    query.clear();
    char last_rrn[20], last_date[15];
    memset(last_rrn,0,sizeof(last_rrn));
    memset(last_date,0,sizeof(last_date));
    query.prepare("SELECT LAST_RRN, LAST_DATE from LastWithdrawalData");
    if(!query.exec())
    {
        qDebug() << query.lastError();
        qDebug() << "LST database query selection failed";
        uidmessage::instance()->hide_Message();
        uidmessage::instance()->DisplayMessage("Record Not Found..");
        qDebug()<<"FUNCTION_FAILED";
        query.clear();
        query.finish();
        ui->stackedWidget->setCurrentIndex(2);
        return FUNCTION_FAILED;
    }
    else
    {
        while(query.next())
        {
            memset(last_rrn,0,sizeof(last_rrn));
            strcpy((char*)last_rrn, query.value(0).toString().trimmed().toAscii().data());
         //   printf("last_rrn = %s\n",last_rrn);
            memset(last_date,0,sizeof(last_date));
            strcpy((char*)last_date, query.value(1).toString().trimmed().toAscii().data());
          //  printf("last_rrn = %s\n",last_date);
        }
    }
    query.clear();

    memcpy((char *)AEPSWITH.ucbankName_AWreq, BankName_disp, strlen(BankName_disp));
    memcpy((char *)AEPSWITH.ucAFTsupportData_PVTXNID_AWreq, last_rrn, strlen(last_rrn));  // 36 PVTXNID Previous Successful Txn ID ..length
    memcpy((char *)AEPSWITH.ucAFTsupportData_PVTXNDT_AWreq, last_date, strlen(last_date));  // 37 PVTXNDT Previous Successful Date 03\/03\/19
    memcpy((char *)AEPSWITH.ucAFTsupportData_IPADDR_AWreq, arr_ip, strlen(arr_ip));  // 38 IPADDR len 13
    memcpy((char *)AEPSWITH.ucAFTsupportData_DEVICEID_AWreq, qMac.trimmed().toAscii().data(), strlen(qMac.trimmed().toAscii().data())); // 39 DEVICEID Device Serial Number (Mac ID) 980CA5F2BEF1
    memcpy((char *)AEPSWITH.ucAFTsupportData_MCCCellID_AWreq, "0", 1); // 40 Cell ID
    memcpy((char *)AEPSWITH.ucAFTsupportData_MCCLAC_AWreq, "0", 1);   //  41 LAC
    memcpy((char *)AEPSWITH.ucAFTsupportData_MCCMCC_AWreq, "0", 1);   // 42 MCC
    memcpy((char *)AEPSWITH.ucAFTsupportData_MCCMNC_AWreq, "0", 1);   // 43 MNC
    memcpy((char *)AEPSWITH.ucAFTsupportData_PRTXNID_AWreq, itimedate, strlen(itimedate)); // 44 PRTXNID  Timestamps (Substring of RequestID) 030319050252729
    memcpy((char *)AEPSWITH.ucAFTsupportData_SPLTSEQ_AWreq, "0", 1);  // 45 SPLTSEQ  1 - Txn Amount>PerTransactionLimit (Total) | 0: Txn Amount<=PerTransactionLimit (Total)
    memcpy((char *)AEPSWITH.ucAFTsupportData_CHGAMT_AWreq, "0", 1);   // 46 CHGAMT Charges (AEPS - 0)

    if(!memcmp((char *)value_latitude.trimmed().toAscii().data(),"00.00",5))
    {
        memcpy(AEPSWITH.ucAFTsupportData_CELLIDLatitude_AWreq,"null",4);//20032019 Aaadarsha
        memcpy(AEPSWITH.ucAFTsupportData_CELLIDLongitude_AWreq,"null",4); //20032019 Aadarsha
        qDebug()<<"inside thr latitude and longitude";
    }
    else
    {
        memcpy((char *)AEPSWITH.ucAFTsupportData_CELLIDLatitude_AWreq, value_latitude.trimmed().toAscii().data(), strlen(value_latitude.trimmed().toAscii().data()));   // 46 CHGAMT Charges (AEPS - 0)
        memcpy((char *)AEPSWITH.ucAFTsupportData_CELLIDLongitude_AWreq, value_longitude.trimmed().toAscii().data(), strlen(value_longitude.trimmed().toAscii().data()));   // 46 CHGAMT Charges (AEPS - 0)
    }
#if DEBUG
    printf("AEPSWITH.ucRequestID_AWreq == %s\n",AEPSWITH.ucRequestID_AWreq);
    printf("AEPSWITH.ucAadharNo_AWreq == %s\n",AEPSWITH.ucAadharNo_AWreq);
    printf("AEPSWITH.ucSessionID_AWreq == %s\n",AEPSWITH.ucSessionID_AWreq);
    printf("AEPSWITH.ucmerchantMobileNumber_AWreq == %s\n",AEPSWITH.ucmerchantMobileNumber_AWreq);
#endif
    ret	= makeJSONAEPSWith();/*-------------Make REQUEST--------------*/
    if(ret!=FUNC_OK)
    {
        return FUNC_ERROR;
    }

    memset(szDate, 0, sizeof(szDate));
    memset(szTime, 0, sizeof(szTime));
    GetLocalTime(szDate, szTime);
    query.exec("delete from AUTOLTS");
    // iquery[500] = "INSERT INTO LST (X_CORRELATION_ID) VALUES('";

    if(!query.exec(iquery))
    {
        qDebug() << "In Insert_LST_DB = " << query.lastError();
        query.clear();
        query.finish();
        sleep(1);
        //return FUNC_ERROR;
    }
    query.clear();

    char temp_uid[13];
    memset(temp_uid,0,sizeof(temp_uid));
    sprintf(temp_uid,"********%s", &Txn.uid[strlen (Txn.uid) - 4]);
   // printf("temp_uid = %s\n",temp_uid);
    sprintf(iquery, "INSERT INTO AUTOLTS (X_CORRELATION_ID,TransactionTypeName,Date,Time,RRN,TxnAmt,TxnStatus,ALTSYesNo,AadharNo,Bankname) VALUES('%s','CASH WITHDRAW','%s','%s','0','%s','','Y','%s','%s')",AEPSWITH.ucRequestID_AWreq,szDate,szTime,AEPSWITH.uctotalAmount_AWreq,temp_uid,Txn.bankname);
#if DEBUG
    printf("QUERY === %s\n",iquery);
#endif
    if(!query.exec(iquery))
    {
        qDebug() << "In Insert_LST_DB = " << query.lastError();
        query.clear();
        query.finish();
        uidmessage::instance()->hide_Message();
        uidmessage::instance()->Display_BusyMessage("LST Database Updation Failed..");

        sleep(1);
        //return FUNC_ERROR;
    }
    query.clear();
    query.finish();
    memset(temp_uid,0,sizeof(temp_uid));
    /****************************Encryption function*********************************/
    if(!strcmp((char *)AEPSWITH.ucIsEncrypt_AWreq,"true"))
    {
        gSendData_len =strlen((char *)gSendData);
        RequestDetachEncryptAttach((char *)gSendData,1,gSendData_len);
        qDebug()<<"After encrypt function";
    }
    /***************************************************************/

    uidmessage::instance()->hide_Message();
    uidmessage::instance()->Display_BusyMessage("Transaction Request in Progress");

#ifdef UAT_HTTP     // File Download

    hComm.close_connection();
    res = hComm.open_connect(SERVERIP,SERVERPORT);
    if(res != 0)
    {
        qDebug() << "Failed to connect to the Server";
        uidmessage::instance()->hide_Message();
        uidmessage::instance()->DisplayMessage("Server Not Connected");
        qDebug()<<"FUNCTION_FAILED";
        ui->stackedWidget->setCurrentIndex(2);
        qDebug()<<"JSON Creation failed";
        return FUNCTION_FAILED;
    }


    //    sleep(1);
    uidmessage::instance()->hide_Message();
    //    uidmessage::instance()->Display_BusyMessage("Transmitting .....");

    //QSqlQuery query;
    query.clear();
    int len_gSendData = strlen((char *)gSendData);
    memset(WITHDRWL_Buff,0,sizeof(WITHDRWL_Buff));
    memset(oBuff, 0, sizeof(oBuff));
    if((hComm.host_2Sendrecv((unsigned char *)gSendData,len_gSendData,( unsigned char*)oBuff,(int *)&WITHDRWL_len,(int)response_timeout1,0))!=SUCCESS)
    {
        printf("No Response From Server\n");
        hComm.close_connection();
        miscObj.DisplayMsgBox((char *)"Response Time-Out");
        uidmessage::instance()->hide_Message();
        qDebug()<<"***   Response Time-Out   ***";

        query.clear();
        query.exec("update AUTOLTS set ALTSYesNo='Y'");


        //        int ret_al = AutoLTS();
        //        if(ret_al == FUNCAL_FAILED)
        //        {
        //            qDebug()<<"AUTO LTS FUNCION FAILED WITH TRANSACTION";
        //        }
        //        else if(ret_al == FUNCTION_FAILED || ret_al == FUNC_ERROR)
        //        {
        //            qDebug()<<"AUTO LTS FUNCION FAILED or FUNC ERROR";
        //        }
        //        else if(ret_al == FUNCTION_SUCCESS || ret_al == FUNC_OK)
        //        {
        //            qDebug()<<"Auto LTS Success";
        //            query.clear();
        //            query.exec("update AUTOLTS set ALTSYesNo='N'");
        //        }

        int iret = AutoLTS();
        if(iret!=FUNC_OK || iret!=FUNCTION_SUCCESS)
        {
            ui->stackedWidget->setCurrentIndex(2);
            return iret;
        }

        ui->stackedWidget->setCurrentIndex(2);
        hComm.close_connection();
        return;
    }
    hComm.close_connection();

#else     // File Download full condition
#if DEBUG
    printf("gSendData = %s\n\n",gSendData);
#endif
    memset(str_url,0,sizeof(str_url));
#ifdef PRODUCTION
    strcpy(str_url,URL_CREATION(AEPS_WITHDRAWAL_URL_PROD));
#else
    strcpy(str_url,URL_CREATION((char *)AEPS_WITHDRAWAL_URL));
#endif

    int retk = file_download(str_url, (char *)gSendData,"/usr/FINO/FPB/RESP/OUTPUT.TXT",150);
    if(retk!=0)
    {
        qDebug()<<"retk = "<<retk;
        if(retk == RESPONSE_TIMEOUT)
        {
//            uidmessage::instance()->hide_Message();
//            miscObj.DisplayMsgBox((char *)"Response Time-Out");
//            uidmessage::instance()->hide_Message();
            qDebug()<<"***   Response Time-Out   ***";
            query.clear();
            query.exec("update AUTOLTS set ALTSYesNo='Y'");
            int iret = AutoLTS();
            if(iret!=FUNC_OK || iret!=FUNCTION_SUCCESS)
            {
                uidmessage::instance()->hide_Message();
                if(iret == SESSION_EXPIRED_FAILED)
                {
                    query.clear();
                    query.exec("update AUTOLTS set ALTSYesNo='Y'");
                    query.clear();
                    ui->stackedWidget->setCurrentIndex(0);
                }
                else
                   ui->stackedWidget->setCurrentIndex(2);
                return iret;
            }

            uidmessage::instance()->hide_Message();
            ui->stackedWidget->setCurrentIndex(2);
            return iret;
        }
        uidmessage::instance()->hide_Message();
        return FUNCTION_FAILED;
    }

#endif


    /****************************Decryption function*********************************/
    if(!strcmp((char *)AEPSWITH.ucIsEncrypt_AWreq,"true"))
    {
        oBuff_len = strlen(oBuff);
#if DEBUG
        qDebug()<<"Length of buffer"<<oBuff_len;
        printf("Buffer%s\n",oBuff);
 #endif
        ResponseDetachEncryptAttach(oBuff,2,oBuff_len);//TEMPORARY
    }
    /**************************************************************************/



    ret=json->iParsJSON((unsigned char*)oBuff,(char *)&AEPSWITH.ucResponseCode,(char *)&AEPSWITH.ucResponseMessage);
    system("cd /usr/FINO/FPB/RESP/");
    system("rm /usr/FINO/FPB/RESP/OUTPUT.TXT");
    if(ret!=FUNC_OK)
    {
        //        sprintf(ErrorMsg,"Message:%s",/*(char *)AEPSWITH.ucResponseCode,*/(char *)AEPSWITH.ucResponseMessage);
        //        miscObj.DisplayMsgBox(ErrorMsg);
        uidmessage::instance()->hide_Message();

        if((strcmp((char *)AEPSWITH.ucResponseMessage,"Time out from NPCI")==0) && ((strcmp((char *)AEPSWITH.ucResponseCode,"10")==0)))
        {
            query.clear();
            query.exec("update AUTOLTS set ALTSYesNo='Y'");
            int iret = AutoLTS();
            if(iret!=FUNC_OK || iret!=FUNCTION_SUCCESS)
            {
                uidmessage::instance()->hide_Message();
                if(iret == SESSION_EXPIRED_FAILED)
                {
                    query.clear();
                    query.exec("update AUTOLTS set ALTSYesNo='Y'");
                    query.clear();
                    ui->stackedWidget->setCurrentIndex(0);
                }
                else
                   ui->stackedWidget->setCurrentIndex(2);
                return iret;
            }

            uidmessage::instance()->hide_Message();
            ui->stackedWidget->setCurrentIndex(2);
            return iret;
        }

        qDebug()<<"Auto LTS Success";
        query.clear();
        query.exec("update AUTOLTS set ALTSYesNo='N'");
        query.clear();

   //     printf("************  AEPSWITH.ucResponseMessage = %s  ************\n", AEPSWITH.ucResponseMessage);
        if(strcmp((char *)AEPSWITH.ucResponseMessage,"Session Expired.")==0)
        {
            uidmessage::instance()->hide_Message();
            sprintf(ErrorMsg,"%s",/*(char *)AEPSWITH.ucResponseCode,*/(char *)AEPSWITH.ucResponseMessage);
            miscObj.DisplayMsgBox(ErrorMsg);
            uidmessage::instance()->hide_Message();
            //            ui->stackedWidget->setCurrentIndex(0);
            return SESSION_EXPIRED_FAILED;
        }
        sprintf(ErrorMsg,"%s",/*(char *)AEPSWITH.ucResponseCode,*/(char *)AEPSWITH.ucResponseMessage);
        miscObj.DisplayMsgBox(ErrorMsg);
        uidmessage::instance()->hide_Message();
        return FUNCTION_FAILED;
    }
    if (!memcmp(AEPSWITH.ucResponseCode,"0",1))
    {

        system("cd /usr/FINO/FPB/REQ/");
        fp1 = fopen("/usr/FINO/FPB/REQ/IntJSON.TXT", "rb"); // open
        if (fp1 == NULL)
        {
            qDebug()<<"Can't Open IntJSON.TXT File";
            uidmessage::instance()->hide_Message();
            //return FUNCTION_FAILED;
            return FUNCTION_FAILED;
        }

        fseek(fp1, 0, SEEK_END);
        template_len = ftell(fp1);
        template_data = (unsigned char *)malloc(template_len);

        if (template_data == NULL)
        {
            fclose(fp1);
            printf("\n in testbio memeory alloc failed");
            uidmessage::instance()->hide_Message();
            //return FUNCTION_FAILED;
            return FUNCTION_FAILED;
        }

        fseek(fp1, 0, SEEK_SET);
        ret=fread(template_data,sizeof(unsigned char ),template_len,fp1);
        if(ret==0)
            fclose(fp1);

        iRet = iParseAEPS_With((unsigned char*)template_data);
        free(template_data);
        system("cd /usr/FINO/FPB/RESP/");
        system("rm /usr/FINO/FPB/RESP/OUTPUT.TXT");
        system("rm /usr/FINO/FPB/REQ/IntJSON.TXT");
        QSqlQuery query;
        query.exec("delete from LST");
        char iquery[500] = "INSERT INTO LST (X_CORRELATION_ID) VALUES('";
        sprintf(iquery, "INSERT INTO LST (X_CORRELATION_ID) VALUES('%s')",AEPSWITH.ucRequestID_AWreq);
 #if DEBUG
        printf("QUERY === %s\n",iquery);
#endif
        if(!query.exec(iquery))
        {
            qDebug() << "In Insert_LST_DB = " << query.lastError();
            query.clear();
            query.finish();
            uidmessage::instance()->hide_Message();
            uidmessage::instance()->Display_BusyMessage("LST Database Updation Failed..");
            sleep(1);
            //return FUNC_ERROR;
        }
        query.clear();
        query.finish();

        if (iRet == FUNC_OK)
        {
            uidmessage::instance()->hide_Message();
            uidmessage::instance()->Display_BusyMessage("Cash Withdrawal Successful");
            sleep(5);
            //uidmessage::instance()->hide_Message();

        }
        else
        {
            qDebug()<<"Inside Pardse  error";
            miscObj.DisplayMsgBox((char *)"Cash Withdrwal Failed");
            uidmessage::instance()->hide_Message();
            return FUNCTION_FAILED;
        }

        memset(iquery,0,sizeof(iquery));
        query.clear();
        sprintf(iquery, "update AUTOLTS set RRN='%s'",AEPSWITH.ucAORtxnReferenceNo);
 #if DEBUG
        printf("QUERY === %s\n",iquery);
  #endif
        query.exec(iquery);
        query.clear();

        query.exec("delete from LastWithdrawalData");
        query.clear();
        memset(iquery,0,sizeof(iquery));
        sprintf(iquery, "INSERT INTO LastWithdrawalData (LAST_RRN, LAST_DATE) VALUES('%s','%s')", AEPSWITH.ucAORtxnReferenceNo,szDate);
#if DEBUG
        printf("QUERY 1 === %s\n",iquery);
#endif
        if(!query.exec(iquery))
        {
            qDebug() << "In Insert_LastWithdrawalData = " << query.lastError();
            query.clear();
            query.finish();
            //            uidmessage::instance()->hide_Message();
            //            uidmessage::instance()->Display_BusyMessage("LST Database Updation Failed..");

            //            sleep(1);
            //return FUNC_ERROR;
        }
        query.clear();
        query.finish();

        memcpy((char *)configLoginLeg2.ucMerchantLimit,(char *)AEPSWITH.ucavailableBalance,strlen((char *)AEPSWITH.ucavailableBalance));
        memcpy((char *)ConfigPrint.ucSTAN,&AEPSWITH.ucAORtxnReferenceNo[6],6);
        memcpy((char *)ConfigPrint.ucTranStatus,"Success(0)",10);
        memcpy((char *)ConfigPrint.ucBCName,configLoginLeg2.ucname,15);
        memcpy((char *)ConfigPrint.ucBCLocation,AEPSWITH.ucCardAcceptorNameLocation,strlen((char *)AEPSWITH.ucCardAcceptorNameLocation));
        memcpy((char *)ConfigPrint.ucAgentID,configLoginLeg2.ucUsername_req,strlen((const char*)configLoginLeg2.ucUsername_req));
        sprintf((char *)ConfigPrint.ucCustAadharNo,"********%s",&AEPSWITH.ucUID_AWreq[8]);
        sprintf((char *)ConfigPrint.ucACBalance,"Rs. %s",AEPSWITH.ucavailableBalance);
  #if DEBUG
        printf("Txn.Amount p = %s\n",Txn.Amount);
        printf("ConfigPrint.ucAmount = %s\n",ConfigPrint.ucAmount);
  #endif
        sprintf((char *)ConfigPrint.ucAmount,"Rs. %s",Txn.Amount);


        if(print_aes_transaction()== FUNC_OK)
        {
            if (!memcmp(AEPSWITH.ucPTRreturnCode,"0",1))
            {
                QString bal = QString::fromUtf8((const char*)(const char*)AEPSWITH.ucPTRavailableBalance);
                bool decimal;
                decimal = bal.contains('.');
                if(decimal == false)
                {
                    bal = bal.append(".00");
                }
                memset(Balance_Display,0,strlen(Balance_Display));
                sprintf(Balance_Display,"Balance : %s\n",bal.toLatin1().data(),strlen(bal.toLatin1().data()));
             //   printf("Balance_Display in withdrawal = %s\n", Balance_Display);
                strncpy(Mer_Latest_Balance, bal.toLatin1().data(),strlen(bal.toLatin1().data()));
                // strncpy(Balance_Display, (const char*)AEPSWITH.ucPTRavailableBalance, strlen((const char*)AEPSWITH.ucPTRavailableBalance));

                printf("Mer_Latest_Balance in withdrawal = %s & %s\n", AEPSWITH.ucPTRavailableBalance, Mer_Latest_Balance);
                //            QString temp1 = QString :: fromUtf8(Mer_Latest_Balance);
                //            QString BL = "Balance : " + temp1;
                //            qDebug()<<"BL == "<<BL;

            }
            qDebug()<<"Auto LTS Success";
            query.clear();
            query.exec("update AUTOLTS set ALTSYesNo='N'");
            query.clear();
            qDebug()<<" Print SUCCESS";
            ui->Dash_Latest_Balance_Label_pg12->setText(Balance_Display);
            ui->SubTransaction_Label1->setText((char *)configLoginLeg2.ucname);
            ui->SubTransaction_Label1->setFont(QFont("SansSerif",10));
            //    ui->SubTransaction_Label1->setFont(QFont("Bold"));
            ui->SubTransaction_Label1->setAlignment(Qt::AlignRight | Qt::AlignBottom);
         //   printf("Balance_Display = %s\n",Balance_Display);

            //    QString bal1 = QString::fromUtf8((const char*)Balance_Display);
            //    qDebug()<<"bal1 = "<<bal1;
            ui->SubTransaction_Label2->setText(QString::fromUtf8((const char*)Balance_Display));
            ui->SubTransaction_Label2->setFont(QFont("SansSerif",9));
            ui->SubTransaction_Label2->setAlignment(Qt::AlignRight | Qt::AlignBottom);


        }
        else
        {
            uidmessage::instance()->hide_Message();
            qDebug()<<" Print FAILED";
            miscObj.DisplayMsgBox((char *)"Print Slip Failed");
            sleep(2);
            uidmessage::instance()->hide_Message();
            //  return FUNCTION_FAILED;
        }
        //            return (FUNC_ERROR);
    }
    else
    {
        uidmessage::instance()->hide_Message();
        uidmessage::instance()->Display_BusyMessage("Cash Withdrwal Failed");
        sleep(1);
        uidmessage::instance()->hide_Message();
        Txn.Txntype = 0;
        ui->stackedWidget->setCurrentIndex(2);
        return FUNCTION_FAILED;
    }
    uidmessage::instance()->hide_Message();
    memset(&AEPSWITH,0,sizeof(AEPSWITH));
    ui->stackedWidget->setCurrentIndex(2);
    return FUNC_OK;

}

int FPB::makeJSONAEPSWith(void)
{
    char tempbuff[30];
    memset(gSendData,0,sizeof(gSendData));
    //char tempbuff[1000];
    int slen=-2,iRet=-2;
  //  FILE *fp=NULL;

    iRet = iMakeRequest(3);
    if (iRet!=FUNC_OK)
    {
        return FUNC_ERROR;
    }
    //memcpy(gSendData,Recvdata,sizeof(Recvdata));

    slen=strlen((char *)Recvdata);
    slen-=2;
    memset(gSendData,0,sizeof(gSendData));

#ifdef UAT_HTTP     // File Download

    strncpy((char *)gSendData,"POST /UIService.svc/AEPSProcessRequest HTTP/1.1\r\n",52);
    strncat((char *)gSendData,"Host: 202.58.125.2:9013\r\n",27);
    strncat((char *)gSendData,"Content-Type: application/json; charset=utf-8\r\n",49);
    sprintf(tempbuff,"Content-Length: %d\r\n",slen);
    strncat((char *)gSendData,tempbuff,strlen(tempbuff));
    strcat((char *)gSendData,(char *)Recvdata);
#else
    strncpy((char *)gSendData,(char *)Recvdata,strlen((char *)Recvdata));
#endif

#if DEBUG
    FILE *fp;
    system("cd /usr/FINO/FPB/REQ/");
    system("rm /usr/FINO/FPB/REQ/WITH_REQ.TXT");
    fp =fopen("/usr/FINO/FPB/REQ/WITH_REQ.TXT","a");
    fwrite(gSendData,strlen((char *)gSendData),1,fp);
    //ttestall(0,100);
    sleep(1);
    fclose(fp);
#endif

    return FUNC_OK;
}


int FPB::iSetAEPS_With(int tagID, unsigned char *value)
{
    switch(tagID)
    {
    case  1:
        strcpy((char *)value,(char *)AEPSWITH.ucRequestID_AWreq);
   //     qDebug()<<"ucRequestID_AWreq 090";
    //    printf("AEPSWITH.ucRequestID_AWreq == %s\n",AEPSWITH.ucRequestID_AWreq);
        //memcpy((char *)value,configLoginLeg1.ucRequestID_req,strlen(configLoginLeg1.ucRequestID_req));
        break;
    case  2:
        strcpy((char *)value,"516");//Method id
        break;
    case  3:
        strcpy((char *)value,(char *)AEPSWITH.ucTellerID_AWreq);//teller id
        break;
    case  4:
        strcpy((char *)value,(char *)AEPSWITH.ucSessionID_AWreq);//session key
    //    printf("AEPSWITH.ucSessionID_AWreq == %s\n",AEPSWITH.ucSessionID_AWreq);
        break;
    case  5:
        strcpy((char *)value,"");//token id
        break;
    case  6:
        strcpy((char *)value,(char *)AEPSWITH.ucIsEncrypt_AWreq);//is encrypt
        break;
    case  7:
        strcpy((char *)value,(char *)AEPSWITH.ucmobileNumber_AWreq);
        break;
    case  8:
        strcpy((char *)value,(char *)AEPSWITH.ucmerchantMobileNumber_AWreq);
   //     printf("AEPSWITH.ucmerchantMobileNumber_AWreq == %s\n",AEPSWITH.ucmerchantMobileNumber_AWreq);
        break;
    case  9:
        strcpy((char *)value,(char *)AEPSWITH.uctotalAmount_AWreq);
        break;
    case  10:
        strcpy((char *)value,"0");
        break;
    case  11:
        strcpy((char *)value,(char *)AEPSWITH.ucAadharNo_AWreq);
   //     printf("AEPSWITH.ucAadharNo_AWreq == %s\n",AEPSWITH.ucAadharNo_AWreq);
        break;
    case  12:
        strcpy((char *)value,"");//free
        break;
    case  13:
        strcpy((char *)value,(char *)AEPSWITH.ucUID_AWreq);
        break;
    case  14:
        strcpy((char *)value,"AUTH");
        break;
    case  15:
        strcpy((char *)value,(char *)RD_Service_data.PID_DATA.toAscii().constData()/*AEPSWITH.ucPID_AWreq*/);
        break;
    case  16:
        strcpy((char *)value,(char *)RD_Service_data.hmac.toAscii().constData()/*AEPSWITH.ucHmac_AWreq*/);
        break;
    case  17:
        strcpy((char *)value,(char *)RD_Service_data.skey.toAscii().constData()/*AEPSWITH.ucSkey_AWreq*/);
        break;
    case  18:
        strcpy((char *)value,(char *)EncPIDData/*AEPSWITH.ucPidData_AWreq*/);
        break;
    case  19:
        strcpy((char *)value,"Y");
        break;
    case  20:
        strcpy((char *)value,(char *)AEPSWITH.ucAppId_AWreq);
        break;
    case  21:
        strcpy((char *)value,"0");
        break;
    case  22:
        strcpy((char *)value,"0");
        break;
    case  23:
        strcpy((char *)value,"182781838054");
        break;
    case  24:
        strcpy((char *)value,"null");
        break;
    case  25:
        //strcpy((char *)value,"");//free
        strcpy((char *)value,(char *)AEPSWITH.uctotalAmount_AWreq);
        break;
    case  26:
        strcpy((char *)value,"INR");
        break;
    case  27:
        strcpy((char *)value,"CASHWAEPSACQ");
        break;
    case  28:
        strcpy((char *)value,(char *)AEPSWITH.uccostCenter_AWreq); // CostCenter from Leg2
        break;
    case  29:
        strcpy((char *)value,"OBU60029078");
        break;
    case  30:
        strcpy((char *)value,(char *)AEPSWITH.uccard_acceptor_namelocation_AWreq);
        break;
    case  31:
        strcpy((char *)value,(char *)AEPSWITH.ucNBIN_AWreq); //NBIN
        break;
    case  32:
        strcpy((char *)value,"80000001");//Terminal ID
        break;
    case  33:
        strcpy((char *)value,"183805");
        break;
    case  34:
        strcpy((char *)value,(char *)AEPSWITH.ucbankName_AWreq);
        break;
    case  35:
        strcpy((char *)value,(char *)AEPSWITH.ucSessionExpiryTime_AWreq);
        break;

    case  36:
        strcpy((char *)value,(char *)AEPSWITH.ucAFTsupportData_PVTXNID_AWreq);
   //     printf("AEPSWITH.ucAFTsupportData_PVTXNID_AWreq = %s\n", AEPSWITH.ucAFTsupportData_PVTXNID_AWreq);
        break;
    case  37:
        strcpy((char *)value,(char *)AEPSWITH.ucAFTsupportData_PVTXNDT_AWreq);
  //      printf("AEPSWITH.ucAFTsupportData_PVTXNID_AWreq = %s\n", AEPSWITH.ucAFTsupportData_PVTXNID_AWreq);
        break;
    case  38:
        strcpy((char *)value,(char *)AEPSWITH.ucAFTsupportData_IPADDR_AWreq);
 //       printf("AEPSWITH.ucAFTsupportData_PVTXNDT_AWreq = %s\n", AEPSWITH.ucAFTsupportData_PVTXNDT_AWreq);
        break;
    case  39:
        strcpy((char *)value,(char *)AEPSWITH.ucAFTsupportData_DEVICEID_AWreq);
  //      printf("AEPSWITH.ucAFTsupportData_DEVICEID_AWreq = %s\n", AEPSWITH.ucAFTsupportData_DEVICEID_AWreq);
        break;
    case  40:
        strcpy((char *)value,(char *)AEPSWITH.ucAFTsupportData_MCCCellID_AWreq);
 //      printf("AEPSWITH.ucAFTsupportData_MCCCellID_AWreq = %s\n", AEPSWITH.ucAFTsupportData_MCCCellID_AWreq);
        break;
    case  41:
        strcpy((char *)value,(char *)AEPSWITH.ucAFTsupportData_MCCLAC_AWreq);
    //    printf("AEPSWITH.ucAFTsupportData_MCCLAC_AWreq = %s\n", AEPSWITH.ucAFTsupportData_MCCLAC_AWreq);
        break;
    case  42:
        strcpy((char *)value,(char *)AEPSWITH.ucAFTsupportData_MCCMCC_AWreq);
//        printf("AEPSWITH.ucAFTsupportData_MCCMCC_AWreq = %s\n", AEPSWITH.ucAFTsupportData_MCCMCC_AWreq);
        break;
    case  43:
        strcpy((char *)value,(char *)AEPSWITH.ucAFTsupportData_MCCMNC_AWreq); //NBIN
   //     printf("AEPSWITH.ucAFTsupportData_MCCMNC_AWreq = %s\n", AEPSWITH.ucAFTsupportData_MCCMNC_AWreq);
        break;
    case  44:
        strcpy((char *)value,(char *)AEPSWITH.ucAFTsupportData_PRTXNID_AWreq);//Terminal ID
  //      printf("AEPSWITH.ucAFTsupportData_PRTXNID_AWreq = %s\n", AEPSWITH.ucAFTsupportData_PRTXNID_AWreq);
        break;
    case  45:
        strcpy((char *)value,(char *)AEPSWITH.ucAFTsupportData_SPLTSEQ_AWreq);
  //      printf("AEPSWITH.ucAFTsupportData_SPLTSEQ_AWreq = %s\n", AEPSWITH.ucAFTsupportData_SPLTSEQ_AWreq);
        break;
    case  46:
        strcpy((char *)value,(char *)AEPSWITH.ucAFTsupportData_CHGAMT_AWreq);
   //     printf("AEPSWITH.ucAFTsupportData_CHGAMT_AWreq = %s\n", AEPSWITH.ucAFTsupportData_CHGAMT_AWreq);
        break;
    case  47:
        strcpy((char *)value,(char *)AEPSWITH.ucChannelID_AWreq);
   //     printf("AEPSWITH.ucChannelID_AWreq = %s\n", AEPSWITH.ucChannelID_AWreq);
        break;
    case  48:
        strcpy((char *)value,(char *)AEPSWITH.ucAFTsupportData_CELLIDLatitude_AWreq);
    //    printf("AEPSWITH.ucAFTsupportData_CELLIDLatitude_AWreq = %s\n", AEPSWITH.ucAFTsupportData_CELLIDLatitude_AWreq);
        break;
    case  49:
        strcpy((char *)value,(char *)AEPSWITH.ucAFTsupportData_CELLIDLongitude_AWreq);
  //      printf("AEPSWITH.ucAFTsupportData_CELLIDLongitude_AWreq = %s\n", AEPSWITH.ucAFTsupportData_CELLIDLongitude_AWreq);
        break;
    case  50:
        strcpy((char *)value,(char *)AEPSWITH.ucaccountNumber_AWreq);
  //      printf("AEPSWITH.ucaccountNumber_AWreq = %s\n", AEPSWITH.ucaccountNumber_AWreq);
        break;

    default:
        return FUNC_ERROR;
    }
    return FUNC_OK;
}

int FPB::iParseAEPS_With(unsigned char *Data)
{
    qDebug()<<"iParseAEPS_With";
    int len,i=0,slen,len2=-1;
    //int j=0;
    char *RecvJsonData;
    char *CheckData;
    char buff[1024];

    //    char *LoginLeg1[]={"RequestId:",
    //            "DisplayMessage:",
    //            "ResponseMessage:",
    //            "ResponseCode:",
    //            "MessageType:",
    //            "ResponseData:returnCode:",
    //            "ResponseData:AuthResponse:",
    //            "ResponseData:AEPSOffusResponse:returnCode:",
    //            "ResponseData:AEPSOffusResponse:responseMessage:",
    //            "ResponseData:AEPSOffusResponse:txnReferenceNo:",
    //            "ResponseData:AEPSOffusResponse:AuthCode:",//10
    //            "ResponseData:AEPSOffusResponse:TransactionTime:",
    //            "ResponseData:AEPSOffusResponse:TransactionDate:",
    //            "ResponseData:AEPSOffusResponse:AuthUID:",
    //            "ResponseData:AEPSOffusResponse:TerminalId:",
    //            "ResponseData:AEPSOffusResponse:cardAcceptorCode:",
    //            "ResponseData:AEPSOffusResponse:CardAcceptorNameLocation:",
    //            "ResponseData:AEPSOffusResponse:balancesList:ledgerBalance:",
    //            "ResponseData:AEPSOffusResponse:balancesList:availableBalance:",
    //            "ResponseData:AEPSOffusResponse:balancesList:currency:",
    //            "ResponseData:PostTransResponse:returnCode:",//20
    ////            "ResponseData:PostTransResponse:responseMessage:",
    //            "ResponseData:TerminalId:",
    //            "MessageId:",
    //            "SessionExpiryTime:",
    //            "SessionId:",
    //            "RouteID:",//30
    //            "ErrorCode:",
    //            "XMLData:",
    //            "AuthmanFlag:",
    //            "ServiceID:"//34
    //    };

    char *LoginLeg1[]={"RequestId:",
                       "DisplayMessage:",
                       "ResponseMessage:",
                       "ResponseCode:",
                       "MessageType:",
                       "ResponseData:returnCode:",
                       "ResponseData:AuthResponse:",
                       "ResponseData:AEPSOffusResponse:returnCode:",
                       "ResponseData:AEPSOffusResponse:responseMessage:",
                       "ResponseData:AEPSOffusResponse:txnReferenceNo:",
                       "ResponseData:AEPSOffusResponse:AuthCode:",//10
                       "ResponseData:AEPSOffusResponse:TransactionTime:",
                       "ResponseData:AEPSOffusResponse:TransactionDate:",
                       "ResponseData:AEPSOffusResponse:AuthUID:",
                       "ResponseData:AEPSOffusResponse:TerminalId:",
                       "ResponseData:AEPSOffusResponse:cardAcceptorCode:",
                       "ResponseData:AEPSOffusResponse:CardAcceptorNameLocation:",
                       "ResponseData:AEPSOffusResponse:balancesList:ledgerBalance:",
                       "ResponseData:AEPSOffusResponse:balancesList:availableBalance:",
                       "ResponseData:AEPSOffusResponse:balancesList:currency:",
                       "ResponseData:PostTransResponse:returnCode:",//20
                       //            "ResponseData:PostTransResponse:responseMessage:",
                       "ResponseData:TerminalId:",
                       "MessageId:",
                       "SessionExpiryTime:",
                       "SessionId:",
                       "RouteID:",//30
                       "ErrorCode:",
                       "XMLData:",
                       "AuthmanFlag:",
                       "ServiceID:"//34
                       "ResponseData:PostTransResponse:cbsTxnReferenceNo:",
                       "ResponseData:PostTransResponse:balancesList:ledgerBalance:",
                       "ResponseData:PostTransResponse:balancesList:availableBalance:",
                       "ResponseData:PostTransResponse:balancesList:currency:",
                      };


    /*"ResponseData:PostTransResponse:txnReferenceNo:",*/
    /* "ResponseData:PostTransResponse:cbsTxnReferenceNo:",
    "ResponseData:PostTransResponse:balancesList:ledgerBalance:",
    "ResponseData:PostTransResponse:balancesList:availableBalance:",
    "ResponseData:PostTransResponse:balancesList:currency:",*/

    // int MandTag[]={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0};//1 for mandatory and  0 for non-mandatory by saurabh
    // int MandTag[]={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1};//1 for mandatory and  0 for non-mandatory by saurabh
    int MandTag[]={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0};//1 for mandatory and  0 for non-mandatory by saurabh

    len=sizeof(LoginLeg1)/sizeof(LoginLeg1[0]);
    len2=sizeof(MandTag)/sizeof(MandTag[0]);
    for (i = 0; i < len; ++i)
    {
        RecvJsonData=strstr((char *)Data,LoginLeg1[i]);
        slen=strlen(LoginLeg1[i]);
      //  qDebug()<<"i = "<<i;
        if (RecvJsonData==NULL && MandTag[i]==1)//If Tag not found and its mandatory.
        {
            //DrawString("TAG Missing",15,25,0);
            // ttestall(0,200);
            return FUNC_ERROR;
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
        {case 0:
            strcpy((char *)AEPSWITH.ucRequestId,buff);
            break;

        case 1:
            strcpy((char *)AEPSWITH.ucDisplayMessage,buff);
            break;

        case 2:
            strcpy((char *)AEPSWITH.ucResponseMessage,buff);
            break;

        case 3:
            strcpy((char *)AEPSWITH.ucResponseCode,buff);
            break;

        case 4:
            strcpy((char *)AEPSWITH.ucMessageType,buff);
            break;

        case 5:
            strcpy((char *)AEPSWITH.ucreturnCode,buff);
            break;

        case 6:
            strcpy((char *)AEPSWITH.ucAuthResponse,buff);
            break;

        case 7:
            strcpy((char *)AEPSWITH.ucAORreturnCode,buff);
            break;

        case 8:
            strcpy((char *)AEPSWITH.ucresponseMessage,buff);
            break;

        case 9:
            strcpy((char *)AEPSWITH.ucAORtxnReferenceNo,buff);
            strcpy((char *)ConfigPrint.ucRRN,buff);
            break;

        case 10:
            strcpy((char *)AEPSWITH.ucAuthCode,buff);
            strcpy((char *)ConfigPrint.ucAUTHCode,buff);
            break;

        case 11:
            strcpy((char *)AEPSWITH.ucTransactionTime,buff);
            break;

        case 12:
            strcpy((char *)AEPSWITH.ucTransactionDate,buff);
            break;

        case 13:
            strcpy((char *)AEPSWITH.ucAuthUID,buff);

            break;

        case 14:
            strcpy((char *)AEPSWITH.ucAORTerminalId,buff);

            break;

        case 15:
            strcpy((char *)AEPSWITH.uccardAcceptorCode,buff);
            break;

        case 16:
            strcpy((char *)AEPSWITH.ucCardAcceptorNameLocation,buff);
            break;

        case 17:
            strcpy((char *)AEPSWITH.ucAORledgerBalance,buff);
            break;

        case 18:
            strcpy((char *)AEPSWITH.ucavailableBalance,buff);
            break;

        case 19:
            strcpy((char *)AEPSWITH.uccurrency,buff);
            break;

        case 20:
            strcpy((char *)AEPSWITH.ucPTRreturnCode,buff);
            break;

            /*case 21:
            strcpy((char *)AEPSWITH.ucPTRresponseMessege,buff);
            break;

        case 22:
            strcpy((char *)AEPSWITH.uccbsTxnReferenceNo,buff);
            break;

        case 23:
            strcpy((char *)AEPSWITH.ucPTRledgerBalance,buff);
            break;

        case 24:
            strcpy((char *)AEPSWITH.ucPTRavailableBalance,buff);
            break;

        case 25:
            strcpy((char *)AEPSWITH.ucPTRcurrency,buff);
            break;
             */
        case 21:
            strcpy((char *)AEPSWITH.ucTerminalId,buff);
            strcpy((char *)ConfigPrint.ucTerminalID,buff);
            break;

        case 22:
            strcpy((char *)AEPSWITH.ucMessageId,buff);
            break;

        case 23:
            strcpy((char *)AEPSWITH.ucSessionExpiryTime,buff);
            break;

        case 24:
            strcpy((char *)AEPSWITH.ucSessionId,buff);
            break;

        case 25:
            strcpy((char *)AEPSWITH.ucRouteID,buff);
            break;

        case 26:
            strcpy((char *)AEPSWITH.ucErrorCode,buff);
            break;

        case 27:
            strcpy((char *)AEPSWITH.ucXMLData,buff);
            break;

        case 28:
            strcpy((char *)AEPSWITH.ucAuthmanFlag,buff);
            break;

        case 29:
            strcpy((char *)AEPSWITH.uccbsTxnReferenceNo,buff);
        //    printf("AEPSWITH.uccbsTxnReferenceNo = %s\n",AEPSWITH.uccbsTxnReferenceNo);
            break;

        case 30:
            strcpy((char *)AEPSWITH.ucPTRledgerBalance,buff);
         //   printf("AEPSWITH.ucPTRledgerBalance = %s\n",AEPSWITH.ucPTRledgerBalance);
            break;

        case 31:
            strcpy((char *)AEPSWITH.ucPTRavailableBalance,buff);
       //     printf("AEPSWITH.ucPTRavailableBalance = %s\n",AEPSWITH.ucPTRavailableBalance);
            break;

        case 32:
            strcpy((char *)AEPSWITH.ucPTRcurrency,buff);
       //     printf("AEPSWITH.ucPTRcurrency = %s\n",AEPSWITH.ucPTRcurrency);
            break;

        }
    }
    return FUNC_OK;
}



