#include "impscommon.h"
#include "ui_FPB.h"
#include "json.h"
#include "Login.h"
#include "displaymessage.h"
#include "print.h"
#include <QSqlDatabase>
#include "userprofile.h"
#include<fstream>
#include "findcust.h"
#include "Reversal.h"
TxnStatement* MiniStatement;
extern Merchant_detail Merdetail;
extern Login_leg2 configLoginLeg2;
extern Login_leg1 configLoginLeg1;
extern char * Convertstr(string str);
TxnStatement MiniState;
int click = 0;
int TRANS_TYPE = -1;
extern Printfpb ConfigPrint;
extern FINDCUSTOMER FINDCUST;
Reversal Merotp;
extern char flag_bc_merchant_account;
extern int OTP_page;
extern int Resend_flag;
extern sCustDetails CustDet;

void FPB::on_Minstatement_clicked()
{
    Check_Conditions();
    QDate date = QDate::currentDate();
    QTime time = QTime::currentTime();
    QDateTime startDate(date, time);
    ui->listWidget->clear();
    ui->dateEdit_5->setDateTime(startDate);
    ui->dateEdit_6->setDateTime(startDate);
    ui->Reversal_conf_button->setVisible(true);
    int iRet =-2;
    TRANS_TYPE = MINISTATEMENT;
    ui->OTP_Lineedit->clear();
    ui->OTP_Lineedit->setFocus();
    if(flag_bc_merchant_account == 'B')
    {
        iRet = Agent_Auth();
        if(iRet == FUNC_OK)
        {
            OTP_page = 7;
            ui->stackedWidget->setCurrentIndex(22);
            return;

        }
        else if(iRet != FUNCTION_SUCCESS)
        {
            if(iRet == SESSION_EXPIRED_FAILED)
            {
                ui->stackedWidget->setCurrentIndex(0);
                return;
            }
            else
            {
                ui->stackedWidget->setCurrentIndex(32);
                return;
            }
        }
    }
    else
    {
        iRet = Mini_Statement(0);
        if(iRet != FUNCTION_SUCCESS)
        {

            if(iRet == SESSION_EXPIRED_FAILED)
                ui->stackedWidget->setCurrentIndex(0);
            else if(iRet == FUNC_BACK)
                ui->stackedWidget->setCurrentIndex(32);
            else
                ui->stackedWidget->setCurrentIndex(15);
            return;
        }
        else if(iRet == FUNCTION_SUCCESS)
        {
            ui->stackedWidget->setCurrentIndex(32); // TEMPORARY
            return ;
        }
    }
    return;
}


int FPB::Mini_Statement(int miniflag)
{
    CMisc miscObj;
    uidmessage::instance()->hide_Message();
    uidmessage::instance()->Display_BusyMessage("Please Wait...");
    QString todate,fromdate;
    QDate cal_todate,cal_fromdate;
    char ErrorMsg[100];
    QString Display;
    QString buff_tmp;
    char* jsondata;
    Json1::Value RequestedData,mainreq,SubData2;
    memset(&Merdetail,0, sizeof(Merdetail));
    QString Mini_Otp = ui->OTP_Lineedit->text();
    if(miniflag ==1)
    {
        todate = ui->dateEdit_5->date().toString("yyyy-MM-dd") ;
        fromdate  = ui->dateEdit_6->date().toString("yyyy-MM-dd") ;
        cal_todate = ui->dateEdit_5->date();
        cal_fromdate  = ui->dateEdit_6->date();

        int diff_days = getDifference(cal_todate, cal_fromdate);
#if DEBUG
        qDebug()<<"diff_days = "<<diff_days;
        qDebug()<<"todate"<<todate;
        qDebug()<<"fromdate"<<fromdate;
#endif
        if((diff_days > 7) || (diff_days < 0))
        {
            uidmessage::instance()->hide_Message();
            miscObj.DisplayMsgBox((char *)"Date difference shouldn't be more than 7 days");
            return FUNC_BACK;
        }
    }

    iGenerateRequestID((char *)username,(unsigned char *)&Merdetail.ucRequestID_req);
    if(flag_bc_merchant_account == 'B' && miniflag == 0)
    {
        mainreq["RequestId"] =  Merdetail.ucRequestID_req;
        mainreq["MethodId"]= 24;
        mainreq["TellerID"]= username;
        mainreq["SessionId"] = (char *)configLoginLeg2.ucSessionId;//buff_tmp.toAscii().data();
        mainreq["TokenId"]= "";
        mainreq["IsEncrypt"]= false;
        mainreq["X_Auth_Token"] ="";
        mainreq["SessionExpiryTime"]= (char *)configLoginLeg2.ucSessionExpiryTime;//"10/11/2018 11:58:44 PM";
        mainreq["ChannelID"]= CHANNEL_ID;
        mainreq["ProductCode"]= "";
        mainreq["Amount"]= "";



        RequestedData["MiniStatement"]["accountNo"]= (char *)CustDet.cFindCustAccountNo;
        RequestedData["MiniStatement"]["order"]= "DESC";


        RequestedData["CustomerAuth"]["MobileNo"] = (char *)FINDCUST.ucRDCLmobilePhoneNumber;
        RequestedData["CustomerAuth"]["AadharNo"] = RequestedData.null;
        RequestedData["CustomerAuth"]["Auth_id"] = 1;
        RequestedData["CustomerAuth"]["Fp_Auth"] = RequestedData.null;
        RequestedData["CustomerAuth"]["Aadhaar"] = "";



        RequestedData["CustomerAuth"]["Otp"]["RequestId"] = Merdetail.ucRequestID_req;
        RequestedData["CustomerAuth"]["Otp"]["MethodId"] = 2;
        RequestedData["CustomerAuth"]["Otp"]["TellerID"] = username;
        RequestedData["CustomerAuth"]["Otp"]["SessionId"] =(char *)configLoginLeg2.ucSessionId;
        RequestedData["CustomerAuth"]["Otp"]["TokenId"] = RequestedData.null;  // RequestedData.null; // Hardcoded 24052019 removed
        RequestedData["CustomerAuth"]["Otp"]["IsEncrypt"] = false;
        RequestedData["CustomerAuth"]["Otp"]["X_Auth_Token"] ="";



        RequestedData["CustomerAuth"]["Otp"]["SessionExpiryTime"] = (char *)configLoginLeg2.ucSessionExpiryTime;//"10/11/201811: 58: 44PM";
        SubData2["MethodId"] ="2";
        SubData2["RequestId"] =(char *) Merotp.Revotp_requestid;
        SubData2["CustomerMobileNo"] = (char *)FINDCUST.ucRDCLmobilePhoneNumber;
        //  SubData2["CustomerMobileNo"] = (char *)configLoginLeg2.ucmobileNumber;
        SubData2["OtpPin"] = (char *)Mini_Otp.toAscii().data();//"";
        SubData2["MessageId"] = -1;
        SubData2["OtpParam"] = "";

        buff_tmp.clear();
        buff_tmp = serialize1(Convertstr(SubData2.toStyledString()));
        RequestedData["CustomerAuth"]["Otp"]["RequestData"] = buff_tmp.toAscii().data();
        buff_tmp.clear();
        buff_tmp = serialize1(Convertstr1(RequestedData.toStyledString()));
        mainreq["RequestData"] = buff_tmp.toAscii().data();

        jsondata = Convertstr1(mainreq.toStyledString());
    }
    else
    {
        mainreq["RequestId"] =  Merdetail.ucRequestID_req;
        if(flag_bc_merchant_account == 'B')
        {
            mainreq["MethodId"] = 91;
        }
        else
        {
            mainreq["MethodId"] = 86;
        }
        mainreq["ChannelID"] = CHANNEL_ID;
        mainreq["TellerID"]= username;
        mainreq["SessionId"] = (char *)configLoginLeg2.ucSessionId;//buff_tmp.toAscii().data();
        mainreq["TokenId"]= "";
        mainreq["IsEncrypt"]= false;
        mainreq["SessionExpiryTime"]= (char *)configLoginLeg2.ucSessionExpiryTime;
        mainreq["X_Auth_Token"] ="";

        mainreq["ProductCode"] =mainreq.null;
        mainreq["Amount"]= mainreq.null;
        if(miniflag==1)
        {
            RequestedData["fromDate"] = todate.toAscii().data();
            RequestedData["toDate"] = fromdate.toAscii().data();

        }
        else
        {
            RequestedData["fromDate"] = "";
            RequestedData["toDate"] = "";
        }
        if(flag_bc_merchant_account == 'B')
        {
            RequestedData["accountNo"] = CustDet.cFindCustAccountNo;
        }
        else
        {
            RequestedData["mobileNumber"] = FC_MobileNumber;
        }
        RequestedData["toAmount"] = "";
        RequestedData["sProduct"] = "";
        RequestedData["tranType"] = "";
        RequestedData["referenceNo"] = "";
        RequestedData["order"] = "DESC";

        buff_tmp = serialize1(Convertstr(RequestedData.toStyledString()));

        mainreq["RequestData"] = buff_tmp.toAscii().data();

        jsondata = Convertstr(mainreq.toStyledString());
    }
#if DEBUG
    qDebug()<<"------------->"<<jsondata;
#endif
    //    //-------------------------------------------------------------
    memset(gSendData,0,sizeof(gSendData));
    strncpy((char*)gSendData, jsondata, strlen(jsondata));
#if DEBUG
    printf("gSendData = %s\n\n",gSendData);
#endif
    //-------------------------------------------------------------

#ifdef UAT_HTTP     // File Download

    uidmessage::instance()->hide_Message();
    uidmessage::instance()->Display_BusyMessage("Connecting server");
    sleep(1);
    uidmessage::instance()->hide_Message();
    hComm.close_connection();
    res = hComm.open_connect(SERVERIP,SERVERPORT);
    if(res != 0)
    {
        qDebug() << "Failed to connect to the Server";
        uidmessage::instance()->hide_Message();
        uidmessage::instance()->DisplayMessage("Server Not Connected");
        qDebug()<<"FUNCTION_FAILED";
        return FUNCTION_FAILED ;
    }


    sleep(1);
    uidmessage::instance()->hide_Message();
    uidmessage::instance()->Display_BusyMessage("Transmitting ..");


    int len_gSendData = strlen((char *)gSendData);
    memset(oBuff,0,sizeof(oBuff));
    if((hComm.host_2Sendrecv((unsigned char *)gSendData,len_gSendData,( unsigned char*)oBuff,(int *)&oLen,(int)response_timeout,0))!=SUCCESS)
    {
        printf("No Response From Server\n");
        hComm.close_connection();
        miscObj.DisplayMsgBox((char *)"Response Time-Out");
        sleep(2);
        qDebug()<<"***   Response Time-Out   ***";
        uidmessage::instance()->hide_Message();
        //return SOCKET_RESPONSE_FAILED;
        return FUNCTION_FAILED;
    }

    hComm.close_connection();

#else     // File Download full condition
#if DEBUG
    printf("gSendData = %s\n\n",gSendData);
#endif
    memset(str_url,0,sizeof(str_url));
#ifdef PRODUCTION
    strcpy(str_url,URL_CREATION(LOGIN_URL_PROD));
#else
    strcpy(str_url,URL_CREATION((char *)COMMON_URL));
#endif

    int retk = file_download(str_url, (char *)gSendData,"/usr/FINO/FPB/RESP/OUTPUT.TXT",120);
    if(retk!=0)
    {
        sleep(1);
        uidmessage::instance()->hide_Message();
        return FUNCTION_FAILED;
    }

#endif


#if DEBUG
    printf("\noBuff = %s\n",oBuff);
#endif
    system("mkdir /usr/FINO/FPB/RESP/");
    system("cd /usr/FINO/FPB/RESP/");
    system("rm /usr/FINO/FPB/RESP/MINI_RESP.txt");
    int gRecv_len = strlen((char *)oBuff);
    FILE *fp2 = NULL;
    fp2 = fopen("/usr/FINO/FPB/RESP/MINI_RESP.TXT","w");
    if(fp2 == NULL)
    {
        return FUNCTION_FAILED;
    }
    fwrite( oBuff, 1, gRecv_len, fp2 );
    fclose(fp2);
    int iRetparse = Ministatement_Parse(miniflag);

    system("cd /usr/FINO/FPB/RESP/");
    system("rm /usr/FINO/FPB/RESP/OUTPUT.TXT");
    system("rm /usr/FINO/FPB/RESP/MER_RESP.TXT");
    if(iRetparse==FUNC_OK)
    {
        int j,i,l;
        QSqlQuery query;

        MiniStatement =(TxnStatement*) malloc(sizeof(TxnStatement)*Merdetail.icount);

        query.prepare("SELECT TRANSACTION_DATE,TIME,TRANSACTION_AMOUNT,TRANSACTION_TYPE_DESCRIPTION,TRANSACTION_TYPE_PRODUCT,REFERENCE_ID,TRANSACTION_TYPE,DEBIT_CREDIT,CHARGE_AMOUNT,STATUS_DESC,CGST,IGST,UGST,SGST,RFUMAPPING,RFU_TEXT11_DESC, RFU_TEXT11_VAL,RFU_TEXT2_DESC,RFU_TEXT2_VAL,RFU_TEXT3_DESC,RFU_TEXT3_VAL,RFU_TEXT4_DESC,RFU_TEXT4_VAL,RFU_TEXT5_DESC,RFU_TEXT5_VAL,RFU_TEXT6_DESC,RFU_TEXT6_VAL,RFU_TEXT7_DESC,RFU_TEXT7_VAL,RFU_TEXT8_DESC,RFU_TEXT8_VAL,RFU_TEXT9_DESC,RFU_TEXT9_VAL,RFU_TEXT10_DESC,RFU_TEXT10_VAL,RFU_NUMERIC1_DESC ,RFU_NUMERIC1_VAL ,RFU_NUMERIC2_DESC,RFU_NUMERIC2_VAL,RFU_NUMERIC3_DESC ,RFU_NUMERIC3_VAL,RFU_NUMERIC4_DESC,RFU_NUMERIC4_VAL,ENDING_BALANCE FROM MINISTATEMENT_TXN  ");
        if(!query.exec())
            qDebug() << query.lastError();
        else
        {
            i=0;
            while(query.next())
            {
                strcpy(MiniStatement[i].transactionDate, query.value(0).toString().trimmed().toAscii().data());
                strcpy(MiniStatement[i].time, query.value(1).toString().trimmed().toAscii().data());
                strcpy(MiniStatement[i].transactionAmount, query.value(2).toString().trimmed().toAscii().data());
                strcpy(MiniStatement[i].TransactionTypeDescription, query.value(3).toString().trimmed().toAscii().data());
                strcpy(MiniStatement[i].TransactionTypeProduct, query.value(4).toString().trimmed().toAscii().data());
                strcpy(MiniStatement[i].referenceId, query.value(5).toString().trimmed().toAscii().data());
                strcpy(MiniStatement[i].transactionType, query.value(6).toString().trimmed().toAscii().data());
                strcpy(MiniStatement[i].debitCredit, query.value(7).toString().trimmed().toAscii().data());
                strcpy(MiniStatement[i].chargeAmount, query.value(8).toString().trimmed().toAscii().data());
                strcpy(MiniStatement[i].StatusDesc, query.value(9).toString().trimmed().toAscii().data());
                strcpy(MiniStatement[i].cGST, query.value(10).toString().trimmed().toAscii().data());
                strcpy(MiniStatement[i].iGST, query.value(11).toString().trimmed().toAscii().data());
                strcpy(MiniStatement[i].uGST, query.value(12).toString().trimmed().toAscii().data());
                strcpy(MiniStatement[i].sGST, query.value(13).toString().trimmed().toAscii().data());
                strcpy(MiniStatement[i].RFUMapping, query.value(14).toString().trimmed().toAscii().data());
                strcpy(MiniStatement[i].rfuText1Desc, query.value(15).toString().trimmed().toAscii().data());
                strcpy(MiniStatement[i].rfuText1Val, query.value(16).toString().trimmed().toAscii().data());
                strcpy(MiniStatement[i].rfuText2Desc, query.value(17).toString().trimmed().toAscii().data());
                strcpy(MiniStatement[i].rfuText2Val, query.value(18).toString().trimmed().toAscii().data());
                strcpy(MiniStatement[i].rfuText3Desc, query.value(19).toString().trimmed().toAscii().data());
                strcpy(MiniStatement[i].rfuText3Val, query.value(20).toString().trimmed().toAscii().data());
                strcpy(MiniStatement[i].rfuText4Desc, query.value(21).toString().trimmed().toAscii().data());
                strcpy(MiniStatement[i].rfuText4Val, query.value(22).toString().trimmed().toAscii().data());
                strcpy(MiniStatement[i].rfuText5Desc, query.value(23).toString().trimmed().toAscii().data());
                strcpy(MiniStatement[i].rfuText5Val, query.value(24).toString().trimmed().toAscii().data());
                strcpy(MiniStatement[i].rfuText6Desc, query.value(25).toString().trimmed().toAscii().data());
                strcpy(MiniStatement[i].rfuText6Val, query.value(26).toString().trimmed().toAscii().data());
                strcpy(MiniStatement[i].rfuText7Desc, query.value(27).toString().trimmed().toAscii().data());
                strcpy(MiniStatement[i].rfuText7Val, query.value(28).toString().trimmed().toAscii().data());
                strcpy(MiniStatement[i].rfuText8Desc, query.value(29).toString().trimmed().toAscii().data());
                strcpy(MiniStatement[i].rfuText8Val, query.value(30).toString().trimmed().toAscii().data());
                strcpy(MiniStatement[i].rfuText9Desc, query.value(31).toString().trimmed().toAscii().data());
                strcpy(MiniStatement[i].rfuText9Val, query.value(32).toString().trimmed().toAscii().data());
                strcpy(MiniStatement[i].rfuText10Desc, query.value(33).toString().trimmed().toAscii().data());
                strcpy(MiniStatement[i].rfuText10Val, query.value(34).toString().trimmed().toAscii().data());
                strcpy(MiniStatement[i].rfuNumeric1Desc, query.value(35).toString().trimmed().toAscii().data());
                strcpy(MiniStatement[i].rfuNumeric1Val, query.value(36).toString().trimmed().toAscii().data());
                strcpy(MiniStatement[i].rfuNumeric2Desc, query.value(37).toString().trimmed().toAscii().data());
                strcpy(MiniStatement[i].rfuNumeric2Val, query.value(38).toString().trimmed().toAscii().data());
                strcpy(MiniStatement[i].rfuNumeric3Desc, query.value(39).toString().trimmed().toAscii().data());
                strcpy(MiniStatement[i].rfuNumeric3Val, query.value(40).toString().trimmed().toAscii().data());
                strcpy(MiniStatement[i].rfuNumeric4Desc, query.value(41).toString().trimmed().toAscii().data());
                strcpy(MiniStatement[i].rfuNumeric4Val, query.value(42).toString().trimmed().toAscii().data());
                strcpy(MiniStatement[i].endingbalance, query.value(43).toString().trimmed().toAscii().data());
                i++;
            }
        }
        ui->listWidget->clear();
        for(i=0;i<Merdetail.icount;i++)
        {
            char disp[70];
            Display.clear();
            Display = QString::fromUtf8(MiniStatement[i].transactionDate) + " "  + QString::fromUtf8(MiniStatement[i].time);

            if(strcmp(MiniStatement[i].debitCredit,"Cr")==0)
            {
                char temp[20];
#if DEBUG
                qDebug()<<"QString::fromUtf8(MiniStatement[i].transactionType).trimmed()"<<QString::fromUtf8(MiniStatement[i].transactionType).trimmed();
#endif
                if(QString::fromUtf8(MiniStatement[i].transactionType).trimmed().contains("BENV"))
                {
                    float Amount = atof(MiniStatement[i].transactionAmount) + atof(MiniStatement[i].chargeAmount);
                    QString Amt = QString::number(Amount).append(".00");
#if DEBUG
                    qDebug()<<"Amount============="<<Amount;
                    qDebug()<<"Amt============="<<Amt;
#endif
                    memset(MiniStatement[i].transactionAmount,0,sizeof(MiniStatement[i].transactionAmount));
                    sprintf(MiniStatement[i].transactionAmount,"  + %s",Amt.toAscii().data());

                    printf("MiniStatement[i].transactionAmount%s",MiniStatement[i].transactionAmount);

                }
                else
                {
                    memset(temp,0,sizeof(temp));
                    strcpy(temp,MiniStatement[i].transactionAmount);
                    memset(MiniStatement[i].transactionAmount,0,sizeof(MiniStatement[i].transactionAmount));
                    sprintf(MiniStatement[i].transactionAmount,"  +%s",temp);

                }

            }
            else if(strcmp(MiniStatement[i].debitCredit,"Dr")==0)
            {
                char temp[20];
                if(QString::fromUtf8(MiniStatement[i].transactionType).trimmed().contains("BENV"))
                {
                    float Amount = atof(MiniStatement[i].transactionAmount) + atof(MiniStatement[i].chargeAmount);
                    QString Amt = QString::number(Amount).append(".00");


                    memset(MiniStatement[i].transactionAmount,0,sizeof(MiniStatement[i].transactionAmount));
                    sprintf(MiniStatement[i].transactionAmount,"  -%s",Amt.toAscii().data());
#if DEBUG
                    qDebug()<<"Amt============="<<Amount;
                    qDebug()<<"Amt============="<<Amt;
                    printf("MiniStatement[i].transactionAmount%s",MiniStatement[i].transactionAmount);
#endif

                }
                else
                {
                    memset(temp,0,sizeof(temp));
                    strcpy(temp,MiniStatement[i].transactionAmount);
                    memset(MiniStatement[i].transactionAmount,0,sizeof(MiniStatement[i].transactionAmount));
                    sprintf(MiniStatement[i].transactionAmount,"  -%s",temp);
                }

            }
            else
            {
                if(QString::fromUtf8(MiniStatement[i].transactionAmount).isEmpty() || strcmp(MiniStatement[i].debitCredit,"null")==0 )
                {
                    memset(MiniStatement[i].transactionAmount,0,sizeof(MiniStatement[i].transactionAmount));
                    strcpy(MiniStatement[i].transactionAmount ,"  N/A");
                }

            }

            if( !QString::fromUtf8(MiniStatement[i].endingbalance).isEmpty() && !strcmp(MiniStatement[i].endingbalance,"null")==0 && flag_bc_merchant_account == 'B')
            {
                Display = Display + " " + QString::fromUtf8(MiniStatement[i].transactionAmount) + " " + QString::fromUtf8(MiniStatement[i].endingbalance) +"\n" ;

            }
            else
            {
                Display = Display + " " + QString::fromUtf8(MiniStatement[i].transactionAmount) + "\n" ;
            }

            if((MiniStatement[i].StatusDesc,"null")==0)
            {
                memset(MiniStatement[i].StatusDesc,0,sizeof(MiniStatement[i].StatusDesc));
                strcpy(MiniStatement[i].StatusDesc ,"");

            }
            Display = Display + QString::fromUtf8(MiniStatement[i].TransactionTypeDescription) +" " + QString::fromUtf8(MiniStatement[i].StatusDesc)+ "\n" ;

            if( !QString::fromUtf8(MiniStatement[i].RFUMapping).isEmpty() && !strcmp(MiniStatement[i].RFUMapping,"null")==0)
            {
                sprintf(disp,"%-30s(i)",MiniStatement[i].referenceId);
                Display = Display + QString::fromUtf8(disp)+ "\n";

            }
            else
            {
                Display = Display + QString::fromUtf8(MiniStatement[i].referenceId) + "\n";

            }

            if(QString::fromUtf8(MiniStatement[i].transactionType).trimmed().contains("CHG") || QString::fromUtf8(MiniStatement[i].transactionType).trimmed().contains("CHRG"))
            {
                char temp[20];
                if( !QString::fromUtf8(MiniStatement[i].sGST).isEmpty() && !strcmp(MiniStatement[i].sGST,"null")==0 && !strcmp(MiniStatement[i].sGST,"0")==0 && !strcmp(MiniStatement[i].sGST,"0.0")==0)
                {
                    if(!QString::fromUtf8(MiniStatement[i].sGST).contains("0."))
                    {
                        memset(temp,0,sizeof(temp));
                        strcpy(temp,MiniStatement[i].cGST);
                        memset(MiniStatement[i].cGST,0,sizeof(MiniStatement[i].cGST));
                        sprintf(MiniStatement[i].cGST,"0%s",temp);
                    }

                }
                else
                {
                    strcpy(MiniStatement[i].sGST,"0.0");

                }

                if( !QString::fromUtf8(MiniStatement[i].cGST).isEmpty() && !strcmp(MiniStatement[i].cGST,"null")==0 && !strcmp(MiniStatement[i].cGST,"0")==0 && !strcmp(MiniStatement[i].cGST,"0.0")==0)
                {
                    if(!QString::fromUtf8(MiniStatement[i].cGST).contains("0."))
                    {
                        memset(temp,0,sizeof(temp));
                        strcpy(temp,MiniStatement[i].cGST);
                        memset(MiniStatement[i].cGST,0,sizeof(MiniStatement[i].cGST));
                        sprintf(MiniStatement[i].cGST,"0%s",temp);
                    }

                }
                else
                {
                    strcpy(MiniStatement[i].cGST,"0.0");

                }

                if( !QString::fromUtf8(MiniStatement[i].iGST).isEmpty() && !strcmp(MiniStatement[i].iGST,"null")==0 && !strcmp(MiniStatement[i].iGST,"0")==0 && !strcmp(MiniStatement[i].iGST,"0.0")==0)
                {
                    if(!QString::fromUtf8(MiniStatement[i].iGST).contains("0."))
                    {
                        memset(temp,0,sizeof(temp));
                        strcpy(temp,MiniStatement[i].iGST);
                        memset(MiniStatement[i].iGST,0,sizeof(MiniStatement[i].iGST));
                        sprintf(MiniStatement[i].iGST,"0%s",temp);
                    }

                }
                else
                {
                    strcpy(MiniStatement[i].iGST,"0.0");
                }



                if( !QString::fromUtf8(MiniStatement[i].chargeAmount).isEmpty() && !strcmp(MiniStatement[i].chargeAmount,"null")==0 && !strcmp(MiniStatement[i].chargeAmount,"0")==0 && !strcmp(MiniStatement[i].chargeAmount,"0.0")==0)
                {
                    if(strcmp(MiniStatement[i].iGST,"0.0")==0)
                    {
                        memset(temp,0,sizeof(temp));
                        strcpy(temp,MiniStatement[i].chargeAmount);

                        float Amount = atof(temp) - atof(MiniStatement[i].cGST) + atof(MiniStatement[i].sGST);
                        QString Amt = QString::number(Amount);

                        memset(MiniStatement[i].chargeAmount,0,sizeof(MiniStatement[i].chargeAmount));
                        sprintf(MiniStatement[i].chargeAmount,"CHARGES: %s  CGST:%s SGST:%s",Amt.toAscii().data(),MiniStatement[i].cGST,MiniStatement[i].sGST);
#if DEBUG
                        qDebug()<<"Amount inside "<<Amount;
                        qDebug()<<"Amount inside Amt "<<Amt;
                        printf("MiniStatement[i].chargeAmount%s",MiniStatement[i].chargeAmount);
#endif
                    }
                    else
                    {
                        memset(temp,0,sizeof(temp));
                        strcpy(temp,MiniStatement[i].chargeAmount);
                        float Amount = atof(temp) - atof(MiniStatement[i].iGST);
                        QString Amt = QString::number(Amount);
                        memset(MiniStatement[i].chargeAmount,0,sizeof(MiniStatement[i].chargeAmount));
                        sprintf(MiniStatement[i].chargeAmount,"CHARGES: %s  IGST:0%s",Amt.toAscii().data(),MiniStatement[i].iGST);
                        printf("MiniStatement[i].chargeAmount%s",MiniStatement[i].chargeAmount);


                    }

                }
                if(!strcmp(MiniStatement[i].sGST,"0.0")==0 && !strcmp(MiniStatement[i].cGST,"0.0")==0 && strcmp(MiniStatement[i].iGST,"0.0")==0)
                {
                    Display = Display + QString::fromUtf8(MiniStatement[i].chargeAmount) + "\n" ;
                }
            }
            Display = Display + "_____________________________________";

            ui->listWidget->addItem(Display);
        }
        uidmessage::instance()->hide_Message();
        return FUNCTION_SUCCESS;

    }
    else
    {
        if(strcmp((char *)Merdetail.ucResponseMessage,"Session Expired.")==0)
        {
            uidmessage::instance()->hide_Message();
            sprintf(ErrorMsg,"%s",(char *)Merdetail.ucResponseMessage);
            miscObj.DisplayMsgBox(ErrorMsg);
            uidmessage::instance()->hide_Message();
            return SESSION_EXPIRED_FAILED;
        }
        if((strcmp((char *)Merdetail.ucResponseMessage,"OTP expired")==0) || (strcmp((char *)Merdetail.ucResponseMessage,"OTP Verification failed")==0) || (strcmp((char *)Merdetail.ucResponseMessage,"OTP Authentication Failed")==0)) // OTP already Consumed
        {
            uidmessage::instance()->hide_Message();
            memset(ErrorMsg, 0, sizeof(ErrorMsg));
            sprintf(ErrorMsg,"%s", (char *)Merdetail.ucResponseMessage);
            miscObj.DisplayMsgBox(ErrorMsg);
            uidmessage::instance()->hide_Message();
            ui->OTP_Lineedit->clear();
            ui->OTP_Lineedit->setFocus();

            return OTP_FAILED;
        }
        sprintf(ErrorMsg,"%s",/*(char *)OTP.ucResponseCode,*/(char *)Merdetail.ucResponseMessage);
        miscObj.DisplayMsgBox(ErrorMsg);
        uidmessage::instance()->hide_Message();
        return FUNCTION_FAILED;
    }
    return FUNCTION_SUCCESS;
}

int FPB::Ministatement_Parse(int miniflag)
{
    system("sed 's/\"ResponseData\":\"{/\"ResponseData\":{/g' /usr/FINO/FPB/RESP/MINI_RESP.TXT > /tmp/1.txt");
    system("sed 's/}\",\"MessageId\"/},\"MessageId\"/g' /tmp/1.txt > /tmp/2.txt  ");
    system(" sed 's/\\\\//g' /tmp/2.txt > /usr/FINO/FPB/RESP/MINI_RESP.TXT");

    system("cd /tmp/");
    system("rm /tmp/1.txt");
    system("rm /tmp/2.txt");

    QSqlQuery query;

    query.clear();
    query.exec("delete from MINISTATEMENT_TXN");
    query.clear();

    Json1::Value root;
    root.clear();
    Json1::Reader reader;
    std::ifstream ReadData("/usr/FINO/FPB/RESP/MINI_RESP.TXT");
    bool parsedSuccess = reader.parse(ReadData,root,false);

    ReadData.close();

    if(not parsedSuccess)
    {

        qDebug()<<"FpsMonthlyPdsData File not Found";
        return FUNCAL_FAILED;
    }
    else
    {
        memset(&Merdetail,0,sizeof(Merdetail));
#if DEBUG
        qDebug()<<"RequestId   : "<<Convertstr(root["RequestId"].asString());
        qDebug()<<"DisplayMessage : "<<Convertstr(root["DisplayMessage"].asString());
        qDebug()<<"ResponseMessage    : "<<Convertstr(root["ResponseMessage"].asString());
        qDebug()<<"ResponseCode : "<<Convertstr(root["ResponseCode"].asString());
        qDebug()<<"MessageType : "<<Convertstr(root["MessageType"].asString());
        qDebug()<<"RespocountNo : "<<Convertstr(root["RespocountNo"].asString());
#endif
        strcpy(Merdetail.ucRequestId,Convertstr(root["RequestId"].asString()));
        strcpy(Merdetail.ucDisplayMessage,Convertstr(root["DisplayMessage"].asString()));
        strcpy(Merdetail.ucResponseMessage,Convertstr(root["ResponseMessage"].asString()));
        strcpy(Merdetail.ucResponseCode,Convertstr(root["ResponseCode"].asString()));
        if(strcmp(Merdetail.ucResponseCode,"0")==0)
        {
            Json1::Value ResponseData;
            ResponseData.clear();
            ResponseData = root.get("ResponseData",0);
            qDebug()<<"Size of the ResponseData"<<ResponseData.size();
            printf("\nSize : %d\n",ResponseData.size());
            if(ResponseData.size()==0)
            {
                qDebug("\n1:Key Register Data Not Found\n");
                return FUNCAL_FAILED;
            }
            else
            {

                Json1::Value walkinTransactionsList;
                walkinTransactionsList.clear();
                Merdetail.icount =0;
                if(flag_bc_merchant_account == 'B' && miniflag == 1)
                {
                    walkinTransactionsList = ResponseData.get("acctTransaction",0);
                }
                else if(flag_bc_merchant_account == 'B')
                {
                    walkinTransactionsList = ResponseData.get("transactionList",0);
                }
                else
                {
                    walkinTransactionsList = ResponseData.get("walkinTransactionsList",0);

                }

                Merdetail.icount = walkinTransactionsList.size();
#if DEBUG
                qDebug()<<"Merdetail.icount"<<Merdetail.icount;
#endif
                for(int i=0;i<walkinTransactionsList.size();i++)
                {




                    strcpy(MiniState.sequenceNumber,Convertstr(walkinTransactionsList[i]["sequenceNumber"].asString()));

                    strcpy(MiniState.transactionAmount,Convertstr(walkinTransactionsList[i]["transactionAmount"].asString()));

                    strcpy(MiniState.transactionDesc,Convertstr(walkinTransactionsList[i]["transactionDesc"].asString()));

                    strcpy(MiniState.debitCredit,Convertstr(walkinTransactionsList[i]["debitCredit"].asString()));

                    strcpy(MiniState.transactionDate,Convertstr(walkinTransactionsList[i]["transactionDate"].asString()));


                    strcpy(MiniState.transactionType,Convertstr(walkinTransactionsList[i]["transactionType"].asString()));

                    strcpy(MiniState.transactionTypeName,Convertstr(walkinTransactionsList[i]["transactionTypeName"].asString()));

                    strcpy(MiniState.TransactionTypeProduct,Convertstr(walkinTransactionsList[i]["TransactionTypeProduct"].asString()));

                    strcpy(MiniState.TransactionTypeDescription,Convertstr(walkinTransactionsList[i]["TransactionTypeDescription"].asString()));

                    strcpy(MiniState.RFUMapping,Convertstr(walkinTransactionsList[i]["RFUMapping"].asString()));

                    strcpy(MiniState.time,Convertstr(walkinTransactionsList[i]["time"].asString()));

                    strcpy(MiniState.originalTxnSequence,Convertstr(walkinTransactionsList[i]["originalTxnSequence"].asString()));

                    strcpy(MiniState.referenceId,Convertstr(walkinTransactionsList[i]["referenceId"].asString()));

                    strcpy(MiniState.reversalFlag,Convertstr(walkinTransactionsList[i]["reversalFlag"].asString()));

                    strcpy(MiniState.debitAmount,Convertstr(walkinTransactionsList[i]["debitAmount"].asString()));

                    strcpy(MiniState.creditAmount,Convertstr(walkinTransactionsList[i]["creditAmount"].asString()));
                    strcpy(MiniState.status,Convertstr(walkinTransactionsList[i]["status"].asString()));


                    strcpy(MiniState.StatusDesc,Convertstr(walkinTransactionsList[i]["statusDesc"].asString()));


                    strcpy(MiniState.rfuNumeric1Desc,Convertstr(walkinTransactionsList[i]["rfuNumeric1Desc"].asString()));
                    strcpy(MiniState.rfuNumeric1Val,Convertstr(walkinTransactionsList[i]["rfuNumeric1Val"].asString()));
                    strcpy(MiniState.rfuNumeric2Desc,Convertstr(walkinTransactionsList[i]["rfuNumeric2Desc"].asString()));
                    strcpy(MiniState.rfuNumeric2Val,Convertstr(walkinTransactionsList[i]["rfuNumeric2Val"].asString()));
                    strcpy(MiniState.rfuNumeric3Desc,Convertstr(walkinTransactionsList[i]["rfuNumeric3Desc"].asString()));
                    strcpy(MiniState.rfuNumeric3Val,Convertstr(walkinTransactionsList[i]["rfuNumeric3Val"].asString()));
                    strcpy(MiniState.rfuNumeric4Desc,Convertstr(walkinTransactionsList[i]["rfuNumeric4Desc"].asString()));
                    strcpy(MiniState.rfuNumeric4Val,Convertstr(walkinTransactionsList[i]["rfuNumeric4Val"].asString()));


                    strcpy(MiniState.rfuText1Desc,Convertstr(walkinTransactionsList[i]["rfuText1Desc"].asString()));
                    strcpy(MiniState.rfuText1Val,Convertstr(walkinTransactionsList[i]["rfuText1Val"].asString()));
                    strcpy(MiniState.rfuText2Desc,Convertstr(walkinTransactionsList[i]["rfuText2Desc"].asString()));
                    strcpy(MiniState.rfuText2Val,Convertstr(walkinTransactionsList[i]["rfuText2Val"].asString()));
                    strcpy(MiniState.rfuText3Desc,Convertstr(walkinTransactionsList[i]["rfuText3Desc"].asString()));
                    strcpy(MiniState.rfuText3Val,Convertstr(walkinTransactionsList[i]["rfuText3Val"].asString()));
                    strcpy(MiniState.rfuText4Desc,Convertstr(walkinTransactionsList[i]["rfuText4Desc"].asString()));
                    strcpy(MiniState.rfuText4Val,Convertstr(walkinTransactionsList[i]["rfuText4Val"].asString()));
                    strcpy(MiniState.rfuText5Desc,Convertstr(walkinTransactionsList[i]["rfuText5Desc"].asString()));
                    strcpy(MiniState.rfuText5Val,Convertstr(walkinTransactionsList[i]["rfuText5Val"].asString()));
                    strcpy(MiniState.rfuText6Desc,Convertstr(walkinTransactionsList[i]["rfuText6Desc"].asString()));
                    strcpy(MiniState.rfuText6Val,Convertstr(walkinTransactionsList[i]["rfuText6Val"].asString()));
                    strcpy(MiniState.rfuText7Desc,Convertstr(walkinTransactionsList[i]["rfuText7Desc"].asString()));
                    strcpy(MiniState.rfuText7Val,Convertstr(walkinTransactionsList[i]["rfuText7Val"].asString()));
                    strcpy(MiniState.rfuText8Desc,Convertstr(walkinTransactionsList[i]["rfuText8Desc"].asString()));
                    strcpy(MiniState.rfuText8Val,Convertstr(walkinTransactionsList[i]["rfuText8Val"].asString()));
                    strcpy(MiniState.rfuText9Desc,Convertstr(walkinTransactionsList[i]["rfuText9Desc"].asString()));
                    strcpy(MiniState.rfuText9Val,Convertstr(walkinTransactionsList[i]["rfuText9Val"].asString()));
                    strcpy(MiniState.rfuText10Desc,Convertstr(walkinTransactionsList[i]["rfuText10Desc"].asString()));
                    strcpy(MiniState.rfuText10Val,Convertstr(walkinTransactionsList[i]["rfuText10Val"].asString()));


                    strcpy(MiniState.parentTxnId,Convertstr(walkinTransactionsList[i]["parentTxnAmount"].asString()));
                    strcpy(MiniState.parentTxnAmount,Convertstr(walkinTransactionsList[i]["parentTxnAmount"].asString()));
                    strcpy(MiniState.parentTxnAmountCurrent,Convertstr(walkinTransactionsList[i]["parentTxnAmountCurrent"].asString()));
                    strcpy(MiniState.chargeAmount,Convertstr(walkinTransactionsList[i]["chargeAmount"].asString()));
                    strcpy(MiniState.chargeAmountCurrent,Convertstr(walkinTransactionsList[i]["chargeAmountCurrent"].asString()));

                    strcpy(MiniState.cGST,Convertstr(walkinTransactionsList[i]["cGST"].asString()));
                    strcpy(MiniState.sGST,Convertstr(walkinTransactionsList[i]["sGST"].asString()));
                    strcpy(MiniState.iGST,Convertstr(walkinTransactionsList[i]["iGST"].asString()));
                    strcpy(MiniState.uGST,Convertstr(walkinTransactionsList[i]["uGST"].asString()));
                    strcpy(MiniState.subType,Convertstr(walkinTransactionsList[i]["subType"].asString()));
                    strcpy(MiniState.endingbalance,Convertstr(walkinTransactionsList[i]["endingBalance"].asString()));

#if DEBUG
                    qDebug()<<"MiniState.sequenceNumber"<<MiniState.sequenceNumber;
                    qDebug()<<"MiniState.transactionAmount"<<MiniState.transactionAmount;
                    qDebug()<<"MiniState.transactionDesc"<<MiniState.transactionDesc;
                    qDebug()<<"MiniState.debitCredit"<<MiniState.debitCredit;
                    qDebug()<<"MiniState.transactionDate"<<MiniState.transactionDate;
                    qDebug()<<"MiniState.transactionType"<<MiniState.transactionType;
                    qDebug()<<"MiniState.transactionTypeName"<<MiniState.transactionTypeName;
                    qDebug()<<"MiniState.TransactionTypeProduct"<<MiniState.TransactionTypeProduct;
                    qDebug()<<"MiniState.TransactionTypeDescription"<<MiniState.TransactionTypeDescription;
                    qDebug()<<"MiniState.RFUMapping"<<MiniState.RFUMapping;
                    qDebug()<<"MiniState.time"<<MiniState.time;
                    qDebug()<<"MiniState.originalTxnSequence"<<MiniState.originalTxnSequence;
                    qDebug()<<"MiniState.referenceId"<<MiniState.referenceId;
                    qDebug()<<"MiniState.reversalFlag"<<MiniState.reversalFlag;
                    qDebug()<<"MiniState.debitAmount"<<MiniState.debitAmount;
                    qDebug()<<"MiniState.creditAmount"<<MiniState.creditAmount;
                    qDebug()<<"MiniState.status"<<MiniState.status;
                    qDebug()<<"MiniState.StatusDesc"<<MiniState.StatusDesc;
                    qDebug()<<"MiniState.rfuNumeric1Desc"<<MiniState.rfuNumeric1Desc;
                    qDebug()<<"MiniState.rfuNumeric1Val"<<MiniState.rfuNumeric1Val;
                    qDebug()<<"MiniState.rfuNumeric2Desc"<<MiniState.rfuNumeric2Desc;
                    qDebug()<<"MiniState.rfuNumeric2Val"<<MiniState.rfuNumeric2Val;
                    qDebug()<<"MiniState.rfuNumeric3Desc"<<MiniState.rfuNumeric3Desc;
                    qDebug()<<"MiniState.rfuNumeric3Val"<<MiniState.rfuNumeric3Val;
                    qDebug()<<"MiniState.rfuNumeric4Desc"<<MiniState.rfuNumeric4Desc;
                    qDebug()<<"MiniState.rfuNumeric4Val"<<MiniState.rfuNumeric4Val;
                    qDebug()<<"MiniState.rfuText1Desc"<<MiniState.rfuText1Desc;
                    qDebug()<<"MiniState.rfuText1Val"<<MiniState.rfuText1Val;
                    qDebug()<<"MiniState.rfuText2Desc"<<MiniState.rfuText2Desc;
                    qDebug()<<"MiniState.rfuText2Val"<<MiniState.rfuText2Val;
                    qDebug()<<"MiniState.rfuText3Desc"<<MiniState.rfuText3Desc;
                    qDebug()<<"MiniState.rfuText3Val"<<MiniState.rfuText3Val;
                    qDebug()<<"MiniState.rfuText4Desc"<<MiniState.rfuText4Desc;
                    qDebug()<<"MiniState.rfuText4Val"<<MiniState.rfuText4Val;
                    qDebug()<<"MiniState.rfuText5Desc"<<MiniState.rfuText5Desc;
                    qDebug()<<"MiniState.rfuText5Val"<<MiniState.rfuText5Val;
                    qDebug()<<"MiniState.rfuText6Desc"<<MiniState.rfuText6Desc;
                    qDebug()<<"MiniState.rfuText6Val"<<MiniState.rfuText6Val;
                    qDebug()<<"MiniState.rfuText7Desc"<<MiniState.rfuText7Desc;
                    qDebug()<<"MiniState.rfuText7Val"<<MiniState.rfuText7Val;
                    qDebug()<<"MiniState.rfuText8Desc"<<MiniState.rfuText8Desc;
                    qDebug()<<"MiniState.rfuText8Val"<<MiniState.rfuText8Val;
                    qDebug()<<"MiniState.rfuText9Desc"<<MiniState.rfuText9Desc;
                    qDebug()<<"MiniState.rfuText9Val"<<MiniState.rfuText9Val;

                    qDebug()<<"MiniState.rfuText10Desc"<<MiniState.rfuText10Desc;
                    qDebug()<<"MiniState.rfuText10Val"<<MiniState.rfuText10Val;
                    qDebug()<<"MiniState.parentTxnId"<<MiniState.parentTxnId;
                    qDebug()<<"MiniState.parentTxnAmount"<<MiniState.parentTxnAmount;
                    qDebug()<<"MiniState.parentTxnAmountCurrent"<<MiniState.parentTxnAmountCurrent;
                    qDebug()<<"MiniState.chargeAmount"<<MiniState.chargeAmount;
                    qDebug()<<"MiniState.chargeAmountCurrent"<<MiniState.chargeAmountCurrent;
                    qDebug()<<"MiniState.cGST"<<MiniState.cGST;
                    qDebug()<<"MiniState.sGST"<<MiniState.sGST;
                    qDebug()<<"MiniState.iGST"<<MiniState.iGST;
                    qDebug()<<"MiniState.uGST"<<MiniState.uGST;
                    qDebug()<<"MiniState.subType"<<MiniState.subType;
                    qDebug()<<"MiniState.endingBalance"<<MiniState.endingbalance;



#endif






                    query.clear();
                    query.prepare("INSERT INTO MINISTATEMENT_TXN (SEQUENCE_NO,TRANSACTION_AMOUNT,TRANSACTION_DESC,DEBIT_CREDIT,TRANSACTION_DATE,TRANSACTION_TYPE,TRANSACTION_TYPE_NAME,TRANSACTION_TYPE_PRODUCT,TRANSACTION_TYPE_DESCRIPTION,RFUMAPPING,TIME,ORIGINAL_TXN_SEQUENCE,REFERENCE_ID,REVERSAL_FLAG,DEBIT_AMOUNT,CREDIT_AMOUNT,STATUS,STATUS_DESC, RFU_NUMERIC1_DESC,RFU_NUMERIC1_VAL,RFU_NUMERIC2_DESC,RFU_NUMERIC2_VAL,RFU_NUMERIC3_DESC,RFU_NUMERIC3_VAL,RFU_NUMERIC4_DESC,RFU_NUMERIC4_VAL,RFU_TEXT11_DESC,RFU_TEXT11_VAL,RFU_TEXT2_DESC,RFU_TEXT2_VAL,RFU_TEXT3_DESC,RFU_TEXT3_VAL,RFU_TEXT4_DESC,RFU_TEXT4_VAL,RFU_TEXT5_DESC,RFU_TEXT5_VAL,RFU_TEXT6_DESC,RFU_TEXT6_VAL,RFU_TEXT7_DESC,RFU_TEXT7_VAL,RFU_TEXT8_DESC,RFU_TEXT8_VAL,RFU_TEXT9_DESC,RFU_TEXT9_VAL,RFU_TEXT10_DESC,RFU_TEXT10_VAL,PARENT_TXN_ID,PARENT_TXN_AMOUNT,PARENT_TXN_AMOUNT_CUR,CHARGE_AMOUNT,CHARGE_AMOUNT_CUR,CGST,SGST,IGST,UGST,SUB_TYPE,ENDING_BALANCE) values (:SEQUENCE_NO,:TRANSACTION_AMOUNT,:TRANSACTION_DESC,:DEBIT_CREDIT,:TRANSACTION_DATE,:TRANSACTION_TYPE,:TRANSACTION_TYPE_NAME,:TRANSACTION_TYPE_PRODUCT,:TRANSACTION_TYPE_DESCRIPTION,:RFUMAPPING,:TIME,:ORIGINAL_TXN_SEQUENCE,:REFERENCE_ID,:REVERSAL_FLAG,:DEBIT_AMOUNT,:CREDIT_AMOUNT,:STATUS,:STATUS_DESC,:RFU_NUMERIC1_DESC,:RFU_NUMERIC1_VAL,:RFU_NUMERIC2_DESC,:RFU_NUMERIC2_VAL,:RFU_NUMERIC3_DESC,:RFU_NUMERIC3_VAL,:RFU_NUMERIC4_DESC,:RFU_NUMERIC4_VAL,:RFU_TEXT11_DESC,:RFU_TEXT11_VAL,:RFU_TEXT2_DESC,:RFU_TEXT2_VAL,:RFU_TEXT3_DESC,:RFU_TEXT3_VAL,:RFU_TEXT4_DESC,:RFU_TEXT4_VAL,:RFU_TEXT5_DESC,:RFU_TEXT5_VAL,:RFU_TEXT6_DESC,:RFU_TEXT6_VAL,:RFU_TEXT7_DESC,:RFU_TEXT7_VAL,:RFU_TEXT8_DESC,:RFU_TEXT8_VAL,:RFU_TEXT9_DESC,:RFU_TEXT9_VAL,:RFU_TEXT10_DESC,:RFU_TEXT10_VAL,:PARENT_TXN_ID,:PARENT_TXN_AMOUNT,:PARENT_TXN_AMOUNT_CUR,:CHARGE_AMOUNT,:CHARGE_AMOUNT_CUR,:CGST,:SGST,:IGST,:UGST,:SUB_TYPE,:ENDING_BALANCE)");
                    query.bindValue(":SEQUENCE_NO",MiniState.sequenceNumber);
                    query.bindValue(":TRANSACTION_AMOUNT",MiniState.transactionAmount);
                    query.bindValue(":TRANSACTION_DESC",MiniState.transactionDesc);
                    query.bindValue(":DEBIT_CREDIT",MiniState.debitCredit);
                    query.bindValue(":TRANSACTION_DATE",MiniState.transactionDate);
                    query.bindValue(":TRANSACTION_TYPE",MiniState.transactionType);
                    query.bindValue(":TRANSACTION_TYPE_NAME",MiniState.transactionTypeName);
                    query.bindValue(":TRANSACTION_TYPE_PRODUCT",MiniState.chargeAmount);
                    query.bindValue(":TRANSACTION_TYPE_DESCRIPTION",MiniState.TransactionTypeDescription);
                    query.bindValue(":RFUMAPPING",MiniState.RFUMapping);
                    query.bindValue(":TIME",MiniState.time);
                    query.bindValue(":ORIGINAL_TXN_SEQUENCE",MiniState.originalTxnSequence);
                    query.bindValue(":REFERENCE_ID",MiniState.referenceId);
                    query.bindValue(":REVERSAL_FLAG",MiniState.reversalFlag);
                    query.bindValue(":DEBIT_AMOUNT",MiniState.debitAmount);
                    query.bindValue(":CREDIT_AMOUNT",MiniState.creditAmount);
                    query.bindValue(":STATUS",MiniState.Status);
                    query.bindValue(":STATUS_DESC",MiniState.StatusDesc);
                    query.bindValue(":RFU_NUMERIC1_DESC",MiniState.rfuNumeric1Desc);
                    query.bindValue(":RFU_NUMERIC1_VAL",MiniState.rfuNumeric1Val);
                    query.bindValue(":RFU_NUMERIC2_DESC",MiniState.rfuNumeric2Desc);
                    query.bindValue(":RFU_NUMERIC2_VAL",MiniState.rfuNumeric2Val);
                    query.bindValue(":RFU_NUMERIC3_DESC",MiniState.rfuNumeric3Desc);
                    query.bindValue(":RFU_NUMERIC3_VAL",MiniState.rfuNumeric3Val);
                    query.bindValue(":RFU_NUMERIC4_DESC",MiniState.rfuNumeric4Desc);
                    query.bindValue(":RFU_NUMERIC4_VAL",MiniState.rfuNumeric4Val);
                    query.bindValue(":RFU_TEXT11_DESC",MiniState.rfuText1Desc);
                    query.bindValue(":RFU_TEXT11_VAL",MiniState.rfuText1Val);
                    query.bindValue(":RFU_TEXT2_DESC",MiniState.rfuText2Desc);
                    query.bindValue(":RFU_TEXT2_VAL",MiniState.rfuText2Val);
                    query.bindValue(":RFU_TEXT3_DESC",MiniState.rfuText3Desc);
                    query.bindValue(":RFU_TEXT3_VAL",MiniState.rfuText3Val);
                    query.bindValue(":RFU_TEXT4_DESC",MiniState.rfuText4Desc);
                    query.bindValue(":RFU_TEXT4_VAL",MiniState.rfuText4Val);
                    query.bindValue(":RFU_TEXT5_DESC",MiniState.rfuText5Desc);
                    query.bindValue(":RFU_TEXT5_VAL",MiniState.rfuText5Val);
                    query.bindValue(":RFU_TEXT6_DESC",MiniState.rfuText6Desc);
                    query.bindValue(":RFU_TEXT6_VAL",MiniState.rfuText6Val);
                    query.bindValue(":RFU_TEXT7_DESC",MiniState.rfuText7Desc);
                    query.bindValue(":RFU_TEXT7_VAL",MiniState.rfuText7Val);
                    query.bindValue(":RFU_TEXT8_DESC",MiniState.rfuText8Desc);
                    query.bindValue(":RFU_TEXT8_VAL",MiniState.rfuText8Val);
                    query.bindValue(":RFU_TEXT9_DESC",MiniState.rfuText9Desc);
                    query.bindValue(":RFU_TEXT9_VAL",MiniState.rfuText9Val);
                    query.bindValue(":RFU_TEXT10_DESC",MiniState.rfuText10Desc);
                    query.bindValue(":RFU_TEXT10_VAL",MiniState.rfuText10Val);https://mapi.finobank.com/mail#1
                        query.bindValue(":PARENT_TXN_ID",MiniState.parentTxnId);
                    query.bindValue(":PARENT_TXN_AMOUNT",MiniState.parentTxnAmount);
                    query.bindValue(":PARENT_TXN_AMOUNT_CUR",MiniState.parentTxnAmountCurrent);
                    query.bindValue(":CHARGE_AMOUNT",MiniState.chargeAmount);
                    query.bindValue(":CHARGE_AMOUNT_CURR",MiniState.chargeAmountCurrent);
                    query.bindValue(":CGST",MiniState.cGST);
                    query.bindValue(":SGST",MiniState.sGST);
                    query.bindValue(":IGST",MiniState.iGST);
                    query.bindValue(":UGST",MiniState.uGST);
                    query.bindValue(":SUB_TYPE",MiniState.subType);
                    query.bindValue(":ENDING_BALANCE",MiniState.endingbalance);



                    if(!query.exec())
                        qDebug() << "get reversal" << ":" << query.lastError();


                }

            }
            return FUNC_OK;
        }
        else
        {

            return FUNC_ERROR;
        }
    }


}


QString FPB::RFU_Display(QString RFUTag,int pos)
{
    QString Tagvalue;
    if(!QString::compare(RFUTag,"rfuNumeric1Desc",Qt::CaseInsensitive))
    {
        Tagvalue = QString::fromUtf8(MiniStatement[pos].rfuNumeric1Desc) + ":" + QString::fromUtf8(MiniStatement[pos].rfuNumeric1Val) + "\n";
        return Tagvalue;
    }
    else if(!QString::compare(RFUTag,"rfuNumeric2Desc",Qt::CaseInsensitive))
    {
        Tagvalue = QString::fromUtf8(MiniStatement[pos].rfuNumeric2Desc) + ":" + QString::fromUtf8(MiniStatement[pos].rfuNumeric2Val) + "\n";
        return Tagvalue;

    }
    else if(!QString::compare(RFUTag,"rfuNumeric3Desc",Qt::CaseInsensitive))
    {
        Tagvalue = QString::fromUtf8(MiniStatement[pos].rfuNumeric3Desc) + ":" + QString::fromUtf8(MiniStatement[pos].rfuNumeric3Val) + "\n";
        return Tagvalue;

    }
    else if(!QString::compare(RFUTag,"rfuNumeric4Desc",Qt::CaseInsensitive))
    {
        Tagvalue = QString::fromUtf8(MiniStatement[pos].rfuNumeric4Desc) + ":" + QString::fromUtf8(MiniStatement[pos].rfuNumeric4Val) + "\n";
        return Tagvalue;

    }
    else if(!QString::compare(RFUTag,"rfuText1Desc",Qt::CaseInsensitive))
    {
        Tagvalue = QString::fromUtf8(MiniStatement[pos].rfuText1Desc) + ":" + QString::fromUtf8(MiniStatement[pos].rfuText1Val) + "\n";
        return Tagvalue;

    }
    else if(!QString::compare(RFUTag,"rfuText2Desc",Qt::CaseInsensitive))
    {
        Tagvalue = QString::fromUtf8(MiniStatement[pos].rfuText2Desc) + ":" + QString::fromUtf8(MiniStatement[pos].rfuText2Val) + "\n";
        return Tagvalue;

    }
    else if(!QString::compare(RFUTag,"rfuText3Desc",Qt::CaseInsensitive))
    {
        Tagvalue = QString::fromUtf8(MiniStatement[pos].rfuText3Desc) + ":" + QString::fromUtf8(MiniStatement[pos].rfuText3Val) + "\n";
        return Tagvalue;

    }
    else if(!QString::compare(RFUTag,"rfuText4Desc",Qt::CaseInsensitive))
    {

        Tagvalue = QString::fromUtf8(MiniStatement[pos].rfuText4Desc) + ":" + QString::fromUtf8(MiniStatement[pos].rfuText4Val) + "\n";
        return Tagvalue;

    }
    else if(!QString::compare(RFUTag,"rfuText5Desc",Qt::CaseInsensitive))
    {
        Tagvalue = QString::fromUtf8(MiniStatement[pos].rfuText5Desc) + ":" + QString::fromUtf8(MiniStatement[pos].rfuText5Val) + "\n";
        return Tagvalue;

    }
    else if(!QString::compare(RFUTag,"rfuText6Desc",Qt::CaseInsensitive))
    {
        Tagvalue = QString::fromUtf8(MiniStatement[pos].rfuText6Desc) + ":" + QString::fromUtf8(MiniStatement[pos].rfuText6Val) + "\n";
        return Tagvalue;

    }
    else if(!QString::compare(RFUTag,"rfuText7Desc",Qt::CaseInsensitive))
    {

        Tagvalue = QString::fromUtf8(MiniStatement[pos].rfuText7Desc) + ":" + QString::fromUtf8(MiniStatement[pos].rfuText7Val) + "\n";
        return Tagvalue;

    }
    else if(!QString::compare(RFUTag,"rfuText8Desc",Qt::CaseInsensitive))
    {
        Tagvalue = QString::fromUtf8(MiniStatement[pos].rfuText8Desc) + ":" + QString::fromUtf8(MiniStatement[pos].rfuText8Val) + "\n";
        return Tagvalue;
    }
    else if(!QString::compare(RFUTag,"rfuText9Desc",Qt::CaseInsensitive))
    {
        Tagvalue = QString::fromUtf8(MiniStatement[pos].rfuText9Desc) + ":" + QString::fromUtf8(MiniStatement[pos].rfuText9Val) + "\n";
        return Tagvalue;
    }
    else if(!QString::compare(RFUTag,"rfuText10Desc",Qt::CaseInsensitive))
    {
        Tagvalue = QString::fromUtf8(MiniStatement[pos].rfuText10Desc) + ":" + QString::fromUtf8(MiniStatement[pos].rfuText10Val) + "\n";
        return Tagvalue;
    }

}


QString FPB::spiltRFUvalue(char * Data,int position)
{
    QStringList RFUspilt;
    QString Details = QString::fromLocal8Bit(Data);
    RFUspilt.clear();
    RFUspilt= Details.split("#");
#if DEBUG
    qDebug()<<"spiltRFUvalue"<<Details;
    qDebug()<<"RFUspilt.length()*********************************"<<RFUspilt.length();
#endif
    Details.clear();
    for(int i=1 ; i < RFUspilt.length() ; i++)
    {
#if DEBUG
        qDebug()<<"RFUspilt"<<RFUspilt.at(i);
#endif
        QString result = RFU_Display(RFUspilt.at(i),position);
        if(QString::compare(result.trimmed(),":",Qt::CaseInsensitive))
        {
            Details = Details +result ;
#if DEBUG
            qDebug()<<"Details"<<Details;
#endif
        }
    }
    return Details;
}


void FPB::on_listWidget_clicked(const QModelIndex &index)
{
    int flag = 0;
    QString Display;
    char disp[70];

    if(TRANS_TYPE == MINISTATEMENT)
    {
        int currentIndex = ui->listWidget->currentRow();
        click = !click;
        if(strcmp(MiniStatement[currentIndex].RFUMapping,"null") !=0)
        {
            ui->listWidget->clear();

            if(click == 0)
            {

                for(int i=0;i<Merdetail.icount;i++)
                {
                    if( !QString::fromUtf8(MiniStatement[i].endingbalance).isEmpty() && !strcmp(MiniStatement[i].endingbalance,"null")==0 && flag_bc_merchant_account == 'B')
                    {
                        Display = QString::fromUtf8(MiniStatement[i].transactionDate) + " "  + QString::fromUtf8(MiniStatement[i].time) + " " + QString::fromUtf8(MiniStatement[i].transactionAmount)  + "  " + QString::fromUtf8(MiniStatement[i].endingbalance) + "\n" + QString::fromUtf8(MiniStatement[i].TransactionTypeDescription) +" " + QString::fromUtf8(MiniStatement[i].StatusDesc)+ "\n" ;

                    }
                    else
                    {
                        Display = QString::fromUtf8(MiniStatement[i].transactionDate) + " "  + QString::fromUtf8(MiniStatement[i].time) + " " + QString::fromUtf8(MiniStatement[i].transactionAmount) + "\n" + QString::fromUtf8(MiniStatement[i].TransactionTypeDescription) +" " + QString::fromUtf8(MiniStatement[i].StatusDesc)+ "\n" ;

                    }
                    if( !QString::fromUtf8(MiniStatement[i].RFUMapping).isEmpty() && !strcmp(MiniStatement[i].RFUMapping,"null")==0)
                    {
                        sprintf(disp,"%-30s(i)",MiniStatement[i].referenceId);
                        Display = Display + QString::fromUtf8(disp)+ "\n";

                    }
                    else
                    {
                        Display = Display + QString::fromUtf8(MiniStatement[i].referenceId) + "\n";
                    }
                    if(!strcmp(MiniStatement[i].sGST,"0.0")==0 && !strcmp(MiniStatement[i].cGST,"0.0")==0 && strcmp(MiniStatement[i].iGST,"0.0")==0)
                    {
                        Display = Display + QString::fromUtf8(MiniStatement[i].chargeAmount) + "\n" ;
                    }
                    Display = Display + "_____________________________________";
                    ui->listWidget->addItem(Display);
                }
            }
            else if(click == 1)
            {
                for(int i=0;i<Merdetail.icount;i++)
                {
#if DEBUG
                    qDebug()<<"i-------"<<i;
                    qDebug()<<"currentIndex"<<currentIndex;
                    qDebug()<<currentIndex<<currentIndex;
#endif
                    if( !QString::fromUtf8(MiniStatement[i].endingbalance).isEmpty() && !strcmp(MiniStatement[i].endingbalance,"null")==0 && flag_bc_merchant_account == 'B')
                    {
                        Display = QString::fromUtf8(MiniStatement[i].transactionDate) + " "  + QString::fromUtf8(MiniStatement[i].time) + " " + QString::fromUtf8(MiniStatement[i].transactionAmount)  + "  " + QString::fromUtf8(MiniStatement[i].endingbalance) + "\n" + QString::fromUtf8(MiniStatement[i].TransactionTypeDescription) +" " + QString::fromUtf8(MiniStatement[i].StatusDesc)+ "\n" ;

                    }
                    else
                    {
                        Display = QString::fromUtf8(MiniStatement[i].transactionDate) + " "  + QString::fromUtf8(MiniStatement[i].time) + " " + QString::fromUtf8(MiniStatement[i].transactionAmount) + "\n" + QString::fromUtf8(MiniStatement[i].TransactionTypeDescription) +" " + QString::fromUtf8(MiniStatement[i].StatusDesc)+ "\n" ;

                    }
                    if( !QString::fromUtf8(MiniStatement[i].RFUMapping).isEmpty() && !strcmp(MiniStatement[i].RFUMapping,"null")==0)
                    {
                        sprintf(disp,"%-30s(i)",MiniStatement[i].referenceId);
                        Display = Display + QString::fromUtf8(disp)+ "\n";

                    }
                    else
                    {
                        Display = Display + QString::fromUtf8(MiniStatement[i].referenceId) + "\n";
                    }

                    if(!strcmp(MiniStatement[i].sGST,"0.0")==0 && !strcmp(MiniStatement[i].cGST,"0.0")==0 && strcmp(MiniStatement[i].iGST,"0.0")==0)
                    {
                        Display = Display + QString::fromUtf8(MiniStatement[i].chargeAmount) + "\n" ;
                    }


                    if(i == currentIndex)
                    {
                        printf("MiniStatement[i].RFUMapping%s\n",MiniStatement[i].RFUMapping);
                        if(!strcmp(MiniStatement[i].RFUMapping,"null")==0 && !QString::fromUtf8(MiniStatement[i].RFUMapping).isEmpty())
                        {
                            QString Details = spiltRFUvalue(MiniStatement[i].RFUMapping,currentIndex);
                            flag =0;
                            QStringList RFUspilt;
                            RFUspilt= QString::fromLocal8Bit(MiniStatement[i].RFUMapping).split("#");
#if DEBUG
                            qDebug()<<"RFUspilt.length()*********************************"<<RFUspilt.length();
#endif
                            if(RFUspilt.length() > 5)
                            {
                                flag =1;
                            }

                            if(flag == 1)
                            {
                                ui->Confirm_rev_list->clear();
                                ui->Confirm_rev_list->setFont(QFont("San Serif",12));
                                ui->Confirm_rev_list->addItem(Details);
                            }
                            else
                            {
                                Display = Display + Details;
                            }
                        }
                    }
                    Display = Display + "_____________________________________";
                    ui->listWidget->addItem(Display);
                }
            }
        }

        if(flag == 1 && click ==1)
        {
            memset(&ConfigPrint,0,sizeof(ConfigPrint));
            memcpy((char *)ConfigPrint.ucCustName, (char *)FINDCUST.ucRDCLfullName_resp,strlen((char *)FINDCUST.ucRDCLfullName_resp));
            memcpy((char *)ConfigPrint.ucCustMobNo, MiniStatement[currentIndex].rfuNumeric4Val, strlen(MiniStatement[currentIndex].rfuNumeric4Val));
            memcpy((char *)ConfigPrint.ucBeneName, MiniStatement[currentIndex].rfuText5Val ,strlen(MiniStatement[currentIndex].rfuText5Val));
            memcpy((char *)ConfigPrint.ucAmount, &MiniStatement[currentIndex].transactionAmount[3],strlen(&MiniStatement[currentIndex].transactionAmount[3]));
            memcpy((char *)ConfigPrint.uctxntype, MiniStatement[currentIndex].TransactionTypeDescription,strlen(MiniStatement[currentIndex].TransactionTypeDescription));
            memcpy((char *)ConfigPrint.ucRefNo, MiniStatement[currentIndex].referenceId,strlen(MiniStatement[currentIndex].referenceId));

            ui->Reversal_label->setText("                  MINISTATEMENT");
            ui->Reversal_conf_button->setText("BACK");
            ui->Reversal_next_button->setText("PRINT");
            ui->stackedWidget->setCurrentIndex(43);
            return;
        }
        else
        {
            ui->stackedWidget->setCurrentIndex(32);
            return;
        }
    }
}


int FPB::Agent_Auth()
{
    int iRet = 0;
    char* jsondata;
    CMisc miscObj;
    Json1::Value RequestedData,mainreq;
    char buff1[100];
    memset(buff1,0,sizeof(buff1));
    char ErrorMsg[150];
    uidmessage::instance()->hide_Message();
    uidmessage::instance()->Display_BusyMessage("Please Wait....");

    iGenerateRequestID((char *)username,(unsigned char *)buff1);

    mainreq["RequestId"] =  buff1;
    mainreq["MethodId"]= 1;
    mainreq["TellerID"]= username;
    mainreq["SessionId"] = (char *)configLoginLeg2.ucSessionId;//buff_tmp.toAscii().data();
    mainreq["TokenId"]= "";
    mainreq["IsEncrypt"]= false;
    mainreq["X_Auth_Token"] ="";
    mainreq["SessionExpiryTime"]= (char *)configLoginLeg2.ucSessionExpiryTime;//"10/11/2018 11:58:44 PM";
    mainreq["ChannelID"]= CHANNEL_ID;
    mainreq["ProductCode"]= "";
    mainreq["Amount"]= "";
    if(Resend_flag == 1)
    {
        RequestedData["MethodId"]= 4;
    }
    else
    {
        RequestedData["MethodId"]= 1;
    }
    RequestedData["CustomerMobileNo"] =(char *)FC_MobileNumber;//ReversalDetails[currentIndex].rfuNumeric4Val;
    RequestedData["MessageId"]=42;
    RequestedData["OtpParam"]["AccountNo"] =mainreq.null;
    serialize1(Convertstr1(RequestedData.toStyledString()));
    mainreq["RequestData"] = buff_tmp.toAscii().data();

    jsondata = Convertstr1(mainreq.toStyledString());

#if DEBUG
    qDebug()<<"------------->"<<jsondata;
#endif
    //-------------------------------------------------------------
    memset(gSendData,0,sizeof(gSendData));
    strncpy((char*)gSendData, jsondata, strlen(jsondata));
#if DEBUG
    printf("gSendData = %s\n\n",gSendData);
#endif

#ifdef UAT_HTTP     // File Download

    uidmessage::instance()->hide_Message();
    uidmessage::instance()->Display_BusyMessage("Connecting server");
    sleep(1);
    uidmessage::instance()->hide_Message();
    hComm.close_connection();
    res = hComm.open_connect(SERVERIP,SERVERPORT);
    if(res != 0)
    {
        qDebug() << "Failed to connect to the Server";
        uidmessage::instance()->hide_Message();
        uidmessage::instance()->DisplayMessage("Server Not Connected");
        qDebug()<<"FUNCTION_FAILED";
        return FUNCTION_FAILED ;
    }


    sleep(1);
    uidmessage::instance()->hide_Message();
    uidmessage::instance()->Display_BusyMessage("Transmitting ..");


    int len_gSendData = strlen((char *)gSendData);
    memset(oBuff,0,sizeof(oBuff));
    if((hComm.host_2Sendrecv((unsigned char *)gSendData,len_gSendData,( unsigned char*)oBuff,(int *)&oLen,(int)response_timeout,0))!=SUCCESS)
    {
        printf("No Response From Server\n");
        hComm.close_connection();
        miscObj.DisplayMsgBox((char *)"Response Time-Out");
        sleep(2);
        qDebug()<<"***   Response Time-Out   ***";
        uidmessage::instance()->hide_Message();
        //return SOCKET_RESPONSE_FAILED;
        return FUNCTION_FAILED;
    }

    hComm.close_connection();

#else     // File Download full condition
#if DEBUG
    printf("gSendData = %s\n\n",gSendData);
#endif
    memset(str_url,0,sizeof(str_url));
#ifdef PRODUCTION
    strcpy(str_url,URL_CREATION(LOGIN_URL_PROD));
#else
    strcpy(str_url,URL_CREATION((char *)OLDNEW_CHANGE_PASSWORD_URL));
#endif

    int retk = file_download(str_url, (char *)gSendData,"/usr/FINO/FPB/RESP/Agent_Auth.TXT",120);
    if(retk!=0)
    {
        sleep(1);
        uidmessage::instance()->hide_Message();
        return FUNCTION_FAILED;
    }

#endif

#if DEBUG
    printf("\noBuff = %s\n",oBuff);
#endif

    FILE *fp1 = fopen("/usr/FINO/FPB/RESP/Agent_Auth.TXT", "rb"); // open
    if (fp1 == NULL)
    {
        qDebug()<<"Can't Open IntJSON.TXT File";
        //uidmessage::instance()->hide_Message();
        return FUNCTION_FAILED;
    }

    fseek(fp1, 0, SEEK_END);
    int template_len = ftell(fp1);

    fseek(fp1, 0, SEEK_SET);
    int ret=fread(oBuff,sizeof(unsigned char ),template_len,fp1);
    if(ret==0)
        fclose(fp1);

    QString OTPBuff = QString::fromLocal8Bit(oBuff);//Get the ouput buffer in QString

    bool check = OTPBuff.contains("ResponseCode");
    if(check == false)
    {
        miscObj.DisplayMsgBox((char *)"Invalid data");
        uidmessage::instance()->hide_Message();
        return FUNCTION_FAILED;
    }

    OTPBuff.remove("\\");
    OTPBuff.remove("{");
    OTPBuff.remove("}");
    OTPBuff.remove('"');
#if DEBUG
    qDebug()<<"OTPBuff"<<OTPBuff;
#endif
    QStringList query = OTPBuff.split(",");
#if DEBUG
    for(int i=0 ; i < query.length() ; i++)
        qDebug()<<"Query Response"<<query.at(i);
#endif
    QStringList RequestId = query.filter("RequestId");
    QStringList DisplayMessage = query.filter("DisplayMessage");
    QStringList ResponseCode = query.filter("ResponseCode");
    QStringList ResponseMessage = query.filter("ResponseMessage");

    QString str ,strToFind,value,value1,value2;
    str = ResponseCode.at(0);
    strToFind = ":";
    value1 = str.mid(str.indexOf(strToFind)+strToFind.length());
    memset(Merotp.Revotp_responsecode, 0, sizeof(Merotp.Revotp_responsecode));
    memcpy(Merotp.Revotp_responsecode,value1.trimmed().toAscii().data(), strlen(value1.trimmed().toAscii().data()));
    //  qDebug()<<"value1"<<value1;
    if(value1 =="0")
    {
        str = DisplayMessage.at(2);
        strToFind = ":";
        value2 = str.mid(str.indexOf(strToFind)+strToFind.length());
        str= RequestId.at(2);
        strToFind= "RequestId:";
        memset(Merotp.Revotp_requestid,0,sizeof(Merotp.Revotp_requestid));
        strcpy(Merotp.Revotp_requestid,str.mid(str.indexOf(strToFind)+strToFind.length()).toAscii().data());
#if DEBUG
        qDebug()<<"RequestId"<<Merotp.Revotp_requestid;
#endif
        char otp_mobileno[20], otp_message[700];
        memset(otp_mobileno, 0, sizeof(otp_mobileno));
        memset(otp_message, 0, sizeof(otp_message));
        //    sprintf(otp_mobileno,"%s",Mobile.toAscii().data());
        sprintf(otp_mobileno,"******%s",&FC_MobileNumber[6]);
        sprintf(otp_message, "One Time Password (OTP) has been\n sent to your registered mobile %s,\n Please enter the same here",otp_mobileno);
        ui->Otp_display_label->setText(otp_message);
        uidmessage::instance()->hide_Message();
        return FUNC_OK;
    }
    else
    {
        str = ResponseMessage.at(0);
        strToFind = ":";
        value2 = str.mid(str.indexOf(strToFind)+strToFind.length());
        memset(Merotp.Revotp_response_msg, 0, sizeof(Merotp.Revotp_response_msg));
        memcpy(Merotp.Revotp_response_msg,value2.trimmed().toAscii().data(), strlen(value2.trimmed().toAscii().data()));
        if(strcmp((char *)Merotp.Revotp_response_msg,"Session Expired.")==0)
        {
            uidmessage::instance()->hide_Message();
            sprintf(ErrorMsg,"%s",/*(char *)LTS.ucResponseCode,*/(char *)Merotp.Revotp_response_msg);
            miscObj.DisplayMsgBox(ErrorMsg);
            uidmessage::instance()->hide_Message();
            return SESSION_EXPIRED_FAILED;
        }
        sprintf(ErrorMsg,"%s",/*(char *)OTP.ucResponseCode,*/(char *)Merotp.Revotp_response_msg);
        miscObj.DisplayMsgBox(ErrorMsg);
        uidmessage::instance()->hide_Message();
        return FUNCTION_FAILED;
    }

    return FUNCTION_SUCCESS;

}


