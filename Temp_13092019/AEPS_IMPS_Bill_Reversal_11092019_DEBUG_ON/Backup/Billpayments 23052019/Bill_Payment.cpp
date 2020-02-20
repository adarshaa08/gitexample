#include "FPB.h"
#include "ui_FPB.h"
#include "database.h"
#include "displaymessage.h"
#include "getprofile.h"
#include "Login.h"
#include "findcust.h"
#include "Bill_Payment.h"
//#include "aadhaar.h"
#include "entrydlg.h"
#include <QIODevice>
#include "impscommon.h"
//#include "impsbill.h"
#include<fstream>
extern "C"
{
#include "gl11.h"
#include "http_client.h"
#include "uid_auth.h"
}
extern char * Convertstr(string str);
extern SUPPORTDATA_DEVICEINFO sd_deviceinfo;
extern Login_leg2 configLoginLeg2;
BILLPAYMENT bill;
BILLERDATA billdata;
QString Mobile_number;
QString Biller_code,Biller_id,Bharatpaylogo;
char FIELD_ID[3][50],FIELD_NAME[3][50],FIELD_TYPE[3][50],FIELD_VISIBILITY[3][50],FIELD_VALIDATION[3][100] ;
int FIELD_LENGTH[3],FIELD_MIN_LENGTH[3];
int count =0 ;
int display_flag =0;
int Calculate_charges =0;
int fetch_bill =0;
QString Total_Amount,Charges,Service_Provider,Category,first_input,Amount;
extern FINDCUSTOMER FINDCUST;
void FPB::on_Bill_payment_Button_clicked()
{
    char msgdisplay[100];
    CMisc miscObj;
    memset(&billdata,0,sizeof(billdata));
    ui->BharatBill_Logo->hide();
    ui->BharatBill_Logo_confirm->hide();
    qDebug()<<"ui->Search_lineEdit->text().size()"<<ui->Search_lineEdit->text().size();


    if (ui->Search_lineEdit->text().size() != 10)
    {
        sprintf(msgdisplay,"Please Search by mobile no. to access this feature");
        miscObj.DisplayMsgBox(msgdisplay);
        return ;
    }
    QSqlQuery query;
    query.clear();

    query.prepare("SELECT  SUBCATEGORY FROM BILLER_DETAILS  where MODE__ID = 2 GROUP BY SUBCATEGORY");

    if(!query.exec())
        qDebug() << query.lastError();
    else
    {
        //  miscObj.AudioTest((char *)"054");  //Please Select Bank
        ui->Bill_category->clear(); // clear list
        ui->Bill_category->setFont(QFont("Courier",14));
        while(query.next())
        {
            if((query.value(0).toString() != NULL))
            {
                ui->Bill_category->addItem(query.value(0).toString());

            }
        }
    }
    ui->Bill_service_provider->clear();
    ui->stackedWidget->setCurrentIndex(37);
    return;
}



void FPB::on_Bill_category_activated(const QString &arg1)
{
    QSqlQuery query;
    query.clear();
    qDebug()<<"Arg value"<<arg1;
    Service_Provider.clear();
    strcpy((char *)billdata.ucCategory,arg1.toAscii().constData());
    Category.clear();
    Category = arg1;
    query.prepare("SELECT  BILLER__NAME FROM BILLER_DETAILS WHERE SUBCATEGORY  = ? ");
    query.addBindValue(arg1);
    if(!query.exec())
        qDebug() << query.lastError();
    else
    {
        ui->Bill_service_provider->clear(); // clear list
        ui->Bill_service_provider->setFont(QFont("Courier",14));
        while(query.next())
        {
            if((query.value(0).toString() != NULL))
            {
                ui->Bill_service_provider->addItem(query.value(0).toString());
            }
        }
    }

    return;
}


void FPB::on_Bill_service_provider_activated(const QString &arg1)
{
    QSqlQuery query;
    Service_Provider.clear();
    Service_Provider = arg1;
    query.clear();
    query.prepare("SELECT  BILLER__CODE,FLAG1 FROM BILLER_DETAILS WHERE BILLER__NAME  = ? AND MODE__ID = 2");
    query.addBindValue(arg1);
    if(!query.exec())
        qDebug() << query.lastError();
    else
    {
        while(query.next())
        {
             Biller_code = query.value(0).toString();
             Bharatpaylogo = query.value(1).toString();
            qDebug()<<"Biller code "<<Biller_code;
            qDebug()<<"Bharatpaylogo"<<Bharatpaylogo;
            strcpy((char *)billdata.ucBillercode,Biller_code.toLatin1().data());

        }
    }
    if(Bharatpaylogo == "true")
    {
        ui->BharatBill_Logo->setVisible(true);
    }
    else if(Bharatpaylogo == "false")
    {
        ui->BharatBill_Logo->setVisible(false);
    }
    return;
}


void FPB::on_Bill_payment_Next_button_clicked()
{

     CMisc miscObj;
    QString temp,remove_dollar;
    char msgdisplay[50];



    if(Service_Provider.isEmpty() || Biller_code.isEmpty())
    {
       sprintf(msgdisplay,"Please Select Service Provider first");
        miscObj.DisplayMsgBox(msgdisplay);
        return ;
    }

    ui->first_editline->setVisible(false);
    ui->second_editline->setVisible(false);
    ui->third_editline->setVisible(false);
    ui->first_label->setVisible(false);
    ui->second_label->setVisible(false);
    ui->third_label->setVisible(false);
    ui->Date_of_birth_edit->setVisible(false);


    ui->first_editline->clear();
    ui->second_editline->clear();
    ui->third_editline->clear();
    ui->first_label->clear();
    ui->second_label->clear();
    ui->third_label->clear();
    ui->Date_of_birth_edit->clear();
    ui->Enter_amount_edit->clear();
    ui->Enter_amount_edit->setReadOnly(false);
    ui->Total_amount_display->setText("       0.00");
    ui->Calculate_charges_button->setText("Calculate Charges");
    ui->Calculate_charges_button->setEnabled(true);
    ui->Calculate_charges_button->setStyleSheet("background-color: rgb(85, 0, 127); color: rgb(255, 255, 255)");

    QRegExp rx("\\d+");
    QValidator *validator = new QRegExpValidator(rx, this);
    ui->Enter_amount_edit->setValidator(validator);



    count =0 ;
    display_flag =0;
    Calculate_charges =0;
    fetch_bill =0;

    QSqlQuery query;
    query.clear();
    query.prepare("SELECT Count(*) FROM FIELD_DETAILS WHERE BILLER__CODE = :code");
    query.bindValue(":code", (char *)billdata.ucBillercode);



    if(!query.exec())
        qDebug() << query.lastError();
    else
    {
        while(query.next())
        {
             count = query.value(0).toInt();
            qDebug()<<"count"<<count;

        }
    }
         int i= 0;
        query.clear();
        query.prepare("SELECT FIELD__ID,FIELD__NAME,FIELD__TYPE,FIELD__LENGTH,FIELD__VISIBILITY,FIELD__MIN__LENGTH,FIELD__VALIDATION from FIELD_DETAILS where BILLER__CODE = :code");
        query.bindValue(":code", (char *)billdata.ucBillercode);


        if(!query.exec())
            qDebug() << query.lastError();
        else
        {
            while(query.next())
            {
                qDebug()<<"Inside the next";
                strcpy(FIELD_ID[i],query.value(0).toString().trimmed().toAscii().data());
                printf("FIELD_ID%s\n",FIELD_ID[i]);
                strcpy(FIELD_NAME[i],query.value(1).toString().trimmed().toAscii().data());
                printf("FIELD_NAME%s\n",FIELD_NAME[i]);
                strcpy(FIELD_TYPE[i],query.value(2).toString().trimmed().toAscii().data());
                printf("FIELD_TYPE%s\n",FIELD_TYPE[i]);
                FIELD_LENGTH[i] =  query.value(3).toInt();
                printf("FIELD_LENGTH%d\n",FIELD_LENGTH[i]);
                strcpy(FIELD_VISIBILITY[i],query.value(4).toString().trimmed().toAscii().data());
                printf("FIELD_VISIBILITY%s\n",FIELD_VISIBILITY[i]);
                FIELD_LENGTH[i] =  query.value(5).toInt();
                printf("FIELD_MIN_LENGTH%d\n",FIELD_MIN_LENGTH[i]);
                strcpy(FIELD_VALIDATION[i],query.value(6).toString().trimmed().toAscii().data());
                printf("FIELD_VALIDATION%s\n",FIELD_VALIDATION[i]);

                i++;
                qDebug()<<"iiiiiiiiiiii"<<i;
            }
        }
        printf("FIELD_NAME%s\n",FIELD_NAME[1]);
        printf("FIELD_NAME%s\n",FIELD_NAME[1]);

        if(count>=1)
        {
            qDebug()<<"Inisde the switch case1";
            ui->first_label->setVisible(true);
            ui->first_editline->setVisible(true);

            switch(count)
            {
            case 1:
                ui->first_label->setGeometry(10,70,91,21);
                ui->first_editline->setGeometry(130,70,181,21);
                break;
            case 2:
                ui->first_label->setGeometry(10,50,91,21);
                ui->first_editline->setGeometry(130,50,181,21);
                break;
            case 3:
                ui->first_label->setGeometry(10,40,91,21);
                ui->first_editline->setGeometry(130,40,181,21);
                break;
            }



            ui->first_label->setText(QString::fromLatin1(FIELD_NAME[0]));
          //  ui->first_editline->setMaxLength(FIELD_LENGTH[0]);
            temp.clear();
            temp = QString::fromLatin1(FIELD_VALIDATION[0]);
           // temp = "^[0-9]{12,12}$";
            qDebug()<<"temp"<<temp;
//            remove_dollar = temp.remove("$");
            qDebug()<<"Inside memcmp";
            QRegExp rx5(temp);
            QValidator *validator5 = new QRegExpValidator(rx5, this);
            ui->first_editline->setValidator(validator5);

        }
        if(count>=2)
        {
            qDebug()<<"Inisde the switch case2";
            ui->second_label->setVisible(true);
            if(memcmp(FIELD_ID[1],"109",3)==0)
            {
                qDebug()<< "Inside the 109";
                ui->Date_of_birth_edit->setVisible(true);
            }
            else
            {
            ui->second_editline->setVisible(true);
            }
            switch(count)
            {
            case 2:
                qDebug()<<"Inside count switch";
                ui->second_label->setGeometry(10,90,91,21);
                if(memcmp(FIELD_ID[1],"109",3)==0)
                {
                    qDebug()<< "Inside the 12233";

                    ui->Date_of_birth_edit->setGeometry(130,90,181,21);
                }
                else
                {
                    ui->second_editline->setGeometry(130,90,181,21);
                }
                break;
            case 3:
                ui->second_label->setGeometry(10,70,91,21);
                ui->second_editline->setGeometry(130,70,181,21);
                break;
            }
            printf("FIELD_NAME%s\n",FIELD_NAME[1]);
            printf("FIELD_NAME%s\n",FIELD_NAME[1]);

            ui->second_label->setText(QString::fromLatin1(FIELD_NAME[1]));
            if(memcmp(FIELD_ID[1],"109",3)==0)
            {
                ui->second_editline->setMaxLength(FIELD_LENGTH[1]);
            }
            temp.clear();
            remove_dollar.clear();
            temp = QString::fromLatin1(FIELD_VALIDATION[1]);
//            qDebug()<<"temp"<<temp;
//             remove_dollar = temp.remove("$");
            qDebug()<<"remove_dollar"<<temp;
            if(memcmp(FIELD_ID[1],"109",3)!=0)
            {
                qDebug()<<"Inside memcmp";
                QRegExp rx2(temp);
                QValidator *validator2 = new QRegExpValidator(rx2, this);
                ui->second_editline->setValidator(validator2);
            }
            ui->third_editline->setVisible(false);
            ui->third_label->setVisible(false);
        }
        if(count>=3)
        {
            qDebug()<<"Inisde the switch case3";
            ui->third_label->setVisible(true);
            ui->third_editline->setVisible(true);
            ui->third_label->setGeometry(10,100,111,20);
            ui->third_editline->setGeometry(130,100,181,21);
            ui->third_label->setText(QString::fromLatin1(FIELD_NAME[2]));
            ui->third_editline->setMaxLength(FIELD_LENGTH[2]);
            temp.clear();
            remove_dollar.clear();
             temp = QString::fromLatin1(FIELD_VALIDATION[2]);
//            qDebug()<<"temp"<<temp;
//             remove_dollar = temp.remove("$");
            qDebug()<<"remove_dollar"<<temp;
            QRegExp rx3(temp);
            QValidator *validator3 = new QRegExpValidator(rx3, this);
            ui->third_editline->setValidator(validator3);

        }

        QRegExp rx4("\\d+");
        QRegExpValidator *validator4 = new QRegExpValidator(rx4,this);
        ui->Enter_amount_edit->setValidator(validator4);

    ui->stackedWidget->setCurrentIndex(38);
    return;
}


void FPB::on_Calculate_charges_button_clicked()
{
    int iRet =-2;

    iRet = billpayment_charges();
    uidmessage::instance()->hide_Message();

    qDebug()<<"After return";
    if(iRet != FUNCTION_SUCCESS)
    {
        if(iRet == SESSION_EXPIRED_FAILED)
        {
            ui->stackedWidget->setCurrentIndex(0);
            return;
        }
        else
        {
            qDebug()<<"Inside calculate page return";
            ui->stackedWidget->setCurrentIndex(38);
            return;
        }
    }
    else if(iRet == FUNCTION_SUCCESS)
    {
        ui->stackedWidget->setCurrentIndex(38);
        return;
    }

}

int FPB::billpayment_charges()
{

    uidmessage::instance()->hide_Message();
    uidmessage::instance()->Display_BusyMessage("Please Wait...");
    char nullbuff[5];
    char ErrorMsg[100];
    memset(ErrorMsg,0,sizeof(ErrorMsg));
    QSqlQuery query;
    QString Amount;
    char msgdisplay[50];
    char IMPS_SupportData[1000];
    CMisc miscObj;
    QString Partialpay_flag;
    query.clear();
    query.prepare("SELECT  PARTIALPAY FROM BILLER_DETAILS WHERE BILLER__CODE  = ? ");
    query.addBindValue(Biller_code);
    if(!query.exec())
        qDebug() << query.lastError();
    else
    {
        while(query.next())
        {
            Partialpay_flag =  query.value(0).toString();
            qDebug()<<"Partialpay_flag"<<Partialpay_flag;
        }
    }
    query.clear();
    QString Acc_Bal;
    Acc_Bal = QString::fromUtf8((const char*)Balance_Display).remove("Balance : ");
    qDebug()<<"Acc_Bal"<<Acc_Bal.trimmed();


    if(Partialpay_flag == "N" && fetch_bill == 0)
    {
        sprintf(msgdisplay,"PLease fetch biller details first");
        miscObj.DisplayMsgBox(msgdisplay);
        return FUNCAL_FAILED;
    }

    if(Acc_Bal.toFloat() < ui->Enter_amount_edit->text().toFloat())
    {

        sprintf(msgdisplay,"Amount should not be greater than Merchant balance");
        miscObj.DisplayMsgBox(msgdisplay);
        qDebug()<<"Outside the ok button";
        return FUNCAL_FAILED;
    }

    if (ui->Enter_amount_edit->text().isEmpty())
    {
        sprintf(msgdisplay,"Amount is mandatory");
        miscObj.DisplayMsgBox(msgdisplay);
        return FUNCAL_FAILED ;
    }
    memset(&bill,0,sizeof(bill));
    memset(nullbuff, 0, sizeof(nullbuff));
    strcpy(nullbuff,"");

    Clear_DeviceInfo();
    int iretDI = DeviceInfo();

    if(iretDI != FUNCTION_SUCCESS)
        return FUNCTION_FAILED;



    iGenerateRequestID((char *)username,(unsigned char *)bill.ucRequestId_req);
    bill.ucMethodId = 38;
    memcpy(bill.ucTellerId_req,username,sizeof(username));
    memcpy((char *)bill.ucSessionId_req,configLoginLeg2.ucSessionId,strlen((char *)configLoginLeg2.ucSessionId));
    printf("bill.ucSessionId_req%s",bill.ucSessionId_req);
    memcpy((char *)bill.ucIsEncrypt_req,"false",5);
    bill.ucChannelID_req =3;
    memcpy((char *)bill.ucSessionExpiryTime_req,(char *)configLoginLeg2.ucSessionExpiryTime,strlen((char *)configLoginLeg2.ucSessionExpiryTime));

    qDebug()<<"3";

    memcpy((char *)bill.Req_asyncFlag_req,"N",1);
    memcpy(bill.Req_amount_req,ui->Enter_amount_edit->text().trimmed().toAscii().data(),strlen(ui->Enter_amount_edit->text().trimmed().toAscii().data()));
    memcpy((char *)bill.Req_mobileNumber_req,FC_MobileNumber,10);
    memcpy((char *)bill.Req_cashoraccount_req,"CASH",4);
    memcpy((char *)bill.Req_txnType_req,"BILLPAY",7);
    memcpy((char *)bill.Req_impsserviceflag_req,"N",1);
    memcpy((char *)bill.Req_requestbitmap_req,"00000100",8);
    memcpy((char *)bill.Req_merchantMobileNumber_req,(char *)configLoginLeg2.ucmobileNumber,strlen((char *)configLoginLeg2.ucSessionId));
    qDebug()<<"4";

    //    memcpy((char *)bill.AB_name_req,"AADARSH SAMGA",13);
    //    memcpy((char *)bill.AB_mobileno_req,"8454984086",13);
    //    memcpy((char *)bill.AB_address_req,"Kamothe",7);

    memcpy((char *)bill.BD_UserID_req,FC_MobileNumber,10);
    memcpy((char *)bill.BD_CustomerID_req,FC_MobileNumber,10);
    memcpy((char *)bill.BD_CustomerMobNo_req,FC_MobileNumber,10);

    memcpy((char *)bill.CA_MobileNo_req,FC_MobileNumber,10);
    bill.CA_Auth_id_req = 0;

    Biller_code = QString::fromLatin1((char *)billdata.ucBillercode);

    query.clear();
    query.prepare("SELECT  BILLER__ID FROM BILLER_DETAILS WHERE BILLER__CODE  = ? ");
    query.addBindValue(Biller_code);
    if(!query.exec())
        qDebug() << query.lastError();
    else
    {
        while(query.next())
        {
            strcpy((char *)bill.BD_BillerId_req,query.value(0).toString().trimmed().toLatin1().data());
            printf("bill.ucBillercode%s",bill.BD_BillerId_req);
        }
    }
    query.clear();

    //  memcpy((char *)bill.BD_BillerId_req,"IGLOB",5);first_editline
    memcpy((char *)bill.BD_PaymentAmount_req,ui->Enter_amount_edit->text().trimmed().toAscii().data(),strlen(ui->Enter_amount_edit->text().trimmed().toAscii().data()));
    memcpy((char *)bill.BD_Authenticator1_req,ui->first_editline->text().trimmed().toAscii().data(),strlen(ui->first_editline->text().trimmed().toAscii().data()));
    if(count >= 2)
    {
        memcpy((char *)bill.BD_Authenticator1_req,ui->second_editline->text().trimmed().toAscii().data(),strlen(ui->second_editline->text().trimmed().toAscii().data()));
    }
    if(count >= 3)
    {
        memcpy((char *)bill.BD_Authenticator1_req,ui->third_editline->text().trimmed().toAscii().data(),strlen(ui->third_editline->text().trimmed().toAscii().data()));
    }
    memcpy((char *)bill.PT_appId_req,CLIENT_ID,strlen(CLIENT_ID));
    bill.PT_analysisFlag_req = 1;
    bill.PT_isInclusive_req =0;
    bill.PT_isClubbed_req =0;


    memcpy((char *)bill.FTL_currency_req,"INR",3);
    memcpy((char *)bill.FTL_costCenter_req,configLoginLeg2.ucBAcode,strlen(configLoginLeg2.ucBAcode));


    memcpy((char *)bill.FTL_first_amount_req,ui->Enter_amount_edit->text().trimmed().toAscii().data(),strlen(ui->Enter_amount_edit->text().trimmed().toAscii().data()));
    memcpy((char *)bill.FTL_first_transactionType_req,"DMTBILLPAY1",11);
    memcpy((char *)bill.FTL_first_creditDebitFlag_req,"C",1);
    memcpy((char *)bill.FTL_first_remitterMobile_req,FC_MobileNumber,10);
    sprintf(IMPS_SupportData,"SPROD#2~PVTXNID#%s~PVTXNDT#%s~IPADDR#%s~DEVICEID#%s~APPID#%s~AUTHID#@authid~LOCATION#~CELLID#%s~MCC#0,0,0,0~RPTFLG#0~PRTXNID#%s~PRTXNAMT#%s~SPLTSEQ#0~CHGAMT#0~ZRFUT1#%s~ZRFUT2#%s~ZRFUT3#%s~ZRFUT4#@traceId~ZRFUT5#%s,%s~ZRFUT6#~ZRFUT7#~ZRFUT8#~ZRFUT9#@txnId~ZRFUT10#~ZRFUN1#~ZRFUN2#~ZRFUN3#~ZRFUN4#%s~ZRFUNDT#",(char *)sd_deviceinfo.ucSupportData_PVTXNID_req,(char *)sd_deviceinfo.ucSupportData_PVTXNDT_req,(char *)sd_deviceinfo.ucSupportData_IPADDR_req,(char *)sd_deviceinfo.ucSupportData_DEVICEID_req,CLIENT_ID,sd_deviceinfo.ucSupportData_CELLID_req,(char *)sd_deviceinfo.ucSupportData_PRTXNID_req,bill.FTL_first_amount_req,bill.BD_BillerId_req,Service_Provider.toAscii().data(),Category.toAscii().data(),FIELD_ID[0],bill.BD_Authenticator1_req,bill.AB_mobileno_req);//Spilt previoustnxs ,prviousdate
    memcpy((char *)bill.FTL_first_supportdata_req,IMPS_SupportData,sizeof(IMPS_SupportData));

    strcpy(IMPS_SupportData,nullbuff);
    memcpy((char *)bill.FTL_second_supportdata_req,IMPS_SupportData,sizeof(IMPS_SupportData));
    memcpy((char *)bill.FTL_second_amount_req,ui->Enter_amount_edit->text().trimmed().toAscii().data(),strlen(ui->Enter_amount_edit->text().trimmed().toAscii().data()));
    memcpy((char *)bill.FTL_second_transactionType_req,"DMTBILLPAY1",11);
    memcpy((char *)bill.FTL_second_creditDebitFlag_req,"D",1);
    memcpy((char *)bill.FTL_second_remitterMobile_req,FC_MobileNumber,10);



    json_creation(0,gSendData);
    qDebug()<<"1";
#if DEBUG
    printf("gSendData = %s\n\n",gSendData);
#endif

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
    strcpy(str_url,URL_CREATION((char *)BILL_PAYMENT_URL));
#endif

    int retk = file_download(str_url, (char *)gSendData,(char *)"/usr/FINO/FPB/RESP/OUTPUT.TXT",120);
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
    system("cd /usr/FINO/FPB/RESP/");
    system("rm /usr/FINO/FPB/RESP/BILL_CHARGES.TXT");
    int gRecv_len = strlen((char *)oBuff);
    FILE *fp2 = NULL;
    fp2 = fopen("/usr/FINO/FPB/RESP/BILL_CHARGES.TXT","w");
    if(fp2 == NULL)
    {
        return FUNCTION_FAILED;
    }
    qDebug()<<"File"<<fp2;
    fwrite( oBuff, 1, gRecv_len, fp2 );
    fclose(fp2);
    int iRetparse = Bill_Charges_parsing();
    if(iRetparse==FUNC_OK)
    {
        bool decimal;
        Charges.clear();
        Total_Amount.clear();
        Amount.clear();
        Charges = QString::fromUtf8((const char*)bill.ucamount[2]);
        memset(billdata.ucCharges,0,strlen(billdata.ucCharges));
        strcpy(billdata.ucCharges,bill.ucamount[2]);

        QString Amount_dot = ui->Enter_amount_edit->text();
        Amount = Amount_dot.mid(0, Amount_dot.indexOf("."));

        qDebug()<<"Amount"<<Amount;
        int temp = Amount.toInt() + Charges.toInt();
        Total_Amount = QString::number(temp);
        memset(billdata.ucTotal_Amount,0,strlen(billdata.ucTotal_Amount));
        strcpy(billdata.ucTotal_Amount,Total_Amount.toAscii().data());
        decimal = Charges.contains('.');
        if(decimal == false)
        {
            Charges = Charges.append(".00");
        }
        decimal = Total_Amount.contains('.');
        if(decimal == false)
        {
            Total_Amount = Total_Amount.append(".00");
        }
        ui->Calculate_charges_button->setText("CHARGES ="+Charges);
        ui->Calculate_charges_button->setEnabled(false);
        ui->Calculate_charges_button->setStyleSheet("background-color: rgb(85, 85, 127); color: rgb(255, 255, 255)");
        ui->Total_amount_display->setText(Total_Amount);
        Calculate_charges = 1;
        uidmessage::instance()->hide_Message();
        return FUNCTION_SUCCESS;

    }
    else
    {

        if(strcmp((char *)bill.ucResponseMswssage,"Session Expired.")==0)
        {
            uidmessage::instance()->hide_Message();
            sprintf(ErrorMsg,"%s",(char *)bill.ucResponseMswssage);
            miscObj.DisplayMsgBox(ErrorMsg);
            uidmessage::instance()->hide_Message();
            return SESSION_EXPIRED_FAILED;
        }
        sprintf(ErrorMsg,"%s",(char *)bill.ucResponseMswssage);
        miscObj.DisplayMsgBox(ErrorMsg);
        uidmessage::instance()->hide_Message();
        return FUNCTION_FAILED;

    }

    return FUNCTION_SUCCESS;

}


void FPB::on_Bill_Back_Button_3_clicked()
{
    ui->BharatBill_Logo->hide();
    ui->BharatBill_Logo_confirm->hide();
    ui->stackedWidget->setCurrentIndex(15);
    return;
}


void FPB::on_Bill_payment_back_button_clicked()
{
    ui->first_editline->clear();
    ui->second_editline->clear();
    ui->third_editline->clear();
    ui->Enter_amount_edit->clear();
    ui->BharatBill_Logo->hide();
    ui->Calculate_charges_button->setText("Calculate Charges");
    ui->Calculate_charges_button->setEnabled(true);
    ui->Calculate_charges_button->setStyleSheet("background-color: rgb(85, 0, 127); color: rgb(255, 255, 255)");
    ui->Bill_service_provider->clear();
    ui->Total_amount_display->clear();
    ui->stackedWidget->setCurrentIndex(37);
    return;
}

int FPB::Bill_Charges_parsing()
{

    system("sed 's/\"ResponseData\":\"/\"ResponseData\":/g' /usr/FINO/FPB/RESP/BILL_CHARGES.TXT > /tmp/1.txt");
    system("sed 's/\",\"MessageId\"/,\"MessageId\"/g' /tmp/1.txt > /tmp/2.txt  ");
    system(" sed 's/\\\\//g' /tmp/2.txt > /usr/FINO/FPB/RESP/BILL_CHARGES.TXT");


    Json1::Value root;
    root.clear();
    //QSqlQuery query;
    Json1::Reader reader;
    std::ifstream ReadData("/usr/FINO/FPB/RESP/BILL_CHARGES.TXT");
    bool parsedSuccess = reader.parse(ReadData,root,false);
    ReadData.close();
    if(not parsedSuccess)
    {
        qDebug()<<"FpsMonthlyPdsData File not Found";
        return FUNCAL_FAILED;
    }
    else
    {
        strcpy((char*)bill.ucResponseCode,Convertstr(root["ResponseCode"].asString()));
        strcpy((char*)bill.ucResponseMswssage,Convertstr(root["ResponseMessage"].asString()));
        strcpy((char*)bill.ucDisplayMswssage,Convertstr(root["DisplayMessage"].asString()));
        if(strcmp((char *)bill.ucResponseCode,"0")==0)
        {
            qDebug()<<"After Condition Success";
            Json1::Value ResponseData;
            ResponseData.clear();
            ResponseData = root.get("ResponseData",0);
            printf("\nSize : %d\n",ResponseData.size());
            if(ResponseData.size()==0)
            {
                qDebug("\n1:Key Register Data Not Found\n");
                return FUNCAL_FAILED;
            }
            else
            {
                Json1::Value ResponseData2;
                ResponseData2.clear();
                for(int j=0;j<ResponseData.size();j++)
                {
                    qDebug()<<"returnCode:"<<Convertstr(ResponseData[j]["returnCode"].asString());
                    qDebug()<<"in  chargesList";
                    ResponseData2 = ResponseData[j].get("chargesList",0);
                    for(int i = 0;i<ResponseData2.size();i++)
                    {
                        strcpy((char*)bill.uccode[i],Convertstr(ResponseData2[i]["code"].asString()));
                        strcpy((char*)bill.ucamount[i],Convertstr(ResponseData2[i]["amount"].asString()));
                        strcpy((char*)bill.ucdescription[i],Convertstr(ResponseData2[i]["description"].asString()));
                        strcpy((char*)bill.ucchargeAcctNum[i],Convertstr(ResponseData2[i]["chargeAcctNum"].asString()));
                        printf("ucchargeAcctNum%s",bill.ucchargeAcctNum[i]);
                        qDebug()<<"code:"<<Convertstr(ResponseData2[i]["code"].asString());
                        qDebug()<<"amount:"<<Convertstr(ResponseData2[i]["amount"].asString());
                        qDebug()<<"description:"<<Convertstr(ResponseData2[i]["description"].asString());
                        qDebug()<<"chargeAcctNum:"<<Convertstr(ResponseData2[i]["chargeAcctNum"].asString());

                    }
                }


            }
        }
        else
        {
            return FUNC_ERROR;
        }
    }

    return FUNC_OK;
}


int FPB::PostBillPayment()
{
    char szDate[16], szTime[16];
    uidmessage::instance()->hide_Message();
    uidmessage::instance()->Display_BusyMessage("Please Wait...");
    char nullbuff[5];
    char ErrorMsg[150];
    memset(ErrorMsg,0,sizeof(ErrorMsg));
    QSqlQuery query;
    char IMPS_SupportData[1000];
    CMisc miscObj;
    memset(nullbuff, 0, sizeof(nullbuff));
    strcpy(nullbuff,"");

    memset(&bill,0,sizeof(bill));

    Clear_DeviceInfo();
    int iretDI = DeviceInfo();

    if(iretDI != FUNCTION_SUCCESS)
        return FUNCTION_FAILED;

    iGenerateRequestID((char *)username,(unsigned char *)bill.ucRequestId_req);
    bill.ucMethodId = 38;
    memcpy(bill.ucTellerId_req,username,sizeof(username));
    memcpy((char *)bill.ucSessionId_req,configLoginLeg2.ucSessionId,strlen((char *)configLoginLeg2.ucSessionId));
    memcpy((char *)bill.ucIsEncrypt_req,"false",5);
    bill.ucChannelID_req =3;
    memcpy((char *)bill.ucSessionExpiryTime_req,(char *)configLoginLeg2.ucSessionExpiryTime,strlen((char *)configLoginLeg2.ucSessionExpiryTime));


    memcpy((char *)bill.Req_asyncFlag_req,"N",1);
    memcpy((char *)bill.Req_amount_req,billdata.ucTotal_Amount,strlen(billdata.ucTotal_Amount));
    memcpy((char *)bill.Req_mobileNumber_req,FC_MobileNumber,10);
    memcpy((char *)bill.Req_cashoraccount_req,"CASH",4);
    memcpy((char *)bill.Req_txnType_req,"BILLPAY",7);
    memcpy((char *)bill.Req_impsserviceflag_req,"N",1);
    memcpy((char *)bill.Req_requestbitmap_req,"00000100",8);
    memcpy((char *)bill.Req_merchantMobileNumber_req,(char *)configLoginLeg2.ucmobileNumber,strlen((char *)configLoginLeg2.ucSessionId));

    memcpy((char *)bill.CA_MobileNo_req,FC_MobileNumber,10);
    bill.CA_Auth_id_req = 0;




    memcpy((char *)bill.AB_name_req,(char *)FINDCUST.ucRDCLfullName_resp,strlen((char *)FINDCUST.ucRDCLfullName_resp));
    memcpy((char *)bill.AB_mobileno_req,FC_MobileNumber,10);
    memcpy((char *)bill.AB_address_req,(char *)FINDCUST.ucRDCMAaddressLine1,strlen((char *)FINDCUST.ucRDCMAaddressLine1));

    memcpy((char *)bill.BD_UserID_req,FC_MobileNumber,10);
    memcpy((char *)bill.BD_CustomerID_req,FC_MobileNumber,10);
    memcpy((char *)bill.BD_CustomerMobNo_req,FC_MobileNumber,10);

    Biller_code = QString::fromLatin1((char *)billdata.ucBillercode);
    qDebug()<<"Biller_code"<<Biller_code;
    qDebug()<<"Biller_code"<<Biller_code;
    qDebug()<<"Biller_code"<<Biller_code;
    qDebug()<<"Biller_code"<<Biller_code;
    qDebug()<<"Biller_code"<<Biller_code;

    query.clear();
    query.prepare("SELECT  BILLER__ID FROM BILLER_DETAILS WHERE BILLER__CODE  = ? ");
    query.addBindValue(Biller_code);
    if(!query.exec())
        qDebug() << query.lastError();
    else
    {
        while(query.next())
        {
           strcpy((char *)bill.BD_BillerId_req,query.value(0).toString().trimmed().toLatin1().data());
           printf("bill.ucBillercode%s",bill.BD_BillerId_req);
        }
    }



  //  memcpy((char *)bill.BD_BillerId_req,"IGLOB",5);
    memcpy((char *)bill.BD_PaymentAmount_req,ui->Enter_amount_edit->text().trimmed().toAscii().data(),strlen(ui->Enter_amount_edit->text().trimmed().toAscii().data()));
    memcpy((char *)bill.BD_Authenticator1_req,ui->first_editline->text().trimmed().toAscii().data(),strlen(ui->first_editline->text().trimmed().toAscii().data()));
    if(count >=2)
    {
        memcpy((char *)bill.BD_Authenticator2_req,ui->second_editline->text().trimmed().toAscii().data(),strlen(ui->second_editline->text().trimmed().toAscii().data()));
    }
    if(count>=3)
    {
        memcpy((char *)bill.BD_Authenticator3_req,ui->third_editline->text().trimmed().toAscii().data(),strlen(ui->third_editline->text().trimmed().toAscii().data()));

    }

    memcpy((char *)bill.PT_appId_req,CLIENT_ID,strlen(CLIENT_ID));
    bill.PT_analysisFlag_req = 0;
    bill.PT_isInclusive_req =0;
    bill.PT_isClubbed_req =0;


    memcpy((char *)bill.FTL_currency_req,"INR",3);
    memcpy((char *)bill.FTL_costCenter_req,configLoginLeg2.ucBAcode,strlen(configLoginLeg2.ucBAcode));

    memset(IMPS_SupportData,0,strlen(IMPS_SupportData));
    memcpy((char *)bill.FTL_first_amount_req,billdata.ucTotal_Amount,strlen(billdata.ucTotal_Amount));
     memcpy((char *)bill.FTL_first_transactionType_req,"DMTCASHD",8);
     memcpy((char *)bill.FTL_first_creditDebitFlag_req,"C",1);
     memcpy((char *)bill.FTL_first_remitterMobile_req,FC_MobileNumber,10);
    sprintf(IMPS_SupportData,"SPROD#5~PVTXNID#%s~PVTXNDT#%s~IPADDR#%s~DEVICEID#%s~APPID#%s~AUTHID#@authid~LOCATION#~CELLID#%s~MCC#0,0,0,0~RPTFLG#0~PRTXNID#%s~PRTXNAMT#%s~SPLTSEQ#0~CHGAMT#0~ZRFUN3#~ZRFUN4#%s~ZRFUT5#%s",(char *)sd_deviceinfo.ucSupportData_PVTXNID_req,(char *)sd_deviceinfo.ucSupportData_PVTXNDT_req,(char *)sd_deviceinfo.ucSupportData_IPADDR_req,(char *)sd_deviceinfo.ucSupportData_DEVICEID_req,CLIENT_ID,sd_deviceinfo.ucSupportData_CELLID_req,(char *)sd_deviceinfo.ucSupportData_PRTXNID_req,billdata.ucTotal_Amount,FC_MobileNumber,(char *)FINDCUST.ucRDCLfullName_resp);//Spilt previoustnxs ,prviousdate
     memcpy((char *)bill.FTL_first_supportdata_req,IMPS_SupportData,sizeof(IMPS_SupportData));


     memset(IMPS_SupportData,0,strlen(IMPS_SupportData));
     memcpy((char *)bill.FTL_second_amount_req,billdata.ucTotal_Amount,strlen(billdata.ucTotal_Amount));
      memcpy((char *)bill.FTL_second_transactionType_req,"DMTCASHD",8);
      printf("bill.FTL_second_transactionType_req%s",bill.FTL_second_transactionType_req);
      printf("bill.FTL_second_transactionType_req%s",bill.FTL_second_transactionType_req);
      memcpy((char *)bill.FTL_second_creditDebitFlag_req,"D",1);
      memcpy((char *)bill.FTL_second_remitterMobile_req,FC_MobileNumber,10);
      sprintf(IMPS_SupportData,"DN1#%s",billdata.ucTotal_Amount);
      memcpy((char *)bill.FTL_second_supportdata_req,IMPS_SupportData,sizeof(IMPS_SupportData));


     memset(IMPS_SupportData,0,strlen(IMPS_SupportData));
     memcpy((char *)bill.FTL_third_amount_req,ui->Enter_amount_edit->text().trimmed().toAscii().data(),strlen(ui->Enter_amount_edit->text().trimmed().toAscii().data()));
     memcpy((char *)bill.FTL_third_transactionType_req,"DMTBILLPAY1",11);
     memcpy((char *)bill.FTL_third_creditDebitFlag_req,"D",1);
     memcpy((char *)bill.FTL_third_remitterMobile_req,FC_MobileNumber,10);
    sprintf(IMPS_SupportData,"SPROD#2~PVTXNID#%s~PVTXNDT#%s~IPADDR#%s~DEVICEID#%s~APPID#%s~AUTHID#@authid~LOCATION#~CELLID#%s~MCC#0,0,0,0~RPTFLG#0~PRTXNID#%s~PRTXNAMT#%s~SPLTSEQ#0~CHGAMT#%s~ZRFUT1#%s~ZRFUT2#%s~ZRFUT3#%s~ZRFUT4#@traceId~ZRFUT5#%s,%s~ZRFUT6#~ZRFUT7#~ZRFUT8#~ZRFUT9#@txnId~ZRFUT10#~ZRFUN1#~ZRFUN2#~ZRFUN3#~ZRFUN4#%s~ZRFUNDT#",(char *)sd_deviceinfo.ucSupportData_PVTXNID_req,(char *)sd_deviceinfo.ucSupportData_PVTXNDT_req,(char *)sd_deviceinfo.ucSupportData_IPADDR_req,(char *)sd_deviceinfo.ucSupportData_DEVICEID_req,CLIENT_ID,sd_deviceinfo.ucSupportData_CELLID_req,(char *)sd_deviceinfo.ucSupportData_PRTXNID_req,bill.FTL_third_amount_req,billdata.ucCharges,bill.BD_BillerId_req,Service_Provider.toAscii().data(),Category.toAscii().data(),FIELD_ID[0],bill.BD_Authenticator1_req,FC_MobileNumber);
    memcpy((char *)bill.FTL_third_supportdata_req,IMPS_SupportData,sizeof(IMPS_SupportData));



     memset(IMPS_SupportData,0,strlen(IMPS_SupportData));
     memcpy((char *)bill.FTL_fourth_supportdata_req,nullbuff,sizeof(nullbuff));
      memcpy((char *)bill.FTL_fourth_amount_req,ui->Enter_amount_edit->text().trimmed().toAscii().data(),strlen(ui->Enter_amount_edit->text().trimmed().toAscii().data()));
      memcpy((char *)bill.FTL_fourth_transactionType_req,"DMTBILLPAY1",11);
      memcpy((char *)bill.FTL_fourth_creditDebitFlag_req,"C",1);
      memcpy((char *)bill.FTL_fourth_remitterMobile_req,nullbuff,sizeof(nullbuff));


      json_creation(1,gSendData);
      memset(szDate, 0, sizeof(szDate));
      memset(szTime, 0, sizeof(szTime));
      GetLocalTime(szDate, szTime);
      qDebug()<<"1";
      uidmessage::instance()->hide_Message();
      uidmessage::instance()->Display_BusyMessage("Transaction Request in Progress");

    //-------------------------------------------------------------
//    qDebug()<<"1";
//    memset(gSendData,0,sizeof(gSendData));
//    strncpy((char*)gSendData, jsondata1, strlen(jsondata1));
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
    strcpy(str_url,URL_CREATION((char *)BILL_PAYMENT_URL));
#endif

    int retk = file_download(str_url, (char *)gSendData,(char *)"/usr/FINO/FPB/RESP/OUTPUT.TXT",120);
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
        system("cd /usr/FINO/FPB/RESP/");
        system("rm /usr/FINO/FPB/RESP/BILL_CHARGES.TXT");
        int gRecv_len = strlen((char *)oBuff);
        FILE *fp2 = NULL;
        fp2 = fopen("/usr/FINO/FPB/RESP/OUTPUT.TXT","w");
        if(fp2 == NULL)
        {
            return FUNCTION_FAILED;
        }
        qDebug()<<"File"<<fp2;
        fwrite( oBuff, 1, gRecv_len, fp2 );
        fclose(fp2);
        int iRetparse = Post_bill_parsing();
        if(iRetparse==FUNC_OK)
        {
            uidmessage::instance()->hide_Message();
            uidmessage::instance()->Display_BusyMessage("Bill Payment Transaction Success");
            sleep(5);



            display_flag = 1;
            char display_confirmpage[11][120];
            sprintf(display_confirmpage[0],"Category             : %s",Category.toAscii().data());
            sprintf(display_confirmpage[1],"Payment through      : Visiontek");
            sprintf(display_confirmpage[2],"Payment mode         : Account Transfer");
            sprintf(display_confirmpage[3],"Service Provider     : %s",Service_Provider.toAscii().data());
            sprintf(display_confirmpage[4],"%-21s: %s",FIELD_NAME[0],first_input.toAscii().data());
            sprintf(display_confirmpage[5],"Bill Amount          : %s",Amount.toAscii().data());
            sprintf(display_confirmpage[6],"Cust. Convenience Fee: %s",Charges.toAscii().data());
            sprintf(display_confirmpage[7],"Total Amount         : %s",Total_Amount.toAscii().data());
            sprintf(display_confirmpage[8],"Cash Contribution    : %s",Total_Amount.toAscii().data());
            sprintf(display_confirmpage[9],"Transaction ID       : %s",bill.txnReferenceNo[1]);
            sprintf(display_confirmpage[10],"Consumer Number      : %s",first_input.toAscii().data());
            sprintf(display_confirmpage[11],"Status              : Success");
            qDebug()<<"Success in the parse";
            int i=0;
            qDebug()<<"Display";
            ui->Confirmation_listwidget->clear(); // clear list
            ui->Confirmation_listwidget->setFont(QFont("Courier",12));

            for(i=0;i<12;i++)
            {
#if DEBUG
                printf("display_confirmpage    =====    %s\n",display_confirmpage[i]);
#endif
                ui->Confirmation_listwidget->addItem((char *)display_confirmpage[i]);
            }



            ui->Confirmation_back_button->setVisible(false);
            ui->Confirmation_next_button->setText("Finish");


            char iquery[500];
            query.clear();

            query.exec("delete from LastWithdrawalData");
            query.clear();
            memset(iquery,0,sizeof(iquery));
            sprintf(iquery, "INSERT INTO LastWithdrawalData (LAST_RRN, LAST_DATE) VALUES('%s','%s')", bill.txnReferenceNo[1],szDate);
    #if DEBUG
            printf("QUERY 1 === %s\n",iquery);
    #endif
            if(!query.exec(iquery))
            {
                qDebug() << "In Insert_LastWithdrawalData = " << query.lastError();
                query.clear();
                query.finish();
            }
            query.clear();
            query.finish();






            QString bal = QString::fromUtf8((const char*)(const char*)bill.BAavailableBalance[0]);
            bool decimal;
            decimal = bal.contains('.');
            if(decimal == false)
            {
                bal = bal.append(".00");
            }
            memset(Balance_Display,0,strlen(Balance_Display));
            sprintf(Balance_Display,"Balance : %s\n",bal.toLatin1().data());
            strncpy(Mer_Latest_Balance, bal.toLatin1().data(),strlen(bal.toLatin1().data()));
            ui->Dash_Latest_Balance_Label_pg12->setText(Balance_Display);
            ui->SubTransaction_Label1->setText((char *)configLoginLeg2.ucname);
            ui->SubTransaction_Label1->setFont(QFont("SansSerif",10));

            ui->SubTransaction_Label1->setAlignment(Qt::AlignRight | Qt::AlignBottom);
            printf("Balance_Display = %s\n",Balance_Display);
            ui->SubTransaction_Label2->setText(QString::fromUtf8((const char*)Balance_Display));
            ui->SubTransaction_Label2->setFont(QFont("SansSerif",9));
            ui->SubTransaction_Label2->setAlignment(Qt::AlignRight | Qt::AlignBottom);
            uidmessage::instance()->hide_Message();

            return FUNCTION_SUCCESS;

        }
        else
        {

            if(strcmp((char *)bill.ucResponseMswssage,"Session Expired.")==0)
            {
                uidmessage::instance()->hide_Message();
                sprintf(ErrorMsg,"%s",(char *)bill.ucResponseMswssage);
                miscObj.DisplayMsgBox(ErrorMsg);
                uidmessage::instance()->hide_Message();
                return SESSION_EXPIRED_FAILED;
            }
            sprintf(ErrorMsg,"%s",(char *)bill.ucResponseMswssage);
            miscObj.DisplayMsgBox(ErrorMsg);
            uidmessage::instance()->hide_Message();
            return FUNCTION_FAILED;

        }

        return FUNCTION_SUCCESS;



      //qDebug()<<"------------->outside jsoncreation"<<jsondata1;
    return 0;
}




void FPB::on_Bill_payment_Next_button_2_clicked()
{
    qDebug()<<"count1"<<count;
    CMisc miscObj;

   char msgdisplay[50];
    if(count>=1)
    {
        qDebug()<<"Inside 1";
        qDebug()<<"Value"<<ui->first_label->text().isEmpty();
        if (ui->first_editline->text().isEmpty())
        {
            qDebug()<<"Inside 2";

            sprintf(msgdisplay,"%s cannot be Blank",FIELD_NAME[0]);
            miscObj.DisplayMsgBox(msgdisplay);
            return;
        }
        printf("FIELD_LENGTH%d\n",FIELD_LENGTH[0]);
        if (ui->first_editline->text().length()<(FIELD_LENGTH[0] -1))
        {
            qDebug()<<"Inside 3";

            sprintf(msgdisplay,"%s should be greater than %d digits",FIELD_NAME[0],FIELD_LENGTH[0]);
            miscObj.DisplayMsgBox(msgdisplay);
            return;
        }
    }

    if(count>=2)
    {
        if(memcmp(FIELD_ID[1],"109",3)==0)
        {
            if (ui->second_editline->text().isEmpty())
            {
                sprintf(msgdisplay,"%s cannot be Blank",FIELD_NAME[1]);
                miscObj.DisplayMsgBox(msgdisplay);
                return;
            }
            if (ui->second_editline->text().length()>(FIELD_LENGTH[1] -1))
            {
                sprintf(msgdisplay,"%s should be greater than %d digits",FIELD_NAME[1],FIELD_LENGTH[1]);
                miscObj.DisplayMsgBox(msgdisplay);
                return;
            }
        }

    }
    if(count>=3)
    {
        if (ui->third_editline->text().isEmpty())
        {
            sprintf(msgdisplay,"%s cannot be Blank",FIELD_NAME[2]);
            miscObj.DisplayMsgBox(msgdisplay);
            return;
        }
        if (ui->third_editline->text().length()>(FIELD_LENGTH[2] -1))
        {
            sprintf(msgdisplay,"%s should be greater than %d digits",FIELD_NAME[2],FIELD_LENGTH[2]);
            miscObj.DisplayMsgBox(msgdisplay);
            return;
        }
    }

    if(Calculate_charges == 0)
    {

        sprintf(msgdisplay,"Please Calculate Charges");
        miscObj.DisplayMsgBox(msgdisplay);
        return;
    }
    int i=0,llop=0;
    if(Bharatpaylogo == "true")
    {
        qDebug()<<"true";
        ui->BharatBill_Logo_confirm->setVisible(true);
        llop =9;
    }
    else if(Bharatpaylogo == "false")
    {
        qDebug()<<"false";

        ui->BharatBill_Logo_confirm->setVisible(false);
        llop =9;

    }

    first_input = ui->first_editline->text();
    Amount = ui->Enter_amount_edit->text();
    display_flag = 0;
    char display_confirmpage[10][120];
    sprintf(display_confirmpage[0],"Category             : %s",Category.toAscii().data());
    sprintf(display_confirmpage[1],"Payment through      : Visiontek");
    sprintf(display_confirmpage[2],"Payment mode         : Account Transfer");
    sprintf(display_confirmpage[3],"Service Provider     : %s",Service_Provider.toAscii().data());
     sprintf(display_confirmpage[4],"%-21s: %s",FIELD_NAME[0],first_input.toAscii().data());
     sprintf(display_confirmpage[5],"Bill Amount          : %s",Amount.toAscii().data());
     sprintf(display_confirmpage[6],"Cust. Convenience Fee: %s",Charges.toAscii().data());
     sprintf(display_confirmpage[7],"Total Amount         : %s",Total_Amount.toAscii().data());
     sprintf(display_confirmpage[8],"Cash Contribution    : %s",Total_Amount.toAscii().data());
//    if(Bharatpaylogo == "true")
//    {
//    sprintf(display_confirmpage[4],"                         ");
//    sprintf(display_confirmpage[5],"                         ");
//    sprintf(display_confirmpage[6],"%-21s: %s",FIELD_NAME[0],first_input.toAscii().data());
//    sprintf(display_confirmpage[7],"Bill Amount          : %s",Amount.toAscii().data());
//    sprintf(display_confirmpage[8],"Cust. Convenience Fee: %s",Charges.toAscii().data());
//    sprintf(display_confirmpage[9],"Total Amount         : %s",Total_Amount.toAscii().data());
//    sprintf(display_confirmpage[10],"Cash Contribution    : %s",Total_Amount.toAscii().data());
//    }
//    else
//    {
//        sprintf(display_confirmpage[4],"%-21s: %s",FIELD_NAME[0],first_input.toAscii().data());
//        sprintf(display_confirmpage[5],"Bill Amount          : %s",Amount.toAscii().data());
//        sprintf(display_confirmpage[6],"Cust. Convenience Fee: %s",Charges.toAscii().data());
//        sprintf(display_confirmpage[7],"Total Amount         : %s",Total_Amount.toAscii().data());
//        sprintf(display_confirmpage[8],"Cash Contribution    : %s",Total_Amount.toAscii().data());
//    }



    qDebug()<<"Display";
    ui->Confirmation_listwidget->clear(); // clear list
    ui->Confirmation_listwidget->setFont(QFont("Courier",12));

    for(i=0;i<llop;i++)
    {
#if DEBUG
        printf("display_confirmpage    =====    %s\n",display_confirmpage[i]);
#endif
        ui->Confirmation_listwidget->addItem((char *)display_confirmpage[i]);

    }

    ui->stackedWidget->setCurrentIndex(39);

    return;
}


int FPB::Post_bill_parsing()
{

    system("sed 's/\"ResponseData\":\"/\"ResponseData\":/g' /usr/FINO/FPB/RESP/OUTPUT.TXT > /tmp/1.txt");
    system("sed 's/\",\"MessageId\"/,\"MessageId\"/g' /tmp/1.txt > /tmp/2.txt  ");
    system(" sed 's/\\\\//g' /tmp/2.txt > /usr/FINO/FPB/RESP/POST_BILL.TXT");


    Json1::Value root;
    root.clear();
    //QSqlQuery query;
    Json1::Reader reader;
    std::ifstream ReadData("/usr/FINO/FPB/RESP/POST_BILL.TXT");
    bool parsedSuccess = reader.parse(ReadData,root,false);
    ReadData.close();
    if(not parsedSuccess)
    {

        qDebug()<<"FpsMonthlyPdsData File not Found";
        return FUNCAL_FAILED;
    }
    else
    {
        strcpy((char*)bill.ucResponseCode,Convertstr(root["ResponseCode"].asString()));
        strcpy((char*)bill.ucResponseMswssage,Convertstr(root["ResponseMessage"].asString()));
        strcpy((char*)bill.ucDisplayMswssage,Convertstr(root["DisplayMessage"].asString()));
        if(strcmp((char *)bill.ucResponseCode,"0")==0)
        {
            qDebug()<<"After Condition Success";
            Json1::Value ResponseData;
            ResponseData.clear();
            ResponseData = root.get("ResponseData",0);
            printf("\nSize : %d\n",ResponseData.size());
            if(ResponseData.size()==0)
            {
                qDebug("\n1:Key Register Data Not Found\n");
                return FUNCAL_FAILED;
            }
            else
            {
                Json1::Value ResponseData1,ResponseData2,BillDeskDataResponse,ValidationResponse,PaymentResponse;
                ResponseData2.clear();

                for(int j=0;j<ResponseData.size();j++)
                {
                    qDebug()<<"The loop value"<<j;
                    qDebug()<<"returnCode:"<<Convertstr(ResponseData[j]["returnCode"].asString());
                    qDebug()<<"txnReferenceNo:"<<Convertstr(ResponseData[j]["txnReferenceNo"].asString());
                    qDebug()<<"cbsTxnReferenceNo:"<<Convertstr(ResponseData[j]["cbsTxnReferenceNo"].asString());
                    strcpy((char*)bill.ucreturncode[j],Convertstr(ResponseData[j]["returnCode"].asString()));
                    strcpy((char*)bill.txnReferenceNo[j],Convertstr(ResponseData[j]["txnReferenceNo"].asString()));
                    strcpy((char*)bill.cbsTxnReferenceNo[j],Convertstr(ResponseData[j]["cbsTxnReferenceNo"].asString()));

                    ResponseData2.clear();
                    ResponseData1 = ResponseData[j].get("balancesList",0);
                    printf("\nSize : %d\n",ResponseData1.size());
                    for(int k=0;k<ResponseData1.size();k++)
                    {
                        qDebug()<<"ledgerBalance:"<<Convertstr(ResponseData1[k]["ledgerBalance"].asString());
                        qDebug()<<"availableBalance:"<<Convertstr(ResponseData1[k]["availableBalance"].asString());
                        qDebug()<<"currency:"<<Convertstr(ResponseData1[k]["currency"].asString());
                        strcpy((char*)bill.BAledgerBalance[k],Convertstr(ResponseData1[k]["ledgerBalance"].asString()));
                        strcpy((char*)bill.BAavailableBalance[k],Convertstr(ResponseData1[k]["availableBalance"].asString()));
                        strcpy((char*)bill.BAcurrency[k],Convertstr(ResponseData1[k]["currency"].asString()));

                    }
                    ResponseData2.clear();
                    if(j==1)
                    {
                        ResponseData2 = ResponseData[j].get("chargesList",0);
                        for(int k=0;k<ResponseData2.size();k++)
                        {
                            qDebug()<<"code:"<<Convertstr(ResponseData2[k]["code"].asString());
                            qDebug()<<"amount:"<<QString::number(ResponseData2[k]["amount"].asFloat());
                            qDebug()<<"description:"<<Convertstr(ResponseData2[k]["description"].asString());
                            qDebug()<<"chargeAcctNum:"<<Convertstr(ResponseData2[k]["chargeAcctNum"].asString());
                            strcpy((char*)bill.uccode[k],Convertstr(ResponseData2[k]["code"].asString()));
                            strcpy((char*)bill.ucamount[k],Convertstr(ResponseData2[k]["amount"].asString()));
                            strcpy((char*)bill.ucdescription[k],Convertstr(ResponseData2[k]["description"].asString()));
                            strcpy((char*)bill.ucchargeAcctNum[k],Convertstr(ResponseData2[k]["chargeAcctNum"].asString()));

                        }
                    }
                     BillDeskDataResponse = ResponseData[j].get("BillDeskDataResponse",0);
                     ValidationResponse = BillDeskDataResponse.get("ValidationResponse",0);
                     qDebug()<<"MessageCode:"<<Convertstr(ValidationResponse["MessageCode"].asString());
                     qDebug()<<"TraceId:"<<Convertstr(ValidationResponse["TraceId"].asString());
                     qDebug()<<"SourceID:"<<Convertstr(ValidationResponse["SourceID"].asString());
                     qDebug()<<"TimeStamp:"<<Convertstr(ValidationResponse["TimeStamp"].asString());
                     qDebug()<<"UserID:"<<Convertstr(ValidationResponse["UserID"].asString());
                     qDebug()<<"CustomerID:"<<Convertstr(ValidationResponse["CustomerID"].asString());
                     qDebug()<<"Valid:"<<Convertstr(ValidationResponse["Valid"].asString());
                     qDebug()<<"ErrorCode:"<<Convertstr(ValidationResponse["ErrorCode"].asString());
                     qDebug()<<"ErrorMessage:"<<Convertstr(ValidationResponse["ErrorMessage"].asString());
                     qDebug()<<"BillerId:"<<Convertstr(ValidationResponse["BillerId"].asString());
                     qDebug()<<"Authenticator1:"<<Convertstr(ValidationResponse["Authenticator1"].asString());
                     qDebug()<<"Authenticator2:"<<Convertstr(ValidationResponse["Authenticator2"].asString());
                     qDebug()<<"Authenticator3:"<<Convertstr(ValidationResponse["Authenticator3"].asString());
                     qDebug()<<"BillNumber:"<<Convertstr(ValidationResponse["BillNumber"].asString());
                     qDebug()<<"BillDate:"<<Convertstr(ValidationResponse["BillDate"].asString());
                     qDebug()<<"BillDueDate:"<<Convertstr(ValidationResponse["BillDueDate"].asString());
                     qDebug()<<"BillAmount:"<<Convertstr(ValidationResponse["BillAmount"].asString());
                     qDebug()<<"PayWithOutBill:"<<Convertstr(ValidationResponse["PayWithOutBill"].asString());
                     qDebug()<<"PartialPayment:"<<Convertstr(ValidationResponse["PartialPayment"].asString());
                     qDebug()<<"Filler1:"<<Convertstr(ValidationResponse["Filler1"].asString());
                     qDebug()<<"Filler2:"<<Convertstr(ValidationResponse["Filler2"].asString());
                     qDebug()<<"Filler3:"<<Convertstr(ValidationResponse["Filler3"].asString());
                     qDebug()<<"Checksum:"<<Convertstr(ValidationResponse["Checksum"].asString());

//                     strcpy((char*)bill.VRMessageCode[j],Convertstr(ValidationResponse["MessageCode"].asString()));
//                     strcpy((char*)bill.VRTraceId[j],Convertstr(ValidationResponse["TraceId"].asString()));
//                     strcpy((char*)bill.VRCustomerID[j],Convertstr(ValidationResponse["CustomerID"].asString()));
//                     strcpy((char*)bill.VRValid[j],Convertstr(ValidationResponse["Valid"].asString()));
//                     strcpy((char*)bill.VRBillerId[j],Convertstr(ValidationResponse["BillerId"].asString()));
//                     strcpy((char*)bill.VRAuthenticator1[j],Convertstr(ValidationResponse["Authenticator1"].asString()));
//                     strcpy((char*)bill.VRPayWithOutBill[j],Convertstr(ValidationResponse["PayWithOutBill"].asString()));
//                     strcpy((char*)bill.VRPartialPayment[j],Convertstr(ValidationResponse["artialPayment"].asString()));
//                     strcpy((char*)bill.VRChecksum[j],Convertstr(ValidationResponse["Checksum"].asString()));

                    strcpy((char*)bill.VRreason[j],Convertstr(ValidationResponse["Reason"].asString()));



                     PaymentResponse = BillDeskDataResponse.get("PaymentResponse",0);
                     qDebug()<<"MessageCode:"<<Convertstr(PaymentResponse["MessageCode"].asString());
                     qDebug()<<"TraceId:"<<Convertstr(PaymentResponse["TraceId"].asString());
                     qDebug()<<"SourceID:"<<Convertstr(PaymentResponse["SourceID"].asString());
                     qDebug()<<"TimeStamp:"<<Convertstr(PaymentResponse["TimeStamp"].asString());
                     qDebug()<<"UserID:"<<Convertstr(PaymentResponse["UserID"].asString());
                     qDebug()<<"CustomerID:"<<Convertstr(PaymentResponse["CustomerID"].asString());
                     qDebug()<<"Valid:"<<Convertstr(PaymentResponse["Valid"].asString());
                     qDebug()<<"ErrorCode:"<<Convertstr(PaymentResponse["ErrorCode"].asString());
                     qDebug()<<"ErrorMessage:"<<Convertstr(PaymentResponse["ErrorMessage"].asString());
                     qDebug()<<"TransactionId:"<<Convertstr(PaymentResponse["TransactionId"].asString());
                     qDebug()<<"PaymentStatus:"<<Convertstr(PaymentResponse["PaymentStatus"].asString());
                     qDebug()<<"Reason:"<<Convertstr(PaymentResponse["Reason"].asString());
                     qDebug()<<"Filler1:"<<Convertstr(PaymentResponse["Filler1"].asString());
                     qDebug()<<"Filler2:"<<Convertstr(PaymentResponse["Filler2"].asString());
                     qDebug()<<"Filler3:"<<Convertstr(PaymentResponse["Filler3"].asString());
                     qDebug()<<"Checksum:"<<Convertstr(PaymentResponse["Checksum"].asString());



//                     strcpy((char*)bill.PRTransactionId[j],Convertstr(PaymentResponse["TransactionId"].asString()));
//                     strcpy((char*)bill.PRPaymentStatus[j],Convertstr(PaymentResponse["PaymentStatus"].asString()));


                }
            }
        }
        else
        {
            return FUNC_ERROR;
        }
    }
    return FUNC_OK;
}

void FPB::on_Confirmation_next_button_clicked()
{
        int iRet =-2;
        if(display_flag == 1)
        {

            display_flag = 0;
            ui->Confirmation_back_button->setVisible(true);
            ui->Confirmation_next_button->setText("Next");
            ui->stackedWidget->setCurrentIndex(15);
            return;
        }
        iRet = PostBillPayment();
        uidmessage::instance()->hide_Message();
        if(iRet != FUNCTION_SUCCESS)
        {
            if(iRet == SESSION_EXPIRED_FAILED)
                ui->stackedWidget->setCurrentIndex(0);
            else
                ui->stackedWidget->setCurrentIndex(12);
            return;
        }
        else if(iRet == FUNCTION_SUCCESS)
        {
            ui->stackedWidget->setCurrentIndex(39);
            return;
        }


}


void FPB::on_Enter_amount_edit_textEdited(const QString &arg1)
{

    ui->Calculate_charges_button->setText("Calculate Charges");
    ui->Calculate_charges_button->setEnabled(true);
    ui->Calculate_charges_button->setStyleSheet("background-color: rgb(85, 0, 127); color: rgb(255, 255, 255)");
    ui->Total_amount_display->setText("       0.00");
    Calculate_charges =0;
    return;
}


void FPB::on_Fetch_bil_button_clicked()
{
    int iRet =-2;
    qDebug()<<"Fetch"<<fetch_bill;
    qDebug()<<"Fetch"<<fetch_bill;
    qDebug()<<"Fetch"<<fetch_bill;

    if(fetch_bill == 1)
    {

       ui->stackedWidget->setCurrentIndex(40);
       return;
    }


    iRet = Fetch_bill();
    uidmessage::instance()->hide_Message();
    if(iRet != FUNCTION_SUCCESS)
    {
        if(iRet == SESSION_EXPIRED_FAILED)
            ui->stackedWidget->setCurrentIndex(0);
        else
            ui->stackedWidget->setCurrentIndex(38);
        return;
    }
    else if(iRet == FUNCTION_SUCCESS)
    {
        ui->stackedWidget->setCurrentIndex(40);
        return;
    }
}


int FPB::Fetch_bill()
{

    uidmessage::instance()->hide_Message();
    uidmessage::instance()->Display_BusyMessage("Please Wait...");
    char nullbuff[5];
    char ErrorMsg[150];
    memset(ErrorMsg,0,sizeof(ErrorMsg));
    QSqlQuery query;
    CMisc miscObj;
    memset(nullbuff, 0, sizeof(nullbuff));
    strcpy(nullbuff,"");
    char * jsondata;

    memset((char *)bill.Req_asyncFlag_req,0,strlen((char *)bill.Req_asyncFlag_req));
    memset((char *)bill.ucSessionId_req,0,strlen((char *)bill.ucSessionId_req));
    memset((char *)bill.ucIsEncrypt_req,0,strlen((char *)bill.ucIsEncrypt_req));
    memset((char *)bill.ucSessionExpiryTime_req,0,strlen((char *)bill.ucSessionExpiryTime_req));
    memset((char *)bill.BD_UserID_req,0,strlen((char *)bill.BD_UserID_req));
    memset((char *)bill.BD_CustomerID_req,0,strlen((char *)bill.BD_CustomerID_req));
    memset((char *)bill.BD_BillerId_req,0,strlen((char *)bill.BD_BillerId_req));
    memset((char *)bill.BD_Authenticator1_req,0,strlen((char *)bill.BD_Authenticator1_req));
    memset((char *)bill.BD_Authenticator2_req,0,strlen((char *)bill.BD_Authenticator2_req));

    iGenerateRequestID((char *)username,(unsigned char *)bill.ucRequestId_req);
    bill.ucMethodId = 16;
    memcpy(bill.ucTellerId_req,username,sizeof(username));
    memcpy((char *)bill.ucSessionId_req,configLoginLeg2.ucSessionId,strlen((char *)configLoginLeg2.ucSessionId));
    memcpy((char *)bill.ucIsEncrypt_req,"false",5);
    bill.ucChannelID_req =3;
    memcpy((char *)bill.ucSessionExpiryTime_req,(char *)configLoginLeg2.ucSessionExpiryTime,strlen((char *)configLoginLeg2.ucSessionExpiryTime));
    memcpy((char *)bill.BD_Authenticator1_req,ui->first_editline->text().trimmed().toAscii().data(),strlen(ui->first_editline->text().trimmed().toAscii().data()));

    if(count >=2)
    {
        memcpy((char *)bill.BD_Authenticator2_req,ui->second_editline->text().trimmed().toAscii().data(),strlen(ui->second_editline->text().trimmed().toAscii().data()));
    }
    if(count>=3)
    {
        memcpy((char *)bill.BD_Authenticator3_req,ui->third_editline->text().trimmed().toAscii().data(),strlen(ui->third_editline->text().trimmed().toAscii().data()));

    }
    memcpy((char *)bill.BD_UserID_req,username,sizeof(username));
    memcpy((char *)bill.BD_CustomerID_req,FC_MobileNumber,10);
    memcpy((char *)bill.BD_BillerId_req,username,sizeof(username));

    Biller_code = QString::fromLatin1((char *)billdata.ucBillercode);
    query.clear();
    query.prepare("SELECT  BILLER__ID FROM BILLER_DETAILS WHERE BILLER__CODE  = ? ");
    query.addBindValue(Biller_code);
    if(!query.exec())
        qDebug() << query.lastError();
    else
    {
        while(query.next())
        {
           strcpy((char *)bill.BD_BillerId_req,query.value(0).toString().trimmed().toLatin1().data());
           printf("bill.ucBillercode%s",bill.BD_BillerId_req);
        }
    }
    Json1::Value RequestedData,mainreq;

    mainreq["RequestId"] =(char *) bill.ucRequestId_req;
    mainreq["MethodId"] = bill.ucMethodId;
    mainreq["TellerID"]= (char *)bill.ucTellerId_req;
    mainreq["SessionId"] =  (char *)bill.ucSessionId_req;
    mainreq["TokenId"] = "";
    mainreq["IsEncrypt"]=(char *) bill.ucIsEncrypt_req;
    mainreq["SessionExpiryTime"]= (char *)bill.ucSessionExpiryTime_req;
    mainreq["X_Auth_Token"]="";
    mainreq["ChannelID"]= bill.ucChannelID_req;
    mainreq["MerchantContribution"] = "";
    mainreq["Charges"] = "";
    mainreq["ProductCode"]= "";
    mainreq["Amount"] = "";
    mainreq["SplitReferenceNo"] = "";






    RequestedData["Authenticator1"] =(char *)bill.BD_Authenticator1_req;
    if(count>=2)
    {
        RequestedData["Authenticator2"] =(char *)bill.BD_Authenticator2_req;

    }
    if(count>=3)
    {
        RequestedData["Authenticator3"] =(char *)bill.BD_Authenticator3_req;

    }
    RequestedData["UserID"] =(char *)bill.BD_UserID_req;
    RequestedData["CustomerID"] =(char *)bill.BD_CustomerID_req;
    RequestedData["BillerId"] =(char *)bill.BD_BillerId_req;

    QString Buff = serialize1(Convertstr(RequestedData.toStyledString()));
    mainreq["RequestData"] = Buff.toAscii().data();
    jsondata = Convertstr(mainreq.toStyledString());
#if DEBUG
    qDebug()<<"------------->"<<jsondata;
#endif
    //-------------------------------------------------------------
    qDebug()<<"1";
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
    strcpy(str_url,URL_CREATION((char *)BILL_PAYMENT_URL));
#endif

    int retk = file_download(str_url, (char *)gSendData,(char *)"/usr/FINO/FPB/RESP/OUTPUT.TXT",120);
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
        int gRecv_len = strlen((char *)oBuff);
        FILE *fp2 = NULL;
        fp2 = fopen("/usr/FINO/FPB/RESP/OUTPUT.TXT","w");
        if(fp2 == NULL)
        {
            return FUNCTION_FAILED;
        }
        qDebug()<<"File"<<fp2;
        fwrite( oBuff, 1, gRecv_len, fp2 );
        fclose(fp2);
        int iRetparse = Fetch_bill_parsing();
        if(iRetparse==FUNC_OK)
        {

            QString bill_amount = QString::fromUtf8((const char*)bill.BillAmount);
            ui->Bill_amount_label->setText(bill_amount);
            QString bill_date = QString::fromUtf8((const char*)bill.BillDate);
            ui->Bill_date_label->setText(bill_date);
            QString bill_duedate = QString::fromUtf8((const char*)bill.BillDueDate);
            ui->Due_date_label->setText(bill_duedate);
            fetch_bill =1;
            uidmessage::instance()->hide_Message();
            qDebug()<<"Success in the parse";
            return FUNCTION_SUCCESS;

        }
        else
        {

            if(strcmp((char *)bill.ucResponseMswssage,"Session Expired.")==0)
            {
                uidmessage::instance()->hide_Message();
                sprintf(ErrorMsg,"%s",(char *)bill.ucResponseMswssage);
                miscObj.DisplayMsgBox(ErrorMsg);
                uidmessage::instance()->hide_Message();
                return SESSION_EXPIRED_FAILED;
            }
            sprintf(ErrorMsg,"%s",(char *)bill.ucResponseMswssage);
            miscObj.DisplayMsgBox(ErrorMsg);
            uidmessage::instance()->hide_Message();
            return FUNCTION_FAILED;

        }

        return FUNCTION_SUCCESS;
}


int FPB::Fetch_bill_parsing()
{

    system("sed 's/\"ResponseData\":\"{/\"ResponseData\":{/g' /usr/FINO/FPB/RESP/OUTPUT.TXT > /tmp/1.txt");
    system("sed 's/}\",\"MessageId\"/},\"MessageId\"/g' /tmp/1.txt > /tmp/2.txt  ");
    system(" sed 's/\\\\//g' /tmp/2.txt > /usr/FINO/FPB/RESP/OUTPUT.TXT");
    system("cd /tmp/");
    system("rm /tmp/1.txt");
    system("rm /tmp/2.txt");

    Json1::Value root;
    root.clear();
    //QSqlQuery query;
    Json1::Reader reader;
    std::ifstream ReadData("/usr/FINO/FPB/RESP/OUTPUT.TXT");
    bool parsedSuccess = reader.parse(ReadData,root,false);

    ReadData.close();

    if(not parsedSuccess)
    {

        qDebug()<<"FpsMonthlyPdsData File not Found";
        return FUNCAL_FAILED;
    }
    else
    {
  #if DEBUG
        qDebug()<<"RequestId   : "<<Convertstr(root["RequestId"].asString());
        qDebug()<<"DisplayMessage : "<<Convertstr(root["DisplayMessage"].asString());
        qDebug()<<"ResponseMessage    : "<<Convertstr(root["ResponseMessage"].asString());
        qDebug()<<"ResponseCode : "<<Convertstr(root["ResponseCode"].asString());
#endif
        strcpy((char*)bill.ucResponseCode,Convertstr(root["ResponseCode"].asString()));
        strcpy((char*)bill.ucResponseMswssage,Convertstr(root["ResponseMessage"].asString()));
        strcpy((char*)bill.ucDisplayMswssage,Convertstr(root["DisplayMessage"].asString()));
        if(strcmp((char *)bill.ucResponseCode,"0")==0)
        {
            Json1::Value ResponseData;
            ResponseData.clear();
            ResponseData = root.get("ResponseData",0);
            printf("\nSize : %d\n",ResponseData.size());
            if(ResponseData.size()==0)
            {
                qDebug("\n1:Key Register Data Not Found\n");
                 return FUNCAL_FAILED;
            }
            else
            {
                qDebug()<<"MessageCode:"<<Convertstr(ResponseData["MessageCode"].asString());
                qDebug()<<"TraceId:"<<Convertstr(ResponseData["TraceId"].asString());
                qDebug()<<"SourceID:"<<Convertstr(ResponseData["SourceID"].asString());
                qDebug()<<"TimeStamp:"<<Convertstr(ResponseData["TimeStamp"].asString());
                qDebug()<<"UserID:"<<Convertstr(ResponseData["UserID"].asString());
                qDebug()<<"CustomerID:"<<Convertstr(ResponseData["CustomerID"].asString());
                qDebug()<<"Valid:"<<Convertstr(ResponseData["Valid"].asString());
                qDebug()<<"ErrorCode:"<<Convertstr(ResponseData["ErrorCode"].asString());
                qDebug()<<"ErrorMessage:"<<Convertstr(ResponseData["ErrorMessage"].asString());
                qDebug()<<"BillerId:"<<Convertstr(ResponseData["BillerId"].asString());
                qDebug()<<"Authenticator1:"<<Convertstr(ResponseData["Authenticator1"].asString());
                qDebug()<<"Authenticator2:"<<Convertstr(ResponseData["Authenticator2"].asString());
                qDebug()<<"Authenticator3:"<<Convertstr(ResponseData["Authenticator3"].asString());
                qDebug()<<"BillNumber:"<<Convertstr(ResponseData["BillNumber"].asString());
                qDebug()<<"BillDate:"<<Convertstr(ResponseData["BillDate"].asString());
                qDebug()<<"BillDueDate:"<<Convertstr(ResponseData["BillDueDate"].asString());
                qDebug()<<"BillAmount:"<<Convertstr(ResponseData["BillAmount"].asString());
                qDebug()<<"PayWithOutBill:"<<Convertstr(ResponseData["PayWithOutBill"].asString());
                qDebug()<<"PartialPayment:"<<Convertstr(ResponseData["PartialPayment"].asString());
                qDebug()<<"Filler1:"<<Convertstr(ResponseData["Filler1"].asString());
                qDebug()<<"Filler2:"<<Convertstr(ResponseData["Filler2"].asString());
                qDebug()<<"Filler3:"<<Convertstr(ResponseData["Filler3"].asString());
                qDebug()<<"Checksum:"<<Convertstr(ResponseData["Checksum"].asString());

                strcpy((char*)bill.BillDate,Convertstr(ResponseData["BillDate"].asString()));
                strcpy((char*)bill.BillDueDate,Convertstr(ResponseData["BillDueDate"].asString()));
                strcpy((char*)bill.BillAmount,Convertstr(ResponseData["BillAmount"].asString()));
                strcpy((char*)bill.PayWithOutBill,Convertstr(ResponseData["PayWithOutBill"].asString()));
                strcpy((char*)bill.PartialPayment,Convertstr(ResponseData["PartialPayment"].asString()));


                return FUNC_OK;
            }
        }
        else
        {
            return FUNC_ERROR;
        }
    }

}






int FPB::json_creation(int flag,unsigned char *gSendData)
{

   Json1::Value ArrayList(Json1::arrayValue),PostTransactionArrayList(Json1::arrayValue);
    Json1::Value acctFundTransfer,PostTransaction,BillDeskData;
    Json1::Value RequestedData,mainreq;
    char buff1[100];
    memset(buff1,0,sizeof(buff1));
    //sprintf(buff1,"1234567890");
    char itimedate[20];
    memset(itimedate, 0, sizeof(itimedate));
    strcpy(itimedate,iTimestamp());
    printf("Time Stamp == %s\n",itimedate);

    //int kkkk = 102920;

    QString Bitmap =(char *) bill.Req_requestbitmap_req;
    mainreq["RequestId"] =  (char *)bill.ucRequestId_req;
    mainreq["MethodId"] = bill.ucMethodId;
    mainreq["SessionId"] =(char *) bill.ucSessionId_req;
    mainreq["TellerID"]= username;
    mainreq["TokenId"]= "";
    mainreq["X_Auth_Token"] ="";
    mainreq["ChannelID"]= bill.ucChannelID_req;
    mainreq["IsEncrypt"]= false;
    mainreq["SessionExpiryTime"]=(char *)configLoginLeg2.ucSessionExpiryTime;

    //RequestData
    RequestedData["requestbitmap"] = (char *)bill.Req_requestbitmap_req;
    RequestedData["asyncFlag"] = (char *)bill.Req_asyncFlag_req;
    RequestedData["totalAmount"] = (char *) bill.Req_amount_req;
    RequestedData["mobileNumber"] =(char *) bill.Req_mobileNumber_req;
    RequestedData["merchantMobileNumber"] = (char *) configLoginLeg2.ucmobileNumber;"7400370122"; //"8055988351";
    RequestedData["txnType"] = (char *) bill.Req_txnType_req;//"IMPS"; //CustDet.cTxnType;
    RequestedData["cashoraccount"] = (char *) bill.Req_cashoraccount_req; //"CASH";
    RequestedData["impsserviceflag"] = (char *) bill.Req_impsserviceflag_req;
    RequestedData["parentRRN"] =(char *)bill.ucRequestId_req;


        //-----------------------------------------------              CustomerAuth              --------------------------------------------------

        //CustomerAuth
        RequestedData["CustomerAuth"]["MobileNo"] =(char *)bill.CA_MobileNo_req; //8268769453";
        RequestedData["CustomerAuth"]["AadharNo"] ="";
        RequestedData["CustomerAuth"]["Auth_id"] =bill.CA_Auth_id_req;
        RequestedData["CustomerAuth"]["Aadhaar"] = "";

    //-----------------------------------------------             CreateCustomer             --------------------------------------------------
    if(Bitmap.mid(1,1)=="1")
    {

    }
    else
    {
        RequestedData["CreateCustomer"] = "";

    }
    //-------------------------------------")----------             CreateAccount              --------------------------------------------------
    if(Bitmap.mid(2,1)=="1")
    {

    }
    else
    {
        RequestedData["CreateAccount"] = "";
    }


    //-----------------------------------------------             AddBeneficiary             --------------------------------------------------
    if(Bitmap.mid(3,1)=="1")
    {

        RequestedData["AddBeneficiary"]["appId"] =(char *)bill.AB_name_req; //"FINOTLR";
        RequestedData["AddBeneficiary"]["mobileNumber"] =(char *)bill.AB_mobileno_req; //username;//"100020442";
        RequestedData["AddBeneficiary"]["address"] =(char *)bill.AB_address_req;
        RequestedData["AddBeneficiary"]["idProofType"] ="";
        RequestedData["AddBeneficiary"]["idProofNumber"] =""; //CustDet.cAccountNumber;//"96786787687";
        RequestedData["AddBeneficiary"]["AddressProofType"] ="";
        RequestedData["AddBeneficiary"]["addressProofNumber"] = "";// CustDet.cBenificiaryName;//"webtset";


    }
    else
    {
        RequestedData["AddBeneficiary"] = "";
    }
    //-----------------------------------------------            AddWalkinCustomer           --------------------------------------------------
    if(Bitmap.mid(4,1)=="1")
    {
        RequestedData["AddWalkinCustomer"]["appId"] =(char *)bill.AB_name_req; //"FINOTLR";
        RequestedData["AddWalkinCustomer"]["mobileNumber"] =(char *)bill.AB_mobileno_req; //username;//"100020442";
        RequestedData["AddWalkinCustomer"]["address"] =(char *)bill.AB_address_req;
        RequestedData["AddWalkinCustomer"]["idProofType"] ="";
        RequestedData["AddWalkinCustomer"]["idProofNumber"] =""; //CustDet.cAccountNumber;//"96786787687";
        RequestedData["AddWalkinCustomer"]["AddressProofType"] ="";
        RequestedData["AddWalkinCustomer"]["addressProofNumber"] = "";// CustDet.cBenificiaryName;//"webtset";

    }
    else
    {
        RequestedData["AddWalkinCustomer"] = "";
    }

    //-----------------------------------------------             PostTransaction            --------------------------------------------------
    if(Bitmap.mid(5,1)=="1")
    {


        PostTransactionArrayList.clear();
        PostTransaction["appId"] =(char *)bill.PT_appId_req; //"FINOTLR";
        PostTransaction["valueDate"] ="";
        PostTransaction["isInclusive"] =bill.PT_isInclusive_req;
        PostTransaction["isClubbed"] = bill.PT_isClubbed_req;
        PostTransaction["analysisFlag"] =bill.PT_analysisFlag_req;
        PostTransaction["reversalFlag"] ="";
        PostTransaction["referenceNo"] =""; //buff1; //"TLR1032_26201913202573";

        ArrayList.clear();


        acctFundTransfer["accountNumber"] = "";
        acctFundTransfer["amount"] = (char *) bill.FTL_first_amount_req;
        acctFundTransfer["currency"] = (char *) bill.FTL_currency_req;
        acctFundTransfer["creditDebitFlag"] = (char *) bill.FTL_first_creditDebitFlag_req;
        acctFundTransfer["transactionType"] = (char *) bill.FTL_first_transactionType_req;
        acctFundTransfer["costCenter"] = (char *) bill.FTL_costCenter_req;
        acctFundTransfer["supportData"] = (char *) bill.FTL_first_supportdata_req ;
        acctFundTransfer["beneficiaryRefOrMmid"] = "";
        acctFundTransfer["beneficiaryMobile"] = "";
        acctFundTransfer["remitterMobile"] = (char *) bill.FTL_first_remitterMobile_req;
        acctFundTransfer["remitterMmid"] = "";
        acctFundTransfer["beneficiaryAccountNo"] = "";
        acctFundTransfer["beneficiaryIfsc"] = "";
        acctFundTransfer["remarks"] ="";
        ArrayList.append(acctFundTransfer);




        acctFundTransfer["accountNumber"] = "";
        acctFundTransfer["amount"] = (char *) bill.FTL_second_amount_req;
        acctFundTransfer["currency"] = (char *) bill.FTL_currency_req;
        acctFundTransfer["creditDebitFlag"] = (char *) bill.FTL_second_creditDebitFlag_req;
        acctFundTransfer["transactionType"] = (char *) bill.FTL_second_transactionType_req;
        acctFundTransfer["costCenter"] = (char *) bill.FTL_costCenter_req;
        acctFundTransfer["supportData"] = (char *) bill.FTL_second_supportdata_req ;
        acctFundTransfer["beneficiaryRefOrMmid"] = "";
        acctFundTransfer["beneficiaryMobile"] = "";
        acctFundTransfer["remitterMobile"] = (char *) bill.FTL_second_remitterMobile_req;
        acctFundTransfer["remitterMmid"] = "";
        acctFundTransfer["beneficiaryAccountNo"] = "";
        acctFundTransfer["beneficiaryIfsc"] = "";
        acctFundTransfer["remarks"] ="";
        ArrayList.append(acctFundTransfer);
        if(flag ==1)
        {
            acctFundTransfer["accountNumber"] = "";
            acctFundTransfer["amount"] = (char *) bill.FTL_third_amount_req;
            acctFundTransfer["currency"] = (char *) bill.FTL_currency_req;
            acctFundTransfer["creditDebitFlag"] = (char *) bill.FTL_third_creditDebitFlag_req;
            acctFundTransfer["transactionType"] = (char *) bill.FTL_third_transactionType_req;
            acctFundTransfer["costCenter"] = (char *) bill.FTL_costCenter_req;
            acctFundTransfer["supportData"] = (char *) bill.FTL_third_supportdata_req ;
            acctFundTransfer["beneficiaryRefOrMmid"] = "";
            acctFundTransfer["beneficiaryMobile"] = "";
            acctFundTransfer["remitterMobile"] = (char *) bill.FTL_third_remitterMobile_req;
            acctFundTransfer["remitterMmid"] = "";
            acctFundTransfer["beneficiaryAccountNo"] = "";
            acctFundTransfer["beneficiaryIfsc"] = "";
            acctFundTransfer["remarks"] ="";
            ArrayList.append(acctFundTransfer);

            acctFundTransfer["accountNumber"] = "";
            acctFundTransfer["amount"] = (char *) bill.FTL_fourth_amount_req;
            acctFundTransfer["currency"] = (char *) bill.FTL_currency_req;
            acctFundTransfer["creditDebitFlag"] = (char *) bill.FTL_fourth_creditDebitFlag_req;
            acctFundTransfer["transactionType"] = (char *) bill.FTL_fourth_transactionType_req;
            acctFundTransfer["costCenter"] = (char *) bill.FTL_costCenter_req;
            acctFundTransfer["supportData"] = (char *) bill.FTL_fourth_supportdata_req ;
            acctFundTransfer["beneficiaryRefOrMmid"] = "";
            acctFundTransfer["beneficiaryMobile"] = "";
            acctFundTransfer["remitterMobile"] = (char *) bill.FTL_fourth_remitterMobile_req;
            acctFundTransfer["remitterMmid"] = "";
            acctFundTransfer["beneficiaryAccountNo"] = "";
            acctFundTransfer["beneficiaryIfsc"] = "";
            acctFundTransfer["remarks"] ="";
            ArrayList.append(acctFundTransfer);

        }
        PostTransaction["acctFundTransferLegs"] = ArrayList;
        PostTransactionArrayList.append(PostTransaction);
        RequestedData["PostTransaction"] = PostTransactionArrayList;

    }
    else
    {
        RequestedData["PostTransaction"] = "";

    }


    ArrayList.clear();

    //-----------------------------------------------               DMSDetails             --------------------------------------------------
    if(Bitmap.mid(6,1)=="1")
    {

    }
    else
    {
        RequestedData["DMSDetails"] = "";
    }
    //-----------------------------------------------            AddExternalAccount          --------------------------------------------------
    if(Bitmap.mid(7,1)=="1")
    {

    }
    else
    {
        RequestedData["AddExternalAccount"] = "";

    }

////////////////////////////////BillDeskData

    BillDeskData["UserID"] = (char *) bill.BD_UserID_req;
    BillDeskData["CustomerID"] = (char *) bill.BD_CustomerID_req;
    BillDeskData["CustomerMobNo"] = (char *) bill.BD_CustomerMobNo_req;
    BillDeskData["BillerId"] = (char *) bill.BD_BillerId_req;
    BillDeskData["BankMessage"] = "";
    BillDeskData["PaymentAmount"] = (char *) bill.BD_PaymentAmount_req;
    BillDeskData["Authenticator1"] = (char *) bill.BD_Authenticator1_req;
    if(count >=2)
    {
        BillDeskData["Authenticator2"] = (char *) bill.BD_Authenticator2_req;
    }
    if(count >=3)
    {
        BillDeskData["Authenticator3"] = (char *) bill.BD_Authenticator3_req;
    }
   // ArrayList.append(BillDeskData);

    RequestedData["BillDeskData"] = BillDeskData;
    RequestedData["IMPSServiceDataList"] ="";

    //-----------------------------------------------------------------------------------------------------------------------------------------
    QString buff_tmp;
    buff_tmp = serialize1(Convertstr(RequestedData.toStyledString()));

    mainreq["RequestData"] = buff_tmp.toAscii().data();
    memset((char *)gSendData,0,sizeof((char *)gSendData));

    strcpy((char *)gSendData,Convertstr(mainreq.toStyledString()));

//    strncpy((char*)gSendData, jsondata, strlen(jsondata));
       printf("gSendData = %s\n\n",gSendData);
    return 1;
}

void FPB::on_second_editline_textEdited()
{
    ui->Enter_amount_edit->setReadOnly(false);
    ui->Total_amount_display->setText("       0.00");
    ui->Enter_amount_edit->clear();
    ui->Bill_date_label->clear();
    ui->Bill_amount_label->clear();
    ui->Due_date_label->clear();
    ui->Calculate_charges_button->setText("Calculate Charges");
    ui->Calculate_charges_button->setEnabled(true);
    ui->Calculate_charges_button->setStyleSheet("background-color: rgb(85, 0, 127); color: rgb(255, 255, 255)");
    fetch_bill = 0;
    Calculate_charges =0;


    return;
}


void FPB::on_first_editline_textEdited()
{
    ui->Enter_amount_edit->setReadOnly(false);
    ui->Total_amount_display->setText("       0.00");
    ui->Enter_amount_edit->clear();
    ui->Bill_date_label->clear();
    ui->Bill_amount_label->clear();
    ui->Due_date_label->clear();
    ui->Calculate_charges_button->setText("Calculate Charges");
    ui->Calculate_charges_button->setEnabled(true);
    ui->Calculate_charges_button->setStyleSheet("background-color: rgb(85, 0, 127); color: rgb(255, 255, 255)");
    fetch_bill = 0;
    Calculate_charges =0;
    return;
}




void FPB::on_Confirmation_back_button_clicked()
{
    ui->stackedWidget->setCurrentIndex(38);
    return ;
}


void FPB::on_pushButton_Ok_clicked()
{
        QString bill_amount = QString::fromUtf8((const char*)bill.BillAmount);
        ui->Enter_amount_edit->setText(bill_amount);
        if(!memcmp(bill.PartialPayment,"N",1))
        {
            ui->Enter_amount_edit->setReadOnly(true);
        }
        else
        {
            ui->Enter_amount_edit->setReadOnly(false);

        }
        ui->stackedWidget->setCurrentIndex(38);
        return;
}
