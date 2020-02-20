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
#include "print.h"
#include<fstream>

extern "C"
{
#include "gl11.h"
#include "http_client.h"
#include "uid_auth.h"
}

extern Printfpb ConfigPrint;
extern char * Convertstr(string str);
extern SUPPORTDATA_DEVICEINFO sd_deviceinfo;
extern Login_leg2 configLoginLeg2;
extern searchWalkCust SearchWalkCust;

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
int service_flag =0;
int fetch_display_flag =0;
QString bill_amount;
QString Filter;
extern walkinCust walkinCustLimit;
QString Total_Amount,Charges,Service_Provider,Category,first_input,Amount;
extern FINDCUSTOMER FINDCUST;
QString Partial_flag;
void FPB::on_Bill_payment_Button_clicked()
{
    char msgdisplay[100];
    memset(&billdata,0,sizeof(billdata));
    CMisc miscObj;
    QString Trans_type = "DMTBILLPAY1";
    int flag = 0;
    QSqlQuery query;
    query.clear();
    query.prepare("Select  TransactionType from GetProfile where ProductTypeID = '9999'");
    if(!query.exec())
    {
        qDebug() << query.lastError()<<"data base entry error";
        return ;
    }
    else
    {
        while(query.next())
        {
            if(Trans_type == query.value(0).toString().trimmed())
            {
                flag = 1;
            }

        }
    }

    query.finish();
    if(flag == 0)
    {
        sprintf(msgdisplay,"Feature currently not available");
        miscObj.DisplayMsgBox(msgdisplay);
        uidmessage::instance()->hide_Message();
        return ;
    }

    ui->BharatBill_Logo->hide();
 #if DEBUG
    qDebug()<<"ui->Search_lineEdit->text().size()"<<ui->Search_lineEdit->text().size();
#endif
    ui->Confirmation_next_button->setText("NEXT");
    ui->Confirmation_back_button->setText("BACK");

    if (ui->Search_lineEdit->text().size() != 10)
    {
        sprintf(msgdisplay,"Please Search by mobile no. to access this feature");
        miscObj.DisplayMsgBox(msgdisplay);
        return ;
    }
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
 #if DEBUG
    qDebug()<<"Arg value"<<arg1;
 #endif
    Service_Provider.clear();
    strcpy((char *)billdata.ucCategory,arg1.toAscii().constData());
    service_flag = 0;
    Category.clear();
    Category = arg1;
    query.prepare("SELECT  BILLER__NAME FROM BILLER_DETAILS WHERE SUBCATEGORY  = ? AND MODE__ID = 2 ");
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
#if DEBUG
            qDebug()<<"Biller code "<<Biller_code;
            qDebug()<<"Bharatpaylogo"<<Bharatpaylogo;
#endif
            strcpy((char *)billdata.ucBillercode,Biller_code.toLatin1().data());
            service_flag =1;


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
    QString Label_display;
    Partial_flag.clear();
    if(service_flag == 0)
    {
        QSqlQuery query;
        QString arg1 = ui->Bill_service_provider->currentText();
#if DEBUG
        qDebug()<<"dsdsdsdsd"<<arg1;
        qDebug()<<"dsdsdsdsd"<<arg1;
        qDebug()<<"dsdsdsdsd"<<arg1;
#endif
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
#if DEBUG
                qDebug()<<"Biller code "<<Biller_code;
                qDebug()<<"Bharatpaylogo"<<Bharatpaylogo;
#endif
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

    }

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
    ui->Fetch_bil_button->setStyleSheet("background-color: rgb(85, 0, 127); color: rgb(255, 255, 255)");


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
 #if DEBUG
            qDebug()<<"count"<<count;
#endif
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
            strcpy(FIELD_ID[i],query.value(0).toString().trimmed().toAscii().data());
            strcpy(FIELD_NAME[i],query.value(1).toString().trimmed().toAscii().data());
            strcpy(FIELD_TYPE[i],query.value(2).toString().trimmed().toAscii().data());
            FIELD_LENGTH[i] =  query.value(3).toInt();
            strcpy(FIELD_VISIBILITY[i],query.value(4).toString().trimmed().toAscii().data());
            FIELD_MIN_LENGTH[i] =  query.value(5).toInt();
            strcpy(FIELD_VALIDATION[i],query.value(6).toString().trimmed().toAscii().data());
#if DEBUG
            printf("FIELD_ID%s\n",FIELD_ID[i]);
            printf("FIELD_NAME%s\n",FIELD_NAME[i]);
            printf("FIELD_TYPE%s\n",FIELD_TYPE[i]);
            printf("FIELD_LENGTH%d\n",FIELD_LENGTH[i]);
            printf("FIELD_VISIBILITY%s\n",FIELD_VISIBILITY[i]);
            printf("FIELD_MIN_LENGTH%d\n",FIELD_MIN_LENGTH[i]);
            printf("FIELD_VALIDATION%s\n",FIELD_VALIDATION[i]);
#endif
            i++;
        }
    }

//    if(QString::fromUtf8(FIELD_ID[1]).isEmpty())
//    {
//        sprintf(msgdisplay," Select Service Provider is not available ");
//        miscObj.DisplayMsgBox(msgdisplay);
//        return ;

//    }
 #if DEBUG
    printf("FIELD_NAME%s\n",FIELD_NAME[1]);
    printf("FIELD_NAME%s\n",FIELD_NAME[1]);
#endif
    if(count>=1)
    {
        ui->first_label->setVisible(true);
        ui->first_editline->setVisible(true);

        switch(count)
        {
        case 1:
            ui->first_label->setGeometry(10,70,111,20);
            ui->first_editline->setGeometry(130,70,181,21);
            break;
        case 2:
            ui->first_label->setGeometry(10,50,111,20);
            ui->first_editline->setGeometry(130,50,181,21);
            break;
        case 3:
            ui->first_label->setGeometry(10,40,111,20);
            ui->first_editline->setGeometry(130,40,181,21);
            break;
        }
        Label_display.clear();
        Label_display = Bill_label(QString::fromLatin1(FIELD_NAME[0]));

        ui->first_label->setText(Label_display);
        ui->first_editline->setMaxLength(FIELD_LENGTH[0]);
        temp.clear();
        temp = QString::fromLatin1(FIELD_VALIDATION[0]);

        if(!temp.isEmpty())
        {
            QRegExp rx5(temp);
            QValidator *validator5 = new QRegExpValidator(rx5, this);
            ui->first_editline->setValidator(validator5);
        }

    }
    if(count>=2)
    {
        ui->second_label->setVisible(true);
        if(memcmp(FIELD_TYPE[1],"109",3)==0)
        {
            ui->Date_of_birth_edit->setVisible(true);
            ui->second_editline->setVisible(false);

        }
        else
        {
            ui->second_editline->setVisible(true);
            ui->Date_of_birth_edit->setVisible(false);

        }
        switch(count)
        {
        case 2:
            ui->second_label->setGeometry(10,90,111,20);


            if(memcmp(FIELD_TYPE[1],"109",3)==0)
            {
                ui->Date_of_birth_edit->setGeometry(130,90,181,21);
                QDate date = QDate::currentDate();
                QTime time = QTime::currentTime();
                QDateTime startDate(date, time);
                ui->Date_of_birth_edit->setDateTime(startDate);
            }
            else
            {
                ui->second_editline->setGeometry(130,90,181,21);
            }
            break;
        case 3:
            if(memcmp(FIELD_TYPE[1],"109",3)==0)
            {

                ui->Date_of_birth_edit->setGeometry(130,70,181,21);
            }
            else
            {
                ui->second_editline->setGeometry(130,70,181,21);
            }
            ui->second_label->setGeometry(10,70,111,20);
            break;
        }


        Label_display.clear();
        Label_display = Bill_label(QString::fromLatin1(FIELD_NAME[1]));


        ui->second_label->setText(Label_display);
        if(memcmp(FIELD_TYPE[1],"109",3)!=0)
        {
            ui->second_editline->setMaxLength(FIELD_LENGTH[1]);
        }
        temp.clear();
        remove_dollar.clear();
        temp = QString::fromLatin1(FIELD_VALIDATION[1]);

        if(memcmp(FIELD_TYPE[1],"109",3)!=0)
        {
            if(!temp.isEmpty())
            {
                QRegExp rx2(temp);
                QValidator *validator2 = new QRegExpValidator(rx2, this);
                ui->second_editline->setValidator(validator2);
            }
        }
        ui->third_editline->setVisible(false);
        ui->third_label->setVisible(false);
    }

    if(count>=3)
    {
        ui->third_label->setVisible(true);
        ui->third_editline->setVisible(true);
        ui->third_label->setGeometry(10,100,111,20);
        ui->third_editline->setGeometry(130,100,181,21);
        Label_display.clear();
        Label_display =Bill_label(QString::fromLatin1(FIELD_NAME[2]));


        ui->third_label->setText(Label_display);
        ui->third_editline->setMaxLength(FIELD_LENGTH[2]);
        temp.clear();
        remove_dollar.clear();
        temp = QString::fromLatin1(FIELD_VALIDATION[2]);

        if(!temp.isEmpty())
        {
            QRegExp rx3(temp);
            QValidator *validator3 = new QRegExpValidator(rx3, this);
            ui->third_editline->setValidator(validator3);

        }

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

    if(iRet != FUNCTION_SUCCESS)
    {
        if(iRet == SESSION_EXPIRED_FAILED)
        {
            ui->stackedWidget->setCurrentIndex(0);
            return;
        }
        else
        {
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

    int ret = bill_input_validation();
    if(ret !=0)
    {
        return FUNCTION_FAILED;
    }
    uidmessage::instance()->hide_Message();
    uidmessage::instance()->Display_BusyMessage("Please Wait...");
    char nullbuff[5];
    char ErrorMsg[100];
    memset(ErrorMsg,0,sizeof(ErrorMsg));
    QSqlQuery query;
    QString Amount;
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
 #if DEBUG
            qDebug()<<"Partialpay_flag"<<Partialpay_flag;
 #endif
        }
    }
    query.clear();
    QString Acc_Bal;
    Acc_Bal = QString::fromUtf8((const char*)Balance_Display).remove("Balance : ");
#if DEBUG
    qDebug()<<"Acc_Bal"<<Acc_Bal.trimmed();
#endif

    if(Partialpay_flag == "N" && fetch_bill == 0)
    {
        sprintf(ErrorMsg,"PLease fetch biller details first");
        miscObj.DisplayMsgBox(ErrorMsg);
        uidmessage::instance()->hide_Message();
        return FUNCAL_FAILED;
    }


    if (ui->Enter_amount_edit->text().isEmpty())
    {
        sprintf(ErrorMsg,"Amount is mandatory");
        miscObj.DisplayMsgBox(ErrorMsg);
        uidmessage::instance()->hide_Message();
        return FUNCAL_FAILED ;
    }


    QString temp_amt;
    unsigned char query_amount[15], query_maxAmount[15],query_minAmount[15];
    memset(query_amount, 0, sizeof(query_amount));
    memset(query_minAmount, 0, sizeof(query_minAmount));
    memset(query_maxAmount, 0, sizeof(query_maxAmount));
    query.clear();
    query.prepare("SELECT RFU, MinTransLimit,MaxTransLimit from GetProfile where TransactionType = 'DMTBILLPAY1' AND ProductTypeID = '9999'");
    if(!query.exec())
    {
        qDebug() << query.lastError()<<"data base entry error";
        return FUNCAL_FAILED;
    }
    else
    {
        while(query.next())
        {
            temp_amt = query.value(0).toString().trimmed();
            memset(query_minAmount, 0, sizeof(query_maxAmount));
            memset(query_maxAmount, 0, sizeof(query_maxAmount));
            strcpy((char*)query_minAmount, query.value(1).toString().trimmed().toAscii().data());
            strcpy((char *)query_maxAmount, query.value(2).toString().trimmed().toAscii().data());
#if DEBUG
            qDebug()<<"temp_amt = "<<temp_amt;
            printf("Minimum Txn Amount = %s\n",query_minAmount);
            printf("Maximum Txn Amount = %s\n",query_maxAmount);
#endif
        }
    }

    query.finish();
    temp_amt = temp_amt.mid(0, temp_amt.indexOf("|"));
    memset(query_amount, 0, sizeof(query_amount));
    memcpy(query_amount, temp_amt.toAscii().trimmed().data(), strlen(temp_amt.toAscii().trimmed().data()));
    float CustTotalAmount = atof((const char*)query_amount) - atof((const char*)Cust_CurrMonthAmount);

#if DEBUG
    qDebug()<<"temp_amt sd = "<<temp_amt;
    printf("\nQuery Cust Amount = %s\n", query_amount);
    qDebug()<<"Acc_Bal.toFloat() "<<Acc_Bal.toFloat() ;
    qDebug()<<"ui->Enter_amount_edit->text().toFloat()"<<ui->Enter_amount_edit->text().toFloat();
#endif

    float cc_amount = atof(ui->Enter_amount_edit->text().toAscii().trimmed().data());

    if(cc_amount == 0)
    {

        sprintf(ErrorMsg,"Amount cannot be zero");
        miscObj.DisplayMsgBox(ErrorMsg);
        uidmessage::instance()->hide_Message();
        return FUNCAL_FAILED;
    }


    if(Acc_Bal.toFloat() < cc_amount)
    {

        sprintf(ErrorMsg,"Amount should not be greater than Merchant balance(%.2f)",Acc_Bal.toFloat());
        miscObj.DisplayMsgBox(ErrorMsg);
        uidmessage::instance()->hide_Message();
        return FUNCAL_FAILED;
    }

//    if(cc_amount > CustTotalAmount) // For Customer Limit
//    {
//        uidmessage::instance()->hide_Message();
//        sprintf(ErrorMsg,"Amount should not be greater than customer available limit(%.2f)",CustTotalAmount);
//        miscObj.DisplayMsgBox(ErrorMsg);
//        uidmessage::instance()->hide_Message();
//        return FUNCAL_FAILED;
//    }
    if(cc_amount < atof((char *)query_minAmount) || cc_amount > atof((const char*)query_maxAmount))
    {
        uidmessage::instance()->hide_Message();
        if(cc_amount < atof((char *)query_minAmount))
        {
            sprintf(ErrorMsg,"Amount should be greater than MinTransaction limit (%.2f)",atof((char *)query_minAmount));
        }
        else if(cc_amount > atof((const char*)query_maxAmount))
        {
            sprintf(ErrorMsg,"Amount should not be greater than MaxTransaction limit (%.2f)",atof((char *)query_maxAmount));
        }

        miscObj.DisplayMsgBox(ErrorMsg);
        uidmessage::instance()->hide_Message();
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
    memcpy((char *)bill.ucIsEncrypt_req,"false",5);
    bill.ucChannelID_req =CHANNEL_ID_IMPS;
    memcpy((char *)bill.ucSessionExpiryTime_req,(char *)configLoginLeg2.ucSessionExpiryTime,strlen((char *)configLoginLeg2.ucSessionExpiryTime));


    memcpy((char *)bill.Req_asyncFlag_req,"N",1);
    memcpy(bill.Req_amount_req,ui->Enter_amount_edit->text().trimmed().toAscii().data(),strlen(ui->Enter_amount_edit->text().trimmed().toAscii().data()));
    memcpy((char *)bill.Req_mobileNumber_req,FC_MobileNumber,10);
    memcpy((char *)bill.Req_cashoraccount_req,"CASH",4);
    memcpy((char *)bill.Req_txnType_req,"BILLPAY",7);
    memcpy((char *)bill.Req_impsserviceflag_req,"N",1);
    memcpy((char *)bill.Req_requestbitmap_req,"00000100",8);
    memcpy((char *)bill.Req_merchantMobileNumber_req,(char *)configLoginLeg2.ucmobileNumber,strlen((char *)configLoginLeg2.ucSessionId));



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
#if DEBUG
            printf("bill.ucBillercode%s",bill.BD_BillerId_req);
 #endif
        }
    }
    query.clear();

    //  memcpy((char *)bill.BD_BillerId_req,"IGLOB",5);first_editline
    memcpy((char *)bill.BD_PaymentAmount_req,ui->Enter_amount_edit->text().trimmed().toAscii().data(),strlen(ui->Enter_amount_edit->text().trimmed().toAscii().data()));
    memcpy((char *)bill.BD_Authenticator1_req,ui->first_editline->text().trimmed().toAscii().data(),strlen(ui->first_editline->text().trimmed().toAscii().data()));
    if(count >= 2)
    {
        if(memcmp(FIELD_TYPE[1],"109",3)==0)
        {
            QString date = ui->dateEdit_2->date().toString("dd-MM-yyyy") ;
            memcpy((char *)bill.BD_Authenticator2_req,date.trimmed().toAscii().data(),strlen(date.trimmed().toAscii().data()));

        }
        else
        {
            memcpy((char *)bill.BD_Authenticator2_req,ui->second_editline->text().trimmed().toAscii().data(),strlen(ui->second_editline->text().trimmed().toAscii().data()));
        }
    }
    if(count >= 3)
    {
        memcpy((char *)bill.BD_Authenticator1_req,ui->third_editline->text().trimmed().toAscii().data(),strlen(ui->third_editline->text().trimmed().toAscii().data()));
    }
    memcpy((char *)bill.BD_BankMessage,Filter.trimmed().toAscii().data(),strlen(Filter.trimmed().toAscii().data()));

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

#if DEBUG
    printf("gSendData = %s\n\n",gSendData);
#endif

    /****************************Encryption function*********************************/
    if(!strcmp(ISENCEYPT,"true"))
    {
        int gSendData_len =strlen((char *)gSendData);
        RequestDetachEncryptAttach((char *)gSendData,1,gSendData_len);
    }
    /***************************************************************/


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
    /****************************Decryption function*********************************/
    if(!strcmp(ISENCEYPT,"true"))
    {
        int oBuff_len = strlen(oBuff);
#if DEBUG
        qDebug()<<"Length of buffer"<<oBuff_len;
        printf("Buffer%s\n",oBuff);
#endif
        ResponseDetachEncryptAttach(oBuff,2,oBuff_len);//TEMPORARY

    }
    /*******************************************************************************/
    system("cd /usr/FINO/FPB/RESP/");
    system("rm /usr/FINO/FPB/RESP/BILL_CHARGES.TXT");
    int gRecv_len = strlen((char *)oBuff);
    FILE *fp2 = NULL;
    fp2 = fopen("/usr/FINO/FPB/RESP/BILL_CHARGES.TXT","w");
    if(fp2 == NULL)
    {
        return FUNCTION_FAILED;
    }
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
        if(strlen((char *)bill.ucResponseMswssage)<=0)
        {
            QString OTPBuff = QString::fromLocal8Bit(oBuff);//Get the ouput buffer in QString

            bool check = OTPBuff.contains("ResponseMessage");
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
            QStringList ResponseMessage = query.filter("ResponseMessage");
            QString str ,strToFind,value;
            str = ResponseMessage.at(0);
            strToFind = ":";
            value = str.mid(str.indexOf(strToFind)+strToFind.length());

            strcpy((char *)bill.ucResponseMswssage,value.toAscii().data());

        }


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
    ui->stackedWidget->setCurrentIndex(15);
    return;
}


void FPB::on_Bill_payment_back_button_clicked()
{
    ui->first_editline->clear();
    ui->second_editline->clear();
    ui->third_editline->clear();
    ui->Enter_amount_edit->clear();
    ui->Calculate_charges_button->setText("Calculate Charges");
    ui->Calculate_charges_button->setEnabled(true);
    ui->Calculate_charges_button->setStyleSheet("background-color: rgb(85, 0, 127); color: rgb(255, 255, 255)");
    ui->Fetch_bil_button->setStyleSheet("background-color: rgb(85, 0, 127); color: rgb(255, 255, 255)");
    service_flag = 0;
    ui->Total_amount_display->clear();
     ui->BharatBill_Logo->hide();

    ui->stackedWidget->setCurrentIndex(37);
    return;
//    ui->Bill_service_provider->clear();
 //   Service_Provider.clear();
 //   ui->Bill_category->setCurrentIndex(0);
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
#if DEBUG
                    qDebug()<<"returnCode:"<<Convertstr(ResponseData[j]["returnCode"].asString());
                    qDebug()<<"in  chargesList";
 #endif
                    ResponseData2 = ResponseData[j].get("chargesList",0);
                    for(int i = 0;i<ResponseData2.size();i++)
                    {
                        strcpy((char*)bill.uccode[i],Convertstr(ResponseData2[i]["code"].asString()));
                     //   strcpy((char*)bill.ucamount[i],Convertstr(ResponseData2[i]["amount"].asString())); // Prod_04092019
                        if(strcmp((char *)bill.uccode[i],"CHGDMTBILLPAY")==0) // Prod_04092019
                        {
                            qDebug()<<"Inside the check loop";
                            strcpy((char*)bill.ucamount[2],Convertstr(ResponseData2[i]["amount"].asString()));
                        }
                        strcpy((char*)bill.ucdescription[i],Convertstr(ResponseData2[i]["description"].asString()));
                        strcpy((char*)bill.ucchargeAcctNum[i],Convertstr(ResponseData2[i]["chargeAcctNum"].asString()));
#if DEBUG
                        printf("ucchargeAcctNum%s",bill.ucchargeAcctNum[i]);
                        qDebug()<<"code:"<<Convertstr(ResponseData2[i]["code"].asString());
                        qDebug()<<"amount:"<<Convertstr(ResponseData2[i]["amount"].asString());
                        qDebug()<<"description:"<<Convertstr(ResponseData2[i]["description"].asString());
                        qDebug()<<"chargeAcctNum:"<<Convertstr(ResponseData2[i]["chargeAcctNum"].asString());
#endif
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
    bill.ucChannelID_req =CHANNEL_ID_IMPS;
    memcpy((char *)bill.ucSessionExpiryTime_req,(char *)configLoginLeg2.ucSessionExpiryTime,strlen((char *)configLoginLeg2.ucSessionExpiryTime));


    memcpy((char *)bill.Req_asyncFlag_req,"N",1);
    memcpy((char *)bill.Req_amount_req,billdata.ucTotal_Amount,strlen(billdata.ucTotal_Amount));
    memcpy((char *)bill.Req_mobileNumber_req,FC_MobileNumber,10);
    memcpy((char *)bill.Req_cashoraccount_req,"CASH",4);
    memcpy((char *)bill.Req_txnType_req,"BILLPAY",7);
    memcpy((char *)bill.Req_impsserviceflag_req,"N",1);
    if(Flag_IMPS_Walkin == 'S' || CUST_TYPE == 'W' )
    {
        if(Flag_IMPS_Walkin == 'S')
            memcpy((char *)bill.Req_requestbitmap_req,"00001100",8);
        else
            memcpy((char *)bill.Req_requestbitmap_req,"00000100",8);

    }
    else
    {
        memcpy((char *)bill.Req_requestbitmap_req,"00000100",8);
    }

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
 #if DEBUG
    qDebug()<<"Biller_code"<<Biller_code;
#endif

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
#if DEBUG
            printf("bill.ucBillercode%s",bill.BD_BillerId_req);
#endif
        }
    }



    //  memcpy((char *)bill.BD_BillerId_req,"IGLOB",5);
    memcpy((char *)bill.BD_PaymentAmount_req,ui->Enter_amount_edit->text().trimmed().toAscii().data(),strlen(ui->Enter_amount_edit->text().trimmed().toAscii().data()));
    memcpy((char *)bill.BD_Authenticator1_req,ui->first_editline->text().trimmed().toAscii().data(),strlen(ui->first_editline->text().trimmed().toAscii().data()));
    if(count >=2)
    {
        if(memcmp(FIELD_TYPE[1],"109",3)==0)
        {
            QString date = ui->dateEdit_2->date().toString("dd-MM-yyyy") ;
            memcpy((char *)bill.BD_Authenticator2_req,date.trimmed().toAscii().data(),strlen(date.trimmed().toAscii().data()));

        }
        else
        {
            memcpy((char *)bill.BD_Authenticator2_req,ui->second_editline->text().trimmed().toAscii().data(),strlen(ui->second_editline->text().trimmed().toAscii().data()));
        }
    }
    if(count>=3)
    {
        memcpy((char *)bill.BD_Authenticator3_req,ui->third_editline->text().trimmed().toAscii().data(),strlen(ui->third_editline->text().trimmed().toAscii().data()));
    }
    memcpy((char *)bill.BD_BankMessage,Filter.trimmed().toAscii().data(),strlen(Filter.trimmed().toAscii().data()));
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

    memcpy((char *)bill.FTL_second_creditDebitFlag_req,"D",1);
    memcpy((char *)bill.FTL_second_remitterMobile_req,FC_MobileNumber,10);
    sprintf(IMPS_SupportData,"DN1#%s",billdata.ucTotal_Amount);
    memcpy((char *)bill.FTL_second_supportdata_req,IMPS_SupportData,sizeof(IMPS_SupportData));


    memset(IMPS_SupportData,0,strlen(IMPS_SupportData));
    memcpy((char *)bill.FTL_third_amount_req,ui->Enter_amount_edit->text().trimmed().toAscii().data(),strlen(ui->Enter_amount_edit->text().trimmed().toAscii().data()));
    memcpy((char *)bill.FTL_third_transactionType_req,"DMTBILLPAY1",11);
    memcpy((char *)bill.FTL_third_creditDebitFlag_req,"D",1);
    memcpy((char *)bill.FTL_third_remitterMobile_req,FC_MobileNumber,10);
    QString bal = QString::fromUtf8(billdata.ucCharges);//29/07/2019
    bool decimal = bal.contains('.');
    if(decimal == false)
    {
        bal = bal.append(".0");
    }
    sprintf(IMPS_SupportData,"SPROD#2~PVTXNID#%s~PVTXNDT#%s~IPADDR#%s~DEVICEID#%s~APPID#%s~AUTHID#@authid~LOCATION#~CELLID#%s~MCC#0,0,0,0~RPTFLG#0~PRTXNID#~PRTXNAMT#%s~SPLTSEQ#0~CHGAMT#%s~ZRFUT1#%s~ZRFUT2#%s~ZRFUT3#%s~ZRFUT4#@traceId~ZRFUT5#%s,%s~ZRFUT6#~ZRFUT7#~ZRFUT8#~ZRFUT9#@txnId~ZRFUT10#~ZRFUN1#~ZRFUN2#~ZRFUN3#~ZRFUN4#%s~ZRFUNDT#",(char *)sd_deviceinfo.ucSupportData_PVTXNID_req,(char *)sd_deviceinfo.ucSupportData_PVTXNDT_req,(char *)sd_deviceinfo.ucSupportData_IPADDR_req,(char *)sd_deviceinfo.ucSupportData_DEVICEID_req,CLIENT_ID,sd_deviceinfo.ucSupportData_CELLID_req,bill.FTL_third_amount_req,bal.toAscii().data(),bill.BD_BillerId_req,Service_Provider.toAscii().data(),Category.toAscii().data(),FIELD_ID[0],bill.BD_Authenticator1_req,FC_MobileNumber);//29/07/2019
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
    uidmessage::instance()->hide_Message();
    uidmessage::instance()->Display_BusyMessage("Transaction Request in Progress");

    /****************************Encryption function*********************************/
    if(!strcmp(ISENCEYPT,"true"))
    {
        int gSendData_len =strlen((char *)gSendData);
        RequestDetachEncryptAttach((char *)gSendData,1,gSendData_len);
    }
    /***************************************************************/
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
    /****************************Decryption function*********************************/
    if(!strcmp(ISENCEYPT,"true"))
    {
        int oBuff_len = strlen(oBuff);
#if DEBUG
        qDebug()<<"Length of buffer"<<oBuff_len;
        printf("Buffer%s\n",oBuff);
#endif
        ResponseDetachEncryptAttach(oBuff,2,oBuff_len);//TEMPORARY


    }
    /*******************************************************************************/
    system("cd /usr/FINO/FPB/RESP/");
    system("rm /usr/FINO/FPB/RESP/BILL_CHARGES.TXT");
    int gRecv_len = strlen((char *)oBuff);
    FILE *fp2 = NULL;
    fp2 = fopen("/usr/FINO/FPB/RESP/OUTPUT.TXT","w");
    if(fp2 == NULL)
    {
        return FUNCTION_FAILED;
    }
    fwrite( oBuff, 1, gRecv_len, fp2 );
    fclose(fp2);
    int iRetparse = Post_bill_parsing();
    if(iRetparse==FUNC_OK)
    {
        uidmessage::instance()->hide_Message();
        uidmessage::instance()->Display_BusyMessage("Bill Payment Transaction Success");
        sleep(1);


        display_flag = 1;
        char display_confirmpage[13][120];
        sprintf(display_confirmpage[0],"Category             : %s",Category.toAscii().data());
        sprintf(display_confirmpage[1],"Payment through      : Visiontek");
        sprintf(display_confirmpage[2],"Payment mode         : Account Transfer");
        sprintf(display_confirmpage[3],"Service Provider     : %s",Service_Provider.toAscii().data());
        sprintf(display_confirmpage[4],"%-21s: %s",FIELD_NAME[0],first_input.toAscii().data());
        sprintf(display_confirmpage[5],"Bill Amount          : %s",bill_amount.toAscii().data());
        sprintf(display_confirmpage[6],"Cust. Convenience Fee: %s",Charges.toAscii().data());
        sprintf(display_confirmpage[7],"Total Amount         : %s",Total_Amount.toAscii().data());
        sprintf(display_confirmpage[8],"Cash Contribution    : %s",Total_Amount.toAscii().data());
        sprintf(display_confirmpage[9],"Transaction ID       : %s",bill.txnReferenceNo[1]);
        if(strcmp((char *)bill.PRFilter3,"NA")!=0)
        {
            sprintf(display_confirmpage[10],"BBPS_TransID       : %s",bill.PRFilter3);
            sprintf(display_confirmpage[11],"Consumer Number      : %s",first_input.toAscii().data());
            sprintf(display_confirmpage[12],"Status              : Success");
        }
        else
        {
            sprintf(display_confirmpage[10],"Consumer Number      : %s",first_input.toAscii().data());
            sprintf(display_confirmpage[11],"Status              : Success");
        }
        int i=0;
        ui->Confirmation_listwidget->clear(); // clear list
        ui->Confirmation_listwidget->setFont(QFont("Courier",14));

        for(i=0;i<12;i++)
        {
#if DEBUG
            printf("display_confirmpage    =====    %s\n",display_confirmpage[i]);
#endif
            ui->Confirmation_listwidget->addItem((char *)display_confirmpage[i]);
        }



        ui->Confirmation_next_button->setText("FINISH");
        ui->Confirmation_back_button->setText("PRINT");


        memset(&ConfigPrint,0,sizeof(ConfigPrint));
        memcpy((char *)ConfigPrint.ucCustName,  (const char *)SearchWalkCust.SWCL_name_resp, strlen((const char *)SearchWalkCust.SWCL_name_resp));
        memcpy((char *)ConfigPrint.ucCustMobNo, (const char *)FC_MobileNumber, strlen((const char *)FC_MobileNumber));
        memcpy((char *)ConfigPrint.ucRefNo, (char *)bill.txnReferenceNo[1],strlen(bill.txnReferenceNo[1]));
        printf("******** bill.PRFilter3 == %s\n", bill.PRFilter3);
        memset(ConfigPrint.ucBBPS_TransID, 0, sizeof(ConfigPrint.ucBBPS_TransID));
         if(strcmp((char *)bill.PRFilter3,"NA")!=0)
         {
            memcpy((char *)ConfigPrint.ucBBPS_TransID, bill.PRFilter3, strlen((const char*)bill.PRFilter3));
         }
         else
         {
             memcpy((char *)ConfigPrint.ucBBPS_TransID, "NA", 2);
         }
         printf("********* ConfigPrint.ucBBPS_TransID == %s\n", ConfigPrint.ucBBPS_TransID);

        memcpy((char *)ConfigPrint.ucServiceProvider, Service_Provider.toAscii().data(),strlen(Service_Provider.toAscii().data()));
        memcpy((char *)ConfigPrint.ucConsumerId, first_input.toAscii().data(),strlen(first_input.toAscii().data()));
        memcpy((char *)ConfigPrint.uctxntype, (char *)bill.Req_txnType_req,strlen((char *)bill.Req_txnType_req));
        memcpy((char *)ConfigPrint.ucCharges, Charges.toAscii().data(),strlen(Charges.toAscii().data()));
        memcpy((char *)ConfigPrint.ucbillamt, bill_amount.toAscii().data(),strlen(bill_amount.toAscii().data()));
        memcpy((char *)ConfigPrint.ucAmount,Amount.toAscii().data(),strlen(Amount.toAscii().data()));
        memcpy((char *)ConfigPrint.ucTotalAmt, Total_Amount.toAscii().data(), strlen(Total_Amount.toAscii().data()));

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


        int ret_wcl = Walkin_Cust_Limit();

        if(ret_wcl == FUNCTION_SUCCESS)
        {
            int temp_cust_amt = atoi((const char*)walkinCustLimit.ucWCL_RDWCLmaxMonthlyLimit_resp) - atoi((const char*)walkinCustLimit.ucWCL_RDWCLcurrMonthAmount_resp);
            bool decimal;
            QString bal = QString::number(temp_cust_amt);
            decimal = bal.contains('.');
            if(decimal == false)
            {
                bal = bal.append(".00");
            }

            ui->Ft_limit_disp->setText(bal);
            bal.clear();
        }
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
#if DEBUG
        printf("Balance_Display = %s\n",Balance_Display);
#endif
        ui->SubTransaction_Label2->setText(QString::fromUtf8((const char*)Balance_Display));
        ui->SubTransaction_Label2->setFont(QFont("SansSerif",9));
        ui->SubTransaction_Label2->setAlignment(Qt::AlignRight | Qt::AlignBottom);
        uidmessage::instance()->hide_Message();

        return FUNCTION_SUCCESS;

    }
    else
    {
        if(strlen((char *)bill.ucResponseMswssage)<=0)
        {
            QString OTPBuff = QString::fromLocal8Bit(oBuff);//Get the ouput buffer in QString

            bool check = OTPBuff.contains("ResponseMessage");
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
            QStringList ResponseMessage = query.filter("ResponseMessage");
            QString str ,strToFind,value;
            str = ResponseMessage.at(0);
            strToFind = ":";
            value = str.mid(str.indexOf(strToFind)+strToFind.length());

            strcpy((char *)bill.ucResponseMswssage,value.toAscii().data());

        }

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
        int ret_wcl = Walkin_Cust_Limit();
        if(ret_wcl == FUNCTION_SUCCESS)
        {
            int temp_cust_amt = atoi((const char*)walkinCustLimit.ucWCL_RDWCLmaxMonthlyLimit_resp) - atoi((const char*)walkinCustLimit.ucWCL_RDWCLcurrMonthAmount_resp);
            bool decimal;
            QString bal = QString::number(temp_cust_amt);
            decimal = bal.contains('.');
            if(decimal == false)
            {
                bal = bal.append(".00");
            }

            ui->Ft_limit_disp->setText(bal);
            bal.clear();
        }
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


    if(Calculate_charges == 0)
    {

        sprintf(msgdisplay,"Please Calculate Charges");
        miscObj.DisplayMsgBox(msgdisplay);
        return;
    }
    int i=0,llop=0;
    //    if(Bharatpaylogo == "true")
    //    {
    //        qDebug()<<"true";
    //        ui->BharatBill_Logo_confirm->setVisible(true);
    //        llop =9;
    //    }
    //    else if(Bharatpaylogo == "false")
    //    {
    //        qDebug()<<"false";

    //        ui->BharatBill_Logo_confirm->setVisible(false);
    //        llop =9;

    //    }
    llop =9;
    first_input = ui->first_editline->text();
    bill_amount.clear();
    bill_amount = ui->Enter_amount_edit->text();
    display_flag = 0;
    char display_confirmpage[10][120];
    sprintf(display_confirmpage[0],"Category             : %s",Category.toAscii().data());
    sprintf(display_confirmpage[1],"Payment through      : Visiontek");
    sprintf(display_confirmpage[2],"Payment mode         : Account Transfer");
    sprintf(display_confirmpage[3],"Service Provider     : %s",Service_Provider.toAscii().data());
    sprintf(display_confirmpage[4],"%-21s: %s",FIELD_NAME[0],first_input.toAscii().data());
    bool decimal;
    decimal = bill_amount.contains('.');
    if(decimal == false)
    {
        bill_amount = bill_amount.append(".00");
    }
    sprintf(display_confirmpage[5],"Bill Amount          : %s",bill_amount.toAscii().data());
    sprintf(display_confirmpage[6],"Cust. Convenience Fee: %s",Charges.toAscii().data());
    sprintf(display_confirmpage[7],"Total Amount         : %s",Total_Amount.toAscii().data());
    sprintf(display_confirmpage[8],"Cash Contribution    : %s",Total_Amount.toAscii().data());




    ui->Confirmation_listwidget->clear(); // clear list
    ui->Confirmation_listwidget->setFont(QFont("Courier",14));

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
#if DEBUG
                    qDebug()<<"The loop value"<<j;
                    qDebug()<<"returnCode:"<<Convertstr(ResponseData[j]["returnCode"].asString());
                    qDebug()<<"txnReferenceNo:"<<Convertstr(ResponseData[j]["txnReferenceNo"].asString());
                    qDebug()<<"cbsTxnReferenceNo:"<<Convertstr(ResponseData[j]["cbsTxnReferenceNo"].asString());
#endif
                    strcpy((char*)bill.ucreturncode[j],Convertstr(ResponseData[j]["returnCode"].asString()));
                    strcpy((char*)bill.txnReferenceNo[j],Convertstr(ResponseData[j]["txnReferenceNo"].asString()));
                    strcpy((char*)bill.cbsTxnReferenceNo[j],Convertstr(ResponseData[j]["cbsTxnReferenceNo"].asString()));

                    ResponseData2.clear();
                    ResponseData1 = ResponseData[j].get("balancesList",0);
                    printf("\nSize : %d\n",ResponseData1.size());
                    for(int k=0;k<ResponseData1.size();k++)
                    {
 #if DEBUG
                        qDebug()<<"ledgerBalance:"<<Convertstr(ResponseData1[k]["ledgerBalance"].asString());
                        qDebug()<<"availableBalance:"<<Convertstr(ResponseData1[k]["availableBalance"].asString());
                        qDebug()<<"currency:"<<Convertstr(ResponseData1[k]["currency"].asString());
#endif
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
#if DEBUG
                            qDebug()<<"code:"<<Convertstr(ResponseData2[k]["code"].asString());
                            qDebug()<<"amount:"<<QString::number(ResponseData2[k]["amount"].asFloat());
                            qDebug()<<"description:"<<Convertstr(ResponseData2[k]["description"].asString());
                            qDebug()<<"chargeAcctNum:"<<Convertstr(ResponseData2[k]["chargeAcctNum"].asString());
 #endif
                            strcpy((char*)bill.uccode[k],Convertstr(ResponseData2[k]["code"].asString()));
                            strcpy((char*)bill.ucamount[k],Convertstr(ResponseData2[k]["amount"].asString()));
                            strcpy((char*)bill.ucdescription[k],Convertstr(ResponseData2[k]["description"].asString()));
                            strcpy((char*)bill.ucchargeAcctNum[k],Convertstr(ResponseData2[k]["chargeAcctNum"].asString()));

                        }
                    }
                    BillDeskDataResponse = ResponseData[j].get("BillDeskDataResponse",0);
                    ValidationResponse = BillDeskDataResponse.get("ValidationResponse",0);
 #if DEBUG
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
#endif
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
#if DEBUG
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
#endif

                    strcpy((char*)bill.PRFilter3,Convertstr(PaymentResponse["Filler3"].asString()));
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
        if(Flag_IMPS_Walkin == 'S' || CUST_TYPE == 'W' )
        {
            ui->stackedWidget->setCurrentIndex(12);
            return;
        }
        else
        {
            ui->stackedWidget->setCurrentIndex(15);
            return;
        }
    }
    iRet = PostBillPayment();
    uidmessage::instance()->hide_Message();
    if(iRet != FUNCTION_SUCCESS)
    {
        if(iRet == SESSION_EXPIRED_FAILED)
            ui->stackedWidget->setCurrentIndex(0);
        else
            ui->stackedWidget->setCurrentIndex(14);
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


    if(fetch_bill == 1)
    {

        if(!QString::compare(Partial_flag,"N",Qt::CaseInsensitive))
        {
            ui->Enter_amount_edit->setReadOnly(true);
        }
        else
        {
            ui->Enter_amount_edit->setReadOnly(false);

        }

        if(fetch_display_flag == 0)
        {
            ui->stackedWidget->setCurrentIndex(38);
            return;
        }
        else
        {
            ui->stackedWidget->setCurrentIndex(40);
            return;
        }
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
        if(fetch_display_flag == 0)
        {
            ui->stackedWidget->setCurrentIndex(38);
            return;
        }
        else
        {
        ui->stackedWidget->setCurrentIndex(40);
        return;
        }
    }
}


int FPB::Fetch_bill()
{

    int ret = bill_input_validation();
    if(ret !=0)
    {
        return FUNCTION_FAILED;
    }

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
    memcpy((char *)bill.ucIsEncrypt_req,ISENCEYPT,strlen(ISENCEYPT));
    bill.ucChannelID_req =CHANNEL_ID_IMPS;
    memcpy((char *)bill.ucSessionExpiryTime_req,(char *)configLoginLeg2.ucSessionExpiryTime,strlen((char *)configLoginLeg2.ucSessionExpiryTime));
    memcpy((char *)bill.BD_Authenticator1_req,ui->first_editline->text().trimmed().toAscii().data(),strlen(ui->first_editline->text().trimmed().toAscii().data()));

    if(count >=2)
    {
        if(memcmp(FIELD_TYPE[1],"109",3)==0)
        {
            QString date = ui->dateEdit_2->date().toString("dd-MM-yyyy") ;
            memcpy((char *)bill.BD_Authenticator2_req,date.trimmed().toAscii().data(),strlen(date.trimmed().toAscii().data()));

        }
        else
        {
            memcpy((char *)bill.BD_Authenticator2_req,ui->second_editline->text().trimmed().toAscii().data(),strlen(ui->second_editline->text().trimmed().toAscii().data()));
        }
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
 #if DEBUG
            printf("bill.ucBillercode%s",bill.BD_BillerId_req);
#endif
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
    memset(gSendData,0,sizeof(gSendData));
    strncpy((char*)gSendData, jsondata, strlen(jsondata));
    /****************************Encryption function*********************************/
    if(!strcmp(ISENCEYPT,"true"))
    {
        int gSendData_len =strlen((char *)gSendData);
        RequestDetachEncryptAttach((char *)gSendData,1,gSendData_len);
    }
    /***************************************************************/



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

    /****************************Decryption function*********************************/
    if(!strcmp(ISENCEYPT,"true"))
    {
        int oBuff_len = strlen(oBuff);
#if DEBUG
        qDebug()<<"Length of buffer"<<oBuff_len;
        printf("Buffer%s\n",oBuff);
#endif
        ResponseDetachEncryptAttach(oBuff,2,oBuff_len);//TEMPORARY

    }
    /*******************************************************************************/


    int gRecv_len = strlen((char *)oBuff);
    FILE *fp2 = NULL;
    fp2 = fopen("/usr/FINO/FPB/RESP/OUTPUT.TXT","w");
    if(fp2 == NULL)
    {
        return FUNCTION_FAILED;
    }
    fwrite( oBuff, 1, gRecv_len, fp2 );
    fclose(fp2);
    int iRetparse = Fetch_bill_parsing();
    if(iRetparse==FUNC_OK)
    {

          QString data = fetch_data();


          switch(fetch_display_flag)
          {
          case 1:
              ui->fetchdatadisplay->setGeometry(30,90,241,51);
              ui->fetchdatadisplay->setText("\n" + data);
              break;
          case 2:
              ui->fetchdatadisplay->setGeometry(40,90,241,61);
              ui->fetchdatadisplay->setText(data);
              break;
          case 3:
              ui->fetchdatadisplay->setGeometry(30,60,241,101);
              ui->fetchdatadisplay->setText(data);
              break;
          case 4:
              ui->fetchdatadisplay->setGeometry(30,60,241,121);
              ui->fetchdatadisplay->setText(data);
              break;
          default:
              break;
          }
          Filter.clear();
          Filter = QString::fromUtf8(bill.Filter2);
          Partial_flag.clear();
          Partial_flag = QString::fromUtf8(bill.PartialPayment);

//        QString bill_amount = QString::fromUtf8((const char*)bill.BillAmount);
//        ui->Bill_amount_label->setText(bill_amount);
//        QString bill_date = QString::fromUtf8((const char*)bill.BillDate);
//        ui->Bill_date_label->setText(bill_date);
//        QString bill_duedate = QString::fromUtf8((const char*)bill.BillDueDate);
//        ui->Due_date_label->setText(bill_duedate);
          ui->Fetch_bil_button->setStyleSheet("background-color: rgb(85, 85, 127); color: rgb(255, 255, 255)");
        fetch_bill =1;
        uidmessage::instance()->hide_Message();
        return FUNCTION_SUCCESS;

    }
    else
    {
        if(strlen((char *)bill.ucResponseMswssage)<=0)
        {
            QString OTPBuff = QString::fromLocal8Bit(oBuff);//Get the ouput buffer in QString

            bool check = OTPBuff.contains("ResponseMessage");
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
            QStringList ResponseMessage = query.filter("ResponseMessage");
            QString str ,strToFind,value;
            str = ResponseMessage.at(0);
            strToFind = ":";
            value = str.mid(str.indexOf(strToFind)+strToFind.length());

            strcpy((char *)bill.ucResponseMswssage,value.toAscii().data());

        }

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
#if DEBUG
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
#endif
                strcpy((char*)bill.BillNumber,Convertstr(ResponseData["BillNumber"].asString()));
                strcpy((char*)bill.BillDate,Convertstr(ResponseData["BillDate"].asString()));
                strcpy((char*)bill.BillDueDate,Convertstr(ResponseData["BillDueDate"].asString()));
                strcpy((char*)bill.BillAmount,Convertstr(ResponseData["BillAmount"].asString()));
                strcpy((char*)bill.PayWithOutBill,Convertstr(ResponseData["PayWithOutBill"].asString()));
                strcpy((char*)bill.PartialPayment,Convertstr(ResponseData["PartialPayment"].asString()));
                strcpy((char*)bill.Filter2,Convertstr(ResponseData["Filler2"].asString()));



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

    //int kkkk = 102920;

    QString Bitmap =(char *) bill.Req_requestbitmap_req;
    mainreq["RequestId"] =  (char *)bill.ucRequestId_req;
    mainreq["MethodId"] = bill.ucMethodId;
    mainreq["SessionId"] =(char *) bill.ucSessionId_req;
    mainreq["TellerID"]= username;
    mainreq["TokenId"]= "";
    mainreq["X_Auth_Token"] ="";
    mainreq["ChannelID"]= bill.ucChannelID_req;
    mainreq["IsEncrypt"]= ISENCEYPT;
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
    RequestedData["CustomerAuth"]["OTP"] = "";
    RequestedData["CustomerAuth"]["FP_Auth"] = "";

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
        RequestedData["AddWalkinCustomer"]["gender"] =""; //"FINOTLR";
        RequestedData["AddWalkinCustomer"]["mobileNumber"] =(char *)bill.AB_mobileno_req; //username;//"100020442";
        RequestedData["AddWalkinCustomer"]["address"] ="";
        RequestedData["AddWalkinCustomer"]["idProofType"] ="";
        RequestedData["AddWalkinCustomer"]["idProofNumber"] =""; //CustDet.cAccountNumber;//"96786787687";
        RequestedData["AddWalkinCustomer"]["AddressProofType"] ="";
        RequestedData["AddWalkinCustomer"]["addressProofNumber"] = "";// CustDet.cBenificiaryName;//"webtset";
        RequestedData["AddWalkinCustomer"]["name"] = "";
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
    BillDeskData["BankMessage"] = (char *)bill.BD_BankMessage;
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
#if DEBUG
    printf("gSendData = %s\n\n",gSendData);
#endif
    return 1;
}

void FPB::on_second_editline_textEdited()
{
    ui->Enter_amount_edit->setReadOnly(false);
    ui->Total_amount_display->setText("       0.00");
    ui->Enter_amount_edit->clear();
    ui->fetchdatadisplay->clear();
    ui->Calculate_charges_button->setText("Calculate Charges");
    ui->Calculate_charges_button->setEnabled(true);
    ui->Calculate_charges_button->setStyleSheet("background-color: rgb(85, 0, 127); color: rgb(255, 255, 255)");
    ui->Fetch_bil_button->setStyleSheet("background-color: rgb(85, 0, 127); color: rgb(255, 255, 255)");


    fetch_bill = 0;
    Calculate_charges =0;
    fetch_display_flag = 0;


    return;
}


void FPB::on_first_editline_textEdited()
{
    ui->Enter_amount_edit->setReadOnly(false);
    ui->Total_amount_display->setText("       0.00");
    ui->Enter_amount_edit->clear();
    ui->fetchdatadisplay->clear();
    fetch_display_flag = 0;
    ui->Calculate_charges_button->setText("Calculate Charges");
    ui->Calculate_charges_button->setEnabled(true);
    ui->Calculate_charges_button->setStyleSheet("background-color: rgb(85, 0, 127); color: rgb(255, 255, 255)");
    ui->Fetch_bil_button->setStyleSheet("background-color: rgb(85, 0, 127); color: rgb(255, 255, 255)");

    fetch_bill = 0;
    Calculate_charges =0;
    return;
}



void FPB::on_Confirmation_back_button_clicked()
{
    CMisc miscObj;

    if(display_flag == 1)
    {
        if(Billpayment_Print()== FUNC_OK)
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
        if(Flag_IMPS_Walkin == 'S' || CUST_TYPE == 'W' )
        {
            ui->stackedWidget->setCurrentIndex(12);
            return;
        }
        else
        {
            ui->stackedWidget->setCurrentIndex(15);
            return;
        }
    }
    else
    {
        ui->stackedWidget->setCurrentIndex(38);
        return ;
    }
    return;
}

void FPB::on_pushButton_Ok_clicked()
{
    if(Calculate_charges == 1)
    {
        QString Amt;
        Amt.clear();
        Amt = ui->Enter_amount_edit->text();
        ui->Enter_amount_edit->setText(Amt);
    }
    else
    {
        ui->Enter_amount_edit->setText(bill_amount);
    }

    if(!QString::compare(Partial_flag,"N",Qt::CaseInsensitive))
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


int FPB::bill_input_validation()
{
    CMisc miscObj;
    char msgdisplay[50];
    if(count>=1)
    {

        if (ui->first_editline->text().isEmpty())
        {


            sprintf(msgdisplay,"%s cannot be Blank",FIELD_NAME[0]);
            miscObj.DisplayMsgBox(msgdisplay);
            return FUNCTION_FAILED;
        }
 #if DEBUG
        printf("FIELD_LENGTH%d\n",FIELD_MIN_LENGTH[0]);
#endif
        if (ui->first_editline->text().length()<(FIELD_MIN_LENGTH[0]))
        {

            sprintf(msgdisplay,"%s should be greater than %d digits",FIELD_NAME[0],FIELD_MIN_LENGTH[0]);
            miscObj.DisplayMsgBox(msgdisplay);
            return FUNCTION_FAILED;
        }

        QString name = QString::fromLatin1(FIELD_NAME[0]);
        if(name.contains("Mobile",Qt::CaseInsensitive))
        {
            if(ui->first_editline->text().mid(0,1)!= "9" && ui->first_editline->text().mid(0,1) != "8" && ui->first_editline->text().mid(0,1)!= "7" && ui->first_editline->text().mid(0,1)!= "6")
            {
                miscObj.DisplayMsgBox((char *)"Mobile number should start from 9,8,7 or 6");
                return FUNCTION_FAILED;
            }
        }

    }

    if(count>=2)
    {
        if(memcmp(FIELD_TYPE[1],"109",3)!=0)
        {
            if (ui->second_editline->text().isEmpty())
            {
                sprintf(msgdisplay,"%s cannot be Blank",FIELD_NAME[1]);
                miscObj.DisplayMsgBox(msgdisplay);
                return FUNCTION_FAILED;
            }
#if DEBUG
            printf("FIELD_LENGTH222222222222%d\n",FIELD_MIN_LENGTH[1]);
#endif
            if (ui->second_editline->text().length()<(FIELD_MIN_LENGTH[1]))
            {
                sprintf(msgdisplay,"%s should be greater than %d digits",FIELD_NAME[1],FIELD_MIN_LENGTH[1]);
                miscObj.DisplayMsgBox(msgdisplay);
                return FUNCTION_FAILED;
            }
        }

        QString name = QString::fromLatin1(FIELD_NAME[1]);
        if(name.contains("Mobile",Qt::CaseInsensitive))
        {
            if(ui->second_editline->text().mid(0,1)!= "9" && ui->second_editline->text().mid(0,1) != "8" && ui->second_editline->text().mid(0,1)!= "7" && ui->second_editline->text().mid(0,1)!= "6")
            {
                miscObj.DisplayMsgBox((char *)"Mobile number should start from 9,8,7 or 6");
                return FUNCTION_FAILED;
            }
        }

    }
    if(count>=3)
    {

        if (ui->third_editline->text().isEmpty())
        {
            sprintf(msgdisplay,"%s cannot be Blank",FIELD_NAME[2]);
            miscObj.DisplayMsgBox(msgdisplay);
            return FUNCTION_FAILED;
        }
        if (ui->third_editline->text().length()<(FIELD_MIN_LENGTH[2]))
        {
            sprintf(msgdisplay,"%s should be greater than %d digits",FIELD_NAME[2],FIELD_MIN_LENGTH[2]);
            miscObj.DisplayMsgBox(msgdisplay);
            return FUNCTION_FAILED;
        }

        QString name = QString::fromLatin1(FIELD_NAME[2]);
        if(name.contains("Mobile",Qt::CaseInsensitive))
        {
            if(ui->third_editline->text().mid(0,1)!= "9" && ui->third_editline->text().mid(0,1) != "8" && ui->third_editline->text().mid(0,1)!= "7" && ui->third_editline->text().mid(0,1)!= "6")
            {
                miscObj.DisplayMsgBox((char *)"Mobile number should start from 9,8,7 or 6");
                return FUNCTION_FAILED;
            }
        }
    }

    return FUNCTION_SUCCESS;
}


QString FPB::fetch_data()
{
    QString Display;
    double Amount,roundoff;
    char *ptr;
    Display.clear();
    int i =0 ;
    if(!strcmp(bill.BillNumber,"NA")==0)
    {
        Display = "Biller Number :" +QString::fromUtf8(bill.BillNumber)+ "\n\n";
        i = i +1;
 #if DEBUG
        qDebug()<<"BillNumber";
#endif
    }

    if(!strcmp(bill.BillDate,"NA")==0)
    {
        Display = Display + "Bill Date     :"  +QString::fromUtf8(bill.BillDate)+ "\n\n";
         i = i +1;
#if DEBUG
         qDebug()<<"BillDate";
#endif
    }

    if(!strcmp(bill.BillAmount,"NA")==0)
    {
        Display = Display + "Bill Amount   :" +QString::fromUtf8(bill.BillAmount)+ "\n\n";
         i = i +1;
#if DEBUG
         qDebug()<<"BillAmount";
#endif

    }

    if(!strcmp(bill.BillDueDate,"NA")==0)
    {
        Display = Display + "Due Date      :"+QString::fromUtf8(bill.BillDueDate);
        i = i +1;
#if DEBUG
        qDebug()<<"BillDueDate";
#endif
    }
#if DEBUG
    qDebug()<<Display;
#endif
     fetch_display_flag = i;
     bill_amount.clear();
     Amount = strtod(bill.BillAmount, &ptr);
     roundoff = ceil(Amount);
     bill_amount= QString::number(roundoff);

    return Display;


}

QString FPB::Bill_label(QString Label)
{
    QString temp;

    if(Label.length()>16)
    {
        if(Label.contains(" OR ",Qt::CaseInsensitive) || Label.contains(" AND ",Qt::CaseInsensitive))
        {
            temp.clear();
            temp = Label.replace(" OR ","/",Qt::CaseInsensitive);
            temp = Label.replace(" AND ","/",Qt::CaseInsensitive);

            Label.clear();
            Label =temp;
        }
        if(Label.contains("Number",Qt::CaseInsensitive))
        {
            temp.clear();
            temp = Label.replace("Number","No",Qt::CaseInsensitive);


            QString str,str1,temp1;

            if(temp.length()>16)
            {
                temp1.clear();
                temp1 =temp;
                str.clear();
                if(temp.count(" ")==2)
                {
                    int index = temp.lastIndexOf(" ");
                    int first = temp.indexOf(" ");


                    int len = index - first;
                    str = temp.trimmed().mid(temp.indexOf(" "),len);
#if DEBUG
                    qDebug()<<"index"<<index;
                    qDebug()<<"first"<<first;
                    qDebug()<<"Str ---"<<str;
#endif
                    if(str.length()<5)
                    {
                        str.clear();
                        str = temp.trimmed().mid(0,temp.indexOf(" ")-1);
#if DEBUG

                        qDebug()<<"Str ---"<<str;
 #endif
                        if(str.contains("Account"))
                        str1 = str.mid(0,3).append(".");
                        else
                        str1 = str.mid(0,4).append(".");


                    }
                    else
                    {
                        if(str.contains("Account"))
                        str1 = str.mid(0,4).append(".");
                        else
                        str1 = str.mid(0,5).append(".");                    }
                        temp.clear();
#if DEBUG
                        qDebug()<<"temp1 ---"<<temp1;
#endif

                        temp = temp1.replace(str,str1);
                }
                else
                {
                    str.clear();
                    int lastindex = temp.lastIndexOf(" ");
                    str =  temp.trimmed().mid(temp.lastIndexOf(" ",lastindex-2),temp.lastIndexOf(" ",lastindex -1));
                    str1 = str.mid(0,5).append(".");
                    temp.clear();
                    temp = temp1.replace(str,str1);
                }
            }



            return temp;
        }
        else
        {
            temp.clear();
            temp = Label.mid(0,15).append(".");
            Label.clear();
            Label = temp;

            return temp;
        }


    }
    else if(Label.length()<10)
    {
        temp.clear();
        temp = temp.append("    ");
        temp = temp.append(Label);
        Label.clear();
        Label = temp;

        return temp;
    }
    temp.clear();
    qDebug()<<"label = "<<Label;
    temp = Label;
    qDebug()<<"temp = "<<temp;

    return temp;

}

