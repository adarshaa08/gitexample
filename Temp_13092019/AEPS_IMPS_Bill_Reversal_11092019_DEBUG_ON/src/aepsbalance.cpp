#include "aepsbalance.h"
#include "ui_FPB.h"
#include "json.h"
#include "FPB.h"
#include "Login.h"
#include "displaymessage.h"
#include "print.h"
extern Login_leg2 configLoginLeg2;
extern Login_leg1 configLoginLeg1;
extern RD_SERVICE_STRUCTURE RD_Service_data;
extern Transaction Txn;
extern Printfpb ConfigPrint;

void FPB::on_BALENQ_pushButton_clicked()
{

    int ret;

    memset(&AEPSBALENQ,0,sizeof(AEPSBALENQ));
    unsigned char BankBin[10];

    ui->lineEdit_Bank_Search->clear();
    memset(BankBin,0,sizeof(BankBin));
    Check_Conditions();
    int iret = validate_connection();

    ret = AutoLTS();
    if(ret!=FUNC_OK && ret!=FUNCTION_SUCCESS)
    {
        uidmessage::instance()->hide_Message();
        ui->SubTransaction_Label1 ->setText((char *)configLoginLeg2.ucname);
        ui->SubTransaction_Label1 ->setFont(QFont("SansSerif",10));
        ui->SubTransaction_Label1 ->setAlignment(Qt::AlignRight | Qt::AlignBottom);

        ui->SubTransaction_Label2->setText(QString::fromUtf8((const char*)Balance_Display));
        ui->SubTransaction_Label2->setFont(QFont("SansSerif",9));
        ui->SubTransaction_Label2->setAlignment(Qt::AlignRight | Qt::AlignBottom);
        ui->stackedWidget->setCurrentIndex(2);
        return;
    }

    ////    int i_ret = AEPSProcess();
    ////    if(i_ret == FUNCTION_FAILED)
    ////    {
    ////        qDebug()<<"Return Failed from AEPSProcess";
    ////        ui->stackedWidget->setCurrentIndex(2);
    ////        return;
    ////    }


    ui->lineEditUID1->setReadOnly(false);
    ui->lineEditUID1->setFocus();
    ui->lineEditUID2->setReadOnly(false);
    ui->lineEditUID3->setReadOnly(false);
    ui->PAGE4_OK_pushButton_8->setEnabled(false);
    ui->PAGE4_OK_pushButton_8->setStyleSheet("background-color: rgb(85, 85, 127); color: rgb(107, 107, 107)");
    QRegExp rx("\\d+");
    QRegExpValidator *validator1 = new QRegExpValidator(rx,this);
    ui->lineEditUID1->setValidator(validator1);
    ui->lineEditUID2->setValidator(validator1);
    ui->lineEditUID3->setValidator(validator1);




    memset(&Txn,0,sizeof(Txn)); // clearing Transaction Structure
    Txn.Txntype = AEPS_BAL;
    uidmessage::instance()->hide_Message();

    ui->BankName_LabelName ->setText((char *)configLoginLeg2.ucname);
    ui->BankName_LabelName ->setFont(QFont("SansSerif",10));
    ui->BankName_LabelName ->setAlignment(Qt::AlignRight | Qt::AlignBottom);

    ui->BankName_LabelBal->setText(QString::fromUtf8((const char*)Balance_Display));
    ui->BankName_LabelBal->setFont(QFont("SansSerif",9));
    ui->BankName_LabelBal->setAlignment(Qt::AlignRight | Qt::AlignBottom);
    QRegExp rx1("\\w+");
    QRegExpValidator *validator2 = new QRegExpValidator(rx1,this);
    ui->lineEdit_Bank_Search->setValidator(validator2);

    ui->stackedWidget->setCurrentIndex(7); // go to page ENTER Bank Search

    return;

    //    memset(ErrorMsg,0,sizeof(ErrorMsg));
    //    memcpy((char *)AEPSBALENQ.ucAadharNo_BEreq,BankBin,strlen((char *)BankBin));
    //    strcat((char *)AEPSBALENQ.ucAadharNo_BEreq,(char *)AEPSBALENQ.ucUID_BEreq);
    //    memcpy((char *)AEPSBALENQ.ucSessionId_BEreq,configLoginLeg2.ucSessionId,strlen((char *)configLoginLeg2.ucSessionId));
    //    memcpy((char *)AEPSBALENQ.ucSessionExpiryTime_BEreq,configLoginLeg2.ucSessionExpiryTime,strlen((char *)configLoginLeg2.ucSessionExpiryTime));
    //    sprintf((char *)AEPSBALENQ.uccard_acceptor_namelocation_BEreq,"%s %s %s%s",configLoginLeg2.ucaddressLine1,configLoginLeg2.ucaddressLine2,configLoginLeg2.uccity,configLoginLeg2.uccountry);
    //    memcpy((char *)AEPSBALENQ.ucmerMobileNo_BEreq,configLoginLeg2.ucmobileNumber,10);
    //    iGenerateRequestID((char *)configLoginLeg2.ucUsername_req,(unsigned char *)&AEPSBALENQ.ucRequestId_BEreq);

    //    ret	= makeJSONAEPSBALENQ();/*-------------Make REQUEST--------------*/
    //    if (ret != FUNC_OK)
    //    {
    //        uidmessage::instance()->Display_BusyMessage("JSON CREATION FAILED");
    //        qDebug()<<"JSON Creation failed";
    //        uidmessage::instance()->hide_Message();
    //        ui->stackedWidget->setCurrentIndex(2);
    //    }


    //#ifdef UAT_HTTP     // File Download

    //    hComm.close_connection();
    //    res = hComm.open_connect(SERVERIP,SERVERPORT);
    //    if(res != 0)
    //    {
    //        qDebug() << "Failed to connect to the Server";
    //        uidmessage::instance()->hide_Message();
    //        uidmessage::instance()->DisplayMessage("Server Not Connected");
    //        qDebug()<<"FUNCTION_FAILED";
    //        ui->stackedWidget->setCurrentIndex(0);
    //    }

    //    hComm.close_connection();
    //    res = hComm.open_connect(SERVERIP,SERVERPORT);
    //    if(res != 0)
    //    {
    //        qDebug() << "Failed to connect to the Server";
    //        uidmessage::instance()->hide_Message();
    //        uidmessage::instance()->DisplayMessage("Server Not Connected");
    //        qDebug()<<"FUNCTION_FAILED";
    //        ui->stackedWidget->setCurrentIndex(0);
    //    }


    //    sleep(1);
    //    uidmessage::instance()->hide_Message();
    //    uidmessage::instance()->Display_BusyMessage("Transmitting .....");

    //    int len_gSendData = strlen((char *)gSendData);
    //    memset(Recvdata,0,sizeof(Recvdata));
    //    //if((hComm.host_2Sendrecv((unsigned char *)gSendData,len_gSendData,( unsigned char*)Recvdata,(int *)&BalLen,(int)response_timeout1,0))!=SUCCESS)
    //    if((hComm.host_2Sendrecv((unsigned char *)gSendData,len_gSendData,( unsigned char*)oBuff,(int *)&BalLen,(int)response_timeout1,0))!=SUCCESS)
    //    {
    //        printf("No Response From Server\n");
    //        hComm.close_connection();
    //        miscObj.DisplayMsgBox((char *)"Response Time-Out");
    //        uidmessage::instance()->hide_Message();
    //        qDebug()<<"***   Response Time-Out   ***";
    //        ui->stackedWidget->setCurrentIndex(1);
    //    }
    //    hComm.close_connection();

    //#else     // File Download full condition

    //    printf("gSendData = %s\n\n",gSendData);

    //    memset(str_url,0,sizeof(str_url));
    //#ifdef PRODUCTION
    //    strcpy(str_url,URL_CREATION(AEPS_BLENQ_URL_PROD));
    //#else
    //    strcpy(str_url,URL_CREATION(AEPS_BLENQ_URL));
    //#endif

    //    int retk = file_download(str_url, (char *)gSendData,"/usr/FINO/FPB/RESP/OUTPUT.TXT",120);
    //    if(retk!=0)
    //    {
    //        qDebug()<<"retk = "<<retk;
    //        sleep(1);
    //        uidmessage::instance()->hide_Message();
    //        return ;//FUNCTION_FAILED;
    //    }

    //    /*    file_download( "https://103.1.112.204/UIService.svc/ProcessLoginRequest", (char *)gSendData,"/usr/FINO/FPB/RESP/LoginCurl1.TXT",50);
    //    file_download( "https://partneruat.finopaymentbank.in/UIService.svc/ProcessLoginRequest", (char *)gSendData,"/usr/FINO/FPB/RESP/OUTPUT.TXT",90);*/

    //#endif



    //    //ret=json->iParsJSON(Recvdata,(char *)&AEPSBALENQ.ucResponseCode,(char *)&AEPSBALENQ.ucResponseMessage);
    //    ret=json->iParsJSON((unsigned char*)oBuff,(char *)&AEPSBALENQ.ucResponseCode,(char *)&AEPSBALENQ.ucResponseMessage);
    //    if(ret!=FUNC_OK)
    //    {
    //        sprintf(ErrorMsg,"Response Code:%s \n Message:%s",(char *)configLoginLeg1.ucResponseCode,(char *)configLoginLeg1.ucResponseMessage);
    //        miscObj.DisplayMsgBox(ErrorMsg);
    //        uidmessage::instance()->hide_Message();
    //        ui->stackedWidget->setCurrentIndex(2);
    //    }
    //    if (!memcmp(AEPSBALENQ.ucResponseCode,"0",1))
    //    {
    //        miscObj.DisplayMsgBox("Balance Enquiry Successful.");
    //        ////        memcpy((char *)ConfigPrint.ucSTAN,&AEPSBALENQ.uctxnReferenceNo[6],6);
    //        ////        memcpy((char *)ConfigPrint.ucTranStatus,"Success(0)",10);
    //        ////        memcpy((char *)ConfigPrint.ucBCName,configLoginLeg2.ucname,15);
    //        ////        memcpy((char *)ConfigPrint.ucBCLocation,AEPSBALENQ.ucCardAcceptorNameLocation,strlen((char *)AEPSBALENQ.ucCardAcceptorNameLocation));
    //        ////        memcpy((char *)ConfigPrint.ucAgentID,configLoginLeg2.ucUsername_req,6);
    //        ////        sprintf((char *)ConfigPrint.ucCustAadharNo,"********%s",&AEPSBALENQ.ucUID_BEreq[8]);
    //        ////        sprintf((char *)ConfigPrint.ucACBalance,"Rs. %s",AEPSBALENQ.ucavailableBalance);
    //        //        //if(nPrintTxn_NPCI( 1 ,0) != FUNC_OK);
    //        //        //return (FUNC_ERROR);
    //    }
    //    else{
    //        miscObj.DisplayMsgBox("Balance Enquiry Failed.");
    //        uidmessage::instance()->hide_Message();
    //    }

    //    memset(&RD_Service_data,0,sizeof(RD_Service_data));
    //    memset(&AEPSBALENQ,0,sizeof(AEPSBALENQ));
    //    return ;
}


int FPB::makeJSONAEPSBALENQ()
{
    memset(gSendData,0,sizeof(gSendData));
    char tempbuff[30];
    int slen=-2,iRet=-2;

    iRet = iMakeRequest(4);
    if (iRet!=FUNC_OK)
    {
        return FUNC_ERROR;
    }

    slen=strlen((char *)Recvdata);
    slen-=2;
    memset(gSendData,0,sizeof(gSendData));//AEPSProcessRequest

#ifdef UAT_HTTP     // File Download
    strncpy((char *)gSendData,"POST /UIService.svc/AEPSProcessRequest HTTP/1.1\r\n",52);
    strncat((char *)gSendData,"Host: 202.58.125.20\r\n",27);//9013
    strncat((char *)gSendData,"Content-Type: application/json; charset=utf-8\r\n",49);
    sprintf(tempbuff,"Content-Length: %d\r\n",slen);
    strncat((char *)gSendData,tempbuff,strlen(tempbuff));
    strcat((char *)gSendData,(char *)Recvdata);
#else
    strncpy((char *)gSendData,(char *)Recvdata,strlen((char *)Recvdata));
#endif

    //memcpy(gSendData,Recvdata,sizeof(Recvdata));
#if DEBUG
    system("cd /usr/FINO/FPB/REQ/");
    system("rm /usr/FINO/FPB/REQ/BALENQ_req.TXT");
    FILE *fp = NULL;
    fp =fopen("/usr/FINO/FPB/REQ/BALENQ_req.TXT","a");
    fwrite(gSendData,strlen((char *)gSendData),1,fp);
    //ttestall(0,100);
    sleep(1);
    fclose(fp);
#endif

    return FUNC_OK;
}

int FPB::Prepare_AEPS_BAL_Func()
{
    HostComm hComm;
    CMisc miscObj;
    int BLEQ_len = 0, response_timeout1 = 0, ret = -3, res = 0, iRet = -3;
    //char BLEQ_Buff[10000];
    char ErrorMsg[150];
    char BuffCardAdr[100], Buff_Final[500];
    FILE *fp1;
    int template_len=0;
    unsigned char  *template_data;
    int gSendData_len = 0;
    int oBuff_len =0;
    uidmessage::instance()->hide_Message();
    uidmessage::instance()->Display_BusyMessage("Please Wait...");
    memset(ErrorMsg,0,sizeof(ErrorMsg));
    memset(AEPSBALENQ.ucAadharNo_BEreq,0,sizeof(AEPSBALENQ.ucAadharNo_BEreq));
    memset(AEPSBALENQ.ucSessionId_BEreq,0,sizeof(AEPSBALENQ.ucSessionId_BEreq));
    memset(AEPSBALENQ.ucSessionExpiryTime_BEreq,0,sizeof(AEPSBALENQ.ucSessionExpiryTime_BEreq));
    memset(AEPSBALENQ.uccard_acceptor_namelocation_BEreq,0,sizeof(AEPSBALENQ.uccard_acceptor_namelocation_BEreq));
    memset(AEPSBALENQ.ucmerMobileNo_BEreq,0,sizeof(AEPSBALENQ.ucmerMobileNo_BEreq));
    memset(AEPSBALENQ.ucAppId_BEreq,0,sizeof(AEPSBALENQ.ucAppId_BEreq));
    memset(AEPSBALENQ.ucChannelID_BEreq,0,sizeof(AEPSBALENQ.ucChannelID_BEreq));
    memset(AEPSBALENQ.ucRequestId_BEreq,0,sizeof(AEPSBALENQ.ucRequestId_BEreq));

    memset(BuffCardAdr, 0, sizeof(BuffCardAdr));
    memset(Buff_Final,  0, sizeof(Buff_Final));

    memcpy((char *)AEPSBALENQ.ucUID_BEreq, Txn.uid,12);
    memcpy((char *)AEPSBALENQ.ucAadharNo_BEreq,Txn.bin,strlen((char *)Txn.bin));
    strcat((char *)AEPSBALENQ.ucAadharNo_BEreq,"0");
    strcat((char *)AEPSBALENQ.ucAadharNo_BEreq,(char *)AEPSBALENQ.ucUID_BEreq);
    memcpy((char *)AEPSBALENQ.ucSessionId_BEreq,configLoginLeg2.ucSessionId,strlen((char *)configLoginLeg2.ucSessionId));
    memcpy((char *)AEPSBALENQ.ucSessionExpiryTime_BEreq,configLoginLeg2.ucSessionExpiryTime,strlen((char *)configLoginLeg2.ucSessionExpiryTime));
    memcpy((char *)AEPSBALENQ.ucChannelID_BEreq,CHANNEL_ID,strlen(CHANNEL_ID));
    memcpy((char *)AEPSBALENQ.ucAppId_BEreq,CLIENT_ID,strlen(CLIENT_ID));
    memcpy((char *)AEPSBALENQ.ucIsEncrypt_BEreq,ISENCEYPT,strlen(ISENCEYPT));
    memcpy((char *)AEPSBALENQ.uccard_acceptor_namelocation_BEreq, configLoginLeg2.ucFCCardAcceptorLocation, strlen(configLoginLeg2.ucFCCardAcceptorLocation));

//    sprintf((char *)BuffCardAdr,"%s%s",configLoginLeg2.ucaddressLine1,configLoginLeg2.ucaddressLine2);
//    sprintf((char *)Buff_Final,"%.19s %s %s%s",BuffCardAdr,configLoginLeg2.uccity, configLoginLeg2.ucstate, configLoginLeg2.uccountry);
//    int q_len = strlen((const char*)Buff_Final);
//    QString temp = QString::fromUtf8((const char*)Buff_Final);
//    if(q_len > 40)
//    {
//        temp = temp.mid(q_len-40).trimmed();
//        sprintf((char *)AEPSBALENQ.uccard_acceptor_namelocation_BEreq,"%s",(char*)temp.toAscii().data());
//        printf("In > 40 Cond. ===== %s\n\n",(char *)AEPSBALENQ.uccard_acceptor_namelocation_BEreq);
//    }
//    else
//    {
//        temp = temp.trimmed();
//        sprintf((char *)AEPSBALENQ.uccard_acceptor_namelocation_BEreq,"%s",(char*)temp.toAscii().data());
//    //    printf("In <= 40 Cond. ===== %s\n\n",(char *)AEPSBALENQ.uccard_acceptor_namelocation_BEreq);
//    }

    //sprintf((char *)AEPSBALENQ.uccard_acceptor_namelocation_BEreq,"%s %s %s%s",configLoginLeg2.ucaddressLine1,configLoginLeg2.ucaddressLine2,configLoginLeg2.uccity,configLoginLeg2.uccountry);
    ////sprintf((char *)AEPSBALENQ.uccard_acceptor_namelocation_BEreq,"%s%s %s %s%s",(char *)configLoginLeg2.ucaddressLine1,configLoginLeg2.ucaddressLine2,configLoginLeg2.uccity,configLoginLeg2.ucstate,configLoginLeg2.uccountry);
    memcpy((char *)AEPSBALENQ.ucmerMobileNo_BEreq,configLoginLeg2.ucmobileNumber,10);
    iGenerateRequestID((char*)configLoginLeg2.ucUsername_req, (unsigned char *)&AEPSBALENQ.ucRequestId_BEreq);

    //    uidmessage::instance()->hide_Message();
    //    uidmessage::instance()->DisplayMessage("Please Wait...");


    ret	= makeJSONAEPSBALENQ();/*-------------Make REQUEST--------------*/
    if(ret!=FUNC_OK)
    {
        qDebug()<<"JSON Creation failed";
        return FUNC_ERROR;
    }

    /****************************Encryption function*********************************/
    if(!strcmp((char *)AEPSBALENQ.ucIsEncrypt_BEreq,"true"))
    {
        gSendData_len =strlen((char *)gSendData);
        RequestDetachEncryptAttach((char *)gSendData,1,gSendData_len);
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
        return FUNCTION_FAILED;
    }

    uidmessage::instance()->hide_Message();
    uidmessage::instance()->Display_BusyMessage("Transmitting .....");

    int len_gSendData = strlen((char *)gSendData);
    memset(BLEQ_Buff,0,sizeof(BLEQ_Buff));
    memset(oBuff,0,sizeof(oBuff));
    if((hComm.host_2Sendrecv((unsigned char *)gSendData,len_gSendData,( unsigned char*)oBuff,(int *)&BLEQ_len,(int)response_timeout1,0))!=SUCCESS)
    {
        printf("No Response From Server\n");
        hComm.close_connection();
        miscObj.DisplayMsgBox((char *)"Response Time-Out");
        uidmessage::instance()->hide_Message();
        qDebug()<<"***   Response Time-Out   ***";
        ui->stackedWidget->setCurrentIndex(1);
        return FUNCTION_FAILED;
    }
    hComm.close_connection();

#else     // File Download full condition
#if DEBUG
    printf("gSendData = %s\n\n",gSendData);
#endif
    memset(str_url,0,sizeof(str_url));
#ifdef PRODUCTION
    strcpy(str_url,URL_CREATION(AEPS_BLENQ_URL_PROD));
#else
    strcpy(str_url,URL_CREATION((char *)AEPS_BLENQ_URL));
#endif

    int retk = file_download(str_url, (char *)gSendData,"/usr/FINO/FPB/RESP/OUTPUT.TXT",120);
    if(retk!=0)
    {
        qDebug()<<"retk = "<<retk;
        uidmessage::instance()->hide_Message();
        return FUNCTION_FAILED;
    }

#endif

    /****************************Decryption function*********************************/
    if(!strcmp((char *)AEPSBALENQ.ucIsEncrypt_BEreq,"true"))
    {
        oBuff_len = strlen(oBuff);
 #if DEBUG
        qDebug()<<"Length of buffer"<<oBuff_len;
        printf("Buffer%s\n",oBuff);
#endif
        ResponseDetachEncryptAttach(oBuff,2,oBuff_len);//TEMPORARY
    }
    /*******************************************************************************/

    ret=json->iParsJSON((unsigned char*)oBuff,(char *)&AEPSBALENQ.ucResponseCode,(char *)&AEPSBALENQ.ucResponseMessage);
    system("cd /usr/FINO/FPB/RESP/");
    system("rm /usr/FINO/FPB/RESP/OUTPUT.TXT");
    if(ret!=FUNC_OK)
    {
        sprintf(ErrorMsg,"%s",/*(char *)AEPSBALENQ.ucResponseCode,*/(char *)AEPSBALENQ.ucResponseMessage);
        miscObj.DisplayMsgBox(ErrorMsg);
        uidmessage::instance()->hide_Message();
        if(strcmp((char *)AEPSBALENQ.ucResponseMessage,"Session Expired.")==0)
        {
            //            miscObj.DisplayMsgBox("Session Is Expired. Please Login");
            uidmessage::instance()->hide_Message();
            //            ui->stackedWidget->setCurrentIndex(0);
            return SESSION_EXPIRED_FAILED;
        }
        return FUNCTION_FAILED;
    }
    if (!memcmp(AEPSBALENQ.ucResponseCode,"0",1))
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

        iRet = iParseAEPS_BalEnq((unsigned char*)template_data);
        free(template_data);
        system("cd /usr/FINO/FPB/RESP/");
        system("rm /usr/FINO/FPB/RESP/OUTPUT.TXT");
        system("rm /usr/FINO/FPB/REQ/IntJSON.TXT");

        if (iRet != FUNC_OK)
        {
            qDebug()<<"Inside Pardse  error";
            miscObj.DisplayMsgBox((char *)"Balance Enquiry Failed");
            uidmessage::instance()->hide_Message();
            return FUNCTION_FAILED;
        }
        uidmessage::instance()->hide_Message();
        uidmessage::instance()->Display_BusyMessage("Balance Enquiry Successful");
        sleep(5);
        // uidmessage::instance()->hide_Message();

        memcpy((char *)ConfigPrint.ucSTAN,&AEPSBALENQ.uctxnReferenceNo[6],6);
        memcpy((char *)ConfigPrint.ucTranStatus,"Success(0)",10);
        memcpy((char *)ConfigPrint.ucBCName,configLoginLeg2.ucname,15);
        memcpy((char *)ConfigPrint.ucBCLocation,AEPSBALENQ.ucCardAcceptorNameLocation,strlen((char *)AEPSBALENQ.ucCardAcceptorNameLocation));
        memcpy((char *)ConfigPrint.ucAgentID,configLoginLeg2.ucUsername_req,strlen((const char*)configLoginLeg2.ucUsername_req));
        sprintf((char *)ConfigPrint.ucCustAadharNo,"********%s",&AEPSBALENQ.ucUID_BEreq[8]);
        sprintf((char *)ConfigPrint.ucACBalance,"Rs. %s",AEPSBALENQ.ucavailableBalance);
#if DEBUG
        printf("ConfigPrint.ucACBalance = %s\n",ConfigPrint.ucACBalance);
        printf("ConfigPrint.ucBCLocation = %s\n",ConfigPrint.ucBCLocation);
#endif
        if(print_aes_transaction()== FUNC_OK)
        {
            qDebug()<<" Print SUCCESS";
        }
        else
        {
            qDebug()<<" Print FAILED";
            uidmessage::instance()->hide_Message();
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
        uidmessage::instance()->Display_BusyMessage("Balance Enquiry Failed");
        sleep(2);
        uidmessage::instance()->hide_Message();
        Txn.Txntype = 0;
        ui->stackedWidget->setCurrentIndex(2);
        return FUNCTION_FAILED;
    }

    memset(&AEPSBALENQ,0,sizeof(AEPSBALENQ));
    ui->stackedWidget->setCurrentIndex(2);
    return FUNC_OK;
}

int FPB::iSetAEPS_BalEnq(int tagID, unsigned char *value)
{
    switch(tagID)
    {
    case  1:
        strcpy((char *)value,(char *)AEPSBALENQ.ucRequestId_BEreq);
        break;
    case  2:
        strcpy((char *)value,"517");//Method id
        break;
    case  3:
        strcpy((char *)value,(char *)configLoginLeg2.ucUsername_req);//teller id
        break;
    case  4:
        strcpy((char *)value,(char *)AEPSBALENQ.ucSessionId_BEreq);//session key
        break;
    case  5:
        strcpy((char *)value,"");//token id
        break;
    case  6:
        strcpy((char *)value,ISENCEYPT/*(char *)AEPSBALENQ.ucIsEncrypt_BEreq*/);//is encrypt
        break;
    case  7:
        strcpy((char *)value,"0"/*(char *)AEPSBALENQ.ucAuth_id_BEreq*/); //Auth Id
        break;
    case  8:
        strcpy((char *)value,(char *)AEPSBALENQ.ucAadharNo_BEreq); //BankBin+Aadhaar no
        break;
    case  9:
        strcpy((char *)value,(char *)AEPSBALENQ.ucmerMobileNo_BEreq);
        break;
    case  10:
        strcpy((char *)value,(char *)AEPSBALENQ.ucUID_BEreq);
        break;
    case  11:
        strcpy((char *)value,"AUTH"/*(char *)AEPSBALENQ.ucAuthType_BEreq*/);
        break;
    case  12:
        //strcpy((char *)value,(char *)RD_fun.ucPid);//check this one
        strcpy((char *)value,(char *)RD_Service_data.PID_DATA.toAscii().constData());
        break;
    case  13:
        strcpy((char *)value,(char *)RD_Service_data.hmac.toAscii().constData());
        break;
    case  14:
        strcpy((char *)value,(char *)EncPIDData);
        break;
    case  15:
        strcpy((char *)value,"Y"/*(char *)RD_fun.ucRC*/);
        break;
    case  16:
        strcpy((char *)value,"851204"/*(char *)AEPSBALENQ.ucMeta_lov_BEreq*/);
        break;
    case  17:
        strcpy((char *)value,"X"/*(char *)AEPSBALENQ.ucData_type_BEreq*/);
        break;
    case  18:
        strcpy((char *)value,"0000"/*(char *)AEPSBALENQ.ucamount_BEreq*/);
        break;
    case  19:
        strcpy((char *)value,"OBU60029078"/*(char *)AEPSBALENQ.uccard_acceptor_ID_BEreq*/);
        break;
    case  20:
        strcpy((char *)value,(char *)AEPSBALENQ.uccard_acceptor_namelocation_BEreq);
        break;
    case  21:
        strcpy((char *)value,"80000001"/*(char *)AEPSBALENQ.ucTerminal_Id_BEreq*/);
        break;
    case  22:
        strcpy((char *)value,""/*(char *)AEPSBALENQ.ucTransactionTime_BEreq*/);
        break;
    case  23:
        strcpy((char *)value,""/*(char *)AEPSBALENQ.ucTransactionDate_BEreq*/);
        break;
    case  24:
        strcpy((char *)value,(char *)AEPSBALENQ.ucAppId_BEreq);
        break;
    case  25:
        strcpy((char *)value,"Y"/*(char *)AEPSBALENQ.ucNewAPI_Flag_BEreq*/);
        break;
    case  26:
        strcpy((char *)value,"2.0"/*(char *)AEPSBALENQ.ucAPI_Version_BEreq*/);
        break;
    case  27:
        strcpy((char *)value,(char *)AEPSBALENQ.ucSessionExpiryTime);
        break;
    case  28:
        strcpy((char *)value,(char *)AEPSBALENQ.ucChannelID_BEreq);
        break;
    case  29:
        strcpy((char *)value,"18"/*(char *)AEPSBALENQ.ucServiceID_BEreq*/);
        break;
    default:
        return FUNC_ERROR;
    }
    return FUNC_OK;
}


int FPB::iParseAEPS_BalEnq(unsigned char *Data)
{
    int len,i=0,slen,len2=-1;
    //int j=0;
    char *RecvJsonData;
    char *CheckData;
    //char gSendData[1024];

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
                       "ResponseData:TerminalId:",//20
                       "MessageId:",
                       "SessionExpiryTime:",
                       "SessionId:",
                       "RouteID:",
                       "ErrorCode:",
                       "XMLData:",
                       "AuthmanFlag:",
                       "ServiceID:",//28
                      };

    int MandTag[]={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0};//1 for mandatory and  0 for non-mandatory by saurabh

    len=sizeof(LoginLeg1)/sizeof(LoginLeg1[0]);
    len2=sizeof(MandTag)/sizeof(MandTag[0]);
    for (i = 0; i < len; ++i)
    {
        RecvJsonData=strstr((char *)Data,LoginLeg1[i]);
        slen=strlen(LoginLeg1[i]);

        if (RecvJsonData==NULL && MandTag[i]==1)//If Tag not found and its mandatory.  //Test Later
        {
            //DrawString("TAG Missing",15,25,0);
            // ttestall(0,200);
            memset(gSendData,0,sizeof(gSendData));
            return FUNC_ERROR;
        }
        else if(RecvJsonData == NULL) //Else proceed to further parsing.
            continue;

        memset(gSendData,0,sizeof(gSendData));

        RecvJsonData=&RecvJsonData[slen];

        CheckData=strchr(RecvJsonData,'\n');
        //CheckData--;
        memset(gSendData,0,sizeof(gSendData));
        strncpy((char *)gSendData,RecvJsonData,CheckData-RecvJsonData);

        switch (i)
        {
        case 0:
            strcpy((char *)AEPSBALENQ.ucRequestId,(char *)gSendData);
            break;

        case 1:
            strcpy((char *)AEPSBALENQ.ucDisplayMessage,(char *)gSendData);
            break;

        case 2:
            strcpy((char *)AEPSBALENQ.ucResponseMessage,(char *)gSendData);
            break;

        case 3:
            strcpy((char *)AEPSBALENQ.ucResponseCode,(char *)gSendData);
            break;

        case 4:
            strcpy((char *)AEPSBALENQ.ucMessageType,(char *)gSendData);
            break;

        case 5:
            strcpy((char *)AEPSBALENQ.ucreturnCode,(char *)gSendData);
            break;

        case 6:
            strcpy((char *)AEPSBALENQ.ucAuthResponse,(char *)gSendData);
            break;

        case 7:
            strcpy((char *)AEPSBALENQ.ucAORreturnCode,(char *)gSendData);
            break;

        case 8:
            strcpy((char *)AEPSBALENQ.ucAORresponseMessage,(char *)gSendData);
            break;

        case 9:
            strcpy((char *)AEPSBALENQ.uctxnReferenceNo,(char *)gSendData);
            strcpy((char *)ConfigPrint.ucRRN,(char *)gSendData);
            break;

        case 10:
            strcpy((char *)AEPSBALENQ.ucAuthCode,(char *)gSendData);
            strcpy((char *)ConfigPrint.ucAUTHCode,(char *)gSendData);
            break;

        case 11:
            strcpy((char *)AEPSBALENQ.ucTransactionTime,(char *)gSendData);
            break;

        case 12:
            strcpy((char *)AEPSBALENQ.ucTransactionDate,(char *)gSendData);
            break;

        case 13:
            strcpy((char *)AEPSBALENQ.ucAuthUID,(char *)gSendData);
            break;

        case 14:
            strcpy((char *)AEPSBALENQ.ucTerminalId,(char *)gSendData);
            break;

        case 15:
            strcpy((char *)AEPSBALENQ.uccardAcceptorCode,(char *)gSendData);
            break;

        case 16:
            strcpy((char *)AEPSBALENQ.ucCardAcceptorNameLocation,(char *)gSendData);
            break;

        case 17:
            strcpy((char *)AEPSBALENQ.ucledgerBalance,(char *)gSendData);
            break;

        case 18:
            strcpy((char *)AEPSBALENQ.ucavailableBalance,(char *)gSendData);
            break;

        case 19:
            strcpy((char *)AEPSBALENQ.uccurrency,(char *)gSendData);
            break;

        case 20:
            strcpy((char *)AEPSBALENQ.ucBLTerminalId,(char *)gSendData);
            strcpy((char *)ConfigPrint.ucTerminalID,(char *)gSendData);
            break;

        case 21:
            strcpy((char *)AEPSBALENQ.ucMessageId,(char *)gSendData);
            break;

        case 22:
            strcpy((char *)AEPSBALENQ.ucSessionExpiryTime,(char *)gSendData);
            break;

        case 23:
            strcpy((char *)AEPSBALENQ.ucSessionId,(char *)gSendData);
            break;

        case 24:
            strcpy((char *)AEPSBALENQ.ucRouteID,(char *)gSendData);
            break;

        case 25:
            strcpy((char *)AEPSBALENQ.ucErrorCode,(char *)gSendData);
            break;

        case 26:
            strcpy((char *)AEPSBALENQ.ucXMLData,(char *)gSendData);
            break;

        case 27:
            strcpy((char *)AEPSBALENQ.ucAuthmanFlag,(char *)gSendData);
            break;

        case 28:
            strcpy((char *)AEPSBALENQ.ucServiceID,(char *)gSendData);
            break;

        default:
            break;
        }
    }
    memset(gSendData,0,sizeof(gSendData));
    return FUNC_OK;
}
